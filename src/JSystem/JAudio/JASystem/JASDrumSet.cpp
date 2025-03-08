#include <JSystem/JAudio/JASystem/JASDrumSet.hpp>
#include <JSystem/JAudio/JASystem/JASBank.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>

namespace JASystem {

bool TDrumSet::getParam(int key, int param_2, TInstParam* param_3) const
{
	if (key >= 128u)
		return false;

	const TPerc* perc = unk4 + key;

	param_3->unk0  = 0;
	param_3->unk38 = 1;
	param_3->unk10 *= perc->unk0;
	param_3->unk14 *= perc->unk4;
	param_3->unk20 = perc->unk8;
	param_3->unk3A = perc->unkC;

	static TOscillator::Osc_ osc;

	osc.unk0  = 0;
	osc.unk4  = 1.0f;
	osc.unk8  = nullptr;
	osc.unkC  = nullptr;
	osc.unk10 = 1.0f;
	osc.unk14 = 0.0f;

	static TOscillator::Osc_* oscp = &osc;

	param_3->mOscData  = &oscp;
	param_3->mOscCount = 1;

	for (int i = 0; i < perc->unk14; i++) {
		TInstEffect* effect = perc->unk10[i];
		if (effect) {
			f32 y = effect->getY(key, param_2);
			switch (effect->unk4) {
			case 0:
				param_3->unk18 *= y;
				break;
			case 1:
				param_3->unk1C *= y;
				break;
			case 2:
				param_3->unk2C = y;
				break;
			case 3:
				param_3->unk30 = y;
				break;
			case 4:
				param_3->unk34 = y;
				break;
			}
		}
	}
	for (int i = 0; i < perc->unk18; i++) {
		TVeloRegion* region = &perc->unk1C[i];
		if (param_2 > region->unk0) {
			continue;
		}
		param_3->unk10 *= region->unk8;
		param_3->unk14 *= region->unkC;
		param_3->unk4 = region->unk4;
		return true;
	}
	return false;
}

TDrumSet::TPerc* TDrumSet::getPerc(int index) { return &unk4[index]; }

const TDrumSet::TPerc* TDrumSet::getPerc(int index) const
{
	return &unk4[index];
}

TDrumSet::TPerc::TPerc()
    : unk0(1.0f)
    , unk4(1.0f)
    , unk8(0.5f)
    , unkC(1000)
    , unk10(nullptr)
    , unk14(0)
    , unk18(0)
    , unk1C(nullptr)
{
}

TDrumSet::TPerc::~TPerc()
{
	delete[] unk10;
	delete[] unk1C;
}

void TDrumSet::TPerc::setEffectCount(u32 count)
{
	delete[] unk10;
	unk14 = count;
	if (count == 0) {
		unk10 = nullptr;
		return;
	}

	unk10 = new (TBank::getCurrentHeap(), 0) TInstEffect*[count];
	Calc::bzero(unk10, count * sizeof(TInstEffect*));
}

void TDrumSet::TPerc::setVeloRegionCount(u32 count)
{
	delete[] unk1C;
	unk1C = new (TBank::getCurrentHeap(), 0) TVeloRegion[count];
	unk18 = count;
}

TVeloRegion* TDrumSet::TPerc::getVeloRegion(int index) { return &unk1C[index]; }

const TVeloRegion* TDrumSet::TPerc::getVeloRegion(int index) const
{
	return &unk1C[index];
}

void TDrumSet::TPerc::setEffect(int index, TInstEffect* effect)
{
	unk10[index] = effect;
}

TInstEffect* TDrumSet::TPerc::getEffect(int index) { return unk10[index]; }

void TDrumSet::TPerc::setRelease(u32 value) { unkC = value; }

} // namespace JASystem
