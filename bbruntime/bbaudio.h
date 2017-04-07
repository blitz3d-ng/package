
#ifndef BBAUDIO_H
#define BBAUDIO_H

#include "bbsys.h"
#include <bb/audio/driver.h>

BBSound *	 BBCALL bbLoadSound( BBStr *file );
void		 BBCALL bbFreeSound( BBSound *sound );
BBChannel *	 BBCALL bbPlaySound( BBSound *sound );
void		 BBCALL bbLoopSound( BBSound *sound );
void		 BBCALL bbSoundPitch( BBSound *sound,int pitch );
void		 BBCALL bbSoundVolume( BBSound *sound,float volume );
void		 BBCALL bbSoundPan( BBSound *sound,float pan );
BBChannel *	 BBCALL bbPlayMusic( BBStr *s );
BBChannel *  BBCALL bbPlayCDTrack( int track,int mode );
void		 BBCALL bbStopChannel( BBChannel *channel );
void		 BBCALL bbPauseChannel( BBChannel *channel );
void		 BBCALL bbResumeChannel( BBChannel *channel );
void		 BBCALL bbChannelPitch( BBChannel *channel,int pitch );
void		 BBCALL bbChannelVolume( BBChannel *channel,float volume );
void		 BBCALL bbChannelPan( BBChannel *channel,float pan );
int			 BBCALL bbChannelPlaying( BBChannel *channel );

#endif
