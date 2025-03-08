#include <JSystem/JAudio/JASystem/JASBasicInst.hpp>
#include <JSystem/JAudio/JASystem/JASBank.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JAudio/JASystem/JASInstEffect.hpp>
#include <types.h>

namespace JASystem {

TBasicInst::TBasicInst()
    : unk4(1.0f)
    , unk8(1.0f)
    , unkC(nullptr)
    , unk10(0)
    , unk14(nullptr)
    , unk18(0)
    , unk1C(0)
    , unk20(nullptr)
{
}

TBasicInst::~TBasicInst()
{
	delete[] unk20;
	delete[] unkC;
	delete[] unk14;
}

TBasicInst::TKeymap* TBasicInst::searchKeymap(int key) const
{
	for (int i = 0; i < unk1C; i++) {
		if (key <= unk20[i].unk0) {
			return &unk20[i];
		}
	}
	return nullptr;
}

bool TBasicInst::getParam(int key, int velo, TInstParam* param) const
{
	char trash[4]; // TODO: what is this

	param->unk0      = 0;
	param->unk38     = 0;
	param->mOscData  = unk14;
	param->mOscCount = unk18;
	param->unk10 *= unk4;
	param->unk14 *= unk8;

	for (int i = 0; i < unk10; i++) {
		TInstEffect* effect = unkC[i];
		if (!effect)
			continue;

		f32 y = effect->getY(key, velo);
		switch (effect->unk4) {
		case 0:
			param->unk18 *= y;
			break;
		case 1:
			param->unk1C *= y;
			break;
		case 2:
			param->unk2C = y;
			break;
		case 3:
			param->unk30 = y;
			break;
		case 4:
			param->unk34 = y;
			break;
		}
	}

	const TKeymap* keymap = nullptr;
	for (int i = 0; i < unk1C; i++) {
		if (key <= unk20[i].unk0) {
			keymap       = &unk20[i];
			param->unk3C = i;
			break;
		}
	}

	if (!keymap)
		return false;

	for (int i = 0; i < keymap->unk4; i++) {
		const TVeloRegion* region = keymap->getVeloRegion(i);
		if (velo <= region->unk0) {
			param->unk10 *= region->unk8;
			param->unk14 *= region->unkC;
			param->unk4 = region->unk4;
			return true;
		}
	}

	return false;
}

int TBasicInst::getKeymapIndex(int index) const { return index; }

void TBasicInst::setKeyRegionCount(u32 count)
{
	delete[] unk20;
	unk20 = new (TBank::getCurrentHeap(), 0) TKeymap[count];
	unk1C = count;
}

void TBasicInst::setEffectCount(u32 count)
{
	delete[] unkC;
	unk10 = count;
	if (!count) {
		unkC = nullptr;
		return;
	}

	unkC = new (TBank::getCurrentHeap(), 0) TInstEffect*[count];
	Calc::bzero(unkC, count * sizeof(TInstEffect*));
}

void TBasicInst::setEffect(int index, TInstEffect* effect)
{
	unkC[index] = effect;
}

TInstEffect* TBasicInst::getEffect(int index) { return unkC[index]; }

void TBasicInst::setOscCount(u32 count)
{
	delete[] unk14;
	unk18 = count;
	if (!count) {
		unk14 = nullptr;
		return;
	}

	unk14 = new (TBank::getCurrentHeap(), 0) TOscillator::Osc_*[count];
	Calc::bzero(unk14, count * sizeof(TOscillator::Osc_*));
}

void TBasicInst::setOsc(int index, TOscillator::Osc_* osc)
{
	unk14[index] = osc;
}

TOscillator::Osc_* TBasicInst::getOsc(int index) { return unk14[index]; }

TBasicInst::TKeymap* TBasicInst::getKeyRegion(int index)
{
	if (index >= unk1C)
		return nullptr;
	return &unk20[index];
}

const TBasicInst::TKeymap* TBasicInst::getKeyRegion(int index) const
{
	if (index >= unk1C)
		return nullptr;
	return &unk20[index];
}

TBasicInst::TKeymap::~TKeymap() { delete[] unk8; }

void TBasicInst::TKeymap::setVeloRegionCount(u32 count)
{
	delete[] unk8;
	unk8 = new (TBank::getCurrentHeap(), 0) TVeloRegion[count];
	unk4 = count;
}

TVeloRegion* TBasicInst::TKeymap::getVeloRegion(int index)
{
	if (index >= unk4)
		return nullptr;
	return &unk8[index];
}

const TVeloRegion* TBasicInst::TKeymap::getVeloRegion(int index) const
{
	if (index >= unk4)
		return nullptr;
	return &unk8[index];
}

} // namespace JASystem
