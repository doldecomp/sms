#ifndef MOVE_BG_MODEL_GATE_HPP
#define MOVE_BG_MODEL_GATE_HPP

#include <Strategic/TakeActor.hpp>
#include <dolphin/mtx.h>

class MActor;
class SampleCtrlModelData;

class TModelGate : public TTakeActor {
public:
	TModelGate(const char* name = "<TModelGate>")
	    : TTakeActor(name)
	{
	}

	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual MtxPtr getTakingMtx() { return nullptr; }

	void screenBlur(JDrama::TGraphics*);
	void startOpen();

	enum {
		STATE_UNK0 = 0x0,
		STATE_UNK1 = 0x1,
		STATE_UNK2 = 0x2,
	};

public:
	// Size 0x118
	/* 0x70 */ u8 unk70;
	/* 0x71 */ u8 unk71;  // Destination stage
	/* 0x72 */ u16 unk72; // center bone index
	/* 0x74 */ s16 unk74;
	/* 0x76 */ u16 unk76;
	/* 0x78 */ MActor* unk78; // Gate model actor
	/* 0x7C */ Mtx unk7C;
	/* 0xAC */ JGeometry::TVec3<f32> unkAC; // Mario hold offset
	/* 0xB8 */ u8 unkB8;
	/* 0xB9 */ u8 unkB9;
	/* 0xBA */ u8 unkBA;
	/* 0xBB */ u8 unkBB;
	/* 0xBC */ u16 unkBC;
	/* 0xBE */ u16 unkBE;
	/* 0xC0 */ SampleCtrlModelData* unkC0;
	/* 0xC4 */ u8 unkC4;
	/* 0xC5 */ u8 unkC5;
	/* 0xC6 */ u8 unkC6;
	/* 0xC7 */ u8 unkC7;
	/* 0xC8 */ s16 unkC8;
	/* 0xCA */ s16 unkCA;
	/* 0xCC */ s16 unkCC;
	/* 0xCE */ s16 unkCE;
	/* 0xD0 */ f32 unkD0;
	/* 0xD4 */ f32 unkD4;
	/* 0xD8 */ f32 unkD8;
	/* 0xDC */ f32 unkDC;
	/* 0xE0 */ u8 unkE0;
	/* 0xE1 */ u8 unkE1[3];
	/* 0xE4 */ f32 unkE4;
	/* 0xE8 */ f32 unkE8;
	/* 0xEC */ f32 unkEC;
	/* 0xF0 */ f32 unkF0;
	/* 0xF4 */ f32 unkF4;
	/* 0xF8 */ f32 unkF8;
	/* 0xFC */ f32 unkFC;
	/* 0x100 */ f32 unk100;
	/* 0x104 */ f32 unk104;
	/* 0x108 */ f32 unk108;
	/* 0x10C */ f32 unk10C;
	/* 0x110 */ f32 unk110;
	/* 0x114 */ f32 unk114;
};

#endif
