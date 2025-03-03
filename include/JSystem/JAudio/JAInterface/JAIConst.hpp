#ifndef JAICONST_HPP
#define JAICONST_HPP

#include <dolphin/mtx.h>

class JAIActor;

namespace JAIConst {

extern void* nullInfoData2;
extern u32 random;
extern JAIActor* nullActor;
extern VecPtr dummyZeroVec;
extern void* sCInfos_0;
extern Mtx camMtx;
extern Vec camTrans;
extern Vec camPreTrans;

} // namespace JAIConst

#endif // JAICONST_HPP
