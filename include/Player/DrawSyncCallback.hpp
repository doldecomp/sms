#ifndef PLAYER_DRAW_SYNC_CALLBACK_HPP
#define PLAYER_DRAW_SYNC_CALLBACK_HPP

#include <dolphin/types.h>

// TODO: This might not have been in Player/
class TDrawSyncCallback {
public:
	virtual void drawSyncCallback(u16);
};

#endif
