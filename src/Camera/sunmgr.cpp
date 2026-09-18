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

	// The four colour words go into an array, not four named scalars: with six
	// named scalar locals in the frame MWCC ranks `this` above the compiler's
	// `.rodata` pool base in callee-saved order (r31/r30 swapped against
	// retail on every one of the nineteen uses), and with the array plus the
	// two colour temporaries it ranks the pool base first, as retail does.
	// Research batch 144 measured the whole ranking: pool base first, then
	// locals and parameters in reverse introduction order, and the swap is
	// decided by the count of named scalar locals alone (see
	// docs/catalog/frame-gaps.md, "Research batch 144").
	// The two colour temporaries have to stay named: they are what hoists all
	// four loads above the first store.
	// The 64 bytes of low region are: chaining the five stream reads into one
	// statement (four continuations, +0x20, the batch-81 rule), one level that
	// binds the name-ref search result at both sites (+0x18) and
	// `SMSGetMarDirector()` over the raw global (+8).
	// Each colour is a 24-bit RGB word followed by an alpha word.
	u32 color[4];
	stream >> color[0] >> color[1] >> color[2] >> color[3] >> unk20;

	u32 col1 = color[0] << 8 | color[1];
	u32 col2 = color[2] << 8 | color[3];
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
