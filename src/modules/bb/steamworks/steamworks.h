#ifndef BB_STEAMWORKS_H
#define BB_STEAMWORKS_H

#include "../stdutil/stdutil.h"

typedef unsigned int uint32;

// some stuff missing from the headers...

struct SteamTVRegion_t{
	uint32 unMinX;
	uint32 unMinY;
	uint32 unMaxX;
	uint32 unMaxY;
};

enum ESteamTVRegionBehavior{
	k_ESteamVideoRegionBehaviorInvalid=-1,
	k_ESteamVideoRegionBehaviorHover=0,
	k_ESteamVideoRegionBehaviorClickPopup=1,
	k_ESteamVideoRegionBehaviorClickSurroundingRegion=2
};

#include <steam/steam_api_flat.h>

#include "bindings.h"

#endif
