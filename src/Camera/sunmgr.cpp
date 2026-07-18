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

	u32 local_24;
	stream >> local_24;
	u32 local_20;
	stream >> local_20;
	u32 local_1c;
	stream >> local_1c;
	u32 local_18;
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
