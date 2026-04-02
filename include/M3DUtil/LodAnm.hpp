#ifndef M3DUTIL_LOD_ANM_HPP
#define M3DUTIL_LOD_ANM_HPP

#include <dolphin/types.h>

class TLiveActor;

struct TLodAnmIndex {
	/* 0x0 */ int unk0[2];
	/* 0x8 */ int unk8[2];
};

class TLodAnm {
public:
	TLodAnm(TLiveActor*, const TLodAnmIndex*, int, f32);
	void execChangeLod();
	bool setBckAndBtpAnm(int);
	bool setBtpAnm_(int);
	bool setBckAnm_(int);

public:
	/* 0x0 */ TLiveActor* unk0;
	/* 0x4 */ const TLodAnmIndex* unk4;
	/* 0x8 */ int unk8;
	/* 0xC */ f32 unkC;
	/* 0x10 */ int unk10;
	/* 0x14 */ u32 unk14;

	// fabricated
	struct BckMapping {
		int unk0;
		int unk4;
	};

	/* 0x18 */ BckMapping* unk18;

	// fabricated
	struct BtpMapping {
		int unk0;
		int unk4;
	};
	/* 0x1C */ BtpMapping* unk1C;
};

#endif
