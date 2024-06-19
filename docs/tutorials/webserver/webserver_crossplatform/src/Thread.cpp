
#include "Thread.h"


namespace Thread
{
  class ThreadData
  {
  public:
    ThreadData() : params(0) {}
    THREAD_FUNCTION func;
    void* params;
  };

  /**
   * This function is executed as thread, and in turn executes the requested
   * function.
   * @param pthreaddata   A pointer to a THREAD_DATA object, which contains
   *                      the address of the function to be executed and
   *                      (optionally) a pointer to a parameter to be send to
   *                      the function.
   */
  #ifdef __linux__
    void* run(void* pthreaddata)
  #endif
  #ifdef WIN32
    unsigned __stdcall run(void* pthreaddata)
  #endif
  {
    ThreadData* threaddata = (ThreadData*)pthreaddata;
    pthreaddata = 0;

    THREAD_FUNCTION func = threaddata->func;
    void* params = threaddata->params;

    // Here the actual function is executed
    // This function is supposed to cleanup the variable params itself.
    func(params);

    // cleanup threaddata
    delete threaddata;
    threaddata = 0;

    return 0;
  }


  /**
   * Run the provided function from within a thread.
   * @param func   A function, defined as:
   *                   static void func(void* params);
   * @param params A pointer to be send to the function. You can pass parameters
   *               To the function via this pointer.
   * @return success  True if succesfully created, else false
   */
  bool create(THREAD_FUNCTION func, void* params)
  {
    ThreadData* threaddata = new ThreadData();
    threaddata->func = func;
    threaddata->params = params;

    #ifdef __linux__
      pthread_t thread = 0;
      unsigned ret = pthread_create( &(thread), 0, run, (void*) threaddata);
      pthread_detach(thread);
      if (ret == 0) return true;  // creating thread was succesfull
    #endif

    #ifdef WIN32
      unsigned ret;
      HANDLE thread = (HANDLE) _beginthreadex(0, 0, run, (void*) threaddata, 0, &ret);
      CloseHandle(thread);
      if (ret != 0) return true; // creating thread was succesfull
    #endif

    // creating thread failed. Cleaup up created data.
    delete threaddata;
    threaddata = 0;
    return false;
  }
}
