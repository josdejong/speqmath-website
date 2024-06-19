
#include "Webserver.h"



Webserver::Webserver()
{
}


Webserver::Webserver(const Settings& settings)
{
  settings_ = settings;
}

/**
 * Start the server
 */
void Webserver::start()
{
  writeLog("Starting server... ");

  Settings* settingsCopy = new Settings(settings_);

  Thread::create(run, settingsCopy);
}



/**
 * Stop the server
 */
void Webserver::stop()
{
/* TODO: terminate thread
  if (hThread_ != 0)
  {
    writeLog("Stopping server... ");

    TerminateThread(hThread_, 0);
    CloseHandle(hThread_);
    hThread_ = 0;

    writeLog("Server stopped.");
  }
  */
}


/**
 * Start running as server
 */
void Webserver::run(void* psettings)
{
  Settings* settings = (Settings*)psettings;
  psettings = 0;

  // Create the socket
  int port = settings->port;
  string rootfolder = settings->rootfolder;

  try
  {
    ServerSocket server ( port );

    printf("C++ Webserver " VERSION "\n\n");
    settings->print();

    while (1)
    {
      ServerSocket* client = new ServerSocket;
      server.accept ( *client );

      string clientIp = client->getIp();
      if (!settings->onBlacklist( client->getIp()))
      {
        // Handle the request
        handleClient(client, settings->rootfolder);
      }
      else
      {
        // Block this ip as it is on the blacklist
        writeLog("client=" +
                 client->getIp() + ":" +
                 toString(client->getPort()) +
                 " ***BLOCKED*** (listed in the blacklist).");
        delete client;
        client = 0;
      }
    }
  }
  catch (SocketException err)
  {
    printf("Error: %s\n", err.description().c_str());
  }

  delete settings;
  settings = 0;
}


struct PARAMS
{
  PARAMS()
    : client(0)
  {}

  ServerSocket* client;
  string rootfolder;
};


void Webserver::handleClient(ServerSocket*& client, const string& rootfolder)
{
  PARAMS* params = new PARAMS;
  params->rootfolder = rootfolder;
  params->client = client;
  client = 0;

  Thread::create(clientThread, params);
}


/**
 * Function to handle a client connection.
 * This function must be executed as a thread.
 */
void Webserver::clientThread(void* vparams)
{
  PARAMS* params = (PARAMS*)vparams;
  vparams = 0;

  RequestHandler requestHandler;
  requestHandler.process(params->client, params->rootfolder);

  delete params;
  params = 0;
}


