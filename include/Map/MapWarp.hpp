#ifndef MAP_MAP_WARP_HPP
#define MAP_MAP_WARP_HPP

#include <JSystem/JGeometry.hpp>

class JSUMemoryInputStream;

class TMapWarp {
public:
	TMapWarp();

	class TMapWarpInfo {
	public:
		TMapWarpInfo() { }

		JGeometry::TVec3<f32>& getUnk8() { return unk8; }

	public:
		/* 0x0 */ u32 unk0;
		/* 0x4 */ u32 unk4;
		/* 0x8 */ JGeometry::TVec3<f32> unk8;
	};

	void init(JSUMemoryInputStream&);
	void initModel();
	void watchToWarp();
	void warp(int);
	void changeModel(int);

public:
	/* 0x0 */ int unk0;
	/* 0x4 */ TMapWarpInfo* unk4;
	/* 0x8 */ int unk8;
	/* 0xC */ f32 unkC;
};

#endif
