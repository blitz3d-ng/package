
#ifndef GXAUDIO_H
#define GXAUDIO_H

#include "sound.h"

struct FSOUND_SAMPLE;

class FMODAudioDriver : public BBAudioDriver{
public:
	FMODAudioDriver();
	~FMODAudioDriver();

	BBChannel *play( FSOUND_SAMPLE *sample );
	BBChannel *play3d( FSOUND_SAMPLE *sample,const float pos[3],const float vel[3] );

private:

	/***** GX INTERFACE *****/
public:
	bool init();

	BBSound *loadSound( const std::string &filename,bool use_3d );
	BBSound *verifySound( BBSound *sound );
	void freeSound( BBSound *sound );

	void setPaused( bool paused );	//master pause
	void setVolume( float volume );	//master volume

	void set3dOptions( float roll,float dopp,float dist );

	void set3dListener( const float pos[3],const float vel[3],const float forward[3],const float up[3] );

	BBChannel *playCDTrack( int track,int mode );
	BBChannel *playFile( const std::string &filename,bool use_3d );
};

#endif
