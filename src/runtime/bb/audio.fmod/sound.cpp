
#include <fmod.h>
#include "sound.h"
#include "driver.h"

FMODSound::FMODSound( FMODAudioDriver *a,FSOUND_SAMPLE *s ):
audio(a),sample(s),defs_valid(true){
	FSOUND_Sample_GetDefaults( sample,&def_freq,&def_vol,&def_pan,&def_pri );
}

FMODSound::~FMODSound(){
	FSOUND_Sample_Free( sample );
}

void FMODSound::setDefaults(){
	if( !defs_valid ){
		FSOUND_Sample_SetDefaults( sample,def_freq,def_vol,def_pan,def_pri );
		defs_valid=true;
	}
}

BBChannel *FMODSound::play(){
	setDefaults();
	return audio->play( sample );
}

BBChannel *FMODSound::play3d( const float pos[3],const float vel[3] ){
	setDefaults();
	return audio->play3d( sample,pos,vel );
}

void FMODSound::setLoop( bool loop ){
	FSOUND_Sample_SetMode( sample,loop ? FSOUND_LOOP_NORMAL : FSOUND_LOOP_OFF );
}

void FMODSound::setPitch( int hertz ){
	def_freq=hertz;
	defs_valid=false;
}

void FMODSound::setVolume( float volume ){
	def_vol=volume*255.0f;
	defs_valid=false;
}

void FMODSound::setPan( float pan ){
	def_pan=(pan+1.0f)*127.5f;
	defs_valid=false;
}
