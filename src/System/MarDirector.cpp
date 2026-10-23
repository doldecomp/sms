#include <System/MarDirector.hpp>
#include <System/PerformList.hpp>
#include <System/EventWatcher.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static void dummy(Vec* v) { *v = (Vec) { 0.0f, 0.0f, 0.0f }; }

void* gpSceneCmnDat;
int gpSceneCmnDatSize;

TMarDirector::TMarDirector()
    : mGamePads(nullptr)
    , mPerformListGX(nullptr)
    , mPerformListSilhouette(nullptr)
    , mPerformListGXPost(nullptr)
    , mPerformListMovement(nullptr)
    , mPerformListCalcAnim(nullptr)
    , mPerformListConsole(new TPerformList)
    , mPerformListPreEntry(new TPerformList)
    , mPerformListPollutionCountObj(new TPerformList)
    , mPerformListPollutionCountLayer(new TPerformList)
    , mPerformListDrawInit(new TPerformList)
    , mPerformListShineMovement(nullptr)
    , mPerformListShineCalcAnim(nullptr)
    , mFlags(0)
    , mDemoFlags(0)
    , mTransitionFlags(0)
    , mPendingSimulationTime(0)
    , unk68(0)
    , unk6C(120.0f)
    , unk80(nullptr)
    , unkA0(nullptr)
    , unkB8(nullptr)
    , unkBC(nullptr)
    , unkC8(0)
    , m2DArchiveBuffer(0)
    , m2DArchive(0)
    , unkDC(nullptr)
    , mTalkFlags(0)
    , mDemoQueueTail(0)
    , mDemoQueueHead(0)
    , mGateDemoActor(0)
    , unk25C(nullptr)
    , mSetupDone(false)
{
	gpMarDirector  = this;
	mMoveTickCount = 0;
	mTickCount     = 0;
	mState         = STATE_UNK0;
	unk88.reserve(100);
	initLoadParticle();
	mNextGameMode = GAME_MODE_NORMAL;
	mPrevGameMode = GAME_MODE_NORMAL;
	mGameMode     = GAME_MODE_NORMAL;
	OSInitStopwatch(&unkE8, "イベント用ストップウォッチ");
}

void* TMarDirector::setupThreadFunc(void* param_1)
{
	((TMarDirector*)param_1)->loadResource();
}

extern OSThread gSetupThread;
extern u8* gpSetupThreadStack;

u32 TMarDirector::setup(JDrama::TDisplay* display, TMarioGamePad** game_pads,
                        u8 stage, u8 scenario)
{
	mDisplay  = display;
	mGamePads = game_pads;
	mStage    = stage;
	mScenario = scenario;
	OSCreateThread(&gSetupThread, &setupThreadFunc, this,
	               (void*)(gpSetupThreadStack + 0x10000), 0x10000, 0x11, 0);
	OSResumeThread(&gSetupThread);
	return 0;
}

void TMarDirector::registerEventWatcher(TEventWatcher* param_1)
{
	unk80->insert(param_1);
}
