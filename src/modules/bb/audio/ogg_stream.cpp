
#include "ogg_stream.h"

#define BITS 16
#define SIGN 1
#define ENDIAN 0

size_t OGGAudioStream::oread( void *ptr, size_t size, size_t nmemb, void *datasource ){
	OGGAudioStream *stream=(OGGAudioStream*)datasource;
	size_t start=stream->in.tellg();
	stream->in.read( (char*)ptr,size*nmemb );
	size_t s=stream->in.gcount();
	stream->in.clear(); // needed because reading past eof causing failbit!?
	return s;
}

int OGGAudioStream::oseek( void *datasource, ogg_int64_t offset, int whence ){
	OGGAudioStream *stream=(OGGAudioStream*)datasource;
	std::ios_base::seekdir way=whence==SEEK_SET?(std::ios_base::beg):(whence==SEEK_CUR?(std::ios_base::cur):std::ios_base::end);
	return stream->in.seekg( offset,way ).gcount();
}

int OGGAudioStream::oclose( void *datasource ){
	return 0;
}

long OGGAudioStream::otell( void *datasource ){
	OGGAudioStream *stream=(OGGAudioStream*)datasource;
	return stream->in.tellg();
}

OGGAudioStream::OGGAudioStream( int buf_size ):AudioStream(buf_size){
	bits=BITS;
}

bool OGGAudioStream::readHeader(){
	callbacks.read_func=oread;
	callbacks.seek_func=oseek;
	callbacks.close_func=oclose;
	callbacks.tell_func=otell;

	if ( ov_open_callbacks( this,&vfile,0,0,callbacks )<0 ){
		return false;
	}

	samples=ov_pcm_total( &vfile,0 );
	channels=ov_info( &vfile,-1 )->channels;
	frequency=ov_info( &vfile,-1 )->rate;

	return true;
}

void OGGAudioStream::seek( long pos ){
	ov_pcm_seek( &vfile,pos );
}

long OGGAudioStream::pos(){
	return ov_pcm_tell( &vfile );
}

size_t OGGAudioStream::decode(){
	int res,bs,bytes=0;
	while( bytes<buf_size ){
		res=ov_read( &vfile,(char *)buf+bytes,buf_size-bytes,ENDIAN,bits/8,SIGN,&bs );
		if( res==0 ) break; // eof
		if( res<0 ) continue;

		bytes+=res;
	}
	return bytes;
}
