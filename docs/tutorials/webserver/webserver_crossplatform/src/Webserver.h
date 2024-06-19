/**
Webserver

This program can be used as client and as server. The client can send
messages to the server, and the server prints these messages on screen
and replies to the message.

Before you can compile the project for windows, you have to add "libws2_32.a"
to the linker of the compiler.

Before you can compile the project for linux, you have to add "pthread"
to the linker of the compiler.


Jos de Jong, 2009.
*/


#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <cstdio>
#include <string>

#include "ServerSocket.h"
#include "SocketException.h"
#include "Settings.h"
#include "GeneralFunctions.h"
#include "Resources.h"
#include "RequestHandler.h"
#include "Thread.h"


using namespace std;


class Webserver
{
public:
  Webserver();
  Webserver(const Settings& settings);

  void start();
  void stop();

private:
  static void run(void* psettings);
  static void handleClient(ServerSocket*& client, const string& rootfolder);
  static void clientThread(void* vparams);

  Settings settings_;
};


#endif
