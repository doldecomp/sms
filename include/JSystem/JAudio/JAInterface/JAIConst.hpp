#ifndef JAICONST_HPP
#define JAICONST_HPP

#include <JSystem/JAudio/JAInterface/JAIActor.hpp>

class Vec;

namespace JAIConst {

extern void* nullInfoData2;
extern unsigned long random;
extern JAIActor* nullActor;
extern Vec* dummyZeroVec;
extern void* sCInfos_0;
extern float (*camMtx)[4];
extern Vec* camTrans;
extern Vec* camPreTrans;

} // namespace JAIConst

#endif // JAICONST_HPP
