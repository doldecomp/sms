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

public:
	/* 0x0 */ TMapObjBase* unk0;
	/* 0x4 */ int unk4;
	/* 0x8 */ JGeometry::TVec3<f32> unk8;
	/* 0x14 */ JGeometry::TVec3<f32> unk14;
};

#endif
