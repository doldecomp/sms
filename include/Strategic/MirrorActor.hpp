#ifndef STRATEGIC_MIRROR_ACTOR_HPP
#define STRATEGIC_MIRROR_ACTOR_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class J3DModel;

class TMirrorActor : public JDrama::TViewObj {
public:
	u8 isInMirror() const;
	void checkIsInMirror();
	void perform(u32 cue, JDrama::TGraphics* graphics);
	static void entryMirrorDrawBufferAlways(J3DModel*);
	void init(J3DModel*, u16);
	TMirrorActor(const char*);

	// fabricated.  perform()'s matrix-copy loops read the source model through
	// getUnk10() and the destination model raw: with both raw the loops get a
	// one-step register rotation and an 8-byte-short frame, with both through
	// accessors the frame is 8 over.  init() wants getUnk14() for its
	// entryMirrorDrawBufferAlways argument (+8, frame exact).
	J3DModel* getUnk10() { return unk10; }
	J3DModel* getUnk14() { return unk14; }

public:
	/* 0x10 */ J3DModel* unk10;
	/* 0x14 */ J3DModel* unk14;
	/* 0x18 */ u8 unk18;
	/* 0x1A */ u16 unk1A;
};

#endif
