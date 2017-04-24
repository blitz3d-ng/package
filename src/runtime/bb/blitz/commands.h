#ifndef BB_BLITZ_COMMANDS_H
#define BB_BLITZ_COMMANDS_H

#include "../../config/config.h"
#include "module.h"

#ifdef __cplusplus
extern "C" {
#else
typedef void BBVecType;
typedef void BBObjType;
typedef void BBObj;
typedef void BBArray;
typedef void BBStr;
typedef void BBData;
#endif

// basic
BBStr *	 BBCALL _bbStrLoad( BBStr **var );
BBStr * BBCALL _bbStrCopy( BBStr *var );
void	 BBCALL _bbStrRelease( BBStr *str );
void	 BBCALL _bbStrStore( BBStr **var,BBStr *str );
int		 BBCALL _bbStrCompare( BBStr *lhs,BBStr *rhs );

BBStr *	 BBCALL _bbStrConcat( BBStr *s1,BBStr *s2 );
int		 BBCALL _bbStrToInt( BBStr *s );
BBStr *	 BBCALL _bbStrFromInt( int n );
float	 BBCALL _bbStrToFloat( BBStr *s );
BBStr *	 BBCALL _bbStrFromFloat( float n );
BBStr *	 BBCALL _bbStrConst( const char *s );

void	 BBCALL _bbDimArray( BBArray *array );
void	 BBCALL _bbUndimArray( BBArray *array );
void	 BBCALL _bbArrayBoundsEx();

void *	 BBCALL _bbVecAlloc( BBVecType *type );
void	 BBCALL _bbVecFree( void *vec,BBVecType *type );
void	 BBCALL _bbVecBoundsEx();

BBObj *	 BBCALL _bbObjNew( BBObjType *t );
void	 BBCALL _bbObjDelete( BBObj *obj );
void	 BBCALL _bbObjDeleteEach( BBObjType *type );
void	 BBCALL _bbObjRelease( BBObj *obj );
void	 BBCALL _bbObjStore( BBObj **var,BBObj *obj );
BBObj *	 BBCALL _bbObjNext( BBObj *obj );
BBObj *	 BBCALL _bbObjPrev( BBObj *obj );
BBObj *	 BBCALL _bbObjFirst( BBObjType *t );
BBObj *  BBCALL _bbObjLast( BBObjType *t );
void	 BBCALL _bbObjInsBefore( BBObj *o1,BBObj *o2 );
void	 BBCALL _bbObjInsAfter( BBObj *o1,BBObj *o2 );
int		 BBCALL _bbObjEachFirst( BBObj **var,BBObjType *type );
int		 BBCALL _bbObjEachNext( BBObj **var );
int		 BBCALL _bbObjEachFirst2( BBObj **var,BBObjType *type );
int		 BBCALL _bbObjEachNext2( BBObj **var );
int		 BBCALL _bbObjCompare( BBObj *o1,BBObj *o2 );
BBStr *	 BBCALL _bbObjToStr( BBObj *obj );
int		 BBCALL _bbObjToHandle( BBObj *obj );
BBObj *	 BBCALL _bbObjFromHandle( int handle,BBObjType *type );
void	 BBCALL _bbNullObjEx();

void	 BBCALL _bbRestore( BBData *data );
int		 BBCALL _bbReadInt();
float	 BBCALL _bbReadFloat();
BBStr *	 BBCALL _bbReadStr();

int		 BBCALL _bbAbs( int n );
int		 BBCALL _bbSgn( int n );
int		 BBCALL _bbMod( int x,int y );
float	 BBCALL _bbFAbs( float n );
float	 BBCALL _bbFSgn( float n );
float	 BBCALL _bbFMod( float x,float y );
float	 BBCALL _bbFPow( float x,float y );

void	 BBCALL bbRuntimeStats();

// app
void BBCALL bbStop();
void BBCALL bbEnd();
void BBCALL bbAppTitle( BBStr *ti,BBStr *cp );
BBStr * BBCALL bbCommandLine();

// debug
void BBCALL _bbDebugLog( const char *t );
void BBCALL _bbDebugInfo( const char *e );
void BBCALL _bbDebugError( const char *e );
void BBCALL _bbDebugStmt( bb_int_t pos,const char *file );
void BBCALL _bbDebugEnter( void *frame,void *env,const char *func );
void BBCALL _bbDebugLeave();
void BBCALL _bbDebugStop();
void BBCALL _bbDebugRun();

void BBCALL bbDebugLog( BBStr *t );

#ifdef __cplusplus
}
#endif

#endif
