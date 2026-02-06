#ifndef MARIOCAP_HPP
#define MARIOCAP_HPP

#include <Player/MarioMain.hpp>

class TMarioCap {
public:
	TMarioCap(TMario*);

	virtual void perform(unsigned long, JDrama::TGraphics*);

	void createMirrorCap();
	void mtxEffectHide();
	void mtxEffectShow();

	// Unused
	void addDirty();

public:
	/* 0x4 */ u16 unk4; // Flag for which hat is active
	/* 0x6 */ u16 unk6;
	/* 0x8 */ u32 unk8;
	/* 0xC */ u32 unkC;
	/* 0x10 */ u32 unk10[0x28];
};

#endif
