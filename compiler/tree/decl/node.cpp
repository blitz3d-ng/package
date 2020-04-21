#include "node.h"

#ifdef USE_LLVM
void DeclNode::translate2( Codegen_LLVM *g ){
	cerr<<"translate2 missing implementation for "<<typeid(*this).name()<<endl;
	abort();
}
#endif
