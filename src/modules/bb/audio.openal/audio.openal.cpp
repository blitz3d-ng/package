
#include "../stdutil/stdutil.h"
#include "audio.openal.h"
#include <OpenAL/OpenAL.h>
#include <bb/audio/ogg_stream.h>
#include <bb/audio/wav_stream.h>
#include <bb/audio/mp3_stream.h>

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

#define NUM_BUFFERS 6
#define BUFFER_SIZE 4096

#define AL( func ) func; checkErrors( __FILE__,__LINE__ );

static std::set<BBChannel*> channel_set;

static void checkErrors( const char *file,int line ){
  switch( alGetError() ){
  case AL_NO_ERROR: return;
  case AL_INVALID_NAME:      LOGE( "[audio.openal] %s (%s:%i)","AL_INVALID_NAME",file,line );break;
  case AL_INVALID_ENUM:      LOGE( "[audio.openal] %s (%s:%i)","AL_INVALID_ENUM",file,line );break;
  case AL_INVALID_VALUE:     LOGE( "[audio.openal] %s (%s:%i)","AL_INVALID_VALUE",file,line );break;
  case AL_INVALID_OPERATION: LOGE( "[audio.openal] %s (%s:%i)","AL_INVALID_OPERATION",file,line );break;
  case AL_OUT_OF_MEMORY:     LOGE( "[audio.openal] %s (%s:%i)","AL_OUT_OF_MEMORY",file,line );break;
  default:                   LOGE( "[audio.openal] %s (%s:%i)","unknown error",file,line );break;
  }
}

class OpenALChannel : public BBChannel{
public:
	AudioStream::Ref *stream;

	ALuint source,buffers[NUM_BUFFERS];
	ALuint frequency;
	ALenum format;

	std::thread playbackThread;
	bool playbackRunning;

	OpenALChannel():stream(0),source(0),frequency(0),playbackRunning(false){}

	~OpenALChannel(){
		playbackRunning=false;
		playbackThread.join(); // TODO: this may not be the best idea...
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
		return playbackRunning && !stream->eof();
	}

	void play(){
		if( !playbackRunning ){
			playbackRunning=true;
			playbackThread=std::thread( &playback,this );
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
					LOGD( "%s","error buffering..." );
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

		alDeleteSources( 1,&channel->source );
		alDeleteBuffers( NUM_BUFFERS,channel->buffers );

		delete channel->stream;channel->stream=0;
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

	float getDuration(){
		return (stream->getSamples() / (float)stream->getChannels()) / (float)stream->getFrequency();
	}

	float getPosition(){
		return (stream->pos / (float)stream->getChannels()) / (float)stream->getFrequency();
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
		channel_set.insert( channel );
		return channel;
	}

	BBChannel *play3d( const float pos[3],const float vel[3] ){
		OpenALChannel *channel=new OpenALChannel();
		if( !channel->setStream( stream ) ){
			return 0;
		}
		channel->set3d( pos,vel );
		channel->play();
		channel_set.insert( channel );
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

	AudioStream *loadStream( const std::string &filename,bool preload ){
		AudioStream *stream=0;

		// TODO: come up with something a little more clever
		const char *ext = strrchr( filename.c_str(),'.' );
		const char *exts[]={ ext,strcasecmp( ext + 1,"wav" )==0?".ogg":".wav",0 };
		int tries=0;
		while( exts[tries] ) {
			if( strcasecmp( exts[tries] + 1,"wav" ) == 0 ){
				stream=new WAVAudioStream( BUFFER_SIZE );
			}else if( strcasecmp( exts[tries] + 1,"ogg" ) == 0 ){
				stream=new OGGAudioStream( BUFFER_SIZE );
			}else if( strcasecmp( exts[tries] + 1,"mp3" ) == 0 ){
				stream=new MP3AudioStream( BUFFER_SIZE );
			}

			if( !stream ) return 0;

			if( stream->init( filename.c_str() ) ){
				break;
			}else{
				delete stream;
				stream=0;
			}

			tries++;
		}

		return stream;
	}

public:
	OpenALAudioDriver():dev(0),ctx(0){
	}

	~OpenALAudioDriver(){
		while( channel_set.size() ) {
			BBChannel *c=*channel_set.begin();
			if( channel_set.erase( c ) ) delete c;
		}
		while( sound_set.size() ) freeSound( *sound_set.begin() );
		alcMakeContextCurrent( NULL );
		if( ctx ){ alcDestroyContext( ctx );ctx=0; }
		if( dev ){ alcCloseDevice( dev );dev=0; }
	}

	bool init(){
		if( !(dev=alcOpenDevice(NULL)) ){
			LOGE("[audio.openal] %s", "Could not open audio device");
			return false;
		}
		if( !(ctx=alcCreateContext(dev,NULL)) ){
			LOGE("[audio.openal] %s", "Could not open audio context");
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
		channel_set.insert( channel );
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
