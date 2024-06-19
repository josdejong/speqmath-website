
#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <map>
#include <cstdlib>



#include "GeneralFunctions.h"

using namespace std;

class HttpRequest
{
public:
  HttpRequest();
  HttpRequest(const string& httpHeader);

  bool parse(const string& httpHeader);
  void clear();

  string get() const;
  friend ostream& operator<<(ostream& os, HttpRequest& http);

  long getRangeFrom() const;
  long getRangeTo() const;

  class Range
  {
  public:
    Range();
    Range(const string& range);
    static long npos;

  private:
    bool set(const string& range);

  public:
    long from;
    long to;
  };

  // Data
  string              method;   // HTTP method (GET, POST, etc.)
  string              version;  // HTTP version
  string              url;
  map<string, string> headers;
};

#endif
