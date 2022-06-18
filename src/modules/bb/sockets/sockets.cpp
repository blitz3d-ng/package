
#include "../../stdutil/stdutil.h"
#include "sockets.h"
#include <bb/runtime/runtime.h>
#include <bb/system/system.h>

#include <set>
#include <vector>
#include <cstring>
using namespace std;

#ifndef BB_WINDOWS
#define FIXME

#include <netdb.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/ioctl.h>

typedef int SOCKET;
typedef hostent HOSTENT;
#else
typedef int socklen_t;
#endif

#ifndef FIXME
#include <windows.h>
#endif

static bool socks_ok=0;
#ifndef FIXME
static WSADATA wsadata;
#endif
static int recv_timeout;
static int read_timeout;
static int accept_timeout;

static void close( SOCKET sock,int e ){
#ifndef FIXME
	if( e<0 ){
		int opt=1;
		setsockopt( sock,SOL_SOCKET,SO_DONTLINGER,(char*)&opt,sizeof(opt) );
	}
	closesocket( sock );
#endif
}

class UDPStream;
class TCPStream;
class TCPServer;

static set<UDPStream*> udp_set;
static set<TCPStream*> tcp_set;
static set<TCPServer*> server_set;

class UDPStream : public bbStream{
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
	SOCKET sock;
	vector<char> in_buf,out_buf;
	sockaddr_in addr,in_addr,out_addr;
	int in_get,e;
};

UDPStream::UDPStream( SOCKET s ):sock(s),in_get(0),e(0){
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
#ifndef FIXME
	if( e ) return 0;
	int tout;
	if( recv_timeout ) tout=bbMilliSecs()+recv_timeout;
	for(;;){
		int dt=0;
		if( recv_timeout ){
			dt=tout-bbMilliSecs();
			if( dt<0 ) dt=0;
		}
		fd_set fd={ 1,sock };
		timeval tv={ dt/1000,(dt%1000)*1000 };
		int n=::select( 0,&fd,0,0,&tv );
		if( !n ) return 0;
		if( n!=1 ){ e=-1;return 0; }
		unsigned long sz=-1;
		if( ioctlsocket( sock,FIONREAD,&sz ) ){ e=-1;return 0; }
		in_buf.resize( sz );in_get=0;
		int len=sizeof(in_addr);
		n=::recvfrom( sock,&in_buf[0],sz,0,(sockaddr*)&in_addr,&len );
		if( n==SOCKET_ERROR ) continue;	//{ e=-1;return 0; }
		in_buf.resize( n );
		return getMsgIP();
	}
#endif
	return 0;
}

//send, empty buffer
int UDPStream::send( int ip,int port ){
#ifdef FIXME
	return 0;
#else
	if( e ) return 0;
	int sz=out_buf.size();
	out_addr.sin_addr.S_un.S_addr=htonl( ip );
	out_addr.sin_port=htons( port ? port : addr.sin_port );
	int n=::sendto( sock,&out_buf[0],sz,0,(sockaddr*)&out_addr,sizeof(out_addr) );
	if( n!=sz ) return e=-1;
	out_buf.clear();
	return sz;
#endif
}

int UDPStream::getIP(){
#ifdef FIXME
	return 0;
#else
	return ntohl( addr.sin_addr.S_un.S_addr );
#endif
}

int UDPStream::getPort(){
#ifdef FIXME
	return 0;
#else
	return ntohs( addr.sin_port );
#endif
}

int UDPStream::getMsgIP(){
#ifdef FIXME
	return 0;
#else
	return ntohl( in_addr.sin_addr.S_un.S_addr );
#endif
}

int UDPStream::getMsgPort(){
#ifdef FIXME
	return 0;
#else
	return ntohs( in_addr.sin_port );
#endif
}

class TCPStream : public bbStream{
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
	SOCKET sock;
	TCPServer *server;
	int e,ip,port;
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
	set<TCPStream*> accepted_set;
};

TCPStream::TCPStream( SOCKET s,TCPServer *t ):sock(s),server(t),e(0){
#ifndef FIXME
	sockaddr_in addr;
	int len=sizeof(addr);
	if( getpeername( s,(sockaddr*)&addr,&len ) ){
		ip=port=0;
		return;
	}
	ip=ntohl(addr.sin_addr.S_un.S_addr);
	port=ntohs(addr.sin_port);
#endif
}

TCPStream::~TCPStream(){
	if( server ) server->remove( this );
#ifndef FIXME
	close( sock,e );
#endif
}

int TCPStream::read( char *buff,int size ){
#ifdef FIXME
	return 0;
#else
	if( e ) return 0;
	char *b=buff,*l=buff+size;
	int tout;
	if( read_timeout ) tout=bbMilliSecs()+read_timeout;
	while( b<l ){
		int dt=0;
		if( read_timeout ){
			dt=tout-bbMilliSecs();
			if( dt<0 ) dt=0;
		}
		fd_set fd={ 1,sock };
		timeval tv={ dt/1000,(dt%1000)*1000 };
		int n=::select( 0,&fd,0,0,&tv );
		if( n!=1 ){ e=-1;break; }
		n=::recv( sock,b,l-b,0 );
		if( n==0 ){ e=1;break; }
		if( n==SOCKET_ERROR ){ e=-1;break; }
		b+=n;
	}
	return b-buff;
#endif
}

int TCPStream::write( const char *buff,int size ){
#ifdef FIXME
	return 0;
#else
	if( e ) return 0;
	int n=::send( sock,buff,size,0 );
	if( n==SOCKET_ERROR ){ e=-1;return 0; }
	return n;
#endif
}

int TCPStream::avail(){
#ifdef FIXME
	return 0;
#else
	unsigned long t;
	int n=::ioctlsocket( sock,FIONREAD,&t );
	if( n==SOCKET_ERROR ){ e=-1;return 0; }
	return t;
#endif
}

int TCPStream::eof(){
	if( e ) return e;
	fd_set fd={ 1,sock };
	timeval tv={ 0,0 };
	switch( ::select( 0,&fd,0,0,&tv ) ){
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
	while( accepted_set.size() ) delete *accepted_set.begin();
	close( sock,e );
}

TCPStream *TCPServer::accept(){
#ifdef FIXME
	return 0;
#else
	if( e ) return 0;
	fd_set fd={ 1,sock };
	timeval tv={ accept_timeout/1000,(accept_timeout%1000)*1000 };
	int n=::select( 0,&fd,0,0,&tv );
	if( n==0 ) return 0;
	if( n!=1 ){ e=-1;return 0; }
	SOCKET t=::accept( sock,0,0 );
	if( t==INVALID_SOCKET ){ e=-1;return 0; }
	TCPStream *s=d_new TCPStream( t,this );
	accepted_set.insert( s );
	return s;
#endif
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

static vector<int> host_ips;

bb_int_t BBCALL bbCountHostIPs( BBStr *host ){
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

UDPStream * BBCALL bbCreateUDPStream( bb_int_t port ){
#ifndef FIXME
	if( !socks_ok ) return 0;
	SOCKET s=::socket( AF_INET,SOCK_DGRAM,0 );
	if( s!=INVALID_SOCKET ){
		sockaddr_in addr={AF_INET,htons(port)};
		if( !::bind( s,(sockaddr*)&addr,sizeof(addr) ) ){
			UDPStream *p=d_new UDPStream( s );
			udp_set.insert( p );
			return p;
		}
		::closesocket( s );
	}
#endif
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

#ifndef FIXME
static int findHostIP( const string &t ){
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
#endif

TCPStream * BBCALL bbOpenTCPStream( BBStr *server,bb_int_t port,bb_int_t local_port ){
#ifndef FIXME
	if( !socks_ok ){
		delete server;
		return 0;
	}
	int ip=findHostIP( *server );delete server;
	if( ip==-1 ) return 0;
	SOCKET s=::socket( AF_INET,SOCK_STREAM,0 );
	if( s!=INVALID_SOCKET ){
		if( local_port ){
			sockaddr_in addr={AF_INET,htons(local_port)};
			if( ::bind( s,(sockaddr*)&addr,sizeof(addr) ) ){
				::closesocket( s );
				return 0;
			}
		}
		sockaddr_in addr={AF_INET,htons(port)};
		addr.sin_addr.S_un.S_addr=ip;
		if( !::connect( s,(sockaddr*)&addr,sizeof(addr) ) ){
			TCPStream *p=d_new TCPStream( s,0 );
			tcp_set.insert( p );
			return p;
		}
		::closesocket( s );
	}
#endif
	return 0;
}

void BBCALL bbCloseTCPStream( TCPStream *p ){
	debugTCPStream( p );
	tcp_set.erase( p );
	delete p;
}

TCPServer * BBCALL bbCreateTCPServer( bb_int_t port ){
#ifndef FIXME
	SOCKET s=::socket( AF_INET,SOCK_STREAM,0 );
	if( s!=INVALID_SOCKET ){
		sockaddr_in addr={AF_INET,htons(port)};
		if( !::bind( s,(sockaddr*)&addr,sizeof(addr) ) ){
			if( !::listen( s,SOMAXCONN ) ){
				TCPServer *p=d_new TCPServer( s );
				server_set.insert( p );
				return p;
			}
		}
		::closesocket(s);
	}
#endif
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
#ifndef FIXME
	socks_ok=WSAStartup( 0x0101,&wsadata )==0;
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
#ifndef FIXME
	if( socks_ok ) WSACleanup();
#endif
	return true;
}
