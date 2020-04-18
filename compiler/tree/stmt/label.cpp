#include "label.h"

////////////////
// user label //
////////////////
void LabelNode::semant( Environ *e ){
	if( Label *l=e->findLabel( ident ) ){
		if( l->def>=0 ) ex( "duplicate label" );
		l->def=pos;l->data_sz=data_sz;
	}else e->insertLabel( ident,pos,-1,data_sz );
	ident=e->funcLabel+ident;
}

void LabelNode::translate( Codegen *g ){
	g->label( "_l"+ident );
}
