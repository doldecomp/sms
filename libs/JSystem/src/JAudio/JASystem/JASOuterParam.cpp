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
	f32* tgt;
	switch (param) {
	case TTrack::UPDATE_VOLUME:
		tgt = &mVolume;
		break;
	case TTrack::UPDATE_PITCH:
		tgt = &mPitch;
		break;
	case TTrack::UPDATE_FXMIX:
		tgt = &mFxmix;
		break;
	case TTrack::UPDATE_DOLBY:
		tgt = &mDolby;
		break;
	case TTrack::UPDATE_PAN:
		tgt = &mPan;
		break;
	case TTrack::UPDATE_TEMPO:
		tgt = &mTempo;
		break;
	default:
		return;
	}
	*tgt = value;
	mDirtyParams |= param;
}

void TTrack::TOuterParam::onSwitch(u16 param)
{
	mEnabledParams |= param;
	mDirtyParams |= param;
}

void TTrack::TOuterParam::setFirFilter(s16* param)
{
	mDirtyParams |= TTrack::UPDATE_FIR_FILTER;
	mEnabledParams |= TTrack::UPDATE_FIR_FILTER;
	for (u8 i = 0; i < 8; i++)
		mFirFilter[i] = param[i];
}

} // namespace JASystem
