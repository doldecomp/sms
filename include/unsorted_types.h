#ifndef UNSORTED_TYPES_H
#define UNSORTED_TYPES_H

#include <types.h>

// TODO
class TDirector {
	char filler0;
};

// TODO
class TGameSequence {
public:
	u8 _000;
	u8 _001;
	u16 _002;
};

// TODO
class TDisplay {
	char filler0;
};

// TODO
class TMarioGamePad {
	char filler0[4];
};

// TODO
class TSMSFader {
	char filler0[4];
};

// TODO
class JKRExpHeap {
	char filler0[4];
};

// TODO: src/System/ProcessMeter.hpp
class TProcessMeter {
	TProcessMeter(s32);
	s32 _000;
	s32 _004;
	s32 _008;
	s32 _00C;
	s32 _010;
	f32 _014;
	f32 _018;
	s16 _01C;
	s16 _01E;
};

class TApplication {
public:
	/* 0x00 */ TApplication* self;
	/* 0x04 */ TDirector* director;
	/* 0x08 */ char undefined1;
	/* 0x0A */ TGameSequence prevArea;
	/* 0x0E */ TGameSequence currArea;
	/* 0x12 */ TGameSequence nextArea;
	/* 0x18 */ int movie;
	/* 0x1C */ TDisplay* display;
	/* 0x20 */ TMarioGamePad* gamePads[4];
	/* 0x30 */ char undefined4[4];
	/* 0x34 */ TSMSFader* unkFader;
	/* 0x38 */ char saveFile;
	/* 0x3C */ char undefined4_2[4];
	/* 0x40 */ JKRExpHeap* heap;
	/* 0x44 */ char undefined2[2];
	/* 0x46 */ char undefined2_2[2];
	/* 0x48 */ TProcessMeter* meter;

	// functions
	TApplication();
	void initialize();
	void proc();
	void finalize();
};

extern TApplication gpApplication;

struct UnkStruct3B0378 {
	unsigned int unk0;
	char* str;
};

class RumbleType {
	float foo;
	static int getIndex(char* strIn);
};

struct gpMarDirectorStruct {
	char filler0[0x7C];
	u8 unk7C;
};

struct gpCameraStruct {
	char filler0[0x50];
	s32 unk50;
};

struct gpCubeFastCStruct {
	char filler0[0x1C];
	u32 unk1C;
};

struct UnkStructUnk58 {
	char filler0[0x14];
	float unk14;
};

class TLiveActor {
public:
	struct J3DModel* getModel() const;
};

struct UnkStruct44 {
	char filler0[0x4C];
	int unk4C;
};

#endif // UNSORTED_TYPES_H
