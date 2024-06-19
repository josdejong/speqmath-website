
#include "Service.h"


namespace service
{
  // Private data
  TCHAR* name_ = 0;
  SERVICE_STATUS status_;
  SERVICE_STATUS_HANDLE statusHandle_ = 0;
  HANDLE stopEvent_ = 0;
  SERVICE_FUNCTION function_ = 0;
  const int errorLen_ = 255;
  char error_[errorLen_] = "";

  // Private functions
  // The following functions are hidden from the header file
  void WINAPI controlHandler( DWORD controlCode );
  void WINAPI main( DWORD argc, TCHAR* argv[] );
  int getStateValue(const SC_HANDLE service);
  int getStateValue();
  bool checkName();
}


/**
 * process a service argument. Possible arguments:
 * "install", "uninstall", "start", "stop", "restart", "state", "help"
 * @param command   pointer to an argument. Can also be zero
 * @return          true if processed succesfully, else false
 */
bool service::processCommand(const char* command)
{
  bool success = false;
  setError("");

  if (checkName())
  {
    if (command != 0)
    {
      if (strcmp(command, "install") == 0)
      {
        success = install();
      }
      else if (strcmp(command, "uninstall") == 0)
      {
        success = uninstall();
      }
      else if (strcmp(command, "start") == 0)
      {
        printf("Starting service...\n");
        success = start();
      }
      else if (strcmp(command, "stop") == 0)
      {
        printf("Stopping service...\n");
        success = stop();
      }
      else if (strcmp(command, "restart") == 0)
      {
        success = restart();
      }
      else if (strcmp(command, "help") == 0)
      {
        printHelp();
        success = true;
      }
      else if (strcmp(command, "state") == 0)
      {
        const char* state = getState();
        if (state != 0 && strlen(state) != 0)
        {
          printf("State of service \"%s\": %s\n", name_, state);
          success = true;
        }
        else
        {
          setError("Could not retrieve the state of service.");
          success = false;
        }
      }
      else if (strlen(command) != 0)
      {
        setError("Unknown command \"%s\".", command);
        success = false;
      }

      if (success)
      {
        printf("Service \"%s\" %s succesfully.\n", name_, command);
      }
    }
    else
    {
      setError("No command provided.");
      success = false;
    }
  }

  return success;
}


/**
 * Initialize the service functions
 * Example usage:
 *     initService("Beeper service", &doBeep);
 * where doBeep is a customized function. This function must contain a loop
 * which checks if stopRequest() is still false and if not, end the
 * function.
 * @param name              The name of the service
 * @param serviceFunction   Address of the function to be executed when the
 *                          service is running.
 */
void service::init(const char* name, SERVICE_FUNCTION serviceFunction)
{
  delete name_;
  name_ = new TCHAR[strlen(name) + 1];
  strcpy(name_, name);

  function_ = serviceFunction;
}


/**
 * Handler for the service control.
 */
void WINAPI service::controlHandler( DWORD controlCode )
{
	switch ( controlCode )
	{
		case SERVICE_CONTROL_INTERROGATE:
			break;

		case SERVICE_CONTROL_SHUTDOWN:
		case SERVICE_CONTROL_STOP:
			status_.dwCurrentState = SERVICE_STOP_PENDING;
			SetServiceStatus( statusHandle_, &status_ );

			SetEvent( stopEvent_ );
			return;

		case SERVICE_CONTROL_PAUSE:
			break;

		case SERVICE_CONTROL_CONTINUE:
			break;

		default:
			if ( controlCode >= 128 && controlCode <= 255 )
				// user defined control code
				break;
			else
				// unrecognised control code
				break;
	}

	SetServiceStatus( statusHandle_, &status_ );
}


/**
 * Main handler for the service
 */
void WINAPI service::main( DWORD argc, TCHAR* argv[] )
{
	// initialise service status
	status_.dwServiceType = SERVICE_WIN32;
	status_.dwCurrentState = SERVICE_STOPPED;
	status_.dwControlsAccepted = 0;
	status_.dwWin32ExitCode = NO_ERROR;
	status_.dwServiceSpecificExitCode = NO_ERROR;
	status_.dwCheckPoint = 0;
	status_.dwWaitHint = 0;

  if (checkName())
  {
    statusHandle_ = RegisterServiceCtrlHandler( name_, controlHandler );
  }
  else
  {
    statusHandle_ = 0;
  }

	if ( statusHandle_ )
	{
		// service is starting
		status_.dwCurrentState = SERVICE_START_PENDING;
		SetServiceStatus( statusHandle_, &status_ );

		// do initialisation here
		stopEvent_ = CreateEvent( 0, FALSE, FALSE, 0 );

		// running
		status_.dwControlsAccepted |= (SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);
		status_.dwCurrentState = SERVICE_RUNNING;
		SetServiceStatus( statusHandle_, &status_ );

    assert(function_ != 0);
    function_();

		// service was stopped
		status_.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus( statusHandle_, &status_ );

		// do cleanup here
		CloseHandle( stopEvent_ );
		stopEvent_ = 0;

		// service is now stopped
		status_.dwControlsAccepted &= ~(SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);
		status_.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus( statusHandle_, &status_ );
	}
}


/**
 * Run the service.
 * Before you can use this function you need to run initService() once.
 * This function can only be used when the executable is started as a service
 * and not as a regular executable.
 * @return true if succesfull, else false
 */
bool service::run()
{
  bool success = false;

  SERVICE_TABLE_ENTRY serviceTable[] =
  {
    { name_, main },
    { 0, 0 }
  };
  success = StartServiceCtrlDispatcher( serviceTable );
  if (!success )
  {
    setError("You cannot run this service as an executable.");
    //printHelp();  // TODO
  }
  return success;
}


/**
 * Install the service.
 * Before you can use this function you need to run initService() once.
 * @return true if succesfull, else false
 */
bool service::install()
{
  bool success = false;

  if ( !isInstalled() )
  {
    SC_HANDLE serviceControlManager = OpenSCManager( 0, 0, SC_MANAGER_CREATE_SERVICE );

    if ( serviceControlManager )
    {
      char path[ _MAX_PATH + 1 ];
      if ( GetModuleFileName( 0, path, sizeof(path)/sizeof(path[0]) ) > 0 )
      {
        SC_HANDLE service = CreateService(serviceControlManager,
                                          name_, name_,
                                          SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
                                          SERVICE_AUTO_START, SERVICE_ERROR_IGNORE, path,
                                          0, 0, 0, 0, 0 );
        if ( service )
        {
          CloseServiceHandle( service );
          success = true;
        }
      }

      CloseServiceHandle( serviceControlManager );
    }
  }
  else
  {
    setError("Cannot install the service: service is already installed.");
  }

  return success;
}


/**
 * Uninstall the service.
 * Before you can use this function you need to run initService() once.
 * @return true if succesfull, else false
 */
bool service::uninstall()
{
  bool success = false;
  if ( isInstalled() )
  {
    if (isRunning())
    {
      success = stop();
    }

    if (!isRunning())
    {
      SC_HANDLE serviceControlManager = OpenSCManager( 0, 0, SC_MANAGER_CONNECT );

      if ( serviceControlManager )
      {
        SC_HANDLE service = OpenService( serviceControlManager, name_, SERVICE_QUERY_STATUS | DELETE );
        if ( service )
        {
          SERVICE_STATUS status_;
          if ( QueryServiceStatus( service, &status_ ) )
          {
            if ( status_.dwCurrentState == SERVICE_STOPPED )
            {
              success = DeleteService( service );
            }
          }

          CloseServiceHandle( service );
        }

        CloseServiceHandle( serviceControlManager );
      }
    }
    else
    {
      setError("Cannot uninstall the service: failed to stop the service.");
    }
  }
  else
  {
    setError("Cannot uninstall the service: service is not installed.");
  }

  return success;
}


/**
 * Start the service.
 * Before you can use this function you need to run initService() once.
 * @return true if succesfull, else false
 */
bool service::start()
{
  bool success = false;

  if ( isInstalled() )
  {
    SC_HANDLE serviceControlManager = OpenSCManager( 0, 0, SC_MANAGER_CREATE_SERVICE );

    if ( serviceControlManager )
    {
      SC_HANDLE service = OpenService( serviceControlManager, name_,  SERVICE_ALL_ACCESS );

      if ( service )
      {
        success = StartService(service, 0, NULL);

        CloseServiceHandle( service );
      }

      CloseServiceHandle( serviceControlManager );
    }
  }
  else
  {
    setError("Cannot start the service: service is not installed.");
  }

  return success;
}

/**
 * Stop the service.
 * Before you can use this function you need to run initService() once.
 * @return true if succesfull, else false
 */
bool service::stop()
{
  bool success = false;

  if ( isInstalled() )
  {
    SC_HANDLE serviceControlManager = OpenSCManager( 0, 0, SC_MANAGER_ALL_ACCESS );

    if ( serviceControlManager )
    {
      SC_HANDLE service = OpenService(serviceControlManager, name_, SERVICE_ALL_ACCESS);
      if ( service )
      {
        SERVICE_STATUS lpServiceStatus;
        if (QueryServiceStatus(service, &lpServiceStatus) && lpServiceStatus.dwCurrentState == SERVICE_RUNNING)
        {
          success = ControlService(service, SERVICE_CONTROL_STOP, &lpServiceStatus );

          while (QueryServiceStatus(service, &lpServiceStatus) )
          {
            if  (lpServiceStatus.dwCurrentState == SERVICE_STOP_PENDING)
            {
              Sleep( 100 );
            }
            else
            {
              break;
            }
          }
        }
        else
        {
          setError("Cannot stop the service: the service is not running.");
        }

        CloseServiceHandle( service );
      }

      CloseServiceHandle( serviceControlManager );
    }
  }
  else
  {
    setError("Cannot stop the service: service is not installed.");
  }

  return success;
}

/**
 * Restart the service.
 * Before you can use this function you need to run initService() once.
 * @return true if succesfull, else false
 */
bool service::restart()
{
  bool success = true;

  if (isRunning())
  {
    success = stop();
  }
  if (success)
  {
    success = start();
  }

  return success;
}



/**
 * Checks if there is a service name provided (thus the service module
 * is initialized correctly). When there is no servicename filled in, an error
 * message is printed in the console
 * @return true if servicename is ok. else returns false
 */
bool service::checkName()
{
  bool success = false;
  if (name_ != 0 && name_[0] != '\0')
  {
    success = true;
  }
  else
  {
    setError("Service name is not initalized or empty.");
  }
  return success;
}


/**
 * Returns the current state of the provided service.
 * Possible values:
 *    SERVICE_CONTINUE_PENDING
 *    SERVICE_PAUSE_PENDING
 *    SERVICE_PAUSED
 *    SERVICE_RUNNING
 *    SERVICE_START_PENDING
 *    SERVICE_STOP_PENDING
 *    SERVICE_STOPPED
 * @param service   an open service.
 * @return          The current state of the service, or 0 when operation failed.
 */
int service::getStateValue(const SC_HANDLE service)
{
  int state = 0;
  if (service != NULL)
  {
    SERVICE_STATUS_PROCESS ssp;
    DWORD dwBytesNeeded = 0;
    if ( QueryServiceStatusEx( service, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssp, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded ) )
    {
      state =  ssp.dwCurrentState;
    }
    else
    {
      setError("QueryServiceStatusEx failed.");
      state = 0;
    }
  }
  else
  {
    setError("Service is not open.");
    state = 0;
  }

  return state;
}


/**
 * Returns the current state of the provided service as a string
 * Possible values:
 *    SERVICE_CONTINUE_PENDING
 *    SERVICE_PAUSE_PENDING
 *    SERVICE_PAUSED
 *    SERVICE_RUNNING
 *    SERVICE_START_PENDING
 *    SERVICE_STOP_PENDING
 *    SERVICE_STOPPED
 * @param name      the name of a service
 * @return          The current state of the service, or 0 when operation failed.
 */
int service::getStateValue()
{
  int stateValue = 0;

  if ( isInstalled() )
  {
    SC_HANDLE serviceControlManager = OpenSCManager( 0, 0, SC_MANAGER_CREATE_SERVICE );

    if ( serviceControlManager )
    {
      SC_HANDLE service = OpenService( serviceControlManager, name_,  SERVICE_ALL_ACCESS );

      if ( service )
      {
        stateValue = getStateValue(service);
        CloseServiceHandle( service );
      }

      CloseServiceHandle( serviceControlManager );
    }
  }
  else
  {
    setError("Cannot retrieve state: service is not installed.");
  }

  return stateValue ;
}



/**
 * Returns the current state of the provided service as a string
 * Possible values:
 *    "Continue Pending"
 *    "Pause Pending"
 *    "Paused"
 *    "Running"
 *    "Start Pending"
 *    "Stop Pending"
 *    "Stopped"
 * Before you can use this function you need to run initService() once.
 * @return          The current state of the service, or an empty string when
 *                  operation failed.
 */
const char* service::getState()
{
  const char* state = 0;

  int stateValue = getStateValue();
  switch (stateValue)
  {
    case SERVICE_CONTINUE_PENDING:  state = "Continue Pending"; break;
    case SERVICE_PAUSE_PENDING:     state = "Pause Pending";    break;
    case SERVICE_PAUSED:            state = "Paused";           break;
    case SERVICE_RUNNING:           state = "Running";          break;
    case SERVICE_START_PENDING:     state = "Start Pending";    break;
    case SERVICE_STOP_PENDING:      state = "Stop Pending";     break;
    case SERVICE_STOPPED:           state = "Stopped";          break;
    default:                        state = "";                 break;
  }

  return state;
}


/**
 * Checks if this service is installed.
 * Before you can use this function you need to run initService() once.
 * @return              true if installed, false if not
 */
bool service::isInstalled()
{
  bool isInstalled = false;

  if (checkName())
  {
    SC_HANDLE serviceControlManager = OpenSCManager( 0, 0, SC_MANAGER_CREATE_SERVICE );

    if ( serviceControlManager )
    {
      SC_HANDLE service = OpenService( serviceControlManager, name_,  SERVICE_ALL_ACCESS );

      if ( service )
      {
        // the service is found and connected
        CloseServiceHandle( service );
        isInstalled = true;
      }
      CloseServiceHandle( serviceControlManager );
    }
  }
  else
  {
    setError("No service name provided.");
  }

  return isInstalled;
}


/**
 * Checks if the service is running
 * Before you can use this function you need to run initService() once.
 * @return         true if the service is running, false if not or if the
 *                 service is not found or not installed.
 */
bool service::isRunning()
{
  return (getStateValue() == SERVICE_RUNNING);
}


/**
 * Checks if the service is stopped
 * Before you can use this function you need to run initService() once.
 * @return         true if the service is stopped, false if not or if the
 *                 service is not found or not installed.
 */
bool service::isStopped()
{
  return (getStateValue() == SERVICE_STOPPED);
}


/**
 * Checks if the service is paused
 * Before you can use this function you need to run initService() once.
 * @return         true if the service is paused, false if not or if the
 *                 service is not found or not installed.
 */
bool service::isPaused()
{
  return (getStateValue() == SERVICE_PAUSED);
}


/**
 * Checks if the service must be stopped.
 * You need to use this function when this program is executed as service
 * (and thus is running and executing its task). When isQuiting() becomes
 * true, a stop command is requested for the service, so the service must
 * terminate its task.
 * @return  true when the service needs to terminate its task.
 */
bool service::stopRequested()
{
  return (WaitForSingleObject( stopEvent_, 1000 ) != WAIT_TIMEOUT);
}


/**
 * Returns the last error. Returns 0 if there is no error.
 */
const char* service::getError()
{
  return error_;
}


/**
 * Set the last error.
 */
void service::setError(const char* msg, ...)
{
  assert(msg != 0);

  va_list args;
  va_start(args, msg);  // id is the last named parameter of this function
  vsnprintf(error_, sizeof(error_)-1, msg, args);
  error_[sizeof(error_)-1] = '\0';
  va_end(args);
}


/**
 * Print help on how to use this program
 */
void service::printHelp()
{
  printf("Usage: \n"
  "   beeper_service.exe command \n"
  "Where command can be: install, uninstall, start, stop, restart, state. \n"
  "Enter \"services.msc\" to manage the state and properties of your services.\n");
}


