
#include "../stdutil/stdutil.h"
#include "stream.h"

#include <cstdlib>


AudioStream::Ref::Ref( AudioStream *s ):stream(s){
	pos=stream->start;
}

AudioStream::Ref::~Ref(){
}

size_t AudioStream::Ref::decode( unsigned char **buf ){
	std::lock_guard<std::mutex> guard( stream->lock );

	stream->seek( pos );
	size_t n=stream->decode();
	pos=stream->pos();
	*buf=stream->buf;
	return n;
}

unsigned int AudioStream::Ref::getChannels(){
	return stream->channels;
}

unsigned int AudioStream::Ref::getBits(){
	return stream->bits;
}

unsigned int AudioStream::Ref::getFrequency(){
	return stream->frequency;
}

unsigned int AudioStream::Ref::getSamples(){
	return stream->samples;
}

bool AudioStream::Ref::eof(){
	return pos==-1;
}

AudioStream::Ref *AudioStream::getRef(){
	return d_new Ref( this );
}


AudioStream::AudioStream( int size ):buf_size(size),channels(0),bits(0),samples(0),frequency(0){
	buf=new unsigned char[buf_size];
}

AudioStream::~AudioStream(){
	in.close();
	delete[] buf;
}

bool AudioStream::init( const char *url ){
	path=url;
	in.open( url,std::ios_base::in|std::ios::binary );
	if( !readHeader() ) return false;
	start=pos();
	return true;
}

size_t AudioStream::read( void *ptr, size_t size ){
	return in.read( (char*)ptr,size ).gcount();
}

bool AudioStream::eof(){
	return in.eof();
}

std::streampos AudioStream::getStart(){
	return start;
}
