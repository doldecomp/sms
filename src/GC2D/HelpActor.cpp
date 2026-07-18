#include <GC2D/HelpActor.hpp>
#include <GC2D/GCConsole2.hpp>
#include <System/MarDirector.hpp>
#include <Strategic/LiveActor.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

THelpActor::THelpActor(const char* name)
    : THitActor(name)
    , unk68(-1)
    , unk6C(nullptr)
    , unk70(nullptr)
    , unk74(0)
{
}

void THelpActor::load(JSUMemoryInputStream& stream)
{
	THitActor::load(stream);
	u32 auStack_c;
	u32 local_10;
	stream >> auStack_c;
	stream >> local_10;
	unk6C = stream.readString();
	initHitActor(0x40000320, 1, -0x80000000, mScaling.x * 100.0f,
	             mScaling.y * 100.0f, 1.0f, 1.0f);
	unk68 = local_10 + 0xE0030;
}

void THelpActor::loadAfter()
{
	THitActor::loadAfter();
	unk70 = JDrama::TNameRefGen::search<TLiveActor>(unk6C);
	JDrama::TNameRefGen::search<TGCConsole2>("GCコンソール")
	    ->entryHelpActor(this);
}

int THelpActor::getHelpID()
{
	if (unk70 != nullptr && check())
		return -1;

	if (mColCount != 0 && getCollision(0)->getActorType() == 0x80000001)
		return unk68;

	return -1;
}

void THelpActor::perform(u32 cue, JDrama::TGraphics*)
{
	if (cue & CUE_MOVE) {
		if (unk74) {
			if (getHelpID() == -1)
				if (SMSGetMarDirector()->getConsole()->startDisappearBalloon(
				        unk68, false))
					unk74 = false;
		} else {
			if (getHelpID() != -1)
				if (SMSGetMarDirector()->getConsole()->startAppearBalloon(
				        unk68, false))
					unk74 = true;
		}
	}
}

bool THelpActor::check() { return unk70->checkLiveFlag(LIVE_FLAG_DEAD); }

TSwitchHelpActor::TSwitchHelpActor(const char* name)
    : THelpActor(name)
{
}

bool TSwitchHelpActor::check()
{
	// I think this is for TMapObjSwitch?
	return ((TMapObjBase*)unk70)->isState(3) ? true : false;
}
