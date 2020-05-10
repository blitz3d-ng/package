#ifndef BB_SYSTEM_LINUX_H
#define BB_SYSTEM_LINUX_H

#include <bb/system/system.h>

class LinuxSystemDriver : public BBSystemDriver{
public:
  bool delay( int ms );

  bool execute( const std::string &cmd );
  int getMilliSecs();

  int getScreenWidth( int i );
  int getScreenHeight( int i );

  void dpiInfo( float &scale_x,float &scale_y );

	bool lookupFontData( const std::string &fontName,BBFontData &font );
};

#endif
