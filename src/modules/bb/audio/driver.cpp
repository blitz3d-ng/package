
#include "../audio/driver.h"

BBAudioDriver *gx_audio;

BBAudioDriver::~BBAudioDriver(){
  while( sound_set.size() ) freeSound( *sound_set.begin() );
}

BBSound *BBAudioDriver::verifySound( BBSound *s ){
	return sound_set.count( s )  ? s : 0;
}

void BBAudioDriver::freeSound( BBSound *s ){
	if( sound_set.erase( s ) ) delete s;
}
