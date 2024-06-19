/**
 * Beeper service
 * Demo C++ windows service application
 *
 * Usage of the beeper service program:
 *    beeper_service.exe command
 * Where command can be: install, uninstall, start, stop, restart, state, help.
 * Enter "services.msc" to manage the state and properties of your services.
 *
 * Jos de Jong, 2009
 */


#include "Service.h"


char serviceName[] = "Beeper Service";


/**
 * Implementation function of the service.
 */
void doBeep()
{
  do
  {
    Beep( 1000, 100 );
  }
  while ( !service::stopRequested() );
}



int main( int argc, char* argv[] )
{
  // initialize the service
  service::init(serviceName, &doBeep);

  if (argc > 1)
  {
    // process an argument like "start" or "stop"
    const char* command = argv[1];
    bool success = service::processCommand(command);
    if ( !success )
    {
      printf("Error: %s\n", service::getError());
    }
  }
  else
  {
    // no arguments provided. Run the service
    bool success = service::run();
    if ( !success )
    {
      printf("Error: %s\n", service::getError());
      printf("\n");
      service::printHelp();
    }
  }

  return 0;
}
