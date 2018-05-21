#ifndef BB_STREAM_COMMANDS_H
#define BB_STREAM_COMMANDS_H

#include <bb/blitz/commands.h>

#ifdef __cplusplus
extern "C" {
#else
typedef void bbStream;
#endif

int BBCALL bbEof( bbStream *s );
int BBCALL bbReadAvail( bbStream *s );

int BBCALL bbReadByte( bbStream *s );
int BBCALL bbReadShort( bbStream *s );
int BBCALL bbReadInt( bbStream *s );
float BBCALL bbReadFloat( bbStream *s );
BBStr * BBCALL bbReadString( bbStream *s );
BBStr * BBCALL bbReadLine( bbStream *s );

void BBCALL bbWriteByte( bbStream *s,int n );
void BBCALL bbWriteShort( bbStream *s,int n );
void BBCALL bbWriteInt( bbStream *s,int n );
void BBCALL bbWriteFloat( bbStream *s,float n );
void BBCALL bbWriteString( bbStream *s,BBStr *t );
void BBCALL bbWriteLine( bbStream *s,BBStr *t );

void BBCALL bbCopyStream( bbStream *s,bbStream *d,int buff_size );

#ifdef __cplusplus
}
#endif

#endif
