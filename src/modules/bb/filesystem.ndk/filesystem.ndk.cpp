#include "../stdutil/stdutil.h"
#include <bb/blitz/app.h>
#include <bb/system.ndk/system.ndk.h>
#include "filesystem.ndk.h"

#include <sys/stat.h>
#include <unistd.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

struct membuf:std::streambuf{
	membuf( char* begin,char* end ){
		this->setg( begin,begin,end );
	}

	pos_type seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which = std::ios_base::in) override{
		if( dir==std::ios_base::cur ){
			gbump( off );
		}else if( dir==std::ios_base::end ){
			setg( eback(),egptr()+off,egptr() );
		}else if( dir==std::ios_base::beg ){
			setg( eback(),eback()+off,egptr() );
		}
		return gptr()-eback();
	}

	pos_type seekpos( pos_type sp,std::ios_base::openmode which ) override{
		return seekoff( sp-pos_type(off_type(0)),std::ios_base::beg,which );
	}
};

NDKFileSystem::NDKFileSystem(){
}

NDKFileSystem::~NDKFileSystem(){
}

std::streambuf *NDKFileSystem::openFile( const std::string &file,ios_base::openmode n ){
	LOGD( "openFile %s",file.c_str() );

	NDKSystemDriver *sys=(NDKSystemDriver*)bbSystemDriver;

	jobject activity=sys->getActivityClass();
	JNIEnv *env=sys->getEnv();

	jclass cls = env->GetObjectClass( activity );
	jmethodID mid = env->GetMethodID( cls,"getAssets","()Landroid/content/res/AssetManager;" );
	if (mid == 0) {
		__android_log_print( ANDROID_LOG_WARN,"blitz3d","cannot find getAssets" );
		return 0;
	}

	jobject assetManager = env->CallObjectMethod( activity,mid );

	AAssetManager* mgr = AAssetManager_fromJava( env,assetManager );
	AAsset *asset = AAssetManager_open( mgr,file.c_str(),AASSET_MODE_STREAMING );

	if( !asset ) {
		LOGD("Cannot open asset: %s", file.c_str());
		return 0;
	}

	// TODO: stream it into the string...not efficient.
	const void *data = AAsset_getBuffer(asset);
	if( !data ){
		LOGD("Could not get data for: %s", file.c_str());
		AAsset_close(asset);
		return 0;
	}
	size_t size = AAsset_getLength64(asset);

	LOGD("Loaded file %s. size, %u.", file.c_str(), size);

	return new membuf( (char*)data, ((char*)data)+size  );
}

bool NDKFileSystem::createDir( const std::string &dir ){
	RTEX( "NDKFileSystem::createDir not implemented" );
}

bool NDKFileSystem::deleteDir( const std::string &dir ){
	RTEX( "NDKFileSystem::deleteDir not implemented" );
}

bool NDKFileSystem::createFile( const std::string &file ){
	RTEX( "NDKFileSystem::createFile not implemented" );
}

bool NDKFileSystem::deleteFile( const std::string &file ){
	RTEX( "NDKFileSystem::deleteFile not implemented" );
}

bool NDKFileSystem::copyFile( const std::string &src,const std::string &dest ){
	RTEX( "NDKFileSystem::copyFile not implemented" );
}

bool NDKFileSystem::renameFile( const std::string &src,const std::string &dest ){
	RTEX( "NDKFileSystem::renameFile not implemented" );
}

bool NDKFileSystem::setCurrentDir( const std::string &dir ){
	return chdir( dir.c_str() )==0;
}

std::string NDKFileSystem::getCurrentDir()const{
	RTEX( "NDKFileSystem::getCurrentDir not implemented" );
}

int NDKFileSystem::getFileSize( const std::string &name )const{
	RTEX( "NDKFileSystem::getFileSize not implemented" );
}

int NDKFileSystem::getFileType( const std::string &name )const{
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

BBDir *NDKFileSystem::openDir( const std::string &name,int flags ){
	RTEX( "NDKFileSystem::openDir not implemented" );
}

BBDir *NDKFileSystem::verifyDir( BBDir *d ){
	RTEX( "NDKFileSystem::verifyDir not implemented" );
}

void NDKFileSystem::closeDir( BBDir *dir ){
	RTEX( "NDKFileSystem::closeDir not implemented" );
}

BBMODULE_CREATE( filesystem_ndk ){
	if( !gx_filesys ){
		gx_filesys=d_new NDKFileSystem();
	}
	return true;
}

BBMODULE_DESTROY( filesystem_ndk ){
	return true;
}
