#ifndef PLAYER_DRAW_SYNC_CALLBACK_HPP
#define PLAYER_DRAW_SYNC_CALLBACK_HPP

#include <dolphin/types.h>

class TDrawSyncCallback {
public:
	virtual void drawSyncCallback(u16);
};

#endif
