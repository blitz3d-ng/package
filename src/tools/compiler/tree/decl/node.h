#ifndef DECLNODE_H
#define DECLNODE_H

#include "../node.h"

struct DeclNode : public Node{
	int pos;
	std::string file;
	DeclNode():pos(-1){}
	virtual void proto( DeclSeq *d,Environ *e ){}
	virtual void semant( Environ *e ){}
	virtual void translate( Codegen *g ){}
	virtual void transdata( Codegen *g ){}
#ifdef USE_LLVM
	virtual void translate2( Codegen_LLVM *g );
	virtual void transdata2( Codegen_LLVM *g );
#endif
};

#endif
