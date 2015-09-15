var cron = require('cron'),
    http = require('http'),
    moment = require('moment'),
    SsdpClient = require('node-ssdp').Client, 
    ssdpClient = new SsdpClient({
      //logLevel: 'TRACE',
      unicastHost: '192.168.1.146'
    });

var xboxLastSeen = moment();
var xboxOn = false;

// run every x seconds
var cronJob = cron.job("*/20 * * * * *", function() {
  ssdpClient.search('ssdp:all');
});

ssdpClient.on('response', function (headers, statusCode, rinfo) {
  var secondsNotSeen = moment().diff(xboxLastSeen) / 1000;
  var h = "192.168.1.45";
  
  var hour = new Date().getHours();
  //console.log(headers);
  //console.log(hour);
  if (headers.USN.indexOf("uuid:58dc09ed-a5e8-46d3-843e-f37c96844a1c") > -1) {
	  if (!xboxOn && hour >= 19) {
      console.log("XBox on");
      http.get({
        host: h,
        path: '/?r=0&g=255&b=0'}).on("error", function () { console.log("GET request error"); });
      xboxOn = true;
    }
    xboxLastSeen = moment();
  } else {
    if (secondsNotSeen >= 30 && xboxOn) {
      console.log("XBox off");
      http.get({
        host: h,
        path: '/?r=0&g=0&b=0'}).on("error", function () { console.log("GET request error"); });
      xboxOn = false;
    }
  }
});

module.exports = {
  init: function () {
    cronJob.start();
  }
};