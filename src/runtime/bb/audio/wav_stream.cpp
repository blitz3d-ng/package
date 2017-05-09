
#include "wav_stream.h"

WAVAudioStream::WAVAudioStream( int buf_size):AudioStream( buf_size){
}

bool WAVAudioStream::readHeader(){
	read( buf, 12 );

	// format tag
	read( buf,8 );
	if( buf[0]!='f' || buf[1]!='m' || buf[2]!='t' || buf[3]!=' ' ){
		return false;
	}

	read( buf,2 );
	if( buf[1]!=0 || buf[0]!=1 ){
		return false;
	}

	// channel count
	read( buf,2 );
	channels  = buf[1]<<8;
	channels |= buf[0];

	// sample frequency
	read( buf,4 );
	frequency  = buf[3]<<24;
	frequency |= buf[2]<<16;
	frequency |= buf[1]<<8;
	frequency |= buf[0];

	read( buf,6 );

	read( buf,2 );
	bits  = buf[1]<<8;
	bits |= buf[0];

	read( buf,8 );
	if( buf[0]!='d' || buf[1]!='a' || buf[2]!='t' || buf[3]!='a' ){
		return false;
	}

	return true;
}

void WAVAudioStream::seek( long pos ){
	in.clear();
	in.seekg( pos,std::ios_base::beg );
}

long WAVAudioStream::pos(){
	return in.tellg();
}

size_t WAVAudioStream::decode(){
	return read( buf,buf_size );
}
