#ifndef JPA_EX_TEX_SHAPE_HPP
#define JPA_EX_TEX_SHAPE_HPP

#include <dolphin/gx/GXEnum.h>
#include <JSystem/JParticle/JPADataBlock.hpp>

class JPAExTexShape : public JPADataBlock {
	typedef f32 Offsets[2][3];

public:
	JPAExTexShape(const u8*);
	~JPAExTexShape() { }

	// from tww
	Offsets* getIndTexMtx() { return &mIndTexMtx; }
	GXIndTexMtxID getIndTexMtxID() { return mIndTexMtxID; }
	s8 getExpScale() { return mExpScale; }
	u32 getIndTexMode() { return mIndTexMode; }
	u8 getIndTextureID() { return mIndTextureID; }
	u8 getSubTextureID() { return mSubTextureID; }
	BOOL isEnableSecondTex() { return unk28 & 1; }
	u8 getSecondTexIndex() { return mSecondTexIndex; }

public:
	/* 0x8 */ Offsets mIndTexMtx;
	/* 0x20 */ GXIndTexMtxID mIndTexMtxID;
	/* 0x24 */ u8 mExpScale;
	/* 0x25 */ u8 mIndTexMode;
	/* 0x26 */ u8 mIndTextureID;
	/* 0x27 */ u8 mSubTextureID;
	/* 0x28 */ u8 unk28;
	/* 0x29 */ u8 mSecondTexIndex;
};

#endif
