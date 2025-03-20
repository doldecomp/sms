#include <JSystem/JAudio/JAInterface/JAIConst.hpp>

namespace JAIConst {

JAIActor nullActor;
Mtx camMtx;
Vec camTrans;
Vec camPreTrans;

Vec dummyZeroVec     = { 0 };
u8 nullInfoData2[32] = { 0 };
u8 sCInfos_0[]       = {
    4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4, 2,
    4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4, 2,
};

JMath::TRandom_fast_ random = 0;

} // namespace JAIConst
