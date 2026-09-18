#include <System/MarDirector.hpp>
#include <System/PerformList.hpp>
#include <System/EventWatcher.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static void dummy(Vec* v) { *v = (Vec) { 0.0f, 0.0f, 0.0f }; }

void* gpSceneCmnDat;
int gpSceneCmnDatSize;

// TODO: blocked on one shared-header change in
// include/JSystem/JGadget/std-vector.hpp. `JGadget::TVector_pointer<T>` should
// carry the same ctor shape its list sibling already has:
//
//   TVector_pointer(const JGadget::TAllocator<void*>& allocator
//                   = JGadget::TAllocator<void*>())
//       : TVector_pointer_void(allocator) { }
//
// in place of `TVector_pointer() { }`. The default-argument
// `TAllocator<void*>()` temporary is then built by the *caller*, one inline
// level shallower, and lands on retail's 0x34 slot instead of 0x20 -- the only
// difference left in this 892-byte constructor (frame 0x60 is already right).
// Measured: with that header change this constructor and the whole unit reach
// 100%, `TCubeManagerBase::TCubeManagerBase` 57.76 -> 57.93,
// `TMarNameRefGen::getNameRef` 92.79 -> 92.81, no regressions, DOL unchanged.
// Ruled out here: dropping `TVector_pointer() { }` for the implicit ctor (no
// change), `unk88()` written first, last or in member order in the initialiser
// list (no change), `new TPerformList()`, moving `unk88.reserve(100)` after
// `initLoadParticle()` (-1.5), an uninitialised body local. Declaring unk88 as
// a bare `TVector_pointer_void` does put the temporary at 0x34, which is what
// identified the depth as the lever, but the map's UNUSED
// `__dt__Q27JGadget27TVector_pointer<P8TBaseNPC>Fv` pins the real type.
TMarDirector::TMarDirector()
    : unk18(nullptr)
    , mPerformListGX(nullptr)
    , mPerformListSilhouette(nullptr)
    , mPerformListGXPost(nullptr)
    , mPerformListMovement(nullptr)
    , mPerformListCalcAnim(nullptr)
    , unk30(new TPerformList)
    , unk34(new TPerformList)
    , unk38(new TPerformList)
    , unk3C(new TPerformList)
    , unk40(new TPerformList)
    , mShinePfLstMov(nullptr)
    , mShinePfLstAnm(nullptr)
    , unk4C(0)
    , unk4E(0)
    , unk50(0)
    , unk54(0)
    , unk68(0)
    , unk6C(120.0f)
    , unk80(nullptr)
    , unkA0(nullptr)
    , unkB8(nullptr)
    , unkBC(nullptr)
    , unkC8(0)
    , unkD4(0)
    , unkD8(0)
    , unkDC(nullptr)
    , unk128(0)
    , unk24C(0)
    , unk24D(0)
    , unk250(0)
    , unk25C(nullptr)
    , unk260(0)
{
	gpMarDirector = this;
	unk58         = 0;
	unk5C         = 0;
	mState        = STATE_UNK0;
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
	mMap  = param_3;
	unk7D = param_4;
	OSCreateThread(&gSetupThread, &setupThreadFunc, this,
	               (void*)(gpSetupThreadStack + 0x10000), 0x10000, 0x11, 0);
	OSResumeThread(&gSetupThread);
	return 0;
}

// TODO: promote into JDRViewObjPtrList.hpp by changing
// TViewObjPtrListT<T>::insert to take `T*` by value instead of `T* const&`.
// With the reference parameter, the TEventWatcher* -> TViewObj* conversion
// temporary is bound at the outer call and lands at the bottom of the frame
// (0xc), while retail binds it inside TList_pointer::push_back's `const T&`
// and parks it above the iterator temporaries at 0x3c, with a frame of 0x50
// against 0x48. Taking the pointer by value reproduces retail exactly (probed
// by editing the header). Parked here because shared headers are off limits
// in this batch.
static inline void
MarDirectorPushViewObj(JDrama::TViewObjPtrListT<JDrama::TViewObj>* list,
                       JDrama::TViewObj* obj)
{
	list->getChildren().push_back(obj);
}

void TMarDirector::registerEventWatcher(TEventWatcher* param_1)
{
	MarDirectorPushViewObj(unk80, param_1);
}
