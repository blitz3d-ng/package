
#ifndef BASIC_H
#define BASIC_H

#include "module.h"
#include <string>

enum{
	BBTYPE_END=0,
	BBTYPE_INT=1,BBTYPE_FLT=2,
	BBTYPE_STR=3,BBTYPE_CSTR=4,
	BBTYPE_OBJ=5,BBTYPE_VEC=6
};

#pragma pack( push,1 )

struct BBObj;
struct BBStr;
struct BBType;
struct BBObjType;
struct BBVecType;
union  BBField;
struct BBArray;

struct BBObj{
	BBField *fields;
	BBObj *next,*prev;
	BBObjType *type;
	int ref_cnt;
};

struct BBType{
	int type;
	BBType( int n ):type(n){}
};

struct BBObjType : public BBType{
	BBObj used,free;
	int fieldCnt;
	BBType *fieldTypes[1];
};

struct BBVecType : public BBType{
	int size;
	BBType *elementType;
};

union BBField{
	int INT;
	float FLT;
	BBStr *STR;
	char *CSTR;
	BBObj *OBJ;
	void *VEC;
};

struct BBArray{
	void *data;
	int elementType,dims,scales[1];
};

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

struct BBData{
	int fieldType;
	BBField field;
};

#pragma pack( pop )

void basic_link();

// extern BBType _bbIntType;
// extern BBType _bbFltType;
// extern BBType _bbStrType;
// extern BBType _bbCStrType;

BBStr *	 BBCALL _bbStrLoad( BBStr **var );
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

#endif
