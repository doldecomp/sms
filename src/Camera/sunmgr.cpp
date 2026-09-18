#include <Camera/SunMgr.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <System/PositionHolder.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <Camera/SunModel.hpp>
#include <Camera/Camera.hpp>
#include <Player/MarioAccess.hpp>
#include <MSound/MSound.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

const char* cSunWarpPointName = "太陽ワープポイント";

TSunMgr* gpSunMgr;

TSunMgr::TSunMgr(const char* name)
    : JDrama::TViewObj(name)
    , unk14(0)
    , unk15(0)
    , unk20(0.0f)
    , unk24(0.0f, 0.0f, 0.0f)
{
	gpSunMgr = this;
}

static inline TSunModel* SunMgrSearch(const char* name)
{
	TSunModel* model = JDrama::TNameRefGen::search<TSunModel>(name);
	return model;
}

void TSunMgr::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);

	// Declared in reverse read order on purpose: named locals grow down from
	// the top of the frame in declaration order, and retail reads the four
	// colour inputs into *ascending* slots (0x8c, 0x90, 0x94, 0x98), so the
	// first value read is the last one declared.
	// TODO: the frame is now retail's 0xb0 and all 114 instructions match, but
	// r30 (`this`) and r31 (the .rodata string-pool base) are still swapped:
	// retail ranks the compiler-generated pool address above the parameter
	// even though `this` has eleven uses to the pool's four, which header
	// round 18's liveness rule does not explain (`this` is live from entry).
	// The 64 bytes of low region that were missing are: chaining the five
	// stream reads into one statement (four continuations, +0x20, the batch-81
	// rule), one level that binds the name-ref search result at both sites
	// (+0x18) and `SMSGetMarDirector()` over the raw global (+8).
	// Also measured: a binding level on `getCurrentMap()` +0x10 but one extra
	// diff, on `TFlagManager::getInstance()->getBool()` +0, on the
	// position-holder search +0x10 and three extra instructions; `search2`
	// with the cast at the call site is codegen-identical here. Splitting the
	// chain differently gives 8 bytes per continuation exactly (4/1 and 3/2
	// both 0x88, 2/2/1 0x80). `unk24.set(...)` over the assignment costs 3
	// instructions. Rejected earlier: readU32() for the four inputs (0xa8 but
	// 121 instructions), an array plus a four-read loop (batch 32).
	u32 local_18;
	u32 local_1c;
	u32 local_20;
	u32 local_24;
	stream >> local_24 >> local_20 >> local_1c >> local_18 >> unk20;

	u32 col1 = local_24 << 8 | local_20;
	u32 col2 = local_1c << 8 | local_18;
	unk18.set(col1);
	unk1C.set(col2);

	TSunModel* sun = SunMgrSearch("太陽モデル");
	if (sun != nullptr) {
		unk14 = 1;
	} else {
		sun = SunMgrSearch("夕日モデル");
		if (sun != nullptr) {
			unk14 = 1;
			unk15 |= 0x2;
		}
	}

	if (unk14 != 0 && SMSGetMarDirector()->getCurrentMap() == 1
	    && TFlagManager::getInstance()->getBool(0x50004)) {
		unk15 |= 0x1;
		TStagePositionInfo* sunWarpPoint
		    = (TStagePositionInfo*)gpPositionHolder->searchF(
		        JDrama::TNameRef::calcKeyCode(cSunWarpPointName),
		        cSunWarpPointName);
		unk24 = sunWarpPoint->unkC;
	}
}

// The 48 bytes of dead low region are six inline expansions, each a read
// through one level that binds its result: `gpMarioPos->x`/`->z` through the
// real `SMS_GetMarioX()`/`SMS_GetMarioZ()` (+8 for the pair), `SMSGetMSound()`
// (+8) and the BGM handle at its three sites (+8 each) reach retail's 0x60
// exactly at 107 instructions.
// Header round 20 promoted the BGM level to `MSound::getBgm()` in
// `MSound/MSound.hpp` (codegen-identical to the parked `static inline` it
// replaced, whole-tree). `SunMgrGetMSound` is a global-accessor fork and stays
// TU-local.
// Other measured levels, all +8 each and interchangeable with the above:
// `gpCamera->isThing2()`, `gpSunModel->isInBounds(0.3f)`. Worth zero:
// `gpMarDirector->setNextStage(9, nullptr)` behind a level.
// Superseded: a single uninitialised 48-byte local reaches 0x60 too, and so
// does one dead 48-byte non-trivial local inside `TSunModel::isInBounds`
// (which also moves the still-open `TLensGlow::perform` 0x120 -> 0x150 toward
// its 0x178); neither names anything, and the level ladder does.
static inline MSound* SunMgrGetMSound()
{
	MSound* sound = SMSGetMSound();
	return sound;
}

void TSunMgr::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!(unk15 & 1))
		return;
	if (!(cue & CUE_MOVE))
		return;
	if (!(graphics->unk0 & 2))
		return;

	if (!gpCamera->isThing2())
		return;

	// Transition to noki bay
	f32 dx = SMS_GetMarioX() - unk24.x;
	f32 dz = SMS_GetMarioZ() - unk24.z;
	if (dx * dx + dz * dz < 160000.0f && gpSunModel->isInBounds(0.3f)) {
		gpMarDirector->setNextStage(9, nullptr);
		MSound* sound = SunMgrGetMSound();
		if (sound->getBgm() != nullptr) {
			sound->getBgm()->setVolume(0.0f, 100, 0);
			sound->getBgm()->setPitch(1.3f, 100, 0);
		}
	}
}

int TSunMgr::getAddColor() const
{
	int result = 0;
	if (unk14)
		result = gpSunModel->getUnkAC();
	return result;
}

void TSunMgr::drawSyncCallback(u16)
{
	if (unk14)
		gpSunModel->getZBufValue();
}
