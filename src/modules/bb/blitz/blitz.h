#ifndef BLITZ_H
#define BLITZ_H

#include "module.h"
#include "ex.h"
#include "basic.h"
#include "env.h"
#include "debug.h"
#include "app.h"

#include <math.h>

enum{
	BBTYPE_END=0,
	BBTYPE_INT=1,BBTYPE_FLT=2,
	BBTYPE_STR=3,BBTYPE_CSTR=4,
	BBTYPE_OBJ=5,BBTYPE_VEC=6
};

#pragma pack( push,1 )

struct BBObj;
typedef struct BBObj BBObj;

#ifdef __cplusplus
struct BBStr;
#else
typedef void BBStr;
#endif

struct BBType;
typedef struct BBType BBType;

struct BBObjType;
typedef struct BBObjType BBObjType;

struct BBVecType;
typedef struct BBVecType BBVecType;

union  BBField;
typedef union BBField BBField;

struct BBArray;
typedef struct BBArray BBArray;

typedef struct BBObj{
	BBField *fields;
	BBObj *next,*prev;
	BBObjType *type;
	bb_int_t ref_cnt;
#ifdef __cplusplus
	BBObj(){}
	BBObj( BBField *f,BBObj *n,BBObj *p,BBObjType *t,int c ):fields(f),next(n),prev(p),type(t),ref_cnt(c){}
#endif
} BBObj;

struct BBType{
	bb_int_t type;
#ifdef __cplusplus
	BBType( int n ):type(n){}
#endif
};

#ifdef __cplusplus
struct BBObjType : public BBType{
#else
struct BBObjType{
	BBType base;
#endif
	BBObj used,free;
	bb_int_t fieldCnt;
	BBType *fieldTypes[1];
#ifdef __cplusplus
	BBObjType( int c,BBType *ft1 ):BBType(5),fieldCnt(c){
		used=BBObj(0,&used,&used,0,-1);
		free=BBObj(0,&free,&free,0,-1);
		fieldTypes[0]=ft1;
	}
#endif
};

#ifdef __cplusplus
struct BBVecType : public BBType{
#else
struct BBVecType{
	BBType base;
#endif
	bb_int_t size;
	BBType *elementType;
};

union BBField{
	bb_int_t INT;
	bb_float_t FLT;
	BBStr *STR;
	char *CSTR;
	BBObj *OBJ;
	void *VEC;
};

struct BBArray{
	void *data;
	bb_int_t elementType,dims,scales[1];
};

#ifdef __cplusplus

#include <string>

struct BBStr : public std::string{
	BBStr *next,*prev;

	BBStr();
	BBStr( const char *s );
	BBStr( const char *s,int n );
	BBStr( const BBStr &s );
	BBStr( const std::string &s );
	BBStr &operator=( const char *s );
	BBStr &operator=( const BBStr &s );
	BBStr &operator=( const std::string &s );
	~BBStr();

	void *operator new( size_t size );
	void operator delete( void *q );

	void *operator new( size_t size,const char *file,int line ){ return operator new( size ); }
	void operator delete( void *q,const char *file,int line ){ operator delete( q ); }
};

#endif

struct BBData{
	bb_int_t fieldType;
	BBField field;
};
typedef struct BBData BBData;

#pragma pack( pop )

#ifdef __cplusplus
extern "C" {
#endif

// basic
BBStr *	 BBCALL _bbStrLoad( BBStr **var );
BBStr * BBCALL _bbStrCopy( BBStr *var );
void	 BBCALL _bbStrRelease( BBStr *str );
void	 BBCALL _bbStrStore( BBStr **var,BBStr *str );
bb_int_t BBCALL _bbStrCompare( BBStr *lhs,BBStr *rhs );

BBStr *	 BBCALL _bbStrConcat( BBStr *s1,BBStr *s2 );
bb_int_t		 BBCALL _bbStrToInt( BBStr *s );
BBStr *	 BBCALL _bbStrFromInt( bb_int_t n );
bb_float_t	 BBCALL _bbStrToFloat( BBStr *s );
BBStr *	 BBCALL _bbStrFromFloat( bb_float_t n );
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
bb_int_t BBCALL _bbObjEachFirst( BBObj **var,BBObjType *type );
bb_int_t BBCALL _bbObjEachNext( BBObj **var );
bb_int_t BBCALL _bbObjEachFirst2( BBObj **var,BBObjType *type );
bb_int_t BBCALL _bbObjEachNext2( BBObj **var );
bb_int_t BBCALL _bbObjCompare( BBObj *o1,BBObj *o2 );
BBStr *	 BBCALL _bbObjToStr( BBObj *obj );
bb_int_t		 BBCALL _bbObjToHandle( BBObj *obj );
BBObj *	 BBCALL _bbObjFromHandle( bb_int_t handle,BBObjType *type );
void	 BBCALL _bbNullObjEx();

void	 BBCALL _bbRestore( BBData *data );
bb_int_t		 BBCALL _bbReadInt();
bb_float_t	 BBCALL _bbReadFloat();
BBStr *	 BBCALL _bbReadStr();

bb_int_t		 BBCALL _bbAbs( bb_int_t n );
bb_int_t		 BBCALL _bbSgn( bb_int_t n );
bb_int_t		 BBCALL _bbMod( bb_int_t x,bb_int_t y );
bb_float_t	 BBCALL _bbFAbs( bb_float_t n );
bb_float_t	 BBCALL _bbFSgn( bb_float_t n );
bb_float_t	 BBCALL _bbFMod( bb_float_t x,bb_float_t y );
bb_float_t	 BBCALL _bbFPow( bb_float_t x,bb_float_t y );

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

extern BBType _bbIntType;
extern BBType _bbFltType;
extern BBType _bbStrType;
extern BBType _bbCStrType;

#ifdef __cplusplus
}
#endif

#include "commands.h"

#endif
