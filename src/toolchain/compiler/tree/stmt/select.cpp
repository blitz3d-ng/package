#include "select.h"
#include "../var/decl_var.h"

////////////////////////
// A select statement //
////////////////////////
json CaseNode::toJSON( Environ *e ){
	json tree;tree["@class"]="CaseNode";
	tree["exprs"]=exprs->toJSON( e );
	tree["stmts"]=stmts->toJSON( e );
	return tree;
}

void SelectNode::semant( Environ *e ){
	expr=expr->semant( e );
	Type *ty=expr->sem_type;
	if( ty->structType() ) ex( "Select cannot be used with objects" );

	//we need a temp var
	Decl *d=e->decls->insertDecl( genLabel(),expr->sem_type,DECL_LOCAL );
	sem_temp=d_new DeclVarNode( d );

	for( int k=0;k<cases.size();++k ){
		CaseNode *c=cases[k];
		c->exprs->semant( e );
		c->exprs->castTo( ty,e );
		c->stmts->semant( e );
	}
	if( defStmts ) defStmts->semant( e );
}

void SelectNode::translate( Codegen *g ){

	Type *ty=expr->sem_type;

	g->code( sem_temp->store( g,expr->translate( g ) ) );

	vector<string> labs;
	string brk=genLabel();

	for( int k=0;k<cases.size();++k ){
		CaseNode *c=cases[k];
		labs.push_back( genLabel() );
		for( int j=0;j<c->exprs->size();++j ){
			ExprNode *e=c->exprs->exprs[j];
			TNode *t=compare( '=',sem_temp->load( g ),e->translate( g ),ty );
			g->code( jumpt( t,labs.back() ) );
		}
	}
	if( defStmts ) defStmts->translate( g );
	g->code( jump( brk ) );
	for( int k=0;k<cases.size();++k ){
		CaseNode *c=cases[k];
		g->label( labs[k] );
		c->stmts->translate( g );
		g->code( jump( brk ) );
	}

	g->label( brk );
}

#ifdef USE_LLVM
void SelectNode::translate2( Codegen_LLVM *g ){
	Type *ty=expr->sem_type;

	auto *func = g->builder->GetInsertBlock()->getParent();

	vector<llvm::BasicBlock*> blocks;
	vector<int> block_idx;
	llvm::BasicBlock *cont=0,*def=0;
	cont=llvm::BasicBlock::Create( *g->context,"case_cont" );

	for( int k=0;k<cases.size();++k ){
		CaseNode *c=cases[k];
		auto b=llvm::BasicBlock::Create( *g->context,"case_cond" );
		blocks.push_back( b );
	}

	if( defStmts ) {
		def=llvm::BasicBlock::Create( *g->context,"case_default" );
		blocks.push_back( def );
	} else {
		blocks.push_back( cont );
	}

	sem_temp->store2( g,expr->translate2( g ) );
	g->builder->CreateBr( blocks[0] );

	for( int k=0;k<cases.size();++k ){
		CaseNode *c=cases[k];

		vector<llvm::BasicBlock*> conds;
		conds.push_back( blocks[k] );
		for( int j=1;j<c->exprs->size();++j ){
			auto b=llvm::BasicBlock::Create( *g->context,"case_cond" );
			conds.push_back( b );
		}
		conds.push_back( blocks[k+1] );

		auto body=llvm::BasicBlock::Create( *g->context,"case_body" );

		for( int j=0;j<c->exprs->size();++j ){
			ExprNode *e=c->exprs->exprs[j];

			func->getBasicBlockList().push_back( conds[j] );
			g->builder->SetInsertPoint( conds[j] );
			auto *t=compare2( '=',sem_temp->load2( g ),e->translate2( g ),ty,g );
			g->builder->CreateCondBr( t,body,conds[j+1] );
		}

		func->getBasicBlockList().push_back( body );
		g->builder->SetInsertPoint( body );
		c->stmts->translate2( g );
		g->builder->CreateBr( cont );
	}

	if( def ) {
		func->getBasicBlockList().push_back( def );
		g->builder->SetInsertPoint( def );
		defStmts->translate2( g );
		g->builder->CreateBr( cont );
	}

	func->getBasicBlockList().push_back( cont );
	g->builder->SetInsertPoint( cont );
}
#endif


json SelectNode::toJSON( Environ *e ){
	json tree;tree["@class"]="SelectNode";
	tree["pos"]=pos;
	tree["expr"]=expr->toJSON( e );
	tree["cases"]=json::array();
	for( int i=0;i<cases.size();i++ ){
		tree["cases"].push_back( cases[i]->toJSON( e ) );
	}
	if( defStmts ) tree["defStmts"]=defStmts->toJSON( e );
	tree["sem_temp"]=sem_temp->toJSON( e );
	return tree;
}
