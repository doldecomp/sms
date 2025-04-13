#ifndef MAP_POLLUTION_POS_HPP
#define MAP_POLLUTION_POS_HPP

#include <dolphin/types.h>

class TPollutionLayer;

class TPollutionPos {
public:
	TPollutionPos();
	void init(TPollutionLayer*, f32, f32, unsigned char*, int, int);
	void worldToTexSize(f32) const;
	void worldToDepth(f32) const;
	void isProhibit(int, int) const;
	void subtractFromYMap(int, int, f32) const;
	void isSame(int, int, f32) const;
	void getDepthWorld(int, int) const;
	void getEdgeDegree(int, int) const;

public:
	/* 0x0 */ u32 unk0;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ u32 unk8;
	/* 0xC */ u32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ u32 unk1C;
	/* 0x20 */ u32 unk20;
};

#endif
