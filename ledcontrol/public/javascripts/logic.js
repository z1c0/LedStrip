function switchColor(r, g, b) {
  var query = "r=" + r + "&g=" + g + "&b=" + b;
  $.ajax({
    type: 'GET',
    url: 'http://192.168.1.45/?' + query,
  });
}

function makeTiles() {
   var colors = [ 
     [255, 255, 255],
     [0, 0, 0],
     [255, 0, 0],
     [0, 255, 0],
     [0, 0, 255],
     [255, 255, 0],
     [255, 128, 0],
     [255, 0, 255], 
     [0, 255, 255] 
   ];
   
   colors.forEach(function(c) {	 
      var r = c[0], g = c[1], b = c[2]; 
      //var html = "<a href='#' onclick='switchColor(" + r + ", " + g + ", " + b + "); return false;'><div class='tile' style='background-color: rgb(" + r + "," + g + "," + b + ")'></div></a>";
      var html = "<div class='tile' style='background-color: rgb(" + r + "," + g + "," + b + ")'></div>";
      $("#container").append(html);
   });
}