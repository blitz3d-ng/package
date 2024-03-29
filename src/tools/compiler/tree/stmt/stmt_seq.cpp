#include "stmt_seq.h"
#include "include.h"

static std::string fileLabel;
static std::map<std::string,std::string> fileMap;

void StmtNode::debug( int pos,Codegen *g ){
	if( g->debug ){
		TNode *t=fileLabel.size() ? global( fileLabel ) : iconst(0);
		g->code( call( "__bbDebugStmt",iconst( pos ),t ) );
	}
}

#ifdef USE_LLVM
static std::map<std::string,llvm::Value*> fileMap2;

void StmtNode::debug2( int pos,Codegen_LLVM *g ){
	if( g->debug ){
		llvm::DIScope *scope=g->dbgBlocks.back();
		int row=(pos>>16)&0xffff;
		int col=pos&0xffff;
		g->builder->SetCurrentDebugLocation( llvm::DILocation::get( scope->getContext(),row,col,scope ) );

		auto t=fileMap2[fileLabel];
		if( !t ) t=fileMap2[fileLabel]=g->builder->CreateGlobalStringPtr( fileLabel );
		g->CallIntrinsic( "_bbDebugStmt",g->voidTy,2,g->constantInt( pos ),t );
	}
}
#endif

void StmtSeqNode::reset( const std::string &file,const std::string &lab ){
	fileLabel="";
	fileMap.clear();

	fileMap[file]=lab;
}

////////////////////////
// Statement Sequence //
////////////////////////
void StmtSeqNode::semant( Environ *e ){
	for( int k=0;k<stmts.size();++k ){
		try{ stmts[k]->semant( e ); }
		catch( Ex &x ){
			if( x.pos<0 ) x.pos=stmts[k]->pos;
			if( !x.file.size() ) x.file=file;
			throw;
		}
	}
}

void StmtSeqNode::translate( Codegen *g ){

	std::string t=fileLabel;
	fileLabel=file.size() ? fileMap[file] : "";
	for( int k=0;k<stmts.size();++k ){
		StmtNode *stmt=stmts[k];
		stmt->debug( stmts[k]->pos,g );
		try{
			stmt->translate( g );
		}
		catch( Ex &x ){
			if( x.pos<0 ) x.pos=stmts[k]->pos;
			if( !x.file.size() ) x.file=file;
			throw;
		}
	}
	fileLabel=t;
}

#ifdef USE_LLVM
void StmtSeqNode::translate2( Codegen_LLVM *g ) {
	std::string t=fileLabel;
	fileLabel=file.size() ? file : "";
	for( int k=0;k<stmts.size();++k ){
		StmtNode *stmt=stmts[k];
		stmt->debug2( stmts[k]->pos,g );
		try{
			stmt->translate2( g );
		}
		catch( Ex &x ){
			if( x.pos<0 ) x.pos=stmts[k]->pos;
			if( !x.file.size() ) x.file=file;
			throw;
		}
	}
	fileLabel=t;
}
#endif

json StmtSeqNode::toJSON( Environ *e ){
	json tree;tree["@class"]="StmtSeqNode";
	tree["file"]=file;
	tree["stmts"]=json::array();
	for( int k=0;k<stmts.size();++k ){
		tree["stmts"].push_back( stmts[k]->toJSON( e ) );
	}
	return tree;
}

/////////////////
// An Include! //
/////////////////
void IncludeNode::semant( Environ *e ){

	label=genLabel();
	fileMap[file]=label;

	stmts->semant( e );
}
