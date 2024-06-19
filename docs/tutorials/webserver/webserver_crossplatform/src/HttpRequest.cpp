
#include "HttpRequest.h"


HttpRequest::HttpRequest()
{
}

HttpRequest::HttpRequest(const string& httpHeader)
{
  parse(httpHeader);
}

/**
 * Parse a httpHeader containing Method, url, version, and extra
 * http headers.
 * Note that you have to decode the parsed url afterwards!
 */
bool HttpRequest::parse(const string& httpHeader)
{
  clear();
  bool success = false;

  const char space = ' ';
  const char ret = '\r';
  const char newline = '\n';
  const char doubledot = ':';

  unsigned i = 0;

  // Get the method (GET, POST, etc)
  while (i < httpHeader.size() && httpHeader[i] != space)
  {
    method += httpHeader[i];
    i++;
  }

  // skip the spaces
  while (i < httpHeader.size() && httpHeader[i] == space) i++;

  // Get the url
  while (i < httpHeader.size() && httpHeader[i] != space)
  {
    url += httpHeader[i];
    i++;
  }

  // skip the spaces
  while (i < httpHeader.size() && httpHeader[i] == space) i++;

  // Get the http version
  while (i < httpHeader.size() &&
         httpHeader[i] != space &&
         httpHeader[i] != ret)
  {
    version += httpHeader[i];
    i++;
  }

  // skip the end of the line
  if (i < httpHeader.size() && httpHeader[i] == ret) i++;
  if (i < httpHeader.size() && httpHeader[i] == newline) i++;

  // Get the http headers one by one
  while (i < httpHeader.size())
  {
    // Get the parameter name
    string name;
    while (i < httpHeader.size() && httpHeader[i] != doubledot)
    {
      name += httpHeader[i];
      i++;
    }

    // skip spaces and doubledot
    while (i < httpHeader.size() &&
          (httpHeader[i] == space ||
           httpHeader[i] == doubledot))
            i++;

    // Get the parameter value
    string value;
    while (i < httpHeader.size() && httpHeader[i] != ret)
    {
      value += httpHeader[i];
      i++;
    }

    // skip spaces and line ends
    while (i < httpHeader.size() &&
          (httpHeader[i] == space ||
           httpHeader[i] == ret ||
           httpHeader[i] == newline))
             i++;

    // add the parameter name and value to the headers
    if (!name.empty())
    {
      // format the name starting with a capital letter, followed by lowercase letters.
      ucase(&name[0]);
      for (unsigned p = 1; p < name.size(); p++)
      {
        lcase(&name[p]);
      }

      headers[name] = value;
    }
  }

  // Define success when we have got a method and url. That is enough
  // to get to work.
  success = (!method.empty() && !url.empty());
  return success;
}

/**
 * Clear all data in the http request
 */
void HttpRequest::clear()
{
  method.clear();
  version.clear();
  url.clear();
  headers.clear();
}


/**
 * Return the complete http header as a string.
 */
string HttpRequest::get() const
{
  string httpHeader;

  httpHeader += method + " ";
  httpHeader += url + " ";
  httpHeader += version + "\r\n";

  for(map<string, string>::const_iterator it = headers.begin();
      it != headers.end();
      ++it)
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
 * Add the http request as a string to the output stream.
 * Example usage:
 *    cout << "http:" << myhttp << endl;
 */
ostream& operator<<(ostream& os, HttpRequest& http)
{
  return os << http.get();
}



long HttpRequest::Range::npos = -1;


HttpRequest::Range::Range()
  : from(npos), to(npos)
{
}

HttpRequest::Range::Range(const string& range)
  : from (npos), to(npos)
{
  set(range);
}


/**
 * Set a range.
 * @param range   String containging the range.
 *                For example, range = "bytes:2000-4000"
 * @return        True if succesfully parsed, else false.
 */
bool HttpRequest::Range::set(const string& range)
{
  from = npos;
  to = npos;

  string token;
  for (unsigned i = 0; i < range.size(); i++)
  {
    char c = range[i];

    switch (c)
    {
      case '=':
        // Check if the current token = "bytes"
        if (token != "bytes")
        {
          // Error: range other than bytes not supported.
          return false;
        }
        token.clear();
        break;

      case '-':
        // End of range-from reached.
        if (!token.empty())
        {
          from = atol(token.c_str());
        }
        token.clear();
        break;

      default:
        token += c;
        break;
    }
  }

  if (!token.empty())
  {
    // End of range-to read
    to = atol(token.c_str());
  }

  return true;
}
