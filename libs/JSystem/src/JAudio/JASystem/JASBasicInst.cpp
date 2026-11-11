#include <JSystem/JAudio/JASystem/JASBasicInst.hpp>
#include <JSystem/JAudio/JASystem/JASBank.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JAudio/JASystem/JASInstEffect.hpp>
#include <dolphin/types.h>

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

bool TBasicInst::getParam(int key, int velocity, TInstParam* out_param) const
{
	out_param->mSourceType = 0;
	out_param->mFixedPitch = 0;
	out_param->mOscData    = mOscillators;
	out_param->mOscCount   = mOscillatorCount;
	out_param->mVolume *= mVolume;
	out_param->mPitch *= mPitch;

	for (int i = 0; i < mEffectCount; i++) {
		TInstEffect* effect = mEffects[i];
		if (!effect)
			continue;

		f32 y = effect->getY(key, velocity);
		switch (effect->mTarget) {
		case EFFECT_VOLUME:
			out_param->mEffectVolume *= y;
			break;
		case EFFECT_PITCH:
			out_param->mEffectPitch *= y;
			break;
		case EFFECT_PAN:
			out_param->mEffectPan = y;
			break;
		case EFFECT_FXMIX:
			out_param->mEffectFxmix = y;
			break;
		case EFFECT_DOLBY:
			out_param->mEffectDolby = y;
			break;
		}
	}

	const TKeymap* keymap = nullptr;
	for (int i = 0; i < mKeyRegionCount; i++) {
		if (key <= mKeyRegions[i].mHighKey) {
			keymap                     = &mKeyRegions[i];
			out_param->mKeyRegionIndex = i;
			break;
		}
	}

	if (!keymap)
		return false;

	for (int i = 0; i < keymap->getVeloRegionCount(); i++) {
		const TVeloRegion* region = keymap->getVeloRegion(i);
		if (velocity <= region->unk0) {
			out_param->mVolume *= region->mVolume;
			out_param->mPitch *= region->mPitch;
			out_param->mWaveId = region->mWaveId;
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
