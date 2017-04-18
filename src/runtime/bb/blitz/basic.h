
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

extern BBType _bbIntType;
extern BBType _bbFltType;
extern BBType _bbStrType;
extern BBType _bbCStrType;

#endif
