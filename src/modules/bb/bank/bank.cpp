

#include <bb/blitz/blitz.h>
#include "../stdutil/stdutil.h"
#include "bank.h"

#include <cstring>
#include <set>

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

static std::set<bbBank*> bank_set;

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

bbBank * BBCALL bbCreateBank( bb_int_t size ){
	bbBank *b=d_new bbBank( size );
	bank_set.insert( b );
	return b;
}

void BBCALL bbFreeBank( bbBank *b ){
	if( bank_set.erase( b ) ) delete b;
}

bb_int_t BBCALL bbBankSize( bbBank *b ){
	debugBank( b );
	return b->size;
}

void BBCALL bbResizeBank( bbBank *b,bb_int_t size ){
	debugBank( b );
	b->resize( size );
}

void BBCALL bbCopyBank( bbBank *src,bb_int_t src_p,bbBank *dest,bb_int_t dest_p,bb_int_t count ){
	if( bb_env.debug ){ debugBank( src,src_p+count-1 );debugBank( dest,dest_p+count-1 ); }
	memmove( dest->data+dest_p,src->data+src_p,count );
}

bb_int_t BBCALL bbPeekByte( bbBank *b,bb_int_t offset ){
	debugBank( b,offset );
	return *(unsigned char*)(b->data+offset);
}

bb_int_t BBCALL bbPeekShort( bbBank *b,bb_int_t offset ){
	debugBank( b,offset+1 );
	return *(unsigned short*)(b->data+offset);
}

bb_int_t BBCALL bbPeekInt( bbBank *b,bb_int_t offset ){
	debugBank( b,offset+3 );
	return *(int*)(b->data+offset);
}

bb_float_t BBCALL bbPeekFloat( bbBank *b,bb_int_t offset ){
	debugBank( b,offset+3 );
	return *(float*)(b->data+offset);
}

void BBCALL bbPokeByte( bbBank *b,bb_int_t offset,bb_int_t value ){
	debugBank( b,offset );
	*(char*)(b->data+offset)=value;
}

void BBCALL bbPokeShort( bbBank *b,bb_int_t offset,bb_int_t value ){
	debugBank( b,offset );
	*(unsigned short*)(b->data+offset)=value;
}

void BBCALL bbPokeInt( bbBank *b,bb_int_t offset,bb_int_t value ){
	debugBank( b,offset );
	*(int*)(b->data+offset)=value;
}

void BBCALL bbPokeFloat( bbBank *b,bb_int_t offset,bb_float_t value ){
	debugBank( b,offset );
	*(float*)(b->data+offset)=value;
}

bb_int_t BBCALL  bbReadBytes( bbBank *b,BBStream *s,bb_int_t offset,bb_int_t count ){
	if( bb_env.debug ){
		debugBank( b,offset+count-1 );
		debugStream( s );
	}
	return s->read( b->data+offset,count );
}

bb_int_t BBCALL  bbWriteBytes( bbBank *b,BBStream *s,bb_int_t offset,bb_int_t count ){
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
