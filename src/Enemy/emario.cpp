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

static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

static const char* MtxCalcTypeName[] = {
	"MActorMtxCalcType_Basic クラシックスケールＯＮ",
	"MActorMtxCalcType_Softimage クラシックスケールＯＦＦ",
	"MActorMtxCalcType_MotionBlend モーションブレンド",
	"MActorMtxCalcType_User ユーザー定義",
};

#include <Player/MarioDirtyStrings.hpp>

TEMario::TEMario(const char* name)
    : TSpineEnemy(name)
{
}

// TODO: 99.9%. Instruction-exact; frame 0xf0 vs 0xa8 and every stream-read
// slot is 92 bytes higher in retail, so ~72 dead bytes sit below them and the
// rest above.
void TEMario::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);

	stream >> mInitialState >> unk158 >> unk15C >> unk160;

	stream.readU32();
	stream.readU32();

	if (mInitialState == 0xFF)
		mInitialState = 0;

	if (unk158 == 0xFF)
		unk158 = 0;

	if (unk15C == 0xFF)
		unk15C = 0;

	if (unk160 == 0xFF)
		unk160 = 0;

	mEnemyMario          = new TEnemyMario;
	mEnemyMario->mEMario = this;

	// "Mario 2 P"
	if (strcmp(mName, "マリオ２Ｐ") == 0) {
		mEnemyMario->setGamePad(gpMarDirector->unk18[1]);
		mEnemyMario->mPlayerType = TMario::PLAYER_TYPE_P2;
	}

	// "Mario 3 P"
	if (strcmp(mName, "マリオ３Ｐ") == 0) {
		mEnemyMario->setGamePad(gpMarDirector->unk18[2]);
		mEnemyMario->mPlayerType = TMario::PLAYER_TYPE_P3;
	}

	// "Mario 4 P
	if (strcmp(mName, "マリオ４Ｐ") == 0) {
		mEnemyMario->setGamePad(gpMarDirector->unk18[3]);
		mEnemyMario->mPlayerType = TMario::PLAYER_TYPE_P4;
	}

	// "Mario Character"
	const char marioCharName[] = "マリオ キャラ";
	mEnemyMario->setCharacter(
	    JDrama::TNameRefGen::search<JDrama::TCharacter>(marioCharName));

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
// TODO: 99.9%. Instruction-exact since the fog loop's body was respelled
// `getMActor()->getModel()` while the loop bound keeps `mMActor->getModel()`
// (closure batch 83, 97.8 -> 99.9): two different receiver expressions stop
// MWCC CSE-ing the J3DModel, so retail re-derives it with `lwz r3, 4(r3)` from
// the cached mMActor and leaves mMActor itself in r3 for the setBtk call.
// Residue: frame 0x88 vs 0x60, 40 dead bytes, no referenced local at all --
// a pure dead-low-region gap, so the only zero-instruction lever for it is an
// uninitialised non-trivial class local inside one of the inlined callees
// (TMActorKeeper's ctor / createMActorFromDefaultBmd / SMS_InitPacket_Fog /
// gpScreenTexture->replace). emario's own map entry has no UNUSED helper to
// carry it, so every candidate is in a shared header: parked, not guessed.
void TEMario::init(TLiveManager* manager)
{
	if (!manager) {
		if (TObjChara* chara = (TObjChara*)unk3C) {
			mMActorKeeper                    = new TMActorKeeper(nullptr, 1);
			mMActorKeeper->mModelLoaderFlags = 0x11300000;
			mMActor = mMActorKeeper->createMActorFromDefaultBmd(
			    chara->getFolder(), 0);
			for (int i = 0;
			     i < mMActor->getModel()->getModelData()->getMaterialNum();
			     i++) {
				SMS_InitPacket_Fog(getMActor()->getModel(), i);
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

// Parked here, not in Emario.hpp: the map has no symbol for it, so retail had
// it as a file-scope `inline` (or a header inline that expanded everywhere).
// It is the level that keeps JGeometry::TUtil<f32>::sqrt a `bl` at both of
// perform's distance tests -- `TVec3::distance()` gives the call but expands
// the subtraction, and a `diff.length()` written at the call site gives
// retail's `bl TVec3::sub` but expands sqrt (closure batch 83:
// TEMario::perform 71.9 -> 77.9 -> 98.1). This is the "second helper such as
// AnimalNerve's calcDist" that docs/catalog/frame-gaps.md predicts for the
// copy-and-subtract distance sites.
static inline f32 EMarioCalcDist(const JGeometry::TVec3<f32>& a,
                                 const JGeometry::TVec3<f32>& b)
{
	JGeometry::TVec3<f32> diff = a - b;
	return diff.length();
}

// UNUSED (map 0x220 = 544). Dead code, not an inline: perform() spells the
// same loop itself, and rewriting perform() as `checkCollision()` drops it
// 98.1 -> 87.9 with the same 752 bytes, so retail's perform() never called
// this. It reads as a self-contained entry point that was superseded, which is
// what the size says too: the loop alone compiles to 472, plus the canControl
// guard 524, plus the live-flag guard 536 of 544.
// TODO: 8 bytes (2 instructions) short. A third guard of the `cmpwi`/`beq`
// shape would land it (a null test on mEnemyMario, an `mColCount == 0`
// early-out), but nothing in the TU is evidence for which, and since no
// emitted function inlines this body its exact form constrains no match.
void TEMario::checkCollision()
{
	if (checkLiveFlag(LIVE_FLAG_UNK40))
		return;

	if (mEnemyMario->canControl() == 0)
		return;

	for (s32 i = 0; i < mColCount; ++i) {
		switch (mCollisions[i]->mActorType) {
		case 0x80000001: {
			if (EMarioCalcDist(mPosition, mCollisions[i]->getPosition())
			    < mEnemyMario->mAttackRange) {
				mCollisions[i]->receiveMessage(this, HIT_MESSAGE_ATTACK);
			}
		} break;

		case 0x400000bc: {
			if (!mEnemyMario->checkStatusType(0x10000)) {
				if (EMarioCalcDist(mCollisions[i]->getPosition(), mPosition)
				    < (mCollisions[i]->getAttackRadius()
				       + mEnemyMario->getDamageRadius())) {
					mEnemyMario->changePlayerStatus(0x810446, 0, false);
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
	} else if (sender->getActorType() == 0x40000246) {
		mEnemyMario->reachGoal();
	}
	return FALSE;
}

void TEMario::kill()
{
	if (SMS_isMultiPlayerMap())
		gpCamera->removeMultiPlayer(&mPosition);
}

// UNUSED (map 0x54). Never called from any emitted function in this TU and no
// inlined site reproduces it, so the body is unknown; left empty.
// TODO: 0x54 of body. Probably the multi-player teardown kill() performs plus
// whatever the enemy-side shutdown was (mEnemyMario->...), but there is no
// evidence for the statements, so nothing is guessed here.
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

// TODO: 98.1%. What is left: retail initialises the strength-reduced byte
// offset from the loop counter (`li r29, 0` then `addi r24, r29, 0`) where we
// emit two independent `li`s, which also swaps r28/r29 between the counter and
// the `cue & CUE_MOVE` bool, and the frame is 0x148 to retail's 0x130 -- two
// 12-byte objects too many. Retail's four referenced vectors are grouped
// differently too (diff2 0xec, diff1 0xe0 adjacent at the top, then the two
// operator- temporaries at 0xb4 and 0x9c with holes between), while ours
// interleaves them (temp1 0xfc, diff1 0xf0, temp2 0xdc, diff2 0xd0). Spelling
// the helper's body `TUtil<f32>::sqrt(diff.squared())` instead of
// `diff.length()` is codegen-identical, so the extra pair is most likely the
// `const TVec3&` binding of squared()/dot() per expansion.
// Closure batch: the slot geometry is retail low region 0xc..0x9c against ours
// 0xc..0xd0 (52 bytes more) with the *named* block larger in retail
// (0x9c..0xfc against 0xd0..0x114) -- the same intra-statement allocation-order
// difference frame-gaps.md batch 142 records for `a = b - c` / `bl TVec3::sub`,
// a known-open class. The counter/bool register swap is not block scope:
// declaring `s32 i;` at function scope and writing `for (i = 0; ...)` is
// byte-identical, which confirms "block scope is inert everywhere", so
// retail's r29 = i above r28 = the `cue & CUE_MOVE` bool has another cause.
void TEMario::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (checkLiveFlag(LIVE_FLAG_UNK40)) {
		return;
	}

	if ((cue & CUE_MOVE) && mEnemyMario->canControl() != 0) {
		for (s32 i = 0; i < mColCount; ++i) {
			switch (mCollisions[i]->mActorType) {
			case 0x80000001: {
				if (EMarioCalcDist(mPosition, mCollisions[i]->getPosition())
				    < mEnemyMario->mAttackRange) {
					mCollisions[i]->receiveMessage(this, HIT_MESSAGE_ATTACK);
				}
			} break;

			case 0x400000bc: {
				if (!mEnemyMario->checkStatusType(0x10000)) {
					if (EMarioCalcDist(mCollisions[i]->getPosition(),
					                   mPosition)
					    < (mCollisions[i]->getAttackRadius()
					       + mEnemyMario->getDamageRadius())) {
						mEnemyMario->changePlayerStatus(0x810446, 0, false);
						mEnemyMario->emitGetEffect();
					}
				}
			} break;
			}
		}
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
