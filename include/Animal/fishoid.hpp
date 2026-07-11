#ifndef ANIMAL_FISHOID_HPP
#define ANIMAL_FISHOID_HPP

#include <Enemy/Enemy.hpp>
#include <Strategic/TakeActor.hpp>
#include <dolphin/mtx.h>

class MActor;
class TBoid;

class TRealoid : public TSpineEnemy {
public:
	TRealoid(const char*);

	virtual ~TRealoid();
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
	virtual void perform(u32, JDrama::TGraphics*);

	void checkHitActors();
	void calcRootMatrix(TBoid*);

public:
	/* 0x70 */ MActor* unk70;
	/* 0x74 */ u32 unk74;
	/* 0x78 */ Mtx unk78;
};

#endif
