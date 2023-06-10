#include "node.h"

#ifdef USE_LLVM
void StmtNode::translate2( Codegen_LLVM *g ){
	std::cerr<<"translate2 missing implementation for "<<typeid(*this).name()<<std::endl;
	abort();
}
#endif
