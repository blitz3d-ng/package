#ifndef BB_AUDIO_MP3_STREAM_H
#define BB_AUDIO_MP3_STREAM_H

#include "stream.h"
#include <minimp3_ex.h>

class MP3AudioStream : public AudioStream{
public:
	MP3AudioStream( int buf_size );
	bool readHeader();

	void seek( long pos );
	long pos();
	size_t decode();

protected:
	mp3dec_io_t io;
	mp3dec_ex_t dec;

	static size_t read_cb( void *buf,size_t size,void *user_data );
	static int seek_cb( uint64_t position,void *user_data );
};


#endif
