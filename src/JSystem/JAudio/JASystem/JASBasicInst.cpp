#include <JSystem/JAudio/JASystem/JASBasicInst.hpp>
#include <JSystem/JAudio/JASystem/JASBank.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JAudio/JASystem/JASInstEffect.hpp>
#include <types.h>

namespace JASystem {

TBasicInst::TBasicInst()
    : mVolume(1.0f)
    , mPitch(1.0f)
    , mEffects(nullptr)
    , mEffectCount(0)
    , mOscillators(nullptr)
    , mOscillatorCount(0)
    , mKeyRegionCount(0)
    , mKeyRegions(nullptr)
{
}

TBasicInst::~TBasicInst()
{
	delete[] mKeyRegions;
	delete[] mEffects;
	delete[] mOscillators;
}

TBasicInst::TKeymap* TBasicInst::searchKeymap(int key) const
{
	for (int i = 0; i < mKeyRegionCount; i++) {
		if (key <= mKeyRegions[i].mHighKey) {
			return &mKeyRegions[i];
		}
	}
	return nullptr;
}

bool TBasicInst::getParam(int key, int velo, TInstParam* param) const
{
	param->mSourceType = 0;
	param->mFixedPitch = 0;
	param->mOscData    = mOscillators;
	param->mOscCount   = mOscillatorCount;
	param->mVolume *= mVolume;
	param->mPitch *= mPitch;

	for (int i = 0; i < mEffectCount; i++) {
		TInstEffect* effect = mEffects[i];
		if (!effect)
			continue;

		f32 y = effect->getY(key, velo);
		switch (effect->mTarget) {
		case 0:
			param->mEffectVolume *= y;
			break;
		case 1:
			param->mEffectPitch *= y;
			break;
		case 2:
			param->mEffectPan = y;
			break;
		case 3:
			param->mEffectFxmix = y;
			break;
		case 4:
			param->mEffectDolby = y;
			break;
		}
	}

	const TKeymap* keymap = nullptr;
	for (int i = 0; i < mKeyRegionCount; i++) {
		if (key <= mKeyRegions[i].mHighKey) {
			keymap       = &mKeyRegions[i];
			param->unk3C = i;
			break;
		}
	}

	if (!keymap)
		return false;

	for (int i = 0; i < keymap->getVeloRegionCount(); i++) {
		const TVeloRegion* region = keymap->getVeloRegion(i);
		if (velo <= region->unk0) {
			param->mVolume *= region->unk8;
			param->mPitch *= region->unkC;
			param->mWaveId = region->unk4;
			return true;
		}
	}

	return false;
}

int TBasicInst::getKeymapIndex(int index) const { return index; }

void TBasicInst::setKeyRegionCount(u32 count)
{
	delete[] mKeyRegions;
	mKeyRegions     = new (TBank::getCurrentHeap(), 0) TKeymap[count];
	mKeyRegionCount = count;
}

void TBasicInst::setEffectCount(u32 count)
{
	delete[] mEffects;
	mEffectCount = count;
	if (!count) {
		mEffects = nullptr;
		return;
	}

	mEffects = new (TBank::getCurrentHeap(), 0) TInstEffect*[count];
	Calc::bzero(mEffects, count * sizeof(TInstEffect*));
}

void TBasicInst::setEffect(int index, TInstEffect* effect)
{
	mEffects[index] = effect;
}

TInstEffect* TBasicInst::getEffect(int index) { return mEffects[index]; }

void TBasicInst::setOscCount(u32 count)
{
	delete[] mOscillators;
	mOscillatorCount = count;
	if (!count) {
		mOscillators = nullptr;
		return;
	}

	mOscillators = new (TBank::getCurrentHeap(), 0) TOscillator::Osc_*[count];
	Calc::bzero(mOscillators, count * sizeof(TOscillator::Osc_*));
}

void TBasicInst::setOsc(int index, TOscillator::Osc_* osc)
{
	mOscillators[index] = osc;
}

TOscillator::Osc_* TBasicInst::getOsc(int index) { return mOscillators[index]; }

TBasicInst::TKeymap* TBasicInst::getKeyRegion(int index)
{
	if (index >= mKeyRegionCount)
		return nullptr;
	return &mKeyRegions[index];
}

const TBasicInst::TKeymap* TBasicInst::getKeyRegion(int index) const
{
	if (index >= mKeyRegionCount)
		return nullptr;
	return &mKeyRegions[index];
}

TBasicInst::TKeymap::~TKeymap() { delete[] mVeloRegions; }

void TBasicInst::TKeymap::setVeloRegionCount(u32 count)
{
	delete[] mVeloRegions;
	mVeloRegions     = new (TBank::getCurrentHeap(), 0) TVeloRegion[count];
	mVeloRegionCount = count;
}

TVeloRegion* TBasicInst::TKeymap::getVeloRegion(int index)
{
	if (index >= mVeloRegionCount)
		return nullptr;
	return &mVeloRegions[index];
}

const TVeloRegion* TBasicInst::TKeymap::getVeloRegion(int index) const
{
	if (index >= mVeloRegionCount)
		return nullptr;
	return &mVeloRegions[index];
}

} // namespace JASystem
