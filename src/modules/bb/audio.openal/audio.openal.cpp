
#include "../stdutil/stdutil.h"
#include "audio.openal.h"
#include <bb/audio/ogg_stream.h>
#include <bb/audio/wav_stream.h>

#ifndef __APPLE__
#include <AL/al.h>
#include <AL/alc.h>
#else
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

#include <thread>
#include <mutex>
#include <string.h>
using namespace std;

#define NUM_BUFFERS 6
#define BUFFER_SIZE 4096

class OpenALChannel : public BBChannel{
public:
	AudioStream::Ref *stream;

	ALuint source,buffers[NUM_BUFFERS];
	ALuint frequency;
	ALenum format;

	thread playbackThread;
	mutex playbackMutex;
	bool playbackRunning;

	OpenALChannel():stream(0),source(0),frequency(0),playbackRunning(false){}

	~OpenALChannel(){
		playbackThread.join(); // TODO: this may not be the best idea...
		alDeleteSources( 1,&source );
		alDeleteBuffers( NUM_BUFFERS,buffers );
	}

	bool setStream( AudioStream *s ){
		stream=s->getRef();

		unsigned int bits=stream->getBits(),channels=stream->getChannels();

		format=0;
		if( bits==8 ){
			if( channels==1 )
				format=AL_FORMAT_MONO8;
			else if( channels==2 )
				format=AL_FORMAT_STEREO8;
		}else if( bits==16 ){
			if( channels==1 )
				format=AL_FORMAT_MONO16;
			else if( channels==2 )
				format=AL_FORMAT_STEREO16;
		}
		if( format==0 ){
			RTEX( "unsupport format" );
			return false;
		}

		alGenBuffers( NUM_BUFFERS,buffers );
		alGenSources( 1,&source );

		return alGetError()==AL_NO_ERROR;
	}

	bool queue( ALuint buffer ){
		unsigned char *buf;
		size_t size=stream->decode( &buf );
		if( size==0 ) return false;

		alBufferData( buffer,format,buf,size,stream->getFrequency() );
		alSourceQueueBuffers( source,1,&buffer );
		return true;
	}

	bool streaming(){
		return !stream->eof();
	}

	void play(){
		if( !playbackRunning ){
			playbackRunning=true;
			playbackThread=thread( &playback,this );
		}
	}

	static void playback( OpenALChannel *channel ){
		int ret;

		for( int i=0;i<NUM_BUFFERS;i++ ){
			if( !channel->queue( channel->buffers[i] ) ) break;
		}

		if( alGetError()!=AL_NO_ERROR ){
			goto end;
		}

		alSourcePlay( channel->source );
		if( alGetError()!=AL_NO_ERROR ){
			channel->playbackRunning=false;
			goto end;
		}

		while( channel->streaming() ){
			ALuint buffer;
			ALint val;

			alGetSourcei( channel->source,AL_BUFFERS_PROCESSED,&val );
			if( val<=0 ) continue;

			while( val-- ){
				alSourceUnqueueBuffers(channel->source, 1, &buffer);
				if( channel->queue( buffer )==false ){
					goto end;
				}else if( alGetError()!=AL_NO_ERROR ){
					_bbLog( "%s","error buffering..." );
					goto end;
				}
			}

			alGetSourcei( channel->source,AL_SOURCE_STATE,&val );
			if( val!=AL_PLAYING ){
				alSourcePlay(channel->source);
			}
		}

		ALint val;
		do{
			alGetSourcei( channel->source,AL_SOURCE_STATE,&val );
		}while( val==AL_PLAYING );

end:
		channel->playbackRunning=false;
	}

	void stop(){
	}
	void setPaused( bool paused ){
	}
	void setPitch( int pitch ){
	}
	void setVolume( float volume ){
	}
	void setPan( float pan ){
	}
	void set3d( const float pos[3],const float vel[3] ){
		float p[3]={ pos[0],pos[1],-pos[2] };
		float v[3]={ vel[0],vel[1],-vel[2] };

		// get these from set3dOptions...
		alSourcef( source,AL_ROLLOFF_FACTOR,0.1 );
		alSourcef( source,AL_REFERENCE_DISTANCE,0.2 );

		alDistanceModel( AL_INVERSE_DISTANCE_CLAMPED );
		alSourcefv( source,AL_POSITION,p );
		alSourcefv( source,AL_VELOCITY,v );
		alSourcei( source,AL_SOURCE_RELATIVE,false );
		// alSourcef( source,AL_MIN_GAIN,0.0 );
		// alSourcef( source,AL_MAX_GAIN,100.0f );
	}
	bool isPlaying(){
		return playbackRunning;
	}
};

class OpenALSound : public BBSound{
public:
	AudioStream *stream;
	ALenum format;

	OpenALSound():format(0){
	}

	~OpenALSound(){
	}

	bool setStream( AudioStream *s ){
		stream=s;
		return true;
	}

	BBChannel *play(){
		OpenALChannel *channel=new OpenALChannel();
		if( !channel->setStream( stream ) ){
			return 0;
		}
		alDistanceModel( AL_NONE );
		channel->play();
		return channel;
	}

	BBChannel *play3d( const float pos[3],const float vel[3] ){
		OpenALChannel *channel=new OpenALChannel();
		if( !channel->setStream( stream ) ){
			return 0;
		}
		channel->set3d( pos,vel );
		channel->play();
		return channel;
	}

	void setLoop( bool loop ){
	}

	void setPitch( int hertz ){
	}

	void setVolume( float volume ){
	}

	void setPan( float pan ){
	}
};

class OpenALAudioDriver : public BBAudioDriver{
protected:
	ALCdevice *dev;
	ALCcontext *ctx;
	AudioStream *stream;

	AudioStream *loadStream( const string &filename,bool preload ){
		AudioStream *stream=0;

		const char *ext = strrchr( filename.c_str(),'.' );
		if( strcasecmp( ext + 1,"wav" ) == 0 ){
			stream=new WAVAudioStream( BUFFER_SIZE );
		}else if( strcasecmp( ext + 1,"ogg" ) == 0 ){
			stream=new OGGAudioStream( BUFFER_SIZE );
		}

		if( !stream ) return 0;

		if( !stream->init( filename.c_str() ) ){
			delete stream;
			return 0;
		}

		return stream;
	}

public:
	OpenALAudioDriver():dev(0),ctx(0),stream(0){
	}

	~OpenALAudioDriver(){
		alcMakeContextCurrent( NULL );
		if( ctx ) alcDestroyContext( ctx );
		if( dev ) alcCloseDevice( dev );
		delete stream;
	}

	bool init(){
		if( !(dev=alcOpenDevice(NULL)) ){
			fprintf(stderr, "Oops\n");
			return false;
		}
		if( !(ctx=alcCreateContext(dev,NULL)) ){
			fprintf(stderr, "Oops2\n");
			return false;
		}
		alcMakeContextCurrent( ctx );

		return true;
	}

	BBSound *loadSound( const std::string &filename,bool use_3d ){
		AudioStream *stream=loadStream( filename,true );
		if( !stream ){
			return 0;
		}

		OpenALSound *sound=new OpenALSound();
		if( !sound->setStream( stream ) ){
			delete sound;
			delete stream;
			return 0;
		}

		sound_set.insert( sound );
		return sound;
	}

	BBSound *verifySound( BBSound *sound ){
		return sound;
	}

	void freeSound( BBSound *sound ){
	}

	void setPaused( bool paused ){
	}
	//master pause
	void setVolume( float volume ){
	}
	//master volume

	void set3dOptions( float roll,float dopp,float dist ){
		alDopplerFactor( dopp );
	}

	void set3dListener( const float pos[3],const float vel[3],const float forward[3],const float up[3] ){
		float p[3]={ pos[0],pos[1],-pos[2] };
		float v[3]={ vel[0],vel[1],-vel[2] };

		alListenerfv( AL_POSITION,p );
		alListenerfv( AL_VELOCITY,v );
		alListenerfv( AL_ORIENTATION,up );
	}

	BBChannel *playCDTrack( int track,int mode ){
		RTEX( "PlayCDTrack not implemented" );
	}

	BBChannel *playFile( const std::string &filename,bool use_3d ){
		AudioStream *stream=loadStream( filename,false );
		if( !stream ){
			return 0;
		}

		OpenALChannel *channel=new OpenALChannel();
		if( !channel->setStream( stream ) ){
			delete stream;
			delete channel;
			return 0;
		}

		channel->play();
		return channel;
	}
};

static OpenALAudioDriver *driver;

BBMODULE_CREATE( audio_openal ){
	driver=d_new OpenALAudioDriver();
	if( !driver->init() ){
		delete driver;
		return true;
	}
	gx_audio=driver;
	return true;
}

BBMODULE_DESTROY( audio_openal ){
	return true;
}
