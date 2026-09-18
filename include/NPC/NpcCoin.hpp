#ifndef NPC_COIN_HPP
#define NPC_COIN_HPP

#include <JSystem/JGeometry/JGVec3.hpp>

class TMapObjBase;

class TNpcCoin {
public:
	TNpcCoin(int);
	void execAppearCoin_();
	void requestAppearCoin(const Vec&, f32, int);
	void updateCoin();

	// Fabricated. The accessor level is load-bearing: routing
	// execAppearCoin_'s three pending-coin uses through it is +8 bytes of
	// frame in updateCoin, which together with SMSGetMarDirector() over
	// gpMarDirector closes updateCoin's 16-byte gap.
	TMapObjBase* getCoin() { return unk0; }

public:
	/* 0x0 */ TMapObjBase* unk0;
	/* 0x4 */ int unk4;
	/* 0x8 */ JGeometry::TVec3<f32> unk8;
	/* 0x14 */ JGeometry::TVec3<f32> unk14;
};

#endif
