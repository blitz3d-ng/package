

#include <bb/blitz/blitz.h>
#include "../../stdutil/stdutil.h"
#include "bank.h"

#include <cstring>
#include <set>
using namespace std;

bbBank::bbBank( int sz ):size(sz){
	capacity=(size+15)&~15;
	data=d_new char[capacity];
	memset( data,0,size );
}

bbBank::~bbBank(){
	delete[] data;
}

void bbBank::resize( int n ){
	if( n>size ){
		if( n>capacity ){
			capacity=capacity*3/2;
			if( n>capacity ) capacity=n;
			capacity=(capacity+15)&~15;
			char *p=d_new char[capacity];
			memcpy( p,data,size );
			delete[] data;
			data=p;
		}else memset( data+size,0,n-size );
	}
	size=n;
}

static set<bbBank*> bank_set;

void debugBank( bbBank *b ){
	if( bb_env.debug ){
		if( !bank_set.count( b ) ) RTEX( "bbBank does not exist" );
	}
}

void debugBank( bbBank *b,int offset ){
	if( bb_env.debug ){
		debugBank( b );
		if( offset>=b->size ) RTEX( "Offset out of range" );
	}
}

bbBank * BBCALL bbCreateBank( int size ){
	bbBank *b=d_new bbBank( size );
	bank_set.insert( b );
	return b;
}

void BBCALL bbFreeBank( bbBank *b ){
	if( bank_set.erase( b ) ) delete b;
}

int BBCALL bbBankSize( bbBank *b ){
	debugBank( b );
	return b->size;
}

void BBCALL bbResizeBank( bbBank *b,int size ){
	debugBank( b );
	b->resize( size );
}

void BBCALL bbCopyBank( bbBank *src,int src_p,bbBank *dest,int dest_p,int count ){
	if( bb_env.debug ){ debugBank( src,src_p+count-1 );debugBank( dest,dest_p+count-1 ); }
	memmove( dest->data+dest_p,src->data+src_p,count );
}

int BBCALL bbPeekByte( bbBank *b,int offset ){
	debugBank( b,offset );
	return *(unsigned char*)(b->data+offset);
}

int BBCALL bbPeekShort( bbBank *b,int offset ){
	debugBank( b,offset+1 );
	return *(unsigned short*)(b->data+offset);
}

int BBCALL bbPeekInt( bbBank *b,int offset ){
	debugBank( b,offset+3 );
	return *(int*)(b->data+offset);
}

float BBCALL bbPeekFloat( bbBank *b,int offset ){
	debugBank( b,offset+3 );
	return *(float*)(b->data+offset);
}

void BBCALL bbPokeByte( bbBank *b,int offset,int value ){
	debugBank( b,offset );
	*(char*)(b->data+offset)=value;
}

void BBCALL bbPokeShort( bbBank *b,int offset,int value ){
	debugBank( b,offset );
	*(unsigned short*)(b->data+offset)=value;
}

void BBCALL bbPokeInt( bbBank *b,int offset,int value ){
	debugBank( b,offset );
	*(int*)(b->data+offset)=value;
}

void BBCALL bbPokeFloat( bbBank *b,int offset,float value ){
	debugBank( b,offset );
	*(float*)(b->data+offset)=value;
}

int BBCALL  bbReadBytes( bbBank *b,bbStream *s,int offset,int count ){
	if( bb_env.debug ){
		debugBank( b,offset+count-1 );
		debugStream( s );
	}
	return s->read( b->data+offset,count );
}

int BBCALL  bbWriteBytes( bbBank *b,bbStream *s,int offset,int count ){
	if( bb_env.debug ){
		debugBank( b,offset+count-1 );
		debugStream( s );
	}
	return s->write( b->data+offset,count );
}

BBMODULE_CREATE( bank ){
	return true;
}

BBMODULE_DESTROY( bank ){
	while( bank_set.size() ) bbFreeBank( *bank_set.begin() );
	return true;
}
