#ifndef JPA_EXTRA_SHAPE_HPP
#define JPA_EXTRA_SHAPE_HPP

#include <JSystem/JParticle/JPADataBlock.hpp>
#include <dolphin/gx/GXStruct.h>

class JPAExtraShape : public JPADataBlock {
public:
	JPAExtraShape(const u8*);
	~JPAExtraShape() { }

	BOOL isEnableScale() { return unk7E & 1; }
	BOOL isEnableAlpha() { return unk78 & 1; }
	BOOL isEnableRotate() { return unk7F; }
	f32 getRotateAngle() { return unk60; }
	f32 getRotateRandomAngle() { return unk6C; }
	f32 getRotateDirection() { return unk70; }
	f32 getRotateSpeed() { return unk64; }
	f32 getRotateRandomSpeed() { return unk68; }
	f32 getRandomScale() { return unk34; }
	f32 getAlphaWaveRandom() { return unk28; }
	f32 getScaleInTiming() { return unk38; }

	f32 getIncreaseRateX() { return unk50; }
	f32 getIncreaseRateY() { return unk58; }
	f32 getScaleInValueX() { return unk48; }
	f32 getScaleInValueY() { return unk40; }
	f32 getDecreaseRateX() { return unk54; }
	f32 getDecreaseRateY() { return unk5C; }
	f32 getScaleOutTiming() { return unk3C; }

	s16 getAnmCycleX() { return unk76; }
	s16 getAnmCycleY() { return unk74; }

	f32 getAlphaInTiming() { return unk8; }
	f32 getAlphaIncreaseRate() { return unk2C; }
	f32 getAlphaInValue() { return unk10; }
	f32 getAlphaOutTiming() { return unkC; }
	f32 getAlphaBaseValue() { return unk14; }
	f32 getAlphaDecreaseRate() { return unk30; }

	f32 getAlphaWaveParam1() { return unk1C; }
	f32 getAlphaWaveParam2() { return unk20; }
	f32 getAlphaWaveParam3() { return unk24; }

public:
	/* 0x8 */ f32 unk8;
	/* 0xC */ f32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ f32 unk20;
	/* 0x24 */ f32 unk24;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ f32 unk2C;
	/* 0x30 */ f32 unk30;
	/* 0x34 */ f32 unk34;
	/* 0x38 */ f32 unk38;
	/* 0x3C */ f32 unk3C;
	/* 0x40 */ f32 unk40;
	/* 0x44 */ f32 unk44;
	/* 0x48 */ f32 unk48;
	/* 0x4C */ f32 unk4C;
	/* 0x50 */ f32 unk50;
	/* 0x54 */ f32 unk54;
	/* 0x58 */ f32 unk58;
	/* 0x5C */ f32 unk5C;
	/* 0x60 */ f32 unk60;
	/* 0x64 */ f32 unk64;
	/* 0x68 */ f32 unk68;
	/* 0x6C */ f32 unk6C;
	/* 0x70 */ f32 unk70;
	/* 0x74 */ s16 unk74;
	/* 0x76 */ s16 unk76;
	/* 0x78 */ u8 unk78;
	/* 0x79 */ u8 unk79;
	/* 0x7A */ u8 unk7A;
	/* 0x7B */ u8 unk7B;
	/* 0x7C */ u8 unk7C;
	/* 0x7D */ u8 unk7D;
	/* 0x7E */ u8 unk7E;
	/* 0x7F */ u8 unk7F;
};

#endif
