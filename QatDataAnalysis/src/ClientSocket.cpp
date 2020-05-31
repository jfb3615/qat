// -----------------------------------------------------------------------
// File adapted by Joe Boudreau from original example described in Linux
// Gazette by Rob Tougher, http://tldp.org/LDP/LG/issue74/tougher.html
// (2002).  The example was updated in 2013 to use standard exceptions and was
// endowed with a template method to allow exchange of arbitrary data
// structures via sockets.  
//-------------------------------------------------------------------------

// Implementation of the ClientSocket class

#include "QatDataAnalysis/ClientSocket.h"


ClientSocket::ClientSocket ( std::string host, int port )
{
  if ( ! Socket::create() )
    {
      throw std::runtime_error ( "ClientSocket: Could not create client socket." );
    }

  if ( ! Socket::connect ( host, port ) )
    {
      throw std::runtime_error ( "ClientSocket: Could not bind to port." );
    }

}





