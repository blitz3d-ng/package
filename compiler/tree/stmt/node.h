
#ifndef STMTNODE_H
#define STMTNODE_H

#include "../node.h"

struct StmtNode : public Node{
	int pos;	//offset in source stream
	StmtNode():pos(-1){}
	void debug( int pos,Codegen *g );

	virtual void semant( Environ *e ){}
	virtual void translate( Codegen *g ){}
};

#endif
