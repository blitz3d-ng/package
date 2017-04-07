
#include "std.h"
#include "bbfilesystem.h"
#include <bb/stream/stream.h>
#include <fstream>

#include "../gxruntime/gxruntime.h"
extern gxRuntime *gx_runtime;

BBFileSystem *gx_filesys;

struct bbFile : public bbStream{
	filebuf *buf;
	bbFile( filebuf *f ):buf(f){
	}
	~bbFile(){
		delete buf;
	}
	int read( char *buff,int size ){
		return buf->sgetn( (char*)buff,size );
	}
	int write( const char *buff,int size ){
		return buf->sputn( (char*)buff,size );
	}
	int avail(){
		return buf->in_avail();
	}
	int eof(){
		return buf->sgetc()==EOF;
	}
};

static set<bbFile*> file_set;

static inline void debugFile( bbFile *f ){
	if( bb_env.debug ){
		if( !file_set.count( f ) ) RTEX( "File does not exist" );
	}
}

static inline void debugDir( BBDir *d ){
	if( bb_env.debug ){
		if( !gx_filesys->verifyDir( d ) ) RTEX( "Directory does not exist" );
	}
}

static bbFile *open( BBStr *f,ios_base::openmode n ){
	string t=*f;
	filebuf *buf=d_new filebuf();
	if( buf->open( t.c_str(),n|ios_base::binary ) ){
		bbFile *f=d_new bbFile( buf );
		file_set.insert( f );
		return f;
	}
	delete buf;
	return 0;
}

bbFile* BBCALL bbReadFile( BBStr *f ){
	return open( f,ios_base::in );
}

bbFile* BBCALL bbWriteFile( BBStr *f ){
	return open( f,ios_base::out|ios_base::trunc );
}

bbFile* BBCALL bbOpenFile( BBStr *f ){
	return open( f,ios_base::in|ios_base::out );
}

void BBCALL bbCloseFile( bbFile *f ){
	debugFile( f );
	file_set.erase( f );
	delete f;
}

int BBCALL bbFilePos( bbFile *f ){
	return f->buf->pubseekoff( 0,ios_base::cur );
}

int BBCALL bbSeekFile( bbFile *f,int pos ){
	return f->buf->pubseekoff( pos,ios_base::beg );
}

BBDir* BBCALL bbReadDir( BBStr *d ){
	string t=*d;delete d;
	return gx_filesys->openDir( t,0 );
}

void BBCALL bbCloseDir( BBDir *d ){
	gx_filesys->closeDir( d );
}

BBStr* BBCALL bbNextFile( BBDir *d ){
	debugDir( d );
	return d_new BBStr( d->getNextFile() );
}

BBStr* BBCALL bbCurrentDir(){
	return d_new BBStr( gx_filesys->getCurrentDir() );
}

void BBCALL bbChangeDir( BBStr *d ){
	gx_filesys->setCurrentDir( *d );
	delete d;
}

void BBCALL bbCreateDir( BBStr *d ){
	gx_filesys->createDir( *d );
	delete d;
}

void BBCALL bbDeleteDir( BBStr *d ){
	gx_filesys->deleteDir( *d );
	delete d;
}

int BBCALL bbFileType( BBStr *f ){
	string t=*f;delete f;
	int n=gx_filesys->getFileType( t );
	return n==BBFileSystem::FILE_TYPE_FILE ? 1 : (n==BBFileSystem::FILE_TYPE_DIR ? 2 : 0);
}

int BBCALL bbFileSize( BBStr *f ){
	string t=*f;delete f;
	return gx_filesys->getFileSize( t );
}

void BBCALL bbCopyFile( BBStr *f,BBStr *to ){
	string src=*f,dest=*to;
	delete f;delete to;
	gx_filesys->copyFile( src,dest );
}

void BBCALL bbDeleteFile( BBStr *f ){
	gx_filesys->deleteFile( *f );
	delete f;
}

BBMODULE_CREATE( filesystem ){
	if( gx_filesys=gx_runtime->openFileSystem( 0 ) ){
		return true;
	}
	return false;
}

BBMODULE_DESTROY( filesystem ){
	while( file_set.size() ) bbCloseFile( *file_set.begin() );
	gx_runtime->closeFileSystem( gx_filesys );
	return true;
}

BBMODULE_LINK( filesystem ){
	rtSym( "%OpenFile$filename",bbOpenFile );
	rtSym( "%ReadFile$filename",bbReadFile );
	rtSym( "%WriteFile$filename",bbWriteFile );
	rtSym( "CloseFile%file_stream",bbCloseFile );
	rtSym( "%FilePos%file_stream",bbFilePos );
	rtSym( "%SeekFile%file_stream%pos",bbSeekFile );

	rtSym( "%ReadDir$dirname",bbReadDir );
	rtSym( "CloseDir%dir",bbCloseDir );
	rtSym( "$NextFile%dir",bbNextFile );
	rtSym( "$CurrentDir",bbCurrentDir );
	rtSym( "ChangeDir$dir",bbChangeDir );
	rtSym( "CreateDir$dir",bbCreateDir );
	rtSym( "DeleteDir$dir",bbDeleteDir );

	rtSym( "%FileSize$file",bbFileSize );
	rtSym( "%FileType$file",bbFileType );
	rtSym( "CopyFile$file$to",bbCopyFile );
	rtSym( "DeleteFile$file",bbDeleteFile );
}
