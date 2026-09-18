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

void TSunMgr::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);

	// Declared in reverse read order on purpose: named locals grow down from
	// the top of the frame in declaration order, and retail reads the four
	// colour inputs into *ascending* slots (0x8c, 0x90, 0x94, 0x98), so the
	// first value read is the last one declared.
	// TODO: frame 0x70 vs retail 0xb0. The four slots now ascend as retail's
	// do but sit 60 bytes lower, i.e. retail has ~64 bytes of inline-expansion
	// temporaries below them that nothing in this body accounts for, and r30
	// (this) / r31 (the .rodata base) are swapped with ours. Measured:
	// readU32() for the four inputs gives frame 0xa8 but 121 instructions
	// instead of 114 and an extra callee-saved pair; an array and a four-read
	// loop are worse (batch 32).
	u32 local_18;
	u32 local_1c;
	u32 local_20;
	u32 local_24;
	stream >> local_24;
	stream >> local_20;
	stream >> local_1c;
	stream >> local_18;
	stream >> unk20;

	u32 col1 = local_24 << 8 | local_20;
	u32 col2 = local_1c << 8 | local_18;
	unk18.set(col1);
	unk1C.set(col2);

	TSunModel* sun = JDrama::TNameRefGen::search<TSunModel>("太陽モデル");
	if (sun != nullptr) {
		unk14 = 1;
	} else {
		sun = JDrama::TNameRefGen::search<TSunModel>("夕日モデル");
		if (sun != nullptr) {
			unk14 = 1;
			unk15 |= 0x2;
		}
	}

	if (unk14 != 0 && gpMarDirector->getCurrentMap() == 1
	    && TFlagManager::getInstance()->getBool(0x50004)) {
		unk15 |= 0x1;
		TStagePositionInfo* sunWarpPoint
		    = (TStagePositionInfo*)gpPositionHolder->searchF(
		        JDrama::TNameRef::calcKeyCode(cSunWarpPointName),
		        cSunWarpPointName);
		unk24 = sunWarpPoint->unkC;
	}
}

// TODO: frame 0x30 vs retail 0x60; the body is instruction-exact and every r1
// displacement is 48 low with nothing referenced in the local area, so retail
// has 48 bytes of locals here that this body does not name. A single
// uninitialised 48-byte local (e.g. a scratch Mtx) reaches 0x60 and 100% with
// no instruction change, but nothing in the function wants a matrix, so it is
// not committed (docs/catalog/frame-gaps.md: the byte count confirms a size,
// never a declaration). The accessor ladder saturates at 0x40: SMS_GetMarioX/
// SMS_GetMarioZ +8, a const-reference getWarpPos() for unk24 +8, and then +0
// each for SMSGetMarDirector(), a TU-static gpSunModel accessor, a parked
// MSound::unk7C accessor and an isWarpEnabled() predicate (all
// codegen-neutral).
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
	f32 dx = gpMarioPos->x - unk24.x;
	f32 dz = gpMarioPos->z - unk24.z;
	if (dx * dx + dz * dz < 160000.0f && gpSunModel->isInBounds(0.3f)) {
		gpMarDirector->setNextStage(9, nullptr);
		MSound* sound = SMSGetMSound();
		if (sound->unk7C != nullptr) {
			sound->unk7C->setVolume(0.0f, 100, 0);
			sound->unk7C->setPitch(1.3f, 100, 0);
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
