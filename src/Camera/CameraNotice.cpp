#include <Camera/Camera.hpp>
#include <Camera/CameraKindParam.hpp>
#include <Camera/CameraMarioData.hpp>
#include <Camera/camerasave.hpp>
#include <Camera/cameralib.hpp>
#include <Enemy/Conductor.hpp>
#include <Strategic/LiveManager.hpp>
#include <Player/Mario.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/WaterGun.hpp>
#include <System/MarioGamePad.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JMath.hpp>
#include <Enemy/BossGesso.hpp>
#include <Enemy/Enemy.hpp>

static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

static const char* sNoticeActorManagerName[] = {
	"ヒノクリマネージャー",
	"ヒノクリ２マネージャー",
	"ボスパックンマネージャー",
	nullptr,
};

const char* bossGesoViewObjName = "ボスゲッソー";

void CPolarSubCamera::setNoticeInfo()
{
	unk2A0 = new TLiveActor*[0x10];
	unk2A4 = nullptr;
	unk29C = 0;

	for (int i = 0; sNoticeActorManagerName[i] != nullptr; ++i) {
		TLiveManager* mgr
		    = gpConductor->getManagerByName(sNoticeActorManagerName[i]);

		if (mgr != nullptr)
			for (int j = 0, e = mgr->getObjNum(); j < e; ++j)
				unk2A0[unk29C++] = (TLiveActor*)mgr->unk18[j];
	}

	unk2A8 = JDrama::TNameRefGen::search<TBossGesso>(bossGesoViewObjName);
}

TLiveActor* CPolarSubCamera::getNoticeActor_()
{
	if (unk2A4 != nullptr && !unk2A4->checkLiveFlag(LIVE_FLAG_DEAD)
	    && !unk2A4->checkLiveFlag(LIVE_FLAG_HIDDEN)) {

		if (unk2A4->mPosition.squared(*gpMarioPos)
		    < CLBSquared<f32>(unk2D0->mOffDist.get())) {
			JGeometry::TVec2<f32> clipPos;
			CLBCalc2DFPos(&clipPos, unk16C, unk1EC, unk2A4->mPosition, nullptr,
			              false);

			// TODO: inline
			f32 clipMax  = unk2D0->mOffClipRatio.get();
			f32 clipMin  = unk2D0->mOffClipRatio.get();
			bool inClipX = false;
			bool inClipY = false;
			if (clipMin <= clipPos.x && clipPos.x <= clipMax)
				inClipX = true;

			if (inClipX && clipMin <= clipPos.y && clipPos.y <= clipMax)
				inClipY = true;

			if (inClipY)
				return unk2A4;
		}
	}

	TLiveActor* noticeActor = nullptr;
	f32 closestDist2        = CLBSquared<f32>(unk2D0->mOnDist.get());

	for (int i = 0; i < unk29C; i++) {
		if (unk2A0[i]->checkLiveFlag(LIVE_FLAG_DEAD)
		    || unk2A0[i]->checkLiveFlag(LIVE_FLAG_HIDDEN)) {
			continue;
		}

		if (unk2A4 != nullptr && unk2A0[i] == unk2A4) {
			continue;
		}

		f32 dist2 = unk2A0[i]->mPosition.squared(*gpMarioPos);
		if (dist2 >= closestDist2)
			continue;

		JGeometry::TVec2<f32> clipPos;
		CLBCalc2DFPos(&clipPos, unk16C, unk1EC, unk2A0[i]->mPosition, nullptr,
		              false);

		// TODO: inline
		f32 clipMax  = unk2D0->mOnClipRatio.get();
		f32 clipMin  = unk2D0->mOnClipRatio.get();
		bool inClipX = false;
		bool inClipY = false;
		if (clipMin <= clipPos.x && clipPos.x <= clipMax) {
			inClipX = true;
		}
		if (inClipX && clipMin <= clipPos.y && clipPos.y <= clipMax) {
			inClipY = true;
		}
		if (!inClipY) {
			continue;
		}

		if (!MsIsInSight(*gpMarioPos, DEG2SHORTANGLE(*gpMarioAngleY),
		                 unk2A0[i]->mPosition, dist2, unk2D0->mOnDegree.get(),
		                 -1.0f)) {
			continue;
		}

		closestDist2 = dist2;
		noticeActor  = unk2A0[i];
	}

	return noticeActor;
}

void CPolarSubCamera::execNoticeOnOffProc_(EnumNoticeOnOffMode mode)
{
	switch (mode) {
	case 0:
		unk2A4 = nullptr;
		unk64 &= ~0x20U;
		break;
	case 1: {
		TLiveActor* actor = getNoticeActor_();
		if (actor != unk2A4 && actor == nullptr) {
			unk2A4 = nullptr;
			unk64 &= ~0x20U;
		}
		break;
	}
	case 2: {
		TLiveActor* actor = getNoticeActor_();
		if (actor != unk2A4 && actor != nullptr) {
			unk2A4 = actor;
			unk64 |= 0x20U;
		}
		break;
	}
	}
}

void CPolarSubCamera::calcNoticeTargetYrot_(const Vec& target)
{
	Vec mPos     = gpCameraMario->unk0;
	f32 dz2      = CLBSquared<f32>(mPos.z - target.z);
	f32 dx2      = CLBSquared<f32>(mPos.x - target.x);
	f32 dist2    = dx2 + dz2;
	f32 farClip2 = CLBSquared<f32>(unk2D0->mRotateMinDistXZ.get());
	f32 near2    = CLBSquared<f32>(unk2D0->mRotateFastMinDistXZ.get());

	if (dist2 > near2) {
		JGeometry::TVec3<f32> diff(mPos.x - target.x, mPos.y - target.y,
		                           mPos.z - target.z);
		MsVECNormalize(&diff, &diff);
		// TODO: many inlines from cameralib maybe?
		f32 dx  = diff.x * 500.0f + mPos.x;
		f32 dz  = diff.z * 500.0f + mPos.z;
		s16 ang = matan(dz - unk80.unkC.z, dx - unk80.unkC.x);
		int absAngle
		    = ang - unk80.unk26 >= 0 ? ang - unk80.unk26 : -(ang - unk80.unk26);
		f32 ratio = DEG2SHORTANGLE(1.0f) * (f32)absAngle;

		f32 chase;
		if (dist2 > farClip2) {
			chase = 1.0f;
		} else {
			chase = CLBCalcRatio<f32>(near2, farClip2, dist2);
		}
		f32 base = CLBLinearInbetween<f32>(
		    1.0f, unk2D0->mRotateMagnifXmax.get(), unk80.unk28);
		f32 speed
		    = unk288
		      * (chase * (ratio * ((f32)unk2D0->mRotateYSpeed.get() * base)));
		if (speed > 32766.998f)
			speed = 32766.998f;
		s16 delta = CLBRoundf<s16>(speed);
		CLBChaseGeneralConstantSpecifySpeed<s16>(&unk80.unk26, ang, delta);
	}
}

void CPolarSubCamera::getNozzleTopPos_(JGeometry::TVec3<f32>* out) const
{
	if (SMS_GetMarioWaterGun() == nullptr) {
		out->set(gpCameraMario->unk0);
	} else {
		MtxPtr mtx = SMS_GetMarioWaterGun()->getNozzleMtx();
		out->x     = mtx[0][3];
		out->y     = mtx[1][3];
		out->z     = mtx[2][3];

		JGeometry::TVec3<f32> dir(mtx[0][1], mtx[1][1], mtx[2][1]);
		dir.normalize();
		dir *= 30.0f;
		*out += dir;
	}
}

void CPolarSubCamera::ctrlLButtonCamera_()
{
	f32 stickX = -unk120->mCompSPos[4];
	f32 stickY = -unk120->mCompSPos[5];

	if (unk7C == 0) {
		if (!SMS_CheckMarioFlag(MARIO_FLAG_HAS_FLUDD)) {
			unk80.unkC.set(gpCameraMario->unk0);
		} else {
			getNozzleTopPos_(&unk80.unkC);
		}
	}

	if (unk78 == 0) {
		if (unk64 & 0x20) {
			if (stickX != 0.0f) {
				rotateY_ByStickX_(stickX);
			} else {
				calcNoticeTargetYrot_(unk2A4->mPosition);
			}
		} else {
			rotateY_ByStickX_(stickX);
		}
		rotateX_ByStickY_(stickY);
	}

	calcPosAndAt_();
}
