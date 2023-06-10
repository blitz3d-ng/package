
#include "stdafx.h"
#include "libs.h"
#include "editor.h"
#include "blitzide.h"

static std::map<std::string,std::string> keyhelps;
std::vector<Runtime> runtimes;

int linker_ver,runtime_ver;

static std::string execProc( const std::string &proc ){
	HANDLE rd,wr;

	SECURITY_ATTRIBUTES sa={sizeof(sa),0,true};

	if( CreatePipe( &rd,&wr,&sa,0 ) ){
		STARTUPINFO si={sizeof(si)};
		si.dwFlags=STARTF_USESTDHANDLES;
		si.hStdOutput=si.hStdError=wr;
		PROCESS_INFORMATION pi={0};
		if( CreateProcess( 0,(char*)proc.c_str(),0,0,true,DETACHED_PROCESS,0,0,&si,&pi ) ){
			CloseHandle( pi.hProcess );
			CloseHandle( pi.hThread );
			CloseHandle( wr );

			std::string t;
			char *buf=new char[1024];
			for(;;){
				unsigned long sz;
				int n=ReadFile( rd,buf,1024,&sz,0 );
				if( !n && GetLastError()==ERROR_BROKEN_PIPE ) break;
				if( !n ){ t="";break; }
				if( !sz ) break;
				t+=std::string( buf,sz );
			}
			delete[] buf;
			CloseHandle(rd);
			return t;
		}
		CloseHandle( rd );
		CloseHandle( wr );
	}
	AfxMessageBox( (proc+" failed").c_str() );
	ExitProcess(0);
	return "";
}

int version( std::string vers,std::string t ){
	t+=" version:";
	int n=vers.find( t );n+=t.size();
	int maj=atoi( vers.substr(n) );n=vers.find( '.',n )+1;
	int min=atoi( vers.substr(n) );
	return maj*100+min;
}

void initLibs(){

	std::string valid=execProc( prefs.homeDir+"/bin/blitzcc.exe -q" );
	if( valid.size() ){
		AfxMessageBox( ("Compiler environment error: "+valid).c_str() );
		ExitProcess(0);
	}

	std::string vers=tolower( execProc( prefs.homeDir+"/bin/blitzcc.exe -v" ) );
	linker_ver=version( vers,"linker" );
	runtime_ver=version( vers,"runtime" );

	//generate keywords!
	std::string kws=execProc( prefs.homeDir+"/bin/blitzcc.exe +k" );

	if( !kws.size() ){
		AfxMessageBox( "Error generating keywords" );
		ExitProcess(0);
	}

	int pos=0,n;
	while( (n=kws.find( '\n',pos ))!=std::string::npos ){
		std::string t=kws.substr( pos,n-pos-1 );
		for( int q=0;(q=t.find('\r',q))!=std::string::npos; ) t=t.replace( q,1,"" );

		std::string help=t;
		int i=t.find(' ');
		if( i!=std::string::npos ){
			t=t.substr(0,i);if( !t.size() ){
				AfxMessageBox( "Error in keywords" );
				ExitProcess(0);
			}
			if( !isalnum(t[t.size()-1]) ) t=t.substr(0,t.size()-1);
		}

		Editor::addKeyword(t);
		keyhelps[t]=help;
		pos=n+1;
	}

	std::string rts=execProc( prefs.homeDir+"/bin/blitzcc.exe -q -l" );
	pos=0;
	while( (n=rts.find( '\n',pos ))!=std::string::npos ){
		std::string t=rts.substr( pos,n-pos-1 );
		for( int q=0;(q=t.find('\r',q))!=std::string::npos; ) t=t.replace( q,1,"" );
		Runtime rt;
		rt.id=rt.name=t;
		runtimes.push_back(rt);
		pos=n+1;
	}
}

std::string quickHelp( const std::string &kw ){
	std::map<std::string,std::string>::const_iterator it=keyhelps.find(kw);
	return it==keyhelps.end() ? "" : it->second;
}

bool isMediaFile( const std::string &f ){

#ifndef PRO
	return false;
#endif

	static char *exts[]={
		"bmp","jpg","png","tga","iff","pcx",
		"wav","mid","mp3","mod","s3m","xm","it","rmi","sgt",
		"x","3ds",0
	};

	int i=f.rfind( '.' );
	if( i==std::string::npos || i+1==f.size() ) return false;
	std::string ext=f.substr( i+1 );
	char **p=exts;
	while( const char *e=*p++ ){
		std::string t(e);
		if( i+t.size()+1!=f.size() ) continue;
		if( ext==t ) return true;
	}
	return false;
}
