#include <MoveBG/MapObjOption.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>
#include <GC2D/CardLoad.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

// TODO: UNUSED at 0x38 in the map, the same size as makeBlockNormal and
// makeBlockRock, so the body is a startAnim/mState pair; which animation index
// and which state value is a guess, continuing those two functions' pattern
// (anim 0 / STATE_NORMAL, anim 1 / STATE_ROCKING).  Nothing calls it, so the
// size is the only evidence.
void TFileLoadBlock::makeBlockNoCard()
{
	startAnim(2);
	mState = STATE_NO_CARD;
}

void TFileLoadBlock::makeBlockNormal()
{
	startAnim(0);
	mState = STATE_NORMAL;
}

void TFileLoadBlock::makeBlockRock()
{
	startAnim(1);
	mState = STATE_ROCKING;
}

static int sRumbleTime = 8;

// TODO: receiveMessage (0x30 vs our 0x20) and touchPlayer (0x28 vs our 0x18)
// are instruction-exact and both want exactly one dead 13-16-byte non-trivial
// local *here*: pushed() is UNUSED (0x9c), so an uninitialised class local
// emits nothing and keeps that size, and it is the only inlined callee the two
// functions share that is not already pinned by a matching out-of-line copy.
// Measured (struct with an empty dtor, zero instruction change):
//   12 bytes -> touchPlayer 0x28 exact, receiveMessage 0x28 (needs 0x30)
//   13-16    -> both exact
//   20       -> receiveMessage exact, touchPlayer 0x30 (over)
//   24       -> both over
// So two independent frames pin the size at 13-16 bytes, but nothing in the
// body names the type, so it is left undeclared rather than fabricated.
void TFileLoadBlock::pushed()
{
	startBck("fileloadblock");
	gpCardLoad->setSelected(unk138);
	SMSRumbleMgr->start(0x15, sRumbleTime, (float*)nullptr);
	gpMarioParticleManager->emit(MAP_MAP_MS_M_FILEBLOCK, &unk144, 0, nullptr);
	gpMarioParticleManager->emit(PARTICLE_MS_M_AMIATTACK, &unk144, 0, nullptr);
	mStateTimer         = 120;
	unk13C->mStateTimer = 120;
	unk140->mStateTimer = 120;
}

void TFileLoadBlock::touchPlayer(THitActor* param_1)
{
	if (isState(STATE_NORMAL) && marioHeadAttack() && !isStateTimerEngaged())
		pushed();
}

BOOL TFileLoadBlock::receiveMessage(THitActor* sender, u32 message)
{
	if (isState(STATE_NORMAL) && message == HIT_MESSAGE_PUSH_UP
	    && !isStateTimerEngaged()) {
		pushed();
		return true;
	}

	return false;
}

void TFileLoadBlock::loadAfter()
{
	TMapObjBase::loadAfter();

	if (unk138 == 0) {
		unk13C
		    = JDrama::TNameRefGen::search<TFileLoadBlock>("ロードブロックＢ");
		unk140
		    = JDrama::TNameRefGen::search<TFileLoadBlock>("ロードブロックＣ");
	} else if (unk138 == 1) {
		unk13C
		    = JDrama::TNameRefGen::search<TFileLoadBlock>("ロードブロックＡ");
		unk140
		    = JDrama::TNameRefGen::search<TFileLoadBlock>("ロードブロックＣ");
	} else {
		unk13C
		    = JDrama::TNameRefGen::search<TFileLoadBlock>("ロードブロックＡ");
		unk140
		    = JDrama::TNameRefGen::search<TFileLoadBlock>("ロードブロックＢ");
	}
}

void TFileLoadBlock::initMapObj()
{
	TMapObjBase::initMapObj();
	if (strcmp("FileLoadBlockA", getUnkF4()) == 0)
		unk138 = 0;
	else if (strcmp("FileLoadBlockB", getUnkF4()) == 0)
		unk138 = 1;
	else if (strcmp("FileLoadBlockC", getUnkF4()) == 0)
		unk138 = 2;

	SMS_LoadParticle("/scene/map/map/ms_m_fileblock.jpa", 0x6E);

	unk144.set(mPosition.x, mPosition.y, mPosition.z);
}

TFileLoadBlock::TFileLoadBlock(const char* name)
    : TMapObjBase(name)
    , unk138(0)
    , unk13C(nullptr)
    , unk140(nullptr)
{
	unk144.x = unk144.y = unk144.z = 0.0f;
}

void TMapObjOptionWall::onCollision() { unk68->setUp(); }

void TMapObjOptionWall::offCollision() { unk68->remove(); }

void TMapObjOptionWall::init()
{
	unk68 = new TMapCollisionWarp;
	unk68->init("/scene/map/map/option_wall.col", 0, nullptr);
}

TMapObjOptionWall::TMapObjOptionWall(const char* name)
    : THitActor(name)
    , unk68(nullptr)
{
}
