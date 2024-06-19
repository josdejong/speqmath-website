/**

DOCUMENTATION
Http:
  http://web-sniffer.net/
  Wireshark
Webservers:
  http://www.adp-gmbh.ch/win/misc/webserver.html
  http://www.jbox.dk/sanos/webserver.htm
Urls:
  Encoding: http://www.blooberry.com/indexdot/html/topics/urlencoding.htm
  Relative URL, Base URL, etc: http://www.webreference.com/html/tutorial2/3.html

*/

#include "Webserver.h"


#define PAUSE {fflush(stdin); getchar(); fflush(stdin);}


using namespace std;


int main ( int argc, char* argv[] )
{
  Settings settings("settings.ini");
  settings.load();
  settings.save();

  Webserver webserver(settings);
  webserver.start();

  printf("Press Enter to quit the webserver\n\n");
  PAUSE;

  webserver.stop();

  return 0;
}
