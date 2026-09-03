#include <JSystem/JAudio/JASystem/JASDrumSet.hpp>
#include <JSystem/JAudio/JASystem/JASBank.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>

namespace JASystem {

bool TDrumSet::getParam(int key, int param_2, TInstParam* param_3) const
{
	if (key >= 128u)
		return false;

	const TPerc* perc = unk4 + key;

	param_3->mSourceType = 0;
	param_3->mFixedPitch = 1;
	param_3->mVolume *= perc->mVolume;
	param_3->mPitch *= perc->mPitch;
	param_3->mPan     = perc->mPan;
	param_3->mRelease = perc->mRelease;

	static TOscillator::Osc_ osc;

	osc.mTarget   = 0;
	osc.mRate     = 1.0f;
	osc.mAdsTable = nullptr;
	osc.mRelTable = nullptr;
	osc.mWidth    = 1.0f;
	osc.mVertex   = 0.0f;

	static TOscillator::Osc_* oscp = &osc;

	param_3->mOscData  = &oscp;
	param_3->mOscCount = 1;

	for (int i = 0; i < perc->mEffectCount; i++) {
		TInstEffect* effect = perc->mEffects[i];
		if (effect) {
			f32 y = effect->getY(key, param_2);
			switch (effect->unk4) {
			case 0:
				param_3->mEffectVolume *= y;
				break;
			case 1:
				param_3->mEffectPitch *= y;
				break;
			case 2:
				param_3->mEffectPan = y;
				break;
			case 3:
				param_3->mEffectFxmix = y;
				break;
			case 4:
				param_3->mEffectDolby = y;
				break;
			}
		}
	}
	for (int i = 0; i < perc->mVeloRegionCount; i++) {
		TVeloRegion* region = &perc->mVeloRegions[i];
		if (param_2 > region->unk0) {
			continue;
		}
		param_3->mVolume *= region->unk8;
		param_3->mPitch *= region->unkC;
		param_3->mWaveId = region->unk4;
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
    : mVolume(1.0f)
    , mPitch(1.0f)
    , mPan(0.5f)
    , mRelease(1000)
    , mEffects(nullptr)
    , mEffectCount(0)
    , mVeloRegionCount(0)
    , mVeloRegions(nullptr)
{
}

TDrumSet::TPerc::~TPerc()
{
	delete[] mEffects;
	delete[] mVeloRegions;
}

void TDrumSet::TPerc::setEffectCount(u32 count)
{
	delete[] mEffects;
	mEffectCount = count;
	if (count == 0) {
		mEffects = nullptr;
		return;
	}

	mEffects = new (TBank::getCurrentHeap(), 0) TInstEffect*[count];
	Calc::bzero(mEffects, count * sizeof(TInstEffect*));
}

void TDrumSet::TPerc::setVeloRegionCount(u32 count)
{
	delete[] mVeloRegions;
	mVeloRegions     = new (TBank::getCurrentHeap(), 0) TVeloRegion[count];
	mVeloRegionCount = count;
}

TVeloRegion* TDrumSet::TPerc::getVeloRegion(int index)
{
	return &mVeloRegions[index];
}

const TVeloRegion* TDrumSet::TPerc::getVeloRegion(int index) const
{
	return &mVeloRegions[index];
}

void TDrumSet::TPerc::setEffect(int index, TInstEffect* effect)
{
	mEffects[index] = effect;
}

TInstEffect* TDrumSet::TPerc::getEffect(int index) { return mEffects[index]; }

void TDrumSet::TPerc::setRelease(u32 value) { mRelease = value; }

} // namespace JASystem
