#ifndef BB_FILESYSTEM_NDK_H
#define BB_FILESYSTEM_NDK_H

#include <bb/filesystem/filesystem.h>
#include <android/native_window_jni.h>

class NDKFileSystem : public BBFileSystem{
public:
	NDKFileSystem();
	~NDKFileSystem();

	std::streambuf *openFile( const std::string &file,std::ios_base::openmode n );

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
