var cron = require('cron'),
    http = require('http'),
    moment = require('moment'),
    SsdpClient = require('node-ssdp').Client, 
    ssdpClient = new SsdpClient({
      //logLevel: 'TRACE',
      unicastHost: '192.168.1.118' // raspi
      //unicastHost: '192.168.1.146' // my pc      
    });

var xboxLastSeen = moment();
var xboxOn = false;
var currentHue = Math.random();
var targetHue = Math.random();

// run every x seconds
var cronJob = cron.job("*/20 * * * * *", function() {
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
  var step = 0.005;
  var delta = 0.01;
  var r = 0,
      g = 0,
      b = 0;
  if (animate) {
    if (Math.abs(currentHue - targetHue) < delta) {
      targetHue = Math.random();
    }
    if (currentHue > targetHue) {
      currentHue -= step;
    } else {
      currentHue += step;
    }
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
  var secondsNotSeen = moment().diff(xboxLastSeen) / 1000;
  if (headers.USN.indexOf("uuid:58dc09ed-a5e8-46d3-843e-f37c96844a1c") > -1) {
    updateLeds(true);
    xboxOn = true;
    xboxLastSeen = moment();
  } else {
    if (secondsNotSeen >= 30 && xboxOn) {
      updateLeds(false);
      xboxOn = false;
    }
  }
});

module.exports = {
  init: function () {
    cronJob.start();
  }
};