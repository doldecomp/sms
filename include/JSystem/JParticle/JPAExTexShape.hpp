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
	Offsets* getIndTexMtx() { return &unk8; }
	s8 getExpScale() { return unk24; }
	GXIndTexMtxID getIndTexMtxID() { return unk20; }

public:
	/* 0x8 */ Offsets unk8;
	/* 0x20 */ GXIndTexMtxID unk20;
	/* 0x24 */ u8 unk24;
	/* 0x25 */ u8 unk25;
	/* 0x26 */ u8 unk26;
	/* 0x27 */ u8 unk27;
	/* 0x28 */ u8 unk28;
	/* 0x29 */ u8 unk29;
};

#endif
