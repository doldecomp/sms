#include <string.h>
#include <MarioUtil/GDUtil.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <macros.h>

static TGDLStatic* currentTGDLStatic;

static void TGDLStaticOverFlow() { currentTGDLStatic->onOverflow(); }

TGDLStatic::TGDLSentinel::~TGDLSentinel()
{
	TGDLStatic* gdls = unk18;
	gdls->mDispList  = nullptr;
	gdls->unk1C      = nullptr;
	gdls->mReady     = false;
}

TGDLStatic::~TGDLStatic() { mDispList = nullptr; }

void TGDLStatic::alloc(u32 size)
{
	mDispListSize = ALIGN_NEXT(size, 0x20);
	mDispList     = new (0x20) u8[mDispListSize];
	memset(mDispList, 0, mDispListSize);
	GDInitGDLObj(&mDispListObj, mDispList, mDispListSize);
	GDSetOverflowCallback(&TGDLStaticOverFlow);
	mReady = false;
	unk1C  = new TGDLSentinel(this);
}

void TGDLStatic::make()
{
	if (!mDispList)
		alloc(mDispListSize);

	GDLObj* prev;

	prev = GDGetCurrent();
	for (;;) {
		mOverflowHappened = false;
		currentTGDLStatic = this;
		GDSetCurrent(&mDispListObj);
		makeDL();
		GDPadCurr32();
		GDFlushCurrToMem();
		if (!mOverflowHappened) {
			mReady = true;
			break;
		}

		mDispListSize *= 2;
		alloc(mDispListSize);
	}
	GDSetCurrent(prev);
}

void TGDLDynamic::TGDLStaticAlt::makeDL() { }

TGDLDynamic::TGDLStaticAlt::~TGDLStaticAlt() { }
