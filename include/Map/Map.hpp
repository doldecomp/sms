#ifndef MAP_HPP
#define MAP_HPP

class THitActor;

struct TBGCheckData {
	char filler0[0x44];
	THitActor* unk44;
};

struct TMap {
	float checkGround(float, float, float, const TBGCheckData**) const;
};

extern TMap* gpMap;

#endif
