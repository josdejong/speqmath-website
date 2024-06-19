
#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <map>

#include "GeneralFunctions.h"
#include "Resources.h"


using namespace std;

class HttpResponse
{
public:
// Functions
  HttpResponse();
  HttpResponse(const int status);

  string get() const;
  void set(const int theStatus = 200);

  friend ostream& operator<<(ostream& os, HttpResponse& http);

// Data
  string              version; // HTTP version
  int                 status;  // HTTP status (for example 200, 404, etc)
  map<string, string> headers;

private:
  string getStatusMessage(const int status) const;
};

#endif
