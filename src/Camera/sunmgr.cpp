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

#include <System/DummyStrings.hpp>

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

	// TODO: frame-only mismatch (0xa0 bytes vs. 0xb0 in retail).
	u32 local_24[4];
	stream >> local_24[0] >> local_24[1] >> local_24[2] >> local_24[3] >> unk20;

	u32 col1 = local_24[0] << 8 | local_24[1];
	u32 col2 = local_24[2] << 8 | local_24[3];
	unk18.set(col1);
	unk1C.set(col2);

	TSunModel* sun
	    = static_cast<TSunModel*>(JDrama::TNameRefGen::search("太陽モデル"));
	if (sun != nullptr) {
		unk14 = 1;
	} else {
		sun = static_cast<TSunModel*>(
		    JDrama::TNameRefGen::search("夕日モデル"));
		if (sun != nullptr) {
			unk14 = 1;
			unk15 |= 0x2;
		}
	}

	if (unk14 != 0 && SMSGetMarDirector()->getCurrentMap() == 1
	    && TFlagManager::getInstance()->getBool(MSF_NOKI_AVAILABLE)) {
		unk15 |= 0x1;
		TStagePositionInfo* sunWarpPoint
		    = (TStagePositionInfo*)gpPositionHolder->searchF(
		        JDrama::TNameRef::calcKeyCode(cSunWarpPointName),
		        cSunWarpPointName);
		unk24 = sunWarpPoint->getPosition();
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
	JGeometry::TVec2<f32> marioPos(SMS_GetMarioPos().x, SMS_GetMarioPos().z);
	JGeometry::TVec2<f32> warpPos(unk24.x, unk24.z);
	marioPos.sub(marioPos, warpPos);
	if (marioPos.squared() < 160000.0f && gpSunModel->isInBounds(0.3f)) {
		SMSGetMarDirector()->setNextStage(9, nullptr);
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
