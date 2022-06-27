#include <string>
#include <cstdlib>
#include <libgen.h>
using namespace std;

int main( int argc,char **argv ){
	printf( "%s\n",argv[0] );

	string blitzpath;
	blitzpath=string( dirname( argv[0] ) )+"/../../../";

	setenv( "blitzpath",blitzpath.c_str(),1 );
	system( (blitzpath+"/bin/ide2").c_str() );
	return 0;
}
