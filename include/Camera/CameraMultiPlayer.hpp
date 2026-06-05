#ifndef CAMERA_CAMERA_MULTI_PLAYER_HPP
#define CAMERA_CAMERA_MULTI_PLAYER_HPP

#include <types.h>
#include <JSystem/JGeometry/JGVec3.hpp>

class TMultiPlayerData {
public:
	TMultiPlayerData()
	    : unk0(nullptr)
	    , unk4(0.0f)
	    , unk8(0.0f)
	{
	}

public:
	/* 0x0 */ const JGeometry::TVec3<f32>* unk0;
	/* 0x4 */ f32 unk4;
	/* 0x8 */ f32 unk8;
};

class TCameraMultiPlayer {
public:
	TCameraMultiPlayer(u8);
	bool addPlayer(const JGeometry::TVec3<f32>*, f32, f32);
	bool removePlayer(const JGeometry::TVec3<f32>*);

public:
	/* 0x0 */ u8 unk0;
	/* 0x1 */ u8 unk1;
	/* 0x4 */ TMultiPlayerData* unk4;
};

#endif
