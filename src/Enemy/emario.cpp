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
#include <M3DUtil/MActor.hpp>
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

#include <M3DUtil/M3UModelMario.hpp>

TEMario::TEMario(const char* name)
    : TSpineEnemy(name)
{
}

void TEMario::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);

	f32 tmp1, tmp2;
	stream >> mInitialState >> mStartIndex[0] >> mStartIndex[1]
	    >> mStartIndex[2] >> tmp1 >> tmp2;

	if (mInitialState == 0xFF)
		mInitialState = 0;

	if (mStartIndex[0] == 0xFF)
		mStartIndex[0] = 0;

	if (mStartIndex[1] == 0xFF)
		mStartIndex[1] = 0;

	if (mStartIndex[2] == 0xFF)
		mStartIndex[2] = 0;

	mEnemyMario = new TEnemyMario;
	mEnemyMario->setEMario(this);

	// "Mario 2 P"
	if (strcmp(mName, "マリオ２Ｐ") == 0) {
		mEnemyMario->setGamePad(SMSGetMarDirector()->getGamePad(1));
		mEnemyMario->setPlayerType(TMario::PLAYER_TYPE_P2);
	}

	// "Mario 3 P"
	if (strcmp(mName, "マリオ３Ｐ") == 0) {
		mEnemyMario->setGamePad(SMSGetMarDirector()->getGamePad(2));
		mEnemyMario->setPlayerType(TMario::PLAYER_TYPE_P3);
	}

	// "Mario 4 P
	if (strcmp(mName, "マリオ４Ｐ") == 0) {
		mEnemyMario->setGamePad(SMSGetMarDirector()->getGamePad(3));
		mEnemyMario->setPlayerType(TMario::PLAYER_TYPE_P4);
	}

	// "Mario Character"
	const char marioCharName[] = "マリオ キャラ";
	mEnemyMario->setCharacter(static_cast<JDrama::TCharacter*>(
	    JDrama::TNameRefGen::search(marioCharName)));

	mEnemyMario->initValues();

	mEnemyMario->mPosition = mPosition;
	mEnemyMario->mRotation = mRotation;
	mEnemyMario->mScaling  = mScaling;

	mEnemyMario->mFaceAngle.y = mRotation.y * 0.0027777778f * 65536.0f;

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
		if (TObjChara* chara = (TObjChara*)mCharacter) {
			mMActorKeeper = new TMActorKeeper(nullptr, 1);
			mMActorKeeper->setModelLoaderFlags(
			    J3DMLF_MaterialPEFull | J3DMLF_MaterialUseIndirect
			    | J3DMLF_UseUniqueMaterials | (16 << J3DMLF_TevStageNumShift));
			mMActor = mMActorKeeper->createMActorFromDefaultBmd(
			    chara->getFolder(), 0);
			for (int i = 0;
			     i < getMActor()->getModel()->getModelData()->getMaterialNum();
			     i++) {
				SMS_InitPacket_Fog(getMActor()->getModel(), i);
			}
			getMActor()->setBtk("kagemario_scroll");
		}
		gpConductor->registerAloneActor(this);
	} else {
		mManager      = manager;
		mMActorKeeper = new TMActorKeeper(mManager, 1);
		mManager->manageActor(this);
		mMActor = mMActorKeeper->createMActorFromNthData(0, 0);
	}

	if (getMActor() != nullptr) {
		gpScreenTexture->replace(getMActor()->getModel()->getModelData(),
		                         "H_kagemario_dummy");
	}

	onHitFilter(HIT_FILTER_NO_COLLISION);
	offLiveFlag(LIVE_FLAG_FORCE_SHADOW);

	if (!mAnmSound)
		initAnmSound();

	initHitActor(ACTOR_TYPE_E_MARIO, 0x4,
	             HIT_CATEGORY_PLAYER | HIT_CATEGORY_MAP_OBJECT
	                 | HIT_CATEGORY_ITEM | HIT_CATEGORY_NPC
	                 | HIT_CATEGORY_WATER,
	             70.0f, 45.0f, 60.0f, 40.0f);

	offHitFilter(HIT_FILTER_NO_COLLISION);
	onLiveFlag(LIVE_FLAG_UNK10);
}

void TEMario::checkCollision()
{
	for (s32 i = 0; i < mColCount; ++i) {
		switch (mCollisions[i]->mActorType) {
		case ACTOR_TYPE_MARIO: {
			if (JGeometry::TVec3<f32>(mPosition - mCollisions[i]->getPosition())
			        .length()
			    < mEnemyMario->mAttackRange) {
				mCollisions[i]->receiveMessage(this, HIT_MESSAGE_ATTACK);
			}
		} break;

		case ACTOR_TYPE_GESO_SURF_BOARD: {
			if (!mEnemyMario->checkStatusType(MARIO_STATUS_FLAG_UNK10000)) {
				if (JGeometry::TVec3<f32>(mCollisions[i]->getPosition()
				                          - mPosition)
				        .length()
				    < (mCollisions[i]->getAttackRadius()
				       + mEnemyMario->getDamageRadius())) {
					mEnemyMario->changePlayerStatus(MARIO_STATUS_SURF, 0,
					                                false);
					mEnemyMario->emitGetEffect();
				}
			}
		} break;
		}
	}
}

BOOL TEMario::receiveMessage(THitActor* sender, u32 message)
{
	TSpineEnemy::receiveMessage(sender, message);

	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		mEnemyMario->hitWater(sender);
		return TRUE;
	} else if (message == HIT_MESSAGE_TRAMPLE) {
		return mEnemyMario->thinkTrample();
	} else if (sender->getActorType() == ACTOR_TYPE_MONTE_GOAL_FLAG) {
		mEnemyMario->reachGoal();
	}
	return FALSE;
}

void TEMario::kill()
{
	if (SMS_isMultiPlayerMap())
		gpCamera->removeMultiPlayer(&mPosition);
}

void TEMario::execKill() { }

bool TEMario::isGoal()
{
	return mEnemyMario->checkEMFlag(TEnemyMario::EM_FLAG_GOAL_REACHED);
}

bool TEMario::isReachedToGate() const { return mEnemyMario->isReachedToGate(); }

bool TEMario::isDownWaitingToTalk() const
{
	return mEnemyMario->isDownWaitingToTalk();
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

	if ((cue & CUE_MOVE) && mEnemyMario->canControl()) {
		checkCollision();
	}

	mEnemyMario->perform(cue, graphics);

	if (cue & CUE_MOVE) {
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
