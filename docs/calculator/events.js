// JavaScript Document
// events.js

var CtrlDown = false;

function body_onload() {
  starttext=win.innerHTML;
  inputline.focus();
  inputline.select();
}

function body_keydown(e) {
  if(e.which==17 | e.keyCode==17) {   //ctrl key
    CtrlDown=true;
  } else {
    if (CtrlDown==false) {
      inputline.focus();
    }
  }
}

function body_keyup(e) {
  if(e.which==17 | e.keyCode==17) {   //ctrl key
    CtrlDown=false;
  }
}

function keydown(e) {
  //alert(e.which);
  //var keynum=0;
  //if (e.keyCode) {keynum=e.keyCode} else if (e.which) {keynum=e.which}
 
  if(e.which==13 | e.keyCode==13) {
    // process the entered expression
    evaluate_line();
    return !1;
  }
  
  if (e.which==38 | e.keyCode==38) {
    // arrow up. insert the previous entered expression in the inputbox
    if (historypos>0) {
      historypos--;
      inputline.value = history[historypos];
      inputline.focus();
      inputline.select();
    }
    return !1;    // block the keydown event
  }
  if (e.which==40 | e.keyCode==40) {
    // arrow down. insert the next entered expression in the inputbox
    if (historypos<history.length-1) {
      historypos++;
      inputline.value = history[historypos];
      inputline.focus();
      inputline.select();
    } 
    return !1;    // block the keydown event
  }
  return 1;       // do not block the keydown event
}

function showHelp() {
  window.open("calculator/help.html", "_blank", "toolbar=0, scrollbars=1, location=0, statusbar=0, menubar=0, resizable=1, width=650, height=560, left=100, top=100");
}

function resetCalculator() {
  //ang.value="rad";
  //inputline.value="2*(3+4)";
  //inputline.focus();
  window.location.reload();
}
