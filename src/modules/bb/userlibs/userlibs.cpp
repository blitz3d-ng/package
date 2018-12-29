
#include "userlibs.h"

#include <windows.h>
#include <vector>
using namespace std;

static vector<HMODULE> _mods;

struct Str{
	char *p;
	int size;
};

static Str _strs[256];
static int _nextStr;

static void libNotFound(){
	RTEX( "User lib not found" );
}

static void procNotFound(){
	RTEX( "User lib function not found" );
}

void BBCALL _bbLoadLibs( char *p ){

	string home;

	if( const char *t=getenv( "blitzpath" ) ) home=t;

	while( *p ){
		HMODULE mod=LoadLibrary( p );
		if( !mod && home.size() ){
			mod=LoadLibrary( (home+"/userlibs/"+p).c_str() );
		}
		p+=strlen(p)+1;
		if( mod ){
			_mods.push_back( mod );
			while( *p ){
				void *proc=(void*)GetProcAddress( mod,p );
				p+=strlen(p)+1;
				void *ptr=*(void**)p;
				p+=4;
				*(void**)ptr=proc ? proc : (void*)procNotFound;
			}
		}else{
			while( *p ){
				p+=strlen(p)+1;
				void *ptr=*(void**)p;
				p+=4;
				*(void**)ptr=(void*)libNotFound;
			}
		}
		++p;
	}
}

const char* BBCALL _bbStrToCStr( BBStr *str ){

	Str &t=_strs[_nextStr++ & 255];

	int size=str->size();

	if( !t.p || t.size<size ){
		delete[] t.p;
		t.p=new char[size+1];
		t.size=size;
	}

	memcpy( t.p,str->data(),size );
	t.p[size]=0;
	delete str;
	return t.p;
}

BBStr*	BBCALL	 _bbCStrToStr( const char *str ){
	return new BBStr( str );
}

BBMODULE_CREATE( userlibs ){
	return true;
}

BBMODULE_DESTROY( userlibs ){
	for( ;_mods.size();_mods.pop_back() ) FreeLibrary( _mods.back() );
	return true;
}
