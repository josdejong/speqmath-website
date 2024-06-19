/* 
Documentation:
https://developer.mozilla.org/en/Canvas_tutorial
http://www.nihilogic.dk/labs/canvas2image/
*/


var graphLeft = 0;
var graphRight = 400;
var graphTop = 0;
var graphBottom = 400;

var xmin = -5;
var xmax = 10;
var ymin = -2;
var ymax = 2;

var xscale = 0;
var xshift = 0;
var yscale = 0;
var yshift = 0;
var axisXstep = 0;
var axisYstep = 0; 
var xstep = 0;

var mouseX = 0;
var mouseY = 0;
var mouseDown = false;

var canvas;  
var func;
var param;

function init()
{
  canvas = document.getElementById("graph");  
  func   = document.getElementById("function").value;
  param  = document.getElementById("variable").value;

  // evaluate the function
  var expr = "f=function(" + param + ") {return " + func + ";}";
  eval(expr);

  // get the size of the graph on screen
  graphLeft = 0;
  graphRight = canvas.width;
  graphTop = 0;
  graphBottom = canvas.height;
}

function init_scale()
{
  // calculate scaling
  xscale = (graphRight - graphLeft) / (xmax - xmin);
  xshift = graphLeft - xmin * xscale;
  yscale = (graphTop - graphBottom) / (ymax - ymin);
  yshift = graphBottom - ymin * yscale;
  xstep = 1 / xscale;  // every step equals one pixel on the screen
  xstep *= 2;  // half the number of steps to make drawing faster
  
  // find a sufficient x-scale. try to find a scale as close as possible to 40 pixels
  var xPixel = 40;
  var scale1 = 1 * pow(10, ceil(log10(xPixel / abs(xshift) / 1)));
  var scale2 = 2 * pow(10, ceil(log10(xPixel / abs(xshift) / 2)));
  var scale5 = 5 * pow(10, ceil(log10(xPixel / abs(xshift) / 5)));
  axisXstep = scale1;
  if (abs(scale2 - xPixel/xshift) <= abs(xshift - xPixel/xshift)) axisXstep = scale2
  if (abs(scale5 - xPixel/xshift) <= abs(xshift - xPixel/xshift)) axisXstep = scale5
  if (axisXstep <= 0) axisXstep = 1		// for safety

  // find a sufficient y-scale. try to find a scale as close as possible to 40 pixels
  var yPixel = 40;
  var scale1 = 1 * pow(10, ceil(log10(yPixel / abs(yshift) / 1)));
  var scale2 = 2 * pow(10, ceil(log10(yPixel / abs(yshift) / 2)));
  var scale5 = 5 * pow(10, ceil(log10(yPixel / abs(yshift) / 5)));
  axisYstep = scale1;
  if (abs(scale2 - yPixel/yshift) <= abs(yshift - yPixel/yshift)) axisYstep = scale2
  if (abs(scale5 - yPixel/yshift) <= abs(yshift - yPixel/yshift)) axisYstep = scale5
  if (axisYstep <= 0) axisYstep = 1		// for safety
  
  // TODO
  axisXstep = 1;
  axisYstep = 1;
}

function draw() 
{  
  var canvas = document.getElementById("graph");  
  var ctx = canvas.getContext("2d");  

  init_scale();

  // clear current drawing
  ctx.clearRect(graphLeft, graphTop, 
              graphRight-graphLeft, graphBottom, graphTop);

  drawAxes(ctx);
  drawGraph(ctx);
}

function drawAxes(ctx)
{
  // draw axes
  ctx.lineWidth = 0.5;

  // x-axis
  ctx.strokeStyle = "black";
  ctx.beginPath();
  ctx.moveTo(xmin * xscale + xshift, 0 * yscale + yshift);  
  ctx.lineTo(xmax * xscale + xshift, 0 * yscale + yshift);  
  ctx.stroke(); 

  var x = ceil(xmin / axisXstep) * axisXstep;
  do
  {
    ctx.beginPath();
    ctx.moveTo(x * xscale + xshift, yshift);  
    ctx.lineTo(x * xscale + xshift, yshift + 5);  
    ctx.stroke();     

    if (abs(x) > 0.1 * axisXstep)
    {
      // var textWidth = ctx.measureText(x + ""); // TODO: calculate text width
      var textWidth = 6;
      var textHeight = 8;// TODO: calculate text height
      ctx.strokeText(x, x * xscale + xshift - 0.5 * textWidth, yshift + 8 + textHeight); 
    }
    
    x += axisXstep;
  }
  while (x <= xmax);

  // y-axis
  ctx.beginPath();
  ctx.moveTo(0 * xscale + xshift, ymin * yscale + yshift);  
  ctx.lineTo(0 * xscale + xshift, ymax * yscale + yshift);  
  ctx.stroke(); 

  var y = ceil(ymin / axisYstep) * axisYstep;
  do
  {
    ctx.beginPath();
    ctx.moveTo(xshift,     y * yscale + yshift);  
    ctx.lineTo(xshift - 5, y * yscale + yshift);  
    ctx.stroke();     
    
    if (abs(y) > 0.1 * axisYstep)
    {
      // var textWidth = ctx.measureText(y + ""); // TODO: calculate text width
      var textWidth = 10;
      var textHeight = 8;// TODO: calculate text height
      ctx.strokeText(y, xshift - 8 - textWidth, y * yscale + yshift + 0.5 * textHeight); 
    }
    
    y += axisYstep;
  }
  while (y <= ymax);  
}

function drawGraph(ctx) 
{
  // draw graph
  ctx.lineWidth = 1.0;
  
  ctx.strokeStyle = "red";
  ctx.beginPath();
  ctx.moveTo(xmin * xscale + xshift, f(xmin) * yscale + yshift); 
  for (x = xmin+xstep; x < xmax; x += xstep)
  {
    ctx.lineTo(x * xscale + xshift, f(x) * yscale + yshift);  
  }
  ctx.stroke(); 
}

function mousedown(evt)
{
  if (evt.which == 1)
  {
    // left mouse button pressed
    mouseX = evt.pageX;
    mouseY = evt.pageY;
    mouseDown = true;
  }
}

function mousemove(evt)
{
  if (mouseDown)
  {
    var diffX = evt.pageX - mouseX;
    var diffY = evt.pageY - mouseY;
    
    xmin -= diffX / xscale;
    xmax -= diffX / xscale;

    ymin -= diffY / yscale;
    ymax -= diffY / yscale;

    mouseX = evt.pageX;
    mouseY = evt.pageY;
        
    draw();
  }
}

function mouseup(evt)
{
  mouseDown = false;
}
