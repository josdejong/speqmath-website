// JavaScript Document

function mouseOver(id) {
  document.getElementById(id).style.color='red';
}

function mouseOut(id) {
  document.getElementById(id).style.color='black';
}

function select(id) {
// expand or collapse the content of the given div
// id is a string containing the id of the div
  if (document.selection) {
    var r = document.body.createTextRange();
    r.moveToElementText(document.getElementById(id)); r.select();
  } else {
    s = window.getSelection();
    var r = document.createRange();
    r.setStartBefore(document.getElementById(id));
    r.setEndAfter(document.getElementById(id));
    s.addRange(r);
  }

}

function expand(id, buttonid, height) {
// expand or collapse the content of the given div
// height must be a string like "300px"
  if(document.getElementById(id).style.height!='') {
    document.getElementById(id).style.height='';
    document.getElementById(buttonid).innerHTML='Contract';
  } else {
    document.getElementById(id).style.height=height;
    document.getElementById(buttonid).innerHTML='Expand';
  }  
}

function createCodeHeader(codeID, height) {
// create a button "Select" and "Expand" for given codeID
// height is a string like "300px"
  document.write(' &lt;<span id="'+codeID+'select" onclick="select(\''+codeID+'\')" onmouseover="mouseOver(\''+codeID+'select\');" onmouseout="mouseOut(\''+codeID+'select\');" style="cursor: hand;">Select</span>&gt; ');
  document.write(' &lt;<span id="'+codeID+'expand" onclick="expand(\''+codeID+'\', \''+codeID+'expand\', \''+height+'\')" onmouseover="mouseOver(\''+codeID+'expand\');" onmouseout="mouseOut(\''+codeID+'expand\');" style="cursor: hand;">Expand</span>&gt; ');
}


