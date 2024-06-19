/**
 * Convert data in CSV (comma separated value) format to a javascript array.
 * Columns are separated by a comma or a custom delimeter.
 * Rows are separated by a new line character "\n".
 *
 * Values can be enclosed by single quotes ' or double quotes ".
 * Inside a quoted value, the escape character \ can be used.
 *
 * Example usage:
 *   var csv = '"x", "y", "z"\n12.3, 2.3, 8.7\n4.5, 1.2, -5.6\n';
 *   var array = csv2array(csv);
 * 
 * Author: Jos de Jong, 2010
 * 
 * @param {string} data      The data in CSV format.
 * @param {string} delimeter [optional] a custom delimeter. Comma ',' by default
 *                           The Delimeter must be a single character.
 * @return {Array} array     A two dimensional array containing the data
 */ 
function csv2array(data, delimeter) {
  // Retrieve the delimeter
  if (delimeter == undefined) 
    delimeter = ',';
  if (delimeter && delimeter.length > 1)
    delimeter = ',';

  // initialize variables
  var newline = '\n';
  var eof = '';
  var i = 0;
  var c = data.charAt(i);
  var row = 0;
  var col = 0;
  var array = new Array();
  
  /**
   * Set the pointer to the next character
   */ 
  var next = function() {
    c = data.charAt(++i);
  }
  
  while (c != eof) {
    // skip whitespaces
    while (c == ' ' || c == '\t') {
      next();
    }
    
    // get value
    var value = "";
    if (c == '\'') {
      // value enclosed by single quotes
      next();
      while (c != eof && c != '\'') {
        // check for escape character
        if (c == '\\')
          next();
          
        value += c;
        next();
      }      
      next();
    } 
    else if (c == '\"') {
      // value enclosed by double quotes
      next();
      while (c != eof && c != '\"') {
        // check for escape character
        if (c == '\\')
          next();

        value += c;
        next();
      }      
      next();      
    }
    else {
      // value without quotes
      while (c != eof && c != delimeter && c!= newline && c != ' ' && c != '\t') {
        value += c;
        next();
      }
    }

    // add the value to the array
    if (array.length <= row) 
      array.push(new Array());
    array[row].push(value);
    
    // skip whitespaces
    while (c == ' ' || c == '\t') {
      next();
    }

    // go to the next row or column
    if (c == delimeter) {
      // to the next column
      col++;
    }
    else if (c == newline) {
      // to the next row
      col = 0;
      row++;
    }
    else {
      // eof reached
    }
    next();
  }  
  
  return array;
}
