#ifndef JAICONST_HPP
#define JAICONST_HPP

#include <dolphin/mtx.h>
#include <JSystem/JMath.hpp>

class JAIActor {
public:
	JAIActor()
	    : unk0(0)
	    , unk4(0)
	    , unk8(0)
	    , unkC(0)
	{
	}

	/* 0x0 */ u32 unk0;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ u32 unk8;
	/* 0xC */ u32 unkC;
};

namespace JAIConst {

extern JAIActor nullActor;
extern Mtx camMtx;
extern Vec camTrans;
extern Vec camPreTrans;

extern Vec dummyZeroVec;
extern u8 nullInfoData2[];
extern u8 sCInfos_0[];

extern JMath::TRandom_fast_ random;

} // namespace JAIConst

#endif // JAICONST_HPP
