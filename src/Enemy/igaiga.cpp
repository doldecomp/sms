#include <Enemy/Igaiga.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <Map/MapData.hpp>
#include <Player/MarioAccess.hpp>
#include <MSound/MSound.hpp>
#include <Strategic/ObjModel.hpp>

static const char* igaiga_bastable[] = {
	"/scene/igaiga/bas/igaiga_down1.bas",
	"/scene/igaiga/bas/igaiga_down2.bas",
	nullptr,
	nullptr,
	"/scene/igaiga/bas/igaiga_shoot1.bas",
	"/scene/igaiga/bas/igaiga_waterdown1.bas",
	"/scene/igaiga/bas/igaiga_waterhit1.bas",
	nullptr,
};
static const char* gorogoro_bastable[] = {
	nullptr,
	nullptr,
	nullptr,
	nullptr,
};
static TRollEnemy* gpCurRollEnemy;

void TRollEnemy::attackToMario()
{
	SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
}

bool TRollEnemy::isCollidMove(THitActor* actor)
{
	if (actor->isActorType(0x4000022B)) {
		kill();
		return true;
	}

	actor->receiveMessage(this, HIT_MESSAGE_ATTACK);
	return false;
}

void TIgaigaPolluteModelManager::init(TLiveActor* actor)
{
	TEnemyPolluteModelManager::init(actor);

	void* res = JKRFileLoader::getGlbResource(
	    "/scene/igaiga/stamp_igaiga_model1.bmd");
	SDLModelData* modelData
	    = new SDLModelData(J3DModelLoaderDataBase::load(res, 0x10210000));

	for (int i = 0; i < unk14; ++i)
		unk18[i] = new TIgaigaPolluteModel(actor, 0, modelData,
		                                   "イガイガ汚染モデル");
}

void TIgaigaManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "igaiga_model1.bmd", 0x11240000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TIgaiga::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("igaiga_model1.bmd", 0);
}

void TIgaiga::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSmallEnemy::perform(cue, graphics);
}

void TIgaiga::calcRootMatrix()
{
	gpCurRollEnemy = this;
	TSpineEnemy::calcRootMatrix();
}

void TIgaiga::rollSE()
{
	MSound* sound = gpMSound;
	sound->startSoundActorSpecial(MSD_SE_EN_IGAIGA_ROLL, &mPosition, mScaling.x,
	                              mMarchSpeed, 0, nullptr, 0, 4);
}

void TIgaiga::boundSE()
{
	SMSGetMSound()->startSoundActorWithInfo(
	    MSD_SE_EN_IGAIGA_BOUND, &mPosition, nullptr,
	    fabsf(getGroundPlane()->getNormal().y), 0, 0, nullptr, 0, 4);
}

void TIgaiga::setWalkAnm() { setBckAnm(3); }

const char** TIgaiga::getBasNameTable() const { return igaiga_bastable; }

void TGorogoroPolluteModelManager::init(TLiveActor* actor)
{
	TEnemyPolluteModelManager::init(actor);

	void* res = JKRFileLoader::getGlbResource(
	    "/scene/gorogoro/bosspaku_head_stamp.bmd");
	SDLModelData* modelData
	    = new SDLModelData(J3DModelLoaderDataBase::load(res, 0x10210000));

	for (int i = 0; i < unk14; ++i)
		unk18[i] = new TGorogoroPolluteModel(actor, 0, modelData, "汚染モデル");
}

void TGorogoro::rollSE()
{
	SMSGetMSound()->startSoundActorWithInfo(
	    MSD_SE_BS_KRPAKU_ROLL, &mPosition, nullptr,
	    fabsf(getGroundPlane()->getNormal().y), 0, 0, nullptr, 0, 4);
}

void TGorogoro::boundSE()
{
	SMSGetMSound()->startSoundActorWithInfo(
	    MSD_SE_BS_KRPAKU_GND, &mPosition, nullptr,
	    fabsf(getGroundPlane()->getNormal().y), 0, 0, nullptr, 0, 4);
}

const char** TGorogoro::getBasNameTable() const { return gorogoro_bastable; }

void TGorogoro::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("bosspaku_head.bmd", 3);
}
