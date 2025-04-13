#include <MarioUtil/GDUtil.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <macros.h>

static TGDLStatic* currentTGDLStatic;

void TGDLStaticOverFlow() { currentTGDLStatic->unk11 = 1; }

TGDLStatic::TGDLSentinel::~TGDLSentinel()
{
	TGDLStatic* gdls = unk18;
	gdls->mDispList  = nullptr;
	gdls->unk1C      = nullptr;
	gdls->unk10      = 0;
}

TGDLStatic::~TGDLStatic() { mDispList = nullptr; }

void TGDLStatic::alloc(u32 size)
{
	mDispListSize = ALIGN_NEXT(size, 0x20);
	mDispList     = new (0x20) u8[mDispListSize];
	memset(mDispList, 0, mDispListSize);
	GDInitGDLObj(&mDispListObj, mDispList, mDispListSize);
	GDSetOverflowCallback(&TGDLStaticOverFlow);
	unk10 = 0;
	unk1C = new TGDLSentinel(this);
}

void TGDLStatic::make()
{
	if (!mDispList)
		alloc(mDispListSize);

	GDLObj* prev;

	prev = GDGetCurrent();
	for (;;) {
		unk11             = 0;
		currentTGDLStatic = this;
		GDSetCurrent(&mDispListObj);
		makeDL();
		GDPadCurr32();
		GDFlushCurrToMem();
		if (unk11 == 0) {
			unk10 = 1;
			break;
		}

		mDispListSize *= 2;
		alloc(mDispListSize);
	}
	GDSetCurrent(prev);
}

void TGDLDynamic::TGDLStaticAlt::makeDL() { }

TGDLDynamic::TGDLStaticAlt::~TGDLStaticAlt() { }
