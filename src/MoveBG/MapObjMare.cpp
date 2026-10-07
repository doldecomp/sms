#include <Camera/CubeManagerBase.hpp>
#include <Enemy/Cannon.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/JStage/JSGActor.hpp>
#include <JSystem/JStage/JSGObject.hpp>
#include <JSystem/JSupport/JSUInputStream.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapData.hpp>
#include <Map/MapEventMare.hpp>
#include <Map/MapWireManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjMare.hpp>
#include <MoveBG/MapObjWave.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Player/WaterGun.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/TakeActor.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <dolphin/gx.h>
#include <macros.h>
#include <math.h>
#include <string.h>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

f32 TCogwheelScale::mWaterLeakSpeed = 0.01f;

static void dummy(Vec* v) { *v = (Vec) { 0.0f, 0.0f, 0.0f }; }
static void dummy2(Vec* v) { *v = (Vec) { 1.0f, 1.0f, 1.0f }; }

static f32 sRadius = 800.0f;

f32 TCogwheel::mRopeWidthX = 10.0f;
f32 TCogwheel::mRopeWidthZ = 7.0f;
f32 TCogwheel::mTexPosRate = 0.01f;
f32 TCogwheel::mMinSpeed   = 3.0f;

static f32 mGrowStartFrame = 90.0f;
static f32 mGrowEndFrame   = 175.0f;

u32 TCogwheelScale::touchWater(THitActor* param_1)
{
	if (unk140 < unk144)
		unk140 += 1.0f;
	return 1;
}

BOOL TCogwheelScale::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 == HIT_MESSAGE_HIP_DROP) {
		unk158->unk138 += unk150;
		return TRUE;
	}
	return TMapObjBase::receiveMessage(param_1, param_2);
}

void TCogwheelScale::touchPlayer(THitActor* param_1)
{
	if (marioIsOn())
		unk148 = unk13C;

	if (mPosition.y - mYOffset > 150.0f + SMS_GetMarioPos().y) {
		if ((unk154 && unk158->unk138 > 0.0f)
		    || (!unk154 && unk158->unk138 < 0.0f)) {
			unk158->rebound();
			if (marioHeadAttack())
				unk158->unk138
				    = unk14C * (unk158->unk138 * SMS_GetMarioSpeedY());
		}
	}
	unk140 = 0.0f;
}

void TCogwheelScale::control()
{
	unk148 = 0.0f;
	TMapObjBase::control();
	if (unk140 > 0.0f) {
		unk140 -= mWaterLeakSpeed;
		SMSGetMSound()->startSoundActorWithInfo(
		    MSD_SE_OBJ_MR_TSUBO_WATER, &mPosition, nullptr, abs(unk140), 0, 0,
		    nullptr, 0, 4);
		if (unk140 < 0.0f)
			unk140 = 0.0f;
	}
}

TCogwheelScale::TCogwheelScale(const char* param_1)
    : TMapObjBase(param_1)
    , unk138(0.0f)
    , unk13C(0.0f)
    , unk140(0.0f)
    , unk144(0.0f)
    , unk148(0.0f)
    , unk14C(0.01f)
    , unk150(5.0f)
    , unk154(0)
    , unk158(nullptr)
{
}

void TCogwheel::initDraw() const
{
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXLoadPosMtxImm(j3dSys.getViewMtx(), GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0, 0, 100, 255 });
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
	f32 yBot = unk154.y;
	f32 dy   = unk150->mPosition.y - unk150->mYOffset;
	f32 yTop = 600.0f + dy;
	f32 x1   = unk154.x + mRopeWidthX;
	f32 x0   = unk154.x - mRopeWidthX;
	f32 z1   = unk154.z + mRopeWidthZ;
	f32 z0   = unk154.z - mRopeWidthZ;
	f32 vTop = mTexPosRate * (yTop - dy);
	f32 vBot = mTexPosRate * (yBot - dy);
	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 8);
	GXPosition3f32(x0, yTop, z0);
	GXTexCoord2f32(0.0f, vTop);
	GXPosition3f32(x0, yBot, z0);
	GXTexCoord2f32(0.0f, vBot);
	GXPosition3f32(x1, yTop, z1);
	GXTexCoord2f32(1.0f, vTop);
	GXPosition3f32(x1, yBot, z1);
	GXTexCoord2f32(1.0f, vBot);
	GXPosition3f32(x1, yTop, z0);
	GXTexCoord2f32(2.0f, vTop);
	GXPosition3f32(x1, yBot, z0);
	GXTexCoord2f32(2.0f, vBot);
	GXPosition3f32(x0, yTop, z1);
	GXTexCoord2f32(3.0f, vTop);
	GXPosition3f32(x0, yBot, z1);
	GXTexCoord2f32(3.0f, vBot);
	GXEnd();
	dy   = unk164->mPosition.y - unk164->mYOffset;
	yTop = 1200.0f + dy;
	x1   = unk168.x + mRopeWidthX;
	x0   = unk168.x - mRopeWidthX;
	z1   = unk168.z + mRopeWidthZ;
	z0   = unk168.z - mRopeWidthZ;
	vBot = mTexPosRate * (yBot - dy);
	vTop = mTexPosRate * (yTop - dy);
	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 8);
	GXPosition3f32(x0, yTop, z0);
	GXTexCoord2f32(0.0f, vTop);
	GXPosition3f32(x0, yBot, z0);
	GXTexCoord2f32(0.0f, vBot);
	GXPosition3f32(x1, yTop, z1);
	GXTexCoord2f32(1.0f, vTop);
	GXPosition3f32(x1, yBot, z1);
	GXTexCoord2f32(1.0f, vBot);
	GXPosition3f32(x1, yTop, z0);
	GXTexCoord2f32(0.0f, vTop);
	GXPosition3f32(x1, yBot, z0);
	GXTexCoord2f32(0.0f, vBot);
	GXPosition3f32(x0, yTop, z1);
	GXTexCoord2f32(1.0f, vTop);
	GXPosition3f32(x0, yBot, z1);
	GXTexCoord2f32(1.0f, vBot);
	GXEnd();
}

void TCogwheel::rebound()
{
	unk138 *= -unk148;
	if (fabsf(unk138) < mMinSpeed)
		unk138 = 0.0f;
}

void TCogwheel::calc()
{
	Mtx rot_z;
	Mtx rot_y;

	f32 length  = 3.14f * (2.0f * sRadius);
	f32 rate    = -unk13C / length;
	mRotation.z = 360.0f * rate;

	makeRootMtxRotZ(rot_z);
	rot_z[0][3] = 0.0f;
	rot_z[1][3] = 0.0f;
	rot_z[2][3] = 0.0f;

	makeRootMtxRotY(rot_y);
	rot_y[0][3] = 0.0f;
	rot_y[1][3] = 0.0f;
	rot_y[2][3] = 0.0f;

	MtxPtr mtx = getModel()->getAnmMtx(0);
	MTXConcat(rot_y, rot_z, mtx);
	mtx[0][3] = mPosition.x;
	mtx[1][3] = mPosition.y;
	mtx[2][3] = mPosition.z;
}

void TCogwheel::control()
{
	TMapObjBase::control();
	unk13C += unk138;
	f32 panR = unk164->unk138 + unk164->unk140 + unk164->unk148;
	f32 panL = unk150->unk138 + unk150->unk140 + unk150->unk148;
	unk138 += (panL - panR) * unk140;
	unk138 *= unk144;
	if (unk13C < unk160 && unk138 < 0.0f) {
		rebound();
	}
	if (unk13C > unk14C - unk174 && unk138 > 0.0f) {
		rebound();
	}
	unk150->mPosition.y = mPosition.y - unk13C + unk150->mYOffset;
	unk164->mPosition.y = mPosition.y - (unk14C - unk13C);
	if (fabsf(unk138) > mMinSpeed) {
		SMSGetMSound()->startSoundActorWithInfo(
		    MSD_SE_OBJ_MR_TSUBO_PULL, &mPosition, nullptr, fabsf(unk138), 0, 0,
		    nullptr, 0, 4);
	}
}

void TCogwheel::initMapObj()
{
	TMapObjBase::initMapObj();
	JGeometry::TVec2<f32> off(sRadius, 0.0f);
	off.rotate(DEG_TO_RAD(mRotation.y));
	f32 dx = off.x;
	f32 dz = off.y;
	JGeometry::TVec3<f32> pos(mPosition.x + dx, mPosition.y, mPosition.z - dz);
	TCogwheelScale* plate
	    = (TCogwheelScale*)TMapObjBaseManager::newAndRegisterObj(
	        "cogwheel_plate", pos, mRotation,
	        JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	unk150         = plate;
	unk150->unk154 = 1;
	unk150->unk158 = this;
	plate->appear();
	unk154.set(pos.x, mPosition.y, pos.z);
	pos.set(mPosition.x - dx, mPosition.y, mPosition.z + dz);
	TCogwheelScale* pot
	    = (TCogwheelScale*)TMapObjBaseManager::newAndRegisterObj(
	        "cogwheel_pot", pos, mRotation,
	        JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	unk164         = pot;
	unk164->unk154 = 0;
	unk164->unk158 = this;
	pot->appear();
	unk168.set(pos.x, mPosition.y, pos.z);
	if (strcmp(getName(), "天秤上") == 0) {
		unk140         = 0.003f;
		unk144         = 0.99f;
		unk148         = 0.8f;
		unk14C         = 3800.0f;
		unk160         = 1000.0f;
		unk174         = 1800.0f;
		unk164->unk138 = 0.0f;
		unk164->unk13C = 0.0f;
		unk164->unk144 = 14.0f;
		unk150->unk138 = 10.0f;
		unk150->unk13C = 0.0f;
		unk150->unk144 = 0.0f;
	} else {
		unk140         = 0.008f;
		unk144         = 0.98f;
		unk148         = 0.8f;
		unk14C         = 3950.0f;
		unk160         = 1000.0f;
		unk174         = 1900.0f;
		unk164->unk138 = 0.0f;
		unk164->unk13C = 0.0f;
		unk164->unk144 = 14.0f;
		unk150->unk138 = 10.0f;
		unk150->unk13C = 0.0f;
		unk150->unk144 = 0.0f;
	}
	unk13C = unk14C / 2.0f;
}

TCogwheel::TCogwheel(const char* param_1)
    : TMapObjBase(param_1)
    , unk138(0.0f)
    , unk13C(0.0f)
    , unk140(0.0f)
    , unk144(0.0f)
    , unk148(0.0f)
    , unk14C(0.0f)
    , unk150(nullptr)
    , unk160(0.0f)
    , unk164(nullptr)
    , unk174(0.0f)
{
	unk154.zero();
	unk168.zero();
}

void TMapObjElasticCode::draw() const
{
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXLoadPosMtxImm(j3dSys.getViewMtx(), GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0, 0, 100, 255 });
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
	GXPosition3f32(mInitialPosition.x, mInitialPosition.y + 1000.0f,
	               mInitialPosition.z);
	GXPosition3f32(mPosition.x, mPosition.y, mPosition.z);
	GXEnd();
}

void TMapObjElasticCode::control()
{
	TMapObjBase::control();
	mVelocity.y *= unk140;
	mVelocity.y += unk13C * (mInitialPosition.y - mPosition.y) - getGravityY();
	if (mHeldObject != nullptr) {
		mVelocity.y -= unk138;
		JGeometry::TVec3<f32> newPos = mHeldObject->mPosition;
		newPos.y += mVelocity.y;
		mHeldObject->moveRequest(newPos);
	}
	mPosition.y += mVelocity.y;
}

void TMapObjElasticCode::initMapObj()
{
	TMapObjBase::initMapObj();
	unk140   = 0.997f;
	mGravity = 0.01f;
	unk138   = 2.0f;
	unk13C   = 0.0005f;
}

f32 TMapObjGrowTree::getGrowHeightFromRate(f32 rate) const
{
	if (mGrowStartFrame < mMActor->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
	    && mMActor->getFrameCtrl(ANM_TYPE_BCK)->getFrame() < mGrowEndFrame)
		return rate * unk138 / (mGrowEndFrame - mGrowStartFrame);
	return 0.0f;
}

void TMapObjGrowTree::updateHeight()
{
	if (mMActor->getFrameCtrl(ANM_TYPE_BCK)->getFrame() > mGrowStartFrame) {
		if (mMActor->getFrameCtrl(ANM_TYPE_BCK)->getFrame() > mGrowEndFrame)
			setDamageHeight(unk138);
		else
			setDamageHeight(
			    unk148
			    + (unk138 - unk148)
			          * (mMActor->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
			             - mGrowStartFrame)
			          / (mGrowEndFrame - mGrowStartFrame));
	} else {
		setDamageHeight(unk148);
	}
}

u32 TMapObjGrowTree::touchWater(THitActor* water)
{
	if (water->mPosition.y > mPosition.y + unk148)
		return 0;
	if (isState(STATE_NORMAL)) {
		startAnim(1);
		mMActor->getFrameCtrl(ANM_TYPE_BCK)->setRate(0.0f);
		mState = STATE_UNK2;
	}
	if (mMActor->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
	    < mMActor->getFrameCtrl(ANM_TYPE_BCK)->getEnd()) {
		soundBas(MSD_SE_OBJ_SANDBOMB_WATER_1, 3.0f, unk13C);
		soundBas(MSD_SE_OBJ_SANDBOMB_WATER_2, 67.0f, unk13C);
		soundBas(MSD_SE_OBJ_SANDBOMB_WATER_3, 103.0f, unk13C);
		soundBas(MSD_SE_OBJ_SANDBOMB_WATER_4, 137.0f, unk13C);
		mMActor->getFrameCtrl(ANM_TYPE_BCK)
		    ->setFrame(unk13C
		               + mMActor->getFrameCtrl(ANM_TYPE_BCK)->getFrame());
		updateHeight();
		if (mHeldObject) {
			JGeometry::TVec3<f32> pos = mHeldObject->mPosition;
			pos.y += getGrowHeightFromRate(unk13C);
			mHeldObject->moveRequest(pos);
		}
	}
	if (mMActor->getFrameCtrl(ANM_TYPE_BCK)->getFrame() > mGrowEndFrame) {
		setUpMapCollision(0);
		mStateTimer = unk144;
	}
	return 1;
}

void TMapObjGrowTree::control()
{
	TMapObjBase::control();
	if (isState(STATE_UNK2) && mColCount == 0 && !isStateTimerEngaged()
	    && mMActor->getFrameCtrl(ANM_TYPE_BCK)->getFrame() > 0.0f) {
		if (mMActor->getFrameCtrl(ANM_TYPE_BCK)->getFrame() < mGrowEndFrame)
			removeMapCollision();
		mMActor->getFrameCtrl(ANM_TYPE_BCK)
		    ->setFrame(mMActor->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
		               - unk140);
		if (mMActor->getFrameCtrl(ANM_TYPE_BCK)->getFrame() < 0.0f) {
			startAnim(0);
			mState = STATE_NORMAL;
		} else {
			f32 frame = mMActor->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
			if (67.0f <= frame && frame <= 240.0f)
				SMSGetMSound()->startSoundActor(MSD_SE_OBJ_SAMDBOMB_REVERSE,
				                                &mPosition, 0, nullptr, 0, 4);
			updateHeight();
			if (mHeldObject) {
				JGeometry::TVec3<f32> pos = mHeldObject->mPosition;
				pos.y -= getGrowHeightFromRate(unk140);
				mHeldObject->moveRequest(pos);
			}
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
	unk138 = 1000.0f;
	unk13C = 0.5f;
	unk140 = 0.1f;
	unk144 = 360;
	unk148 = mDamageHeight;
	mMActor->setBtp("moyasi_wink");
}

TMapObjGrowTree::TMapObjGrowTree(const char* param_1)
    : TMapObjBase(param_1)
    , unk138(0.0f)
    , unk13C(0.0f)
    , unk140(0.0f)
    , unk144(0)
    , unk148(0.0f)
{
}

void TWireBell::initDraw() const
{
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXLoadPosMtxImm(j3dSys.getViewMtx(), GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0, 0, 100, 255 });
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

	f32 xPlus  = unk14C.x + unk140;
	f32 xMinus = unk14C.x - unk140;
	f32 zPlus  = unk14C.z + unk144;
	f32 zMinus = unk14C.z - unk144;

	f32 bottomY = mPosition.y;
	f32 topY    = unk14C.y;

	f32 topTexCoordY    = unk148 * (topY - bottomY);
	f32 bottomTexCoordY = unk148 * (bottomY - mPosition.y);

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 8);
	GXPosition3f32(xMinus, topY, zMinus);
	GXTexCoord2f32(0.0f, topTexCoordY);
	GXPosition3f32(xMinus, bottomY, zMinus);
	GXTexCoord2f32(0.0f, bottomTexCoordY);
	GXPosition3f32(xPlus, topY, zPlus);
	GXTexCoord2f32(1.0f, topTexCoordY);
	GXPosition3f32(xPlus, bottomY, zPlus);
	GXTexCoord2f32(1.0f, bottomTexCoordY);
	GXPosition3f32(xPlus, topY, zMinus);
	GXTexCoord2f32(0.0f, topTexCoordY);
	GXPosition3f32(xPlus, bottomY, zMinus);
	GXTexCoord2f32(0.0f, bottomTexCoordY);
	GXPosition3f32(xMinus, topY, zPlus);
	GXTexCoord2f32(1.0f, topTexCoordY);
	GXPosition3f32(xMinus, bottomY, zPlus);
	GXTexCoord2f32(1.0f, bottomTexCoordY);
	GXEnd();
}

void TWireBell::control()
{
	gpMapWireManager->getPointPosInNthWire(unk138, mPosition, &unk14C);
	mPosition.set(unk14C.x, unk14C.y - unk13C, unk14C.z);
	Mtx mtx;
	MsMtxSetTRS(mtx, mPosition, mRotation, mScaling);
	MTXCopy(mtx, getModel()->getAnmMtx(0));
}

void TWireBell::loadAfter()
{
	TMapObjBase::loadAfter();
	unk138 = gpMapWireManager->getWireNo(mPosition);
}

TWireBell::TWireBell(const char* param_1)
    : TMapObjBase(param_1)
    , unk138(-1)
    , unk13C(200.0f)
    , unk140(10.0f)
    , unk144(5.0f)
    , unk148(0.01f)
{
	unk14C.zero();
}

void TMapObjPuncher::touchPlayer(THitActor* param_1)
{
	awake();
	startAnim(1);
	JGeometry::TVec3<f32> localZ;
	makeVecToLocalZ(1.0f, &localZ);
	JGeometry::TVec3<f32> marioPos = SMS_GetMarioPos();
	marioPos += localZ * 100.0f;
	SMS_MarioMoveRequest(marioPos);
	SMS_SendMessageToMario(this, HIT_MESSAGE_THROWN);
	SMS_ThrowMario(localZ, unk138);
	onHitFilter(HIT_FILTER_NO_COLLISION);
	JGeometry::TVec3<f32> scale(2.0f, 2.0f, 2.0f);
	emitAndScale(PARTICLE_MS_ENM_DISAP_A_W, 0, &mPosition, scale);
	emitAndScale(PARTICLE_MS_ENM_DISAP_B, 0, &mPosition, scale);
	SMSGetMSound()->startSoundActor(MSD_SE_SMOKE_EFFECT, &mPosition, 0, nullptr,
	                                0, 4);
	mState = STATE_UNK2;
}

void TMapObjPuncher::control()
{
	TMapObjBase::control();
	switch (mState) {
	case STATE_NORMAL:
		break;
	case STATE_UNK2:
		soundBas(MSD_SE_OBJ_PUNCHER_RETURN, 101.0f,
		         mMActor->getFrameCtrl(ANM_TYPE_BCK)->getRate());
		if (animIsFinished()) {
			JGeometry::TVec3<f32> scale(2.0f, 2.0f, 2.0f);
			emitAndScale(PARTICLE_MS_ENM_DISAP_A_W, 0, &mPosition, scale);
			emitAndScale(PARTICLE_MS_ENM_DISAP_B, 0, &mPosition, scale);
			SMSGetMSound()->startSoundActor(MSD_SE_SMOKE_EFFECT, &mPosition, 0,
			                                nullptr, 0, 4);
			kill();
		}
		break;
	}
}

void TMapObjPuncher::load(JSUMemoryInputStream& param_1)
{
	s32 value;
	TMapObjBase::load(param_1);
	param_1 >> value;
	unk138 = value;
	sleep();
	offHitFilter(HIT_FILTER_NO_COLLISION);
}

void TMuddyBoat::moveByWater()
{
	if (SMS_GetMarioWaterGun()->mIsEmitWater != 0) {
		MtxPtr emitMtx = SMS_GetMarioWaterGun()->getEmitMtx(0);
		JGeometry::TVec3<f32> dir(-emitMtx[0][0], 0.0f, -emitMtx[2][0]);
		MsVECNormalize(&dir, &dir);

		MtxPtr modelMtx = getModel()->getAnmMtx(0);
		JGeometry::TVec3<f32> fwd(modelMtx[0][2], 0.0f, modelMtx[2][2]);

		f32 dot = dir.dot(fwd);

		JGeometry::TVec3<f32> normal;
		getNormalVecFromTargetXZ(SMS_GetMarioPos().x, SMS_GetMarioPos().z,
		                         &normal);
		if (normal.x != 0.0f || normal.z != 0.0f)
			MsVECNormalize(&normal, &normal);

		JGeometry::TVec3<f32> delta;
		delta.sub(dir, fwd);
		JGeometry::TVec3<f32> turnVec;
		turnVec.cross(fwd, delta);
		f32 turn = turnVec.y * fwd.dot(normal);
		if (turn > 0.0f)
			unk14C += unk148 * (1.0f - fabsf(dot));
		else
			unk14C -= unk148 * (1.0f - fabsf(dot));

		if (dot > 0.0f)
			unk140 += dot * unk138;
		else
			unk140 += dot * unk13C;
		offLiveFlag(LIVE_FLAG_UNK10);
	}
}

void TMuddyBoat::calcRootMatrix() { }

void TMuddyBoat::kill()
{
	unk140 = 0.0f;
	unk14C = 0.0f;
	SMS_EasyEmitParticle(PARTICLE_MS_M_AMIATTACK, &unk170, nullptr,
	                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_DORO_BROKEN, &mPosition, 0,
	                                nullptr, 0, 4);
	MtxPtr dst = getModel()->getBaseTRMtx();
	MtxPtr src = getModel()->getAnmMtx(0);
	MTXCopy(src, dst);
	offMapObjFlag(MAP_OBJ_FLAG_NO_ANIMATIONS);
	onLiveFlag(LIVE_FLAG_UNK10);
	startAnim(1);
	startAnim(2);
	mState = STATE_UNK2;
}

void TMuddyBoat::touchWall(JGeometry::TVec3<f32>* param_1,
                           const TBGWallCheckRecord& param_2)
{
	const TBGCheckData* wall = param_2.mResultWalls[0];
	unk170                   = JGeometry::TVec3<f32>(
        param_2.mCenter.x - wall->getNormal().x * (50.0f + param_2.mRadius),
        100.0f + (mPosition.y - getObjCollisionHeightOffset()),
        param_2.mCenter.z - wall->getNormal().z * (50.0f + param_2.mRadius));
	*param_1 = mPosition;
	kill();
	mPositionDelta.zero();
}

bool TMuddyBoat::bindToWall(const JGeometry::TVec3<f32>& param_1, f32 param_2,
                            JGeometry::TVec3<f32>* param_3)
{
	TBGWallCheckRecord record(param_1, param_2, 4,
	                          TBGWallCheckRecord::DONT_MOVE_XZ);
	if (gpMap->isTouchedWallsAndMoveXZ(&record)) {
		touchWall(param_3, record);
		return true;
	}
	return false;
}

void TMuddyBoat::bind()
{
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;
	JGeometry::TVec3<f32> pos = mPosition;
	MtxPtr mtx                = getModel()->getAnmMtx(0);
	pos.x += mtx[0][2] * unk140;
	pos.z += mtx[2][2] * unk140;
	int cubeNo = gpCubeStream->getInCubeNo(SMS_GetMarioPos());
	if (cubeNo != -1) {
		TCubeStreamInfo& cube
		    = (TCubeStreamInfo&)(*gpCubeStream->unk14)[cubeNo];
		Mtx streamMtx;
		MsMtxSetXYZRPH(streamMtx, 0.0f, 0.0f, 0.0f, cube.unk18.x, cube.unk18.y,
		               cube.unk18.z);
		JGeometry::TVec3<f32> direction(mtx[0][2], 0.0f, mtx[2][2]);
		JGeometry::TVec3<f32> streamDirection(streamMtx[0][2], 0.0f,
		                                      streamMtx[2][2]);
		unk140 += 0.0001f * (direction.dot(streamDirection) * cube.unk40);
	}
	const TBGCheckData* ground;
	f32 height = gpMap->checkGroundIgnoreWaterSurface(pos, &ground);
	f32 bottom = mPosition.y - getObjCollisionHeightOffset();
	if (height > bottom - 100.0f || ground->isIllegalData()) {
		pos = mPosition;
		kill();
		mPositionDelta.zero();
		return;
	}
	JGeometry::TVec3<f32> center;
	center.set(pos.x + mtx[0][2] * unk160, bottom, pos.z + mtx[2][2] * unk160);
	if (bindToWall(center, unk158, &pos))
		return;
	center.set(pos.x - mtx[0][2] * unk164,
	           mPosition.y - getObjCollisionHeightOffset(),
	           pos.z - mtx[2][2] * unk164);
	if (bindToWall(center, unk15C, &pos))
		return;
	center.set(pos.x, mPosition.y - getObjCollisionHeightOffset(), pos.z);
	if (bindToWall(center, unk154, &pos))
		return;
	mPositionDelta = pos - mPosition;
}

void TMuddyBoat::control()
{
	TMapObjBase::control();
	if (marioIsOn())
		moveByWater();
	switch (mState) {
	case STATE_NORMAL: {
		unk140 *= unk144;
		SMSGetMSound()->startSoundActorWithInfo(
		    MSD_SE_OBJ_DORO_FLOAT, &mPosition, nullptr, fabsf(unk140), 0, 0,
		    nullptr, 0, 4);
		if (unk14C == 0.0f)
			break;
		mRotation.y += unk14C;
		mRotation.y = MsWrap(mRotation.y, 0.0f, 360.0f);
		unk14C *= unk150;
		if (fabsf(unk14C) < 0.0001f)
			unk14C = 0.0f;
		break;
	}
	case STATE_UNK2: {
		if (!animIsFinished())
			break;
		mStateTimer = unk168;
		onMapObjFlag(MAP_OBJ_FLAG_NO_ANIMATIONS);
		sleep();
		mState = STATE_UNK3;
		break;
	}
	case STATE_UNK3: {
		if (isStateTimerEngaged())
			break;
		awake();
		makeObjDead();
		makeObjDefault();
		makeObjAppeared();
		JGeometry::TVec3<f32> scale(2.0f * mScaling.x, 2.0f * mScaling.y,
		                            3.0f * mScaling.z);
		unk170.set(mPosition.x, mPosition.y - mYOffset, mPosition.z);
		emitAndSRT(PARTICLE_MS_ENM_DISAP_A, 0, &unk170, mRotation, scale);
		emitAndSRT(PARTICLE_MS_ENM_DISAP_B, 0, &unk170, mRotation, scale);
		SMSGetMSound()->startSoundActor(MSD_SE_SMOKE_EFFECT, &mPosition, 0,
		                                nullptr, 0, 4);
		mState = STATE_NORMAL;
		break;
	}
	}
}

void TMuddyBoat::calc()
{
	f32 waveY = mPosition.y - mYOffset;
	waveY += gpMapObjWave->getWaveHeight(mPosition.x, mPosition.z);
	MsMtxSetXYZRPH(getModel()->getAnmMtx(0), mPosition.x, waveY, mPosition.z,
	               0.0f, mRotation.y, 0.0f);

	JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > scaleMtx;
	scaleMtx.identity();
	MTXScale(scaleMtx, mInitialScaling.x, mInitialScaling.y, mInitialScaling.z);

	MTXConcat(getModel()->getAnmMtx(0), scaleMtx, getModel()->getAnmMtx(0));

	if (unk140 != 0.0f) {
		unk170.set(mPosition.x, mPosition.y - mYOffset, mPosition.z);
		JGeometry::TVec3<f32> scale(3.0f * mScaling.x, 2.0f * mScaling.y,
		                            3.0f * mScaling.z);
		emitAndBindScale(PARTICLE_MS_M_HAMON_B, 3, &unk170, scale);
		emitAndBindScale(PARTICLE_MS_M_HAMON_A, 1, &unk170, scale);
		unk16C = 0;
	}
}

u32 TMuddyBoat::getSDLModelFlag() const { return 0; }

void TMuddyBoat::initMapObj()
{
	TMapObjBase::initMapObj();
	unk138 = 0.04f;
	unk144 = 0.998f;
	unk148 = 0.002f;
	unk150 = 0.997f;
	unk13C = 0.01f;
	unk168 = 600;

	if (SMSGetMarDirector()->getCurrentMap() == 0x34) {
		unk158 = 126.0f;
		unk154 = 185.0f;
		unk15C = 150.0f;
		unk160 = 170.0f;
		unk164 = 185.0f;
	} else {
		unk158 = 100.0f;
		unk154 = 170.0f;
		unk15C = 150.0f;
		unk160 = 180.0f;
		unk164 = 100.0f;
	}

	unk17C.set(3.0f, 2.0f, 5.0f);
}

TMuddyBoat::TMuddyBoat(const char* param_1)
    : TMapObjBase(param_1)
    , unk138(0.0f)
    , unk13C(0.0f)
    , unk140(0.0f)
    , unk144(0.0f)
    , unk148(0.0f)
    , unk14C(0.0f)
    , unk150(0.0f)
    , unk154(0.0f)
    , unk158(0.0f)
    , unk15C(0.0f)
    , unk160(0.0f)
    , unk164(0.0f)
    , unk168(0)
    , unk16C(0)
{
	unk170.zero();
	unk17C.zero();
}

static JGeometry::TVec3<f32> fall_upper_pos(2827.0f, 8604.0f, 7202.0f);

void TMareFall::calc()
{
	SMSGetMSound()->startSoundActor(MSD_SE_GE_FALL, &mPosition, 0, nullptr, 0,
	                                4);
	SMSGetMSound()->startSoundActor(MSD_SE_GE_FALL_UPPER, &fall_upper_pos, 0,
	                                nullptr, 0, 4);
	gpMarioParticleManager->emit(MAPOBJ_MAREFALLSPLASH, &mPosition, 1, this);
	gpMarioParticleManager->emit(MAPOBJ_MAREFALLSMOKE, &mPosition, 1, this);
}

void TMareFall::load(JSUMemoryInputStream& param_1)
{
	TMapObjBase::load(param_1);

	SMS_LoadParticle("/scene/mapObj/mareFallSplash.jpa", MAPOBJ_MAREFALLSPLASH);
	SMS_LoadParticle("/scene/mapObj/mareFallSmoke.jpa", MAPOBJ_MAREFALLSMOKE);
}

void TMareCork::loadAfter()
{
	unk138 = static_cast<TCannon*>(JDrama::TNameRefGen::search("砲台"));
	if (unk138->receiveMessage(this, HIT_MESSAGE_TAKE))
		mHeldObject = unk138;

	SMS_LoadParticle("/scene/map/map/ms_mare_gunwat_a.jpa",
	                 MAP_MAP_MS_MARE_GUNWAT_A);
	SMS_LoadParticle("/scene/map/map/ms_mare_gunwat_b.jpa",
	                 MAP_MAP_MS_MARE_GUNWAT_B);
	SMS_LoadParticle("/scene/map/map/ms_mare_gunwat_c.jpa",
	                 MAP_MAP_MS_MARE_GUNWAT_C);

	TMapObjBase::loadAfter();
	unk13C.set(0.0f, 0.0f, 0.0f);
	initAnmSound();
}

void TMareCork::moveObject()
{
	if (unk138->isObject() && !unk154) {
		mMActor->setBck("marecork");
		setAnmSound("/scene/mapObj/marecork.bas");
		removeMapCollision();
		unk154 = 1;
	}
}

void TMareCork::calcRootMatrix()
{
	if (unk154) {
		mMActor->getFrameCtrl(ANM_TYPE_BCK)->checkPass(350.0f);
		if (mMActor->getFrameCtrl(ANM_TYPE_BCK)->checkPass(250.0f)) {
			unk138->startChorobeiShout();
			gpItemManager->makeShineAppearWithDemo(
			    "シャイン（ボス用）", "ボスシャインカメラ", mPosition.x,
			    mPosition.y, mPosition.z);
			unk148.set(2773.0f, 8618.0f, 7006.0f);
			JPABaseEmitter* emitter = gpMarioParticleManager->emitWithRotate(
			    PARTICLE_MS_M_SPHIPD_HIT_B, &unk148, 0x4000, 0xD82, 0, 0,
			    nullptr);
			if (emitter)
				emitter->setGlobalScale(JGeometry::TVec3<f32>(2.5f));
		}
	}
	TMapObjBase::calcRootMatrix();
}

MtxPtr TMareCork::getTakingMtx() { return mMActor->getModel()->getAnmMtx(2); }

void TMareCork::drawObject(JDrama::TGraphics* param_1)
{
	TLiveActor::drawObject(param_1);
	if (unk154) {
		if (mMActor->getFrameCtrl(ANM_TYPE_BCK)->getFrame() > 250.0f) {
			unk148.set(2773.0f, 8618.0f, 7006.0f);
			SMSGetMSound()->startSoundActor(MSD_SE_ENV_FALL_JET_LEVEL, &unk148,
			                                0, nullptr, 0, 4);
			gpMarioParticleManager->emitAndBindToPosPtr(
			    MAP_MAP_MS_MARE_GUNWAT_A, &unk13C, 1, this);
			gpMarioParticleManager->emitAndBindToPosPtr(
			    MAP_MAP_MS_MARE_GUNWAT_B, &unk13C, 1, this);
			gpMarioParticleManager->emitAndBindToPosPtr(
			    MAP_MAP_MS_MARE_GUNWAT_C, &unk13C, 1, this);
		}
	}
}

BOOL TMareEventPoint::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 == HIT_MESSAGE_SPRAYED_BY_WATER) {
		if (!gpModelWaterManager->checkFlagBottom4Bits(
		        TMapObjBase::getWaterID(param_1), 1)
		    && TMapObjBase::getWaterPlane(param_1)) {
			f32 normal_y = TMapObjBase::getWaterPlane(param_1)->getNormal().y;
			if (normal_y < 0.1f) {
				if (unk68->startEvent()) {
					gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT,
					                             &param_1->mPosition, 0,
					                             nullptr);
					SMSGetMSound()->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK,
					                              &mPosition, 0, 0.0f, 0, 0, 4);
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}

void TMareEventPoint::load(JSUMemoryInputStream& param_1)
{
	TActor::load(param_1);
	initHitActor(ACTOR_TYPE_MARE_EVENT_POINT, 0, 0, 0.0f, 0.0f, 300.0f, 600.0f);
}
