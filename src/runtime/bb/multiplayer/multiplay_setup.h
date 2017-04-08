
#ifndef MULTIPLAY_SETUP_H
#define MULTIPLAY_SETUP_H

#include <dplay.h>
#include <dplobby.h>
#include <string>

extern IDirectPlay4 *dirPlay;

void multiplay_setup_create();
void multiplay_setup_destroy();

int		multiplay_setup_open();
int		multiplay_setup_host( const std::string &game_name );
int		multiplay_setup_join( const std::string &game_name,const std::string &ip_add );
void	multiplay_setup_close();

#endif
