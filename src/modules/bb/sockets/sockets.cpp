
#include "../stdutil/stdutil.h"
#include "sockets.h"
#include <bb/runtime/runtime.h>
#include <bb/system/system.h>

#include <set>
#include <vector>
#include <cstring>

#ifndef BB_WINDOWS
#define FIXME

#include <netdb.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#ifndef BB_NX
#include <ifaddrs.h>
#endif

typedef int SOCKET;
typedef hostent HOSTENT;
#define ioctlsocket(fd,flags,ptr) ioctl(fd,flags,ptr)
#define closesocket(fd) close(fd)
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#else
typedef int socklen_t;
#endif

#ifdef WIN32
#include <windows.h>
#include <iphlpapi.h>
#endif

#ifdef BB_NX
#include <switch.h>
#endif

#ifdef WIN32
#define S_ADDR( a ) a.S_un.S_addr
#else
#define S_ADDR( a ) a.s_addr
#endif

static bool socks_ok=0;
#ifdef WIN32
static WSADATA wsadata;
#endif
static int recv_timeout;
static int read_timeout;
static int accept_timeout;

static bool bind( SOCKET s,int port ){
#if defined(BB_MACH) || defined(BB_NX)
	sockaddr_in addr={sizeof(sockaddr_in),AF_INET,htons(port)};
#else
	sockaddr_in addr={AF_INET,htons(port)};
#endif
	return ::bind( s,(sockaddr*)&addr,sizeof(addr) )==0;
}

static int waitForRead( SOCKET s,int ms ){
#ifdef BB_NX
	// TODO: this should be investigated further
	return 1;
#endif
#ifdef WIN32
	fd_set fd={ 1,s };
	timeval tv={ ms/1000,(ms%1000)*1000 };
	int n=::select( 0,&fd,0,0,&tv );
	return n;
#else
	pollfd fd={ s,POLLIN,0 };
	int n=poll( &fd,1,ms );
	if( fd.revents&POLLIN ) return 1;
#endif
	return n;
}

static void close( SOCKET sock,int e ){
	if( e<0 ){
		int opt=1;
#ifndef FIXME
		setsockopt( sock,SOL_SOCKET,SO_DONTLINGER,(char*)&opt,sizeof(opt) );
#endif
	}
	closesocket( sock );
}

class UDPStream;
class TCPStream;
class TCPServer;

static std::vector<int> host_ips;
static std::vector<std::string> iface_names;
static std::vector<std::string> iface_ips;

static std::set<UDPStream*> udp_set;
static std::set<TCPStream*> tcp_set;
static std::set<TCPServer*> server_set;

class SocketStream : public BBStream{
protected:
	SOCKET sock;
	int e;

	SocketStream( SOCKET s );
	int readAvail( size_t *sz );
};

SocketStream::SocketStream( SOCKET s ):sock(s),e(0){
}

int SocketStream::readAvail( size_t *sz ){
#ifdef WIN32
	u_long t=-1;
#else
	int t=-1;
#endif
	int n=::ioctlsocket( sock,FIONREAD,&t );
	*sz=t;
	return n;
}

class UDPStream : public SocketStream{
public:
	UDPStream( SOCKET s );
	~UDPStream();

	int read( char *buff,int size );
	int write( const char *buff,int size );
	int avail();
	int eof();

	int recv();
	int send( int ip,int port );
	int getIP();
	int getPort();
	int getMsgIP();
	int getMsgPort();

private:
	std::vector<char> in_buf,out_buf;
	sockaddr_in addr,in_addr,out_addr;
	int in_get;
};

UDPStream::UDPStream( SOCKET s ):SocketStream(s),in_get(0){
	socklen_t len=sizeof(addr);
	getsockname( s,(sockaddr*)&addr,&len );
	in_addr=out_addr=addr;
}

UDPStream::~UDPStream(){
	close( sock,e );
}

int UDPStream::read( char *buff,int size ){
	if( e ) return 0;
	int n=in_buf.size()-in_get;
	if( n<size ) size=n;
	memcpy( buff,&in_buf[in_get],size );
	in_get+=size;
	return size;
}

int UDPStream::write( const char *buff,int size ){
	if( e ) return 0;
	out_buf.insert( out_buf.end(),buff,buff+size );
	return size;
}

int UDPStream::avail(){
	if( e ) return 0;
	return in_buf.size()-in_get;
}

int UDPStream::eof(){
	return e ? e : in_get==in_buf.size();
}

//fill buffer, return sender
int UDPStream::recv(){
	if( e ) return 0;
	int tout;
	if( recv_timeout ) tout=bbMilliSecs()+recv_timeout;
	for(;;){
		int dt=0;
		if( recv_timeout ){
			dt=tout-bbMilliSecs();
			if( dt<0 ) dt=0;
		}
		int n=waitForRead( sock,dt );
		if( !n ) return 0;
		if( n!=1 ){ e=-1;return 0; }
		size_t sz=-1;
		if( readAvail( &sz ) ){ e=-1;return 0; }
		in_buf.resize( sz );in_get=0;
		socklen_t len=sizeof(in_addr);
		n=::recvfrom( sock,&in_buf[0],sz,0,(sockaddr*)&in_addr,&len );
		if( n==SOCKET_ERROR ) continue;	//{ e=-1;return 0; }
		in_buf.resize( n );
		return getMsgIP();
	}
	return 0;
}

//send, empty buffer
int UDPStream::send( int ip,int port ){
	if( e ) return 0;
	int sz=out_buf.size();
	S_ADDR(out_addr.sin_addr)=htonl( ip );
	out_addr.sin_port=htons( port ? port : addr.sin_port );
	int n=::sendto( sock,&out_buf[0],sz,0,(sockaddr*)&out_addr,sizeof(out_addr) );
	if( n!=sz ) return e=-1;
	out_buf.clear();
	return sz;
}

int UDPStream::getIP(){
	return ntohl( S_ADDR( addr.sin_addr ) );
}

int UDPStream::getPort(){
	return ntohs( addr.sin_port );
}

int UDPStream::getMsgIP(){
	return ntohl( S_ADDR( in_addr.sin_addr ) );
}

int UDPStream::getMsgPort(){
	return ntohs( in_addr.sin_port );
}

class TCPStream : public SocketStream{
public:
	TCPStream( SOCKET s,TCPServer *t );
	~TCPStream();

	int read( char *buff,int size );
	int write( const char *buff,int size );
	int avail();
	int eof();

	int getIP();
	int getPort();

private:
	TCPServer *server;
	int ip,port;
};

class TCPServer{
public:
	TCPServer( SOCKET S );
	~TCPServer();

	TCPStream *accept();

	void remove( TCPStream *s );

private:
	int e;
	SOCKET sock;
	std::set<TCPStream*> accepted_set;
};

TCPStream::TCPStream( SOCKET s,TCPServer *t ):SocketStream(s),server(t){
	sockaddr_in addr;
	socklen_t len=sizeof(addr);
	if( getpeername( s,(sockaddr*)&addr,&len ) ){
		ip=port=0;
		return;
	}
	ip=ntohl( S_ADDR(addr.sin_addr ) );
	port=ntohs(addr.sin_port);
}

TCPStream::~TCPStream(){
	if( server ) server->remove( this );
	close( sock,e );
}

int TCPStream::read( char *buff,int size ){
	if( e ) return 0;
	char *b=buff,*l=buff+size;
	int n,tout;
	if( read_timeout ) tout=bbMilliSecs()+read_timeout;
	while( b<l ){
		int dt=0;
		if( read_timeout ){
			dt=tout-bbMilliSecs();
			if( dt<0 ) dt=0;
		}
		n=waitForRead( sock,dt );
		if( n!=1 ){ e=-1;break; }
		n=::recv( sock,b,l-b,0 );
		if( n==0 ){ e=1;break; }
		if( n==SOCKET_ERROR ){ e=-1;break; }
		b+=n;
	}
	return b-buff;
}

int TCPStream::write( const char *buff,int size ){
	if( e ) return 0;
	int n=::send( sock,buff,size,0 );
	if( n==SOCKET_ERROR ){ e=-1;return 0; }
	return n;
}

int TCPStream::avail(){
	size_t t=-1;
	int n=readAvail( &t );
	if( n==SOCKET_ERROR ){ e=-1;return 0; }
	return t;
}

int TCPStream::eof(){
	if( e ) return e;
	switch( waitForRead( sock,0 ) ){
	case 0:break;
	case 1:if( !avail() ) e=1;break;
	default:e=-1;
	}
	return e;
}

int TCPStream::getIP(){
	return ip;
}

int TCPStream::getPort(){
	return port;
}

TCPServer::TCPServer( SOCKET s ):sock(s),e(0){
}

TCPServer::~TCPServer(){
	while( accepted_set.size() ) bbCloseTCPStream( *accepted_set.begin() );
	close( sock,e );
}

TCPStream *TCPServer::accept(){
	if( e ) return 0;
#ifndef BB_NX
	int n=waitForRead( sock,accept_timeout );
	if( !n ) return 0;
	if( n!=1 ){ e=-1;return 0; }
#endif
	SOCKET t=::accept( sock,0,0 );
	if( t==INVALID_SOCKET ){ e=-1;return 0; }
	TCPStream *s=d_new TCPStream( t,this );
	accepted_set.insert( s );
	return s;
}

void TCPServer::remove( TCPStream *s ){
	accepted_set.erase( s );
}

static inline void debugUDPStream( UDPStream *p ){
	if( bb_env.debug && !udp_set.count(p) ){
		RTEX( "UDP Stream does not exist" );
	}
}

static inline void debugTCPStream( TCPStream *p ){
	if( bb_env.debug && !tcp_set.count(p) ){
		RTEX( "TCP Stream does not exist" );
	}
}

static inline void debugTCPServer( TCPServer *p ){
	if( bb_env.debug && !server_set.count(p) ){
		RTEX( "TCP Server does not exist" );
	}
}

static inline void debugIface( int index ){
	if( bb_env.debug && (index<1 || index>iface_ips.size()) ){
		RTEX( "Interface index out of range" );
	}
}

bb_int_t BBCALL bbCountHostIPs( BBStr *host ){
#ifndef WIN32
	if( *host=="" ) *host="localhost";
#endif
	host_ips.clear();
	HOSTENT *h=gethostbyname( host->c_str() );
	delete host;if( !h ) return 0;
	char **p=h->h_addr_list;
	while( char *t=*p++ ) host_ips.push_back( ntohl(*(int*)t) );
	return host_ips.size();
}

bb_int_t BBCALL bbHostIP( bb_int_t index ){
	if( bb_env.debug ){
		if( index<1 || index>host_ips.size() ){
			RTEX( "Host index out of range" );
		}
	}
	return host_ips[index-1];
}

bb_int_t BBCALL bbCountNetInterfaces(){
	iface_names.clear();
	iface_ips.clear();

#ifdef BB_NX
	static bool inited=false;
	if( !inited ){
		nifmInitialize( NifmServiceType_System );
		inited=true;
	}

	NifmNetworkProfileData profile;
	nifmGetCurrentNetworkProfile( &profile );

	NifmIpAddressSetting setting=profile.ip_setting_data.ip_address_setting;
	NifmIpV4Address curr_addr=setting.current_addr;

	std::string ip=itoa(curr_addr.addr[0])+"."+itoa(curr_addr.addr[1])+"."+itoa(curr_addr.addr[2])+"."+itoa(curr_addr.addr[3]);
	iface_names.push_back( "nifm" );
	iface_ips.push_back( ip );

#else
#ifdef WIN32
	ULONG sz=sizeof( IP_ADAPTER_INFO );
	IP_ADAPTER_INFO *info=(IP_ADAPTER_INFO*)malloc( sz );
	if( GetAdaptersInfo( info,&sz )==ERROR_BUFFER_OVERFLOW ){
		info=(IP_ADAPTER_INFO*)realloc( info,sz );
		if( GetAdaptersInfo( info,&sz )!=0 ) return 0;
	}
	IP_ADAPTER_INFO *adapter=info;
	while( adapter ){
		if( adapter->Type==MIB_IF_TYPE_LOOPBACK||adapter->Type==MIB_IF_TYPE_ETHERNET||adapter->Type==IF_TYPE_IEEE80211 ){
			iface_ips.push_back( adapter->IpAddressList.IpAddress.String );
			iface_names.push_back( adapter->AdapterName );
		}
		adapter=adapter->Next;
	}
	free( info );
#else
	ifaddrs *ifaddr;
	if( getifaddrs( &ifaddr )==-1 ){
		return -1;
	}

	for( ifaddrs *ifa=ifaddr;ifa!=NULL;ifa=ifa->ifa_next ){
		if( !ifa->ifa_addr ){
			continue;
		}

		char host[NI_MAXHOST];
		if( getnameinfo( ifa->ifa_addr,sizeof(struct sockaddr_in),host,NI_MAXHOST,0,0,NI_NUMERICHOST )==0 ){
			if(
				!(ifa->ifa_flags&IFF_LOOPBACK) &&
				(ifa->ifa_flags&IFF_UP) &&
				(ifa->ifa_flags&IFF_RUNNING) &&
				(
					ifa->ifa_addr->sa_family==AF_INET||
					ifa->ifa_addr->sa_family==AF_INET6
				)
			){
				std::string ip=host;
				if( size_t t=ip.find( "%" ) ){
					ip=ip.substr( 0,t );
				}

				iface_ips.push_back( ip );
				iface_names.push_back( ifa->ifa_name  );
			}
		}
	}

	freeifaddrs( ifaddr );
#endif
#endif

	return iface_ips.size();
}

BBStr * BBCALL bbNetInterfaceName( bb_int_t index ){
	debugIface( index );
	return d_new BBStr( iface_names[index-1] );
}

BBStr * BBCALL bbNetInterfaceIP( bb_int_t index ){
	debugIface( index );
	return d_new BBStr( iface_ips[index-1] );
}

UDPStream * BBCALL bbCreateUDPStream( bb_int_t port ){
	if( !socks_ok ) return 0;
	SOCKET s=::socket( AF_INET,SOCK_DGRAM,0 );
	if( s!=INVALID_SOCKET ){
		if( bind( s,port ) ){
			UDPStream *p=d_new UDPStream( s );
			udp_set.insert( p );
			return p;
		}
		::closesocket( s );
	}
	return 0;
}

void BBCALL bbCloseUDPStream( UDPStream *p ){
	debugUDPStream( p );
	udp_set.erase( p );
	delete p;
}

bb_int_t BBCALL bbRecvUDPMsg( UDPStream *p ){
	debugUDPStream( p );
	return p->recv();
}

void BBCALL bbSendUDPMsg( UDPStream *p,bb_int_t ip,bb_int_t port ){
	debugUDPStream( p );
	p->send( ip,port );
}

bb_int_t BBCALL bbUDPStreamIP( UDPStream *p ){
	debugUDPStream( p );
	return p->getIP();
}

bb_int_t BBCALL bbUDPStreamPort( UDPStream *p ){
	debugUDPStream( p );
	return p->getPort();
}

bb_int_t BBCALL bbUDPMsgIP( UDPStream *p ){
	debugUDPStream( p );
	return p->getMsgIP();
}

bb_int_t BBCALL bbUDPMsgPort( UDPStream *p ){
	debugUDPStream( p );
	return p->getMsgPort();
}

void BBCALL bbUDPTimeouts( bb_int_t rt ){
	recv_timeout=rt;
}

BBStr * BBCALL bbDottedIP( bb_int_t ip ){
	return d_new BBStr(
		itoa((ip>>24)&255)+"."+itoa((ip>>16)&255)+"."+
		itoa((ip>>8)&255)+"."+itoa(ip&255) );
}

static int findHostIP( const std::string &t ){
	int ip=inet_addr( t.c_str() );
	if( ip!=INADDR_NONE ) return ip;
	HOSTENT *h=gethostbyname( t.c_str() );
	if( !h ) return -1;
	char *p;
	for( char **list=h->h_addr_list;(p=*list);++list ){
		return *(int*)p;
	}
	return 0;
}

TCPStream * BBCALL bbOpenTCPStream( BBStr *server,bb_int_t port,bb_int_t local_port ){
	if( !socks_ok ){
		delete server;
		return 0;
	}
	int ip=findHostIP( *server );delete server;
	if( ip==-1 ) return 0;
	SOCKET s=::socket( AF_INET,SOCK_STREAM,0 );
	if( s!=INVALID_SOCKET ){
		if( local_port ){
			if( !bind( s,local_port ) ){
				::closesocket( s );
				return 0;
			}
		}
#if defined(BB_MACH) || defined(BB_NX)
	sockaddr_in addr={sizeof(sockaddr_in),AF_INET,htons(port)};
#else
	sockaddr_in addr={AF_INET,htons(port)};
#endif
		S_ADDR(addr.sin_addr)=ip;
		if( !::connect( s,(sockaddr*)&addr,sizeof(addr) ) ){
			TCPStream *p=d_new TCPStream( s,0 );
			tcp_set.insert( p );
			return p;
		}
		::closesocket( s );
	}
	return 0;
}

void BBCALL bbCloseTCPStream( TCPStream *p ){
	debugTCPStream( p );
	tcp_set.erase( p );
	delete p;
}

TCPServer * BBCALL bbCreateTCPServer( bb_int_t port ){
	SOCKET s=::socket( AF_INET,SOCK_STREAM,0 );
	if( s!=INVALID_SOCKET ){
		if( bind( s,port ) ){
			if( !::listen( s,SOMAXCONN ) ){
				TCPServer *p=d_new TCPServer( s );
				server_set.insert( p );
				return p;
			}
		}
		::closesocket(s);
	}
	return 0;
}

void BBCALL bbCloseTCPServer( TCPServer *p ){
	debugTCPServer( p );
	server_set.erase( p );
	delete p;
}

TCPStream * BBCALL bbAcceptTCPStream( TCPServer *server ){
	debugTCPServer( server );
	if( !bbRuntimeIdle() ) RTEX( 0 );
	if( TCPStream *tcp=server->accept() ){
		tcp_set.insert( tcp );
		return tcp;
	}
	return 0;
}

bb_int_t BBCALL bbTCPStreamIP( TCPStream *p ){
	debugTCPStream( p );
	return p->getIP();
}

bb_int_t BBCALL bbTCPStreamPort( TCPStream *p ){
	debugTCPStream( p );
	return p->getPort();
}

void BBCALL bbTCPTimeouts( bb_int_t rt,bb_int_t at ){
	read_timeout=rt;
	accept_timeout=at;
}

BBMODULE_CREATE( sockets ){
#ifdef BB_NX
	socketInitializeDefault();
#endif
#ifdef WIN32
	socks_ok=WSAStartup( 0x0101,&wsadata )==0;
#else
	socks_ok=true;
#endif
	recv_timeout=0;
	read_timeout=10000;
	accept_timeout=0;
	return true;
}

BBMODULE_DESTROY( sockets ){
	while( udp_set.size() ) bbCloseUDPStream( *udp_set.begin() );
	while( tcp_set.size() ) bbCloseTCPStream( *tcp_set.begin() );
	while( server_set.size() ) bbCloseTCPServer( *server_set.begin() );
#ifdef WIN32
	if( socks_ok ) WSACleanup();
#endif
	return true;
}
