#ifndef BB_AUDIO_WAV_STREAM_H
#define BB_AUDIO_WAV_STREAM_H

#include "stream.h"

class WAVAudioStream : public AudioStream{
public:
	WAVAudioStream( int buf_size);
	bool readHeader();

	void seek( long pos );
	long pos();
	size_t decode();
};


#endif
