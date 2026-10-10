#include "JSystem/JGeometry/JGVec3.hpp"
#include <dolphin/types.h>
#include <Player/Tongue.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <MarioUtil/ModelUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MSound/MSound.hpp>
#include <Strategic/MirrorActor.hpp>
#include <Map/Map.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <System/DummyStrings.hpp>
static const char cDirtyFileName[] = "/scene/map/pollution/H_ma_rak.bti";
static const char cDirtyTexName[]  = "H_ma_rak_dummy";

// TODO: fabricated boundary; keeps the first scale call out of line.
static inline JGeometry::TVec3<f32>& scaleVector(JGeometry::TVec3<f32>& vector,
                                                 f32 scale)
{
	vector *= scale;
	return vector;
}

void TYoshiTongue::init(TYoshi* yoshi)
{
	J3DModelData* modelData = J3DModelLoaderDataBase::load(
	    JKRGetResource("/mario/bmd/yoshi_tongue.bmd"),
	    J3DMLF_MaterialPEFull | (4 << J3DMLF_TevStageNumShift));

	mYoshi = yoshi;
	mModel = new J3DModel(modelData, 0x10000, 1);

	mModel->getModelData()->onFlag1OnAllShapes();

	mTipModel = new J3DModel(
	    J3DModelLoaderDataBase::load(
	        JKRGetResource("/mario/bmd/yoshi_tongue_tip.bmd"),
	        J3DMLF_MaterialPEFull | (4 << J3DMLF_TevStageNumShift)),
	    0x10000, 1);

	mTipModel->getModelData()->onFlag1OnAllShapes();

	mState       = STATE_IDLE;
	mProgress    = 0;
	mMaxProgress = 60;
	unk82        = 600;

	mInitialSpeed    = 10.0f;
	mExtendAmount    = 0.05f;
	mRetractAmount   = 0.7f;
	mPullAmount      = 0.01f;
	mRetractedLength = 80.0f;
	mMaxReach        = 500.0f + mRetractedLength;
	mElasticity      = 0.8f;

	mHeadPos.set(0.0f, 0.0f, 0.0f);
	mTipPos.set(0.0f, 0.0f, 0.0f);
	mInitialVelocity.set(0.0f, 0.0f, 0.0f);

	mActorTypeInMouth = 0;
	unkD4             = 0;

	initHitActor(ACTOR_TYPE_YOSHI_TONGUE, 5U,
	             HIT_CATEGORY_MAP_OBJECT | HIT_CATEGORY_ITEM
	                 | HIT_CATEGORY_ENEMY,
	             1000.0f, 500.0f, 50.0f, 500.0f);
	offHitFilter(HIT_FILTER_NO_COLLISION);
}

void TYoshiTongue::initInLoadAfter()
{
	JDrama::TViewObjPtrListT<JDrama::TViewObj>* grp
	    = static_cast<JDrama::TViewObjPtrListT<JDrama::TViewObj>*>(
	        JDrama::TNameRefGen::search("敵グループ"));
	grp->getChildren().push_back(this);

	TMirrorActor* ma = new TMirrorActor("ヨッシー舌in鏡");
	ma->init(mModel, 4);

	TMirrorActor* mb = new TMirrorActor("ヨッシー舌先in鏡");
	mb->init(mTipModel, 4);
}

void TYoshiTongue::emit(const JGeometry::TVec3<f32>& src,
                        const JGeometry::TVec3<f32>& dir,
                        const JGeometry::TVec3<f32>& vel)
{
	if (mState == STATE_IDLE) {
		mProgress = 0;
		mState    = STATE_EXTENDING;

		mTipPos     = src;
		mPosition   = src;
		mPosition.y = -((0.5f * mAttackHeight) - mPosition.y);

		mHeadPos = src;
		mHeadDir = dir;

		// TODO: frame-only mismatch (target 0x60, current 0x50); no padding.
		f32 initialSpeed                 = mInitialSpeed;
		JGeometry::TVec3<f32> initialDir = dir;
		mInitialVelocity = scaleVector(initialDir, initialSpeed);
		mInitialVelocity += vel * 0.5f;

		if (mInitialVelocity.y < -50.0f)
			mInitialVelocity.y = -50.0f;
		if (mInitialVelocity.y > 50.0f)
			mInitialVelocity.y = 50.0f;
	}
}

void TYoshiTongue::rest(const JGeometry::TVec3<f32>& a,
                        const JGeometry::TVec3<f32>& b)
{
}

// TODO: fakematch; without this boundary, canGo is inlined into movement.
#pragma dont_inline on
BOOL TYoshiTongue::canGo()
{
	// TODO: stack-only mismatch; subtraction temporary is at 0x20, not 0x14.
	JGeometry::TVec3<f32> toTip = mTipPos - mHeadPos;
	f32 dot                     = toTip.dot(mHeadDir);

	if (dot < 0.0f)
		return false;

	if ((int)gpMap->isTouchedOneWallAndMoveXZ(&mTipPos.x, 10.0f + mTipPos.y,
	                                          &mTipPos.z, 50.0f)
	    > 0)
		return false;

	const TBGCheckData* ground;
	f32 groundY = gpMap->checkGround(mTipPos.x, mTipPos.y, mTipPos.z, &ground);
	if (50.0f + groundY > mTipPos.y) {
		mTipPos.y = 50.0f + groundY;
		return true;
	}

	const TBGCheckData* roof;
	f32 roofY
	    = gpMap->checkRoof(mTipPos.x, mTipPos.y, mTipPos.z, &roof) - 50.0f;
	if (roofY < mTipPos.y) {
		mTipPos.y = roofY;
		return true;
	}

	return true;
}
// TODO: end the canGo out-of-line boundary above.
#pragma dont_inline off

THitActor* TYoshiTongue::findTarget(bool allowExtra, bool checkForward)
{
	THitActor* best = nullptr;
	f32 bestDist    = 10000.0f;

	for (s32 i = 0; i < getColNum(); ++i) {
		s32 type = getCollision(i)->getActorType();
		if (type == ACTOR_TYPE_SEAL || type == ACTOR_TYPE_BASKET_REVERSE) {
			mState = STATE_RETRACTING;
			return nullptr;
		}
	}

	for (s32 i = 0; i < getColNum(); ++i) {
		THitActor* actor = getCollision(i);
		s32 type         = actor->getActorType();
		int ok           = 0;

		if (type == ACTOR_TYPE_FRUIT_COCONUT)
			ok = 1;
		if (type == ACTOR_TYPE_FRUIT_PAPAYA)
			ok = 1;
		if (type == ACTOR_TYPE_FRUIT_PINE)
			ok = 1;
		if (type == ACTOR_TYPE_FRUIT_DURIAN)
			ok = 1;
		if (type == ACTOR_TYPE_FRUIT_BANANA)
			ok = 1;
		if (type == ACTOR_TYPE_RED_PEPPER)
			ok = 1;
		if (type == ACTOR_TYPE_FRUIT_COVER_PINE)
			ok = 1;

		if (allowExtra == true) {
			if (type == ACTOR_TYPE_BOTTLE_SHORT)
				ok = 1;
			if (type == ACTOR_TYPE_BOTTLE_LARGE)
				ok = 1;
			if (type == ACTOR_TYPE_WOOD_BARREL)
				ok = 1;
			if (type & HIT_CATEGORY_ENEMY ? true : false)
				ok = 1;
		}

		if (ok != 1)
			continue;

		JGeometry::TVec3<f32> targetPos = actor->mPosition;
		targetPos.y += 0.5f * actor->getDamageHeight();
		JGeometry::TVec3<f32> delta = targetPos - mTipPos;

		bool isZero = delta.isZero();
		if (isZero)
			continue;

		// TODO: squared() is recomputed across the length() inline;
		// the target reuses it. Vector stack homes also still differ.
		f32 dist = delta.length();
		delta.normalize();

		if (checkForward && !(delta.dot(mHeadDir) > 0.5f))
			continue;

		if (dist < bestDist) {
			bestDist = dist;
			best     = getCollision(i);
		}
	}

	return best;
}

void TYoshiTongue::movement()
{
	// TODO: vector return-value copies, length inlines, and stack homes differ.
	if (unkD4 != 0) {
		mColCount = mSavedColCount;
		unkD4--;
	} else if (mColCount != 0) {
		mSavedColCount = mColCount;
		unkD4          = 3;
	}

	switch (mState) {
	case STATE_IDLE:
	case STATE_GRABBED:
	default:
		mAttackRadius = 300.0f;
		calcEntryRadius();
		offHitFilter(HIT_FILTER_NO_ATTACK);
		break;

	case STATE_EXTENDING:
		mAttackRadius = 1000.0f;
		calcEntryRadius();
		mProgress += 1;

		if (mProgress >= mMaxProgress)
			mState = STATE_RETRACTING;
		else if (!canGo())
			mState = STATE_RETRACTING;
		break;

	case STATE_RETRACTING: {
		mAttackRadius = 10.0f;
		calcEntryRadius();

		JGeometry::TVec3<f32> diff = mTipPos - mHeadPos;

		if (mHeldObject != nullptr) {
			// TODO: expanded length; scalar inputs preserve the target FMA.
			// movement's @2843 table addends depend on this contraction.
			f32 x = diff.x;
			f32 y = diff.y;
			f32 z = diff.z;
			if (JGeometry::TUtil<f32>::sqrt(x * x + y * y + z * z)
			    < mMaxReach) {
				JGeometry::TVec3<f32>& heldScale = mHeldObject->mScaling;
				f32 elasticity                   = mElasticity;
				JGeometry::TVec3<f32> scale      = heldScale;
				scale *= elasticity;
				if (scale.x < 0.01f)
					scale.set(0.01f, 0.01f, 0.01f);
				heldScale = scale;
			}
		}

		// TODO: expanded length; retain the target contraction as above.
		f32 x = diff.x;
		f32 y = diff.y;
		f32 z = diff.z;
		if (JGeometry::TUtil<f32>::sqrt(x * x + y * y + z * z)
		    < mRetractedLength) {
			if (mHeldObject != nullptr) {
				mActorTypeInMouth = mHeldObject->mActorType;
				mHeldObject->receiveMessage(this, HIT_MESSAGE_DETACH);
				mHeldObject->receiveMessage(this, HIT_MESSAGE_UNKB);
				mHeldObject = nullptr;
			}
			mState = STATE_IDLE;
		}
		break;
	}

	case STATE_UNK5:
		mAttackRadius = 10.0f;
		calcEntryRadius();
		mProgress++;
		if (mProgress >= unk82)
			mState = STATE_RETRACTING;
		break;

	case STATE_PULLING:
	case STATE_PULLING_SLOW:
		mProgress++;
		break;
	}

	switch (mState) {
	default:
		mTipPos = mHeadPos;
		break;

	case STATE_EXTENDING: {
		THitActor* target = findTarget(true, true);
		if (target != nullptr && mHeldObject == nullptr) {
			JGeometry::TVec3<f32> tpos = target->mPosition;
			tpos.y += 0.5f * target->mDamageHeight;
			JGeometry::TVec3<f32> step = mTipPos;
			mTipPos += (tpos - mTipPos) * mExtendAmount;
			mInitialVelocity = mTipPos - step;

			if (JGeometry::TVec3<f32>(tpos - mTipPos).length() < 200.0f
			    && target->receiveMessage(this, HIT_MESSAGE_TAKE) == true) {
				mHeldObject = (TTakeActor*)target;
				SMSGetMSound()->startSoundActor(MSD_SE_YV_PERON, &mTipPos, 0,
				                                nullptr, 0, 4);
				mProgress = 0;
				mState    = STATE_GRABBED;
			}
		} else {
			mTipPos += mInitialVelocity;
		}
		break;
	}

	case STATE_GRABBED:
		mProgress += 1;
		if (mProgress > 10)
			mState = STATE_RETRACTING;
		break;

	case STATE_RETRACTING:
		mTipPos = mHeadPos + (mTipPos - mHeadPos) * mRetractAmount;
		break;

	case STATE_PULLING:
	case STATE_PULLING_SLOW: {
		JGeometry::TVec3<f32> diff = mTipPos - mHeadPos;
		f32 len                    = diff.length();

		MtxPtr mtx = getTakingMtx();
		JGeometry::TVec3<f32> step;
		step.x = -mtx[0][0];
		step.y = -mtx[1][0];
		step.z = -mtx[2][0];

		f32 amount = len * mPullAmount;
		if (mState == STATE_PULLING_SLOW)
			amount *= 0.5f;
		step *= amount;

		JGeometry::TVec3<f32> next = mTipPos + step;
		if (mHolder->moveRequest(next) == true)
			mTipPos += step;
		break;
	}
	}

	checkTaking();
	mPosition   = mTipPos;
	mPosition.y = -((0.5f * mAttackHeight) - mPosition.y);
}

void TYoshiTongue::calcAnim(MtxPtr mtx)
{
	// TODO: cross() scalar temporaries change FPR allocation.
	mHeadPos.x = mtx[0][3];
	mHeadPos.y = mtx[1][3];
	mHeadPos.z = mtx[2][3];
	mHeadDir.x = mtx[0][0];
	mHeadDir.y = mtx[1][0];
	mHeadDir.z = mtx[2][0];

	switch (mState) {
	case STATE_IDLE:
		mModel->getModelData()->onFlag1OnAllShapes();
		mTipModel->getModelData()->onFlag1OnAllShapes();
		break;
	default:
		mModel->getModelData()->offFlag1OnAllShapes();
		mTipModel->getModelData()->offFlag1OnAllShapes();

		JGeometry::TVec3<f32> tip = mTipPos;
		tip.y += 50.0f;
		SMS_MakeJointsToArc(mModel, mHeadPos, mHeadDir, tip);

		Mtx modelMtx;
		MtxPtr a = mModel->getAnmMtx(mModel->getModelData()->getJointNum() - 2);
		MtxPtr b = mModel->getAnmMtx(mModel->getModelData()->getJointNum() - 1);

		JGeometry::TVec3<f32> dir;
		f32 x = b[0][3] - a[0][3];
		dir.x = x;
		dir.y = 0.0f;
		f32 z = b[2][3] - a[2][3];
		dir.z = z;
		MsVECNormalize(&dir, &dir);

		JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
		JGeometry::TVec3<f32> tmp;
		tmp.cross(up, dir);

		modelMtx[0][0] = tmp.x;
		modelMtx[0][1] = up.x;
		modelMtx[0][2] = dir.x;
		modelMtx[0][3] = tip.x;

		modelMtx[1][0] = tmp.y;
		modelMtx[1][1] = up.y;
		modelMtx[1][2] = dir.y;
		modelMtx[1][3] = tip.y;

		modelMtx[2][0] = tmp.z;
		modelMtx[2][1] = up.z;
		modelMtx[2][2] = dir.z;
		modelMtx[2][3] = tip.z;

		mTipModel->setBaseTRMtx(modelMtx);
		mTipModel->calc();
		break;
	}
}

void TYoshiTongue::viewCalc()
{
	switch (mState) {
	case STATE_IDLE:
		break;
	default:
		mModel->viewCalc();
		mTipModel->viewCalc();
		break;
	}
}

void TYoshiTongue::entry()
{
	switch (mState) {
	case STATE_IDLE:
		break;
	default:
		mModel->entry();
		mTipModel->entry();
		break;
	}
}
