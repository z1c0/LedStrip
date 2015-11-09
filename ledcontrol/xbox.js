var cron = require('cron'),
    http = require('http'),
    xmldoc = require('xmldoc'),
    moment = require('moment'),
    SsdpClient = require('node-ssdp').Client, 
    ssdpClient = new SsdpClient({
      //logLevel: 'TRACE',
      unicastHost: getIPAddress()
      //unicastHost: '192.168.1.118' // raspi
      //unicastHost: '192.168.1.146' // my pc      
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

var xboxLastSeen = moment();
var xboxOn = false;
var currentHue = Math.random();
var targetHue = Math.random();
var ssdpDone = false;
var r = 0,
    g = 0,
    b = 0;

// run every x seconds
var cronJob = cron.job("*/25 * * * * *", function() {
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
  var step = 0.0025;
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
    r = c[0];
    g = c[1];
    b = c[2];
  }
  http.get({
    host: "192.168.1.45",
    path: "/?r=" + r + "&g=" + g + "&b=" + b}).
      on("error", function () { console.log("GET request error"); });
}



ssdpClient.on('response', function (headers, statusCode, rinfo) {
  //console.log(headers);
  var req = http.get(headers.LOCATION, function(res) {
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
                xboxLastSeen = moment();
                if (!ssdpDone) {
                  ssdpDone = true;
                  updateLeds(true);
                  xboxOn = true;
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
    console.log(err);
  });
  // Turn off?
  var secondsNotSeen = moment().diff(xboxLastSeen) / 1000;
  if (secondsNotSeen >= 60 && xboxOn) {
    updateLeds(false);
    xboxOn = false;
  }    
});

module.exports = {
  isXBoxOn: function() {
    return xboxOn;
  },
  r: function() {
    return r;
  },
  g: function() {
    return g;
  },
  b: function() {
    return b;
  },
  init: function () {
    cronJob.start();
  }
};
