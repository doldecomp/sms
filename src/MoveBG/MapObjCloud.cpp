#include <MoveBG/MapObjCloud.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Enemy/Graph.hpp>
#include <Player/MarioAccess.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

f32 TRideCloud::mMoveSpeedDefault = 2.0f;
f32 TRideCloud::mCushionLen       = 80.0f;
f32 TRideCloud::mCushionSpeed     = 0.015f;
f32 TRideCloud::mScaleSpeed       = 0.005f;

TRideCloud::TRideCloud(const char* name)
    : TRailMapObj(name)
    , unk150(0.0f)
    , unk154(0.0f)
    , unk158(0)
    , unk15C(0.0f)
    , unk16C(0)
{
	unk15C = mMoveSpeedDefault;
	unk160 = 1.0f;
	unk164 = 1.0f;
	unk168 = 1.0f;

	unk16E.r = 0;
	unk16E.g = 0;
	unk16E.b = 0;
	unk16E.a = 0;
}

u32 TRideCloud::touchWater(THitActor*)
{
	unk158 += 10;
	if (unk158 > 600)
		unk158 = 600;
	return 1;
}

void TRideCloud::setGroundCollision()
{
	if (unkEC) {
		// TODO: this is used in MapObjRailBlock too, inline global?
		TMtx34f mtx;
		mtx.set(getModel()->getAnmMtx(0));
		if (TMapCollisionBase* col = unkEC->unk8)
			col->moveMtx(mtx);
	}
}

void TRideCloud::calcRootMatrix()
{
	f32 fVar2 = JMAHermiteInterpolation(unk150, 0.0f, 0.0f, 0.0f, 1.0f,
	                                    -mCushionLen, 0.0f);

	MsMtxSetTRS(getModel()->getBaseTRMtx(), mPosition.x, mPosition.y + fVar2,
	            mPosition.z, mRotation.x, mRotation.y, mRotation.z,
	            mScaling.x * unk160, mScaling.y * unk164, mScaling.z * unk168);
}

void TRideCloud::initMapObj()
{
	TRailMapObj::initMapObj();
	unk154 = unkB8;
}

void TRideCloud::load(JSUMemoryInputStream& stream)
{
	TRailMapObj::load(stream);
	u32 r;
	u32 g;
	u32 b;
	u32 a;
	stream.read(&r, 4);
	stream.read(&g, 4);
	stream.read(&b, 4);
	stream.read(&a, 4);
	unk16E.r = r & 0xff;
	unk16E.g = g & 0xff;
	unk16E.b = b & 0xff;
	unk16E.a = 0xff;
	stream.read(&r, 4);
	stream.read(&g, 4);
	stream.read(&b, 4);
	stream.read(&a, 4);
	unk176.r = r & 0xff;
	unk176.g = g & 0xff;
	unk176.b = b & 0xff;
	unk176.a = 0xff;
	SMS_InitPacket_TwoTevColor(getModel(), 0, GX_TEVREG0, &unk16E, GX_TEVREG1,
	                           &unk176);
	offLiveFlag(0x400);
	onLiveFlag(0x8);
}

u32 TRideCloud::getShadowType() { return 0; }

void TRideCloud::control()
{
	TMapObjBase::control();
	TMapCollisionBase* col = unkEC->unk8;
	if (*gpMarioSpeedY > 0.0f)
		col->setAllBGType(0x400);
	else
		col->setAllBGType(0);

	checkMarioRiding();
	if (!checkRailFlag(0x1)) {
		unk150 = MsClamp<f32>(unk150 - mCushionSpeed, 0.0f, 1.0f);
	} else {
		unk150 = MsClamp<f32>(unk150 + mCushionSpeed, 0.0f, 1.0f);
	}

	f32 fVar8;
	if (unk158 > 0) {
		fVar8 = mScaleSpeed;
		--unk158;
	} else {
		fVar8 = -mScaleSpeed;
	}

	unk160        = MsClamp<f32>(unk160 + fVar8, 1.0f, 3.0f);
	unk168        = MsClamp<f32>(unk168 + fVar8, 1.0f, 3.0f);
	mDamageRadius = mScaling.x * 300.0f * unk160;
	mDamageHeight = 50.0f;
	calcEntryRadius();
	if (!calcRecycle() && !checkRailFlag(0x2)) {
		if (unk16C != 0) {
			--unk16C;
		} else {
			// TODO: common subexpression elimination did a mess here and it is
			// painful to figure out....

			if (!unk138->unk0 || unk138->unk0->isDummy())
				return;

			if (moveToNextNode(unk15C)) {
				TGraphNode& node = unk138->getCurrent();

				if ((node.getRailNode()->mFlags & 0x1000)) {
					unk14A = 180;
					unk148 = 2;
				}

				unk138->moveToShortestNext();

				if (node.getRailNode()->mFlags & 0x800)
					unk16C = node.getRailNode()->mPitch;

				TRailNode* node2 = unk138->getCurrent().getRailNode();
				if (node2->mYaw != 0xffff)
					unk15C = node2->mYaw * 0.01f;

				resetStep(unk15C);
			}
		}
	}
}

void TRideCloud::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 4)
		unkB8 = unk154 * mScaling.x;
	TRailMapObj::perform(param_1, param_2);
}
