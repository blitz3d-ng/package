#include "dpi.h"

#ifdef _WIN32

#include <windows.h>

bool _dpi_scale_calculated=false;
float dpi_scale_x=1.0f;
float dpi_scale_y=1.0f;

void _CalculateDPIScale(){
  if ( _dpi_scale_calculated ) return;

  HDC hdc = GetDC( GetDesktopWindow() );
	dpi_scale_x = GetDeviceCaps( hdc,LOGPIXELSX ) / 96.0f;
	dpi_scale_y = GetDeviceCaps( hdc,LOGPIXELSY ) / 96.0f;
	ReleaseDC( GetDesktopWindow(),hdc );
  _dpi_scale_calculated=true;
}

float GetDPIScaleX(){ _CalculateDPIScale();return dpi_scale_x; }
float GetDPIScaleY(){ _CalculateDPIScale();return dpi_scale_y; }

#else

float GetDPIScaleX() { return 1.0f; }
float GetDPIScaleY() { return 1.0f; }

#endif
