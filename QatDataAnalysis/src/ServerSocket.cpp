// -----------------------------------------------------------------------
// File adapted by Joe Boudreau from original example described in Linux
// Gazette by Rob Tougher, http://tldp.org/LDP/LG/issue74/tougher.html
// (2002).  The example was updated in 2013 to use standard exceptions and was
// endowed with a template method to allow exchange of arbitrary data
// structures via sockets.  
//-------------------------------------------------------------------------

// Implementation of the ServerSocket class

#include "QatDataAnalysis/ServerSocket.h"


ServerSocket::ServerSocket ( int port )
{
  if ( ! Socket::create() )
    {
      throw std::runtime_error ( "ServerSocket:  Could not create server socket." );
    }

  if ( ! Socket::bind ( port ) )
    {
      throw std::runtime_error ( "ServerSocket:  Could not bind to port." );
    }

  if ( ! Socket::listen() )
    {
      throw std::runtime_error ( "ServerSocket:  Could not listen to socket." );
    }

}

ServerSocket::~ServerSocket()
{
}



void ServerSocket::accept ( ServerSocket& sock )
{
  if ( ! Socket::accept ( sock ) )
    {
      throw std::runtime_error ( "ServerSocket:  Could not accept socket." );
    }
}
