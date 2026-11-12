#include <JSystem/JAudio/JASystem/JASTrack.hpp>

namespace JASystem {

TTrack::TOuterParam::TOuterParam()
{
	mEnabledParams = 0;
	mDirtyParams   = 0;
	mVolume        = 0.0f;
	mPitch         = 0.0f;
	mFxmix         = 0.0f;
	mDolby         = 0.0f;
	mPan           = 0.0f;
	mTempo         = 0.0f;
	for (int i = 0; i < 8; i++)
		mFirFilter[i] = 0;
}

void TTrack::TOuterParam::initExtBuffer()
{
	mEnabledParams = 0;
	mDirtyParams   = 0;
}

void TTrack::TOuterParam::setOuterSwitch(u16 param) { mEnabledParams = param; }

u16 TTrack::TOuterParam::getSwitch() { return mEnabledParams; }

bool TTrack::TOuterParam::checkOuterSwitch(u16 param)
{
	return mEnabledParams & param;
}

void TTrack::TOuterParam::setOuterUpdate(u16 param) { mDirtyParams = param; }

u16 TTrack::TOuterParam::getOuterUpdate() { return mDirtyParams; }

void TTrack::TOuterParam::setIntFirFilter(s16 param1, u8 param2)
{
	mFirFilter[param2] = param1;
}

s16 TTrack::TOuterParam::getIntFirFilter(u8 param) { return mFirFilter[param]; }

void TTrack::TOuterParam::setParam(u8 param, f32 value)
{
	f32* var1;
	switch (param) {
	case TTrack::UPDATE_Volume:
		var1 = &mVolume;
		break;
	case TTrack::UPDATE_Pitch:
		var1 = &mPitch;
		break;
	case TTrack::UPDATE_Fxmix:
		var1 = &mFxmix;
		break;
	case TTrack::UPDATE_Dolby:
		var1 = &mDolby;
		break;
	case TTrack::UPDATE_Pan:
		var1 = &mPan;
		break;
	case TTrack::UPDATE_Tempo:
		var1 = &mTempo;
		break;
	default:
		return;
	}
	*var1 = value;
	mDirtyParams |= param;
}

void TTrack::TOuterParam::onSwitch(u16 param)
{
	mEnabledParams |= param;
	mDirtyParams |= param;
}

void TTrack::TOuterParam::setFirFilter(s16* param)
{
	mDirtyParams |= TTrack::UPDATE_FirFilter;
	mEnabledParams |= TTrack::UPDATE_FirFilter;
	for (u8 i = 0; i < 8; i++)
		mFirFilter[i] = param[i];
}

} // namespace JASystem
