#include <Enemy/RiccoHook.hpp>
#include <Strategic/Spine.hpp>
#include <Enemy/Graph.hpp>
#include <JSystem/JMath.hpp>

MtxPtr THookTake::getTakingMtx() {
    return nullptr;
}

f32 THookTake::getRadiusAtY(f32 y) const {
    return mOwner->getSaveLoadParam()->mSLHangRadius.get();
}

BOOL THookTake::receiveMessage(THitActor* param_1, u32 param_2) {
    if (param_1->mActorType == 0x80000001) {
        if (param_2 == 5) {
            mHeldObject = (TTakeActor*)param_1;
            return TRUE;
        }

        if (param_2 == 7 || param_2 == 8) {
            mHeldObject = nullptr;
            return TRUE;
        }
    }

    return FALSE;
}

// @non-matching - minor stack issue
void THookTake::perform(u32 param_1, JDrama::TGraphics* param_2) {
    if ((param_1 & 1) != 0) {
        mPosition = mOwner->getPosition();
        mPosition.y -= 900.0f;
    }

    THitActor::perform(param_1, param_2);
    
    if ((param_1 & 1) != 0 && mHeldObject != nullptr) {
        JGeometry::TVec3<f32> pos = mHeldObject->getPosition();
        pos += mOwner->mLinearVelocity;
        mHeldObject->moveRequest(pos);
    }
}

TRiccoHook::TRiccoHook(const char* name)
    : TSpineEnemy(name)
    , mHookTake(nullptr)
    , mTimer(0)
{
}

// @non-matching - stack issues
void TRiccoHook::init(TLiveManager* manager) {    
    TSpineEnemy::init(manager);
    mSpine->initWith(&TNerveRHGraphWander::theNerve());
    unk64 |= 1;
    mHookTake = new THookTake(this, "フックつかみ");
    unk124->reset();
    goToShortestNextGraphNode();
    mMarchSpeed = getSaveLoadParam()->mSLMoveSpeed.get();
    mTurnSpeed = 10.0f;
    mLiveFlag |= 0x10;
}

void TRiccoHook::kill() {
}

BOOL TRiccoHook::receiveMessage(THitActor* param_1, u32 param_2) {
    return FALSE;
}

void TRiccoHook::perform(u32 param_1, JDrama::TGraphics* param_2) {
    TSpineEnemy::perform(param_1, param_2);
    mHookTake->perform(param_1, param_2);
    if ((param_1 & 1) && mTimer > 0) {
        mTimer--;
    }
}

TRiccoHookParams::TRiccoHookParams(const char* path)
    : TSpineEnemyParams(path)
    , PARAM_INIT(mSLHitHeight, 900.0f)
    , PARAM_INIT(mSLHitRadius, 120.0f)
    , PARAM_INIT(mSLHangRadius, 30.0f)
    , PARAM_INIT(mSLMoveSpeed, 4.0f)
{
	TParams::load(mPrmPath);
}

TRiccoHookManager::TRiccoHookManager(const char* name)
    : TEnemyManager(name)
{
}

const u8 arr1[12] = { 0 };
const char* msg1 = "メモリが足りません\n"; // 'Not enough memory\n'
const char* msg2 = "MActorMtxCalcType_Basic クラシックスケールＯＮ"; // 'Actor Matrix Calculation Type: Basic (Classic Scale ON)'
const char* msg3 = "MActorMtxCalcType_Softimage クラシックスケールＯＦＦ"; // 'Actor Matrix Calculation Type: Softimage (Classic Scale OFF)'
const char* msg4 = "MActorMtxCalcType_MotionBlend モーションブレンド"; // 'Actor Matrix Calculation Type: Motion Blend'
const char* msg5 = "MActorMtxCalcType_User ユーザー定義"; // 'Actor Matrix Calculation Type: User-Defined'

void TRiccoHookManager::createModelData() {
    static const TModelDataLoadEntry entry = { "riccohook.bmd", 0x10000000, 0 };
    createModelDataArray(&entry);
}

const u8 arr2[12] = { 0 };

void TRiccoHookManager::load(JSUMemoryInputStream& stream) {
    unk38 = new TRiccoHookParams("/enemy/riccohook.prm");
    TEnemyManager::load(stream);
}

TSpineEnemy* TRiccoHookManager::createEnemyInstance() {
    return nullptr;
}

// Same "fake" inline as seen in walkerEnemy.cpp's moveObject func
// Can't find any other way to get the * 1.0f's to emit
static inline JGeometry::TVec3<f32> polarXZ(f32 theta, f32 radius)
{
	f32 c = radius * JMACos(theta);
	f32 s = radius * JMASin(theta);
	return JGeometry::TVec3<f32>(s, 0.0f, c);
}

// @non-matching - stack issues
DEFINE_NERVE(TNerveRHGraphWander, TLiveActor) {
    TRiccoHook* self = (TRiccoHook*)spine->getBody();

    if (spine->getTime() == 0) {
        self->goToDirectedNextGraphNode(polarXZ(self->getRotation().y, 1.0f));
    }

    if ((self->unk104.getPoint() - self->getPosition()).length() < 10.0f) {
        TGraphNode& node = self->unk124->getCurrent();

        if (node.checkFlag(0x800)) {
            self->mTimer = node.getRailNode()->mPitch;
        }

        spine->pushRaw(&TNerveRHGraphWander::theNerve());
        return true;
    } else {
        if (self->unk124->unk0 == nullptr || self->unk124->unk0->isDummy()) {
            return false;
        }

        if (self->mTimer > 0) {
            return false;
        }

        JGeometry::TVec3<f32> dPos = self->getUnkF4().getPoint();
        dPos.sub(self->getPosition());
        PSVECNormalize(&dPos, &dPos);
        dPos.scale(self->mMarchSpeed);
        self->mPosition.add(dPos);
        return false;
    }
}
