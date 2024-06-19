/**
Thread functions

This file contains platform specific code.
To compile on linux, add the library "pthread" to the linker settings.


DOCUMENTATION
  http://opengroup.org/onlinepubs/007908799/xsh/pthread.h.html
  https://computing.llnl.gov/tutorials/pthreads/#PthreadsAPI
  http://www.yolinux.com/TUTORIALS/LinuxTutorialPosixThreads.html
**/

#ifndef THREAD_H
#define THREAD_H

#include <vector>

#ifdef __linux__
  #include <pthread.h>
#endif

#ifdef WIN32
  #include <process.h>
  #include "windows.h"
#endif

using namespace std;

namespace Thread
{
  typedef void (*THREAD_FUNCTION)(void*);

  bool create(THREAD_FUNCTION func, void* params);
};


#endif
