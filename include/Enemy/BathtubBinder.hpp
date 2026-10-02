#ifndef ENEMY_BATHTUBBINDER_HPP
#define ENEMY_BATHTUBBINDER_HPP

#include <JSystem/JGeometry/JGVec3.hpp>
#include <Strategic/Binder.hpp>
#include <dolphin/types.h>

class TBathtub;
class TBathWaterManager;

class TBathtubBinder : public TBinder {
public:
	TBathtubBinder();
	virtual ~TBathtubBinder();

	virtual void bind(TLiveActor*);

	void constrain_(JGeometry::TVec3<f32>&, f32);
	void float_(TLiveActor*);
	bool init(f32, f32, f32, f32, f32);

public:
	/* 0x4 */ TBathtub* unk4;
	/* 0x8 */ TBathWaterManager* unk8;
	/* 0xC */ f32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ f32 unk20;
};

#endif // ENEMY_BATHTUBBINDER_HPP
