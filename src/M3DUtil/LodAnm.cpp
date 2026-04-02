#include <M3DUtil/LodAnm.hpp>
#include <M3DUtil/MActor.hpp>
#include <Strategic/LiveActor.hpp>

TLodAnm::TLodAnm(TLiveActor* param_1, const TLodAnmIndex* param_2, int param_3,
                 f32 param_4)
{
	unk0  = param_1;
	unk4  = param_2;
	unk8  = param_3;
	unkC  = param_4;
	unk10 = 0;
	unk14 = -1;
	unk18 = nullptr;
	unk1C = 0;
	if (param_2) {
		while (param_2[unk10].unk0[0] >= -1)
			++unk10;
	}
}

bool TLodAnm::setBckAnm_(int param_1)
{
	if (param_1 < 0) {
		unk0->getMActor()->setBckFromIndex(-1);
		return true;
	}

	bool result = false;

	int iVar3 = unk0->getMActor()->getCurAnmIdx(0);

	int tmp;
	if (unk4 == nullptr)
		tmp = param_1;
	else
		tmp = unk4[param_1].unk0[unk8];

	if (unk18 != nullptr)
		for (BckMapping* it = unk18; it->unk0 >= 0; ++it)
			if (tmp == it->unk0) {
				tmp = it->unk4;
				break;
			}

	if (iVar3 != tmp) {
		unk0->getMActor()->setBckFromIndex(tmp);
		result = true;
	}

	return result;
}

bool TLodAnm::setBtpAnm_(int param_1)
{
	if (param_1 < 0) {
		unk0->getMActor()->setBtpFromIndex(-1);
		return true;
	}

	bool result = false;

	int iVar3 = unk0->getMActor()->getCurAnmIdx(3);

	int tmp = unk4[param_1].unk8[unk8];

	if (unk1C != nullptr)
		for (BtpMapping* it = unk1C; it->unk0 >= 0; ++it)
			if (tmp == it->unk0) {
				tmp = it->unk4;
				break;
			}

	if (iVar3 != tmp) {
		unk0->getMActor()->setBtpFromIndex(tmp);
		result = true;
	}

	return result;
}

bool TLodAnm::setBckAndBtpAnm(int param_1)
{
	bool result = setBckAnm_(param_1);
	if (unk4 != nullptr)
		setBtpAnm_(param_1);
	unk14 = param_1;
	return result;
}
