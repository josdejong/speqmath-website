// Javascript evaluator for the SpeQ Online Calculator
// copyright © 2006-2010 SpeQ Mathematics, Jos de Jong 
// www.speqmath.com

var history = new Array();
var historypos = 0;

// TODO: Firefox does not initialize these variables
var CtrlDown = false;
var angles   = "rad";
var decimals = "float";
var ans = 0.0;
var definitions = new Array();  // all defined variable and function names


// clear all function and variable definitions
function clear()
{
  // first time initialization
  if (!definitions) 
    definitions = new Array();
  
  for (i = 0; i < definitions.length; i++)
  {
    writeLog("clear " + definitions[i]);
    eval("delete " +  definitions[i] + ";");
  }
  definitions = new Array();
}


function evaluate(expr, ansDiv) 
{
  ans = 0.0;

  try
  {
    // evaluate the expression
    expr = replacefactorial(expr);  // replace "5!" with "factorial(5)"
    expr = replacepower(expr);      // replace "2^3" with "pow(2,3)"

    writeLog("expr={" + expr + "}");
    
    if(variable = isVariableDef(expr))
    {
      definitions[definitions.length] = variable;
      
      ans = eval(expr.toLowerCase()); 
      if (typeof(ans) == "number")
      {
        ans = round(ans, 14);   // round up to a limited number of digits
      }
      
      ansDiv.className = "ans";
      ansDiv.innerHTML = variable + " = " + ans;  // TODO: more informative description
    }
    else if (func = isFunctionDef(expr))
    {
      definitions[definitions.length] = func;
      var funcdesc = trim(expr.substring(0, expr.indexOf("=")));

      expr = replacefunction(expr);   // replace "f(x)=x^2" with "f=function(x) {return x^2;}"
      
      ans = eval(expr.toLowerCase());

      ansDiv.className = "ans";
      ansDiv.innerHTML = funcdesc + " defined";  // TODO: more informative description
    }
    else if(isPlot(expr))
    {
      plot(expr, ansDiv);

      ansDiv.className = "ans";
    }
    else
    {
      // TODO: get variable ans working
      ans = eval(expr.toLowerCase()); 
      if (typeof(ans) == "number")
      {
        ans = round(ans, 14); // round up to a limited number of digits
      }  
      else if (typeof(ans) == "string")
      {
        ans = "\"" + ans + "\"";  // enclose string with quotes
      }

      ansDiv.className = "ans";
      ansDiv.innerHTML = ans;
    }

    // TODO: enable using history with alt+arrowup or alt+arrowdown
    history[history.length] = expr;
    historypos = history.length;
  }
  catch(error)
  {
    ansDiv.innerHTML = error;
    ansDiv.className = "anserror";  // errors will be displayed in red
  }
}

/// replace "f(x)=x^2" with "f=function(x) {return x^2;}"
function replacefunction(expr)
{
  var equals = expr.indexOf('=');
  if (equals > 0 && 
      expr[equals+1] != '=' &&  // check if not dealing with operator ==
      expr[equals-1] != '!')    // check if not dealing with operator !=
  {
    // variable or function definition found
    var parenthesis = expr.indexOf('(');
    if (parenthesis != -1 && parenthesis < equals)
    {
      // this is a function definition
      var name = expr.substring(0, parenthesis);
      var params = expr.substring(parenthesis, equals);
      var func = expr.substring(equals+1);
      
      expr = name + "=function" + 
             params + " {return " + func + ";}";
    }
  }
  return expr;
}

// returns the variable name when expr is a variable definition 
// (for example "a=4.5/2"), else returns false. 
function isVariableDef(expr)
{
  var equals = expr.indexOf('=');
  var par = expr.indexOf('(');
  if (equals > 0 && 
      (par < 0 || par > equals) &&
      expr[equals+1] != '=' &&  // check if not dealing with operator ==
      expr[equals-1] != '!')    // check if not dealing with operator !=
  {
    return expr.substring(0, equals);
  }
  return false;
}

// returns the variable name when expr is a function definition 
// (for example "f(x)=x^2-4"), else returns false. 
function isFunctionDef(expr)
{
  var equals = expr.indexOf('=');
  var par = expr.indexOf('(');
  if (equals > 0 && 
      par > 0 && par < equals &&
      expr[equals+1] != '=' &&  // check if not dealing with operator ==
      expr[equals-1] != '!')    // check if not dealing with operator !=
  {
    return trim(expr.substring(0, par));
  }
  return false;
}


/*
 * Plot given expression
 * @param expr  A string like "plot(sin(x), x)"
 * @param ansDiv 
 */ 
function plot(expr, ansDiv)
{  
  var comma = expr.lastIndexOf(',');
  var parOpen = expr.indexOf('(');
  var parClose = expr.lastIndexOf(')');
  
  var func = trim(expr.substring(parOpen+1, comma));
  var variable = trim(expr.substring(comma+1, parClose));

  var graph = new Graph(func, variable, ansDiv);
}

function isPlot(expr)
{
  if (expr.toLowerCase().substring(0, 4) == "plot")
  {
    return true;
  }
  return false;
}


/// replace "value!" with "factorial(value)"
function replacefactorial(expr) 
{
  var f = -1;
  do
  {
		f      = expr.indexOf("!", f+1);
    equals = expr.indexOf("=", f);
		if (f > 0 && equals != f+1) 
    {
			//find the value before the !
			i=f; startpos=-1;
			bopen=0; bclose=0;	//brackets open and brackets close
			do 
      {
				i=i-1;
				if (i>=0) 
        {
					sign = expr.substr(i,1);
					if (("+-*/%^!(=, ".indexOf(sign)>=0) & (bopen==bclose)) {startpos=i+1;}
					if (sign=="(") {bopen=bopen+1;}
					if (sign==")") {bclose=bclose+1;}
				} 
        else 
        {
					startpos=0;
				}
			} 
      while (startpos==-1);
			value = expr.substr(startpos, f-startpos);

			// replace "value!" with "factorial(value)"
			expr = expr.substr(0,startpos) + "factorial(" + value + ")" + expr.substr(f+1, expr.length-f-1);
		}
	}
	while(f > 0);

	return expr;
}

/// replace all "value1^value2" with "pow(value1,value2)"
function replacepower(expr) 
{

	while(expr.indexOf("^")>0) 
  {
		f=expr.indexOf("^");
		if (f>0) 
    {
			//find the value before the ^
			i=f; startpos=-1;
			bopen=0; bclose=0;	//brackets open and brackets close
			do 
      {
				i=i-1;
				if (i>=0) 
        {
					sign = expr.substr(i,1);
					if (("+-*/%^!(=, ".indexOf(sign)>=0) & (bopen==bclose)) {startpos=i+1;}
					if (sign=="(") {bopen=bopen+1;}
					if (sign==")") {bclose=bclose+1;}
				}
        else 
        {
					startpos=0;
				}
			} 
      while (startpos==-1);
			value1=expr.substr(startpos, f-startpos);
	
			// find the value after the ^
			i=f; endpos=-1;
			bopen=0; bclose=0;	//brackets open and brackets close
			do 
      {
				i=i+1;
				if (i<expr.length) 
        {
					sign = expr.substr(i,1);
					if (("+-*/%^!)=, ".indexOf(sign)>=0) & (bopen==bclose)) {endpos=i-1;}
					if ((sign=="-") & (i==(f+1))) {endpos=-1;}	// for negative powers
					if (sign=="(") {bopen=bopen+1;}
					if (sign==")") {bclose=bclose+1;}
				} 
        else 
        {
					endpos=expr.length-1;
				}
			} 
      while (endpos==-1);
			value2 = expr.substr(f+1, endpos-f);

			// replace "value1^value2" with "pow(value1,value2)"
			expr = expr.substr(0,startpos) + "pow(" + value1 + "," + value2 + ")" + expr.substr(endpos+1, expr.length-endpos-1);
		}
	}
  return expr;
}

function angleFactor() 
{
  if (angles=="rad") {return 1;}
  if (angles=="deg") {return Math.PI/180;}    // 2*pi / 360
  if (angles=="grad") {return Math.PI/200;}   // 2*pi / 400
  return 1;
}

function setAngles(ang) 
{
  // set value for angles. can be "rad", "deg" or "grad"
  angles = ang;
}

function setDecimals(dec) 
{
  // set value for decimals. can be "1", "2", "3", "4", "5", "float"
  decimals = dec;
}
