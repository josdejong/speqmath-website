
#include "Settings.h"


Settings::Settings()
 : filename("settings.ini"), port(80), rootfolder("www"), blacklist("blacklist.ini")
{
}

Settings::Settings(const char* filename_)
 : filename(filename_), port(80), rootfolder("www"), blacklist("blacklist.ini")
{
}

Settings::Settings(const string& filename_)
 : filename(filename_), port(80), rootfolder("www"), blacklist("blacklist.ini")
{
}



/**
 * Set the value of a parameter
 * @param param   The lowercase name of a parameter such
 *                as "port" or "rootfolder".
 * @param value   The value of the parameter
 * @return        True if the parameter is recognized and adjusted, else false.
 */
bool Settings::set(const string& param, const string& value)
{
  if (!param.empty())
  {
    if (param == "port")
    {
      port = atoi(value.c_str());
      return true;
    }
    else if(param == "rootfolder")
    {
      rootfolder = value;
      return true;
    }
    else if(param == "blacklist")
    {
      blacklist = value;
      return true;
    }
  }

  return false;
}


/**
 * Load settings from the provided file
 * Returns true if succesfull, else returns false
 */
bool Settings::load()
{
  bool success = false;

  FILE * pFile;
  pFile = fopen (filename.c_str(), "r");
  if (pFile != 0)
  {
    char c = '\0';
    string token;
    string param;
    string value;

    do
    {
      c = fgetc (pFile);

      switch (c)
      {
        case '#':
        {
          // handle a comment line
          set(param, value); // process the parameter and value
          token.clear();
          param.clear();
          value.clear();

          // read the rest of the line
          do
          {
            c = fgetc (pFile);
          }
          while (c != EOF && c != '\n');

          break;
        }

        case '=':
          // end of a parameter name
          param = lcase(token);

          token.clear();
          value.clear();
          break;

        case '\n':
        case EOF:
          // end of a parameter value
          value = token;
          set(param, value); // process the parameter and value

          token.clear();
          param.clear();
          value.clear();
          break;

        case '\r':
          // Just neglect '\r'
          break;

        default:
          token += c;
          break;
      }
    }
    while (c != EOF);

    fclose (pFile);
    success = true;
  }
  else
  {
    success = false;
  }
  if (!success) writeLog("Failed to load settings. Filename:" + filename);

  loadBlacklist();

  return success;
}


/**
 * Load all ip's in the blacklist
 * Returns true if succesfull, else returns false
 */
bool Settings::loadBlacklist()
{
  bool success = false;
  blacklist_.clear();

  FILE * pFile;
  pFile = fopen (blacklist.c_str(), "r");
  if (pFile != 0)
  {
    char c = '\0';
    string ip;

    do
    {
      c = fgetc (pFile);

      switch (c)
      {
        case '#':
        {
          // handle a comment line
          if (!ip.empty())
          {
            blacklist_.push_back(ip);
            ip.clear();
          }

          // read the rest of the line
          do
          {
            c = fgetc (pFile);
          }
          while (c != EOF && c != '\n');

          break;
        }

        case '\n':
        case EOF:
          // end of a parameter value
          if (!ip.empty())
          {
            blacklist_.push_back(ip);
            ip.clear();
          }
          break;

        case '\r':
          // Just neglect '\r'
          break;

        default:
          ip += c;
          break;
      }
    }
    while (c != EOF);

    fclose (pFile);
    success = true;
  }
  else
  {
    success = false;
  }

  if (!success) writeLog("Failed to load blacklist. Filename: " + blacklist);

  return success;
}


/**
 * Check if an ip is on the blacklist.
 * @param ip      Ip address to be checked
 * @param onlist  True if the given ip is on the blacklist, else false.
 */
bool Settings::onBlacklist(const string& ip)
{
  if (blacklist_.size() > 0)
  {
    for (unsigned i = 0; i < blacklist_.size(); i++)
    {
      if (ip == blacklist_[i])
        return true;
    }
  }

  return false;
}


/**
 * Save settings to the provided file
 * Returns true if succesfull, else returns false
 */
bool Settings::save()
{
  bool success = false;

  FILE * pFile;
  pFile = fopen (filename.c_str(), "w");
  if (pFile != 0)
  {
    fprintf(pFile, "# Webserver " VERSION "\n");
    fprintf(pFile, "port=%i\n", port);
    fprintf(pFile, "rootfolder=%s\n", rootfolder.c_str());
    fprintf(pFile, "blacklist=%s\n", blacklist.c_str());

    fclose (pFile);
    success = true;
  }
  else
  {
    success = false;
  }

  if (success)
  {
    if (!fileExists(blacklist))
    {
      FILE * pFile;
      pFile = fopen (blacklist.c_str(), "w");
      if (pFile != 0)
      {
        fprintf(pFile, "# Blacklist\n");
        fprintf(pFile, "# Enter ip addresses to be blocked here, one ip per line.\n");

        fclose (pFile);
        success = true;
      }
      else
      {
        success = false;
      }
    }
  }

  return success;
}



/**
 * Print the settings
 */
void Settings::print()
{
  printf("Settings\n");
  printf("port=%i\n", port);
  printf("rootfolder=%s\n", rootfolder.c_str());
  printf("blacklist=%s\n", blacklist.c_str());
  printf("\n");

  printf("Blacklist\n");
  if (blacklist_.size() > 0)
  {
    for (unsigned i =0; i < blacklist_.size(); i++)
    {
      printf("%s\n", blacklist_[i].c_str());
    }
  }
  else
  {
    printf("(empty)\n");
  }
  printf("\n");
}
