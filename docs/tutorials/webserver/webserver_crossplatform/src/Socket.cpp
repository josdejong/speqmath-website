// Implementation of the Socket class.


#include "Socket.h"


#include "string.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#ifdef WIN32
static int nofSockets_= 0;

void WsaRegister()
{
  if (!nofSockets_) {
    WSADATA info;
    if (WSAStartup(MAKEWORD(2,0), &info)) {
      throw "Could not start WSA";
    }
  }
  ++nofSockets_;
}

void WsaUnregister()
{
  --nofSockets_;
  if (!nofSockets_)
  {
    WSACleanup();
  }
}
#endif   // WIN32


Socket::Socket()
  : m_sock(INVALID_SOCKET)
{
  memset ( &m_addr, 0, sizeof ( m_addr ) );

  #ifdef WIN32
    WsaRegister();
  #endif
}

Socket::~Socket()
{
  if ( is_valid() )
  {
    #ifdef WIN32
      closesocket(m_sock);
    #else
      ::close ( m_sock );
    #endif
  }

  #ifdef WIN32
    WsaUnregister();
  #endif
}

bool Socket::create()
{
  m_sock = socket ( AF_INET, SOCK_STREAM, 0 );

  if ( ! is_valid() )
  {
    return false;
  }

  // TIME_WAIT - argh
  int on = 1;
  if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
  {
    return false;
  }

  return true;

}



bool Socket::bind ( const int port )
{
  if ( ! is_valid() )
  {
    return false;
  }

  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = INADDR_ANY;
  m_addr.sin_port = htons ( port );

  int bind_return = ::bind ( m_sock,
			     ( struct sockaddr * ) &m_addr,
			     sizeof ( m_addr ) );


  if ( bind_return == -1 )
  {
    return false;
  }
  return true;
}


bool Socket::listen() const
{
  if ( ! is_valid() )
  {
    return false;
  }

  int listen_return = ::listen ( m_sock, MAXCONNECTIONS );

  if ( listen_return == -1 )
  {
    return false;
  }

  return true;
}


bool Socket::accept ( Socket& new_socket ) const
{
  int addr_length = sizeof ( m_addr );
  new_socket.m_sock = ::accept ( m_sock, ( sockaddr * ) &new_socket.m_addr, ( socklen_t * ) &addr_length );

  if ( new_socket.m_sock <= 0 )
    return false;
  else
    return true;
}


bool Socket::send ( const std::string s ) const
{
  int status = ::send ( m_sock, s.c_str(), s.size(), 0 );
  // Documentation: http://linux.die.net/man/2/send
  if ( status == -1 )
  {
    return false;
  }
  else
  {
    return true;
  }
}


bool Socket::send ( const char* s, int len) const
{
  int status = ::send ( m_sock, s, len, 0 );
  if ( status == -1 )
  {
    return false;
  }
  else
  {
    return true;
  }
}


int Socket::recv ( std::string& s ) const
{
  char buf [ MAXRECV + 1 ];

  s = "";

  memset ( buf, 0, MAXRECV + 1 );

  int status = ::recv ( m_sock, buf, MAXRECV, 0 );

  if ( status == -1 )
  {
    return 0;
  }
  else if ( status == 0 )
  {
    return 0;
  }
  else
  {
    s = buf;
    return status;
  }
}



bool Socket::connect ( const std::string host, const int port )
{
  if ( ! is_valid() ) return false;
  int status = 0;

  #ifdef __linux__
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons ( port );

    status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );
    if ( errno == EAFNOSUPPORT ) return false;
  #endif

  #ifdef WIN32
    hostent *he;
    if ((he = gethostbyname(host.c_str())) == 0)
    {
      return false;
    }
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons ( port );
    m_addr.sin_addr = *((in_addr *)he->h_addr);
    memset(&(m_addr.sin_zero), 0, 8);
  #endif

  status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

  if ( status == 0 )
    return true;
  else
    return false;
}



std::string Socket::getIp() const
{
  std::string ip = inet_ntoa(m_addr.sin_addr);
  return ip;
}

int Socket::getPort() const
{
  int port = m_addr.sin_port;
  return port;
}

