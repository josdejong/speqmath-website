/**
 * Class Error
 */


class Error extends Exception
{

  /**
   * Create an error with given message id and fill in given string in message
   * @PARAM id    id of the message
   * @PARAM str   a string which will be filled in in the message
   */
  Error(final int id, final String str)
  { 
    row_ = -1;
    col_ = -1;
    id_ = id;
  
    msg_ = String.format(errorMsg(id_), str);
  }

  /**
   * Create an error with given message id and fill in given string in message
   * @PARAM id    id of the message
   */
  Error(final int id)
  { 
    row_ = -1;
    col_ = -1;
    id_ = id;
  
    msg_ = errorMsg(id_);
  }

  /**
   * Create an error with given message id and fill in given string in message
   * @PARAM row   row where the error occured
   * @PARAM col   column where the error occured 
   * @PARAM id    id of the message
   * @PARAM str   a string which will be filled in in the message
   */
  Error(final int row, final int col, final int id, final String str)
  { 
    row_ = row;
    col_ = col;
    id_ = id;
  
    msg_ = String.format(errorMsg(id_), str);
  }

  /**
   * Create an error with given message id and fill in given string in message
   * @PARAM row   row where the error occured 
   * @PARAM col   column where the error occured 
   * @PARAM id    id of the message
   */
  Error(final int row, final int col, final int id)
  { 
    row_ = row;
    col_ = col;
    id_ = id;
  
    msg_ = errorMsg(id_);
  }

  /**
   * Returns the error message, including line and column number
   */
  final String get()
  {
    String res;
    if (row_ == -1)
    {
      if (col_ == -1)
      {
        res = String.format("Error: %s", msg_);
      }
      else
      {
        res = String.format("Error: %s (col %d)", msg_, col_);
      }
    }
    else
    {
      res = String.format("Error: %s (ln %d, col %d)", msg_, row_, col_);
    }
    return res;
  }

  int get_id()
  {
    return id_;
  }

/// Private functions

  /**
   * Returns a pointer to the message description for the given message id.
   * Returns "Unknown error" if id was not recognized.
   */
  private String errorMsg(final int id)
  {
    switch (id)
    {
      // syntax errors
      case 1: return "Syntax error in part \"%s\"";
      case 2: return "Syntax error";
      case 3: return "Parentesis ) missing";
      case 4: return "Empty expression";
      case 5: return "Unexpected part \"%s\"";
      case 6: return "Unexpected end of expression";
      case 7: return "Value expected";

      // wrong or unknown operators, functions, variables
      case 101: return "Unknown operator %s";
      case 102: return "Unknown function %s";
      case 103: return "Unknown variable %s";
      case 104: return "Unknown operator";

      // domain errors
      case 200: return "Too long expression, maximum number of characters exceeded";
      
      // error in assignments of variables
      case 300: return "Defining variable failed";
      
      // error in functions
      case 400: return "Integer value expected in function %s";
      
      // unknown error
      case 500: return "%s";
    }

    return "Unknown error";
  }  

/// Data  
  private int row_;    /// row where the error occured
  private int col_;    /// column (position) where the error occured
  private int id_;     /// id of the error
  private String msg_;
}
