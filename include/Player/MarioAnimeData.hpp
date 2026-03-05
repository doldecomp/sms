#ifndef PLAYER_MARIO_ANIME_DATA_HPP
#define PLAYER_MARIO_ANIME_DATA_HPP

#include <types.h>

struct TMarioAnimeData {
public:
	bool isPumpOK() const;

public:
	/* 0x0 */ u16 unk0;
	/* 0x2 */ u16 unk2;
	/* 0x4 */ u8 unk4;
	/* 0x5 */ u8 unk5; // Hand id
	/* 0x6 */ u8 unk6;
	/* 0x7 */ u8 unk7;
};

extern TMarioAnimeData gMarioAnimeData[336];

struct unkTMarioAnimeFilesStruct {
public:
	u32 unk0;
	const char* unk4;
};

#endif
