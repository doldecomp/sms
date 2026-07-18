#ifndef M3DUTIL_LOD_ANM_HPP
#define M3DUTIL_LOD_ANM_HPP

#include <dolphin/types.h>

class TLiveActor;

struct TLodAnmIndex {
	/* 0x0 */ int unk0[2];
	/* 0x8 */ int unk8[2];
};

// fabricated
struct TAnmBckMapping {
	int unk0;
	int unk4;
};

// fabricated
struct TAnmBtpMapping {
	int unk0;
	int unk4;
};

class TLodAnm {
public:
	TLodAnm(TLiveActor*, const TLodAnmIndex*, int, f32);
	void execChangeLod();
	bool setBckAndBtpAnm(int);
	bool setBtpAnm_(int);
	bool setBckAnm_(int);

	int getCurrentAnmKind() const { return mCurrentAnmKind; }

public:
	/* 0x0 */ TLiveActor* mOwner;
	/* 0x4 */ const TLodAnmIndex* mLodAnmIndexTable;
	/* 0x8 */ int unk8;
	/* 0xC */ f32 unkC;
	/* 0x10 */ int unk10;
	/* 0x14 */ int mCurrentAnmKind; // actually EnumNpcAnmKind
	/* 0x18 */ const TAnmBckMapping* unk18;
	/* 0x1C */ const TAnmBtpMapping* unk1C;
};

#endif
