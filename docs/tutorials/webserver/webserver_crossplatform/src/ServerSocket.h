// Definition of the ServerSocket class

#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include "Socket.h"


class ServerSocket : private Socket
{
 public:

  ServerSocket ( int port );
  ServerSocket (){};

  const ServerSocket& operator << ( const std::string& ) const;
  const ServerSocket& operator >> ( std::string& ) const;

  bool send ( const std::string s ) const;
  bool send ( const char* s, int len) const;
  int recv ( std::string& s ) const;

  void accept ( ServerSocket& );

  std::string getIp() const;
  int getPort() const;
};


#endif
