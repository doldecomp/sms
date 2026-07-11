#ifndef ANIMAL_FISHOID_HPP
#define ANIMAL_FISHOID_HPP

#include <Enemy/Enemy.hpp>
#include <Strategic/TakeActor.hpp>

class MActor;

class TRealoid : public TSpineEnemy {
public:
	TRealoid(const char*);

	virtual void perform(u32, JDrama::TGraphics*);

	void clipBoids(JDrama::TGraphics*);
	void loadDefault(JSUMemoryInputStream&, const char*, int);

public:
	/* 0x150 */ s32 unk150;
};

class TRealoidActor : public TTakeActor {
public:
	TRealoidActor(MActor*);

	virtual ~TRealoidActor();
	virtual MtxPtr getTakingMtx();

public:
	/* 0x70 */ MActor* unk70;
	/* 0x74 */ s32 unk74;
};

#endif
