
#include "GeneralFunctions.h"



/**
 * Returns the size of the file
 * Works for files up to 2.14 GB.
 * http://www.codeproject.com/KB/files/filesize.aspx
 */
long getFileSize(const char* sFileName)
{
  std::ifstream f;
  f.open(sFileName, std::ios_base::binary | std::ios_base::in);
  if (!f.good() || f.eof() || !f.is_open()) { return 0; }
  f.seekg(0, std::ios_base::beg);
  std::ifstream::pos_type begin_pos = f.tellg();
  f.seekg(0, std::ios_base::end);
  return static_cast<long>(f.tellg() - begin_pos);
}


/**
 * check if given file exists and is readable
 * @param filename  the name of the file
 * @return          true if the file exists. Else false
 */
bool fileExists(const char* filename)
{
  // will not work if you do not have read permissions
  // to the file, but if you don't have read, it
  // may as well not exist to begin with.
  bool success = false;

  FILE* fp = 0;
  fp = fopen64(filename, "rb");
  if(fp != 0)
  {
    fclose(fp);
    success = true;
  }
  return success;
}


/**
 * check if given file exists and is readable
 * @param fileName  the name of the file
 * @return          true if the file exists. Else false
 */
bool fileExists(const string& filename)
{
  return fileExists(filename.c_str());
}



/**
 * Convert int to string
 */
string toString(const int value)
{
  char str[10] = "";
  sprintf(str, "%i", value);
  return str;
}

/**
 * Convert long to string
 */
string toString(const long value)
{
  char str[20] = "";
  sprintf(str, "%ld", value);
  return str;
}


/**
 * append text to the logfile of today
 * the text will be saved in the file named "log.txt"
 * when the provided text is empty, an empty line will be added to the logfile
 * @param text    the text that will be added to the logfile
 * @return        true if succesfull, else false.
 */
bool writeLog(const string& text)
{
  bool success = false;

  char logfilename[] = "log.txt";

  FILE* fp = 0;
  fp = fopen(logfilename, "a");
  if(fp != 0)
  {
    if (text.empty() == false)
    {
      // write the text to the file
      fprintf(fp, "%s %s\n", getCurrentDatetime().c_str(), text.c_str());
    }
    else
    {
      // add empty line to logfile
      fprintf(fp, "\n");
    }
    fclose(fp);
    success = true;
  }
  else
  {
    success = false;
  }

  return success;
}


/**
 * Create an error with given message id and fill in given string in message
 * Example usage:
 *     writeLog("New serialnumber %i, created at %s", nr, datetime);
 * @PARAM text  text to write to the log file.
 * @PARAM ...   additional arguments which will be filled in in the message,
 *              replacing %s, %i, %f, %c. (Like with printf()).
 */
bool writeLog(const char* text, ...)
{
  bool success = false;

  char logfilename[] = "log.txt";

  FILE* fp = 0;
  fp = fopen(logfilename, "a");
  if(fp != 0)
  {
    // fill in the additional parameters in the text,
    // and write the text to the file
    char textArgs[255] = "";
    va_list args;
    va_start(args, text);  // text is the last named parameter of this function
    vsnprintf(textArgs, sizeof(textArgs)-1, text, args);
    textArgs[sizeof(textArgs)-1] = '\0';
    va_end(args);

    fprintf(fp, "%s %s\n", getCurrentDatetime().c_str(), textArgs);
    fclose(fp);
    success = true;
  }
  else
  {
    success = false;
  }

  return success;
}


/**
 * Returns a string with the current date/time.
 * Returns a string with format "yyyy-mm-dd hh:mm:ss"
 */
string getCurrentDatetime()
{
  time_t t = time(0);
  tm time = *localtime(&t);

  // reformat the datetime to "yyyy-mm-dd hh:mm:ss"
  char timeChar[20];
  sprintf(timeChar,
          "%04i-%02i-%02i %02i:%02i:%02i",
          time.tm_year + 1900,
          time.tm_mon + 1,
          time.tm_mday,
          time.tm_hour,
          time.tm_min,
          time.tm_sec);

  return string(timeChar);
}


/**
 * Returns the path of the currently running program
 */
/* following works only on windows
string getStartPath()
{
  string path = "";


  #define MAX_PATH 1024
  char filename[ MAX_PATH + 1 ];
  if ( GetModuleFileName( 0, filename, sizeof(filename)/sizeof(filename[0]) ) > 0 )
  {
    string filenameStr(filename);
    unsigned int pos = filenameStr.rfind('\\');  // on linux this should be '/'

    if (pos != string::npos)
    {
      path = filenameStr.substr(0, pos);
    }
    else
    {
      // path empty
      path = ".";
    }
  }

  return path;
}*/


bool directoryExists( const char* path )
{
  if ( path == 0) return false;

  DIR *pDir;
  bool exists = false;

  pDir = opendir (path);
  if (pDir != 0)
  {
    exists = true;
    closedir (pDir);
  }

  return exists;
}


bool directoryExists( const string& path )
{
  return directoryExists(path.c_str());
}


/**
 * Make the provided string lowercase
 * @param   string which will be made lowercase
 * @return  the lowercase string
 */
string lcase(const string& str)
{
  string strLower = str;
  for (unsigned i = 0; i < strLower.size(); i++)
  {
    strLower[i] = tolower(strLower[i]);
  }
  return strLower;
}

/**
 * Make the provided string uppercase
 * @param   string which will be made uppercase
 * @return  the uppercase string
 */
string ucase(const string& str)
{
  string strUpper = str;
  for (unsigned i = 0; i < strUpper.size(); i++)
  {
    strUpper[i] = toupper(strUpper[i]);
  }
  return strUpper;
}


/**
 * Remove spaces and tabs from the left side of the provided string
 * @param s   string to get a left trim
 * @return    string without spaces and tabs at the left side
 */
string triml(const string& s)
{
  string sTrim = s;
  int pos(0);
  for ( ; sTrim[pos]==' ' || sTrim[pos]=='\t'; ++pos );
  sTrim.erase(0, pos);
  return sTrim;
}

/**
 * Remove spaces and tabs from the right side of the provided string
 * @param s   string to get a right trim
 * @return    string without spaces and tabs at the right side
 */
string trimr(const string& s)
{
  string sTrim = s;
  int pos(sTrim.size());
  for ( ; pos && sTrim[pos-1]==' ' || sTrim[pos]=='\t'; --pos );
  sTrim.erase(pos, sTrim.size()-pos);
  return sTrim;
}

/**
 * Remove spaces and tabs from the both sides of the provided string
 * @param s   string to get a trim
 * @return    string without spaces and tabs at start and end
 */
string trim(const string& s)
{
  return triml(trimr(s));
}


// comparison, not case sensitive.
bool compare_nocase (string first, string second)
{
  unsigned int i=0;
  while ( (i<first.length()) && (i<second.length()) )
  {
    if (tolower(first[i])<tolower(second[i])) return true;
    else if (tolower(first[i])>tolower(second[i])) return false;
    ++i;
  }
  if (first.length()<second.length()) return true;
  else return false;
}



/**
 * returns the decimal value of the provided char containing a hex value like 'D'
 */
int hex2dec(const char c)
{
  if (c == 0) return 0;

  char str[] = "0123456789ABCDEF";
  char* pch = static_cast<char*>(memchr(str, c, strlen(str)));
  if (pch)
  {
    return pch - str;
  }
  else
  {
    return 0;
  }
}


/**
 * Beep on linux
 */
void beep()
{
  printf("%c", 7);
}


