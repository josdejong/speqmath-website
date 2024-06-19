// functions.js

// define mathematical constants
var pi = Math.PI;
var e  = Math.E;

// define mathematical functions
pow  = Math.pow;
exp  = Math.exp;
function epower(x, power) {return x * Math.pow(10,power)}   //returns x * 10^power
sqrt = Math.sqrt;
ln   = Math.log;
log  = Math.log;
function log10(x) {return Math.log(x) * Math.LOG10E;}
abs = Math.abs;
ceil = Math.ceil;
floor = Math.floor;
max = Math.max;
min = Math.min;

function rand() {return Math.random();}

function round(x, n) 
{
  if (n === undefined) 
  {
    return Math.round(x);
  }
  else
  {
    var f = Math.pow(10,n); 
    return Math.round(x * f) / f;
  }
}

function sin(x) {return Math.sin(x*angleFactor());}
function cos(x) {return Math.cos(x*angleFactor());}
function tan(x) {return Math.tan(x*angleFactor());}
function asin(x) {return Math.asin(x)/angleFactor();}
function acos(x) {return Math.acos(x)/angleFactor();}
function atan(x) {return Math.atan(x)/angleFactor();}
function atan2(y,x) {return Math.atan2(y,x)/angleFactor();}

function csc(x) {return 1/Math.sin(x);}
function sec(x) {return 1/Math.cos(x);}
function cot(x) {return 1/Math.tan(x);}

function sinh(x) {return (Math.exp(x) - Math.exp(-x)) / 2;}
function cosh(x) {return (Math.exp(x) + Math.exp(-x)) / 2;}
function tanh(x) {return sinh(x) / cosh(x);}

function csch(x) {return 1/sinh(x);}
function sech(x) {return 1/cosh(x);}
function coth(x) {return 1/tanh(x);}


function factorial(value) {
// calculate the factorial of value
// this sub returns and error if value<0 or is no number
// for example factorial(5) = 5! = 1*2*3*4*5 = 120

  //if (value<0) {return "Error: Can not calculate Factorial for negative values";}
  //if (Math.round(value)!=value) {return "Error: Can not calculate Factorial for decimal values";}
  if (value<0) {return value + "!";}
  if (Math.round(value)!=value) {return value + "!";}
  
  fact = 1;
  for (i=1; i <= value; i++) {fact *= i;}
  return fact;
}


function fraction(value) 
{
  // calculate the fraction of value
  // for example 0.125 = "1/8"
  max = 2000;
  result = "";
  original = value;

  beginpart = "";
  if (value < 0) {
    beginpart = "-";
    value = Math.abs(value);
  }
  
  if ((value < max) & (value > 1/max)) {
    num = 1;
    denom = 1;
    result = "";
  
    do {
      valv = num / denom;
      if (valv == value) {
      //if (Math.abs(valv - value) < .000000000000001) {
        // fraction is found
        result = num + "/" + denom;
        if (denom == 1) {result = num;}	// fraction is a number
      } else {
        if (valv < value) {
          d = value * denom - num;
          if (Math.round(d)==0) {d=1;}
          d = Math.round(d);
          num = num + d;
        }
        if (valv > value) {
          d = num / value - denom;
          if (Math.round(d)==0) {d=1;}
          d = Math.round(d);
          denom = denom + d;
        }
      }
    } while ((result == "") & (num < max) & (denom < max));
    
    if (result!="") {result = beginpart + result;} else {result=value}
    return result;
  }
}
