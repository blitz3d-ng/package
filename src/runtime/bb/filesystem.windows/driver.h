
#ifndef GXFILESYSTEM_H
#define GXFILESYSTEM_H

#include <string>
#include <bb/filesystem/driver.h>

class WindowsFileSystem : public BBFileSystem{
public:
	WindowsFileSystem();
	~WindowsFileSystem();

	/***** GX INTERFACE *****/
public:
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
