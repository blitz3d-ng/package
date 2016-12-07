
#ifndef BBSTRING_H
#define BBSTRING_H

#include "basic.h"

BBStr *  BBCALL bbString( BBStr *s,int n );
BBStr *	 BBCALL bbLeft( BBStr *s,int n );
BBStr *	 BBCALL bbRight( BBStr *s,int n );
BBStr *  BBCALL bbReplace( BBStr *s,BBStr *from,BBStr *to );
int		 BBCALL bbInstr( BBStr *s,BBStr *t,int from );
BBStr *	 BBCALL bbMid( BBStr *s,int o,int n );
BBStr *  BBCALL bbUpper( BBStr *s );
BBStr *	 BBCALL bbLower( BBStr *s );
BBStr *  BBCALL bbTrim( BBStr *s );
BBStr *  BBCALL bbLSet( BBStr *s,int n );
BBStr *  BBCALL bbRSet( BBStr *s,int n );
BBStr *  BBCALL bbChr( int n );
int		 BBCALL bbAsc( BBStr *s );
int		 BBCALL bbLen( BBStr *s );
BBStr *	 BBCALL bbHex( int n );
BBStr *  BBCALL bbBin( int n );
BBStr *  BBCALL bbCurrentDate();
BBStr *  BBCALL bbCurrentTime();

#endif
