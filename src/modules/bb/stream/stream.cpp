
#include <set>

#include "../stdutil/stdutil.h"
#include <bb/blitz/blitz.h>
#include "stream.h"

using namespace std;

static set<bbStream*> stream_set;

void debugStream( bbStream *s ){
	if( stream_set.count(s) ) return;
	RTEX( "Stream does not exist" );
}

bbStream::bbStream(){
	stream_set.insert( this );
}

bbStream::~bbStream(){
	stream_set.erase( this );
}

bb_int_t BBCALL bbEof( bbStream *s ){
	if( bb_env.debug ) debugStream( s );
	return s->eof();
}

bb_int_t BBCALL bbReadAvail( bbStream *s ){
	if( bb_env.debug ) debugStream( s );
	return s->avail();
}

bb_int_t BBCALL bbReadByte( bbStream *s ){
	if( bb_env.debug ) debugStream( s );
	int n=0;
	s->read( (char*)&n,1 );
	return n;
}

bb_int_t BBCALL bbReadShort( bbStream *s ){
	if( bb_env.debug ) debugStream( s );
	int n=0;
	s->read( (char*)&n,2 );
	return n;
}

bb_int_t BBCALL bbReadInt( bbStream *s ){
	if( bb_env.debug ) debugStream( s );
	int n=0;
	s->read( (char*)&n,4 );
	return n;
}

bb_float_t BBCALL bbReadFloat( bbStream *s ){
	if( bb_env.debug ) debugStream( s );
	float n=0;
	s->read( (char*)&n,4 );
	return n;
}

BBStr * BBCALL bbReadString( bbStream *s ){
	if( bb_env.debug ) debugStream( s );
	int len;
	BBStr *str=d_new BBStr();
	if( s->read( (char*)&len,4 ) ){
		char *buff=d_new char[len];
		if( s->read( buff,len ) ){
			*str=string( buff,len );
		}
		delete[] buff;
	}
	return str;
}

BBStr * BBCALL bbReadLine( bbStream *s ){
	if( bb_env.debug ) debugStream( s );
	unsigned char c;
	BBStr *str=d_new BBStr();
	for(;;){
		if( s->read( (char*)&c,1 )!=1 ) break;
		if( c=='\n' ) break;
		if( c!='\r' ) *str+=c;
	}
	return str;
}

void BBCALL bbWriteByte( bbStream *s,bb_int_t n ){
	if( bb_env.debug ) debugStream( s );
	s->write( (char*)&n,1 );
}

void BBCALL bbWriteShort( bbStream *s,bb_int_t n ){
	if( bb_env.debug ) debugStream( s );
	s->write( (char*)&n,2 );
}

void BBCALL bbWriteInt( bbStream *s,bb_int_t n ){
	if( bb_env.debug ) debugStream( s );
	s->write( (char*)&n,4 );
}

void BBCALL bbWriteFloat( bbStream *s,bb_float_t n ){
	if( bb_env.debug ) debugStream( s );
	s->write( (char*)&n,4 );
}

void BBCALL bbWriteString( bbStream *s,BBStr *t ){
	if( bb_env.debug ) debugStream( s );
	int n=t->size();
	s->write( (char*)&n,4 );
	s->write( t->data(),t->size() );
	delete t;
}

void BBCALL bbWriteLine( bbStream *s,BBStr *t ){
	if( bb_env.debug ) debugStream( s );
	s->write( t->data(),t->size() );
	s->write( "\r\n",2 );
	delete t;
}

void BBCALL bbCopyStream( bbStream *s,bbStream *d,bb_int_t buff_size ){
	if( bb_env.debug ){
		debugStream( s );debugStream( d );
		if( buff_size<1 || buff_size>1024*1024 ) RTEX( "Illegal buffer size" );
	}
	char *buff=d_new char[buff_size];
	while( s->eof()==0 && d->eof()==0 ){
		int n=s->read( buff,buff_size );
		d->write( buff,n );
		if( n<buff_size ) break;
	}
	delete[] buff;
}

BBMODULE_CREATE( stream ){
	return true;
}

BBMODULE_DESTROY( stream ){
	return true;
}
