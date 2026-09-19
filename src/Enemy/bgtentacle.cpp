#include <Enemy/BossGessoTentacle.hpp>
#include <Enemy/BossGesso.hpp>
#include <Enemy/Spline.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Mario.hpp>
#include <Map/Map.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/LightUtil.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JMath.hpp>
#include <stdlib.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

// FABRICATED
inline bool TBGTentacle::isAttackable()
{
	if (mTakeHit->checkHitFlag(HIT_FLAG_CANNOT_ATTACK))
		return false;
	if (mState == 10)
		return false;
	if (mState == 4)
		return false;
	if (mState == 6)
		return false;
	if (mState == 1 || mOwner->getAttackMode() == 7)
		return true;
	return false;
}

static const char* tstatestr[] = {
	"TSTATE_WAIT",     "TSTATE_ATTACK", "TSTATE_REST", "TSTATE_HELD",
	"TSTATE_AMPUTEE",  "TSTATE_STUN",   "TSTATE_HIDE", "TSTATE_FOLLOWBODY",
	"TSTATE_SYNCBODY", "TSTATE_GUARD",  nullptr,
};

TBGTentacle::TTentacleParams::TTentacleParams(const char* path)
    : TParams(path)
    , PARAM_INIT(mNodeLength, 130.0f)
    , PARAM_INIT(mNodeLenLimit, 1000.0f)
    , PARAM_INIT(mTotalLenLimit, 800.0f)
    , PARAM_INIT(mSpeedProp, 1.4f)
    , PARAM_INIT(mInertiaProp, 0.6f)
    , PARAM_INIT(mReflectProp, 0.1f)
    , PARAM_INIT(mSpeedMax, 50.0f)
    , PARAM_INIT(mVibrationForce, 6.0f)
    , PARAM_INIT(mVibrationSpeed, 3.0f)
    , PARAM_INIT(mDamagePropF, 4.0f)
    , PARAM_INIT(mDamagePropS, 2.0f)
    , PARAM_INIT(mDamageCountMax, 256)
    , PARAM_INIT(mDamageReflectProp, 1.5f)
    , PARAM_INIT(mDamageNodeLength, 100.0f)
    , PARAM_INIT(mAttackRadius, 80.0f)
    , PARAM_INIT(mAttackHeight, 120.0f)
    , PARAM_INIT(mDamageRadius, 150.0f)
    , PARAM_INIT(mDamageHeight, 200.0f)
    , PARAM_INIT(mAtkSoundTime, 120.0f)
{
	TParams::load(mPrmPath);
}

TBGTentacleMtxCalc::TBGTentacleMtxCalc(TBGTentacle* owner)
    : J3DMtxCalcSoftimageAnm(nullptr)
    , mOwner(owner)
{
}

void TBGTentacleMtxCalc::calc(u16 param_1)
{
	int uVar9 = mOwner->getUnk2C()->getModel()->getModelData()->getJointNum();
	J3DMtxCalcAnm::calc(param_1);
	int iVar8 = uVar9 - 1;

	// TODO: 99.3%, every instruction exact, frame 0x2c0 vs 0x2d0.  Slot map:
	// the four named vectors below (local_278, local_68, local_74, local_80)
	// sit as one descending 12-byte block in both builds, but retail has a
	// dead 4-byte slot between local_278 and local_68 (its block spans 0x250
	// to 0x283, ours 0x240 to 0x26f) and 12 more bytes of unreferenced
	// inline-temp pool: retail's pool leaves 0x1c dead above the
	// TVec3::sub argument copy and 0x80 dead between the volatile sqrt slot
	// and the spline-result temporary, and it orders those two the other way
	// round (volatile below the spline temp, ours above).  So the 16 bytes are
	// one 4-byte named local declared right after the spline point plus a
	// 12-byte pool temporary, not a body local we can name from the code.
	MtxPtr asdf = mOwner->getUnk2C()->getModel()->getAnmMtx(param_1);

	JGeometry::TVec3<f32> local_278
	    = mOwner->mSpline->getPoint(param_1 / f32(iVar8));

	asdf[0][3]  = local_278.x;
	f32 nodeY   = local_278.y;
	asdf[1][3]  = nodeY;
	f32 nodeZ   = local_278.z;
	asdf[2][3]  = nodeZ;

	JGeometry::TVec3<f32> local_68;
	JGeometry::TVec3<f32> local_74;
	JGeometry::TVec3<f32> local_80;

	if (param_1 == iVar8) {
		// TODO: an inline for extracting a column out of a matrix?
	// (The column indices below are read off the load offsets: 0xc/0x1c/0x2c
	// is [i][3], 8/0x18/0x28 is [i][2] and 4/0x14/0x24 is [i][1].)
		MtxPtr mtx1 = mOwner->getUnk2C()->getModel()->getAnmMtx(param_1 - 2);
		JGeometry::TVec3<f32> vec1(mtx1[0][3], mtx1[1][3], mtx1[2][3]);

		MtxPtr mtx2 = mOwner->getUnk2C()->getModel()->getAnmMtx(param_1 - 1);
		JGeometry::TVec3<f32> vec2;
		vec2.x = mtx2[0][3];
		vec2.y = mtx2[1][3];
		vec2.z = mtx2[2][3];

		JGeometry::TVec3<f32> diff = vec2 - vec1;

		if (!diff.isZero()) {
			VECNormalize(&diff, &local_68);
		} else {
			local_68.set(1.0f, 0.0f, 0.0f);
		}
	} else {
		MtxPtr mtx1 = mOwner->getUnk2C()->getModel()->getAnmMtx(param_1 + 1);
		JGeometry::TVec3<f32> vec1(mtx1[0][3], mtx1[1][3], mtx1[2][3]);

		vec1.x -= local_278.x;
		vec1.y -= nodeY;
		vec1.z -= nodeZ;

		if (!vec1.isZero()) {
			VECNormalize(&vec1, &local_68);
		} else {
			local_68.set(1.0f, 0.0f, 0.0f);
		}
	}

	f32 fVar1 = 1.0f;
	if (param_1 == 0) {
		local_74.set(0.0f, 1.0f, 0.0f);
	} else {
		MtxPtr mtx1 = mOwner->getUnk2C()->getModel()->getAnmMtx(param_1 - 1);
		JGeometry::TVec3<f32> zDir(mtx1[0][2], mtx1[1][2], mtx1[2][2]);

		local_74.cross(local_68, zDir);

		if (local_74.squared() < 0.01f) {
			local_74.set(mtx1[0][1], mtx1[1][1], mtx1[2][1]);
		}

		VECNormalize(&local_74, &local_74);
		JGeometry::TVec3<f32> local_d4(mtx1[0][3], mtx1[1][3], mtx1[2][3]);
		local_d4.x -= local_278.x;
		local_d4.y -= nodeY;
		local_d4.z -= nodeZ;

		f32 fVar13 = VECMag(&local_d4);
		if (fVar13 == 0.0f) {
			fVar1 = 1.0f;
		} else {
			f32 tmp = fVar13 * 3.0f;
			fVar1   = mOwner->getNodeLen() / tmp;
		}

		JGeometry::TVec3<f32> tmp1(mtx1[0][0], mtx1[1][0], mtx1[2][0]);
		JGeometry::TVec3<f32> tmp2(mtx1[0][1], mtx1[1][1], mtx1[2][1]);
		if (param_1 < iVar8 && tmp1.dot(local_68) > 0.0f
		    && tmp2.dot(local_74) < 0.0f) {
			local_74.negate();
		}
	}

	local_80.cross(local_68, local_74);

	if (!local_80.isZero()) {
		VECNormalize(&local_80, &local_80);
	} else {
		local_80.set(0.0f, 0.0f, 1.0f);
	}

	if ((param_1 == iVar8 || param_1 == uVar9 - 2) && fVar1 < 1.0f) {
		fVar1 = 1.0f;
	}

	if (abs(fVar1) > 0.01f) {
		volatile f32 f = fVar1 * __frsqrte(fVar1);
		fVar1          = f;
	}

	// Spelled out rather than MsClamp(fVar1, 0.7f, 1.2f): MsClamp's by-value
	// parameter is bound in a scratch FPR, so the clamp lands in f1 and costs
	// an `fmr f1, f29` retail does not have.  Retail clamps in fVar1's own
	// callee-saved f29, which only an in-place assignment produces.  (The
	// named result `f32 fVar13 = MsClamp(...)` is worth +8 of frame, so with
	// this spelling the frame gap above is 16 rather than 8.)
	if (fVar1 > 1.2f)
		fVar1 = 1.2f;
	else if (fVar1 < 0.7f)
		fVar1 = 0.7f;

	local_68.scale(fVar1);
	local_74.scale(fVar1);
	local_80.scale(fVar1);

	if (mOwner->getState() == 5 && (param_1 == iVar8 || param_1 == uVar9 - 2)) {
		local_74.set(0.0f, 0.3f, 0.0f);
		local_68.cross(local_74, local_80);
		VECNormalize(&local_68, &local_68);
		local_80.cross(local_68, local_74);
		VECNormalize(&local_80, &local_80);
		local_80.scale(2.0f);
	}

	MtxPtr dst = mOwner->getUnk2C()->getModel()->getAnmMtx(param_1);

	dst[0][0] = local_68.x;
	dst[1][0] = local_68.y;
	dst[2][0] = local_68.z;
	dst[0][1] = local_74.x;
	dst[1][1] = local_74.y;
	dst[2][1] = local_74.z;
	dst[0][2] = local_80.x;
	dst[1][2] = local_80.y;
	dst[2][2] = local_80.z;
}

TBGTakeHit::TBGTakeHit(TBGTentacle* owner, const char* name)
    : TTakeActor(name)
    , mOwner(owner)
{
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);

	TBGTentacle::TTentacleParams* pTVar3 = mOwner->getParams();
	initHitActor(0x8000006, 1, -0x80000000, pTVar3->mAttackRadius.get(),
	             pTVar3->mAttackHeight.get(), pTVar3->mDamageRadius.get(),
	             pTVar3->mDamageHeight.get());
	offHitFlag(HIT_FLAG_NO_COLLISION);
	unk74.zero();
}

// TODO: these were almost surely calling onHitFlag/offHitFlag, but what flag?..
void TBGTakeHit::enableAttackCheck() { }
void TBGTakeHit::disableAttackCheck() { }

MtxPtr TBGTakeHit::getTakingMtx() { return unk80; }

// TODO: fake
static inline JGeometry::TVec3<f32> fromPolar(f32 theta, f32 radius)
{
	return JGeometry::TVec3<f32>(radius * JMASSin(theta * (65536.0f / 360.0f)),
	                             0.0f,
	                             radius * JMASCos(theta * (65536.0f / 360.0f)));
}

BOOL TBGTakeHit::moveRequest(const JGeometry::TVec3<f32>& where_to)
{
	JGeometry::TVec3<f32> local_EC = where_to;
	gpMap->isTouchedOneWallAndMoveXZ(&local_EC.x, local_EC.y, &local_EC.z,
	                                 150.0f);

	// TODO: tentacle inline?
	JGeometry::TVec3<f32> delta = local_EC;
	TBGTentacle* ten            = mOwner;
	delta -= ten->mOwner->getPosition();
	f32 totalLenLimit = ten->getParams()->mTotalLenLimit.value;

	if (delta.length() > totalLenLimit) {
		unk74 = fromPolar(
		    gpMarioOriginal->getIntendedYaw(),
		    gpMarioOriginal->getIntendedMag()
		        * mOwner->mOwner->getSaveParam2()->getSLTentacleStretch());

		JGeometry::TVec3<f32> local_44 = mOwner->getOwner()->getPosition();
		local_44 -= local_EC;

		if (local_44.dot(unk74) < 0.0f)
			mOwner->incDamage();

		mOwner->getLastNode()->setPosition(local_EC);
		return false;
	} else {
		mOwner->getLastNode()->setPosition(local_EC);
		return true;
	}
}

// Binding level worth +16 of low region, landing
// TBGTakeHit::receiveMessage's frame at 0x90 (batch 121).
static inline TTakeActor* BgtentacleGetHeldObject(TTakeActor* p)
{
	TTakeActor* heldObject = p->getHeldObject();
	return heldObject;
}

BOOL TBGTakeHit::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->getActorType() == 0x80000001) {
		if (message == HIT_MESSAGE_TAKE) {
			TTakeActor* casted = (TTakeActor*)sender;
			if (BgtentacleGetHeldObject(casted) != nullptr
			    && casted->getHeldObject() != this)
				return false;

			if (mOwner->isThing3()) {
				mHolder = casted;
				mOwner->changeStateAndFixNodes(3);
				mOwner->getOwner()->unk1A0 = 1;
				return true;
			}
		}

		if (message == HIT_MESSAGE_THROWN || message == HIT_MESSAGE_UNK8) {
			mHolder               = nullptr;
			TBGTentacle* tentacle = mOwner;
			if (tentacle->getState() != 4) {
				if (tentacle->getOwner()->getAttackMode() == 6)
					tentacle->changeStateAndFixNodes(9);
				else
					tentacle->changeStateAndFixNodes(0);
			}
			return true;
		}

		// TODO: 16 bytes of frame short (0x80 vs 0x90); every member read here
		// already goes through an accessor except TBGTakeHit's own mOwner,
		// which has no getter in the map.
		if (message == HIT_MESSAGE_TRAMPLE || message == HIT_MESSAGE_HIP_DROP) {
			if (mOwner->getState() != 4 && mOwner->getState() != 5
			    && mOwner->getState() != 3 && mOwner->getState() != 6
			    && mOwner->getState() != 10) {
				mOwner->changeStateAndFixNodes(5);
				return true;
			}
		}
	}

	return false;
}

// TNode::addVelocity is a one-statement in-class body, so `mVelocity += v`
// inside it puts TVec3::add at inline depth 3 when the call is made directly
// -- and retail `bl`s add here, i.e. depth 4. One forwarding level supplies
// it (91.6 -> 95.9). The natural home is a TBGTentacle member
// (`addVelocityToLastNode`), but BossGessoTentacle.hpp is shared with
// bossgesso.cpp, so the level is parked TU-local.
// TODO: promote when that header is next swept.
// fabricated
static inline void BGTakeHitAddLastNodeVelocity(TBGTentacle* owner,
                                                const JGeometry::TVec3<f32>& v)
{
	owner->getLastNode()->addVelocity(v);
}

void TBGTakeHit::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		mPosition = mOwner->mSpline->getPoint(1.0f);
		mPosition.y -= mDamageHeight * 0.5f;

		ensureTakeSituation();

		if (mHolder != nullptr) {
			J3DModel* model = mOwner->unk2C->getModel();
			MtxPtr mtx
			    = model->getAnmMtx(model->getModelData()->getJointNum() - 1);

			JGeometry::TVec3<f32> vec1(mtx[0][0], mtx[1][0], mtx[2][0]);

			JGeometry::TVec3<f32> vec2;
			vec2.cross(vec1, JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f));
			vec2.normalize();

			JGeometry::TVec3<f32> vec3;
			vec3.cross(vec2, JGeometry::TVec3<f32>(0.0f, 0.0f, 1.0f));
			vec3.normalize();

			unk80.mMtx[0][0] = vec2.x;
			unk80.mMtx[1][0] = vec2.y;
			unk80.mMtx[2][0] = vec2.z;

			unk80.mMtx[0][1] = 0.0f;
			unk80.mMtx[1][1] = 1.0f;
			unk80.mMtx[2][1] = 0.0f;

			unk80.mMtx[0][2] = vec3.x;
			unk80.mMtx[1][2] = vec3.y;
			unk80.mMtx[2][2] = vec3.z;

			vec3.scale(mDamageRadius);

			JGeometry::TVec3<f32> tip(mtx[0][3], mtx[1][3], mtx[2][3]);
			tip += vec3;

			unk80.mMtx[0][3] = tip.x;
			unk80.mMtx[1][3] = tip.y;
			unk80.mMtx[2][3] = tip.z;

			JGeometry::TVec3<f32> local_8c
			    = mOwner->getLastNode()->getVelocity();
			local_8c.y = 0.0f;
			local_8c.scale(0.1f);

			if (!unk74.isZero()) {
				BGTakeHitAddLastNodeVelocity(mOwner, unk74);
				local_8c += unk74;
			}

			local_8c += mHolder->getPosition();
			gpMap->isTouchedOneWallAndMoveXZ(&local_8c.x, local_8c.y,
			                                 &local_8c.z, 150.0f);
			mHolder->moveRequest(local_8c);
			unk74.zero();
		}

		if (mOwner->isAttackable()) {
			for (int i = 0; i < mColCount; ++i) {
				THitActor* col = mCollisions[i];
				if (!col->isActorType(0x80000001))
					continue;

				if (gpMarioOriginal->isRoofing())
					continue;

				if (mOwner->mOwner->getAttackMode() == 7)
					mOwner->throwMario(col, this);
				else if (mOwner->mOwner->getAttackMode() == 4)
					mOwner->throwMario(col, mOwner->mOwner);
				else
					col->receiveMessage(this, HIT_MESSAGE_ATTACK);
			}
		}

		if (mOwner->getState() != 3 && mOwner->getState() != 4
		    && mHolder != nullptr)
			mHolder->receiveMessage(this, HIT_MESSAGE_UNK8);
	}
}

TBGAttackHit::TBGAttackHit(TBGTentacle* owner, f32 pos_on_spline,
                           const char* name)
    : THitActor(name)
    , mOwner(owner)
    , mPosOnSpline(pos_on_spline)
{
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);
	initHitActor(0x8000007, 1, -0x80000000, 50.0f, 50.0f, 50.0f, 50.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBGAttackHit::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		mPosition = mOwner->mSpline->getPoint(mPosOnSpline);

		if (mOwner->isAttackable()) {
			for (int i = 0; i < mColCount; ++i) {
				THitActor* col = mCollisions[i];
				if (gpMarioOriginal->isRoofing())
					continue;

				// TODO: this is an inline, see TBGTakeHit::perform
				if (mOwner->mOwner->getAttackMode() == 7)
					mOwner->throwMario(col, this);
				else if (mOwner->mOwner->getAttackMode() == 4)
					mOwner->throwMario(col, mOwner->mOwner);
				else
					col->receiveMessage(this, HIT_MESSAGE_ATTACK);
			}
		}
	}
}

TBGTentacle::TNode::TNode()
    : unk24(0)
{
	mPosition.zero();
	mVelocity.zero();
	unk18.zero();
}

void TBGTentacle::TNode::calcVelocity(TBGTentacle* param_1,
                                      TBGTentacle::TNode* param_2, f32 param_3)
{
	f32 inertiaProp = param_1->getParams()->mInertiaProp.get();
	f32 speedMax    = param_1->getParams()->mSpeedMax.get();

	JGeometry::TVec3<f32> local_A8 = param_2->mPosition;
	local_A8 += param_2->mVelocity;

	JGeometry::TVec3<f32> local_9C = getPosition();
	local_9C -= local_A8;

	f32 nodeLen = param_1->getNodeLen();

	f32 nodeLenLimit = param_1->getParams()->mNodeLenLimit.get();

	JGeometry::TVec3<f32> local_8C = local_9C;
	f32 fVar5;
	if (local_9C.isZero()) {
		fVar5 = 0.0f;
		local_8C.zero();
	} else {
		fVar5 = VECMag(&local_9C);
		VECNormalize(&local_8C, &local_8C);
	}

	if (fVar5 > nodeLenLimit) {
		if (unk24) {
			if (!param_2->unk24) {
				local_8C.scale(nodeLenLimit);
				mPosition += local_8C;
			}
		} else {
			local_8C.scale(nodeLenLimit);
			mPosition = local_A8;
			mPosition += local_8C;
		}
	} else {
		if (nodeLen < fVar5) {
			f32 m = nodeLen - fVar5;
			if (m > speedMax)
				m = speedMax;
			fVar5 += m;
		} else {
			f32 m = fVar5 - nodeLen;
			if (m > speedMax)
				m = speedMax;
			fVar5 -= m;
		}

		local_8C.scale(fVar5);

		JGeometry::TVec3<f32> local_80 = local_A8;
		local_80 += local_8C;

		JGeometry::TVec3<f32> local_74 = local_80;
		local_74 -= getPosition();

		local_74.scale(param_3);

		mVelocity.scale(inertiaProp);
		mVelocity += local_74;
	}
}

void TBGTentacle::TNode::calcPosition(TBGTentacle* param_1)
{
	if (unk24)
		return;

	if (param_1->getState() == 5) {
		mPosition += mVelocity;
		return;
	}

	if (param_1->getState() == 4) {
		mPosition += mVelocity;
		return;
	}

	if (param_1->getState() == 7) {
		mPosition += mVelocity;
		return;
	}

	if (param_1->getState() == 3) {
		mPosition += mVelocity;
		return;
	}

	JGeometry::TVec3<f32> local_1c = unk18;
	local_1c -= mPosition;
	f32 len = local_1c.squared();
	if (len > 0.01f) {

		// TODO: inline?
		f32 fVar2;
		if (len > 100000000.0f) {
			fVar2 = 1.0f;
		} else {
			if (param_1->mState == 8) {
				if (param_1->mTimeInCurrentState < 60)
					fVar2 = (param_1->mTimeInCurrentState * 0.5f) / 60.0f;
				else
					fVar2 = 0.5f;
			} else if (param_1->mState == 1 || param_1->mState == 10) {
				if (param_1->mTimeInCurrentState < 180)
					fVar2 = (param_1->mTimeInCurrentState * 0.4f) / 180.0f;
				else
					fVar2 = 0.4f;
			} else if (param_1->mState == 2) {
				if (param_1->mTimeInCurrentState < 120)
					fVar2 = (param_1->mTimeInCurrentState * 0.05f) / 120.0f;
				else
					fVar2 = 0.05f;
			} else {
				fVar2 = VECMag(&local_1c);
				fVar2 = fVar2 * 0.00025f;
				fVar2 *= fVar2;
			}
		}

		if (fVar2 > 1.0f)
			fVar2 = 1.0f;

		local_1c.scale(fVar2);
		mVelocity += local_1c;
	}

	mPosition += mVelocity;
}

TBGTentacle::TBGTentacle(TBossGesso* owner, int node_num, int index)
    : JDrama::TViewObj("あし")
    , mState(0)
    , mTimeInCurrentState(0)
    , mIndex(index)
    , mDamageCount(0)
    , mNodeNum(node_num)
    , mNodes(new TNode[node_num])
    , mSpline(nullptr)
    , unk2C(nullptr)
    , mMtxCalc(nullptr)
    , mOwner(owner)
    , mParams(new TTentacleParams("/enemy/tentacle.prm"))
    , mTakeHit(nullptr)
    , unk40(0.0f)
    , unk44(0.0f)
    , unk48(0)
    , unk4C(0)
    , unk80(nullptr)
{
	mSpline = new TSplinePath(node_num);
	unk40   = (int)(MsRandF() * 360.0f);
	unk44   = (int)(MsRandF() * 360.0f);

	TMActorKeeper* keeper = mOwner->getActorKeeper();

	unk2C    = keeper->createMActor("bgeso_hand.bmd", 0);
	mMtxCalc = new TBGTentacleMtxCalc(this);
	unk2C->setCalcForBck(mMtxCalc);
	unk2C->calc();
	unk2C->setLightType(LIGHT_TYPE_OBJECT);
	unk80 = keeper->createMActor("bgeso_shand.bmd", 0);
	unk84.zero();
	mTakeHit = new TBGTakeHit(this);
	for (int i = 0; i < 5; ++i)
		mAttackHit[i] = new TBGAttackHit(this, i / 5.0f);
}

void TBGTentacle::incDamage()
{
	if (getState() != 4)
		changeStateAndFixNodes(3);

	if (mDamageCount < getParams()->mDamageCountMax.get())
		mDamageCount += 1;
}

void TBGTentacle::throwMario(THitActor* param_1, THitActor* param_2)
{
	JGeometry::TVec3<f32> local_e0 = param_1->getPosition();
	local_e0 -= param_2->getPosition();

	local_e0.normalize();
	local_e0.scale(2.0f);
	local_e0.y += 1.0f;

	SMS_SendMessageToMario(param_2, HIT_MESSAGE_ATTACK);
	SMS_SendMessageToMario(param_2, 0x7);

	SMS_ThrowMario(local_e0, 3.21875f);
	mOwner->showMessage(5);
	mOwner->stopIfRoll();
}

BOOL TBGTentacle::isAttacking() const { }

bool TBGTentacle::canTake() const { }

f32 TBGTentacle::getNodeLen() const
{
	return mState == 4 ? mParams->mDamageNodeLength.get()
	                   : mParams->mNodeLength.get();
}

void TBGTentacle::continuousRumble()
{
	switch (mState) {
	case 4:
		if (mTakeHit->getHolder() != nullptr && mTimeInCurrentState % 4 == 0)
			mOwner->rumblePad(1, mTakeHit->getPosition());
		break;
	case 3:
		if (mTakeHit->getHolder() != nullptr && mTimeInCurrentState % 16 == 0)
			mOwner->rumblePad(0, mTakeHit->getPosition());
		break;
	}
}

void TBGTentacle::beatNode(int index, const JGeometry::TVec3<f32>& param_2)
{
	mNodes[index].setVelocity(param_2);

	f32 fVar1;
	if (getState() == 4)
		fVar1 = getParams()->mDamageReflectProp.get();
	else
		fVar1 = getParams()->mReflectProp.get();

	for (int i = index + 1; i < mNodeNum; ++i) {
		mNodes[i].calcVelocity(this, &mNodes[i - 1], fVar1);

		if (mNodes[i].isUnk24()) {
			for (int j = i - 1; j >= 0; --j)
				mNodes[j].calcVelocity(this, &mNodes[j + 1], fVar1);
			break;
		}
	}

	for (int i = index - 1; i >= 0; --i) {
		mNodes[i].calcVelocity(this, &mNodes[i + 1], fVar1);

		if (mNodes[i].isUnk24()) {
			for (int j = i + 1; j < mNodeNum; ++j)
				mNodes[j].calcVelocity(this, &mNodes[j - 1], fVar1);
			break;
		}
	}
}

void TBGTentacle::setAttackTarget()
{
	unk84 = SMS_GetMarioPos();

	if (mOwner->getAttackMode() == 2) {
		int iVar9;
		switch (mIndex) {
		case 0:
			iVar9 = -2;
			break;
		case 1:
			iVar9 = -1;
			break;
		case 2:
			iVar9 = 2;
			break;
		case 3:
			iVar9 = 1;
			break;
		default:
			iVar9 = 0;
			break;
		}

		JGeometry::TVec3<f32> local_148 = unk84;
		local_148 -= mOwner->mPosition;

		JGeometry::TVec3<f32> local_3c;
		local_3c.cross(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f), local_148);
		local_3c.normalize();

		// TODO: the cross above is the open `TVec3::cross()` store-order
		// header item: retail stores x and y, reloads local_148.x and then
		// stores z, while our header computes all three before storing.
		// Nothing at this call site changes it (JGVec3.hpp item).
		// The frame is 112 bytes short, all of it low region, and
		// getOwner()->mPosition for the subtrahend buys 8 of that with no
		// instruction change.
		// TODO: retail makes four 12-byte copies around the two out-of-line
		// scale() calls (parameter, result, parameter, result); the named
		// intermediate below makes five and the single-expression form
		// `unk84 += local_3c * (f32)iVar9 * 80.0f` makes three, so retail
		// elides the first result into the named vector and we do not.
		JGeometry::TVec3<f32> local_cc = local_3c * (f32)iVar9;
		unk84 += local_cc * 80.0f;
	}

	if (mOwner->is2ndFightNow() && unk84.y < mOwner->mPosition.y + 20.0f)
		unk84.y = mOwner->mPosition.y + 20.0f;

	if (mOwner->getAttackMode() == 2) {
		if (gpMarioOriginal->isTouchGround4cm()
		    || gpMarioOriginal->isRoofing()) {
			unk80->setBckFromIndex(23);
		} else {
			unk80->setBckFromIndex(21);
		}
	} else if (mOwner->getAttackMode() == 1) {
		unk80->setBckFromIndex(23);
	} else if (mOwner->getAttackMode() == 4) {
		unk80->setBckFromIndex(24);
	} else {
		if (gpMarioOriginal->isTouchGround4cm()
		    || gpMarioOriginal->isRoofing()) {
			unk80->setBckFromIndex(20);
		} else {
			unk80->setBckFromIndex(21);
		}
	}

	J3DFrameCtrl* ctrl = unk80->getFrameCtrl(0);
	ctrl->setRate(mOwner->getAttackSpeed() * SMSGetAnmFrameRate());
	unk4C = 0;
}

void TBGTentacle::changeStateAndFixNodes(int new_state)
{
	if (mState == new_state)
		return;

	mState              = new_state;
	mTimeInCurrentState = 0;

	for (int i = 0; i < mNodeNum; ++i)
		mNodes[i].offUnk24();

	switch (mState) {
	case 1:
		getFirstNode()->onUnk24();
		setAttackTarget();
		break;

	case 4:
		if (unk48) {
			getLastNode()->onUnk24();
			break;
		}

		{
			getLastNode()->onUnk24();
			mOwner->gotTentacleDamage();
			unk48        = 1;
			mDamageCount = 0;

			static const int idx[] = { 8, 14, 28, 34 };
			MtxPtr mtx             = mOwner->getModel()->getAnmMtx(idx[mIndex]);
			gpMarioParticleManager->emitAndBindToMtxPtr(0x95, mtx, 0, nullptr);
			gpMarioParticleManager->emitAndBindToMtxPtr(0x96, mtx, 0, nullptr);

			mOwner->rumblePad(1, mOwner->getPosition());
		}
		break;

	case 5: {
		getFirstNode()->onUnk24();
		if (mOwner->getAttackMode() == 6)
			mNodes[1].onUnk24();
		JGeometry::TVec3<f32> zero;
		zero.zero();
		for (int i = 0; i < mNodeNum; ++i)
			mNodes[i].setVelocity(zero);
		break;
	}

	case 3:
		getFirstNode()->onUnk24();
		getLastNode()->onUnk24();
		break;

	case 10:
		getFirstNode()->onUnk24();
		unk80->setBckFromIndex(0x16);
		break;

	case 0:
	case 2:
	case 7:
	case 8:
		getFirstNode()->onUnk24();
		break;

	case 9:
		getFirstNode()->onUnk24();
		mNodes[1].onUnk24();
		break;

	case 6:
		getFirstNode()->onUnk24();
		mDamageCount = 0;
		unk48        = 0;
		break;
	}

	if (mState == 6)
		mTakeHit->onHitFlag(HIT_FLAG_NO_COLLISION);
	else
		mTakeHit->offHitFlag(HIT_FLAG_NO_COLLISION);

	if (mState == 9)
		mTakeHit->onHitFlag(HIT_FLAG_CANNOT_ATTACK);

	mTakeHit->offHitFlag(HIT_FLAG_CANNOT_GET_HIT);
}

void TBGTentacle::returnToDefaultState() { }

void TBGTentacle::moveNode()
{
	f32 fVar1 = mParams->mVibrationSpeed.get();
	f32 fVar2 = mParams->mVibrationForce.get();

	if (mState == 4) {
		fVar2 *= mParams->mDamagePropF.get();
		fVar1 *= mParams->mDamagePropS.get();
		unk40 = MsWrap(unk40 + fVar1, 0.0f, 360.0f);
		unk44 = MsWrap(unk44 + fVar1 * 1.3f, 0.0f, 360.0f);
	} else {
		unk40 = MsWrap(unk40 + fVar1, 0.0f, 360.0f);
		unk44 = MsWrap(unk44 + fVar1 * 1.3f, 0.0f, 360.0f);
	}

	JGeometry::TVec3<f32> local_88;
	switch (mIndex) {
	case 0:
		local_88.x = JMASin(unk40) * -1.9f * fVar2;
		local_88.y = JMASin(unk40) * 1.0f * fVar2;
		local_88.z = JMACos(unk44) * 1.8f * fVar2;
		break;
	case 1:
		local_88.x = JMASin(unk40) * -1.0f * fVar2;
		local_88.y = JMASin(unk44) * 0.8f * fVar2;
		local_88.z = JMACos(unk44) * 1.1f * fVar2;
		break;
	case 3:
		local_88.x = JMASin(unk40) * 1.0f * fVar2;
		local_88.y = JMASin(unk40) * 0.8f * fVar2;
		local_88.z = JMACos(unk44) * 1.1f * fVar2;
		break;
	default:
		local_88.x = JMASin(unk40) * 1.9f * fVar2;
		local_88.y = JMASin(unk44) * 1.0f * fVar2;
		local_88.z = JMACos(unk44) * 1.8f * fVar2;
		break;
	}

	if (mState == 5) {
		for (int i = 0; i < mNodeNum; ++i) {
			JGeometry::TVec3<f32> local_94 = mNodes[i].getVelocity();

			local_94.x = 0.0f;
			local_94.z = 0.0f;

			local_94.y -= mOwner->getGravityY();

			mNodes[i].setVelocity(local_94);
		}
	} else if (mState == 3) {
		beatNode(mNodeNum - 1, local_88);
	} else {
		beatNode(mNodeNum - 1, local_88);
		local_88.negate();
		beatNode(mNodeNum - 3, local_88);
	}

	for (int i = 0; i < mNodeNum; ++i) {
		mNodes[i].calcPosition(this);

		if (i <= 1 && mOwner->getAttackMode() == 6)
			continue;

		TNode* node = &mNodes[i];

		JGeometry::TVec3<f32> local_ac = node->getPosition();

		gpMap->isTouchedOneWallAndMoveXZ(&local_ac.x, local_ac.y, &local_ac.z,
		                                 150.0f);

		const TBGCheckData* pTStack_b0;
		f32 dVar16;
		if ((gpMarDirector->getCurrentMap() == 3
		     && gpMarDirector->getCurrentStage() == 0)
		    || gpMarDirector->getCurrentMap() == 59) {
			dVar16 = gpMap->checkGround(local_ac.x, local_ac.y + 200.0f,
			                            local_ac.z, &pTStack_b0);
		} else if (mOwner->is2ndFightNow()) {
			dVar16 = mOwner->mPosition.y;
		} else if (gpMarDirector->getCurrentMap() == 9
		           && gpMarDirector->getCurrentStage() == 1) {
			dVar16
			    = gpMap->checkGround(local_ac.x, mOwner->mPosition.y + 500.0f,
			                         local_ac.z, &pTStack_b0);
			f32 fVar15 = mOwner->mPosition.y;
			if (dVar16 < fVar15)
				dVar16 = fVar15;
		} else {
			dVar16
			    = gpMap->checkGround(local_ac.x, mOwner->mPosition.y + 500.0f,
			                         local_ac.z, &pTStack_b0);
		}

		if (local_ac.y < dVar16 + 20.0f) {
			mNodes[i].setVelocity(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
			local_ac.y = dVar16 + 20.0f;
		}

		if (mState == 5 && local_ac.y < mOwner->mPosition.y) {
			local_ac.y = mOwner->mPosition.y + 10.0f;
		}

		// TODO: inline, or need to try harder to make MWCC merge the loads?
		TBossGesso* bossGesso          = mOwner;
		JGeometry::TVec3<f32> local_c8 = bossGesso->mPosition;
		if (local_c8.y < local_ac.y) {
			local_c8 -= local_ac;

			f32 thing = bossGesso->mBodyScale * bossGesso->mBodyRadius;
			// TODO: common subexpression eliminator doesn't want to merge
			// the `squared`  calls here =(
			if (local_c8.length() < thing) {
				local_c8.normalize();
				local_c8.scale(thing);
				local_ac += local_c8;
			}
		}

		mNodes[i].setPosition(local_ac);
	}

	for (int i = 0; i < mNodeNum; ++i)
		mSpline->setPoint(i, mNodes[i].getPosition());
}

void TBGTentacle::moveConstraint()
{
	switch (mState) {
	case 1:
		for (int i = 0; i < mNodeNum; ++i) {
			MtxPtr mtx = unk80->getModel()->getAnmMtx(i);
			JGeometry::TVec3<f32> guide;
			guide.x = mtx[0][3];
			guide.y = mtx[1][3];
			guide.z = mtx[2][3];
			mNodes[i].setUnk18(guide);
		}
		int iVar10;
		if (mOwner->beakHeld()) {
			iVar10 = mOwner->getSaveParam2()->mSLBeakHoming.value;
		} else {
			if (mOwner->getAttackMode() == 2) {
				iVar10 = mOwner->getSaveParam2()->mSLUnisonHoming.value;
			} else {
				iVar10 = mOwner->getSaveParam2()->mSLSingleHoming.value;
			}
		}

		if (mTimeInCurrentState < iVar10 && mOwner->getAttackMode() != 2) {
			unk84.x = SMS_GetMarioPos().x;
			unk84.z = SMS_GetMarioPos().z;
		}
		break;

	case 10:
		for (int i = 0; i < mNodeNum; ++i) {
			MtxPtr mtx = unk80->getModel()->getAnmMtx(i);
			JGeometry::TVec3<f32> guide;
			guide.x = mtx[0][3];
			guide.y = mtx[1][3];
			guide.z = mtx[2][3];
			mNodes[i].setUnk18(guide);
		}
		break;

	case 5:
		if (mOwner->getAttackMode() == 6)
			return;
	// FALLTHROUGH
	case 0:
	case 2:
	case 8:
		if (mOwner->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
			JGeometry::TVec3<f32> local_38 = mOwner->getPosition();
			for (int i = 0; i < mNodeNum; ++i) {
				mNodes[i].setUnk18(local_38);
				local_38.y += getNodeLen();
			}
		} else {
			static int jntidx[] = { 8, 14, 28, 34 };
			int iVar15          = jntidx[mIndex];
			for (int i = 0; i < mNodeNum; ++i) {
				MtxPtr mtx = mOwner->getModel()->getAnmMtx(i + iVar15);
				JGeometry::TVec3<f32> joint;
				joint.x = mtx[0][3];
				joint.y = mtx[1][3];
				joint.z = mtx[2][3];
				mNodes[i].setUnk18(joint);
			}
		}
		break;

	case 9: {
		// TODO: retail makes two 12-byte copies here (a temporary at 0xc plus
		// the local), which a by-value TNode::getPosition() would explain --
		// but that costs calcAttackGuideAnm, calcAtkParticleAndSE,
		// decideOwnState and moveNode 2-3 points each, so it is not that.
		JGeometry::TVec3<f32> local_58 = mNodes[0].getPosition();
		mNodes[0].setUnk18(local_58);
		for (int i = 1; i < mNodeNum; ++i) {
			local_58.x -= getNodeLen() * 1.3f;
			local_58.y -= getNodeLen() * 1.5f;
			mNodes[i].setUnk18(local_58);
		}
		break;
	}

	case 6: {
		JGeometry::TVec3<f32> local_38 = mOwner->mPosition;
		for (int i = 0; i < mNodeNum; ++i) {
			mNodes[i].setUnk18(local_38);
			local_38.y += 0.5f * getNodeLen();
		}
		break;
	}
	}
}

void TBGTentacle::decideOwnState()
{
	switch (getState()) {
	case 1:
		if (unk80->curAnmEndsNext())
			changeStateAndFixNodes(2);
		break;

	case 5: {
		JGeometry::TVec3<f32> delta = getLastNode()->getPosition();
		delta -= mOwner->getPosition();
		f32 rot = MsGetRotFromZaxisY(delta);
		if (abs(MsAngleDiff(mOwner->getRotation().y, rot)) > 45.0f) {
			changeStateAndFixNodes(2);
		}

		if (getState() == 5
		    && mTimeInCurrentState >= mOwner->getSaveParam2()->getSLStunTime()) {
			if (mOwner->getAttackMode() == 6)
				changeStateAndFixNodes(9);
			else
				changeStateAndFixNodes(0);
		}
	} // FALLTHROUGH

	case 0:
	case 2:
		if (getState() == 2
		    && mTimeInCurrentState >= mOwner->getSaveParam2()->getSLRestTime()) {
			if (mOwner->getAttackMode() == 6)
				changeStateAndFixNodes(9);
			else
				changeStateAndFixNodes(0);
		}
		break;

	case 4: {
		int amputeeTime = mOwner->getSaveParam2()->getSLAmputeeTime();
		if (mTimeInCurrentState >= amputeeTime) {
			changeStateAndFixNodes(6);
			break;
		}

		if (mTimeInCurrentState >= amputeeTime - 240
		    && mTakeHit->getHolder() != nullptr) {
			TTakeActor* holder = mTakeHit->getHolder();
			holder->receiveMessage(mTakeHit, HIT_MESSAGE_UNK8);
		}
		break;
	}

	case 10:
		break;
	}
}

void TBGTentacle::checkDamage()
{
	if (mDamageCount >= mParams->mDamageCountMax.get()) {
		if (mOwner->getAttackMode() == 6)
			gpMarDirector->fireStreamingMovie(10);

		mOwner->unk1A8 = mOwner->getSaveParam2()->mSLAmputeeWait.get();
		changeStateAndFixNodes(4);
	}
}

void TBGTentacle::calcAtkParticleAndSE()
{
	if ((unk80->checkCurBckFromIndex(20) || unk80->checkCurBckFromIndex(23))
	    && unk4C == 0) {
		f32 frame = unk80->getFrameCtrl(0)->getFrame();
		if ((unk80->checkCurBckFromIndex(20) && 118.0f <= frame
		     && frame <= 160.0f)
		    || (unk80->checkCurBckFromIndex(23) && 75.0f <= frame
		        && frame <= 110.0f)) {
			JGeometry::TVec3<f32> local_28 = getLastNode()->getPosition();
			const TBGCheckData* pTStack_2c;
			f32 dVar10 = gpMap->checkGround(local_28.x, local_28.y + 500.0f,
			                                local_28.z, &pTStack_2c);
			if (local_28.y < dVar10 + 100.0f) {
				local_28.y = dVar10 + 5.0f;
				gpMarioParticleManager->emit(BGESO_JPA_MS_BOGE_ATTACK_A,
				                             &local_28, 0, nullptr);
				gpMarioParticleManager->emit(BGESO_JPA_MS_BOGE_ATTACK_B,
				                             &local_28, 0, nullptr);
				gpMarioParticleManager->emit(BGESO_JPA_MS_BOGE_ATTACK_C,
				                             &local_28, 0, nullptr);
				gpMarioParticleManager->emit(BGESO_JPA_MS_BOGE_ATTACK_D,
				                             &local_28, 0, nullptr);
				unk4C = 1;

				SMSGetMSound()->startSoundActor(MSD_SE_BS_GESO_ATK_IMPACT,
				                                &mOwner->getPosition(), 0,
				                                nullptr, 0, 4);

				mOwner->rumblePad(2, mOwner->getPosition());
			}
		}
	}

	f32 fVar2 = mOwner->getSaveParam2()->mSLBlurScale.value;
	int iVar5 = unk80->checkCurBckFromIndex(23)
	                ? 7
	                : mOwner->getSaveParam2()->mSLBlurJoint.get();

	unk80->getModel()->setAnmMtx(3, unk50);
	MTXScaleApply(unk50, unk50, fVar2, fVar2, fVar2);

	MtxPtr mtx  = unk2C->getModel()->getAnmMtx(iVar5);
	unk50[0][3] = mtx[0][3];
	unk50[1][3] = mtx[1][3];
	unk50[2][3] = mtx[2][3];

	if (mState != 10 && mState != 8) {
		if (unk80->checkCurBckFromIndex(24)) {
			if (unk80->checkBckPass(115.0f))
				gpMarioParticleManager->emitAndBindToSRTMtxPtr(0x92, unk50, 0,
				                                               nullptr);
		} else if (unk80->checkCurBckFromIndex(20)) {
			if (unk80->checkBckPass(115.0f))
				gpMarioParticleManager->emitAndBindToSRTMtxPtr(0x90, unk50, 0,
				                                               nullptr);
		} else if (unk80->checkCurBckFromIndex(23)) {
			if (unk80->checkBckPass(86.0f))
				gpMarioParticleManager->emitAndBindToSRTMtxPtr(0x90, unk50, 0,
				                                               nullptr);
		} else {
			if (unk80->checkBckPass(81.0f))
				gpMarioParticleManager->emitAndBindToSRTMtxPtr(0x91, unk50, 0,
				                                               nullptr);
		}
	}

	f32 atkSoundTime = mParams->mAtkSoundTime.get();
	s16 endFrame     = unk80->getFrameCtrl(0)->getEnd();
	if (unk80->checkBckPass(endFrame - atkSoundTime)) {
		if (mState != 10) {
			SMSGetMSound()->startSoundActor(
			    MSD_SE_BS_GESO_ATK, &mTakeHit->getPosition(), 0, nullptr, 0, 4);

			if (!mOwner->unk1AC) {
				mOwner->unk1AC = 0xF0;

				SMSGetMSound()->startSoundActor(MSD_SE_BS_GESO_VO_ATTACK,
				                                &mOwner->getPosition(), 0,
				                                nullptr, 0, 4);
			}
		}
	}
}

void TBGTentacle::decideAtkColExists()
{
	mTakeHit->onHitFlag(HIT_FLAG_CANNOT_ATTACK);

	f32 frame = unk80->getFrameCtrl(0)->getFrame();

	BOOL shouldCollisionExist = false;

	if (mState == 10) {
		shouldCollisionExist = true;
	} else if (mOwner->getAttackMode() == 7) {
		shouldCollisionExist = true;
	} else if (unk80->checkCurBckFromIndex(21)) {
		if (75.0f <= frame && frame <= 130.0f)
			shouldCollisionExist = true;
		else
			shouldCollisionExist = false;
	} else if (unk80->checkCurBckFromIndex(23)) {
		if (75.0f <= frame && frame <= 110.0f)
			shouldCollisionExist = true;
		else
			shouldCollisionExist = false;
	} else if (unk80->checkCurBckFromIndex(20)) {
		if (75.0f <= frame && frame <= 130.0f)
			shouldCollisionExist = true;
		else
			shouldCollisionExist = false;
	} else if (unk80->checkCurBckFromIndex(24)) {
		if (80.0f <= frame && frame <= 220.0f)
			shouldCollisionExist = true;
		else
			shouldCollisionExist = false;
	}

	if (shouldCollisionExist) {
		mTakeHit->offHitFlag(HIT_FLAG_CANNOT_ATTACK);
	} else {
		mTakeHit->onHitFlag(HIT_FLAG_CANNOT_ATTACK);
	}
}

void TBGTentacle::calcAttackGuideAnm()
{
	if (mState != 1 && mState != 8 && mState != 10)
		return;

	JGeometry::TVec3<f32> local_30 = getFirstNode()->getPosition();
	// Naming the two components retail keeps in f30/f31 across the two calls
	// below; see the TODO above MsMtxSetTRS.
	f32 f31 = local_30.z;
	f32 f30 = local_30.y;

	JGeometry::TVec3<f32> local_3c = unk84;
	local_3c.x -= local_30.x;
	local_3c.y -= f30;
	local_3c.z -= f31;
	JGeometry::TVec3<f32> local_b4 = MsGetRotFromZaxis(local_3c);

	f32 guideScale;
	if (mState == 10) {
		guideScale = 1.0f;
	} else if (unk80->checkCurBckFromIndex(20)) {
		guideScale = (1.0f / 1500.0f) * local_3c.length();
	} else {
		guideScale = (1.0f / 1200.0f) * local_3c.length();
	}

	if (guideScale > 2.0f)
		guideScale = 2.0f;

	// TODO: 98.9%.  Two residues, both measured in closure batch 90.
	// (1) Retail loads local_30.z into f31 at the third subtraction above,
	//     ours hoists it eight instructions earlier; the instruction set is
	//     identical, only the schedule differs.  Splitting the declaration
	//     (`f32 f31; ... f31 = local_30.z;`) or declaring it with its
	//     initialiser at the use site both put the load in retail's place but
	//     add an `fmr` from the scratch register the load lands in, so they
	//     are worse (99.3% with an extra instruction).
	// (2) Frame 0x170 vs 0x150.  The named region is byte-identical (local_30
	//     0x28 from the top, local_3c 0x3c, local_b4 0x48, then the two Mtx
	//     as one 0x60 block); the 32 bytes are unreferenced inline-temp pool
	//     between the MsGetRotFromZaxis out-parameter temporary at 0x9c and
	//     the first Mtx (retail packs them adjacent at 0xa8, ours starts at
	//     0xc8).  So we expand one level more than retail somewhere in this
	//     body: spelling both `local_3c.length()` sites as
	//     TUtil<f32>::sqrt(local_3c.squared()) removes exactly 8 of the 32
	//     (4 per expansion), which is the one lever found; the remaining 24
	//     are unattributed.  `getFirstNode()->mPosition` for the copy cannot
	//     be tried without making TNode::mPosition public.
	// (3) The four zangle tables must stay `static const`: as plain const
	//     locals MWCC copies each one to the stack (76.4%, +42 instructions).
	//
	// Retail keeps local_30's y and z in f30/f31 from the subtraction
	// above all the way into this call and reloads only .x here. Measured in a
	// scratch TU with the game flags: MWCC gives a callee-saved FPR only to a
	// *named* f32 local of the function's own body, never to an aggregate
	// member read nor to a local of an inlined callee, so the two components
	// retail preserves were named in the source and .x was not. Declaring z
	// before y is what puts y in f30 and z in f31 (the reverse order gives
	// f31/f30). The per-component subtraction above is forced by the same
	// measurement: with `local_3c -= local_30` the subtrahend loads come back
	// from the stack instead of feeding f30/f31.
	Mtx afStack_78;
	MsMtxSetTRS(afStack_78, local_30.x, f30, f31, local_b4.x, local_b4.y,
	            local_b4.z, 1.0f, 1.0f, guideScale);

	Mtx local_a8;
	if (mState == 10) {
		static const f32 zangle[] = { 80.0f, 60.0f, -80.0f, -60.0f };
		f32 s                     = JMASin(zangle[mIndex]);
		f32 c                     = JMACos(zangle[mIndex]);

		local_a8[0][0] = c;
		local_a8[0][1] = -s;
		local_a8[0][2] = 0.0f;
		local_a8[0][3] = 0.0f;

		local_a8[1][0] = s;
		local_a8[1][1] = c;
		local_a8[1][2] = 0.0f;
		local_a8[1][3] = 0.0f;

		local_a8[2][0] = 0.0f;
		local_a8[2][1] = 0.0f;
		local_a8[2][2] = 1.0f;
		local_a8[2][3] = 0.0f;
	} else if (mOwner->getAttackMode() == 2 || mOwner->getAttackMode() == 1) {
		static const f32 zangle[] = { 65.0f, 40.0f, -65.0f, -40.0f };
		f32 s                     = JMASin(zangle[mIndex]);
		f32 c                     = JMACos(zangle[mIndex]);

		local_a8[0][0] = c;
		local_a8[0][1] = -s;
		local_a8[0][2] = 0.0f;
		local_a8[0][3] = 0.0f;

		local_a8[1][0] = s;
		local_a8[1][1] = c;
		local_a8[1][2] = 0.0f;
		local_a8[1][3] = 0.0f;

		local_a8[2][0] = 0.0f;
		local_a8[2][1] = 0.0f;
		local_a8[2][2] = 1.0f;
		local_a8[2][3] = 0.0f;
	} else if (mOwner->getAttackMode() == 4) {
		static const f32 zangle[] = { 80.0f, 70.0f, -80.0f, -70.0f };
		f32 s                     = JMASin(zangle[mIndex]);
		f32 c                     = JMACos(zangle[mIndex]);

		local_a8[0][0] = c;
		local_a8[0][1] = -s;
		local_a8[0][2] = 0.0f;
		local_a8[0][3] = 0.0f;

		local_a8[1][0] = s;
		local_a8[1][1] = c;
		local_a8[1][2] = 0.0f;
		local_a8[1][3] = 0.0f;

		local_a8[2][0] = 0.0f;
		local_a8[2][1] = 0.0f;
		local_a8[2][2] = 1.0f;
		local_a8[2][3] = 0.0f;
	} else {
		static const f32 zangle[] = { 20.0f, 7.5f, -20.0f, -7.5f };
		f32 s                     = JMASin(zangle[mIndex]);
		f32 c                     = JMACos(zangle[mIndex]);

		local_a8[0][0] = c;
		local_a8[0][1] = -s;
		local_a8[0][2] = 0.0f;
		local_a8[0][3] = 0.0f;

		local_a8[1][0] = s;
		local_a8[1][1] = c;
		local_a8[1][2] = 0.0f;
		local_a8[1][3] = 0.0f;

		local_a8[2][0] = 0.0f;
		local_a8[2][1] = 0.0f;
		local_a8[2][2] = 1.0f;
		local_a8[2][3] = 0.0f;
	}

	MTXConcat(afStack_78, local_a8, afStack_78);
	unk80->getModel()->setBaseTRMtx(afStack_78);
	unk80->calcAnm();
	calcAtkParticleAndSE();
}

void TBGTentacle::resetAllNodes(const JGeometry::TVec3<f32>& param_1)
{
	JGeometry::TVec3<f32> local_78 = param_1;
	JGeometry::TVec3<f32> zero;
	zero.z = 0.0f;
	zero.y = 0.0f;
	zero.x = 0.0f;

	getFirstNode()->onUnk24();

	for (int i = 0; i < mNodeNum; ++i) {
		mNodes[i].setPosition(local_78);
		mNodes[i].setUnk18(local_78);
		mNodes[i].setVelocity(zero);
		local_78.y += getNodeLen();
	}

	unk2C->getModel()->setBaseScale(JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));

	unk80->setBckFromIndex(0x14);
	MtxPtr mtx = unk80->getModel()->getBaseTRMtx();
	MTXTrans(mtx, param_1.x, param_1.y, param_1.z);
	unk80->calc();
}

void TBGTentacle::perform(u32 cue, JDrama::TGraphics* graphics)
{
	mTakeHit->perform(cue, graphics);

	if (cue & CUE_MOVE) {
		// Uuuuh... the hit boxes move along the tentacle's spline,
		// looping back after reaching the end? Why?!
		for (int i = 0; i < 5; ++i) {
			f32 pos = mAttackHit[i]->getPosOnSpline() + 0.05f;
			if (pos > 0.8f)
				pos -= 0.8f;
			mAttackHit[i]->setPosOnSpline(pos);
		}
	}

	for (int i = 0; i < 5; ++i)
		mAttackHit[i]->perform(cue, graphics);

	if (cue & CUE_CALC_ANIM)
		calcAttackGuideAnm();

	if (cue & CUE_MOVE) {
		decideAtkColExists();
		moveConstraint();
		moveNode();
		decideOwnState();
		checkDamage();
		continuousRumble();

		++mTimeInCurrentState;
	}

	if (cue & CUE_CALC_ANIM)
		unk2C->getModel()->getModelData()->getJointNodePointer(0)->setMtxCalc(
		    mMtxCalc);

	if (cue & CUE_ENTRY) {
		unk2C->setLightData(mOwner->getGroundPlane(), mOwner->getPosition());

		if (mState == 4) {
			if (mTimeInCurrentState
			        >= mOwner->getSaveParam2()->getSLAmputeeTime() - 240
			    && mTimeInCurrentState % 6 >= 3) {
				cue &= ~CUE_ENTRY;
			}
		}
	}

	if ((cue & CUE_MOVE) && mState == 4
	    && mTimeInCurrentState
	           < mOwner->getSaveParam2()->getSLAmputeeTime() - 240) {
		SMSGetMSound()->startSoundActor(MSD_SE_BS_GESO_TAKEN_HAND,
		                                &mTakeHit->mPosition, 0, nullptr, 0, 4);
	}

	if (mState != 6)
		unk2C->perform(cue, graphics);
}
