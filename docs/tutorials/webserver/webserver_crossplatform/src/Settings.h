


#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "GeneralFunctions.h"
#include "Resources.h"

using namespace std;

class Settings
{
public:
  Settings();
  Settings(const char* filename_);
  Settings(const string& filename_);

  bool load();
  bool save();
  void print();

  bool onBlacklist(const string& ip);

private:
  bool set(const string& param, const string& value);

  bool loadBlacklist();

public:
  string filename;    // default is "settings.ini"

  int port;           // default is port 80
  string rootfolder;  // this is the subfolder "www"
  string blacklist;   // filename of the blacklist

private:
  vector<string> blacklist_;
};


