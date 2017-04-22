
#ifndef BBSOUND_H
#define BBSOUND_H

#include "channel.h"

class BBSound{
  /***** GX INTERFACE *****/
public:
  virtual ~BBSound();

  //actions
  virtual BBChannel *play()=0;
  virtual BBChannel *play3d( const float pos[3],const float vel[3] )=0;

  //modifiers
  virtual void setLoop( bool loop )=0;
  virtual void setPitch( int hertz )=0;
  virtual void setVolume( float volume )=0;
  virtual void setPan( float pan )=0;
};

#endif
