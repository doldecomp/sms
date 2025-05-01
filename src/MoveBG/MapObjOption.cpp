#include <MoveBG/MapObjOption.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <System/EmitterViewObj.hpp>
#include <GC2D/CardLoad.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static void dummy(Vec* v)
{
	*v = (Vec) { 0.0f, 0.0f, 0.0f };
	*v = (Vec) { 1.0f, 1.0f, 1.0f };
}

void TFileLoadBlock::makeBlockNoCard() { }

void TFileLoadBlock::makeBlockNormal()
{
	startAnim(0);
	mState = 1;
}

void TFileLoadBlock::makeBlockRock()
{
	startAnim(1);
	mState = 2;
}

static int sRumbleTime = 8;

void TFileLoadBlock::pushed()
{
	startBck("fileloadblock");
	gpCardLoad->setSelected(unk138);
	SMSRumbleMgr->start(0x15, sRumbleTime, (float*)nullptr);
	gpMarioParticleManager->emit(0x6E, &unk144, 0, nullptr);
	gpMarioParticleManager->emit(0x39, &unk144, 0, nullptr);
	unk104         = 0x78;
	unk13C->unk104 = 0x78;
	unk140->unk104 = 0x78;
}

void TFileLoadBlock::touchPlayer(THitActor* param_1)
{
	if (isState(1) && marioHeadAttack() && !isUnk104Positive()) {
		pushed();
	}
}

u32 TFileLoadBlock::receiveMessage(THitActor* param_1, u32 param_2)
{

	if (isState(1) && param_2 == 2 && !isUnk104Positive()) {
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

// TODO: move this
extern u8 gParticleFlagLoaded[];
extern JPAResourceManager* gpResourceManager;
inline static void loadParticle(const char* path, u32 id)
{
	if (gParticleFlagLoaded[id] == 0) {
		gpResourceManager->load(path, id);
		gParticleFlagLoaded[id] = 1;
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

	loadParticle("/scene/map/map/ms_m_fileblock.jpa", 0x6e);

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
