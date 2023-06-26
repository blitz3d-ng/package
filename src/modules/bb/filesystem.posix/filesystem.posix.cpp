
#include "../stdutil/stdutil.h"
#include "filesystem.posix.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

class PosixDir : public BBDir{
public:
	PosixDir( DIR *dp ):dp(dp){
	}

	~PosixDir(){
		closedir( dp );
	}

private:
	DIR *dp;

public:
	std::string getNextFile(){
		struct dirent *ep=readdir( dp );
		if( ep ){
			return std::string( ep->d_name );
		}
		return "";
	}
};


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
	char buff[PATH_MAX];
	if( getcwd( buff,sizeof(buff) ) ){
		std::string t=buff;
		if( t.size() && t[t.size()-1]!='/' ) t+='/';
		return t;
	}
	return "";
}

int PosixFileSystem::getFileSize( const std::string &name )const{
	struct stat fstat;
	if( stat( name.c_str(),&fstat )==0 ){
		return fstat.st_size;;
	}
	return 0;
}

int PosixFileSystem::getFileType( const std::string &name )const{
	struct stat fstat;
	if( stat( name.c_str(),&fstat )==0 ){
		if( S_ISREG( fstat.st_mode ) ){
			return 1;
		}else if( S_ISDIR( fstat.st_mode ) ){
			return 2;
		}
	}

	return 0;
}

BBDir *PosixFileSystem::openDir( const std::string &name,int flags ){
	DIR *dp = opendir( name.c_str() );
	if( !dp ) return 0;

	return d_new PosixDir( dp );
}

BBDir *PosixFileSystem::verifyDir( BBDir *d ){
	return d; // TODO
}

void PosixFileSystem::closeDir( BBDir *dir ){
	delete dir;
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
