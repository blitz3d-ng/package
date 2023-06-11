
#include "string.h"
#include "../stdutil/stdutil.h"
#include <string>

#include <time.h>

#define CHKPOS(x) if( (x)<0 ) RTEX( "parameter must be positive" );
#define CHKOFF(x) if( (x)<=0 ) RTEX( "parameter must be greater than 0" );

BBStr * BBCALL bbString( BBStr *s,bb_int_t n ){
	BBStr *t=d_new BBStr();
	while( n-->0 ) *t+=*s;
	delete s;return t;
}

BBStr * BBCALL bbLeft( BBStr *s,bb_int_t n ){
	CHKPOS( n );
	*s=s->substr( 0,n );return s;
}

BBStr * BBCALL bbRight( BBStr *s,bb_int_t n ){
	CHKPOS( n );
	n=s->size()-n;if( n<0 ) n=0;
	*s=s->substr( n );return s;
}

BBStr * BBCALL bbReplace( BBStr *s,BBStr *from,BBStr *to ){
	int n=0,from_sz=from->size(),to_sz=to->size();
	while( n<s->size() && (n=s->find( *from,n ))!=std::string::npos ){
		s->replace( n,from_sz,*to );
		n+=to_sz;
	}
	delete from;delete to;return s;
}

bb_int_t BBCALL bbInstr( BBStr *s,BBStr *t,bb_int_t from ){
	CHKOFF( from );--from;
	int n=s->find( *t,from );
	delete s;delete t;
	return n==std::string::npos ? 0 : n+1;
}

BBStr * BBCALL bbMid( BBStr *s,bb_int_t o,bb_int_t n ){
	CHKOFF( o );--o;
	if( o>s->size() ) o=s->size();
	if( n>=0 ) *s=s->substr( o,n );
	else *s=s->substr( o );
	return s;
}

BBStr * BBCALL bbUpper( BBStr *s ){
	for( int k=0;k<s->size();++k ) (*s)[k]=toupper( (*s)[k] );
	return s;
}

BBStr * BBCALL bbLower( BBStr *s ){
	for( int k=0;k<s->size();++k ) (*s)[k]=tolower( (*s)[k] );
	return s;
}

BBStr * BBCALL bbTrim( BBStr *s ){
	int n=0,p=s->size();
	while( n<s->size() && !isgraph( (*s)[n] ) ) ++n;
	while( p>n && !isgraph( (*s)[p-1] ) ) --p;
	*s=s->substr( n,p-n );return s;
}

BBStr * BBCALL bbLSet( BBStr *s,bb_int_t n ){
	CHKPOS(n);
	if( s->size()>n ) *s=s->substr( 0,n );
	else{
		while( s->size()<n ) *s+=' ';
	}
	return s;
}

BBStr * BBCALL bbRSet( BBStr *s,bb_int_t n ){
	CHKPOS(n);
	if( s->size()>n ) *s=s->substr( s->size()-n );
	else{
		while( s->size()<n ) *s=' '+*s;
	}
	return s;
}

BBStr * BBCALL bbChr( bb_int_t n ){
	BBStr *t=d_new BBStr();
	*t+=(char)n;return t;
}

BBStr * BBCALL bbHex( bb_int_t n ){
	char buff[12];
	for( int k=7;k>=0;n>>=4,--k ){
		int t=(n&15)+'0';
		buff[k]=t>'9' ? t+='A'-'9'-1 : t;
	}
	buff[8]=0;
	return d_new BBStr( buff );
}

BBStr * BBCALL bbBin( bb_int_t n ){
	char buff[36];
	for( int k=31;k>=0;n>>=1,--k ){
		buff[k]=n&1 ? '1' : '0';
	}
	buff[32]=0;
	return d_new BBStr( buff );
}

bb_int_t BBCALL bbAsc( BBStr *s ){
	int n=s->size() ? (*s)[0] & 255 : -1;
	delete s;return n;
}

bb_int_t BBCALL bbLen( BBStr *s ){
	int n=s->size();
	delete s;return n;
}

BBStr * BBCALL bbCurrentDate(){
	time_t t;
	time( &t );
	char buff[256];
	strftime( buff,256,"%d %b %Y",localtime( &t ) );
	return d_new BBStr( buff );
}

BBStr * BBCALL bbCurrentTime(){
	time_t t;
	time( &t );
	char buff[256];
	strftime( buff,256,"%H:%M:%S",localtime( &t ) );
	return d_new BBStr( buff );
}

BBMODULE_CREATE( string ){
	return true;
}

BBMODULE_DESTROY( string ){
	return true;
}
