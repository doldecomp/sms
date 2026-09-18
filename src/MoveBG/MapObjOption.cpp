#include <MoveBG/MapObjOption.hpp>
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
// TMapCollisionBase::setUpTrans' two compound literals are this TU's zero and
// one vectors, and they must be emitted *after* InfectiousStrings' names: with
// this include at the top of the file the pool comes out reversed (one, zero,
// then the names) and the object still scores 100% while the linked DOL loses
// 234 bytes of .rodata order.
#include <Map/MapCollisionEntry.hpp>

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

// The 16 bytes of low region that receiveMessage (0x30) and touchPlayer (0x28)
// used to be short are two inlined bindings inside this body, each worth 8 in
// both callers with zero instruction change:
//   * startStateTimer(120) instead of the raw mStateTimer stores (the real
//     accessor that already exists in MapObjBase.hpp), and
//   * getEffectPos() at the *second* emit only (per site: using it at both
//     makes MWCC CSE the address into a callee-saved register, +2 instructions
//     and a register save).
// Measured alternatives, all +8/+8 and interchangeable with getEffectPos()
// here: a binding fork of gpCardLoad, of SMSRumbleMgr or of
// gpMarioParticleManager, a binding wrapper around emit(), and a TU-local
// static inline taking the block by pointer and binding &block->unk144.
// A 13-16-byte dead non-trivial local in this body (the earlier reading) lands
// the same two frames, but nothing here names such an object.
void TFileLoadBlock::pushed()
{
	startBck("fileloadblock");
	gpCardLoad->setSelected(unk138);
	SMSRumbleMgr->start(0x15, sRumbleTime, (float*)nullptr);
	gpMarioParticleManager->emit(MAP_MAP_MS_M_FILEBLOCK, &unk144, 0, nullptr);
	gpMarioParticleManager->emit(PARTICLE_MS_M_AMIATTACK, getEffectPos(), 0,
	    nullptr);
	startStateTimer(120);
	unk13C->startStateTimer(120);
	unk140->startStateTimer(120);
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
