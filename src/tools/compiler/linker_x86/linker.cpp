#include "std.h"
#include "linker.h"
#include "image_util.h"

#include <cstring>

class X86Module : public Module{
public:
	X86Module();
	X86Module( std::istream &in );
	~X86Module();

	void *link( Module *libs );
	bool createExe( const char *exe_file,const char *dll_file );

	int getPC();

	void emit( int byte );
	void emitw( int word );
	void emitd( int dword );
	void emitx( void *mem,int sz );
	bool addSymbol( const char *sym,int pc );
	bool addReloc( const char *dest_sym,int pc,bool pcrel );

	bool findSymbol( const char *sym,int *pc );

private:
	char *data;
	int data_sz,pc;
	bool linked;

	std::map<std::string,int> symbols;
	std::map<int,std::string> rel_relocs,abs_relocs;

	bool findSym( const std::string &t,Module *libs,int *n ){
		if( findSymbol( t.c_str(),n ) ) return true;
		if( libs->findSymbol( t.c_str(),n ) ) return true;
		std::string err="Symbol '"+t+"' not found";
#ifdef BB_WINDOWS
		MessageBox( GetDesktopWindow(),err.c_str(),"Blitz Linker Error",MB_TOPMOST|MB_SETFOREGROUND );
#else
		std::cerr<<err<<std::endl;
#endif
		return false;
	}

	void ensure( int n ){
		if( pc+n<=data_sz ) return;
		data_sz=data_sz/2+data_sz;
		if( data_sz<pc+n ) data_sz=pc+n;
		char *old_data=data;
		data=d_new char[data_sz];
		memcpy( data,old_data,pc );
		delete old_data;
	}
};

X86Module::X86Module():data(0),data_sz(0),pc(0),linked(false){
}

X86Module::~X86Module(){
#ifdef BB_WINDOWS // TODO: probably memleak...
	if( linked ) VirtualFree( data,0,MEM_RELEASE );
	else delete[] data;
#endif
}

void *X86Module::link( Module *libs ){
#ifdef BB_WINDOWS
	if( linked ) return data;

	int dest;
	std::map<int,std::string>::iterator it;

	char *p=(char*)VirtualAlloc( 0,pc,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE );
	memcpy( p,data,pc );
	delete[] data;
	data=p;

	linked=true;

	for( it=rel_relocs.begin();it!=rel_relocs.end();++it ){
		if( !findSym( it->second,libs,&dest ) ) return 0;
		int *p=(int*)(data+it->first);*p+=(dest-(bb_int_t)p);
	}

	for( it=abs_relocs.begin();it!=abs_relocs.end();++it ){
		if( !findSym( it->second,libs,&dest ) ) return 0;
		int *p=(int*)(data+it->first);*p+=dest;
	}

	return data;
#else
	return 0;
#endif
}

int X86Module::getPC(){
	return pc;
}

void X86Module::emit( int byte ){
	ensure(1);data[pc++]=byte;
}

void X86Module::emitw( int word ){
	ensure(2);*(short*)(data+pc)=word;pc+=2;
}

void X86Module::emitd( int dword ){
	ensure(4);*(int*)(data+pc)=dword;pc+=4;
}

void X86Module::emitx( void *mem,int sz ){
	ensure(sz);memcpy( data+pc,mem,sz );pc+=sz;
}

bool X86Module::addSymbol( const char *sym,int pc ){
	std::string t(sym);
	if( symbols.find( t )!=symbols.end() ) return false;
	symbols[t]=pc;return true;
}

bool X86Module::addReloc( const char *dest_sym,int pc,bool pcrel ){
	std::map<int,std::string> &rel=pcrel ? rel_relocs : abs_relocs;
	if( rel.find( pc )!=rel.end() ) return false;
	rel[pc]=std::string(dest_sym);return true;
}

bool X86Module::findSymbol( const char *sym,int *pc ){
	std::string t=std::string(sym);
	std::map<std::string,int>::iterator it=symbols.find( t );
	if( it==symbols.end() ) return false;
	*pc=it->second + (bb_int_t)data;
	return true;
}

int Linker::version(){
	return VERSION;
}

bool Linker::canCreateExe(){
#ifdef DEMO
	return false;
#else
	return true;
#endif
}

Module *Linker::createModule(){
	return d_new X86Module();
}

void Linker::deleteModule( Module *mod ){
	delete mod;
}

Linker * CDECL linkerGetLinker(){
	static Linker linker;return &linker;
}

bool X86Module::createExe( const char *exe_file,const char *dll_file ){
#ifdef BB_WINDOWS
#ifdef DEMO
	return false;
#else

	//find proc address of bbWinMain
	HMODULE hmod=LoadLibrary( dll_file );if( !hmod ) return false;
	bb_int_t proc=(bb_int_t)GetProcAddress( hmod,"_bbWinMain@0" );
	bb_int_t entry=proc-(bb_int_t)hmod;FreeLibrary( hmod );if( !proc ) return false;

	if( !CopyFile( dll_file,exe_file,false ) ) return false;

	if( !openImage( exe_file ) ) return false;

	makeExe( entry );

	//create module
	//code size: code...
	//num_syms:  name,val...
	//num_rels:  name,val...
	//num_abss:  name,val...
	//
	qstreambuf buf;
	std::iostream out( &buf );

	std::map<std::string,int>::iterator it;
	std::map<int,std::string>::iterator rit;

	//write the code
	int sz=pc;out.write( (char*)&sz,4 );out.write( data,pc );

	//write symbols
	sz=symbols.size();out.write( (char*)&sz,4 );
	for( it=symbols.begin();it!=symbols.end();++it ){
		std::string t=it->first+'\0';
		out.write( t.data(),t.size() );
		sz=it->second;out.write( (char*)&sz,4 );
	}

	//write relative relocs
	sz=rel_relocs.size();out.write( (char*)&sz,4 );
	for( rit=rel_relocs.begin();rit!=rel_relocs.end();++rit ){
		std::string t=rit->second+'\0';
		out.write( t.data(),t.size() );
		sz=rit->first;out.write( (char*)&sz,4 );
	}

	//write absolute relocs
	sz=abs_relocs.size();out.write( (char*)&sz,4 );
	for( rit=abs_relocs.begin();rit!=abs_relocs.end();++rit ){
		std::string t=rit->second+'\0';
		out.write( t.data(),t.size() );
		sz=rit->first;out.write( (char*)&sz,4 );
	}

	replaceRsrc( 10,1111,1033,buf.data(),buf.size() );

	closeImage();

	return true;
#endif
#else
	return false;
#endif
}
