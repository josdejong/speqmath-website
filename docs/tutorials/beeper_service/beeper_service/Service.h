/**
 * Service
 *
 * Usage of the service functions:
 *
 * initService(myServiceName, &myServiceFunction);
 *      myServiceFunction is a custom function. This function must contain
 *      a loop which checks whether stopRequest() is false. (If not,
 *      the function must be exited).
 * processServiceArgument(argument);
 *      argument is a string "install", "uninstall", "start", "stop",
 *      "restart", "state", "help".
 * run();
 *      run the service. Can only be executed when the program is started
 *      as service by Windows, not when just running the program as executable.
 *
 * Todo:
 * It would be nice to pack all functions and data in a class. But this
 * gives problems with running the service: StartServiceCtrlDispatcher() needs
 * to access the functions and data. (so functions must be static...)
 *
 * Sources:
 * http://www.programmersheaven.com/mb/Win32API/185575/186103/re-almost-there/?S=B20000
 * http://www.experts-exchange.com/codeSnippetPopup.jsp?csid=28916
 * http://msdn.microsoft.com/en-us/library/ms810429.aspx
 * http://msdn.microsoft.com/en-us/library/ms686315(VS.85).aspx
 * http://msdn.microsoft.com/en-us/library/ms686335(VS.85).aspx
 *
 * Jos de Jong, 2009
 */


#ifndef SERVICE_H
#define SERVICE_H

#include <cassert>
#include <windows.h>
#include <tchar.h>
#include <cstdio>


namespace service
{
  // type definition for the service function
  typedef void (*SERVICE_FUNCTION)(void);

  // functions for initializing and controlling the service namespace
  void init(const char* name, SERVICE_FUNCTION serviceFunction);
  bool processCommand(const char* argument);
  bool stopRequested();

  const char* getError();
  void setError(const char* error, ...);
  void printHelp();

  // functions for retrieving the status of the service
  bool isInstalled();
  bool isRunning();
  bool isStopped();
  bool isPaused();
  const char* getState();

  // functions for changing the state of the service
  bool install();
  bool uninstall();
  bool start();
  bool stop();
  bool restart();

  bool run();
}


#endif

