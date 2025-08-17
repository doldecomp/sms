#ifndef MAP_OBJ_MESSENGER_HPP
#define MAP_OBJ_MESSENGER_HPP

#include <Strategic/HitActor.hpp>

class TMapObjMessenger : public THitActor {
public:
	TMapObjMessenger(const char*);

	virtual BOOL receiveMessage(THitActor* sender, u32 message);

public:
	/* 0x68 */ u32 unk68;
};

#endif
