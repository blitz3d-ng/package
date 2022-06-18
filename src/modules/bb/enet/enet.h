#ifndef BB_ENET_H
#define BB_ENET_H

#include <guid.h>
#include <bb/blitz/commands.h>

class BBPlayer{
public:
	bool local;
	Guid id;
	std::string name;

	BBPlayer();
	BBPlayer( std::string name );

	const std::string getId();
};

#include "commands.h"

#endif
