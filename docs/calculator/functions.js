// functions.js

// define mathematical constants
var PI = Math.PI;
var E  = Math.E;
var TRUE = true;
var FALSE = false;

// define mathematical functions
POW  = Math.pow;
EXP  = Math.exp;
function EPOWER(x, power) {return x * Math.pow(10,power)}   //returns x * 10^power
SQRT = Math.sqrt;
LN   = Math.log;
LOG  = Math.log;
function LOG10(x) {return Math.log(x) * Math.LOG10E;}
ABS = Math.abs;
CEIL = Math.ceil;
FLOOR = Math.floor;
MAX = Math.max;
MIN = Math.min;
ROUND = Math.round;
//function RAND() {return Math.random;}

function SIN(x) {return Math.sin(x*angleFactor());}
function COS(x) {return Math.cos(x*angleFactor());}
function TAN(x) {return Math.tan(x*angleFactor());}
function ASIN(x) {return Math.asin(x)/angleFactor();}
function ACOS(x) {return Math.acos(x)/angleFactor();}
function ATAN(x) {return Math.atan(x)/angleFactor();}
function ATAN2(y,x) {return Math.atan2(y,x)/angleFactor();}


function CSC(x) {return 1/Math.sin(x);}
function SEC(x) {return 1/Math.cos(x);}
function COT(x) {return 1/Math.tan(x);}

function SINH(x) {return (Math.exp(x) - Math.exp(-x)) / 2;}
function COSH(x) {return (Math.exp(x) + Math.exp(-x)) / 2;}
function TANH(x) {return SINH(x)/COSH(x);}

function CSCH(x) {return 1/SINH(x);}
function SECH(x) {return 1/COSH(x);}
function COTH(x) {return 1/TANH(x);}


function FACTORIAL(value) {
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

function FRACTION(value) {
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
      //If (Math.Round(valv,12) == Math.Round(val,12)) {
      if (valv == value) {
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
