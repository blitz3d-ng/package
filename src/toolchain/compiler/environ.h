
/*

  An environ represent a stack frame block.

  */

#ifndef ENVIRON_H
#define ENVIRON_H

#include <string>
#include <vector>
#include <list>
using namespace std;

#ifdef USE_LLVM
#include <llvm/IR/Value.h>
#endif

#include "tree/type.h"
#include "tree/decl.h"
#include "tree/label.h"

class Environ{
public:
	int level;
	DeclSeq *decls;
	DeclSeq *funcDecls;
	DeclSeq *typeDecls;

	vector<Type*> types;

	vector<Label*> labels;
	Environ *globals;
	Type *returnType;
	string funcLabel,breakLabel;
	list<Environ*> children;		//for delete!

	Environ( const string &f,Type *r,int l,Environ *gs );
	~Environ();

	Decl *findDecl( const string &s );
	Decl *findFunc( const string &s );
	Type *findType( const string &s );
	Label *findLabel( const string &s );
	Label *insertLabel( const string &s,int def,int src,int sz );

	string setBreak( const string &s );

#ifdef USE_LLVM
	std::string rt;
	vector<std::string> modules;

	map<std::string,llvm::Value*> allocas;
	llvm::BasicBlock *setBreak( llvm::BasicBlock *b );
#endif
};

#endif
