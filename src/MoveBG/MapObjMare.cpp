#include <MoveBG/MapObjMare.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/ItemManager.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <Enemy/Cannon.hpp>
#include <M3DUtil/MActor.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapEventMare.hpp>
#include <Map/MapWireManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MoveBG/MapObjWave.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Player/WaterGun.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <string.h>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

f32 TCogwheelScale::mWaterLeakSpeed = 0.01f;

/// Distance from the wheel's centre to either rope.
static f32 sRadius = 800.0f;

f32 TCogwheel::mRopeWidthX = 10.0f;
f32 TCogwheel::mRopeWidthZ = 7.0f;
f32 TCogwheel::mTexPosRate = 0.01f;
f32 TCogwheel::mMinSpeed   = 3.0f;

static f32 mGrowStartFrame = 90.0f;
static f32 mGrowEndFrame   = 175.0f;

/// Second emitter position for the waterfall's upper half.
static JGeometry::TVec3<f32> fall_upper_pos(2827.0f, 8604.0f, 7202.0f);

u32 TCogwheelScale::touchWater(THitActor* water)
{
	if (mWaterAmount < mWaterAmountMax)
		mWaterAmount += 1.0f;

	return 1;
}

BOOL TCogwheelScale::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_HIP_DROP) {
		mCogwheel->mSpeed = mPushSpeed + mCogwheel->mSpeed;
		return true;
	}

	return TMapObjBase::receiveMessage(sender, message);
}

void TCogwheelScale::touchPlayer(THitActor* player)
{
	if (marioIsOn())
		mRiderWeight = mMarioWeight;

	if (mPosition.y - mYOffset > 150.0f + gpMarioPos->y
	    && ((mIsUpper && mCogwheel->mSpeed > 0.0f)
	        || (!mIsUpper && mCogwheel->mSpeed < 0.0f))) {
		mCogwheel->rebound();

		// Landing on the high bucket makes the wheel spin the other way at a
		// rate proportional to how hard Mario hit it.
		if (marioHeadAttack())
			mCogwheel->mSpeed
			    = mHeadAttackRate * (mCogwheel->mSpeed * *gpMarioSpeedY);
	}

	mWaterAmount = 0.0f;
}

void TCogwheelScale::control()
{
	mRiderWeight = 0.0f;
	TMapObjBase::control();

	if (mWaterAmount > 0.0f) {
		mWaterAmount -= mWaterLeakSpeed;
		SMSGetMSound()->startSoundActorWithInfo(
		    MSD_SE_OBJ_MR_TSUBO_WATER, &mPosition, nullptr,
		    fabsf(mWaterAmount), 0, 0, nullptr, 0, 4);
		if (mWaterAmount < 0.0f)
			mWaterAmount = 0.0f;
	}
}

TCogwheelScale::TCogwheelScale(const char* name)
    : TMapObjBase(name)
    , mWeight(0.0f)
    , mMarioWeight(0.0f)
    , mWaterAmount(0.0f)
    , mWaterAmountMax(0.0f)
    , mRiderWeight(0.0f)
    , mHeadAttackRate(0.01f)
    , mPushSpeed(5.0f)
    , mIsUpper(0)
    , mCogwheel(nullptr)
{
}

void TCogwheel::initDraw() const
{
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

	GXLoadPosMtxImm(j3dSys.mViewMtx, GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);

	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);

	GXColor color = { 0, 0, 100, 255 };
	GXSetChanMatColor(GX_COLOR0A0, JUtility::TColor(color));

	GXSetNumTexGens(1);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
	                  GX_FALSE, GX_PTIDENTITY);

	JUTTexture texture(gpMapObjManager->unkC8);
	texture.load(GX_TEXMAP0);

	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);

	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetCullMode(GX_CULL_BACK);
}

void TCogwheel::draw() const
{
	initDraw();

	// The plate's rope: a cross of two quads from the wheel down to the plate.
	f32 wheelY   = mPlateRopePos.y;
	f32 plateY   = mPlate->mPosition.y - mPlate->mYOffset;
	f32 plateTop = 600.0f + plateY;
	f32 plateX1  = mPlateRopePos.x + mRopeWidthX;
	f32 plateX0  = mPlateRopePos.x - mRopeWidthX;
	f32 plateZ1  = mPlateRopePos.z + mRopeWidthZ;
	f32 plateZ0  = mPlateRopePos.z - mRopeWidthZ;
	f32 plateVT  = mTexPosRate * (plateTop - plateY);
	f32 plateVB  = mTexPosRate * (wheelY - plateY);

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 8);
	GXPosition3f32(plateX0, plateTop, plateZ0);
	GXTexCoord2f32(0.0f, plateVT);
	GXPosition3f32(plateX0, wheelY, plateZ0);
	GXTexCoord2f32(0.0f, plateVB);
	GXPosition3f32(plateX1, plateTop, plateZ1);
	GXTexCoord2f32(1.0f, plateVT);
	GXPosition3f32(plateX1, wheelY, plateZ1);
	GXTexCoord2f32(1.0f, plateVB);
	GXPosition3f32(plateX1, plateTop, plateZ0);
	GXTexCoord2f32(2.0f, plateVT);
	GXPosition3f32(plateX1, wheelY, plateZ0);
	GXTexCoord2f32(2.0f, plateVB);
	GXPosition3f32(plateX0, plateTop, plateZ1);
	GXTexCoord2f32(3.0f, plateVT);
	GXPosition3f32(plateX0, wheelY, plateZ1);
	GXTexCoord2f32(3.0f, plateVB);

	// The pot's rope. Same shape, but it hangs twice as far and only uses two
	// texture columns.
	f32 potY   = mPot->mPosition.y - mPot->mYOffset;
	f32 potX1  = mPotRopePos.x + mRopeWidthX;
	f32 potX0  = mPotRopePos.x - mRopeWidthX;
	f32 potZ1  = mPotRopePos.z + mRopeWidthZ;
	f32 potZ0  = mPotRopePos.z - mRopeWidthZ;
	f32 potTop = 1200.0f + potY;
	f32 potVB  = mTexPosRate * (wheelY - potY);
	f32 potVT  = mTexPosRate * (potTop - potY);

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 8);
	GXPosition3f32(potX0, potTop, potZ0);
	GXTexCoord2f32(0.0f, potVT);
	GXPosition3f32(potX0, wheelY, potZ0);
	GXTexCoord2f32(0.0f, potVB);
	GXPosition3f32(potX1, potTop, potZ1);
	GXTexCoord2f32(1.0f, potVT);
	GXPosition3f32(potX1, wheelY, potZ1);
	GXTexCoord2f32(1.0f, potVB);
	GXPosition3f32(potX1, potTop, potZ0);
	GXTexCoord2f32(0.0f, potVT);
	GXPosition3f32(potX1, wheelY, potZ0);
	GXTexCoord2f32(0.0f, potVB);
	GXPosition3f32(potX0, potTop, potZ1);
	GXTexCoord2f32(1.0f, potVT);
	GXPosition3f32(potX0, wheelY, potZ1);
	GXTexCoord2f32(1.0f, potVB);
}

/**
 * @brief Bounce the wheel back off one of its rope's end stops.
 *
 * @details UNUSED in the map (0x34): inlined at all three call sites, twice in
 * control() and once in TCogwheelScale::touchPlayer().
 */
void TCogwheel::rebound()
{
	mSpeed *= -mReboundRate;
	if (fabsf(mSpeed) < mMinSpeed)
		mSpeed = 0.0f;
}

void TCogwheel::calc()
{
	mRotation.z = 360.0f * (-mPlateRopeLength / (3.14f * (2.0f * sRadius)));

	Mtx rotZ;
	makeRootMtxRotZ(rotZ);
	rotZ[0][3] = 0.0f;
	rotZ[1][3] = 0.0f;
	rotZ[2][3] = 0.0f;

	Mtx rotY;
	makeRootMtxRotY(rotY);
	rotY[0][3] = 0.0f;
	rotY[1][3] = 0.0f;
	rotY[2][3] = 0.0f;

	MtxPtr mtx = getModel()->getAnmMtx(0);
	MTXConcat(rotY, rotZ, mtx);
	mtx[0][3] = mPosition.x;
	mtx[1][3] = mPosition.y;
	mtx[2][3] = mPosition.z;
}

void TCogwheel::control()
{
	TMapObjBase::control();

	mPlateRopeLength += mSpeed;
	mSpeed += mAccelRate
	    * ((mPlate->mRiderWeight + (mPlate->mWeight + mPlate->mWaterAmount))
	       - (mPot->mRiderWeight + (mPot->mWeight + mPot->mWaterAmount)));
	mSpeed *= mSpeedDecay;

	if (mPlateRopeLength < mUpperMargin && mSpeed < 0.0f)
		rebound();

	if (mPlateRopeLength > mRopeLength - mLowerMargin && mSpeed > 0.0f)
		rebound();

	mPlate->mPosition.y = mPosition.y - mPlateRopeLength + mPlate->mYOffset;
	mPot->mPosition.y   = mPosition.y - (mRopeLength - mPlateRopeLength);

	f32 speed = fabsf(mSpeed);
	if (speed > 0.01f) {
		f32 volume = 10.0f * speed;
		gpMSound->startSoundActorWithInfo(MSD_SE_OBJ_MR_TSUBO_PULL, &mPosition,
		                                  nullptr, volume, 0, 0, nullptr, 0,
		                                  4);
	}
}

void TCogwheel::initMapObj()
{
	TMapObjBase::initMapObj();

	// TODO: the ROM keeps a literal `0.0f * sin` and `0.0f * cos` here (the
	// z component of the unrotated (sRadius, 0, 0) offset), which means the
	// rotation came out of an inlined helper -- MWCC only fails to fold a
	// multiply by zero across an inline boundary. No such helper has been
	// found: TMapObjBase::rotateVecByAxisY is a full three-axis rotation and
	// is never called here, and spelling the zeros out folds them away.
	f32 radius  = sRadius;
	f32 rad     = 0.017453294f * mRotation.y;
	f32 cos     = cosf(rad);
	f32 sin     = sinf(rad);
	f32 offsetX = radius * cos - 0.0f * sin;
	f32 offsetZ = radius * sin + 0.0f * cos;

	JGeometry::TVec3<f32> pos(mPosition.x + offsetX, mPosition.y,
	                          mPosition.z - offsetZ);
	JGeometry::TVec3<f32> scale(1.0f, 1.0f, 1.0f);
	mPlate = (TCogwheelScale*)TMapObjBaseManager::newAndRegisterObj(
	    "cogwheel_plate", pos, mRotation, scale);
	mPlate->mIsUpper  = true;
	mPlate->mCogwheel = this;
	mPlate->appear();

	mPlateRopePos.set(pos.x, mPosition.y, pos.z);

	pos.set(mPosition.x - offsetX, mPosition.y, mPosition.z + offsetZ);
	JGeometry::TVec3<f32> potScale(1.0f, 1.0f, 1.0f);
	mPot = (TCogwheelScale*)TMapObjBaseManager::newAndRegisterObj(
	    "cogwheel_pot", pos, mRotation, potScale);
	mPot->mIsUpper  = false;
	mPot->mCogwheel = this;
	mPot->appear();

	mPotRopePos.set(pos.x, mPosition.y, pos.z);

	// The Noki Bay pair of balances differ only in tuning; the upper one is
	// slower and has a shorter rope.
	if (strcmp(getName(), "天秤上") == 0) {
		mAccelRate       = 0.003f;
		mSpeedDecay      = 0.99f;
		mReboundRate     = 0.8f;
		mRopeLength      = 3800.0f;
		mUpperMargin     = 1000.0f;
		mLowerMargin     = 1800.0f;
		mPot->mWeight    = 0.0f;
		mPot->mMarioWeight = 0.0f;
		mPot->mWaterAmountMax = 14.0f;
		mPlate->mWeight  = 10.0f;
		mPlate->mMarioWeight = 0.0f;
		mPlate->mWaterAmountMax = 0.0f;
	} else {
		mAccelRate       = 0.008f;
		mSpeedDecay      = 0.98f;
		mReboundRate     = 0.8f;
		mRopeLength      = 3950.0f;
		mUpperMargin     = 1000.0f;
		mLowerMargin     = 1900.0f;
		mPot->mWeight    = 0.0f;
		mPot->mMarioWeight = 0.0f;
		mPot->mWaterAmountMax = 14.0f;
		mPlate->mWeight  = 10.0f;
		mPlate->mMarioWeight = 0.0f;
		mPlate->mWaterAmountMax = 0.0f;
	}

	mPlateRopeLength = mRopeLength / 2.0f;
}

TCogwheel::TCogwheel(const char* name)
    : TMapObjBase(name)
    , mSpeed(0.0f)
    , mPlateRopeLength(0.0f)
    , mAccelRate(0.0f)
    , mSpeedDecay(0.0f)
    , mReboundRate(0.0f)
    , mRopeLength(0.0f)
    , mPlate(nullptr)
    , mUpperMargin(0.0f)
    , mPot(nullptr)
    , mLowerMargin(0.0f)
{
	mPotRopePos.set(0.0f, 0.0f, 0.0f);
	mPlateRopePos.set(0.0f, 0.0f, 0.0f);
}

void TMapObjElasticCode::draw() const
{
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);

	GXLoadPosMtxImm(j3dSys.mViewMtx, GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);

	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);

	GXColor color = { 0, 0, 100, 255 };
	GXSetChanMatColor(GX_COLOR0A0, JUtility::TColor(color));

	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_RASC, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_RASA, GX_CA_ZERO, GX_CA_ZERO,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);

	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
	GXSetCullMode(GX_CULL_NONE);
	GXSetLineWidth(24, GX_TO_ZERO);

	GXBegin(GX_LINES, GX_VTXFMT0, 2);
	GXPosition3f32(mInitialPosition.x, 1000.0f + mInitialPosition.y,
	               mInitialPosition.z);
	GXPosition3f32(mPosition.x, mPosition.y, mPosition.z);
}

void TMapObjElasticCode::control()
{
	TMapObjBase::control();

	mVelocity.y *= mSpeedDecay;
	mVelocity.y += mSpringRate * (mInitialPosition.y - mPosition.y)
	    - getGravityY();

	if (mHeldObject != nullptr) {
		mVelocity.y -= mHoldWeight;

		JGeometry::TVec3<f32> pos = mHeldObject->mPosition;
		JGeometry::TVec3<f32> velocity = mVelocity;
		pos.y += velocity.y;
		mHeldObject->moveRequest(pos);
	}

	JGeometry::TVec3<f32> velocity = mVelocity;
	mPosition.y += velocity.y;
}

void TMapObjElasticCode::initMapObj()
{
	TMapObjBase::initMapObj();

	mSpeedDecay = 0.997f;
	mGravity    = 0.01f;
	mHoldWeight = 2.0f;
	mSpringRate = 0.0005f;
}

/**
 * @brief How far the tree's collision has to move this frame.
 *
 * @details UNUSED in the map (0x88); inlined into control() and touchWater().
 */
f32 TMapObjGrowTree::getGrowHeightFromRate(f32 rate) const
{
	if (mGrowStartFrame < getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
	    && getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
	        < mGrowEndFrame)
		return rate * mGrowHeight / (mGrowEndFrame - mGrowStartFrame);

	return 0.0f;
}

/**
 * @brief Resize the damage cylinder to the tree's current height.
 *
 * @details UNUSED in the map (0xcc); inlined into control() and touchWater().
 */
void TMapObjGrowTree::updateHeight()
{
	if (getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
	    > mGrowStartFrame) {
		if (getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
		    > mGrowEndFrame) {
			setDamageHeight(mGrowHeight);
		} else {
			setDamageHeight(
			    mInitialHeight
			    + (mGrowHeight - mInitialHeight)
			        * (getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
			           - mGrowStartFrame)
			        / (mGrowEndFrame - mGrowStartFrame));
		}
	} else {
		setDamageHeight(mInitialHeight);
	}
}

u32 TMapObjGrowTree::touchWater(THitActor* water)
{
	if (water->mPosition.y > mPosition.y + mInitialHeight)
		return 0;

	if (isState(1)) {
		startAnim(1);
		getMActor()->getFrameCtrl(ANM_TYPE_BCK)->setRate(0.0f);
		mState = 2;
	}

	if (getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
	    < (f32)getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getEnd()) {
		soundBas(MSD_SE_OBJ_SANDBOMB_WATER_1, 3.0f, mGrowSpeed);
		soundBas(MSD_SE_OBJ_SANDBOMB_WATER_2, 67.0f, mGrowSpeed);
		soundBas(MSD_SE_OBJ_SANDBOMB_WATER_3, 103.0f, mGrowSpeed);
		soundBas(MSD_SE_OBJ_SANDBOMB_WATER_4, 137.0f, mGrowSpeed);

		f32 rate = mGrowSpeed;
		getMActor()->getFrameCtrl(ANM_TYPE_BCK)
		    ->setFrame(rate
		               + getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame());
		updateHeight();

		// The collision block rides up with the sprout.
		if (mHeldObject != nullptr) {
			JGeometry::TVec3<f32> pos = mHeldObject->mPosition;
			pos.y += getGrowHeightFromRate(mGrowSpeed);
			mHeldObject->moveRequest(pos);
		}
	}

	if (getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame() > mGrowEndFrame) {
		setUpMapCollision(0);
		mStateTimer = mGrownTime;
	}

	return 1;
}

void TMapObjGrowTree::control()
{
	TMapObjBase::control();

	if (isState(2) && mColCount == 0 && !isStateTimerEngaged()
	    && getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame() > 0.0f) {
		if (getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
		    < mGrowEndFrame)
			removeMapCollision();

		f32 rate = -mShrinkSpeed;
		getMActor()->getFrameCtrl(ANM_TYPE_BCK)
		    ->setFrame(rate
		               + getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame());
		if (getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame() < 0.0f) {
			startAnim(0);
			mState = 1;
			return;
		}

		f32 frame = getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
		if (67.0f <= frame && frame <= 240.0f)
			gpMSound->startSoundActor(MSD_SE_OBJ_SAMDBOMB_REVERSE, &mPosition,
			                          0, nullptr, 0, 4);

		updateHeight();

		if (mHeldObject != nullptr) {
			JGeometry::TVec3<f32> pos = mHeldObject->mPosition;
			pos.y -= getGrowHeightFromRate(mShrinkSpeed);
			mHeldObject->moveRequest(pos);
		}
	}
}

void TMapObjGrowTree::loadAfter()
{
	TMapObjBase::loadAfter();
	removeMapCollision();
}

void TMapObjGrowTree::initMapObj()
{
	TMapObjBase::initMapObj();

	mGrowHeight    = 1000.0f;
	mGrowSpeed     = 0.5f;
	mShrinkSpeed   = 0.1f;
	mGrownTime     = 360;
	mInitialHeight = mDamageHeight;
	getMActor()->setBtp("moyasi_wink");
}

TMapObjGrowTree::TMapObjGrowTree(const char* name)
    : TMapObjBase(name)
    , mGrowHeight(0.0f)
    , mGrowSpeed(0.0f)
    , mShrinkSpeed(0.0f)
    , mGrownTime(0)
    , mInitialHeight(0.0f)
{
}

void TWireBell::initDraw() const
{
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

	GXLoadPosMtxImm(j3dSys.mViewMtx, GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);

	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);

	GXColor color = { 0, 0, 100, 255 };
	GXSetChanMatColor(GX_COLOR0A0, JUtility::TColor(color));

	GXSetNumTexGens(1);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
	                  GX_FALSE, GX_PTIDENTITY);

	JUTTexture texture(gpMapObjManager->unkC8);
	texture.load(GX_TEXMAP0);

	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);

	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetCullMode(GX_CULL_BACK);
}

void TWireBell::draw() const
{
	initDraw();

	// A cross of two quads running up the wire from the bell to its hanger.
	f32 bellY  = mPosition.y;
	f32 wireY  = mWirePos.y;
	f32 vWire  = mTexPosRate * (wireY - bellY);
	f32 vBell  = mTexPosRate * (bellY - bellY);
	f32 x1     = mWirePos.x + mRopeWidthX;
	f32 x0     = mWirePos.x - mRopeWidthX;
	f32 z1     = mWirePos.z + mRopeWidthZ;
	f32 z0     = mWirePos.z - mRopeWidthZ;

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 8);
	GXPosition3f32(x0, wireY, z0);
	GXTexCoord2f32(0.0f, vWire);
	GXPosition3f32(x0, bellY, z0);
	GXTexCoord2f32(0.0f, vBell);
	GXPosition3f32(x1, wireY, z1);
	GXTexCoord2f32(1.0f, vWire);
	GXPosition3f32(x1, bellY, z1);
	GXTexCoord2f32(1.0f, vBell);
	GXPosition3f32(x1, wireY, z0);
	GXTexCoord2f32(0.0f, vWire);
	GXPosition3f32(x1, bellY, z0);
	GXTexCoord2f32(0.0f, vBell);
	GXPosition3f32(x0, wireY, z1);
	GXTexCoord2f32(1.0f, vWire);
	GXPosition3f32(x0, bellY, z1);
	GXTexCoord2f32(1.0f, vBell);
}

void TWireBell::control()
{
	gpMapWireManager->getPointPosInNthWire(mWireNo, mPosition, &mWirePos);

	mPosition.x = mWirePos.x;
	mPosition.y = mWirePos.y - mHangLength;
	mPosition.z = mWirePos.z;

	Mtx mtx;
	MsMtxSetTRS(mtx, mPosition.x, mPosition.y, mPosition.z, mRotation.x,
	            mRotation.y, mRotation.z, mScaling.x, mScaling.y, mScaling.z);
	MTXCopy(mtx, getModel()->getAnmMtx(0));
}

void TWireBell::loadAfter()
{
	TMapObjBase::loadAfter();
	mWireNo = gpMapWireManager->getWireNo(mPosition);
}

TWireBell::TWireBell(const char* name)
    : TMapObjBase(name)
    , mWireNo(-1)
    , mHangLength(200.0f)
    , mRopeWidthX(10.0f)
    , mRopeWidthZ(5.0f)
    , mTexPosRate(0.01f)
{
	mWirePos.set(0.0f, 0.0f, 0.0f);
}

void TMapObjPuncher::touchPlayer(THitActor* player)
{
	awake();
	startAnim(1);

	JGeometry::TVec3<f32> dir;
	makeVecToLocalZ(1.0f, &dir);

	JGeometry::TVec3<f32> dest(*gpMarioPos);
	dest += dir * 100.0f;
	SMS_MarioMoveRequest(dest);
	SMS_SendMessageToMario(this, HIT_MESSAGE_THROWN);
	SMS_ThrowMario(dir, mThrowSpeed);
	onHitFlag(HIT_FLAG_NO_COLLISION);

	JGeometry::TVec3<f32> scale(2.0f, 2.0f, 2.0f);
	emitAndScale(PARTICLE_MS_ENM_DISAP_A_W, 0, &mPosition, scale);
	emitAndScale(PARTICLE_MS_ENM_DISAP_B, 0, &mPosition, scale);
	gpMSound->startSoundActor(MSD_SE_SMOKE_EFFECT, &mPosition, 0, nullptr, 0,
	                          4);

	mState = 2;
}

void TMapObjPuncher::control()
{
	TMapObjBase::control();

	switch (mState) {
	case 1:
		break;

	case 2:
		soundBas(MSD_SE_OBJ_PUNCHER_RETURN, 101.0f,
		         getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getRate());
		if (animIsFinished()) {
			JGeometry::TVec3<f32> scale(2.0f, 2.0f, 2.0f);
			emitAndScale(PARTICLE_MS_ENM_DISAP_A_W, 0, &mPosition, scale);
			emitAndScale(PARTICLE_MS_ENM_DISAP_B, 0, &mPosition, scale);
			gpMSound->startSoundActor(MSD_SE_SMOKE_EFFECT, &mPosition, 0,
			                          nullptr, 0, 4);
			kill();
		}
		break;
	}
}

void TMapObjPuncher::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);

	int speed;
	stream.read(&speed, 4);
	mThrowSpeed = speed;

	sleep();
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

void TMuddyBoat::moveByWater()
{
	if (SMS_GetMarioWaterGun()->mIsEmitWater == 0)
		return;

	MtxPtr emitMtx = SMS_GetMarioWaterGun()->getEmitMtx(0);
	JGeometry::TVec3<f32> spray(-emitMtx[0][0], 0.0f, -emitMtx[2][0]);
	MsVECNormalize(&spray, &spray);

	// The boat's heading, flat in XZ. Keeping the y as a literal 0.0f inside a
	// real vector is what leaves the ROM's trivial `y * 0.0f` products in both
	// dot products; spelling the dots out folds them away.
	MtxPtr mtx = getModel()->getAnmMtx(0);
	JGeometry::TVec3<f32> dir(mtx[0][2], 0.0f, mtx[2][2]);
	f32 push = spray.dot(dir);

	JGeometry::TVec3<f32> toMario;
	getNormalVecFromTargetXZ(gpMarioPos->x, gpMarioPos->z, &toMario);
	if (toMario.x != 0.0f || toMario.z != 0.0f)
		MsVECNormalize(&toMario, &toMario);

	// Which side of the boat the spray landed on decides the turn direction.
	f32 side = dir.z * (spray.x - dir.x) - dir.x * (spray.z - dir.z);
	if (side * dir.dot(toMario) > 0.0f)
		mTurnSpeed += mTurnAccel * (1.0f - fabsf(push));
	else
		mTurnSpeed -= mTurnAccel * (1.0f - fabsf(push));

	if (push > 0.0f)
		mSpeed += push * mAccelForward;
	else
		mSpeed += push * mAccelBackward;

	offLiveFlag(LIVE_FLAG_UNK10);
}

void TMuddyBoat::calcRootMatrix() { }

void TMuddyBoat::kill()
{
	mSpeed     = 0.0f;
	mTurnSpeed = 0.0f;

	SMS_EasyEmitParticle(PARTICLE_MS_M_AMIATTACK, &mEffectPos, nullptr,
	                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	gpMSound->startSoundActor(MSD_SE_OBJ_DORO_BROKEN, &mPosition, 0, nullptr,
	                          0, 4);

	MTXCopy(getModel()->getAnmMtx(0), getModel()->getBaseTRMtx());

	offMapObjFlag(MAP_OBJ_FLAG_UNK100);
	onLiveFlag(LIVE_FLAG_UNK10);
	startAnim(1);
	startAnim(2);
	mState = 2;
}

/**
 * @brief Break the boat against the walls a sweep just hit.
 *
 * @details UNUSED in the map (0xa8); inlined into bind() three times, once per
 * probe.
 */
void TMuddyBoat::touchWall(JGeometry::TVec3<f32>* pos,
                           const TBGWallCheckRecord& record)
{
	f32 z = -(record.mResultWalls[0]->mNormal.z * (50.0f + record.mRadius)
	          - record.mCenter.z);
	// TODO: retail emits the weak getObjCollisionHeightOffset() into this
	// object and `bl`s it three times from bind(); ours still expands it
	// (`lfs 0x108`). That is the caller-size rule, not a wrong spelling: our
	// bind() frame is 0x140 against retail's 0x270, and MWCC stops inlining
	// two-instruction accessors only once the caller is big enough. It should
	// fall out when bind()'s missing locals are recovered.
	f32 yOffset  = getObjCollisionHeightOffset();
	mEffectPos.x = -(record.mResultWalls[0]->mNormal.x
	                     * (50.0f + record.mRadius)
	                 - record.mCenter.x);
	mEffectPos.y = 100.0f + (mPosition.y - yOffset);
	mEffectPos.z = z;

	*pos = mPosition;
	kill();
	mLinearVelocity.zero();
}

/**
 * @brief Sweep a wall probe and, if it hits, break the boat.
 *
 * @details UNUSED in the map (0x104); inlined into bind() three times.
 */
void TMuddyBoat::bindToWall(const JGeometry::TVec3<f32>& probe, f32 radius,
                            JGeometry::TVec3<f32>* pos)
{
	TBGWallCheckRecord record(probe, radius, 4,
	                          TBGWallCheckRecord::DONT_MOVE_XZ);
	if (gpMap->isTouchedWallsAndMoveXZ(&record))
		touchWall(pos, record);
}

void TMuddyBoat::bind()
{
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;

	JGeometry::TVec3<f32> next(mPosition);
	MtxPtr mtx = getModel()->getAnmMtx(0);
	next.x += mtx[0][2] * mSpeed;
	next.z += mtx[2][2] * mSpeed;

	// The Noki Bay stream cubes carry a flow direction and strength; the boat
	// picks up whatever component of it points along its own heading.
	int cubeNo = gpCubeStream->getInCubeNo(*gpMarioPos);
	if (cubeNo != -1) {
		TCubeStreamInfo* info
		    = (TCubeStreamInfo*)gpCubeStream->unk14->getChildren()[cubeNo];
		Mtx flow;
		MsMtxSetXYZRPH(flow, 0.0f, 0.0f, 0.0f, info->unk18.x, info->unk18.y,
		               info->unk18.z);
		f32 alongHeading = 0.0f;
		alongHeading += mtx[0][2] * flow[0][2];
		alongHeading += mtx[2][2] * flow[2][2];
		mSpeed += 0.0001f * (alongHeading * info->unk40);
	}

	const TBGCheckData* ground;
	f32 groundY = gpMap->checkGroundIgnoreWaterSurface(next, &ground);
	f32 waterY  = mPosition.y - mYOffset;
	if (groundY > waterY - 100.0f || ground->isIllegalData()) {
		next = mPosition;
		kill();
		mLinearVelocity.zero();
		return;
	}

	JGeometry::TVec3<f32> probe;
	probe.x = mtx[0][2] * mFrontOffset + next.x;
	probe.y = waterY;
	probe.z = mtx[2][2] * mFrontOffset + next.z;
	bindToWall(probe, mWallRadiusFront, &next);
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;

	probe.x = -(mtx[0][2] * mBackOffset - next.x);
	probe.y = mPosition.y - mYOffset;
	probe.z = -(mtx[2][2] * mBackOffset - next.z);
	bindToWall(probe, mWallRadiusBack, &next);
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;

	probe.x = next.x;
	probe.y = mPosition.y - mYOffset;
	probe.z = next.z;
	bindToWall(probe, mWallRadiusCenter, &next);
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;

	// `a = b - c` reaches the map's out-of-line TVec3::sub: operator= is one
	// inline level and the difference nested in its argument two more.
	mLinearVelocity = next - mPosition;
}

void TMuddyBoat::control()
{
	TMapObjBase::control();

	if (marioIsOn())
		moveByWater();

	switch (mState) {
	case 1:
		mSpeed *= mSpeedDecay;
		gpMSound->startSoundActorWithInfo(MSD_SE_OBJ_DORO_FLOAT, &mPosition,
		                                  nullptr, fabsf(mSpeed), 0, 0,
		                                  nullptr, 0, 4);
		if (mTurnSpeed != 0.0f) {
			mRotation.y += mTurnSpeed;
			mRotation.y = MsWrap(mRotation.y, 0.0f, 360.0f);
			mTurnSpeed *= mTurnDecay;
			if (fabsf(mTurnSpeed) < 0.0001f)
				mTurnSpeed = 0.0f;
		}
		break;

	case 2:
		if (animIsFinished()) {
			mStateTimer = mRespawnTime;
			onMapObjFlag(MAP_OBJ_FLAG_UNK100);
			sleep();
			mState = 3;
		}
		break;

	case 3:
		if (!isStateTimerEngaged()) {
			awake();
			makeObjDead();
			makeObjDefault();
			makeObjAppeared();

			JGeometry::TVec3<f32> scale(2.0f * mScaling.x, 2.0f * mScaling.y,
			                            3.0f * mScaling.z);
			mEffectPos.set(mPosition.x, mPosition.y - mYOffset, mPosition.z);
			emitAndSRT(PARTICLE_MS_ENM_DISAP_A, 0, &mEffectPos, mRotation,
			           scale);
			emitAndSRT(PARTICLE_MS_ENM_DISAP_B, 0, &mEffectPos, mRotation,
			           scale);
			gpMSound->startSoundActor(MSD_SE_SMOKE_EFFECT, &mPosition, 0,
			                          nullptr, 0, 4);
			mState = 1;
		}
		break;
	}
}

void TMuddyBoat::calc()
{
	f32 z = mPosition.z;
	f32 y = mPosition.y - mYOffset;
	f32 waveY = y + gpMapObjWave->getWaveHeight(mPosition.x, z);
	f32 yaw   = mRotation.y;
	f32 x     = mPosition.x;
	MsMtxSetXYZRPH(getModel()->getAnmMtx(0), x, waveY, z, 0,
	               (s16)(182.04445f * yaw), 0);

	// The redundant identity initialiser is the ROM's: MTXScale overwrites
	// all twelve elements right after it.
	Mtx scale;
	scale[0][0] = 1.0f;
	scale[0][1] = 0.0f;
	scale[0][2] = 0.0f;
	scale[0][3] = 0.0f;
	scale[1][0] = 0.0f;
	scale[1][1] = 1.0f;
	scale[1][2] = 0.0f;
	scale[1][3] = 0.0f;
	scale[2][0] = 0.0f;
	scale[2][1] = 0.0f;
	scale[2][2] = 1.0f;
	scale[2][3] = 0.0f;
	MTXScale(scale, mInitialScaling.x, mInitialScaling.y, mInitialScaling.z);
	MTXConcat(getModel()->getAnmMtx(0), scale, getModel()->getAnmMtx(0));

	if (mSpeed != 0.0f) {
		mEffectPos.set(mPosition.x, mPosition.y - mYOffset, mPosition.z);
		JGeometry::TVec3<f32> wakeScale(3.0f * mScaling.x, 2.0f * mScaling.y,
		                                3.0f * mScaling.z);
		emitAndBindScale(PARTICLE_MS_M_HAMON_B, 3, &mEffectPos, wakeScale);
		emitAndBindScale(PARTICLE_MS_M_HAMON_A, 1, &mEffectPos, wakeScale);
		unk16C = 0;
	}
}

u32 TMuddyBoat::getSDLModelFlag() const { return 0; }

void TMuddyBoat::initMapObj()
{
	TMapObjBase::initMapObj();

	mAccelForward  = 0.04f;
	mSpeedDecay    = 0.998f;
	mTurnAccel     = 0.002f;
	mTurnDecay     = 0.997f;
	mAccelBackward = 0.01f;
	mRespawnTime   = 600;

	if (gpMarDirector->getCurrentMap() == 52) {
		mWallRadiusFront  = 126.0f;
		mWallRadiusCenter = 185.0f;
		mWallRadiusBack   = 150.0f;
		mFrontOffset      = 170.0f;
		mBackOffset       = 185.0f;
	} else {
		mWallRadiusFront  = 100.0f;
		mWallRadiusCenter = 170.0f;
		mWallRadiusBack   = 150.0f;
		mFrontOffset      = 180.0f;
		mBackOffset       = 100.0f;
	}

	unk17C = 3.0f;
	unk180 = 2.0f;
	unk184 = 5.0f;
}

TMuddyBoat::TMuddyBoat(const char* name)
    : TMapObjBase(name)
    , mAccelForward(0.0f)
    , mAccelBackward(0.0f)
    , mSpeed(0.0f)
    , mSpeedDecay(0.0f)
    , mTurnAccel(0.0f)
    , mTurnSpeed(0.0f)
    , mTurnDecay(0.0f)
    , mWallRadiusCenter(0.0f)
    , mWallRadiusFront(0.0f)
    , mWallRadiusBack(0.0f)
    , mFrontOffset(0.0f)
    , mBackOffset(0.0f)
    , mRespawnTime(0)
    , unk16C(0)
{
	mEffectPos.zero();
	unk184 = 0.0f;
	unk180 = 0.0f;
	unk17C = 0.0f;
}

void TMareFall::calc()
{
	gpMSound->startSoundActor(MSD_SE_GE_FALL, &mPosition, 0, nullptr, 0, 4);
	gpMSound->startSoundActor(MSD_SE_GE_FALL_UPPER, &fall_upper_pos, 0,
	                          nullptr, 0, 4);

	// TODO: Particles.hpp has no names for these two; they are
	gpMarioParticleManager->emit(MAPOBJ_MAREFALLSPLASH, &mPosition, 1, this);
	gpMarioParticleManager->emit(MAPOBJ_MAREFALLSMOKE, &mPosition, 1, this);
}

void TMareFall::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);

	SMS_LoadParticle("/scene/mapObj/mareFallSplash.jpa",
	                 MAPOBJ_MAREFALLSPLASH);
	SMS_LoadParticle("/scene/mapObj/mareFallSmoke.jpa",
	                 MAPOBJ_MAREFALLSMOKE);
}

void TMareCork::loadAfter()
{
	mCannon = JDrama::TNameRefGen::search<TCannon>("砲台");
	if (mCannon->receiveMessage(this, HIT_MESSAGE_TAKE))
		mHeldObject = mCannon;

	SMS_LoadParticle("/scene/map/map/ms_mare_gunwat_a.jpa",
	                 MAP_MAP_MS_MARE_GUNWAT_A);
	SMS_LoadParticle("/scene/map/map/ms_mare_gunwat_b.jpa",
	                 MAP_MAP_MS_MARE_GUNWAT_B);
	SMS_LoadParticle("/scene/map/map/ms_mare_gunwat_c.jpa",
	                 MAP_MAP_MS_MARE_GUNWAT_C);

	TMapObjBase::loadAfter();

	mEffectPos.set(0.0f, 0.0f, 0.0f);
	initAnmSound();
}

void TMareCork::moveObject()
{
	if (mCannon->isObject() && !mIsBlownOut) {
		getMActor()->setBck("marecork");
		setAnmSound("/scene/mapObj/marecork.bas");
		removeMapCollision();
		mIsBlownOut = true;
	}
}

void TMareCork::calcRootMatrix()
{
	if (mIsBlownOut) {
		getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(350.0f);
		if (getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(250.0f)) {
			mCannon->startChorobeiShout();
			gpItemManager->makeShineAppearWithDemo(
			    "シャイン（ボス用）", "ボスシャインカメラ", mPosition.x,
			    mPosition.y, mPosition.z);

			mSoundPos.set(2773.0f, 8618.0f, 7006.0f);
			JPABaseEmitter* emitter = gpMarioParticleManager->emitWithRotate(
			    PARTICLE_MS_M_SPHIPD_HIT_B, &mSoundPos, 0x4000, 0xD82, 0, 0,
			    nullptr);
			if (emitter != nullptr) {
				emitter->setGlobalScale(
				    JGeometry::TVec3<f32>(2.5f, 2.5f, 2.5f));
			}
		}
	}

	TMapObjBase::calcRootMatrix();
}

MtxPtr TMareCork::getTakingMtx()
{
	return getMActor()->getModel()->getAnmMtx(2);
}

void TMareCork::drawObject(JDrama::TGraphics* graphics)
{
	TLiveActor::drawObject(graphics);

	if (mIsBlownOut
	    && getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame() > 250.0f) {
		mSoundPos.set(2773.0f, 8618.0f, 7006.0f);
		gpMSound->startSoundActor(MSD_SE_ENV_FALL_JET_LEVEL, &mSoundPos);

		gpMarioParticleManager->emitAndBindToPosPtr(
		    MAP_MAP_MS_MARE_GUNWAT_A, &mEffectPos, 1, this);
		gpMarioParticleManager->emitAndBindToPosPtr(
		    MAP_MAP_MS_MARE_GUNWAT_B, &mEffectPos, 1, this);
		gpMarioParticleManager->emitAndBindToPosPtr(
		    MAP_MAP_MS_MARE_GUNWAT_C, &mEffectPos, 1, this);
	}
}

BOOL TMareEventPoint::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		// Only clean water counts, and only once the spray has slowed down
		// enough to be a deliberate hose rather than a passing jet.
		if (!gpModelWaterManager->checkFlagBottom4Bits(
		        TMapObjBase::getWaterID(sender), 1)) {
			if (TMapObjBase::getWaterPlane(sender) != nullptr
			    && TMapObjBase::getWaterPlane(sender)->mNormal.y < 0.1f) {
				if (mDepressWall->startEvent()) {
					gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT,
					                             &sender->mPosition, 0,
					                             nullptr);
					gpMSound->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK,
					                        &mPosition, 0, 0.0f, 0, 0, 4);
				}
				return true;
			}
		}
	}

	return false;
}

void TMareEventPoint::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);
	initHitActor(0x40000236, 0, 0, 0.0f, 0.0f, 300.0f, 600.0f);
}
