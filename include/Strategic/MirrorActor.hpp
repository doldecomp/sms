#ifndef STRATEGIC_MIRROR_ACTOR_HPP
#define STRATEGIC_MIRROR_ACTOR_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class J3DModel;

class TMirrorActor : public JDrama::TViewObj {
public:
	enum {
		MIRROR_ACTOR_FLAG_HIDDEN    = 0x1,
		MIRROR_ACTOR_FLAG_ALWAYS    = 0x2,
		MIRROR_ACTOR_FLAG_MARIO     = 0x4,
		MIRROR_ACTOR_FLAG_SDL_MODEL = 0x8,
		MIRROR_ACTOR_FLAG_UNK10     = 0x10,
	};

	bool isInMirror() const;
	void checkIsInMirror();
	void perform(u32 cue, JDrama::TGraphics* graphics);
	static void entryMirrorDrawBufferAlways(J3DModel*);
	void init(J3DModel* model, u16 flags);
	TMirrorActor(const char*);

	// fabricated
	void onFlag(u16 flag) { mFlags |= flag; }
	void offFlag(u16 flag) { mFlags &= ~flag; }

public:
	/* 0x10 */ J3DModel* mNormalModel;
	/* 0x14 */ J3DModel* mMirrorModel;
	/* 0x18 */ bool mIsInMirror;
	/* 0x1A */ u16 mFlags;
};

#endif
