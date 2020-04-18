#include "decl_seq.h"

//////////////////////////////
// Sequence of declarations //
//////////////////////////////
void DeclSeqNode::proto( DeclSeq *d,Environ *e ){
	for( int k=0;k<decls.size();++k ){
		try{ decls[k]->proto( d,e ); }
		catch( Ex &x ){
			if( x.pos<0 ) x.pos=decls[k]->pos;
			if(!x.file.size() ) x.file=decls[k]->file;
			throw;
		}
	}
}

void DeclSeqNode::semant( Environ *e ){
	for( int k=0;k<decls.size();++k ){
		try{ decls[k]->semant( e ); }
		catch( Ex &x ){
			if( x.pos<0 ) x.pos=decls[k]->pos;
			if(!x.file.size() ) x.file=decls[k]->file;
			throw;
		}
	}
}

void DeclSeqNode::translate( Codegen *g ){
	for( int k=0;k<decls.size();++k ){
		try{ decls[k]->translate( g ); }
		catch( Ex &x ){
			if( x.pos<0 ) x.pos=decls[k]->pos;
			if(!x.file.size() ) x.file=decls[k]->file;
			throw;
		}
	}
}

void DeclSeqNode::transdata( Codegen *g ){
	for( int k=0;k<decls.size();++k ){
		try{ decls[k]->transdata( g ); }
		catch( Ex &x ){
			if( x.pos<0 ) x.pos=decls[k]->pos;
			if(!x.file.size() ) x.file=decls[k]->file;
			throw;
		}
	}
}

json DeclSeqNode::toJSON( Environ *e ){
	json tree=json::array();
	for( int i=0;i<decls.size();i++ ){
		tree.push_back( decls[i]->toJSON( e ) );
	}
	return tree;
}
