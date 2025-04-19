#include <System/ZBufferCatch.hpp>
#include <System/DrawSyncManager.hpp>
#include <MarioUtil/ReinitGX.hpp>
#include <dolphin/gx.h>

void TZBufferCatch::perform(u32 param_1, JDrama::TGraphics*)
{
	if (param_1 & 8) {
		TDrawSyncManager::smInstance->pushBreakPoint();
		ReInitializeGX();
		GXSetDrawSync(0x7D);
	}
}

void TAlphaCatch::perform(u32 param_1, JDrama::TGraphics*)
{
	if (param_1 & 8) {
		TDrawSyncManager::smInstance->pushBreakPoint();
		GXSetDrawSync(0x7C);
		TDrawSyncManager::smInstance->pushBreakPoint();
		GXSetDrawSync(0x0);
	}
}
