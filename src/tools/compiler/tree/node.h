
#ifndef NODE_H
#define NODE_H

#include <string>

#include "../ex.h"
#include "toker.h"
#include "../environ.h"
#include "../codegen.h"

struct VarNode;
struct ConstNode;

#define DEFAULT_NODE_JSON( name ) virtual json toJSON( Environ *e ){ json tree;tree["@class"]=#name; return tree; }

struct Node{
	virtual ~Node(){}

	//used user funcs...
	static std::set<std::string> usedfuncs;

	//helper funcs
	static void ex();
	static void ex( const std::string &e );
	static void ex( const std::string &e,int pos );
	static void ex( const std::string &e,int pos,const std::string &f );

	static std::string genLabel();
	static VarNode *genLocal( Environ *e,Type *ty );

	static TNode *compare( int op,TNode *l,TNode *r,Type *ty );
	static ConstNode *constValue( Type *ty );

	static int enumVars( Environ *e );
	static Type *tagType( const std::string &s,Environ *e );

	static TNode *createVars( Environ *e );
	static TNode *deleteVars( Environ *e );

	static TNode *seq( TNode *l,TNode *r );
	static TNode *move( TNode *src,TNode *dest );
	static TNode *global( const std::string &s );
	static TNode *local( int offset );
	static TNode *arg( int offset );
	static TNode *mem( TNode *ref );
	static TNode *add( TNode *l,TNode *r );
	static TNode *mul( TNode *l,TNode *r );
	static TNode *iconst( int n );
	static TNode *ret();
	static TNode *jsr( const std::string &s );
	static TNode *jump( const std::string &s );
	static TNode *jumpt( TNode *cond,const std::string &s );
	static TNode *jumpf( TNode *cond,const std::string &s );
	static TNode *jumpge( TNode *l,TNode *r,const std::string &s );
	static TNode *call( const std::string &func,TNode *a0=0,TNode *a1=0,TNode *a2=0 );
	static TNode *fcall( const std::string &func,TNode *a0=0,TNode *a1=0,TNode *a2=0 );

	virtual json toJSON( Environ *e )=0;

#ifdef USE_LLVM
	static llvm::Value *compare2( int op,llvm::Value *l,llvm::Value *r,Type *ty,Codegen_LLVM *g );
	static void createVars2( Environ *e, Codegen_LLVM *g );
#endif
};

#endif
