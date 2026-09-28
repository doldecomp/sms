#ifndef JAICONST_HPP
#define JAICONST_HPP

#include <dolphin/mtx.h>
#include <JSystem/JAudio/JAInterface/JAIData.hpp>
#include <JSystem/JMath.hpp>

class JAIActor {
public:
	JAIActor()
	    : mIdentity(0)
	    , mTranslation(0)
	    , unk8(0)
	    , mGroundNumber(0)
	{
	}

	JAIActor(const void* a, const Vec* b, const Vec* c, u32 d)
	    : mIdentity(a)
	    , mTranslation(b)
	    , unk8(c)
	    , mGroundNumber(d)
	{
	}

	/* 0x0 */ const void* mIdentity;
	/* 0x4 */ const Vec* mTranslation;
	/* 0x8 */ const Vec* unk8;
	/* 0xC */ u32 mGroundNumber;
};

namespace JAIConst {

extern JAIActor nullActor;
extern Mtx camMtx;
extern Vec camTrans;
extern Vec camPreTrans;

extern Vec dummyZeroVec;
extern u8 nullInfoData2[];
extern JAICategoryInfo sCInfos_0[];

extern JMath::TRandomFast random;

} // namespace JAIConst

#endif // JAICONST_HPP
