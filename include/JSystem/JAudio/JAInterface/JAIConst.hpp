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

	JAIActor(VecPtr a, VecPtr b, VecPtr c, u32 d)
	    : unk0(a)
	    , unk4(b)
	    , unk8(c)
	    , unkC(d)
	{
	}

	/* 0x0 */ VecPtr unk0;
	/* 0x4 */ VecPtr unk4;
	/* 0x8 */ VecPtr unk8;
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
