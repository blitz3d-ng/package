
#ifndef EXPRNODE_H
#define EXPRNODE_H

#include "node.h"

struct ConstNode;	//is constant int,float or string

struct ExprNode : public Node{
	Type *sem_type;
	ExprNode():sem_type(0){}
	ExprNode( Type *t ):sem_type( t ){}

	ExprNode *castTo( Type *ty,Environ *e );
	ExprNode *semant( Environ *e,Type *ty );

	virtual ExprNode *semant( Environ *e )=0;
	virtual TNode *translate( Codegen *g )=0;
	virtual ConstNode *constNode(){ return 0; }

	DEFAULT_NODE_JSON( ExprNode );
};

struct ExprSeqNode : public Node{
	vector<ExprNode*> exprs;
	~ExprSeqNode(){ for( ;exprs.size();exprs.pop_back() ) delete exprs.back(); }
	void push_back( ExprNode *e ){ exprs.push_back( e ); }
	int  size(){ return exprs.size(); }
	void semant( Environ *e );
	TNode *translate( Codegen *g,bool userlib );
	void castTo( DeclSeq *ds,Environ *e,bool userlib );
	void castTo( Type *t,Environ *e );

	json toJSON( Environ *e ){
		json tree=json::array();
		for( int i=0;i<exprs.size();i++ ){
			tree.push_back( exprs[i]->toJSON( e ) );
		}
		return tree;
	}
};

#include "varnode.h"

struct CastNode : public ExprNode{
	ExprNode *expr;
	Type *type;
	CastNode( ExprNode *ex,Type *ty ):expr( ex ),type( ty ){}
	~CastNode(){ delete expr; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="CastNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["type"]=type->toJSON();
		tree["expr"]=expr->toJSON( e );
		return tree;
	}
};

struct CallNode : public ExprNode{
	string ident,tag;
	ExprSeqNode *exprs;
	Decl *sem_decl;
	CallNode( const string &i,const string &t,ExprSeqNode *e ):ident(i),tag(t),exprs(e){}
	~CallNode(){ delete exprs; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="CallNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["ident"]=ident;
		tree["tag"]=tag;
		tree["sem_decl"]=sem_decl->toJSON();
		tree["exprs"]=exprs->toJSON( e );
		return tree;
	}
};

struct VarExprNode : public ExprNode{
	VarNode *var;
	VarExprNode( VarNode *v ):var(v){}
	~VarExprNode(){ delete var; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="VarExprNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["var"]=var->toJSON( e );
		return tree;
	}
};

struct ConstNode : public ExprNode{
	ExprNode *semant( Environ *e ){ return this; }
	ConstNode *constNode(){ return this; }
	virtual int intValue()=0;
	virtual float floatValue()=0;
	virtual string stringValue()=0;

	DEFAULT_NODE_JSON( ConstNode );
};

struct IntConstNode : public ConstNode{
	int value;
	IntConstNode( int n );
	TNode *translate( Codegen *g );
	int intValue();
	float floatValue();
	string stringValue();

	json toJSON( Environ *e ){
		json tree;tree["@class"]="IntConstNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["value"]=value;
		return tree;
	}
};

struct FloatConstNode : public ConstNode{
	float value;
	FloatConstNode( float f );
	TNode *translate( Codegen *g );
	int intValue();
	float floatValue();
	string stringValue();

	json toJSON( Environ *e ){
		json tree;tree["@class"]="FloatConstNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["value"]=value;
		return tree;
	}
};

struct StringConstNode : public ConstNode{
	string value;
	StringConstNode( const string &s );
	TNode *translate( Codegen *g );
	int intValue();
	float floatValue();
	string stringValue();

	json toJSON( Environ *e ){
		json tree;tree["@class"]="StringConstNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["value"]=value;
		return tree;
	}
};

struct UniExprNode : public ExprNode{
	int op;ExprNode *expr;
	UniExprNode( int op,ExprNode *expr ):op( op ),expr( expr ){}
	~UniExprNode(){ delete expr; }
	ExprNode *constize();
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="UniExprNode";
		tree["sem_type"]=sem_type->toJSON();
		switch( op ){
		case '-':tree["op"]="SUB";break;
		case '+':tree["op"]="ADD";break;
		case ABS:tree["op"]="ABS";break;
		case SGN:tree["op"]="SGN";break;
		}
		tree["expr"]=expr->toJSON( e );
		return tree;
	}
};

// and, or, eor, lsl, lsr, asr
struct BinExprNode : public ExprNode{
	int op;ExprNode *lhs,*rhs;
	BinExprNode( int op,ExprNode *lhs,ExprNode *rhs ):op( op ),lhs( lhs ),rhs( rhs ){}
	~BinExprNode(){ delete lhs;delete rhs; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="BinExprNode";
		tree["sem_type"]=sem_type->toJSON();
		switch( op ){
		case AND:tree["op"]="AND";break;
		case OR: tree["op"]="OR";break;
		case XOR:tree["op"]="XOR";break;
		case SHL:tree["op"]="SHL";break;
		case SHR:tree["op"]="SHR";break;
		case SAR:tree["op"]="SAR";break;
		}
		tree["lhs"]=lhs->toJSON( e );
		tree["rhs"]=rhs->toJSON( e );
		return tree;
	}
};

// *,/,Mod,+,-
struct ArithExprNode : public ExprNode{
	int op;ExprNode *lhs,*rhs;
	ArithExprNode( int op,ExprNode *lhs,ExprNode *rhs ):op( op ),lhs( lhs ),rhs( rhs ){}
	~ArithExprNode(){ delete lhs;delete rhs; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="ArithExprNode";
		tree["sem_type"]=sem_type->toJSON();
		switch( op ){
		case '+':tree["op"]="ADD";break;
		case '-':tree["op"]="SUB";break;
		case '*':tree["op"]="MUL";break;
		case '/':tree["op"]="DIV";break;
		case MOD:tree["op"]="MOD";break;
		case '^':tree["op"]="SQR";break;
		}
		tree["lhs"]=lhs->toJSON( e );
		tree["rhs"]=rhs->toJSON( e );
		return tree;
	}
};

//<,=,>,<=,<>,>=
struct RelExprNode : public ExprNode{
	int op;ExprNode *lhs,*rhs;
	Type *opType;
	RelExprNode( int op,ExprNode *lhs,ExprNode *rhs ):op( op ),lhs( lhs ),rhs( rhs ){}
	~RelExprNode(){ delete lhs;delete rhs; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="RelExprNode";
		tree["sem_type"]=sem_type->toJSON();
		switch( op ){
		case '<': tree["op"]="LT";break;
		case '=': tree["op"]="EQ";break;
		case '>': tree["op"]="GT";break;
		case LE: tree["op"]="LE";break;
		case NE: tree["op"]="NE";break;
		case GE: tree["op"]="GE";break;
		}
		tree["lhs"]=lhs->toJSON( e );
		tree["rhs"]=rhs->toJSON( e );
		tree["opType"]=opType->toJSON();
		return tree;
	}
};

struct NewNode : public ExprNode{
	string ident;
	NewNode( const string &i ):ident(i){}
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="NewNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["ident"]=ident;
		return tree;
	}
};

struct FirstNode : public ExprNode{
	string ident;
	FirstNode( const string &i ):ident(i){}
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="FirstNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["ident"]=ident;
		return tree;
	}
};

struct LastNode : public ExprNode{
	string ident;
	LastNode( const string &i ):ident(i){}
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="LastNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["ident"]=ident;
		return tree;
	}
};

struct AfterNode : public ExprNode{
	ExprNode *expr;
	AfterNode( ExprNode *e ):expr(e){}
	~AfterNode(){ delete expr; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="AfterNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["expr"]=expr->toJSON( e );
		return tree;
	}
};

struct BeforeNode : public ExprNode{
	ExprNode *expr;
	BeforeNode( ExprNode *e ):expr(e){}
	~BeforeNode(){ delete expr; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="BeforeNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["expr"]=expr->toJSON( e );
		return tree;
	}
};

struct NullNode : public ExprNode{
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="NullNode";
		return tree;
	}
};

struct ObjectCastNode : public ExprNode{
	ExprNode *expr;
	string type_ident;
	ObjectCastNode( ExprNode *e,const string &t ):expr(e),type_ident(t){}
	~ObjectCastNode(){ delete expr; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="ObjectCastNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["expr"]=expr->toJSON( e );
		tree["type_ident"]=type_ident;
		return tree;
	}
};

struct ObjectHandleNode : public ExprNode{
	ExprNode *expr;
	ObjectHandleNode( ExprNode *e ):expr(e){}
	~ObjectHandleNode(){ delete expr; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="ObjectHandleNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["expr"]=expr->toJSON( e );
		return tree;
	}
};

#endif
