
#include "Mimetype.h"

/**
 * Initialize array with all available mime types (static array)
 */
Mimetype::MIME Mimetype::Mimetypes_[] =
{
  // The list with mime types comes from
  // http://en.wikipedia.org/wiki/Internet_media_type

  // Type application: Multipurpose files
  {"edi", "application/EDI-X12"},
  {"js", "application/javascript"},
  //{"???", "application/octet-stream"},
  {"ogg", "application/ogg"},
  {"pdf", "application/pdf"},
  {"xhtml", "application/xhtml+xml"},
  {"json", "application/json"},
  {"zip", "application/zip"},

  // Type audio: Audio
  {"mpeg", "audio/mpeg"},
  {"mp3", "audio/mpeg"},
  {"wma", "audio/x-ms-wma"},
  {"ram", "audio/vnd.rn-realaudio"},
  {"wav", "audio/x-wav"},

  // Type image
  {"gif", "image/gif"},
  {"jpeg", "image/jpeg"},
  {"jpg", "image/jpeg"},
  {"png", "image/png"},
  {"svg", "image/svg+xml"},
  {"tiff", "image/tiff"},
  {"ico", "image/vnd.microsoft.icon"},

  //Type message
  //{"http", "message/http"},  // --> defined as type text

  // Type multipart: Archives and other objects made of more than one part
  /*
  {"eml", "multipart/mixed"},  
  {"???", "multipart/alternative"},
  {"???", "multipart/related"},
  {"???", "multipart/form-data"},
  */

  // Type text: Human-readable text and source code
  {"css", "text/css"},
  {"csv", "text/csv"},
  {"html", "text/html"},
  // {"js", "text/javascript"},  // obsolete --> has become application/javascript
  {"txt", "text/plain"},
  {"xml", "text/xml"},

  // Type video: Video
  {"mpeg", "video/mpeg"},
  {"mpg", "video/mpeg"},
  {"mpg", "video/mpeg"},
  {"mp4", "video/mp4"},
  {"mov", "video/quicktime"},
  {"wmv", "video/x-ms-wmv"},

  // Type vnd: Vendor Specific Files
  {"xls", "application/vnd.ms-excel"},
  {"ppt", "application/vnd.ms-powerpoint"},
  {"pps", "application/vnd.ms-powerpoint"},
  {"doc", "application/msword"},
  {"xul", "application/vnd.mozilla.xul+xml"},

  // Type x: Non-standard files
  {"dvi", "application/x-dvi"},
  {"php", "application/x-httpd-php"},
  {"php", "application/x-httpd-php-source"},  // ???
  {"tex", "application/x-latex"},
  {"swf", "application/x-shockwave-flash"},
  {"sit", "application/x-stuffit"},
  {"rar", "application/x-rar-compressed"},
  {"tar", "application/x-tar"},
  {"exe", "application/x-msdownload"},
};


Mimetype::Mimetype()
{
  mimeCount_ = sizeof(Mimetypes_) / sizeof(MIME);
  set("");
}

Mimetype::Mimetype(const string& filename)
{
  mimeCount_ = sizeof(Mimetypes_) / sizeof(MIME);
  set(filename);
}

/**
 * Set the Mimetype depending on the extension of the provided filename
 * @param filename    name of a file with certain extension.
 *                    For example "index.html"
 */
void Mimetype::set(const string& filename)
{
  int dot = filename.rfind(".");
  string extension = lcase(filename.substr(dot + 1));
  mimeId_ = -1;

  for (int i = 0; i < mimeCount_; i++)
  {
    if (strcmp(extension.c_str(), Mimetypes_[i].extension) == 0)
    {
      mimeId_ = i;
      break;
    }
  }

  if (mimeId_ != -1)
  {
    type_ = Mimetypes_[mimeId_].type;
  }
  else
  {
    // unknown mime type
    type_ = "application/x";
  }
}


/**
 * Returns the content type for the lastly set filename. Returns an empty
 * string if the type is unknown or not set.
 * @return contenttype Content type to be added to the html header.
 *                     For example "text/html" or "image/jpeg".
 */
string Mimetype::get()
{
  return type_;
}

/**
 * Returns true if the currently set file type is known
 */
bool Mimetype::isKnownType()
{
  return (mimeId_ != -1);
}

