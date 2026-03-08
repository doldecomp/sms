#include <Enemy/FireWanwan.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <Strategic/Strategy.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <Map/MapData.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/PollutionManager.hpp>
#include <Map/MapCollisionData.hpp>
#include <MoveBG/ItemManager.hpp>
#include <GC2D/GCConsole2.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/CameraShake.hpp>
#include <Enemy/Graph.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

namespace {
const GXColorS10 cBodyColorOnFire   = { 400, -50, -100, 0 };
const GXColorS10 cBodyColorOnCool   = { -50, -50, 50, 0 };
const GXColorS10 cBodyColorOnSilver = { 260, 260, 270, 0 };
} // namespace

namespace MyUtil {
template <class T> T value_lerp(const T& a, const T& b, f32 t)
{
	return a + (b - a) * t;
}
} // namespace MyUtil

static const char* fireWanwan_bastable[] = {
	nullptr,
	nullptr,
	"/scene/fireWanwan/bas/wanwan_return.bas",
	"/scene/fireWanwan/bas/wanwan_run.bas",
	nullptr,
	"/scene/fireWanwan/bas/wanwan_search.bas",
	"/scene/fireWanwan/bas/wanwan_wait.bas",
	"/scene/fireWanwan/bas/wanwan_walk.bas",
};

void TLerpControl::init(const GXColorS10& to, const GXColorS10& from,
                        f32 param_3)
{
	mStart   = from;
	mEnd     = to;
	mCurrent = from;
	unk18.init(param_3);
	unk18.setAttribute(J3DFrameCtrl::ATTR_ONCE);
	unk18.setRate(SMSGetAnmFrameRate());
}

void TLerpControl::update()
{
	unk18.update();
	f32 fVar1;
	if (unk18.checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE))
		fVar1 = 1.0f;
	else
		fVar1 = unk18.getFrame() / unk18.getEnd();

	mCurrent.r = MyUtil::value_lerp<s16>(mStart.r, mEnd.r, fVar1);
	mCurrent.g = MyUtil::value_lerp<s16>(mStart.g, mEnd.g, fVar1);
	mCurrent.b = MyUtil::value_lerp<s16>(mStart.b, mEnd.b, fVar1);
}

TTailRubber::TTailRubber(int count)
    : mFixHeadPos(true)
    , mFixTailPos(true)
    , mBoundRate(0.25f)
    , mDecay(0.5f)
{
	unk0.set(new Node[count], count);
}

void TTailRubber::reset(const JGeometry::TVec3<f32>& param_1,
                        const JGeometry::TVec3<f32>& param_2)
{
	setHeadPos(param_1);
	setTailPos(param_2);

	JGeometry::TVec3<f32> diff;
	diff.sub(param_2, param_1);

	for (int i = 1; i < unk0.size() - 1; ++i) {
		JGeometry::TVec3<f32> pos;
		pos.scaleAdd(f32(i) / f32(unk0.size()), param_1, diff);

		Node& node = unk0[i];
		node.mPos.set(pos);
		node.mVel.set(0.0f, 0.0f, 0.0f);
	}
}

void TTailRubber::setHeadPos(const JGeometry::TVec3<f32>& param_1)
{
	Node& node = unk0.front();
	node.mPos  = param_1;
	node.mVel.set(0.0f, 0.0f, 0.0f);
}

void TTailRubber::setTailPos(const JGeometry::TVec3<f32>& param_1)
{
	Node& node = *(unk0.end() - 1);
	node.mPos  = param_1;
	node.mVel.set(0.0f, 0.0f, 0.0f);
}

void TTailRubber::movement()
{
	adjust();
	restrict();
	bind();

	Node* end = unk0.end();
	for (Node *it = unk0.begin() + 1, *e = end - 1; it != e; ++it)
		it->mPos += it->mVel;

	if (!mFixHeadPos)
		unk0.front().mPos += unk0.front().mVel;

	if (!mFixTailPos)
		unk0.back().mPos += unk0.back().mVel;
}

void TTailRubber::bind()
{
	Node* end = unk0.end();
	for (Node *it = unk0.begin() + 1, *e = end - 1; it != e; ++it)
		bindOne(*it);
}

void TTailRubber::bindOne(Node& node)
{
	JGeometry::TVec3<f32> next = node.mPos;
	next += node.mVel;

	const TBGCheckData* checkData;
	f32 fVar1 = gpMap->checkGround(next.x, next.y + 40.0f, next.z, &checkData);
	fVar1 += 1.0f;
	fVar1 += 20.0f;
	if (next.y <= 0.05f + fVar1) {
		node.mVel.set(0.0f, 0.0f, 0.0f);
		next.y = fVar1;
	}

	gpMap->isTouchedOneWallAndMoveXZ(&next.x, next.y + 40.0f, &next.z, 20.0f);
	node.mVel = next - node.mPos;
}

void TTailRubber::adjust()
{
	for (Node *it = unk0.begin() + 1, *end = unk0.end(); it != end - 1; ++it)
		adjustOne(*it, *(it - 1), *(it + 1));

	Node fake;
	fake.mVel.zero();

	fake.mPos.set(unk0.front().mPos);
	adjustOne(unk0[0], fake, unk0[1]);

	fake.mPos.set(unk0.back().mPos);
	adjustOne(unk0[unk0.size() - 1], unk0[unk0.size() - 2], fake);
}

void TTailRubber::adjustOne(Node& curr, const Node& prev, const Node& next)
{
	JGeometry::TVec3<f32> diff = prev.mPos;
	diff -= curr.mPos;
	diff *= mBoundRate;

	JGeometry::TVec3<f32> diff2 = next.mPos;
	diff2 -= curr.mPos;
	diff2 *= mBoundRate;

	curr.mVel += diff;
	curr.mVel += diff2;
	curr.mVel *= mDecay;
}

void TTailRubber::restrict()
{
	f32 avgHorLen = getLength();

	if (avgHorLen <= mMaxLength)
		return;

	avgHorLen /= (f32)(unk0.size() - 1);

	if (mFixTailPos) {
		for (Node *e = unk0.begin() - 1, *it = unk0.end() - 2; it != e; --it) {
			JGeometry::TVec3<f32> diff = (it + 1)->mPos;
			diff -= it->mPos;
			diff.y = 0.0f;
			if (avgHorLen < diff.length()) {
				diff.setLength(diff.length() - avgHorLen);
				it->mPos += diff;
			}
		}
	} else {
		for (Node *it = unk0.begin() + 1, *e = unk0.end(); it != e; ++it) {
			JGeometry::TVec3<f32> diff = (it - 1)->mPos;
			diff -= it->mPos;
			diff.y = 0.0f;
			if (avgHorLen < diff.length()) {
				diff.setLength(diff.length() - avgHorLen);
				it->mPos += diff;
			}
		}
	}
}

f32 TTailRubber::getLength() const
{
	f32 total     = 0.0f;
	const Node* e = unk0.end() - 1;
	for (const Node* it = unk0.begin(); it != e; ++it) {
		JGeometry::TVec3<f32> diff = it->mPos;
		diff -= (it + 1)->mPos;
		diff.y = 0.0f;
		total += diff.length();
	}
	return total;
}

TFireWanwanSaveLoadParams::TFireWanwanSaveLoadParams(const char* path)
    : TSmallEnemyParams(path)
    , PARAM_INIT(mMarchSpeed, 3.0f)
    , PARAM_INIT(mAttackSpeedRate, 2.6f)
    , PARAM_INIT(mAttackTimerMax, 600)
    , PARAM_INIT(mEscapeSpeedRate, 1.0f)
    , PARAM_INIT(mEscapeRecoverTime, 600)
    , PARAM_INIT(mSLFlySpeed, 30.0f)
    , PARAM_INIT(mSLFlyGravityY, 5.0f)
    , PARAM_INIT(mAirFric, 0.99f)
    , PARAM_INIT(mFloorFric, 0.9f)
    , PARAM_INIT(mRotateY, 1.0f)
    , PARAM_INIT(mNoCollideTimerMax, 300)
    , PARAM_INIT(mStopSearchTimerMax, 600)
    , PARAM_INIT(mTailLengthToFly, 100.0f)
    , PARAM_INIT(mCamShakeRange, 1000.0f)
    , PARAM_INIT(mContShakeRange, 750.0f)
    , PARAM_INIT(mPolluteTimerMax, 20)
    , PARAM_INIT(mFreezeTimeThreshold, 50.0f)
    , PARAM_INIT(mSLFreezeWaitShort, 120)
    , PARAM_INIT(mRecoverRate, 0.1f)
    , PARAM_INIT(mPolluteNormalRate, 0.7f)
    , PARAM_INIT(mPolluteAttackRate, 1.5f)
    , PARAM_INIT(mTailEndPowRate, 0.06f)
    , PARAM_INIT(mRubberBoundRate, 0.25f)
    , PARAM_INIT(mRubberDecay, 0.5f)
    , PARAM_INIT(mRubberBoundRateHitting, 0.7f)
    , PARAM_INIT(mRubberDecayHitting, 0.6f)
    , PARAM_INIT(mTailEndColRange, 100.0f)
    , PARAM_INIT(mTailApartPowRate, 0.1f)
    , PARAM_INIT(mTailMaxLength, 300.0f)
    , PARAM_INIT(mBoss22HelpRange, 2000.0f)
    , PARAM_INIT(mBoss23TimerMax, 1200)
{
	TParams::load(mPrmPath);
}

TFireWanwanManager::TFireWanwanManager(const char* name)
    : TSmallEnemyManager(name)
    , mBoss22BalloonWasShown(false)
    , mShineAppeared(false)
    , unk64(0)
    , mWanwanRecoversBeforeHelpBalloon(3)
    , mWanwansKilled(0)
    , mLastKillerWanwan(nullptr)
{
}

void TFireWanwanManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TFireWanwanSaveLoadParams("/enemy/fireWanwan.prm");
}

void TFireWanwanManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "wanwan.bmd", 0x10210000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TFireWanwanManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TEnemyManager::perform(param_1, param_2);

	for (int i = 0; i < mObjNum; ++i) {
		TFireWanwan* wanwan = (TFireWanwan*)unk18[i];
		if (!gpMap->isInArea(wanwan->mPosition.x, wanwan->mPosition.z)
		    || (wanwan->getGroundPlane()
		        && wanwan->getGroundPlane()->isDeathPlane())) {
			wanwan->kill();
		}
	}

	if (param_1 & 1) {
		checkBalloonHelpBoss22();
		checkBalloonHelpBoss23();
		checkBalloonHelpBoss24();
		checkShineAppear();
	}
}

void TFireWanwanManager::checkBalloonHelpBoss22()
{
	if (mBoss22BalloonWasShown)
		return;

	TFireWanwan* wanwan = (TFireWanwan*)getNearestEnemy(SMS_GetMarioPos());
	if (wanwan) {
		JGeometry::TVec3<f32> diff = SMS_GetMarioPos();
		diff -= wanwan->getPosition();
		f32 helpRange22 = getWanwanParams()->mBoss22HelpRange.get();
		if (diff.squared() < helpRange22 * helpRange22) {
			mBoss22BalloonWasShown = true;
			gpMarDirector->getConsole()->startAppearBalloon(0xE0016, true);
		}
	}
}

void TFireWanwanManager::checkBalloonHelpBoss23()
{
	if (mWanwansKilled <= 0 && unk64 != 0
	    && getWanwanParams()->mBoss23TimerMax.get() > unk64) {
		if (++unk64 >= getWanwanParams()->mBoss23TimerMax.get()) {
			unk64 = getWanwanParams()->mBoss23TimerMax.get();
			gpMarDirector->getConsole()->startAppearBalloon(0xE0017, true);
		}
	}
}

void TFireWanwanManager::checkBalloonHelpBoss24()
{
	if (mWanwanRecoversBeforeHelpBalloon == 0) {
		gpMarDirector->getConsole()->startAppearBalloon(0xE0018, true);
		mWanwanRecoversBeforeHelpBalloon = -1;
	}
}

void TFireWanwanManager::checkShineAppear()
{
	if (mShineAppeared)
		return;

	if (mWanwansKilled == getActiveObjNum() && gpMarDirector->unk124 != 3
	    && gpMarDirector->getConsole()->unk10 == 0) {
		mShineAppeared = true;
		gpItemManager->makeShineAppearWithDemo(
		    "シャイン（ボス用）", "ボスシャインカメラ",
		    mLastKillerWanwan->mPosition.x,
		    mLastKillerWanwan->mPosition.y + 500.0f,
		    mLastKillerWanwan->mPosition.z);
	}
}

void TFireWanwanManager::receiveMessageFromTail(int) { }

void TFireWanwanManager::receiveMessageFromBody(const TFireWanwan* wanwan,
                                                BodyMsgType msg)
{
	switch (msg) {
	case BODY_MSG_RECOVERED:
		if (mWanwanRecoversBeforeHelpBalloon > 0)
			mWanwanRecoversBeforeHelpBalloon -= 1;
		break;
	}
}

TFireWanwanTailNode::TFireWanwanTailNode(MActor* actor)
    : mMActor(actor)
    , mScale(1.0f, 1.0f, 1.0f)
    , unk10(0)
    , mJointIdx(
          (u16)mMActor->getModel()->getModelData()->getJointName()->getIndex(
              "tail_null1"))
{
}

void TFireWanwanTailNode::setBarAnmMtx(MtxPtr mtx)
{
	mMActor->getModel()->setAnmMtx(mJointIdx, mtx);
}

#pragma dont_inline on
void TFireWanwanTailNode::perform(u32 param_1, JDrama::TGraphics* param_2,
                                  const JGeometry::TVec3<f32>& param_3,
                                  const JGeometry::TVec3<f32>& param_4)
{
	if (param_1 & 2) {
		TPosition3f mtx;

		SMS_CalcToDirMatrix(mtx, param_4,
		                    JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f));

		mtx.setTrans(param_3);

		mMActor->getModel()->setBaseScale(mScale);
		mMActor->getModel()->setBaseTRMtx(mtx);
		mMActor->getModel()->calc();
	}

	if (!(unk10 & 0x4))
		mMActor->perform(param_1, param_2);
}
#pragma dont_inline off

TFireWanwanTailHit::TFireWanwanTailHit(TFireWanwan& param_1)
    : TTakeActor("ファイヤーワンワン尻尾当たり")
    , mOwner(&param_1)
{
	MTXIdentity(unk74);
}

BOOL TFireWanwanTailHit::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_1->getActorType() == 0x80000001) {
		if (param_2 == HIT_MESSAGE_TAKE) {
			if (!mOwner->canTakenByMario())
				return false;

			behaveTaken(param_1);
			return true;
		}

		if (param_2 == HIT_MESSAGE_UNK7 || param_2 == HIT_MESSAGE_UNK8) {
			behaveApart();
			return true;
		}
	}

	return false;
}

void TFireWanwanTailHit::behaveTaken(THitActor* param_1)
{
	mHolder            = (TTakeActor*)param_1;
	unkA4->mFixTailPos = true;
	if (gpMSound->gateCheck(0x28DE))
		MSoundSESystem::MSoundSE::startSoundActor(0x28DE, &mPosition, 0,
		                                          nullptr, 0, 4);
	((TFireWanwanManager*)mOwner->getManager())->unk64 = 1;

	mCurTailLength  = unkA4->getLength();
	mPrevTailLength = mCurTailLength;
	moveRequest(mPosition);
}

void TFireWanwanTailHit::behaveApart()
{
	unkA4->mFixTailPos = false;

	((TFireWanwanManager*)mOwner->getManager())->unk64 = 0;
	mOwner->startThrownSound();
	mThrowPow       = calcApartPow();
	mCurTailLength  = 0.0f;
	mPrevTailLength = 0.0f;
	mHolder         = nullptr;
}

void TFireWanwanTailHit::init()
{
	unkA4      = new TTailRubber(5);
	MtxPtr mtx = mOwner->getTailMtx();

	JGeometry::TVec3<f32> pos;
	pos.x = mtx[0][3];
	pos.y = mtx[1][3];
	pos.z = mtx[2][3];
	unkA4->reset(pos, pos);

	unkA4->mFixHeadPos = true;
	unkA4->mFixTailPos = false;

	TMActorKeeper* keeper = mOwner->getActorKeeper();

	unkBC = new TLerpControl;

	unkBC->setCurrent(cBodyColorOnFire);

	for (int i = 0; i < 5; ++i) {
		MActor* actor = keeper->createMActor("wanwanTail.bmd", 3);
		SMS_InitPacket_OneTevColor(actor->getModel(), 0, GX_TEVREG0,
		                           &unkBC->getCurrent());
		unkA8[i] = new TFireWanwanTailNode(actor);

		JGeometry::TVec3<f32> local_78(mOwner->getBodyScale());
		if (i == 4)
			local_78 *= 2.0f;

		unkA8[i]->setScale(local_78);

		TPosition3f mtx;
		mtx.translation(unkA4->getNode(i)->mPos);
		actor->getModel()->setBaseTRMtx(mtx);
		actor->getModel()->setBaseScale(local_78);
		actor->getModel()->calc();
	}

	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);
	initHitActor(0x10000028, 0, 0, 0.0f, 0.0f, 30.0f, 200.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	onHitFlag(HIT_FLAG_UNK2);
	mIsOnFire = false;
}

void TFireWanwanTailHit::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	THitActor::perform(param_1, param_2);

	setDamageRadius(mOwner->getSaveParam2()->mTailEndColRange.get());

	if (param_1 & 1) {
		MtxPtr mtx = mOwner->getTailMtx();
		movementBody(JGeometry::TVec3<f32>(mtx[0][3], mtx[1][3], mtx[2][3]));
	}

	performNodes(param_1, param_2);

	{
		MtxPtr mtx = unkA8[4]->mMActor->getModel()->getBaseTRMtx();
		JSGSetTranslation(JGeometry::TVec3<f32>(
		    mtx[0][3], mtx[1][3] - mDamageHeight * 0.5f, mtx[2][3]));
	}

	if ((param_1 & 2) && mIsOnFire)
		onFireEffect();

	if (param_1 & 2)
		unkBC->update();

	if (mHolder != nullptr) {
		JGeometry::TVec3<f32> local_3c = unkA4->getNode(4)->mPos;
		local_3c -= mOwner->mPosition;
		SMS_CalcToDirMatrix(unk74, local_3c,
		                    JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f));

		JGeometry::TVec3<f32> local_48 = unkA4->unk0.back().mVel;
		f32 fVar1 = mOwner->getSaveParam2()->mTailEndPowRate.get();
		local_48 *= fVar1;
		local_48.y = 0.0f;
		mPosition += local_48;

		moveRequest(mPosition);

		unk74.setTrans(mPosition);

		mPrevTailLength = mCurTailLength;
		mCurTailLength  = unkA4->getLength();

		if (mCurTailLength - mPrevTailLength > 0.0f) {
			JGeometry::TVec3<f32> local_60 = SMS_GetMarioPos();
			local_60 -= mOwner->mPosition;
			f32 fVar1 = local_60.length();

			// TODO: this is definitely an inline!!! but where???
			JGeometry::TVec3<f32>* pos = &unkA4->unk0[2].mPos;
			if (gpMSound->gateCheck(0x20DF))
				MSoundSESystem::MSoundSE::startSoundActorWithInfo(
				    0x20DF, pos, nullptr, fVar1, 0, 0, nullptr, 0, 4);
		}
	}
}

void TFireWanwanTailHit::performNodes(u32 param_1, JDrama::TGraphics* param_2)
{
	for (int i = 0; i < 5; ++i) {
		JGeometry::TVec3<f32> local_68 = unkA4->getNode(i)->mPos;
		JGeometry::TVec3<f32> local_74;
		if (i == 4) {
			local_74.set(0.0f, 0.0f, 1.0f);
		} else {
			local_74.set(unkA4->getNode(i + 1)->mPos);
			local_74 -= local_68;
			local_74.normalize();
		}
		unkA8[i]->perform(param_1, param_2, local_68, local_74);
	}

	for (int i = 0; i < 5; ++i) {
		TPosition3f afStack_a4;
		MTXCopy(afStack_a4, unkA8[i]->mMActor->getModel()->getBaseTRMtx());

		afStack_a4.setTrans(unkA4->getNode(i + 1)->mPos);

		unkA8[i]->setBarAnmMtx(afStack_a4);
	}
}

void TFireWanwanTailHit::clipNodes(JDrama::TGraphics*) { }

void TFireWanwanTailHit::movementBody(const JGeometry::TVec3<f32>& param_1)
{
	if (mOwner->isHungTailNerve() && !mOwner->unk194->isTaken()
	    && !mOwner->isReadyToFly()) {
		unkA4->mBoundRate
		    = mOwner->getSaveParam2()->mRubberBoundRateHitting.get();
		unkA4->mDecay = mOwner->getSaveParam2()->mRubberDecayHitting.get();
	} else if (mOwner->isAttacking()) {
		unkA4->mBoundRate = 0.7f;
		unkA4->mDecay     = 0.4f;
	} else {
		unkA4->mBoundRate = mOwner->getSaveParam2()->mRubberBoundRate.get();
		unkA4->mDecay     = mOwner->getSaveParam2()->mRubberDecay.get();
	}

	if (mOwner->isFlying())
		unkA4->mMaxLength = mOwner->getSaveParam2()->mTailMaxLength.get();
	else
		unkA4->mMaxLength = 10000.0f;

	unkA4->unk0[0].mPos = param_1;
	unkA4->movement();
}

void TFireWanwanTailHit::bindBody() { }

JGeometry::TVec3<f32> TFireWanwanTailHit::getBodyNthPos(int i) const { }

f32 TFireWanwanTailHit::getBodyTailPow() const { }

f32 TFireWanwanTailHit::getBodyHeadPow() const { }

f32 TFireWanwanTailHit::calcApartPow()
{
	f32 fVar3 = mOwner->getSaveParam2()->mTailApartPowRate.get()
	            * unkA4->getLength()
	            / mOwner->getSaveParam2()->mSLFlySpeed.get();
	f32 fVar1 = fVar3 < 0.0f ? 0.0f : fVar3 > 1.0f ? 1.0f : fVar3;
	return fVar1 * fVar1 * mOwner->getSaveParam2()->mSLFlySpeed.get();
}

MtxPtr TFireWanwanTailHit::getTakingMtx() { return unk74; }

bool TFireWanwanTailHit::moveRequest(const JGeometry::TVec3<f32>& param_1)
{
	JGeometry::TVec3<f32> next = param_1;

	const TBGCheckData* checkData;
	f32 fVar1 = gpMap->checkGround(next.x, next.y + 100.0f, next.z, &checkData);
	if (next.y <= 0.05f + fVar1) {
		next.y = fVar1 + 1.0f;
	}

	next.y += 50.0f;

	gpMap->isTouchedOneWallAndMoveXZ(&next.x, next.y, &next.z, 70.0f);
	unkA4->unk0.back().mPos = next;
	mPosition               = next;
	unk74.translation(next);
	return true;
}

void TFireWanwanTailHit::onFireEffect()
{
	u8 maxHp = mOwner->getMaxHitPoints();

	JGeometry::TVec3<f32> scaleVec(mOwner->getBodyScale()
	                               * mOwner->getHitPoints() / maxHp);

	MtxPtr mtx = unkA8[4]->mMActor->getModel()->getBaseTRMtx();
	SMS_EasyEmitParticle(PARTICLE_MS_MOE_FIRE_C, mtx, this, scaleVec);
	SMS_EasyEmitParticle(PARTICLE_MS_MOE_FIRE_A, mtx, this, scaleVec);
	SMS_EasyEmitParticle(PARTICLE_MS_MOE_FIRE_B, mtx, this, scaleVec);
	SMS_EasyEmitParticle(PARTICLE_MS_MOE_FIRE_D, mtx, this, scaleVec);

	if (gpMSound->gateCheck(0x20AB))
		MSoundSESystem::MSoundSE::startSoundActor(0x20AB, &mPosition, 0,
		                                          nullptr, 0, 4);
}

void TFireWanwanTailHit::offFireEffect()
{
	JGeometry::TVec3<f32> scale(mOwner->mBodyScale);
	MtxPtr mtx = unkA8[4]->mMActor->getModel()->getBaseTRMtx();
	SMS_EasyEmitParticle(PARTICLE_MS_MOE_FIRE_OFF, mtx, this, scale);
}

void TFireWanwanTailHit::changeBodyToRed(f32 param_1)
{
	unkBC->init(cBodyColorOnFire, unkBC->getCurrent(), param_1);
}

void TFireWanwanTailHit::changeBodyToBlack(f32 param_1)
{
	unkBC->init(cBodyColorOnCool, unkBC->getCurrent(), param_1);
}

void TFireWanwanTailHit::changeBodyToSilver(f32 param_1)
{
	unkBC->init(cBodyColorOnSilver, unkBC->getCurrent(), param_1);
}

f32 TFireWanwanTailHit::getTailLength() const { }

const JGeometry::TVec3<f32>& TFireWanwanTailHit::getHostPos() const
{
	return mOwner->getPosition();
}

u8 TFireWanwan::mTailJntIndex = 5;

TFireWanwan::TFireWanwan(const char* name)
    : TSmallEnemy(name)
    , unk194(nullptr)
    , mNoCollideTimer(0)
    , mApproachRumbleTimer(0)
    , mHungTailRumbleTimer(0)
    , unk1B8(nullptr)
{
}

void TFireWanwan::init(TLiveManager* manager)
{
	TSmallEnemy::init(manager);
	mActorType = 0x1000000E;
	unk150     = 1;
	mSpine->initWith(&TNerveFireWanwanGraphWander::theNerve());
	TPosition3f mtx;
	mtx.translation(mPosition);
	getModel()->setBaseTRMtx(mtx);
	getModel()->setBaseScale(mScaling);
	getModel()->calc();

	unk194 = new TFireWanwanTailHit(*this);
	unk194->init();
	unk238           = new TLerpControl;
	mInitialPosition = mPosition;

	initParticle();

	mCenterJointIdx
	    = getModel()->getModelData()->getMaterialName()->getIndex("center");
	mHeadJointIdx
	    = getModel()->getModelData()->getMaterialName()->getIndex("jnt_head");
	int idx
	    = getModel()->getModelData()->getMaterialName()->getIndex("_mat_body");
	SMS_InitPacket_OneTevColor(mMActor->getModel(), idx, GX_TEVREG0,
	                           &unk238->getCurrent());
	reset();
}

void TFireWanwan::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 7);
	mMActor       = mMActorKeeper->createMActor("wanwan.bmd", 0);
}

void TFireWanwan::reset()
{
	mPosition = mInitialPosition;

	unk194->mIsOnFire = true;
	unk124->reset();
	goToShortestNextGraphNode();
	TSmallEnemy::reset();
	mPolluteTimer = 0;
	mRecoverTimer = 0.0f;
	TFireWanwanSaveLoadParams* params
	    = (TFireWanwanSaveLoadParams*)getSaveParam();
	mMarchSpeed = params->mMarchSpeed.get();
	mSpine->reset();
	mSpine->setDefaultNext();
	unk238->setCurrent(cBodyColorOnFire);
	unk238->init(cBodyColorOnFire, unk238->getCurrent(), 1.0f);
	unk194->unkBC->init(cBodyColorOnFire, unk194->unkBC->getCurrent(), 1.0f);
}

void TFireWanwan::initParticle()
{
	SMS_LoadParticle("/scene/firewanwan/jpa/ms_can_yugami.jpa", 0x1F4);
	SMS_LoadParticle("/scene/firewanwan/jpa/ms_can_hityuge.jpa", 0x18F);
	SMS_LoadParticle("/scene/firewanwan/jpa/ms_bwan_kira.jpa", 0x168);
}

void TFireWanwan::doWalk()
{
	// TODO: are these 3 things in doWalk, doEscape & doAttack an inline?
	if (mSprayedByWaterCooldown != 0) {
		MActor* mActor = getMActor();

		if (!mActor->checkCurBckFromIndex(1))
			mActor->setBckFromIndex(1);

		setCurAnmSound();
	}

	if (checkCurAnmEnd(0))
		setBckAnm(7);

	walkToCurPathNode(mMarchSpeed, mTurnSpeed, 0.0f);
}

void TFireWanwan::doEscape()
{
	// TODO: inline?
	if (mSprayedByWaterCooldown != 0) {
		MActor* mActor = getMActor();

		if (!mActor->checkCurBckFromIndex(1))
			mActor->setBckFromIndex(1);

		setCurAnmSound();
	}

	if (checkCurAnmEnd(0))
		setBckAnm(0);

	walkToCurPathNode(mMarchSpeed * getSaveParam2()->mEscapeSpeedRate.get(),
	                  mTurnSpeed, 0.0f);
}

// TODO: incorrect size + improbable pattern?..
bool TFireWanwan::doAttack()
{
	// TODO: inline?
	if (mSprayedByWaterCooldown != 0) {
		MActor* mActor = getMActor();

		if (!mActor->checkCurBckFromIndex(1))
			mActor->setBckFromIndex(1);

		setCurAnmSound();
	} else {
		walkToCurPathNode(mMarchSpeed * getSaveParam2()->mAttackSpeedRate.get(),
		                  mTurnSpeed, 0.0f);
	}

	if (checkCurAnmEnd(0))
		setBckAnm(3);

	return isMissMario();
}

static bool is_antiparallel(const JGeometry::TVec3<f32>& v1,
                            const JGeometry::TVec3<f32>& v2)
{
	f32 fVar8 = v1.dot(v2) - -1.0f;
	f32 eps   = JGeometry::TUtil<f32>::epsilon();
	return -eps <= fVar8 && fVar8 <= eps;
}

void TFireWanwan::decideTarget(const JGeometry::TVec3<f32>& param_1)
{
	JGeometry::TVec3<f32> local_54 = param_1;
	local_54 -= mPosition;

	JGeometry::TVec3<f32> local_2C = local_54;

	local_54.y = 0.0f;

	local_54.normalize();

	if (is_antiparallel(local_2C, JGeometry::TVec3<f32>(0.0f, 0.0f, 1.0f))) {
		unk1CC.setEulerY(JGeometry::TUtil<f32>::PI());
	} else {
		unk1CC.setRotate(JGeometry::TVec3<f32>(0.0f, 0.0f, 1.0f), local_54,
		                 1.0f);
	}

	unk1BC.setEulerY(DEG_TO_RAD(mRotation.y));
}

void TFireWanwan::doAdjustTarget()
{
	J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(0);

	f32 fVar8 = JGeometry::TUtil<f32>::clamp(ctrl->getFrame() / ctrl->getEnd(),
	                                         0.0f, 1.0f);

	JGeometry::TQuat4<f32> local_70;
	local_70.slerp(unk1BC, unk1CC, fVar8);
	local_70.normalize();

	JGeometry::TVec3<f32> local_60(0.0f, 0.0f, 1.0f);

	local_70.rotate(local_60);

	f32 rot = MsGetRotFromZaxisY(local_60);

	mRotation.y = MsAngleWrap(rot);
}

int TFireWanwan::getFreezeTime() const { return mFreezeWait + 75; }

bool TFireWanwan::isFindMario(f32 param_1)
{
	if (mStopSearchTimer > 0)
		return false;

	if (isMarioInWater())
		return false;

	return isFindMarioFromParam(param_1);
}

static inline f32 dist(const JGeometry::TVec3<f32>& a,
                       const JGeometry::TVec3<f32>& b)
{
	JGeometry::TVec3<f32> tmp = a;
	tmp.sub(b);
	return tmp.length();
}

bool TFireWanwan::isMissMario() const
{
	if (SMS_CheckMarioFlag(0x2) || SMS_CheckMarioFlag(0x10000)
	    || SMS_GetMarioGroundPlane()->isWaterSurface()
	    || SMS_CheckMarioFlag(0x20000))
		return true;

	f32 giveUpHeight = getSaveParam2()->mSLGiveUpHeight.get();
	if (abs(SMS_GetMarioPos().y - mPosition.y) > giveUpHeight)
		return true;

	f32 giveUpLen = getSaveParam2()->mSLGiveUpLength.get();
	if (dist(unk104.getPoint(), mPosition) > giveUpLen)
		return true;

	return false;
}

bool TFireWanwan::isOverApproachRumble() const
{
	return mApproachRumbleTimer > 3600;
}

bool TFireWanwan::isOverHungTailRumble() const
{
	return mHungTailRumbleTimer > 3600;
}

BOOL TFireWanwan::receiveMessage(THitActor* param_1, u32 param_2)
{
	switch (param_2) {
	case HIT_MESSAGE_TRAMPLE:
	case HIT_MESSAGE_HIP_DROP:
		return false;

	case HIT_MESSAGE_SPRAYED_BY_WATER: {
		SMS_EasyEmitParticle(PARTICLE_MS_ENM_WATHIT, &param_1->getPosition(),
		                     nullptr, JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
		u8 maxHp = getMaxHitPoints();
		if (maxHp == mHitPoints)
			if (gpMSound->gateCheck(0x290F))
				MSoundSESystem::MSoundSE::startSoundActor(0x290F, &mPosition, 0,
				                                          nullptr, 0, 4);
		decHpByWater(param_1);
		behaveToWater(param_1);
		if (mSprayedByWaterCooldown == 0)
			mSprayedByWaterCooldown = 1;
		return true;
	}

	default:
		return TSmallEnemy::receiveMessage(param_1, param_2);
	}
}

void TFireWanwan::behaveToWater(THitActor* param_1)
{
	if (!unk194->mIsOnFire) {
		if (mSprayedByWaterCooldown == 0) {
			if (gpMSound->gateCheck(0x28D1))
				MSoundSESystem::MSoundSE::startSoundActor(0x28D1, &mPosition, 0,
				                                          nullptr, 0, 4);
			if (!isRecovering()) {
				JGeometry::TVec3<f32> scale = mScaling;
				scale *= 0.75f;
				SMS_EasyEmitParticle(PARTICLE_MS_CHO_ASE, &mPosition, this,
				                     scale);
			}
		}

		return;
	}

	if (mHitPoints != 0) {
		if (mSprayedByWaterCooldown == 0)
			if (gpMSound->gateCheck(0x28C4))
				MSoundSESystem::MSoundSE::startSoundActor(0x28C4, &mPosition, 0,
				                                          nullptr, 0, 4);
		if (isWandering()) {
			mSpine->reset();
			mSpine->setNext(&TNerveFireWanwanFindMario::theNerve());
		}

		return;
	}

	if (gpMSound->gateCheck(0x28BD))
		MSoundSESystem::MSoundSE::startSoundActor(0x28BD, &mPosition, 0,
		                                          nullptr, 0, 4);
	mSpine->reset();
	mSpine->setNext(&TNerveFireWanwanEscape::theNerve());
	unk194->offFireEffect();
	unk194->mIsOnFire       = false;
	mSprayedByWaterCooldown = 20;
}

void TFireWanwan::behaveHitComrades()
{
	if (!isWalking() || mNoCollideTimer > 0)
		return;

	mNoCollideTimer = getSaveParam2()->mNoCollideTimerMax.get();
	mSpine->reset();
	mSpine->setNext(&TNerveFireWanwanTurn::theNerve());
}

void TFireWanwan::changeBodyToRed(f32 param_1)
{
	unk238->init(cBodyColorOnFire, unk238->getCurrent(), param_1);
	unk194->changeBodyToRed(param_1);
}

void TFireWanwan::changeBodyToBlack(f32 param_1)
{
	unk238->init(cBodyColorOnCool, unk238->getCurrent(), param_1);
	unk194->changeBodyToBlack(param_1);
}

void TFireWanwan::changeBodyToSilver(f32 param_1)
{
	unk238->init(cBodyColorOnSilver, unk238->getCurrent(), param_1);
	unk194->changeBodyToSilver(param_1);
}

void TFireWanwan::startThrownSound()
{
	if (gpMSound->gateCheck(0x28E0))
		MSoundSESystem::MSoundSE::startSoundActor(0x28E0, mPosition, 0, &unk1B8,
		                                          0, 4);
}

void TFireWanwan::stopTriggerSound()
{
	if (unk1B8)
		unk1B8->stop(1);
}

void TFireWanwan::checkInPond()
{
	if (!checkLiveFlag(LIVE_FLAG_AIRBORNE) && !mGroundPlane->isIllegalData()
	    && (mGroundPlane->isPool() || mGroundPlane->isWaterSurface())
	    && mPosition.y - mGroundHeight < mHeadHeight)
		kill();
}

void TFireWanwan::kill()
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	if (isDefeat()) {
		(void)mSpine;
	} else {
		mSpine->reset();
		mSpine->setNext(&TNerveFireWanwanDie::theNerve());
	}
}

bool TFireWanwan::isHitValid(u32) { return false; }

void TFireWanwan::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TSmallEnemy::perform(param_1, param_2);
	if (!(param_1 & 2)) {
		calcRootMatrix();
		mMActor->calc();
	}

	if (param_1 & 2) {
		emitEffects();
		unk238->update();
	}

	if (!checkLiveFlag(LIVE_FLAG_DEAD))
		unk194->perform(param_1, param_2);
}

void TFireWanwan::calcRootMatrix()
{
	TSpineEnemy::calcRootMatrix();
	if (mSpine->getLatestNerve() != &TNerveFireWanwanDie::theNerve())
		return;
	{
		MtxPtr mtx = getModel()->getBaseTRMtx();
		JGeometry::TVec3<f32> v1(mtx[0][1], mtx[1][1], mtx[2][1]);
		JGeometry::TVec3<f32> v2(mtx[0][2], mtx[1][2], mtx[2][2]);
		v1.normalize();
		v2.normalize();
		v1 *= 0.0f;
		v2 *= -160.0f;
		v1 += v2;

		mtx[0][3] += v1.x;
		mtx[1][3] += v1.y;
		mtx[2][3] += v1.z;
	}
}

void TFireWanwan::moveObject()
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	if (mNoCollideTimer > 0)
		mNoCollideTimer -= 1;

	if (mStopSearchTimer > 0)
		mStopSearchTimer -= 1;

	updateSquareToMario();
	updateCollisionFromParam();
	updateCameraShake();
	updateRumble();
	updatePollute();
	updateHitPoint();
	checkHungTail();

	mLinearVelocity.zero();
	mAngularVelocity.zero();

	control();

	if (!isInhibitedForceMove())
		calcRideMomentum();

	checkHitActors();
	bind();

	mGroundHeight = gpMap->checkGround(mPosition.x, mPosition.y + mHeadHeight,
	                                   mPosition.z, &mGroundPlane);

	checkInPond();
	setBehavior();

	mPosition += mLinearVelocity;
	mRotation += mAngularVelocity;

	if (mSprayedByWaterCooldown > 0)
		mSprayedByWaterCooldown += 1;

	if (mSprayedByWaterCooldown > 30)
		mSprayedByWaterCooldown = 0;

	if (mMapCollisionManager && mMapCollisionManager->unk8)
		mMapCollisionManager->unk8->moveSRT(mPosition, mRotation, mScaling);

	if (!isInhibitedForceMove())
		calcRidePos();
}

// Tiny size mismatch
void TFireWanwan::updateCollisionFromParam()
{
	if (isDefeat())
		return;

	setHitParams(getSaveParam2()->mSLAttackRadius.get(),
	             getSaveParam2()->mSLAttackHeight.get(),
	             getSaveParam2()->mSLDamageRadius.get(),
	             getSaveParam2()->mSLDamageHeight.get());
}

// Tiny size mismatch
void TFireWanwan::updateCameraShake()
{
	f32 shakeRange = getSaveParam2()->mCamShakeRange.get();
	if (!isWalking() && !isAttacking()) {
		gpCameraShake->keepShake(CAM_SHAKE_MODE_UNK3, 0.5f);
	}
}

void TFireWanwan::updateRumble()
{
	f32 fVar1 = getSaveParam2()->mContShakeRange.get();

	if (!isCameraShake() && mDistToMarioSquared < fVar1 * fVar1) {
		if (!isOverApproachRumble()) {
			SMSRumbleMgr->start(9, &mPosition);
			mApproachRumbleTimer += 1;
		}
	} else {
		mApproachRumbleTimer = 0;
	}

	if (unk194->isTaken()) {
		if (!isOverHungTailRumble()) {
			SMSRumbleMgr->start(9, (f32*)nullptr);
			mHungTailRumbleTimer += 1;
		}
	} else {
		mHungTailRumbleTimer = 0;
	}
}

void TFireWanwan::updatePollute()
{
	if (!unk194->mIsOnFire)
		return;

	if (mPolluteTimer != 0) {
		mPolluteTimer -= 1;
		return;
	}

	mPolluteTimer = getSaveParam2()->mPolluteTimerMax.get();
	MtxPtr mtx    = getModel()->getBaseTRMtx();
	JGeometry::TVec3<f32> v1(mtx[0][0], mtx[1][0], mtx[2][0]);
	v1.scaleAdd((MsRandF() - 0.5f) * 2.0f * mAttackRadius, mPosition, v1);

	f32 radius = 375.0f;
	if (isAttacking())
		radius *= getSaveParam2()->mPolluteAttackRate.get();
	else
		radius *= getSaveParam2()->mPolluteNormalRate.get();

	gpPollution->stamp(1, v1.x, v1.y, v1.z, radius);
}

void TFireWanwan::updateHitPoint()
{
	if (unk194->mIsOnFire) {
		mRecoverTimer += getSaveParam2()->mRecoverRate.get();
		if (1.0f <= mRecoverTimer) {
			mRecoverTimer -= 1.0f;
			mHitPoints = JGeometry::TUtil<u32>::clamp((u8)(mHitPoints + 1), 0,
			                                          getMaxHitPoints());
		}
	}
	ensureTakeSituation();
}

void TFireWanwan::emitEffects()
{
	MtxPtr mtx = getModel()->getAnmMtx(mCenterJointIdx);
	unk1F0.set(mtx[0][3], mtx[1][3], mtx[2][3]);

	if (mHitPoints != 0 && unk194->mIsOnFire) {
		SMS_EasyEmitParticle(FIREWANWAN_JPA_MS_CAN_YUGAMI, &unk1F0, this,
		                     mScaling);
	}

	if (mHitPoints != 0 && mSprayedByWaterCooldown != 0)
		SMS_EasyEmitParticle(FIREWANWAN_JPA_MS_CAN_HITYUGE, &unk1F0, this,
		                     mScaling);

	if (isFreeze() && mSpine->getTime() < mFreezeWait)
		SMS_EasyEmitParticle(PARTICLE_MS_POI_KIZETSU, &unk1F0, this, mScaling);

	if (isFlying() || unk194->isTaken()) {
		MtxPtr pos;
		if (isFlying()) {
			pos = getModel()->getBaseTRMtx();
		} else {
			pos = unk1FC;
			MTXCopy(getModel()->getBaseTRMtx(), pos);
			JGeometry::TVec3<f32> thing(pos[0][2], pos[1][2], pos[2][2]);
			thing.normalize();
			thing *= 150.0f;
			pos[0][3] += thing.x;
			pos[1][3] += thing.y;
			pos[2][3] += thing.z;
		}

		if (JPABaseEmitter* emitter = SMS_EasyEmitParticle(
		        PARTICLE_MS_M_SLIPSMOKE, pos, this, mScaling))
			emitter->unk180.a = 179;
	}

	if (isRecovering()) {
		JGeometry::TVec3<f32> local_40 = mScaling;
		local_40 *= 1.8f;
		SMS_EasyEmitParticle(PARTICLE_MS_HIKAGE1_A, &mPosition, this, local_40);
	}

	if (isDefeat()) {
		calcRipplePos();

		if (mSpine->getTime() < 600)
			SMS_EasyEmitParticle(FIREWANWAN_JPA_MS_CAN_HITYUGE, &mRipplePos,
			                     this, mScaling);

		JGeometry::TVec3<f32> local_4c = mScaling;
		local_4c *= 0.22f;
		if (JPABaseEmitter* emitter = SMS_EasyEmitParticle(
		        BWANWAN_JPA_MS_BWAN_KIRA, mtx, this, local_4c))
			emitter->unk1B0 = 0.1f;
		local_4c.set(1.5f, 1.5f, 1.5f);
		SMS_EasyEmitParticle(PARTICLE_MS_NPC_HAMON_B, &mRipplePos, this,
		                     local_4c);
		SMS_EasyEmitParticle(PARTICLE_MS_NPC_HAMON_A, &mRipplePos, this,
		                     local_4c);
	}
}

// TODO: wrong, unused arg + too small
void TFireWanwan::emitEffectsOnHittingWall(
    const JGeometry::TVec3<f32>& local_54, const JGeometry::TVec3<f32>&)
{
	JGeometry::TQuat4<f32> local_d8;

	if (is_antiparallel(local_54, JGeometry::TVec3<f32>(0.0f, 0.0f, -1.0f))) {
		local_d8.setEulerZ(JGeometry::TUtil<f32>::PI());
	} else {
		local_d8.setRotate(JGeometry::TVec3<f32>(0.0f, 0.0f, -1.0f), local_54,
		                   1.0f);
	}

	TPosition3f transform;
	transform.setQT(local_d8, mPosition);

	if (JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtx(
	        PARTICLE_MS_WALLKICK_A, transform.mMtx, 0, nullptr))
		emitter->setScale(mScaling);

	if (JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtx(
	        PARTICLE_MS_WALLKICK_B, transform.mMtx, 0, nullptr))
		emitter->setScale(mScaling);
}

void TFireWanwan::checkHitActors()
{
	for (THitActor **it = mCollisions, **e = mCollisions + mColCount; it != e;
	     ++it) {
		if (*it == this)
			continue;

		if ((*it)->isActorType(0x80000001)) {
			attackToMario();
			continue;
		}

		if ((*it)->isActorType(0x10000028))
			continue;

		if ((*it)->isActorType(0x1000000E)) {
			behaveToHitOthers((*it));
			behaveHitComrades();

			static_cast<TFireWanwan*>(*it)->behaveHitComrades();
			continue;
		}

		behaveToHitOthers((*it));
	}
}

void TFireWanwan::checkHungTail()
{
	if (!canTakenByMario() || unk194->isTaken()) {
		mSpine->reset();
		mSpine->setNext(&TNerveFireWanwanHungTail::theNerve());
	}
}

void TFireWanwan::emitTailHitEffect() { }

// correct but popCurr is incorrect
void TFireWanwan::initTurnNextGraphNode()
{
	unk124->mCurrIdx = unk124->popCurr();
	setGoalPathFromGraph();
	unk128 = 0;
	unk12C = 0.0f;
}

void TFireWanwan::initEscapeNextGraphNode()
{
	u32 uVar1 = unk124->unk0->findNearestNodeIndex(mPosition, -1);
	u32 uVar2 = unk124->unk0->getEscapeFromMarioIndex(uVar1, -1, mPosition, -1);

	JGeometry::TVec3<f32> p1 = SMS_GetMarioPos();
	JGeometry::TVec3<f32> p2;
	unk124->getGraph()->getGraphNode(uVar1).getPoint(&p2);
	JGeometry::TVec3<f32> p3;
	unk124->getGraph()->getGraphNode(uVar2).getPoint(&p3);

	p2 -= mPosition;
	p3 -= mPosition;
	p1 -= mPosition;

	f32 dot1 = p1.dot(p2);
	f32 dot2 = p1.dot(p3);
	if (0.0f <= dot1 && dot2 < 0.0f) {
		unk124->mPrevIdx = -1;
		unk124->mCurrIdx = uVar2;
		setGoalPathFromGraph();
		unk128 = 0;
		unk12C = 0.0f;
	} else {
		unk124->mPrevIdx = -1;
		unk124->mCurrIdx = uVar1;
		setGoalPathFromGraph();
		unk128 = 0;
		unk12C = 0.0f;
	}
}

void TFireWanwan::recoverFire()
{
	mHitPoints        = getMaxHitPoints();
	unk194->mIsOnFire = true;
}

bool TFireWanwan::isDefeat() const
{
	return mSpine->getLatestNerve() == &TNerveFireWanwanDie::theNerve();
}

bool TFireWanwan::canTakenByMario() const
{
	TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	return nerve == &TNerveFireWanwanEscape::theNerve()
	       || nerve == &TNerveFireWanwanFreeze::theNerve();
}

bool TFireWanwan::isTurning() const
{
	return mSpine->getLatestNerve() == &TNerveFireWanwanTurn::theNerve();
}

bool TFireWanwan::isFlying() const
{
	return mSpine->getLatestNerve() == &TNerveFireWanwanFly::theNerve();
}

bool TFireWanwan::isWalking() const
{
	TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	return nerve == &TNerveFireWanwanGraphWander::theNerve();
}

bool TFireWanwan::isWandering() const
{
	TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	return nerve == &TNerveFireWanwanRecover::theNerve()
	       || nerve == &TNerveFireWanwanTurn::theNerve() || isWalking();
}

bool TFireWanwan::isAttacking() const
{
	return mSpine->getLatestNerve() == &TNerveFireWanwanAttack::theNerve();
}

bool TFireWanwan::isFreeze() const
{
	return mSpine->getLatestNerve() == &TNerveFireWanwanFreeze::theNerve();
}

bool TFireWanwan::isReadyToFly() const
{
	bool taken = unk194->isTaken();
	return !taken
	       && unk194->unkA4->getLength()
	              <= getSaveParam2()->mTailLengthToFly.get();
}

bool TFireWanwan::isRecovering() const
{
	return mSpine->getLatestNerve() == &TNerveFireWanwanRecover::theNerve();
}

// Probably wrong?
bool TFireWanwan::isCameraShake() const
{
	return isFreeze() || isDefeat() || unk194->isTaken() || isRecovering()
	       || mSpine->getLatestNerve() == &TNerveFireWanwanEscape::theNerve();
}

bool TFireWanwan::isHungTailNerve() const
{
	return mSpine->getLatestNerve() == &TNerveFireWanwanHungTail::theNerve();
}

void TFireWanwan::prepareTurn()
{
	mTurnTargetAngle = MsAngleWrap(mRotation.y + 180.0f);
}

// TODO: is this real?
template <class T> static inline T clamp2(T t, T r)
{
	if (t < -r)
		return -r;

	if (t > r)
		return r;

	return t;
}

// TODO: wrong size + improbable pattern
bool TFireWanwan::doTurn()
{
	f32 fVar2 = getSaveParam2()->mRotateY.get();
	f32 fVar1 = MsAngleDiff(mTurnTargetAngle, mRotation.y);
	mRotation.y += clamp2(fVar1, fVar2);
	mRotation.y = MsAngleWrap(mRotation.y);

	if (abs(mRotation.y - mTurnTargetAngle) < 1.0f)
		return true;

	return false;
}

void TFireWanwan::sendAttackMsgToMario() { }

void TFireWanwan::attackToMario()
{
	if (!unk194->mIsOnFire)
		return;

	TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	if (nerve != &TNerveFireWanwanFly::theNerve()
	    && nerve != &TNerveFireWanwanEscape::theNerve()
	    && nerve != &TNerveFireWanwanRecover::theNerve()) {
		if (isFreeze()) {
			(void)nerve;
		} else {
			SMS_SendMessageToMario(this, HIT_MESSAGE_UNKA);
			mStopSearchTimer = getSaveParam2()->mStopSearchTimerMax.get();
		}
	}
}

void TFireWanwan::bind()
{
	mVelocity.y -= getGravityY();

	if (mVelocity.y < mVelocityMinY)
		mVelocity.y = mVelocityMinY;

	// clang-format off
	if (mVelocity.x < -100.0f) mVelocity.x = -100.0f;
	if (mVelocity.y < -100.0f) mVelocity.y = -100.0f;
	if (mVelocity.z < -100.0f) mVelocity.z = -100.0f;
	if (mVelocity.x >  100.0f) mVelocity.x =  100.0f;
	if (mVelocity.y >  100.0f) mVelocity.y =  100.0f;
	if (mVelocity.z >  100.0f) mVelocity.z =  100.0f;
	// clang-format on

	mVelocity *= getSaveParam2()->mAirFric.get();

	JGeometry::TVec3<f32> vel     = mVelocity;
	JGeometry::TVec3<f32> velStep = mLinearVelocity;
	velStep += vel;

	int stepCount = int(velStep.length() / 25.0f) + 1;
	velStep *= 1.0f / stepCount;

	JGeometry::TVec3<f32> totalNormal(0.0f, 0.0f, 0.0f);
	int iVar12 = 0;
	bool bVar2 = true;
	for (int i = 0; i < stepCount; ++i) {
		JGeometry::TVec3<f32> boundStep;
		JGeometry::TVec3<f32> stepNormal;
		iVar12 += bindBody(&boundStep, &stepNormal, velStep);

		bVar2 &= checkLiveFlag2(LIVE_FLAG_AIRBORNE);

		mPosition += boundStep;
		totalNormal += stepNormal;
	}

	mLinearVelocity.zero();

	if (bVar2)
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	else
		offLiveFlag(LIVE_FLAG_AIRBORNE);

	if (isFlying() && iVar12 > 0 && !totalNormal.isZero()) {
		totalNormal.normalize();

		// Reflect velocity on the plane of collision -- "bounce off"
		f32 fVar1 = mVelocity.dot(totalNormal);
		if (fVar1 < 0.0f)
			mVelocity.scaleAdd(abs(fVar1 * 2.0f), mVelocity, totalNormal);

		f32 rot = MsGetRotFromZaxisY(mVelocity);
		mRotation.set(0.0f, MsAngleWrap(rot), 0.0f);

		emitEffectsOnHittingWall(totalNormal, JGeometry::TVec3<f32>());

		gpCameraShake->startShake(CAM_SHAKE_MODE_UNK3, 8.0f);
		stopTriggerSound();
		if (gpMSound->gateCheck(0x28E1))
			MSoundSESystem::MSoundSE::startSoundActor(0x28E1, &mPosition, 0,
			                                          nullptr, 0, 4);
	}

	if (isFlying() && !checkLiveFlag(LIVE_FLAG_AIRBORNE)) {
		mVelocity *= getSaveParam2()->mAirFric.get();
		mVelocity.y *= -0.05f;
	}
}

int TFireWanwan::bindBody(JGeometry::TVec3<f32>* bound_step,
                          JGeometry::TVec3<f32>* normal_sum,
                          const JGeometry::TVec3<f32>& step)
{
	int collisionNum = 0;
	f32 fVar1        = mWallRadius * 0.27676865f;

	bound_step->zero();
	normal_sum->zero();

	JGeometry::TVec3<f32> currPos = mPosition;

	bool bVar8 = true;

	for (int i = 0; i < 8; ++i) {
		JGeometry::TVec3<f32> point = currPos;

		{
			// Give a man a hammer...
			JGeometry::TVec3<f32> off(0.0f, 0.0f, mWallRadius - fVar1);
			JGeometry::TQuat4<f32> quat;
			quat.setEulerY(i * JGeometry::TUtil<f32>::halfPI());
			quat.rotate(off, off);
			point += off;
		}

		JGeometry::TVec3<f32> boundPointStep;
		TBGWallCheckRecord checkRecord;
		bindPoint(&boundPointStep, point, step, fVar1, &checkRecord);
		bVar8 &= isAirborne();

		JGeometry::TVec3<f32> pointCorrection = boundPointStep - step;
		currPos += pointCorrection;

		for (int i = 0; i < checkRecord.mResultWallsNum; ++i)
			*normal_sum += checkRecord.mResultWalls[i]->getNormal();

		collisionNum += checkRecord.mResultWallsNum;
	}
	JGeometry::TVec3<f32> totalCorrection = currPos - mPosition;

	*bound_step = totalCorrection + step;

	if (bVar8)
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	else
		offLiveFlag(LIVE_FLAG_AIRBORNE);

	return collisionNum;
}

void TFireWanwan::bindPoint(JGeometry::TVec3<f32>* out_offset,
                            const JGeometry::TVec3<f32>& point,
                            const JGeometry::TVec3<f32>& param_3, f32 radius,
                            TBGWallCheckRecord* out_record)
{
	JGeometry::TVec3<f32> actualPoint = point;
	actualPoint += param_3;

	const TBGCheckData* local_30;

	if (checkLiveFlag(LIVE_FLAG_UNK1000))
		mGroundHeight = gpMap->checkGroundIgnoreWaterSurface(
		    actualPoint.x, actualPoint.y + mHeadHeight, actualPoint.z,
		    &local_30);
	else
		mGroundHeight
		    = gpMap->checkGround(actualPoint.x, actualPoint.y + mHeadHeight,
		                         actualPoint.z, &local_30);

	mGroundHeight += 1.0f;

	if (point.y > actualPoint.y && !local_30->isIllegalData()) {
		if (!local_30->isEnemyThrough()) {
			f32 dVar9;
			const TBGCheckData* local_34;
			if (checkLiveFlag(LIVE_FLAG_UNK1000))
				dVar9 = gpMap->checkGroundIgnoreWaterSurface(
				    actualPoint.x, actualPoint.y + mHeadHeight, actualPoint.z,
				    &local_34);
			else
				dVar9 = gpMap->checkGround(actualPoint.x,
				                           actualPoint.y + mHeadHeight,
				                           actualPoint.z, &local_34);

			dVar9 += 1.0f;
			if (dVar9 > mGroundHeight) {
				mGroundHeight = dVar9;
				local_30      = local_34;
			}
		}
	}

	mGroundPlane = local_30;
	if (actualPoint.y <= mGroundHeight + 0.05f) {
		if (mGroundPlane->isIllegalData()) {
			reset();
			return;
		}

		offLiveFlag(LIVE_FLAG_AIRBORNE);
		if (isFlying() || isDefeat()) {
			JGeometry::TVec3<f32> normal;
			normal.set(mGroundPlane->getNormal());
			f32 fVar4 = 1.0f
			            - (normal.dot(actualPoint)
			               - normal.dot(JGeometry::TVec3<f32>(
			                   actualPoint.x, mGroundHeight, actualPoint.z)));
			if (fVar4 > 0.0f)
				actualPoint.scaleAdd(fVar4, actualPoint, normal);
		} else {
			actualPoint.y = mGroundHeight + 1.0f;
		}
	} else {
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	checkWalls(&actualPoint, out_record, radius);
	*out_offset = actualPoint - point;
}

bool TFireWanwan::checkWalls(JGeometry::TVec3<f32>* point,
                             TBGWallCheckRecord* out_record, f32 radius)
{
	TBGWallCheckRecord local_58(point->x, point->y + radius, point->z, radius,
	                            4, 0);

	bool cVar6  = gpMap->isTouchedWallsAndMoveXZ(&local_58);
	*out_record = local_58;
	if (!cVar6)
		return false;

	if (!isFlying()) {
		point->x = local_58.mCenter.x;
		point->z = local_58.mCenter.z;
		return true;
	}

	bool bVar1 = false;
	for (int i = 0, e = local_58.mResultWallsNum; i < e; ++i)
		bVar1 |= behaveHitWallOnFlying(local_58.mResultWalls[i]);

	if (bVar1) {
		checkWalls(point, out_record, radius);
	} else {
		point->x = local_58.mCenter.x;
		point->z = local_58.mCenter.z;
	}

	return true;
}

bool TFireWanwan::behaveHitWallOnFlying(const TBGCheckData* check_data)
{
	const TLiveActor* actor = check_data->getActor();

	if (actor && actor->isActorType(0x4000001C)) {
		((TLiveActor*)actor)->receiveMessage(this, HIT_MESSAGE_HIP_DROP);
		return true;
	}

	return false;
}

void TFireWanwan::calcShadowPos() { }

void TFireWanwan::calcRipplePos()
{
	MtxPtr mtx = getModel()->getAnmMtx(mCenterJointIdx);
	const TBGCheckData* checkData;
	mRipplePos.set(mtx[0][3],
	               gpMap->checkGround(mtx[0][3], mtx[1][3] + 200.0f, mtx[2][3],
	                                  &checkData),
	               mtx[2][3]);
}

f32 TFireWanwan::getGravityY() const
{
	f32 result = mGravity;
	if (isFlying() || isDefeat())
		result = getSaveParam2()->mSLFlyGravityY.get();
	return result;
}

MtxPtr TFireWanwan::getTailMtx() const
{
	return mMActor->getModel()->getAnmMtx(TFireWanwan::mTailJntIndex);
}

const char** TFireWanwan::getBasNameTable() const
{
	return fireWanwan_bastable;
}

bool TFireWanwan::doKeepDistance() { return !unk194->mIsOnFire; }

bool TFireWanwan::isCollidMove(THitActor*) { return true; }

DEFINE_NERVE(TNerveFireWanwanGraphWander, TLiveActor)
{
	TFireWanwan* self = (TFireWanwan*)spine->getBody();

	if (spine->getTime() == 0)
		self->setBckAnm(7);

	if (self->isReachedToGoal())
		self->goToRandomNextGraphNode();

	if (self->isFindMario(1.0f)) {
		spine->pushAfterCurrent(&TNerveFireWanwanFindMario::theNerve());
		return true;
	}

	self->doWalk();

	return false;
}

DEFINE_NERVE(TNerveFireWanwanTurn, TLiveActor)
{
	TFireWanwan* self = (TFireWanwan*)spine->getBody();
	if (spine->getTime() == 0) {
		self->prepareTurn();
		self->setBckAnm(5);
	}

	if (self->checkCurAnmEnd(0))
		self->setBckAnm(7);

	if (self->doTurn()) {
		spine->pushAfterCurrent(&TNerveFireWanwanGraphWander::theNerve());
		self->initTurnNextGraphNode();

		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveFireWanwanFindMario, TLiveActor)
{
	TFireWanwan* self = (TFireWanwan*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setBckAnm(5);
		self->decideTarget(SMS_GetMarioPos());
	}

	self->doAdjustTarget();

	if (self->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveFireWanwanAttack::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveFireWanwanAttack, TLiveActor)
{
	TFireWanwan* self = (TFireWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setBckAnm(3);
		self->setGoalPathMario();
	}

	if (self->doAttack()) {
		spine->pushAfterCurrent(&TNerveFireWanwanRecoverGraph::theNerve());
		return true;
	}

	if (self->getSaveParam2()->mAttackTimerMax.get() < spine->getTime()) {
		spine->pushAfterCurrent(&TNerveFireWanwanTired::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveFireWanwanTired, TLiveActor)
{
	TFireWanwan* self = (TFireWanwan*)spine->getBody();

	if (spine->getTime() == 0)
		self->setBckAnm(4);

	if (self->checkCurAnmEnd(0)) {
		self->setBckAnm(4);

		if (self->getSaveParam2()->mSLAttackWait.get() < spine->getTime()) {
			if (self->isFindMario(1.0f))
				spine->pushAfterCurrent(&TNerveFireWanwanFindMario::theNerve());
			else
				spine->pushAfterCurrent(
				    &TNerveFireWanwanRecoverGraph::theNerve());
			return true;
		}
	}

	return false;
}

DEFINE_NERVE(TNerveFireWanwanRecoverGraph, TLiveActor)
{
	TFireWanwan* self = (TFireWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setBckAnm(5);
		self->unk124->mPrevIdx = -1;
		self->unk124->mCurrIdx = -1;
		self->goToShortestNextGraphNode();

		self->decideTarget(self->unk104.getPoint());
	}

	// TODO: inline?
	bool b = self->getMActor()->checkCurBckFromIndex(7);
	if (b) {
		self->doAdjustTarget();
		if (self->checkCurAnmEnd(0)) {
			// removed code?
		}
	} else {
		if (self->checkCurAnmEnd(0))
			self->setBckAnm(7);
	}

	if (self->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveFireWanwanGraphWander::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveFireWanwanRecover, TLiveActor)
{
	TFireWanwan* self = (TFireWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setBckAnm(2);
		self->getMActor()->setFrameRate(0.0f, 4);
		self->getMActor()->getFrameCtrl(4)->setFrame(0.0f);
		TFireWanwanManager* manager = (TFireWanwanManager*)self->getManager();
		manager->receiveMessageFromBody(self,
		                                TFireWanwanManager::BODY_MSG_RECOVERED);

		if (gpMSound->gateCheck(0x28E2))
			MSoundSESystem::MSoundSE::startSoundActor(0x28E2, &self->mPosition,
			                                          0, nullptr, 0, 4);

		f32 end = self->getMActor()->getFrameCtrl(0)->getEnd();

		self->changeBodyToRed(end);
	}

	if (self->checkCurAnmEnd(0)) {
		self->recoverFire();
		spine->pushAfterCurrent(spine->getDefault());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveFireWanwanDie, TLiveActor)
{
	TFireWanwan* self = (TFireWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setBckAnm(0);
		self->unk194->mIsOnFire = false;
		self->mRotation.x       = 90.0f;

		TFireWanwanManager* manager = (TFireWanwanManager*)self->getManager();

		manager->mLastKillerWanwan                = self;
		manager->mWanwanRecoversBeforeHelpBalloon = -1;

		if (++manager->mWanwansKilled == manager->getActiveObjNum()) {
			gpMarDirector->getConsole()->startAppearBalloon(0xE0019, true);
		}

		self->stopTriggerSound();
		if (gpMSound->gateCheck(0x28E3))
			MSoundSESystem::MSoundSE::startSoundActor(0x28E3, &self->mPosition,
			                                          0, nullptr, 0, 4);

		self->changeBodyToSilver(40);

		self->generateEffectColumWater();
		self->onLiveFlag(LIVE_FLAG_UNK1000);
		self->offLiveFlag(LIVE_FLAG_AIRBORNE);
		self->mHitPoints = 0;
		self->mDamageHeight -= 250.0f;
		self->calcEntryRadius();
	}

	JGeometry::TVec3<f32> vel = self->mVelocity;
	vel.x *= 0.9f;
	vel.z *= 0.9f;
	self->mVelocity = vel;

	if (gpMSound->gateCheck(0x20E4))
		MSoundSESystem::MSoundSE::startSoundActor(0x20E4, &self->mPosition, 0,
		                                          nullptr, 0, 4);

	return false;
}

DEFINE_NERVE(TNerveFireWanwanHungTail, TLiveActor)
{
	TFireWanwan* self = (TFireWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setBckAnm(0);
		self->getMActor()->setBtkFromIndex(0);
		self->getMActor()->setFrameRate(SMSGetAnmFrameRate(), 4);
	}

	JGeometry::TVec3<f32> vec = self->mPosition;
	vec -= SMS_GetMarioPos();

	self->mRotation.y = MsGetRotFromZaxisY(vec);
	if (self->isReadyToFly()) {
		spine->pushAfterCurrent(&TNerveFireWanwanFly::theNerve());
		return true;
	}

	return false;
}

// TODO: fake
static inline JGeometry::TVec3<f32> fromPolar(f32 theta, f32 radius)
{
	return JGeometry::TVec3<f32>(radius * JMASSin(theta * (65536.0f / 360.0f)),
	                             0.0f,
	                             radius * JMASCos(theta * (65536.0f / 360.0f)));
}

DEFINE_NERVE(TNerveFireWanwanFly, TLiveActor)
{
	TFireWanwan* self = (TFireWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setBckAnm(0);
		self->getMActor()->setBtkFromIndex(0);
		self->getMActor()->setFrameRate(SMSGetAnmFrameRate(), 4);

		JGeometry::TVec3<f32> vel
		    = fromPolar(self->mRotation.y, self->unk194->mThrowPow);
		self->mVelocity = vel;

		SMS_EasyEmitParticle(PARTICLE_MS_FUMI_C, self->getTailMtx(), self,
		                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));

		f32 fVar1 = self->getSaveParam2()->mFreezeTimeThreshold.get();
		if (fVar1 * fVar1 < vel.squared())
			self->mFreezeWait = self->getSaveParam2()->mSLFreezeWait.get();
		else
			self->mFreezeWait = self->getSaveParam2()->mSLFreezeWaitShort.get();
	}

	JGeometry::TVec3<f32> vel = self->mVelocity;
	vel *= self->getSaveParam2()->mAirFric.get();
	self->mVelocity = vel;
	vel.y           = 0.0f;

	if (vel.squared() < 1.0f) {
		spine->pushAfterCurrent(&TNerveFireWanwanFreeze::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveFireWanwanFreeze, TLiveActor)
{
	TFireWanwan* self = (TFireWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setBckAnm(0);
		self->getMActor()->setBtkFromIndex(0);
		self->getMActor()->setFrameRate(SMSGetAnmFrameRate(), 4);

		if (SMSGetMSound()->gateCheck(0x28A6))
			MSoundSESystem::MSoundSE::startSoundActor(0x28A6, &self->mPosition,
			                                          0, nullptr, 0, 4);
	}

	self->setVelocity(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));

	if (self->getFreezeTime() < spine->getTime()) {
		spine->pushAfterCurrent(&TNerveFireWanwanRecover::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveFireWanwanEscape, TLiveActor)
{
	TFireWanwan* self = (TFireWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		self->initEscapeNextGraphNode();
		self->setBckAnm(0);
		self->getMActor()->setBtkFromIndex(0);
		self->getMActor()->setFrameRate(SMSGetAnmFrameRate(), 4);

		self->changeBodyToBlack(40);
	}

	if (self->getSaveParam2()->mEscapeRecoverTime.get() < spine->getTime()) {
		spine->pushAfterCurrent(&TNerveFireWanwanRecover::theNerve());
		return true;
	}

	if (self->isReachedToGoal())
		self->goToRandomEscapeGraphNode();

	self->doEscape();

	if (gpMSound->gateCheck(0x20DD))
		MSoundSESystem::MSoundSE::startSoundActor(0x20DD, &self->mPosition, 0,
		                                          nullptr, 0, 4);

	return false;
}
