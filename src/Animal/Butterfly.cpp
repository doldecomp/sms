#include <Animal/Butterfly.hpp>
#include <Animal/boid.hpp>
#include <Enemy/Launcher.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <M3DUtil/MActor.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <System/Particles.hpp>
#include <System/EmitterViewObj.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>

namespace {
const char* const cButterflyMdlNames[]
    = { "butterflyA.bmd", "butterflyB.bmd", "butterflyC.bmd" };
} // namespace

TButterfly::TButterfly(MActor* actor, TButterfloid* butterfloid)
    : TRealoidActor(actor)
{
	unkA8 = butterfloid;
}

void TButterfly::init()
{
	initHitActor(0x10000030, 0, 0, 0.0f, 0.0f, 50.0f, 50.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	onHitFlag(HIT_FLAG_CANNOT_ATTACK);

	TIdxGroupObj* group
	    = JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ");
	group->getChildren().push_back(this);
}

BOOL TButterfly::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_TAKE:
		if (mHolder == nullptr) {
			mHolder = (TTakeActor*)sender;
			gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT, &mPosition, 0,
			                             nullptr);
			return TRUE;
		}
		break;
	case HIT_MESSAGE_UNK8:
		if (mHolder != nullptr) {
			mHolder = nullptr;
			return TRUE;
		}
		break;
	case HIT_MESSAGE_UNKB:
		unkA8->receiveMessageFromChild(this);
		return TRUE;
	}

	return FALSE;
}

TButterfloid::TButterfloid(int type, const char* name)
    : TRealoid(name)
{
	mType     = type;
	mItem     = nullptr;
	mNumEaten = 0;
}

void TButterfloid::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	mSpine->initWith(&TNerveWaitForever<TLiveActor>::theNerve());
	initHitActor(0, 1, 0, 0.0f, 0.0f, 0.0f, 0.0f);
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TButterfloid::initBoids()
{
	for (int i = 0; i < getBoidNum(); ++i)
		getRealoid(i)->init();
}

void TButterfloid::receiveMessageFromChild(TButterfly* child)
{
	child->onFlag(TRealoidActor::FLAG_UNK4);
	child->onHitFlag(HIT_FLAG_NO_COLLISION);

	if (++mNumEaten == getBoidLeader()->getBoidNum()) {
		TMapObjBase* obj = mItem;
		if (obj != nullptr) {
			if (obj->isActorType(0x2000000E))
				obj = gpItemManager->makeObjAppear(0x2000000E);

			if (obj != nullptr) {
				obj->appear();
				obj->mPosition = child->mPosition;
				obj->setVelocityAndFlag10(0.0f, 15.0f, 0.0f);
			}
		}
	}
}

void TButterfloid::load(JSUMemoryInputStream& stream)
{
	loadDefault(stream, cButterflyMdlNames[mType], 0);
	loadItem(stream);

	// TODO: every accessor below is required by the frame, which is 136
	// bytes of low region short with raw member access and instruction
	// identical either way. Measured ladder (from 0x80 towards the ROM's
	// 0x108): the two loops' getRealoid()/getBoidNum() +16 and +24, the
	// leader accessor on the ten writes below +48, the nine TBoidLeader
	// setters +40, getMActor() in the setBck loop +8, and the forwarding
	// TRealoid::getBoidNum() the last +8. A plain u32 read of eventId
	// through readU32() also buys 8 but costs a reload, so it is not the
	// ROM's spelling.
	getBoidLeader()->setNeighborRadius(100.0f);
	getBoidLeader()->setBaseSpeed(6.0f);
	getBoidLeader()->setYawSpeed(25.0f);
	getBoidLeader()->setPitchSpeed(8.0f);
	getBoidLeader()->setMaxPitch(16.0f);
	getBoidLeader()->setAlignmentStrength(0.7f);
	getBoidLeader()->setFleeTarget((THitActor*)gpMarioAddress);
	getBoidLeader()->setFleeRadius(500.0f);
	getBoidLeader()->setFleeStrength(2.0f);
	getBoidLeader()->onFlag(TBoidLeader::FLAG_UNK2);

	for (int i = 0; i < getBoidNum(); ++i)
		getRealoid(i)->getMActor()->setBck("butterfly_fly");

	initBoids();
}

void TButterfloid::loadItem(JSUMemoryInputStream& stream)
{
	u32 eventId;
	stream >> eventId;

	switch (mType) {
	case 0:
		mItem = TMapObjBaseManager::newAndRegisterObj("coin");
		break;
	case 1:
		mItem = TMapObjBaseManager::newAndRegisterObjByEventID(eventId, "");
		break;
	case 2:
		mItem = TMapObjBaseManager::newAndRegisterObj("mushroom1upR");
		break;
	}
}

TButterfly* TButterfloid::createRealoidActor(MActor* actor)
{
	return new TButterfly(actor, this);
}

TButterfloidManager::TButterfloidManager(const char* name)
    : TEnemyManager(name)
{
}

void TButterfloidManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "butterflyA.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (1 << J3DMLF_TevStageNumShift),
		  0 },
		{ "butterflyB.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (1 << J3DMLF_TevStageNumShift),
		  0 },
		{ "butterflyC.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (1 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}
