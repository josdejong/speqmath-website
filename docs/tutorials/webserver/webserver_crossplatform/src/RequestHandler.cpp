
#include "RequestHandler.h"


RequestHandler::RequestHandler()
  : client_(0)
{
}

RequestHandler::~RequestHandler()
{
  if (client_ != 0)
  {
    delete client_;
    client_ = 0;
  }
}


/**
 * Process a connection to a client:
 * - receive the http request
 * - send a reply with a (html) file
 * @param client   Pointer to a connected client
 * @return success True if succesfully handled, else false
 */
bool RequestHandler::process(ServerSocket*& client, const string& rootfolder)
{
  bool success = false;

  if (client != 0)
  {
    client_ = client;
    client = 0;
  }
  else
  {
    success = false;
    return success;
  }
  rootfolder_ = rootfolder;

  try
  {
    string http;
    (*client_) >> http;

    #ifdef DEBUG
      cout << "http=\n" << http << endl;
    #endif

    HttpRequest httpRequest(http);
    url_ = decodeUrl(httpRequest.url);

    string fullname = rootfolder_ + url_;

    string info = "client=" + client_->getIp() + ":" + toString(client_->getPort()) +
                  ", url='" + url_ + "'";
    printf("%s %s\n", getCurrentDatetime().c_str(), info.c_str());
    writeLog(info);

    if ( !directoryExists(fullname) )
    {
      // the url is a file
      if (fileExists(fullname))
      {
        string r = httpRequest.headers["Range"];
        HttpRequest::Range range(r);
        success = replyFile(range);
      }
      else
      {
        success = replyNotFound();
      }
    }
    else
    {
      // the url is a directory
      if (fullname.size() > 0 &&
          fullname[fullname.size()-1] == '/')
      {
        // The url ends with a slash
        if (fileExists(fullname + "index.html"))
        {
          // There is an index file available in this directory.
          url_ += "index.html";
          success = replyFile();
        }
        else
        {
          // No index file. reply with the contents of the directory
          success = replyDirectory();
        }
      }
      else
      {
        // The url does not end with a slash. send a moved permanently
        url_ += '/';
        success = replyMovedPermanently();
      }
    }

    if (!success)
    {
      success = replyInternalServerError();
    }
  }
  catch ( SocketException& err)
  {
    printf("Error: %s\n", err.description().c_str());
    success = false;
  }

  return success;
}


bool RequestHandler::replyFile(const HttpRequest::Range& range)
{
  bool success = false;
  if (client_ == 0) return false;

  string fullname = rootfolder_ + url_;

  Mimetype type(fullname);
  long filesize = getFileSize(fullname.c_str());
  if (filesize < 0)
  {
    writeLog("Error: Too big file (exceeds 2^31 bytes)\n");
    printf("Error: Too big file (exceeds 2^31 bytes)\n");
    return false;
  }

  HttpResponse http(200);
  http.headers["Accept-Ranges"]  = "bytes";
  http.headers["Content-Type"]   = type.get();

  if (range.from == range.npos && range.to == range.npos)
  {
    http.headers["Content-Length"] = toString(filesize);
  }
  else
  {
    // range is provided. Filesize is different.
    long rangeFrom = range.from;
    long rangeTo = range.to;
    if (range.from == range.npos) rangeFrom = 0;
    if (range.to == range.npos)   rangeTo = filesize - 1;
    long length = rangeTo - rangeFrom;

    http.headers["Content-Range"] = "bytes=" + toString(rangeFrom)
                                    + "-" + toString(rangeTo)
                                    + "/" + toString(filesize);
    http.headers["Content-Length"] = toString(length);
  }

  (*client_) << http.get();

  #ifdef DEBUG
    cout << "http response:\n" << http.get() << endl;
  #endif


  FILE * pFile;
  pFile = fopen (fullname.c_str(), "rb");
  if (pFile != 0)
  {
    // read and send in blocks of limited size.
    const unsigned buffsize = min(long(1024 * 1024), filesize);
    char* buff = new char[buffsize];
    unsigned buffread = 0;

    if (range.from != range.npos || range.to != range.npos)
    {
      // start range is provided. Put file pointer there
      fseek ( pFile, range.from, SEEK_SET );
    }

    do
    {
      buffread = fread (buff, 1, buffsize, pFile);
      client_->send(buff, buffread);
    }
    while (buffread > 0);

    delete buff;
    buff = 0;

    fclose (pFile);
    success = true;
  }
  else
  {
    success = false;
  }

  return success;
}


bool RequestHandler::replyDirectory()
{
  if (client_ == 0) return false;

  HttpResponse http(200);
  (*client_) << http.get();

  string html =
    "<html>\n"
    "<head>\n"
    "<title>Index of " + url_ + "</title>\n"
    "<style>\n"
    "  td {padding-right: 50px; vertical-align:top}\n"
    "</style>\n"
    "</head>\n"
    "<body>\n"
    "<h1>Index of " + url_ + "</h1>\n";

  string fullpath = rootfolder_ + url_;

  DIR *dir;
  dir = opendir(fullpath.c_str());
  if (dir != 0)
  {
    std::list<string> dirs;
    std::list<string> files;

    struct dirent *dirp;
    while ((dirp = readdir(dir)) != NULL)
    {
      string name = dirp->d_name;
      if (name != ".")
      {
        if (directoryExists(fullpath + name))
        {
          // this is a directory
          dirs.push_back(name);
        }
        else
        {
          // this is a file
          files.push_back(name);
        }
      }
    }
    closedir(dir);

    // sort the files and dirs
    dirs.sort(compare_nocase);
    files.sort(compare_nocase);

    html += "<table><th>\n";
    html += "<td>\n";
    html += "<h3>Directories</h3>\n";
    for (std::list<string>::iterator d = dirs.begin(); d != dirs.end(); ++d)
    {
      html += "<a href='" + url_ + *d + "/' class='dir'>"
             + *d + "</a><br>\n";
    }
    html += "</td>\n";
    html += "<td>\n";
    html += "<h3>Files</h3>\n";
    for (std::list<string>::iterator f = files.begin(); f != files.end(); ++f)
    {
      html += "<a href='" + url_ + *f + "'>" + *f + "</a><br>\n";
    }
    html += "</td>\n";
    html += "</th></table>\n";
  }
  else
  {
    html += "<font style='color:red;'>Error: could not read directory</font>";
  }

  html +=
    "</body>\n"
    "</html>\n";

  (*client_) << html;

  bool success = true;
  return success;
}


bool RequestHandler::replyMovedPermanently()
{
  if (client_ == 0) return false;

  HttpResponse http(301);
  http.headers["Location"] = url_;
  (*client_) << http.get();

  string html =
    "<html>"
    "<head>"
    "<title>301 Moved Permanently</title>"
    "</head>"
    "<body>"
    "<h1>301 Moved Permanently</h1>"
    "<p>The document has moved <a href=\"" + url_ + "\">here</a></p>"
    "</body>"
    "</html>";

  (*client_) << html;

  bool success = true;
  return success;
}


bool RequestHandler::replyNotFound()
{
  if (client_ == 0) return false;

  HttpResponse http(404);
  (*client_) << http.get();

  // send error code "Not Found"
  string html =
    "<html>"
    "<head>"
    "<title>Error 404: Not Found</title>"
    "</head>"
    "<body>"
    "<h1 style='color:red;'>Error 404: Not Found</h1>"
    "File " + url_ + " not found."
    "</body>"
    "</html>";

  (*client_) << html;

  bool success = true;
  return success;
}

bool RequestHandler::replyInternalServerError()
{
  if (client_ == 0) return false;

  HttpResponse http(500);
  (*client_) << http.get();

  // send error code "Internal Server Error"
  string html =
    "<html>"
    "<head>"
    "<title>Error 500: Internal Server Error</title>"
    "</head>"
    "<body>"
    "<h1 style='color:red;'>Error 500: Internal Server Error</h1>"
    "Request of file " + url_ + " resulted in an error."
    "</body>"
    "</html>";

  (*client_) << html;

  bool success = true;
  return success;
}


bool RequestHandler::replyUnsupported()
{
  if (client_ == 0) return false;

  HttpResponse http(415);
  (*client_) << http.get();

  // send error code "Not Found"
  string html =
    "<html>"
    "<head>"
    "<title>Error 415: Unsupported Media Type</title>"
    "</head>"
    "<body>"
    "<h1 style='color:red;'>Error 415: Unsupported Media Type</h1>"
    "File " + url_ + " not supported."
    "</body>"
    "</html>";

  (*client_) << html;

  bool success = true;
  return success;
}




/**
 * Pre-process the provided url.
 * - Remove references to parent directories "/.."
 * - Decode characters like "%20"
 * The resulting url always starts with a slash '/' and does end without slash
 * @param rawUrl     string with raw data that must be processed
 * @return url       the pre-processed data
 */
string RequestHandler::decodeUrl(const string& rawUrl)
{
  string url;

  for (string::const_iterator i = rawUrl.begin(); i != rawUrl.end(); i++)
  {
    char c = '\0';

    if (*i != '%')
    {
      // this is a normal character
      c = *i;
    }
    else
    {
      // replace all special html characters like "%20"
      //char c1 = rawUrl[i + 1];
      //char c2 = rawUrl[i + 2];

      c = (hex2dec(*(i+1)) << 4) + hex2dec(*(i+2));
      i += 2;
    }

    // Replace backslashes "\" with forward slash "/"
    if (c == '\\')
    {
      c = '/';
    }

    // Handle references to parent directory "/.." and ".."
    unsigned s = url.size();
    if ( s == 1 && url[0] == '.' ||
        (s > 1  && url[s-2] == '/' && url[s-1] == '.'))
    {
      // Last two characters in url are "/." or the url so far is ".",
      // so now we would get "/..". Therefore remove last directory in the url.
      unsigned slash = url.rfind('/', s - 3);
      if (slash != string::npos)
      {
        url.erase(slash);
      }
      else
      {
        url.clear();
      }
    }
    else
    {
      url += c;
    }
  }

  // the url may not be empty
  if (url.empty())
  {
    url = '/';
  }

  return url;
}
