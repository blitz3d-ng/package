#ifndef BB_AUDIO_OGG_STREAM_H
#define BB_AUDIO_OGG_STREAM_H

#include "stream.h"
#include <vorbis/vorbisfile.h>

class OGGAudioStream : public AudioStream{
private:
	OggVorbis_File vfile;
	ov_callbacks callbacks;

public:
	static size_t oread( void *ptr, size_t size, size_t nmemb, void *datasource );
	static int oseek( void *datasource, ogg_int64_t offset, int whence );
	static int oclose( void *datasource );
	static long otell( void *datasource );

	OGGAudioStream( int buf_size );
	bool readHeader();

	void seek( long pos );
	long pos();
	size_t decode();
};



#endif
