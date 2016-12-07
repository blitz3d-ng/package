
#ifndef BBAUDIO_H
#define BBAUDIO_H

#include "bbsys.h"
#include "../gxruntime/gxaudio.h"

extern gxAudio *gx_audio;

gxSound *	 BBCALL bbLoadSound( BBStr *file );
void		 BBCALL bbFreeSound( gxSound *sound );
gxChannel *	 BBCALL bbPlaySound( gxSound *sound );
void		 BBCALL bbLoopSound( gxSound *sound );
void		 BBCALL bbSoundPitch( gxSound *sound,int pitch );
void		 BBCALL bbSoundVolume( gxSound *sound,float volume );
void		 BBCALL bbSoundPan( gxSound *sound,float pan );
gxChannel *	 BBCALL bbPlayMusic( BBStr *s );
gxChannel *  BBCALL bbPlayCDTrack( int track,int mode );
void		 BBCALL bbStopChannel( gxChannel *channel );
void		 BBCALL bbPauseChannel( gxChannel *channel );
void		 BBCALL bbResumeChannel( gxChannel *channel );
void		 BBCALL bbChannelPitch( gxChannel *channel,int pitch );
void		 BBCALL bbChannelVolume( gxChannel *channel,float volume );
void		 BBCALL bbChannelPan( gxChannel *channel,float pan );
int			 BBCALL bbChannelPlaying( gxChannel *channel );

#endif
