
#include "module.h"

#include "../../gxruntime/gxruntime.h"
extern gxRuntime *gx_runtime;

static inline void debugSound( BBSound *s ){
	if( bb_env.debug ){
		if( !gx_audio->verifySound( s ) ) RTEX( "Sound does not exist" );
	}
}

static BBSound *loadSound( BBStr *f,bool use_3d ){
	string t=*f;delete f;
	return gx_audio ? gx_audio->loadSound( t,use_3d ) : 0;
}

static BBChannel *playMusic( BBStr *f,bool use_3d ){
	string t=*f;delete f;
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

void BBCALL bbSoundPitch( BBSound *sound,int pitch ){
	if( !sound ) return;
	debugSound( sound );
	sound->setPitch( pitch );
}

void BBCALL bbSoundVolume( BBSound *sound,float volume ){
	if( !sound ) return;
	debugSound( sound );
	sound->setVolume( volume );
}

void BBCALL bbSoundPan( BBSound *sound,float pan ){
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

BBChannel * BBCALL bbPlayCDTrack( int track,int mode ){
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

void BBCALL bbChannelPitch( BBChannel *channel,int pitch ){
	if( !channel ) return;
	channel->setPitch( pitch );
}

void BBCALL bbChannelVolume( BBChannel *channel,float volume ){
	if( !channel ) return;
	channel->setVolume( volume );
}

void BBCALL bbChannelPan( BBChannel *channel,float pan ){
	if( !channel ) return;
	channel->setPan( pan );
}

int BBCALL bbChannelPlaying( BBChannel *channel ){
	return channel ? channel->isPlaying() : 0;
}

#ifdef PRO
BBSound * BBCALL bbLoad3DSound( BBStr *f ){
	return loadSound( f,true );
}
#endif

BBMODULE_CREATE( audio ){
	gx_audio=gx_runtime->openAudio( 0 );
	return true;
}

BBMODULE_DESTROY( audio ){
	if( gx_audio ) gx_runtime->closeAudio( gx_audio );
	gx_audio=0;
	return true;
}

BBMODULE_LINK( audio ){
	rtSym( "%LoadSound$filename",bbLoadSound );
	rtSym( "FreeSound%sound",bbFreeSound );
	rtSym( "LoopSound%sound",bbLoopSound );
	rtSym( "SoundPitch%sound%pitch",bbSoundPitch );
	rtSym( "SoundVolume%sound#volume",bbSoundVolume );
	rtSym( "SoundPan%sound#pan",bbSoundPan );
	rtSym( "%PlaySound%sound",bbPlaySound );
	rtSym( "%PlayMusic$midifile",bbPlayMusic );
	rtSym( "%PlayCDTrack%track%mode=1",bbPlayCDTrack );
	rtSym( "StopChannel%channel",bbStopChannel );
	rtSym( "PauseChannel%channel",bbPauseChannel );
	rtSym( "ResumeChannel%channel",bbResumeChannel );
	rtSym( "ChannelPitch%channel%pitch",bbChannelPitch );
	rtSym( "ChannelVolume%channel#volume",bbChannelVolume );
	rtSym( "ChannelPan%channel#pan",bbChannelPan );
	rtSym( "%ChannelPlaying%channel",bbChannelPlaying );
#ifdef PRO
	rtSym( "%Load3DSound$filename",bbLoad3DSound );
#endif
}
