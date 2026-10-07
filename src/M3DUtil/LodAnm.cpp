#include <M3DUtil/LodAnm.hpp>
#include <M3DUtil/MActor.hpp>
#include <Strategic/LiveActor.hpp>

TLodAnm::TLodAnm(TLiveActor* param_1, const TLodAnmIndex* param_2, int param_3,
                 f32 param_4)
{
	mOwner            = param_1;
	mLodAnmIndexTable = param_2;
	mCurrentLod       = param_3;
	mLodChangeDist    = param_4;
	mAnmKindNum       = 0;
	mCurrentAnmKind   = -1;
	mIndividualBck    = nullptr;
	mIndividualBtp    = 0;
	if (param_2) {
		while (param_2[mAnmKindNum].mBckIndex[0] >= -1)
			++mAnmKindNum;
	}
}

bool TLodAnm::setBckAnm_(int param_1)
{
	if (param_1 < 0) {
		mOwner->getMActor()->setBckFromIndex(-1);
		return true;
	}

	bool result = false;

	int iVar3 = mOwner->getMActor()->getCurAnmIdx(ANM_TYPE_BCK);

	int tmp;
	if (mLodAnmIndexTable == nullptr)
		tmp = param_1;
	else
		tmp = mLodAnmIndexTable[param_1].mBckIndex[mCurrentLod];

	if (mIndividualBck != nullptr)
		for (const TAnmBckMapping* it = mIndividualBck; it->mFrom >= 0; ++it)
			if (tmp == it->mFrom) {
				tmp = it->mTo;
				break;
			}

	if (iVar3 != tmp) {
		mOwner->getMActor()->setBckFromIndex(tmp);
		result = true;
	}

	return result;
}

bool TLodAnm::setBtpAnm_(int param_1)
{
	if (param_1 < 0) {
		mOwner->getMActor()->setBtpFromIndex(-1);
		return true;
	}

	bool result = false;

	int iVar3 = mOwner->getMActor()->getCurAnmIdx(ANM_TYPE_BTP);

	int tmp = mLodAnmIndexTable[param_1].mBtpIndex[mCurrentLod];

	if (mIndividualBtp != nullptr)
		for (const TAnmBtpMapping* it = mIndividualBtp; it->mFrom >= 0; ++it)
			if (tmp == it->mFrom) {
				tmp = it->mTo;
				break;
			}

	if (iVar3 != tmp) {
		mOwner->getMActor()->setBtpFromIndex(tmp);
		result = true;
	}

	return result;
}

bool TLodAnm::setBckAndBtpAnm(int param_1)
{
	bool result = setBckAnm_(param_1);
	if (mLodAnmIndexTable != nullptr)
		setBtpAnm_(param_1);
	mCurrentAnmKind = param_1;
	return result;
}

void TLodAnm::execChangeLod() { }
