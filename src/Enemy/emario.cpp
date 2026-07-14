#include <Enemy/Conductor.hpp>
#include <Enemy/Emario.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/EnemyMario.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <Player/Mario.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/ObjModel.hpp>
#include <System/MarDirector.hpp>
#include <System/StageUtil.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TEMario::TEMario(const char* name)
    : TSpineEnemy(name)
{
}

void TEMario::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);

	stream.read(&unk154, 4);
	stream.read(&unk158, 4);
	stream.read(&unk15C, 4);
	stream.read(&unk160, 4);

	stream.readU32();
	stream.readU32();

	if (unk154 == 0xff) {
		unk154 = 0;
	}

	if (unk158 == 0xFF) {
		unk158 = 0;
	}

	if (unk15C == 0xFF) {
		unk15C = 0;
	}

	if (unk160 == 0xFF) {
		unk160 = 0;
	}

	mEnemyMario          = new TEnemyMario();
	mEnemyMario->mEMario = this;

	// "Mario 2 P"
	if (strcmp(mName, "マリオ２Ｐ") == 0) {
		mEnemyMario->setGamePad(gpMarDirector->unk18[1]);
		mEnemyMario->unk388 = 3;
	}

	// "Mario 3 P"
	if (strcmp(mName, "マリオ３Ｐ") == 0) {
		mEnemyMario->setGamePad(gpMarDirector->unk18[2]);
		mEnemyMario->unk388 = 4;
	}

	// "Mario 4 P
	if (strcmp(mName, "マリオ４Ｐ") == 0) {
		mEnemyMario->setGamePad(gpMarDirector->unk18[3]);
		mEnemyMario->unk388 = 5;
	}

	TNameRef* rootNameRef
	    = JDrama::TNameRefGen::getInstance()->getRootNameRef();

	// "Mario Character"
	const char marioCharName[] = "マリオ キャラ";
	mEnemyMario->unk3C
	    = JDrama::TNameRefGen::search<JDrama::TCharacter>(marioCharName);

	mEnemyMario->initValues();

	mEnemyMario->mPosition = getPosition();
	mEnemyMario->mRotation = getRotation();
	mEnemyMario->mScaling  = getScaling();

	mEnemyMario->mFaceAngle.y = getRotation().y * 0.0027777778f * 65536.0f;

	mEnemyMario->initEnemyValues();

	mEnemyMario->onFlag(MARIO_FLAG_NPC_TALKING);
}

void TEMario::loadAfter()
{
	mEnemyMario->loadAfter();
	if (SMS_isMultiPlayerMap()) {
		gpCamera->addMultiPlayer(&mPosition, 60.0f, 150.0f);
	}
}
void TEMario::init(TLiveManager* manager)
{
	if (!manager) {
		if (TObjChara* chara = (TObjChara*)unk3C) {
			mMActorKeeper                    = new TMActorKeeper(nullptr, 1);
			mMActorKeeper->mModelLoaderFlags = 0x11300000;
			mMActor = mMActorKeeper->createMActorFromDefaultBmd(
			    chara->getFolder(), 0);
			for (int i = 0; i < mMActor->unk4->mModelData->mMaterialNum; i++) {
				SMS_InitPacket_Fog(mMActor->unk4, i);
			}
			mMActor->setBtk("kagemario_scroll");
		}
		gpConductor->registerAloneActor(this);
	} else {
		mManager      = manager;
		mMActorKeeper = new TMActorKeeper(mManager, 1);
		mManager->manageActor(this);
		mMActor = mMActorKeeper->createMActorFromNthData(0, 0);
	}

	if (mMActor != nullptr) {
		gpScreenTexture->replace(mMActor->getModel()->getModelData(),
		                         "H_kagemario_dummy");
	}

	onHitFlag(HIT_FLAG_NO_COLLISION);
	offLiveFlag(LIVE_FLAG_UNK400);

	if (!mAnmSound)
		initAnmSound();

	initHitActor(0x8000002, 0x4, 0xe5000000, 70.0f, 45.0f, 60.0f, 40.0f);

	offHitFlag(HIT_FLAG_NO_COLLISION);
	onLiveFlag(LIVE_FLAG_UNK10);
}

BOOL TEMario::receiveMessage(THitActor* sender, u32 message)
{
	TSpineEnemy::receiveMessage(sender, message);

	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		mEnemyMario->hitWater(sender);
		return TRUE;
	} else if (message == HIT_MESSAGE_TRAMPLE) {
		return mEnemyMario->thinkTrample();
	} else if (sender->getActorType() == 0x40000246) {
		mEnemyMario->reachGoal();
	}
	return FALSE;
}

void TEMario::kill()
{
	if (SMS_isMultiPlayerMap()) {
		gpCamera->removeMultiPlayer(&mPosition);
	}
}

BOOL TEMario::isGoal()
{
	if (mEnemyMario->unk4290 & 1) {
		return TRUE;
	}
	return FALSE;
}

BOOL TEMario::isReachedToGate() const
{
	if (mEnemyMario->unk4292 == 0x18) {
		return TRUE;
	}
	return FALSE;
}

BOOL TEMario::isDownWaitingToTalk() const
{
	if (mEnemyMario->unk4292 == 0xf) {
		return TRUE;
	}
	return FALSE;
}

void TEMario::startRunAway() { mEnemyMario->startRunAway(); }

void TEMario::startMonteReplay(u32 param1)
{
	mEnemyMario->startMonteReplay(param1);
}

void TEMario::startGateDrawing() { mEnemyMario->startGateDrawing(); }

void TEMario::forceDisappear() { mEnemyMario->startDisappear(9); }

void TEMario::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (checkLiveFlag(LIVE_FLAG_UNK40)) {
		return;
	}

	if (!(cue & CUE_MOVE)) {
		return;
	}

	if (mEnemyMario->checkUnk4292() == 0) {
		return;
	}

	for (s32 i = 0; i < mColCount; ++i) {
		switch (mCollisions[i]->mActorType) {
		case 0x80000001: {
			const f32 d = vecDist(mPosition, mCollisions[i]->getPosition());
			if (d < mEnemyMario->unk42B0) {
				mCollisions[i]->receiveMessage(this, HIT_MESSAGE_ATTACK);
			}
		} break;

		case 0x400000bc: {
			if (!mEnemyMario->checkStatusType(0x10000)) {
				const f32 dmgRadius = mEnemyMario->getDamageRadius();
				const f32 atkRadius = mCollisions[i]->getAttackRadius();
				const f32 d = vecDist(mCollisions[i]->getPosition(), mPosition);

				if (d < (dmgRadius + atkRadius)) {
					mEnemyMario->changePlayerStatus(0x810446, 0, false);
					mEnemyMario->emitGetEffect();
				}
			}
		} break;
		}
	}

	mEnemyMario->perform(cue, graphics);

	if (cue & 0x1) {
		mPosition = mEnemyMario->getPosition();
		mRotation = mEnemyMario->getRotation();
		mScaling  = mEnemyMario->getScaling();

		setAttackRadius(mEnemyMario->getAttackRadius());
		setAttackHeight(mEnemyMario->getAttackHeight());
		setDamageRadius(mEnemyMario->getDamageRadius());
		setDamageHeight(mEnemyMario->getDamageHeight());
	}
}

TEMarioManager::TEMarioManager(const char* name)
    : TEnemyManager(name)
{
}

void TEMarioManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TSpineEnemyParams("/enemy/emario.prm");
	TEnemyManager::load(stream);
}

TSpineEnemy* TEMarioManager::createEnemyInstance()
{
	// "Pseudo Mario"
	return new TEMario("マリオモドキ");
}
