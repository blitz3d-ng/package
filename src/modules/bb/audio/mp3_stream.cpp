#define MINIMP3_IMPLEMENTATION
#define MINIMP3_NO_STDIO
#include "mp3_stream.h"
#include <string.h>
#include <bb/blitz/app.h>

#define BITS 16

static bool mp3d_init=false;
static mp3dec_t mp3d;

size_t MP3AudioStream::read_cb( void *buf,size_t size,void *user_data ){
	// LOGD("read_cb: %li", size);
	MP3AudioStream *stream=(MP3AudioStream*)user_data;
	size_t start=stream->in.tellg();
	stream->in.read( (char*)buf,size );
	size_t s=stream->in.gcount();
	stream->in.clear(); // needed because reading past eof causing failbit!?
	return s;
}

int MP3AudioStream::seek_cb( uint64_t position,void *user_data ){
	// LOGD("seek_cb: %llu", position);
	MP3AudioStream *stream=(MP3AudioStream*)user_data;
	stream->in.seekg( position,std::ios_base::beg ).gcount();
	return 0;
}


MP3AudioStream::MP3AudioStream( int buf_size ):AudioStream( buf_size ){
	if( !mp3d_init ){
		mp3dec_init( &mp3d );
		mp3d_init=true;
	}
}

bool MP3AudioStream::readHeader(){
	io=mp3dec_io_t{
		read_cb,
		this,
		seek_cb,
		this,
	};

	if( mp3dec_ex_open_cb(&dec, &io, MP3D_SEEK_TO_SAMPLE) ){
		return false;
	}

	if( dec.samples==0 ){
		return false;
	}

	start=dec.offset;

	samples=dec.samples;
	bits=BITS;
	channels=dec.info.channels;
	frequency=dec.info.hz;

	// LOGD("frame_bytes=%i",dec.info.frame_bytes);
	// LOGD("bitrate_kbps=%i",dec.info.bitrate_kbps);
	// LOGD("layer=%i",dec.info.layer);
	// LOGD("samples=%i, channels=%i, frequency=%i",samples,channels,frequency);

	return true;
}

void MP3AudioStream::seek( long pos ){
	// LOGD("%s %li", "MP3AudioStream::seek", pos);
	mp3dec_ex_seek( &dec,pos );
}

long MP3AudioStream::pos(){
	// LOGD("%s %lu", "MP3AudioStream::pos",dec.offset);
	return dec.cur_sample;
}

size_t MP3AudioStream::decode(){
	memset( buf,0,buf_size );
	size_t samples=mp3dec_ex_read( &dec,(mp3d_sample_t*)buf,buf_size/sizeof(mp3d_sample_t) );
	return samples*sizeof( mp3d_sample_t );
}
