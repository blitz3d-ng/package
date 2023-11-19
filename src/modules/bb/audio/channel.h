#ifndef BBCHANNEL_H
#define BBCHANNEL_H

class BBChannel{

	/***** GX INTERFACE *****/
public:
	virtual ~BBChannel();
	//modifiers
	virtual void stop()=0;
	virtual void setPaused( bool paused )=0;
	virtual void setPitch( int pitch )=0;
	virtual void setVolume( float volume )=0;
	virtual void setPan( float pan )=0;
	virtual void set3d( const float pos[3],const float vel[3] )=0;

	virtual float getDuration()=0;
	virtual float getPosition()=0;

	virtual bool isPlaying()=0;
};

#endif
