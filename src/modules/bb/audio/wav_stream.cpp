
#include "wav_stream.h"
#include <string.h>
#include <bb/blitz/app.h>

WAVAudioStream::WAVAudioStream( int buf_size ):AudioStream( buf_size ){
}

bool WAVAudioStream::readHeader(){
	read( buf, 4 );
	if( buf[0]!='R' || buf[1]!='I' || buf[2]!='F' || buf[3]!='F' ){
		LOGD( "[audio] (%s) missing RIFF",path.c_str() );
		return false;
	}

	read( buf, 4 ); // filesize

	read( buf, 4 );
	if( buf[0]!='W' || buf[1]!='A' || buf[2]!='V' || buf[3]!='E' ){
		LOGD( "[audio] (%s) missing WAVE",path.c_str() );
		return false;
	}

	// format tag
	read( buf,4 );
	if( buf[0]!='f' || buf[1]!='m' || buf[2]!='t' || buf[3]!=' ' ){
		LOGD( "[audio] (%s) missing 'fmt'",path.c_str() );
		return false;
	}

	// length
	read( buf,4 );

	// format
	read( buf,2 );
	if( buf[1]!=0 || buf[0]!=1 ){
		LOGD( "[audio] (%s) unsupported format type %i %i",path.c_str(),buf[0],buf[1] );
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

	// bitrate?
	read( buf,4 );

	// bits per sample
	read( buf,2 );

	read( buf,2 );
	bits  = buf[1]<<8;
	bits |= buf[0];

	read( buf,4 );

	if( strncmp( "LIST",(char*)buf,4 )==0 ){
		read( buf,4 ); // size
		int size=*(int*)buf;
		read( buf,size ); // eat it up

		read( buf,4 ); // cont...
	}

	if( buf[0]!='d' || buf[1]!='a' || buf[2]!='t' || buf[3]!='a' ){
		LOGD( "[audio] (%s) missing 'data' %c%c%c%c",path.c_str(),buf[0],buf[1],buf[2],buf[3] );
		return false;
	}

	// data size
	read( buf,4 );

	start=pos();

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
