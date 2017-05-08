#ifndef BB_FILESYSTEM_POSIX_H
#define BB_FILESYSTEM_POSIX_H

#include <bb/filesystem/filesystem.h>

class PosixFileSystem : public BBFileSystem{
public:
	PosixFileSystem();
	~PosixFileSystem();

	bool createDir( const std::string &dir );
	bool deleteDir( const std::string &dir );
	bool createFile( const std::string &file );
	bool deleteFile( const std::string &file );
	bool copyFile( const std::string &src,const std::string &dest );
	bool renameFile( const std::string &src,const std::string &dest );
	bool setCurrentDir( const std::string &dir );

	std::string getCurrentDir()const;
	int getFileSize( const std::string &name )const;
	int getFileType( const std::string &name )const;

	BBDir *openDir( const std::string &name,int flags );
	BBDir *verifyDir( BBDir *d );
	void closeDir( BBDir *dir );
};

#endif
