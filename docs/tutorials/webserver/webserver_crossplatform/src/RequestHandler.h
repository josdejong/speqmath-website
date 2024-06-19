

#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <string>
#include <vector>
#include <iostream>
#include <list>
#include <algorithm>

#include "ServerSocket.h"
#include "SocketException.h"
#include "Mimetype.h"
#include "GeneralFunctions.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

using namespace std;

class RequestHandler
{
public:
  RequestHandler();
  ~RequestHandler();

  bool process(ServerSocket*& client, const string& rootfolder);

private:
  bool replyFile(const HttpRequest::Range& range = HttpRequest::Range());
  bool replyDirectory();
  bool replyMovedPermanently();
  bool replyNotFound();
  bool replyUnsupported();
  bool replyInternalServerError();

private:
  string decodeUrl(const string& rawUrl);

private:
  ServerSocket* client_;
  string rootfolder_;
  string url_;
};

#endif
