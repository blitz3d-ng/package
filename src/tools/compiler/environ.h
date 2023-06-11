
/*

  An environ represent a stack frame block.

  */

#ifndef ENVIRON_H
#define ENVIRON_H

#include <string>
#include <vector>
#include <list>

#include "tree/type.h"
#include "tree/decl.h"
#include "tree/label.h"

class Environ{
public:
	int level;
	DeclSeq *decls;
	DeclSeq *funcDecls;
	DeclSeq *typeDecls;

	std::vector<Type*> types;

	std::vector<Label*> labels;
	Environ *globals;
	Type *returnType;
	std::string funcLabel,breakLabel;
	std::list<Environ*> children;		//for delete!
	bool strict;

	Environ( const std::string &f,Type *r,int l,Environ *gs );
	~Environ();

	Decl *findDecl( const std::string &s );
	Decl *findFunc( const std::string &s );
	Type *findType( const std::string &s );
	Label *findLabel( const std::string &s );
	Label *insertLabel( const std::string &s,int def,int src,int sz );

	std::string setBreak( const std::string &s );
};

#endif
