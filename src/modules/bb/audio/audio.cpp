
#include "../../../stdutil/stdutil.h"
#include <bb/runtime/runtime.h>
#include "audio.h"

#include <string>
using namespace std;

static inline void debugSound( BBSound *s ){
	if( bb_env.debug ){
		if( !gx_audio->verifySound( s ) ) RTEX( "Sound does not exist" );
	}
}

static BBSound *loadSound( BBStr *f,bool use_3d ){
	string t=canonicalpath(*f);delete f;
	return gx_audio ? gx_audio->loadSound( t,use_3d ) : 0;
}

static BBChannel *playMusic( BBStr *f,bool use_3d ){
	string t=canonicalpath(*f);delete f;
	return gx_audio ? gx_audio->playFile( t,use_3d ) : 0;
}

BBSound * BBCALL bbLoadSound( BBStr *f ){
	return loadSound( f,false );
}

void BBCALL bbFreeSound( BBSound *sound ){
	if( !sound ) return;
	debugSound( sound );
	gx_audio->freeSound( sound );
}

void BBCALL bbLoopSound( BBSound *sound ){
	if( !sound ) return;
	debugSound( sound );
	sound->setLoop( true );
}

void BBCALL bbSoundPitch( BBSound *sound,bb_int_t pitch ){
	if( !sound ) return;
	debugSound( sound );
	sound->setPitch( pitch );
}

void BBCALL bbSoundVolume( BBSound *sound,bb_float_t volume ){
	if( !sound ) return;
	debugSound( sound );
	sound->setVolume( volume );
}

void BBCALL bbSoundPan( BBSound *sound,bb_float_t pan ){
	if( !sound ) return;
	debugSound( sound );
	sound->setPan( pan );
}

BBChannel * BBCALL bbPlaySound( BBSound *sound ){
	if( !sound ) return 0;
	debugSound( sound );
	return sound->play();
}

BBChannel * BBCALL bbPlayMusic( BBStr *f ){
	return playMusic( f,false );
}

BBChannel * BBCALL bbPlayCDTrack( bb_int_t track,bb_int_t mode ){
	return gx_audio ? gx_audio->playCDTrack( track,mode ) : 0;
}

void BBCALL bbStopChannel( BBChannel *channel ){
	if( !channel ) return;
	channel->stop();
}

void BBCALL bbPauseChannel( BBChannel *channel ){
	if( !channel ) return;
	channel->setPaused( true );
}

void BBCALL bbResumeChannel( BBChannel *channel ){
	if( !channel ) return;
	channel->setPaused( false );
}

void BBCALL bbChannelPitch( BBChannel *channel,bb_int_t pitch ){
	if( !channel ) return;
	channel->setPitch( pitch );
}

void BBCALL bbChannelVolume( BBChannel *channel,bb_float_t volume ){
	if( !channel ) return;
	channel->setVolume( volume );
}

void BBCALL bbChannelPan( BBChannel *channel,bb_float_t pan ){
	if( !channel ) return;
	channel->setPan( pan );
}

bb_int_t BBCALL bbChannelPlaying( BBChannel *channel ){
	return channel ? channel->isPlaying() : 0;
}

BBSound * BBCALL bbLoad3DSound( BBStr *f ){
	return loadSound( f,true );
}

void pauseAudio( void *data,void *context ){
	if( gx_audio ) gx_audio->setPaused( true );
}

void resumeAudio( void *data,void *context ){
	if( gx_audio ) gx_audio->setPaused( false );
}

BBMODULE_CREATE( audio ){
	gx_audio=0;
	bbRuntimeOnSuspend->add( pauseAudio,0 );
	bbRuntimeOnResume->add( resumeAudio,0 );
	return true;
}

BBMODULE_DESTROY( audio ){
	if( gx_audio ){
		delete gx_audio;
		gx_audio=0;
	}
	return true;
}
