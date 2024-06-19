// Definition of the ClientSocket class

#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include "Socket.h"


class ClientSocket : private Socket
{
 public:

  ClientSocket ( std::string host, int port );

  const ClientSocket& operator << ( const std::string& ) const;
  const ClientSocket& operator >> ( std::string& ) const;

  bool send ( const std::string s ) const;
  bool send ( const char* s, int len) const;
  int recv ( std::string& s ) const;
};


#endif
