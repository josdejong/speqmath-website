// Definition of the Socket class

#ifndef SOCKET_H
#define SOCKET_H


#include <string>

#ifdef __linux__
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <netdb.h>
  #include <unistd.h>
  #include <arpa/inet.h>

  #define INVALID_SOCKET -1
#endif

#ifdef WIN32
  #include <WinSock2.h>

  typedef int socklen_t;
#endif



const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 25;
const int MAXRECV = 1000;

class Socket
{
 public:
  Socket();
  virtual ~Socket();

  // Server initialization
  bool create();
  bool bind ( const int port );
  bool listen() const;
  bool accept ( Socket& ) const;

  // Client initialization
  bool connect ( const std::string host, const int port );

  // Data Transimission
  bool send ( const std::string ) const;
  bool send ( const char* s, int len) const; // to send raw data
  int recv ( std::string& ) const;

  bool is_valid() const { return m_sock != INVALID_SOCKET; }

  std::string getIp() const;
  int getPort() const;

 private:

  #ifdef __linux__
    int m_sock;
    sockaddr_in m_addr;
  #endif

  #ifdef WIN32
    SOCKET m_sock;
    sockaddr_in m_addr;
  #endif
};


#endif
