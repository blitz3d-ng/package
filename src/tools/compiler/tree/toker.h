
/*

  The Toker converts an inout stream into tokens for use by the parser.

  */

#ifndef TOKER_H
#define TOKER_H

#include <istream>
#include <map>
#include <vector>

enum{
	DIM=0x8000,GOTO,GOSUB,EXIT,RETURN,
	IF,THEN,ELSE,ENDIF,ELSEIF,
	WHILE,WEND,
	FOR,TO,STEP,NEXT,
	FUNCTION,ENDFUNCTION,
	TYPE,ENDTYPE,EACH,
	GLOBAL,LOCAL,FIELD,BBCONST,
	SELECT,CASE,DEFAULT,ENDSELECT,
	REPEAT,UNTIL,FOREVER,
	DATA,READ,RESTORE,
	ABS,SGN,MOD,
	PI,BBTRUE,BBFALSE,
	BBINT,BBFLOAT,BBSTR,
	INCLUDE,

	BBNEW,BBDELETE,FIRST,LAST,INSERT,BEFORE,AFTER,BBNULL,
	OBJECT,BBHANDLE,
	AND,OR,XOR,NOT,SHL,SHR,SAR,

	LE,GE,NE,
	IDENT,INTCONST,BINCONST,HEXCONST,FLOATCONST,STRINGCONST,

	BUNDLEIDENTIFIER,BUNDLEAPPNAME,BUNDLEFILE,
	STRICT
};

class Toker{
public:
	Toker( std::istream &in );

	int pos();
	int curr();
	int next();
	std::string text();
	int lookAhead( int n );

	static int chars_toked;

	static std::map<std::string,int> &getKeywords();

private:
	struct Toke{
		int n,from,to;
		Toke( int n,int f,int t ):n(n),from(f),to(t){}
	};
	std::istream &in;
	std::string line;
	std::vector<Toke> tokes;
	void nextline();
	int curr_row,curr_toke;
};

#endif
