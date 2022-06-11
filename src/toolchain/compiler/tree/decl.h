
#ifndef DECL_H
#define DECL_H

#include <json.hpp>
using namespace nlohmann;

#ifdef USE_LLVM
#include <llvm/IR/Value.h>
#endif

enum{
	DECL_FUNC=1,DECL_ARRAY=2,DECL_STRUCT=4,						//NOT vars
	DECL_GLOBAL=8,DECL_LOCAL=16,DECL_PARAM=32,DECL_FIELD=64		//ARE vars
};

struct Type;
struct ConstType;

struct Decl{
	string name;
	Type *type;			//type
	int kind,offset;
	ConstType *defType;	//default value
	Decl( const string &s,Type *t,int k,ConstType *d=0 ):name(s),type(t),kind(k),defType(d){}
	~Decl();

	virtual void getName( char *buff );

	virtual json toJSON();

#ifdef USE_LLVM
	int arg_index;
	llvm::Value *ptr;
#endif
};

struct DeclSeq{
	vector<Decl*> decls;
	DeclSeq();
	~DeclSeq();
	Decl *findDecl( const string &s );
	Decl *insertDecl( const string &s,Type *t,int kind,ConstType *d=0 );
	int size(){ return decls.size(); }

	json toJSON(){
		json tree=json::array();
		for( int i=0;i<decls.size();i++ ){
			tree[i]=decls[i]->toJSON();
		}
		return tree;
	}
};

#endif
