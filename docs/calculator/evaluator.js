// Javascript evaluator for the SpeQ Online Calculator
// copyright © 2006 SpeQ Mathematics, Jos de Jong 
// www.speqmath.com

var history = new Array();
var historypos = 0;
var starttext;
var CtrlDown = false;
var angles   = "rad";
var decimals = "float";


function evaluate_line() {
  // evaluate the entered expression
  var ansline="";
  var expr;

  expr_line = inputline.value;
  if ("+*/^!>".indexOf(expr_line.substr(0,1))>=0) {expr_line = "Ans" + expr_line;} // replace expressions like "+3" with "Ans+3"
  expr = expr_line;

  if (evaluate_linecommands(expr)==false) {
    try {
      ansline = evaluate(expr)
      inputline.value = ansline;
      ansline = "&nbsp;&nbsp;&nbsp;<font color=blue>Ans = " + ansline + "</font>";
    } catch(err) {
      // an error occured. give an error message as result
      if (err.description.indexOf(";")>=0) {
        ansline = "&nbsp;&nbsp;&nbsp;<font color=red>Error</font>";
      } else {
        ansline = "&nbsp;&nbsp;&nbsp;<font color=red>Error: " + err.description + "</font>";
      }
    }
    win.innerHTML=win.innerHTML + expr_line + "<br>" + ansline + "<BR><BR>";
    //document.focus();    
    window.open("#bottom", "_self", 0, true);   // to move the focus to the bottom of the page
    inputline.focus();
    inputline.select();
  }
}

function evaluate(expr) {
  // evaluate the expression
  
  expr = replacefactorial(expr);  // replace "5!" with "factorial(5)"
  expr = replacepower(expr);      // replace "2^3" with "pow(2,3)"
  expr = replaceFraction(expr);   // replace "3/4 + 5/7 >frac" with "fraction(3/4 + 5/7)"

  ansline = eval("("+expr.toUpperCase()+")");
  eval("ANS="+ansline);

  history[history.length] = expr_line;
  historypos = history.length;
  //inputline.value = ans;
  return ansline;
}

function evaluate_linecommands(expr) {
  var ansline = ""
  
  switch (expr.toUpperCase()) {
    case "CLC":
      // clear screen
      history.Length = 0;
      historypos=0;
      win.innerHTML = starttext
      inputline.value = "";
      inputline.focus();
      inputline.select();
      return true
      break
    /*case "MEM":
      // show a list with all variables in memory on screen
      ansline = "&nbsp;&nbsp;&nbsp;<font color=blue>Variables in memory:</font>"
      break*/
    case "HELP":
      // open a window with help
      showHelp()
      ansline = "&nbsp;&nbsp;&nbsp;<font color=blue>Help opened.</font>"
      break
    default:
      return false;
  }
  win.innerHTML=win.innerHTML + expr + "<br>" + ansline + "<BR><BR>";
  inputline.focus();
  inputline.select();
  return true
}

function replacefactorial(expr) {
	// replace "value!" with "factorial(value)"

	while(expr.indexOf("!")>0) {
		f=expr.indexOf("!");
		if (f>0) {
			//find the value before the !
			i=f; startpos=-1;
			bopen=0; bclose=0;	//brackets open and brackets close
			do {
				i=i-1;
				if (i>=0) {
					sign = expr.substr(i,1);
					if (("+-*/%^(".indexOf(sign)>=0) & (bopen==bclose)) {startpos=i+1;}
					if (sign=="(") {bopen=bopen+1;}
					if (sign==")") {bclose=bclose+1;}
				} else {
					startpos=0;
				}
			} while (startpos==-1);
			value = expr.substr(startpos, f-startpos);

			// replace "value!" with "factorial(value)"
			expr = expr.substr(0,startpos) + "factorial(" + value + ")" + expr.substr(f+1, expr.length-f-1);
		}
	}

return expr;
}

function replacepower(expr) {
	// replace all "value1^value2" with "pow(value1,value2)"

	while(expr.indexOf("^")>0) {
		f=expr.indexOf("^");
		if (f>0) {
			//find the value before the ^
			i=f; startpos=-1;
			bopen=0; bclose=0;	//brackets open and brackets close
			do {
				i=i-1;
				if (i>=0) {
					sign = expr.substr(i,1);
					if (("+-*/%^!(".indexOf(sign)>=0) & (bopen==bclose)) {startpos=i+1;}
					if (sign=="(") {bopen=bopen+1;}
					if (sign==")") {bclose=bclose+1;}
				} else {
					startpos=0;
				}
			} while (startpos==-1);
			value1=expr.substr(startpos, f-startpos);
	
			// find the value after the ^
			i=f; endpos=-1;
			bopen=0; bclose=0;	//brackets open and brackets close
			do {
				i=i+1;
				if (i<expr.length) {
					sign = expr.substr(i,1);
					if (("+-*/%^!)".indexOf(sign)>=0) & (bopen==bclose)) {endpos=i-1;}
					if ((sign=="-") & (i==(f+1))) {endpos=-1;}	// for negative powers
					if (sign=="(") {bopen=bopen+1;}
					if (sign==")") {bclose=bclose+1;}
				} else {
					endpos=expr.length-1;
				}
			} while (endpos==-1);
			value2 = expr.substr(f+1, endpos-f);

			// replace "value1^value2" with "pow(value1,value2)"
			expr = expr.substr(0,startpos) + "pow(" + value1 + "," + value2 + ")" + expr.substr(endpos+1, expr.length-endpos-1);
		}
	}
  return expr;
}

function replaceFraction(expr) {
  if (expr.toUpperCase().indexOf(">FRAC")>0) {
    if (expr.toUpperCase().indexOf(">FRAC") == expr.length-5) {
      expr = "fraction(" + expr.substr(0, expr.toUpperCase().indexOf(">FRAC")) + ")";
    }
  }
  return expr;
}

function angleFactor() {
  if (angles=="rad") {return 1;}
  if (angles=="deg") {return Math.PI/180;}    // 2*pi / 360
  if (angles=="grad") {return Math.PI/200;}   // 2*pi / 400
  return 1;
}

function setAngles(ang) {
  // set value for angles. can be "rad", "deg" or "grad"
  angles = ang;
}

function setDecimals(dec) {
  // set value for decimals. can be "1", "2", "3", "4", "5", "float"
  decimals = dec;
}
