// AUTOGENERATED. DO NOT EDIT.
// RUN `make` TO UPDATE.

#include <bb/blitz/module.h>
#include <bb/string/string.h>

BBMODULE_LINK( string ){
	rtSym( "$String$string%repeat","bbString",bbString );
	rtSym( "$Left$string%count","bbLeft",bbLeft );
	rtSym( "$Right$string%count","bbRight",bbRight );
	rtSym( "$Replace$string$from$to","bbReplace",bbReplace );
	rtSym( "%Instr$string$find%from=1","bbInstr",bbInstr );
	rtSym( "$Mid$string%start%count=-1","bbMid",bbMid );
	rtSym( "$Upper$string","bbUpper",bbUpper );
	rtSym( "$Lower$string","bbLower",bbLower );
	rtSym( "$Trim$string","bbTrim",bbTrim );
	rtSym( "$LSet$string%size","bbLSet",bbLSet );
	rtSym( "$RSet$string%size","bbRSet",bbRSet );
	rtSym( "$Chr%ascii","bbChr",bbChr );
	rtSym( "%Asc$string","bbAsc",bbAsc );
	rtSym( "%Len$string","bbLen",bbLen );
	rtSym( "$Hex%value","bbHex",bbHex );
	rtSym( "$Bin%value","bbBin",bbBin );
}
