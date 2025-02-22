#include <JSystem/JDrama/JDRVideo.hpp>
#include <JSystem/JDrama/JDRRenderMode.hpp>
#include <dolphin/vi.h>
#include <dolphin/os.h>
#include <types.h>

using namespace JDrama;

TVideo::TVideo()
{
	mCurFrameBuffer   = nullptr;
	mNextFrameBuffer  = nullptr;
	mLastRetraceTime  = OSGetTick();
	mNextRetraceIndex = VIGetRetraceCount() + 1;

	mNextRenderMode.viTVmode  = (VITVMode)-1;
	mNextRenderMode.fbWidth   = 0;
	mNextRenderMode.efbHeight = 0;
	mNextRenderMode.xfbHeight = 0;
	mNextRenderMode.viXOrigin = 0;
	mNextRenderMode.viYOrigin = 0;
	mNextRenderMode.viWidth   = 0;
	mNextRenderMode.viHeight  = 0;
	mNextRenderMode.xFBmode   = (VIXFBMode)-1;

	mCurRenderMode = mNextRenderMode;
}

void TVideo::setNextXFB(const void* fb) { mNextFrameBuffer = fb; }

void TVideo::waitForRetrace(u16 param_1)
{
	while (mNextRetraceIndex - (int)VIGetRetraceCount() > 1)
		VIWaitForRetrace();

	if (!IsEqualRenderModeVIParams(mCurRenderMode, mNextRenderMode)) {
		VIConfigure(&mNextRenderMode);
		if (mCurRenderMode.viTVmode != mNextRenderMode.viTVmode) {
			VISetBlack(1);
			mCurFrameBuffer = 0;
			VIFlush();
			VIWaitForRetrace();
			s32 uVar11 = mCurRenderMode.viTVmode & 3;
			if (((uVar11 == 2) && ((mNextRenderMode.viTVmode & 3) != 2))
			    || ((uVar11 != 2 && ((mNextRenderMode.viTVmode & 3) == 2)))) {

				for (int i = 0; i < 0x3C; ++i)
					VIWaitForRetrace();
			}
		}
	}

	if (mCurFrameBuffer != mNextFrameBuffer) {
		if (mNextFrameBuffer != nullptr) {
			VISetNextFrameBuffer((void*)mNextFrameBuffer);
			VISetBlack(0);
		} else {
			VISetBlack(1);
		}
	}

	mCurRenderMode  = mNextRenderMode;
	mCurFrameBuffer = mNextFrameBuffer;
	VIFlush();
	VIWaitForRetrace();
	mLastRetraceTime  = OSGetTick();
	mNextRetraceIndex = param_1 + VIGetRetraceCount();
}
