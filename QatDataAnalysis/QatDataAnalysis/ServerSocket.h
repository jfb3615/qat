// -----------------------------------------------------------------------
// File adapted by Joe Boudreau from original example described in Linux
// Gazette by Rob Tougher, http://tldp.org/LDP/LG/issue74/tougher.html
// (2002).  The example was updated in 2013 to use standard exceptions and was
// endowed with a template method to allow exchange of arbitrary data
// structures via sockets.  
//-------------------------------------------------------------------------

// Definition of the ServerSocket class

#ifndef ServerSocket_class
#define ServerSocket_class

#include "QatDataAnalysis/Socket.h"

class ServerSocket : private Socket
{
 public:

  ServerSocket ( int port );
  ServerSocket (){};
  virtual ~ServerSocket();
  
  template <class T> const ServerSocket& operator << ( const T & query) const;
  template <class T> const ServerSocket& operator >> ( T & query) const;

  void accept ( ServerSocket& );

};

#include "QatDataAnalysis/ServerSocket.icc"

#endif
