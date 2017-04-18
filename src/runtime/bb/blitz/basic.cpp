
#include "../../stdutil/stdutil.h"
#include "blitz.h"
#include "app.h"
#include "debug.h"

#include <math.h>
#include <map>
using namespace std;

//how many strings allocated
static int stringCnt;

//how many objects new'd but not deleted
static int objCnt;

//how many objects deleted but not released
static int unrelObjCnt;

//how many objects to alloc per block
static const int OBJ_NEW_INC=512;

//how many strings to alloc per block
static const int STR_NEW_INC=512;

//current data ptr
static BBData *dataPtr;

//chunks of mem - WHAT THE FUCK WAS I ON?!?!?!?
//static list<char*> memBlks;

//strings
static BBStr usedStrs,freeStrs;

//object handle number
static int next_handle;

//object<->handle maps
static map<int,BBObj*> handle_map;
static map<BBObj*,int> object_map;

BBType _bbIntType( BBTYPE_INT );
BBType _bbFltType( BBTYPE_FLT );
BBType _bbStrType( BBTYPE_STR );
BBType _bbCStrType( BBTYPE_CSTR );

static void *bbMalloc( int size ){
	return malloc(size);
/*
	char *c=d_new char[ size ];
	memBlks.push_back( c );
	return c;
*/
}

static void bbFree( void *q ){
	free(q);
/*
	if( !q ) return;
	char *c=(char*)q;
	memBlks.remove( c );
	delete [] c;
*/
}

static void removeStr( BBStr *str ){
	str->next->prev=str->prev;
	str->prev->next=str->next;
}

static void insertStr( BBStr *str,BBStr *next ){
	str->next=next;
	str->prev=next->prev;
	str->prev->next=str;
	next->prev=str;
}

void *BBStr::operator new( size_t size ){
	if( freeStrs.next==&freeStrs ){
		BBStr *t=(BBStr*)bbMalloc( sizeof(BBStr)*STR_NEW_INC );
		for( int k=0;k<STR_NEW_INC;++k ) insertStr( t++,&freeStrs );
	}
	BBStr *t=freeStrs.next;
	removeStr( t );insertStr( t,&usedStrs );
	return t;
}

void BBStr::operator delete( void *q ){
	if( !q ) return;
	BBStr *t=(BBStr*)q;
	removeStr( t );insertStr( t,&freeStrs );
}

BBStr::BBStr(){
	++stringCnt;
}

BBStr::BBStr( const char *s ):string(s){
	++stringCnt;
}

BBStr::BBStr( const char *s,int n ):string(s,n){
	++stringCnt;
}

BBStr::BBStr( const BBStr &s ):string(s){
	++stringCnt;
}

BBStr::BBStr( const string &s ):string(s){
	++stringCnt;
}

BBStr &BBStr::operator=( const char *s ){
	string::operator=( s );return *this;
}

BBStr &BBStr::operator=( const BBStr &s ){
	string::operator=( s );return *this;
}

BBStr &BBStr::operator=( const string &s ){
	string::operator=( s );return *this;
}

BBStr::~BBStr(){
	--stringCnt;
}

BBStr * BBCALL _bbStrLoad( BBStr **var ){
	return *var ? d_new BBStr( **var ) : d_new BBStr();
}

void BBCALL _bbStrRelease( BBStr *str ){
	delete str;
}

void BBCALL _bbStrStore( BBStr **var,BBStr *str ){
	_bbStrRelease( *var );*var=str;
}

BBStr * BBCALL _bbStrConcat( BBStr *s1,BBStr *s2 ){
	*s1+=*s2;delete s2;return s1;
}

int BBCALL _bbStrCompare( BBStr *lhs,BBStr *rhs ){
	int n=lhs->compare( *rhs );
	delete lhs;delete rhs;return n;
}

int BBCALL _bbStrToInt( BBStr *s ){
	int n=atoi( *s );
	delete s;return n;
}

BBStr * BBCALL _bbStrFromInt( int n ){
	return d_new BBStr( itoa( n ) );
}

float BBCALL _bbStrToFloat( BBStr *s ){
	float n=(float)atof( *s );
	delete s;return n;
}

BBStr * BBCALL _bbStrFromFloat( float n ){
	return d_new BBStr( ftoa( n ) );
}

BBStr * BBCALL _bbStrConst( const char *s ){
	return d_new BBStr( s );
}

void * BBCALL _bbVecAlloc( BBVecType *type ){
	void *vec=bbMalloc( type->size*4 );
	memset( vec,0,type->size*4 );
	return vec;
}

void BBCALL _bbVecFree( void *vec,BBVecType *type ){
	if( type->elementType->type==BBTYPE_STR ){
		BBStr **p=(BBStr**)vec;
		for( int k=0;k<type->size;++p,++k ){
			if( *p ) _bbStrRelease( *p );
		}
	}else if( type->elementType->type==BBTYPE_OBJ ){
		BBObj **p=(BBObj**)vec;
		for( int k=0;k<type->size;++p,++k ){
			if( *p ) _bbObjRelease( *p );
		}
	}
	bbFree( vec );
}

void BBCALL _bbVecBoundsEx(){
	RTEX( "Blitz array index out of bounds" );
}

void BBCALL _bbUndimArray( BBArray *array ){
	if( void *t=array->data ){
		if( array->elementType==BBTYPE_STR ){
			BBStr **p=(BBStr**)t;
			int size=array->scales[array->dims-1];
			for( int k=0;k<size;++p,++k ){
				if( *p ) _bbStrRelease( *p );
			}
		}else if( array->elementType==BBTYPE_OBJ ){
			BBObj **p=(BBObj**)t;
			int size=array->scales[array->dims-1];
			for( int k=0;k<size;++p,++k ){
				if( *p ) _bbObjRelease( *p );
			}
		}
		bbFree( t );
		array->data=0;
	}
}

void BBCALL _bbDimArray( BBArray *array ){
	int k;
	for( k=0;k<array->dims;++k ) ++array->scales[k];
	for( k=1;k<array->dims;++k ){
		array->scales[k]*=array->scales[k-1];
	}
	int size=array->scales[array->dims-1];
	array->data=bbMalloc( size*4 );
	memset( array->data,0,size*4 );
}

void BBCALL _bbArrayBoundsEx(){
	RTEX( "Array index out of bounds" );
}

static void unlinkObj( BBObj *obj ){
	obj->next->prev=obj->prev;
	obj->prev->next=obj->next;
}

static void insertObj( BBObj *obj,BBObj *next ){
	obj->next=next;
	obj->prev=next->prev;
	next->prev->next=obj;
	next->prev=obj;
}

BBObj * BBCALL _bbObjNew( BBObjType *type ){
	if( type->free.next==&type->free ){
		int obj_size=sizeof(BBObj)+type->fieldCnt*4;
		BBObj *o=(BBObj*)bbMalloc( obj_size*OBJ_NEW_INC );
		for( int k=0;k<OBJ_NEW_INC;++k ){
			insertObj( o,&type->free );
			o=(BBObj*)( (char*)o+obj_size );
		}
	}
	BBObj *o=type->free.next;
	unlinkObj( o );
	o->type=type;
	o->ref_cnt=1;
	o->fields=(BBField*)(o+1);
	for( int k=0;k<type->fieldCnt;++k ){
		switch( type->fieldTypes[k]->type ){
		case BBTYPE_VEC:
			o->fields[k].VEC=_bbVecAlloc( (BBVecType*)type->fieldTypes[k] );
			break;
		default:
			o->fields[k].INT=0;
		}
	}
	insertObj( o,&type->used );
	++unrelObjCnt;
	++objCnt;
	return o;
}

void BBCALL _bbObjDelete( BBObj *obj ){
	if( !obj ) return;
	BBField *fields=obj->fields;
	if( !fields ) return;
	BBObjType *type=obj->type;
	for( int k=0;k<type->fieldCnt;++k ){
		switch( type->fieldTypes[k]->type ){
		case BBTYPE_STR:
			_bbStrRelease( fields[k].STR );
			break;
		case BBTYPE_OBJ:
			_bbObjRelease( fields[k].OBJ );
			break;
		case BBTYPE_VEC:
			_bbVecFree( fields[k].VEC,(BBVecType*)type->fieldTypes[k] );
			break;
		}
	}
	map<BBObj*,int>::iterator it=object_map.find( obj );
	if( it!=object_map.end() ){
		handle_map.erase( it->second );
		object_map.erase( it );
	}
	obj->fields=0;
	_bbObjRelease( obj );
	--objCnt;
}

void BBCALL _bbObjDeleteEach( BBObjType *type ){
	BBObj *obj=type->used.next;
	while( obj->type ){
		BBObj *next=obj->next;
		if( obj->fields ) _bbObjDelete( obj );
		obj=next;
	}
}

extern void BBCALL _bbDebugLog( BBStr *t );
extern void BBCALL bbStop( );

void BBCALL _bbObjRelease( BBObj *obj ){
	if( !obj || --obj->ref_cnt ) return;
	unlinkObj( obj );
	insertObj( obj,&obj->type->free );
	--unrelObjCnt;
}

void BBCALL _bbObjStore( BBObj **var,BBObj *obj ){
	if( obj ) ++obj->ref_cnt;	//do this first incase of self-assignment
	_bbObjRelease( *var );
	*var=obj;
}

int BBCALL _bbObjCompare( BBObj *o1,BBObj *o2 ){
	return (o1 ? o1->fields : 0)!=(o2 ? o2->fields : 0);
}

BBObj * BBCALL _bbObjNext( BBObj *obj ){
	do{
		obj=obj->next;
		if( !obj->type ) return 0;
	}while( !obj->fields );
	return obj;
}

BBObj * BBCALL _bbObjPrev( BBObj *obj ){
	do{
		obj=obj->prev;
		if( !obj->type ) return 0;
	}while( !obj->fields );
	return obj;
}

BBObj * BBCALL _bbObjFirst( BBObjType *type ){
	return _bbObjNext( &type->used );
}

BBObj * BBCALL _bbObjLast( BBObjType *type ){
	return _bbObjPrev( &type->used );
}

void BBCALL _bbObjInsBefore( BBObj *o1,BBObj *o2 ){
	if( o1==o2 ) return;
	unlinkObj( o1 );
	insertObj( o1,o2 );
}

void BBCALL _bbObjInsAfter( BBObj *o1,BBObj *o2 ){
	if( o1==o2 ) return;
	unlinkObj( o1 );
	insertObj( o1,o2->next );
}

int BBCALL _bbObjEachFirst( BBObj **var,BBObjType *type ){
	_bbObjStore( var,_bbObjFirst( type ) );
	return *var!=0;
}

int BBCALL _bbObjEachNext( BBObj **var ){
	_bbObjStore( var,_bbObjNext( *var ) );
	return *var!=0;
}

int BBCALL _bbObjEachFirst2( BBObj **var,BBObjType *type ){
	*var=_bbObjFirst( type );
	return *var!=0;
}

int BBCALL _bbObjEachNext2( BBObj **var ){
	*var=_bbObjNext( *var );
	return *var!=0;
}

BBStr * BBCALL _bbObjToStr( BBObj *obj ){
	if( !obj || !obj->fields ) return d_new BBStr( "[NULL]" );

	static BBObj *root;
	static int recurs_cnt;

	if( obj==root ) return d_new BBStr( "[ROOT]" );
	if( recurs_cnt==8 ) return d_new BBStr( "...." );

	++recurs_cnt;
	BBObj *oldRoot=root;
	if( !root ) root=obj;

	BBObjType *type=obj->type;
	BBField *fields=obj->fields;
	BBStr *s=d_new BBStr("["),*t;
	for( int k=0;k<type->fieldCnt;++k ){
		if( k ) *s+=',';
		switch( type->fieldTypes[k]->type ){
		case BBTYPE_INT:
			t=_bbStrFromInt( fields[k].INT );*s+=*t;delete t;
			break;
		case BBTYPE_FLT:
			t=_bbStrFromFloat( fields[k].FLT );*s+=*t;delete t;
			break;
		case BBTYPE_STR:
			if( fields[k].STR ) *s+='\"'+*fields[k].STR+'\"';
			else *s+="\"\"";
			break;
		case BBTYPE_OBJ:
			t=_bbObjToStr( fields[k].OBJ );*s+=*t;delete t;
			break;
		default:
			*s+="???";
		}
	}
	*s+=']';
	root=oldRoot;
	--recurs_cnt;
	return s;
}

int BBCALL _bbObjToHandle( BBObj *obj ){
	if( !obj || !obj->fields ) return 0;
	map<BBObj*,int>::const_iterator it=object_map.find( obj );
	if( it!=object_map.end() ) return it->second;
	++next_handle;
	object_map[obj]=next_handle;
	handle_map[next_handle]=obj;
	return next_handle;
}

BBObj * BBCALL _bbObjFromHandle( int handle,BBObjType *type ){
	map<int,BBObj*>::const_iterator it=handle_map.find( handle );
	if( it==handle_map.end() ) return 0;
	BBObj *obj=it->second;
	return obj->type==type ? obj : 0;
}

void BBCALL _bbNullObjEx(){
	RTEX( "Object does not exist" );
}

void BBCALL _bbRestore( BBData *data ){
	dataPtr=data;
}

int BBCALL _bbReadInt(){
	switch( dataPtr->fieldType ){
	case BBTYPE_END:RTEX( "Out of data" );return 0;
	case BBTYPE_INT:return dataPtr++->field.INT;
	case BBTYPE_FLT:return dataPtr++->field.FLT;
	case BBTYPE_CSTR:return atoi( dataPtr++->field.CSTR );
	default:RTEX( "Bad data type" );return 0;
	}
}

float BBCALL _bbReadFloat(){
	switch( dataPtr->fieldType ){
	case BBTYPE_END:RTEX( "Out of data" );return 0;
	case BBTYPE_INT:return dataPtr++->field.INT;
	case BBTYPE_FLT:return dataPtr++->field.FLT;
	case BBTYPE_CSTR:return atof( dataPtr++->field.CSTR );
	default:RTEX( "Bad data type" );return 0;
	}
}

BBStr * BBCALL _bbReadStr(){
	switch( dataPtr->fieldType ){
	case BBTYPE_END:RTEX( "Out of data" );return 0;
	case BBTYPE_INT:return d_new BBStr( itoa( dataPtr++->field.INT ) );
	case BBTYPE_FLT:return d_new BBStr( ftoa( dataPtr++->field.FLT ) );
	case BBTYPE_CSTR:return d_new BBStr( dataPtr++->field.CSTR );
	default:RTEX( "Bad data type" );return 0;
	}
}

int BBCALL _bbAbs( int n ){
	return n>=0 ? n : -n;
}

int BBCALL _bbSgn( int n ){
	return n>0 ? 1 : (n<0 ? -1 : 0);
}

int BBCALL _bbMod( int x,int y ){
	return x%y;
}

float BBCALL _bbFAbs( float n ){
	return n>=0 ? n : -n;
}

float BBCALL _bbFSgn( float n ){
	return n>0.0f ? 1.0f : (n<0.0f ? -1.0f : 0.0f);
}

float BBCALL _bbFMod( float x,float y ){
	return (float)fmod( x,y );
}

float BBCALL _bbFPow( float x,float y ){
	return (float)pow( x,y );
}

void BBCALL bbRuntimeStats(){
	_bbDebugLog( ("Active strings :"+itoa(stringCnt)).c_str() );
	_bbDebugLog( ("Active objects :"+itoa(objCnt)).c_str() );
	_bbDebugLog( ("Unreleased objs:"+itoa(unrelObjCnt)).c_str() );
	/*
	clog<<"Active strings:"<<stringCnt<<endl;
	clog<<"Active objects:"<<objCnt<<endl;
	clog<<"Unreleased Objects:"<<unrelObjCnt<<endl;
	for( BBStr *t=usedStrs.next;t!=&usedStrs;t=t->next ){
		clog<<"string@"<<(void*)t<<endl;
	}
	*/
}

BBMODULE_CREATE( blitz ){
	next_handle=0;
//	memBlks.clear();
	handle_map.clear();
	object_map.clear();
	stringCnt=objCnt=unrelObjCnt=0;
	usedStrs.next=usedStrs.prev=&usedStrs;
	freeStrs.next=freeStrs.prev=&freeStrs;
	return true;
}

BBMODULE_DESTROY( blitz ){
	while( usedStrs.next!=&usedStrs ) delete usedStrs.next;
//	while( memBlks.size() ) bbFree( memBlks.back() );
	handle_map.clear();
	object_map.clear();
	return true;
}
