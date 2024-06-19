// Implementation of the ServerSocket class

#include "ServerSocket.h"
#include "SocketException.h"


ServerSocket::ServerSocket ( int port )
{
  if ( ! Socket::create() )
  {
    throw SocketException ( "Could not create server socket." );
  }

  if ( ! Socket::bind ( port ) )
  {
    throw SocketException ("Could not bind to port. Check if the port is free. You may also need admin rights.");
  }

  if ( ! Socket::listen() )
  {
    throw SocketException ( "Could not listen to socket." );
  }

}

void ServerSocket::accept ( ServerSocket& sock )
{
  if ( ! Socket::accept ( sock ) )
  {
    throw SocketException ( "Could not accept socket." );
  }
}


const ServerSocket& ServerSocket::operator << ( const std::string& s ) const
{
  if ( ! Socket::send ( s ) )
  {
    throw SocketException ( "Could not write to socket." );
  }

  return *this;
}


const ServerSocket& ServerSocket::operator >> ( std::string& s ) const
{
  if ( ! Socket::recv ( s ) )
  {
    throw SocketException ( "Could not read from socket." );
  }

  return *this;
}

bool ServerSocket::send ( const std::string s ) const
{
  bool success = Socket::send ( s );
  if ( ! success )
  {
    throw SocketException ( "Could not write to socket." );
  }
  return success;
}

bool ServerSocket::send ( const char* s, int len) const
{
  bool success = Socket::send ( s, len );
  if ( ! success )
  {
    throw SocketException ( "Could not write to socket." );
  }
  return success;
}

int ServerSocket::recv ( std::string& s ) const
{
  int bytes = Socket::recv ( s );
  if ( bytes == 0 )
  {
    throw SocketException ( "Could not read from socket." );
  }
  return bytes;
}

std::string ServerSocket::getIp() const
{
  return Socket::getIp();
}

int ServerSocket::getPort() const
{
  return Socket::getPort();
}


