#include <JSystem/JAudio/JAInterface/JAIConst.hpp>

namespace JAIConst {

JAIActor nullActor;
Mtx camMtx;
Vec camTrans;
Vec camPreTrans;

Vec dummyZeroVec            = { 0 };
u8 nullInfoData2[32]        = { 0 };
JAICategoryInfo sCInfos_0[] = {
	{ 4, 2 }, { 4, 2 }, { 4, 2 }, { 4, 2 }, { 4, 2 }, { 4, 2 },
	{ 4, 2 }, { 4, 2 }, { 4, 2 }, { 4, 2 }, { 4, 2 }, { 4, 2 },
	{ 4, 2 }, { 4, 2 }, { 4, 2 }, { 4, 2 },
};

JMath::TRandomFast random = 0;

} // namespace JAIConst
