
#ifndef BBFILESYSTEM_H
#define BBFILESYSTEM_H

#include <bb/blitz/blitz.h>

class BBDir{
  /***** GX INTERFACE *****/
public:
  virtual ~BBDir();

  virtual std::string getNextFile()=0;
};

class BBFileSystem{
public:
  virtual ~BBFileSystem();
  /***** GX INTERFACE *****/
public:
  enum{
    FILE_TYPE_NONE=0,FILE_TYPE_FILE=1,FILE_TYPE_DIR=2
  };

  virtual bool createDir( const std::string &dir )=0;
  virtual bool deleteDir( const std::string &dir )=0;
  virtual bool createFile( const std::string &file )=0;
  virtual bool deleteFile( const std::string &file )=0;
  virtual bool copyFile( const std::string &src,const std::string &dest )=0;
  virtual bool renameFile( const std::string &src,const std::string &dest )=0;
  virtual bool setCurrentDir( const std::string &dir )=0;

  virtual std::string getCurrentDir()const=0;
  virtual int getFileSize( const std::string &name )const=0;
  virtual int getFileType( const std::string &name )const=0;

  virtual BBDir *openDir( const std::string &name,int flags )=0;
  virtual BBDir *verifyDir( BBDir *d )=0;
  virtual void closeDir( BBDir *dir )=0;
};

extern BBFileSystem *gx_filesys;

struct BBFile;

#include "commands.h"

#endif
