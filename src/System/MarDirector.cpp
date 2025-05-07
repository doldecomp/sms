#include <System/MarDirector.hpp>
#include <System/PerformList.hpp>
#include <System/EventWatcher.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TMarDirector::TMarDirector()
    : unk18(nullptr)
    , unk1C(0)
    , unk20(0)
    , unk24(nullptr)
    , unk28(nullptr)
    , unk2C(nullptr)
    , unk30(new TPerformList)
    , unk34(new TPerformList)
    , unk38(new TPerformList)
    , unk3C(new TPerformList)
    , unk40(new TPerformList)
    , unk44(0)
    , unk48(0)
    , unk4C(0)
    , unk4E(0)
    , unk50(0)
    , unk54(0)
    , unk68(0)
    , unk6C(120.0f)
    , unk80(nullptr)
    , unkA0(0)
    , unkB8(0)
    , unkBC(0)
    , unkCC(0)
    , unkC8(0)
    , unkD4(0)
    , unkD8(0)
    , unkDC(0)
    , unk128(0)
    , unk24C(0)
    , unk24D(0)
    , unk250(0)
    , unk25C(0)
    , unk260(0)
{
	gpMarDirector = this;
	unk58         = 0;
	unk5C         = 0;
	unk64         = 0;
	unk88.reserve(100);
	initLoadParticle();
	unk126 = 0;
	unk125 = 0;
	unk124 = 0;
	OSInitStopwatch(&unkE8, "イベント用ストップウォッチ");
}

void* TMarDirector::setupThreadFunc(void* param_1)
{
	((TMarDirector*)param_1)->loadResource();
}

extern OSThread gSetupThread;
extern u8* gpSetupThreadStack;

u32 TMarDirector::setup(JDrama::TDisplay* param_1, TMarioGamePad** param_2,
                        u8 param_3, u8 param_4)
{
	unkC0 = param_1;
	unk18 = param_2;
	map   = param_3;
	_07D  = param_4;
	OSCreateThread(&gSetupThread, &setupThreadFunc, this,
	               (void*)(gpSetupThreadStack + 0x10000), 0x10000, 0x11, 0);
	OSResumeThread(&gSetupThread);
	return 0;
}

void TMarDirector::registerEventWatcher(TEventWatcher* param_1)
{
	unk80->insert(param_1);
}
