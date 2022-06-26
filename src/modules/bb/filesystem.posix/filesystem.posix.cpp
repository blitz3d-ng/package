
#include "../stdutil/stdutil.h"
#include "filesystem.posix.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

PosixFileSystem::PosixFileSystem(){
}

PosixFileSystem::~PosixFileSystem(){
}

bool PosixFileSystem::createDir( const std::string &dir ){
	RTEX( "PosixFileSystem::createDir not implemented" );
}

bool PosixFileSystem::deleteDir( const std::string &dir ){
	RTEX( "PosixFileSystem::deleteDir not implemented" );
}

bool PosixFileSystem::createFile( const std::string &file ){
	RTEX( "PosixFileSystem::createFile not implemented" );
}

bool PosixFileSystem::deleteFile( const std::string &file ){
	RTEX( "PosixFileSystem::deleteFile not implemented" );
}

bool PosixFileSystem::copyFile( const std::string &src,const std::string &dest ){
	RTEX( "PosixFileSystem::copyFile not implemented" );
}

bool PosixFileSystem::renameFile( const std::string &src,const std::string &dest ){
	RTEX( "PosixFileSystem::renameFile not implemented" );
}

bool PosixFileSystem::setCurrentDir( const std::string &dir ){
	return chdir( dir.c_str() )==0;
}

std::string PosixFileSystem::getCurrentDir()const{
	RTEX( "PosixFileSystem::getCurrentDir not implemented" );
}

int PosixFileSystem::getFileSize( const std::string &name )const{
	RTEX( "PosixFileSystem::getFileSize not implemented" );
}

int PosixFileSystem::getFileType( const std::string &name )const{
	struct stat fstat;
	stat( name.c_str(),&fstat );

	if( S_ISREG( fstat.st_mode ) ){
		return 1;
	}else if( S_ISDIR( fstat.st_mode ) ){
		return 2;
	}

	return 0;
}

BBDir *PosixFileSystem::openDir( const std::string &name,int flags ){
	RTEX( "PosixFileSystem::openDir not implemented" );
}

BBDir *PosixFileSystem::verifyDir( BBDir *d ){
	RTEX( "PosixFileSystem::verifyDir not implemented" );
}

void PosixFileSystem::closeDir( BBDir *dir ){
	RTEX( "PosixFileSystem::closeDir not implemented" );
}

BBMODULE_CREATE( filesystem_posix ){
	if( !gx_filesys ){
		gx_filesys=d_new PosixFileSystem();
	}
	return true;
}

BBMODULE_DESTROY( filesystem_posix ){
	return true;
}
