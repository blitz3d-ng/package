#ifndef BB_FILESYSTEM_COMMANDS_H
#define BB_FILESYSTEM_COMMANDS_H

#include <bb/blitz/commands.h>

#ifdef __cplusplus
extern "C"{
#else
typedef void BBDir;
typedef void bbFile;
#endif

bbFile* BBCALL bbReadFile( BBStr *f );
bbFile* BBCALL bbWriteFile( BBStr *f );
bbFile* BBCALL bbOpenFile( BBStr *f );
void BBCALL bbCloseFile( bbFile *f );
int BBCALL bbFilePos( bbFile *f );
int BBCALL bbSeekFile( bbFile *f,int pos );
BBDir* BBCALL bbReadDir( BBStr *d );
void BBCALL bbCloseDir( BBDir *d );
BBStr* BBCALL bbNextFile( BBDir *d );
BBStr* BBCALL bbCurrentDir();
void BBCALL bbChangeDir( BBStr *d );
void BBCALL bbCreateDir( BBStr *d );
void BBCALL bbDeleteDir( BBStr *d );
int BBCALL bbFileType( BBStr *f );
int BBCALL bbFileSize( BBStr *f );
void BBCALL bbCopyFile( BBStr *f,BBStr *to );
void BBCALL bbDeleteFile( BBStr *f );

#ifdef __cplusplus
}
#endif

#endif
