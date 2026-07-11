#include <Enemy/EggGen.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <M3DUtil/MActor.hpp>
#include <Player/Mario.hpp>
#include <Player/Yoshi.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <dolphin/mtx.h>

TEggGenerator::TEggGenerator(const char* name)
    : TSpineEnemy(name)
{
	mLiveFlag |= 0x10;
	mLiveFlag |= 8;
}

void TEggGenerator::init(TLiveManager* manager)
{
	// TODO: 99.9% - byte-exact logic; residual is MWCC stack-padding (+8).
	mManager = manager;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("gene_egg_model1.bmd", 0);
	initHitActor(0x02000001, 1, 0x80000000, 10.0f, 10.0f, 10.0f, 10.0f);
	mMActor->setBckFromIndex(0);

	f32 angle = mRotation.x - 90.0f;
	while (angle >= 360.0f)
		angle -= 360.0f;
	while (angle < 0.0f)
		angle += 360.0f;
	mRotation.x = angle;
}

void TEggGenerator::control()
{
	// TODO: 99.8% - byte-exact logic; residual is MWCC stack-padding (+0x18).
	if (PSVECSquareDistance(&mPosition, &gpMarioOriginal->mPosition)
	    < 250000.0f) {
		if (!gpMarioOriginal->mYoshi->isHatched())
			mMActor->setBckFromIndex(0);
	}
}

TEggGenManager::TEggGenManager(const char* name)
    : TEnemyManager(name)
{
}

void TEggGenManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "gene_egg_model1.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (1 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TEggGenManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TSpineEnemyParams("/enemy/egggen.prm");
	TEnemyManager::load(stream);
}
