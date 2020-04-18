#include "stmt_seq.h"
#include "include.h"

static string fileLabel;
static map<string,string> fileMap;

void StmtNode::debug( int pos,Codegen *g ){
	if( g->debug ){
		TNode *t=fileLabel.size() ? global( fileLabel ) : iconst(0);
		g->code( call( "__bbDebugStmt",iconst( pos ),t ) );
	}
}

void StmtSeqNode::reset( const string &file,const string &lab ){
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

	string t=fileLabel;
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

/////////////////
// An Include! //
/////////////////
void IncludeNode::semant( Environ *e ){

	label=genLabel();
	fileMap[file]=label;

	stmts->semant( e );
}

void IncludeNode::translate( Codegen *g ){

	if( g->debug ) g->s_data( file,label );

	stmts->translate( g );
}
