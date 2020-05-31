// -----------------------------------------------------------------------
// File adapted by Joe Boudreau from original example described in Linux
// Gazette by Rob Tougher, http://tldp.org/LDP/LG/issue74/tougher.html
// (2002).  The example was updated in 2013 to use standard exceptions and was
// endowed with a template method to allow exchange of arbitrary data
// structures via sockets.  
//-------------------------------------------------------------------------

// Definition of the ClientSocket class

#ifndef ClientSocket_class
#define ClientSocket_class

#include "QatDataAnalysis/Socket.h"

class ClientSocket : private Socket
{
 public:

  ClientSocket ( std::string host, int port );
  virtual ~ClientSocket(){};

  template<class T> const ClientSocket& operator << ( const T & ) const;
  template<class T> const ClientSocket& operator >> ( T & ) const;

};

#include "QatDataAnalysis/ClientSocket.icc"

#endif
