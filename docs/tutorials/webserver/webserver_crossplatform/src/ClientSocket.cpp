// Implementation of the ClientSocket class

#include "ClientSocket.h"
#include "SocketException.h"


ClientSocket::ClientSocket ( std::string host, int port )
{
  if ( ! Socket::create() )
  {
    throw SocketException ( "Could not create client socket." );
  }

  if ( ! Socket::connect ( host, port ) )
  {
    throw SocketException ( "Could not bind to port." );
  }
}


const ClientSocket& ClientSocket::operator << ( const std::string& s ) const
{
  if ( ! Socket::send ( s ) )
  {
    throw SocketException ( "Could not write to socket." );
  }

  return *this;

}


const ClientSocket& ClientSocket::operator >> ( std::string& s ) const
{
  if ( ! Socket::recv ( s ) )
  {
    throw SocketException ( "Could not read from socket." );
  }

  return *this;
}


bool ClientSocket::send ( const std::string s ) const
{
  bool success = Socket::send ( s );
  if ( ! success )
  {
    throw SocketException ( "Could not write to socket." );
  }
  return success;
}

bool ClientSocket::send ( const char* s, int len) const
{
  bool success = Socket::send ( s, len );
  if ( ! success )
  {
    throw SocketException ( "Could not write to socket." );
  }
  return success;
}

int ClientSocket::recv ( std::string& s ) const
{
  int bytes = Socket::recv ( s );
  if ( bytes == 0 )
  {
    throw SocketException ( "Could not read from socket." );
  }
  return bytes;
}
