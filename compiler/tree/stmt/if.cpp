#include "if.h"
#include "../expr/const.h"

//////////////////
// If statement //
//////////////////
void IfNode::semant( Environ *e ){
	expr=expr->semant( e );
	expr=expr->castTo( Type::int_type,e );
	stmts->semant( e );
	if( elseOpt ) elseOpt->semant( e );
}

void IfNode::translate( Codegen *g ){
	if( ConstNode *c=expr->constNode() ){
		if( c->intValue() ) stmts->translate( g );
		else if( elseOpt ) elseOpt->translate( g );
	}else{
		string _else=genLabel();
		g->code( jumpf( expr->translate( g ),_else ) );
		stmts->translate( g );
		if( elseOpt ){
			string _else2=genLabel();
			g->code( jump( _else2 ) );
			g->label( _else );
			elseOpt->translate( g );
			_else=_else2;
		}
		g->label( _else );
	}
}

json IfNode::toJSON( Environ *e ){
	json tree;tree["@class"]="IfNode";
	tree["pos"]=pos;
	tree["expr"]=expr->toJSON( e );
	tree["stmts"]=stmts->toJSON( e );
	if( elseOpt ) tree["elseOpt"]=elseOpt->toJSON( e );
	return tree;
}
