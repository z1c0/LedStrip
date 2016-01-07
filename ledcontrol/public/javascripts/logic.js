function switchColor(r, g, b) {
  var query = "r=" + r + "&g=" + g + "&b=" + b;
  //console.log(query);
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
      var html = "<a href='#' onclick='switchColor(" + r + ", " + g + ", " + b + "); return false;'><div class='tile' style='background-color: rgb(" + r + "," + g + "," + b + ")'></div></a>";
      $("#container").append(html);
   });
}

function sliderChange() {
  var r = $("#sliderR").val();
  var g = $("#sliderG").val();
  var b = $("#sliderB").val();
  switchColor(r, g, b);
}