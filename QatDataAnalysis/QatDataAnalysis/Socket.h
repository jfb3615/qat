// -----------------------------------------------------------------------
// File adapted by Joe Boudreau from original example described in Linux
// Gazette by Rob Tougher, http://tldp.org/LDP/LG/issue74/tougher.html
// (2002).  The example was updated in 2013 to use standard exceptions and was
// endowed with a template method to allow exchange of arbitrary data
// structures via sockets.  
//-------------------------------------------------------------------------

// Definition of the Socket class

#ifndef Socket_class
#define Socket_class


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

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
  int recv ( std::string& ) const;

  template<class T> bool send ( const T & query ) const;
  template<class T> bool recv ( T & query) const;


  void set_non_blocking ( const bool );

  bool is_valid() const { return m_sock != -1; }

 private:

  int m_sock;
  sockaddr_in m_addr;


};

#include "QatDataAnalysis/Socket.icc"

#endif
