#include "node.h"

#ifdef USE_LLVM
void DeclNode::translate2( Codegen_LLVM *g ){
	std::cerr<<"translate2 missing implementation for "<<typeid(*this).name()<<std::endl;
	abort();
}

void DeclNode::transdata2( Codegen_LLVM *g ){
	std::cerr<<"transdata2 missing implementation for "<<typeid(*this).name()<<std::endl;
	abort();
}
#endif
