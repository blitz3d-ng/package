
#ifndef GXSOUND_H
#define GXSOUND_H

#include <bb/audio/driver.h>

class FMODAudioDriver;
struct FSOUND_SAMPLE;

class FMODSound : public BBSound{
public:
	FMODAudioDriver *audio;

	FMODSound( FMODAudioDriver *audio,FSOUND_SAMPLE *sample );
	~FMODSound();

private:
	bool defs_valid;
	int def_freq,def_vol,def_pan,def_pri;
	FSOUND_SAMPLE *sample;
	float pos[3],vel[3];

	void setDefaults();

	/***** GX INTERFACE *****/
public:
	//actions
	BBChannel *play();
	BBChannel *play3d( const float pos[3],const float vel[3] );

	//modifiers
	void setLoop( bool loop );
	void setPitch( int hertz );
	void setVolume( float volume );
	void setPan( float pan );
};

#endif
