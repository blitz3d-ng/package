
#include "../stdutil/stdutil.h"
#include <bb/blitz/module.h>
#include <bb/runtime/runtime.h>
#include <vector>
#include <map>
#include <set>
using namespace std;

#include <fmod.h>
#include "driver.h"

struct StaticChannel : public BBChannel{
	virtual void play()=0;
};

struct SoundChannel : public BBChannel{
	SoundChannel():channel(-1){
	}
	void set( int n ){
		channel=n;
	}
	void stop(){
		FSOUND_StopSound( channel );
	}
	void setPaused( bool paused ){
		FSOUND_SetPaused( channel,paused );
	}
	void setPitch( int pitch ){
		FSOUND_SetFrequency( channel,pitch );
	}
	void setVolume( float volume ){
		FSOUND_SetVolume( channel,volume * 255.0f );
	}
	void setPan( float pan ){
		FSOUND_SetPan( channel,(pan+1)*127.5f );
	}
	void set3d( const float pos[3],const float vel[3] ){
		FSOUND_3D_SetAttributes( channel,(float*)pos,(float*)vel );
	}
	bool isPlaying(){
		return FSOUND_IsPlaying( channel ) ? true : false;
	}
private:
	int channel;
};

struct CDChannel : public BBChannel{
	void play( int track,int mode ){
		stop();
		int cd_mode=FSOUND_CD_PLAYONCE;
		if( mode==BBAudioDriver::CD_MODE_LOOP ) cd_mode=FSOUND_CD_PLAYLOOPED;
		else if( mode==BBAudioDriver::CD_MODE_ALL ) cd_mode=FSOUND_CD_PLAYCONTINUOUS;
		FSOUND_CD_SetPlayMode( 0,cd_mode );
		FSOUND_CD_Play( 0,track );
	}
	void stop(){
		FSOUND_CD_Stop(0 );
	}
	void setPaused( bool paused ){
		FSOUND_CD_SetPaused( 0,paused );
	}
	void setPitch( int pitch ){
	}
	void setVolume( float volume ){
		FSOUND_CD_SetVolume( 0,volume*255.0f );
	}
	void setPan( float pan ){
	}
	void set3d( const float pos[3],const float vel[3] ){
	}
	bool isPlaying(){
		return true;
	}
};

struct StreamChannel : public StaticChannel{
	StreamChannel( FSOUND_STREAM *s ):stream(s){
		channel=FSOUND_Stream_Play( FSOUND_FREE,stream );
	}
	~StreamChannel(){
		FSOUND_Stream_Close( stream );
	}
	void play(){
		stop();
		channel=FSOUND_Stream_Play( FSOUND_FREE,stream );
	}
	void stop(){
		FSOUND_Stream_Stop( stream );
		channel=-1;
	}
	void setPaused( bool paused ){
		FSOUND_SetPaused( channel,paused );
	}
	void setPitch( int pitch ){
		FSOUND_SetFrequency( channel,pitch );
	}
	void setVolume( float volume ){
		FSOUND_SetVolume( channel,volume * 255.0f );
	}
	void setPan( float pan ){
		FSOUND_SetPan( channel,(pan+1)*127.5f );
	}
	void set3d( const float pos[3],const float vel[3] ){
	}
	bool isPlaying(){
		return FSOUND_IsPlaying( channel ) ? true : false;
	}
private:
	FSOUND_STREAM *stream;
	int channel;
};

struct MusicChannel : public StaticChannel{
	MusicChannel( FMUSIC_MODULE *m ):module(m){
		play();
	}
	~MusicChannel(){
		FMUSIC_FreeSong( module );
	}
	void play(){
		FMUSIC_PlaySong( module );
	}
	void stop(){
		FMUSIC_StopSong( module );
	}
	void setPaused( bool paused ){
		FMUSIC_SetPaused( module,paused );
	}
	void setPitch( int pitch ){
	}
	void setVolume( float volume ){
		FMUSIC_SetMasterVolume( module,volume*255.0f );
	}
	void setPan( float pan ){
	}
	void set3d( const float pos[3],const float vel[3] ){
	}
	bool isPlaying(){
		return FMUSIC_IsFinished( module ) ? false : true;
	}
private:
	FMUSIC_MODULE *module;
};

static set<FMODSound*> sound_set;
static vector<BBChannel*> channels;
static map<string,StaticChannel*> songs;
static CDChannel *cdChannel;

static int next_chan;
static vector<SoundChannel*> soundChannels;

static BBChannel *allocSoundChannel( int n ){

	SoundChannel *chan=0;
	for( int k=0;k<soundChannels.size();++k ){
		chan=soundChannels[next_chan];
		if( !chan ){
			chan=soundChannels[next_chan]=d_new SoundChannel();
			channels.push_back(chan);
		}else if( chan->isPlaying() ){
			chan=0;
		}
		if( ++next_chan==soundChannels.size() ) next_chan=0;
		if( chan ) break;
	}

	if( !chan ){
		next_chan=soundChannels.size();
		soundChannels.resize(soundChannels.size()*2);
		for( int k=next_chan;k<soundChannels.size();++k ) soundChannels[k]=0;
		chan=soundChannels[next_chan++]=d_new SoundChannel();
		channels.push_back( chan );
	}

	chan->set(n);
	return chan;
}

FMODAudioDriver::FMODAudioDriver(){
}

FMODAudioDriver::~FMODAudioDriver(){
	//free all channels
	for( ;channels.size();channels.pop_back() ) delete channels.back();
	//free all sound_set
	while( sound_set.size() ) freeSound( *sound_set.begin() );
	soundChannels.clear();
	songs.clear();

	FSOUND_Close();
}

bool FMODAudioDriver::init(){
	int f_flags=
		FSOUND_INIT_GLOBALFOCUS|
		FSOUND_INIT_USEDEFAULTMIDISYNTH;

	FSOUND_SetHWND( (HWND)bbRuntimeWindow() );
	if( !FSOUND_Init( 44100,1024,f_flags ) ){
		return false;
	}

	next_chan=0;
	soundChannels.resize( 4096 );
	for( int k=0;k<4096;++k ) soundChannels[k]=0;

	cdChannel=d_new CDChannel();
	channels.push_back( cdChannel );

	return true;
}

BBChannel *FMODAudioDriver::play( FSOUND_SAMPLE *sample ){

	int n=FSOUND_PlaySound( FSOUND_FREE,sample );
	return n>=0 ? allocSoundChannel( n ) : 0;
}

BBChannel *FMODAudioDriver::play3d( FSOUND_SAMPLE *sample,const float pos[3],const float vel[3] ){

	int n=FSOUND_PlaySoundEx( FSOUND_FREE,sample,0,true );
	if( n<0 ) return 0;
	FSOUND_3D_SetAttributes( n,(float*)pos,(float*)vel );
	FSOUND_SetPaused( n,false );
	return allocSoundChannel( n );
}

BBSound *FMODAudioDriver::loadSound( const string &f,bool use3d ){

	int flags=FSOUND_NORMAL | (use3d ? FSOUND_FORCEMONO : FSOUND_2D);

	FSOUND_SAMPLE *sample=FSOUND_Sample_Load( FSOUND_FREE,f.c_str(),flags,0,0 );
	if( !sample ) return 0;

	FMODSound *sound=d_new FMODSound( this,sample );
	sound_set.insert( sound );
	return sound;
}

BBSound *FMODAudioDriver::verifySound( BBSound *s ){
	return sound_set.count( (FMODSound*)s )  ? s : 0;
}

void FMODAudioDriver::freeSound( BBSound *s ){
	if( sound_set.erase( (FMODSound*)s ) ) delete s;
}

void FMODAudioDriver::setPaused( bool paused ){
	FSOUND_SetPaused( FSOUND_ALL,paused );
}

void FMODAudioDriver::setVolume( float volume ){
}

void FMODAudioDriver::set3dOptions( float roll,float dopp,float dist ){
	FSOUND_3D_SetRolloffFactor( roll );
	FSOUND_3D_SetDopplerFactor( dopp );
	FSOUND_3D_SetDistanceFactor( dist );
}

void FMODAudioDriver::set3dListener( const float pos[3],const float vel[3],const float forward[3],const float up[3] ){
	FSOUND_3D_Listener_SetAttributes( (float*)pos,(float*)vel,forward[0],forward[1],forward[2],up[0],up[1],up[2] );
	FSOUND_Update();
}

BBChannel *FMODAudioDriver::playFile( const string &t,bool use_3d ){
	string f=tolower( t );
	StaticChannel *chan=0;
	map<string,StaticChannel*>::iterator it=songs.find(f);
	if( it!=songs.end() ){
		chan=it->second;
		chan->play();
		return chan;
	}else if(
		f.find( ".raw" )!=string::npos ||
		f.find( ".wav" )!=string::npos ||
		f.find( ".mp2" )!=string::npos ||
		f.find( ".mp3" )!=string::npos ||
		f.find( ".ogg" )!=string::npos ||
		f.find( ".wma" )!=string::npos ||
		f.find( ".asf" )!=string::npos ){
		FSOUND_STREAM *stream=FSOUND_Stream_Open( f.c_str(),use_3d,0,0 );
		if( !stream ) return 0;
		chan=d_new StreamChannel( stream );
	}else{
		FMUSIC_MODULE *module=FMUSIC_LoadSong( f.c_str() );
		if( !module ) return 0;
		chan=d_new MusicChannel( module );
	}
	channels.push_back( chan );
	songs[f]=chan;
	return chan;
}

BBChannel *FMODAudioDriver::playCDTrack( int track,int mode ){
	cdChannel->play( track,mode );
	return cdChannel;
}

BBMODULE_CREATE( audio_fmod ){
	if( !gx_audio ){
		gx_audio=d_new FMODAudioDriver();
		if( !gx_audio->init() ){
			gx_audio=0;
		}
	}
	return true;
}

BBMODULE_DESTROY( audio_fmod ){
	return true;
}
