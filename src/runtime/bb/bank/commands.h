#ifndef BB_BANK_COMMANDS_H
#define BB_BANK_COMMANDS_H

#include <bb/blitz/commands.h>
#include <bb/stream/commands.h>

#ifdef __cplusplus
extern "C" {
#else
typedef void bbBank;
#endif

bbBank * BBCALL bbCreateBank( int size );
void BBCALL bbFreeBank( bbBank *b );
int BBCALL bbBankSize( bbBank *b );
void BBCALL bbResizeBank( bbBank *b,int size );
void BBCALL bbCopyBank( bbBank *src,int src_p,bbBank *dest,int dest_p,int count );

int BBCALL bbPeekByte( bbBank *b,int offset );
int BBCALL bbPeekShort( bbBank *b,int offset );
int BBCALL bbPeekInt( bbBank *b,int offset );
float BBCALL bbPeekFloat( bbBank *b,int offset );

void BBCALL bbPokeByte( bbBank *b,int offset,int value );
void BBCALL bbPokeShort( bbBank *b,int offset,int value );
void BBCALL bbPokeInt( bbBank *b,int offset,int value );
void BBCALL bbPokeFloat( bbBank *b,int offset,float value );

int BBCALL  bbReadBytes( bbBank *b,bbStream *s,int offset,int count );
int BBCALL  bbWriteBytes( bbBank *b,bbStream *s,int offset,int count );

#ifdef __cplusplus
}
#endif

#endif
