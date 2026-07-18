#include <M3DUtil/LodAnm.hpp>
#include <M3DUtil/MActor.hpp>
#include <Strategic/LiveActor.hpp>

TLodAnm::TLodAnm(TLiveActor* param_1, const TLodAnmIndex* param_2, int param_3,
                 f32 param_4)
{
	mOwner            = param_1;
	mLodAnmIndexTable = param_2;
	unk8              = param_3;
	unkC              = param_4;
	unk10             = 0;
	mCurrentAnmKind   = -1;
	unk18             = nullptr;
	unk1C             = 0;
	if (param_2) {
		while (param_2[unk10].unk0[0] >= -1)
			++unk10;
	}
}

bool TLodAnm::setBckAnm_(int param_1)
{
	if (param_1 < 0) {
		mOwner->getMActor()->setBckFromIndex(-1);
		return true;
	}

	bool result = false;

	int iVar3 = mOwner->getMActor()->getCurAnmIdx(0);

	int tmp;
	if (mLodAnmIndexTable == nullptr)
		tmp = param_1;
	else
		tmp = mLodAnmIndexTable[param_1].unk0[unk8];

	if (unk18 != nullptr)
		for (const TAnmBckMapping* it = unk18; it->unk0 >= 0; ++it)
			if (tmp == it->unk0) {
				tmp = it->unk4;
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

	int iVar3 = mOwner->getMActor()->getCurAnmIdx(3);

	int tmp = mLodAnmIndexTable[param_1].unk8[unk8];

	if (unk1C != nullptr)
		for (const TAnmBtpMapping* it = unk1C; it->unk0 >= 0; ++it)
			if (tmp == it->unk0) {
				tmp = it->unk4;
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
