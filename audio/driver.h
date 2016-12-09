#ifndef BBAUDIODRIVER_H
#define BBAUDIODRIVER_H

#include <string>

#include "channel.h"
#include "sound.h"

struct BBEnv;

class BBAudioDriver{
  /***** GX INTERFACE *****/
public:
  enum{
    CD_MODE_ONCE=1,CD_MODE_LOOP,CD_MODE_ALL
  };

  virtual bool init( const BBEnv &env )=0;

  virtual BBSound *loadSound( const std::string &filename,bool use_3d )=0;
  virtual BBSound *verifySound( BBSound *sound )=0;
  virtual void freeSound( BBSound *sound )=0;

  virtual void setPaused( bool paused )=0;	//master pause
  virtual void setVolume( float volume )=0;	//master volume

  virtual void set3dOptions( float roll,float dopp,float dist )=0;

  virtual void set3dListener( const float pos[3],const float vel[3],const float forward[3],const float up[3] )=0;

  virtual BBChannel *playCDTrack( int track,int mode )=0;
  virtual BBChannel *playFile( const std::string &filename,bool use_3d )=0;
};

extern BBAudioDriver *gx_audio;

#endif
