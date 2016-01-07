var cron = require('cron'),
    http = require('http'),
    xmldoc = require('xmldoc'),
    moment = require('moment'),
    SsdpClient = require('node-ssdp').Client, 
    ssdpClient = new SsdpClient({
      //logLevel: 'TRACE',
      unicastHost: getIPAddress()
    });
    
function getIPAddress() {
  var interfaces = require('os').networkInterfaces();
  for (var dev in interfaces) {  
    var iface = interfaces[dev].filter(function(details) {
      return details.family === 'IPv4' && details.internal === false && details.address.indexOf("169.") != 0;
    });  
    if (iface.length > 0)  {
      var a = iface[0].address; 
      console.log(a);
      return a;
    }
  }
  return "0.0.0.0";
}    

var xboxLastSeen = moment([1980, 3, 10]);
var xboxOn = false;
var currentHue = Math.random();
var targetHue = Math.random();
var ssdpDone = false;
var currentColor = { r : 0, g : 0, b : 0 };
var targetColor = { r : 0, g : 0, b : 0 };
var location = "";
var deviceType = "";
var checkInterval = 10;

// run every x seconds
var cronJob = cron.job("*/" + checkInterval + " * * * * *", function() {
  ssdpDone = false;
  ssdpClient.search('ssdp:all');
});


function hsvToRgb(h, s, v) {
    var r, g, b;
    var i = Math.floor(h * 6);
    var f = h * 6 - i;
    var p = v * (1 - s);
    var q = v * (1 - f * s);
    var t = v * (1 - (1 - f) * s);

    switch (i % 6) {
        case 0:
            r = v, g = t, b = p;
            break;
        case 1:
            r = q, g = v, b = p;
            break;
        case 2:
            r = p, g = v, b = t;
            break;
        case 3:
            r = p, g = q, b = v;
            break;
        case 4:
            r = t, g = p, b = v;
            break;
        case 5:
            r = v, g = p, b = q;
            break;
    }

    r = Math.floor(r * 255);
    g = Math.floor(g * 255);
    b = Math.floor(b * 255);
    return [r, g, b];
}

function updateLeds(animate) {
  currentColor.r = 0;
  currentColor.g = 0;
  currentColor.b = 0;

  var step = 0.0050;
  var delta = 0.01;
  if (animate) {
    if (Math.abs(currentHue - targetHue) < delta) {
      targetHue = Math.random();
    }
    if (currentHue > targetHue) {
      currentHue -= step;
    } else {
      currentHue += step;
    }
    currentHue = Math.max(0, Math.min(1, currentHue)); // clamp
    var c = hsvToRgb(currentHue, 1, 1);
    currentColor.r = c[0];
    currentColor.g = c[1];
    currentColor.b = c[2];
    
    var t = hsvToRgb(targetHue, 1, 1);
    targetColor.r = t[0];
    targetColor.g = t[1];
    targetColor.b = t[2];    
  }
  var query = "/?r=" + currentColor.r + "&g=" + currentColor.g + "&b=" + currentColor.b;
  http.get({    
    host: "192.168.1.45",
    path: query }).
      on("error", function () { console.log("FAILED Arduino GET: "+ query); });
}



ssdpClient.on('response', function (headers, statusCode, rinfo) {
  //console.log(headers);
  var url = headers.LOCATION;
  if (url && url.indexOf("http") != -1) 
  {
    var req = http.get(url, function(res) {
      var xml = '';
      res.on('data', function(chunk) {
        xml += chunk;
      });  
      res.on('end', function() {
        // parse xml
        try {
            if (xml[0] == '<') { // otherwise, don't bother
            var device = new xmldoc.XmlDocument(xml).childNamed("device");
            if (device) {
                var friendlyName = device.childNamed("friendlyName");
                if (friendlyName) {      
                //console.log(friendlyName.val);
                if (friendlyName.val === "Xbox-SystemOS") {                
                    var dt = device.childNamed("deviceType");
                    if (dt) {              
                    deviceType = dt.val;
                    if (deviceType == "urn:schemas-upnp-org:device:MediaRenderer:1") {                    
                        xboxLastSeen = moment();
                        location = headers.LOCATION;
                        if (!ssdpDone) {
                        ssdpDone = true;
                        updateLeds(true);
                        xboxOn = true;
                        }
                    }
                  }
                }
              }
            }
          }        
        } catch (e) {
            //console.log("Error: " + headers.LOCATION + " - " + xml);
        }
      });  
    });  
    req.on('error', function(err) {
        console.log("Error for URL: " + headers.LOCATION + " - " + err);
    });
  }
  // Turn off?
  var secondsNotSeen = moment().diff(xboxLastSeen) / 1000;
  if (secondsNotSeen >= (checkInterval * 2.5) && xboxOn) {
    updateLeds(false);
    xboxOn = false;
  }
});

module.exports = {
  isOn: function() {
    return xboxOn;
  },
  getLastSeen: function() {
    return moment(xboxLastSeen).fromNow();
  },
  getLocation: function() {
    return location;    
  },
  getDeviceType: function() {
    return deviceType;    
  },
  getCurrentColor: function() {
    return currentColor;
  },
  getTargetColor: function() {
    return targetColor;
  },
  init: function () {
    cronJob.start();
  }
};
