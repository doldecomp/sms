#ifndef M3DUTIL_LOD_ANM_HPP
#define M3DUTIL_LOD_ANM_HPP

#include <dolphin/types.h>

class TLiveActor;

struct TLodAnmIndex {
	/* 0x0 */ int mBckIndex[2];
	/* 0x8 */ int mBtpIndex[2];
};

// fabricated
struct TAnmBckMapping {
	int mFrom;
	int mTo;
};

// fabricated
struct TAnmBtpMapping {
	int mFrom;
	int mTo;
};

class TLodAnm {
public:
	TLodAnm(TLiveActor*, const TLodAnmIndex*, int, f32);
	void execChangeLod();
	bool setBckAndBtpAnm(int);
	bool setBtpAnm_(int);
	bool setBckAnm_(int);

	// fabricated
	int getCurrentAnmKind() const { return mCurrentAnmKind; }
	int getCurrentLod() const { return mCurrentLod; }
	void setIndividualBck(const TAnmBckMapping* mapping)
	{
		mIndividualBck = mapping;
	}
	void setIndividualBtp(const TAnmBtpMapping* mapping)
	{
		mIndividualBtp = mapping;
	}

private:
	/* 0x0 */ TLiveActor* mOwner;
	/* 0x4 */ const TLodAnmIndex* mLodAnmIndexTable;
	/* 0x8 */ int mCurrentLod;
	/* 0xC */ f32 mLodChangeDist;
	/* 0x10 */ int mAnmKindNum;
	/* 0x14 */ int mCurrentAnmKind; // actually EnumNpcAnmKind
	/* 0x18 */ const TAnmBckMapping* mIndividualBck;
	/* 0x1C */ const TAnmBtpMapping* mIndividualBtp;
};

#endif
