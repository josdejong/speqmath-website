
#include "HttpResponse.h"

HttpResponse::HttpResponse()
{
  set();
}

HttpResponse::HttpResponse(const int theStatus)
{
  set(theStatus);
}

/**
 * Initialize a new header
 * @param status   The status for the http response.
 *                 Status is optional, the default status is 200.
 */
void HttpResponse::set(const int theStatus)
{
  version = "HTTP/1.1";
  status = theStatus;

  headers.clear();
  headers["Date"] = getCurrentDatetime();
  headers["Server"] = "C++ Webserver " VERSION;
  headers["Content-Type"] = "text/html";
}



/**
 * Return the complete http header as a string.
 */
string HttpResponse::get() const
{
  string httpHeader;

  httpHeader += version + " ";
  httpHeader += toString(status) + " ";
  httpHeader += getStatusMessage(status) + "\r\n";

  for(map<string, string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
  {
    httpHeader += it->first;
    httpHeader += ": ";
    httpHeader += it->second;
    httpHeader += "\r\n";
  }
  httpHeader += "\r\n";

  return httpHeader;
}

/**
 * Add the http response as a string to the output stream.
 * Example usage:
 *    HttpResponse myhttp(404);
 *    cout << "http:" << myhttp << endl;
 */
ostream& operator<<(ostream& os, HttpResponse& http)
{
  return os << http.get();
}


/**
 * Returns the description of the given http status message.
 * For example status=200 returns "Ok". If the given status number is
 * unknown, the function returns an empty string.
 * @param status        Status number
 * @return description  Status description
 */
string HttpResponse::getStatusMessage(const int status) const
{
  switch (status)
  {
    case 200: return string("Ok");
    case 301: return string("Moved Permanently");
    case 404: return string("Not Found");
    case 415: return string("Unsupported Media Type");
    case 500: return string("Internal Server Error");
    default:  return string("");
  }
}
