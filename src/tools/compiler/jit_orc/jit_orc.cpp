#include "jit_orc.h"

#include <llvm/ExecutionEngine/Orc/LLJIT.h>

int JIT_ORC::run( Runtime *runtime,const std::string &obj, const std::string &home, const std::string &rt,std::string &args ) {
	auto J=llvm::cantFail( llvm::orc::LLJITBuilder().create() );

	std::map<const char*,void*> syms;
	runtime->loadSyms( syms );

	llvm::orc::SymbolMap symmap;
	for( auto sym:syms ){
		std::string ident=sym.first;
		if( !isalnum(ident[0]) && ident[0]!='_' ){
			ident=ident.substr(1);
		}
		size_t e=1;
		while( isalnum(ident[e]) || ident[e]=='_' ) e++;
		ident=ident.substr( 0,e );
		if( ident[0]!='_' ) ident=runtime->symName( sym.first );

		symmap.insert(
			std::pair< llvm::orc::SymbolStringPtr,llvm::orc::ExecutorSymbolDef >(
				J->mangleAndIntern( ident ),
				llvm::orc::ExecutorSymbolDef( llvm::orc::ExecutorAddr( (uint64_t)sym.second ),llvm::JITSymbolFlags::Absolute|llvm::JITSymbolFlags::Exported )
			)
		);
	}

	auto RT=J->getMainJITDylib().createResourceTracker();
	llvm::cantFail( J->getMainJITDylib().define( llvm::orc::absoluteSymbols( symmap ),RT ) );

	auto buff=llvm::MemoryBuffer::getMemBuffer( obj );
	llvm::cantFail( J->addObjectFile( std::move(buff) ) );

	auto main_sym=cantFail( J->lookup( "bbMain" ) );
	auto start_sym=cantFail( J->lookup( "_bbStart" ) );
	BBMAIN bbMain=(BBMAIN)main_sym.getValue();
	BBSTART bbStart=(BBSTART)start_sym.getValue();

	const char *argv[2]={ "blitzcc",args.c_str() };
	int retcode=bbStart( 2, (char**)argv, bbMain );

	cantFail( RT->remove() );

	return retcode;
}
