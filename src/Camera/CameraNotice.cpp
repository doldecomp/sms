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

// TU-local and staying that way (decided in header round 16). Both
// notice-distance sites keep their three products apart (`fmuls` x3 +
// `fadds` x2) where JGeometry::TVec3<f32>::squared(const TVec3&) contracts
// them into `fmadds`, and the helper has to be its own inline level. Trials on
// getNoticeActor_ (79.7% with the form below):
//   * three named squares spelled out at both sites, no helper: 77.2%
//   * helper body using CLBSquared<f32> for each square: 71.3%
//   * three named squares in TVec3::squared(const TVec3&) itself: costs
//     TShine::calc (99.9 -> 95.5), the only user of that overload
// The map offers no squared-distance symbol anywhere and only
// `CLBSquared<f>__Ff` (weak, 8 bytes, from this TU), so there is no evidence
// for a shared header helper; CameraMultiPlayer's inner loop parks its own.
static inline f32 CameraNoticeSquaredDist(const JGeometry::TVec3<f32>& a,
                                          const JGeometry::TVec3<f32>& b)
{
	f32 dx = a.x - b.x;
	f32 dy = a.y - b.y;
	f32 dz = a.z - b.z;

	f32 sqX = dx * dx;
	f32 sqY = dy * dy;
	f32 sqZ = dz * dz;

	return sqX + sqY + sqZ;
}

void CPolarSubCamera::setNoticeInfo()
{
	unk2A0       = new TLiveActor*[0x10];
	mNoticeActor = nullptr;
	unk29C       = 0;

	for (int i = 0; sNoticeActorManagerName[i] != nullptr; ++i) {
		TLiveManager* mgr
		    = gpConductor->getManagerByName(sNoticeActorManagerName[i]);

		if (mgr != nullptr) {
			s32 e = mgr->getObjNum();
			for (int j = 0; j < e; ++unk29C, ++j)
				unk2A0[unk29C] = mgr->getObj(j);
		}
	}

	unk2A8 = JDrama::TNameRefGen::search<TLiveActor>(bossGesoViewObjName);
}

// TODO (closure batch 87): 79.7%, up from 73.2%. Restored here: the clip range
// is [-ratio, +ratio] (retail's `fneg` off the single param read, hoisted above
// CLBCalc2DFPos), the clip test is a **three**-bool ladder whose result is then
// normalised again (`inClipY ? true : false`), and the squared distance goes
// through a helper with three named squares instead of TVec3::squared(), which
// contracts to `fmadds`. What is left:
//   - frame 0xf0 vs 0xc8, and retail saves f27 and r25 where we start at f28
//     and r26: retail hoists **both** `this + 0x16c` and `this + 0x1ec` into
//     callee-saved registers before the loop (we only hoist 0x1ec), so two
//     named matrix locals are missing.
//   - at the first site MWCC schedules our `bl CLBSquared` before the three
//     `fmuls`; retail evaluates the whole distance first. Naming the
//     CLBSquared result as well (`f32 offDist2 = ...`) costs an instruction and
//     does not move it.
TLiveActor* CPolarSubCamera::getNoticeActor_()
{
	if (mNoticeActor != nullptr && !mNoticeActor->checkLiveFlag(LIVE_FLAG_DEAD)
	    && !mNoticeActor->checkLiveFlag(LIVE_FLAG_HIDDEN)) {

		f32 noticeDist2
		    = CameraNoticeSquaredDist(mNoticeActor->mPosition, *gpMarioPos);
		if (noticeDist2 < CLBSquared<f32>(mSaveNotice->mOffDist.get())) {
			f32 clipMax = mSaveNotice->mOffClipRatio.get();

			JGeometry::TVec2<f32> clipPos;
			CLBCalc2DFPos(&clipPos, unk16C, unk1EC, mNoticeActor->mPosition,
			              nullptr, false);

			// TODO: inline
			f32 clipMin     = -clipMax;
			bool inClipX    = false;
			bool overClipMinY = false;
			bool inClipY    = false;
			if (clipMin <= clipPos.x && clipPos.x <= clipMax)
				inClipX = true;

			if (inClipX && clipMin <= clipPos.y)
				overClipMinY = true;

			if (overClipMinY && clipPos.y <= clipMax)
				inClipY = true;

			if (inClipY ? true : false)
				return mNoticeActor;
		}
	}

	TLiveActor* noticeActor = nullptr;
	f32 closestDist2        = CLBSquared<f32>(mSaveNotice->mOnDist.get());

	for (int i = 0; i < unk29C; i++) {
		if (unk2A0[i]->checkLiveFlag(LIVE_FLAG_DEAD)
		    || unk2A0[i]->checkLiveFlag(LIVE_FLAG_HIDDEN))
			continue;

		if (mNoticeActor != nullptr && unk2A0[i] == mNoticeActor)
			continue;

		f32 dist2
		    = CameraNoticeSquaredDist(unk2A0[i]->mPosition, *gpMarioPos);
		if (dist2 >= closestDist2)
			continue;

		JGeometry::TVec2<f32> clipPos;
		CLBCalc2DFPos(&clipPos, unk16C, unk1EC, unk2A0[i]->getPosition(),
		              nullptr, false);

		// TODO: inline
		f32 clipMax  = mSaveNotice->mOnClipRatio.get();
		f32 clipMin  = -clipMax;
		bool inClipX = false;
		bool inClipY = false;
		if (clipMin <= clipPos.x && clipPos.x <= clipMax) {
			inClipX = true;
		}
		if (inClipX && clipMin <= clipPos.y && clipPos.y <= clipMax) {
			inClipY = true;
		}
		if (!inClipY)
			continue;

		if (!MsIsInSight(*gpMarioPos, SHORTANGLE2DEG(*gpMarioAngleY),
		                 unk2A0[i]->mPosition, dist2,
		                 mSaveNotice->mOnDegree.get(), -1.0f))
			continue;

		closestDist2 = dist2;
		noticeActor  = unk2A0[i];
	}

	return noticeActor;
}

void CPolarSubCamera::execNoticeOnOffProc_(EnumNoticeOnOffMode mode)
{
	switch (mode) {
	case NOTICE_MODE_UNK0:
		mNoticeActor = nullptr;
		unk64 &= ~CAMERA_FLAG_NOTICE_ACTIVE;
		break;
	case NOTICE_MODE_UNK1: {
		TLiveActor* actor = getNoticeActor_();
		if (actor != mNoticeActor && actor == nullptr) {
			mNoticeActor = nullptr;
			unk64 &= ~CAMERA_FLAG_NOTICE_ACTIVE;
		}
		break;
	}
	case NOTICE_MODE_UNK2: {
		TLiveActor* actor = getNoticeActor_();
		if (actor != mNoticeActor && actor != nullptr) {
			mNoticeActor = actor;
			unk64 |= CAMERA_FLAG_NOTICE_ACTIVE;
		}
		break;
	}
	}
}

// TODO (closure batch 87): 92.0%, data now exact. The angle factor is
// `|angle| * (2.0f / 65536.0f)` (a 0..1 fraction of a half turn), not
// DEG2SHORTANGLE(1.0f) -- the old 182.04445f literal was displacing the whole
// `.sdata2` pool. What is left: frame 0xa8 vs 0x88 (low region), the
// `fmadds` pair for `dir * 500 + marioPos` has its two terms swapped against
// retail, and the `ang - mCurrentTarget.mYaw` block sign-extends once where
// retail extends at every s16-typed use (the same `extsh` family as
// CameraNormal's ctrlNormalOrTowerCamera_).
void CPolarSubCamera::calcNoticeTargetYrot_(const Vec& target)
{
	Vec mPos     = gpCameraMario->unk0;
	f32 dz2      = CLBSquared<f32>(mPos.z - target.z);
	f32 dx2      = CLBSquared<f32>(mPos.x - target.x);
	f32 dist2    = dx2 + dz2;
	f32 farClip2 = CLBSquared<f32>(mSaveNotice->mRotateMinDistXZ.get());
	f32 near2    = CLBSquared<f32>(mSaveNotice->mRotateFastMinDistXZ.get());

	if (dist2 > near2) {
		JGeometry::TVec3<f32> diff(mPos.x - target.x, mPos.y - target.y,
		                           mPos.z - target.z);
		MsVECNormalize(&diff, &diff);
		// TODO: many inlines from cameralib maybe?
		f32 dx       = diff.x * 500.0f + mPos.x;
		f32 dz       = diff.z * 500.0f + mPos.z;
		s16 ang      = matan(dz - mCurrentTarget.mTarget.z,
		                     dx - mCurrentTarget.mTarget.x);
		int absAngle = ang - mCurrentTarget.mYaw >= 0
		                   ? ang - mCurrentTarget.mYaw
		                   : -(ang - mCurrentTarget.mYaw);
		f32 ratio    = (f32)absAngle * (2.0f / 65536.0f);

		f32 chase;
		if (dist2 > farClip2) {
			chase = 1.0f;
		} else {
			chase = CLBCalcRatio<f32>(near2, farClip2, dist2);
		}
		f32 base = CLBLinearInbetween<f32>(
		    1.0f, mSaveNotice->mRotateMagnifXmax.get(), mCurrentTarget.unk28);
		f32 speed
		    = unk288
		      * (chase
		         * (ratio * ((f32)mSaveNotice->mRotateYSpeed.get() * base)));
		if (speed > 32766.998f)
			speed = 32766.998f;
		s16 delta = CLBRoundf<s16>(speed);
		CLBChaseGeneralConstantSpecifySpeed<s16>(&mCurrentTarget.mYaw, ang,
		                                         delta);
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

// TODO (closure batch 87): pure frame gap, 0x70 vs 0x40. The one named local
// (a scratch Vec) sits at 0x34 in retail and 0x10 here, so 36 of the 48 bytes
// are low region (inline-expansion temporaries) and 12 sit above the vector.
// getNozzleTopPos_ is the same shape at 0x58 vs 0x40.
void CPolarSubCamera::ctrlLButtonCamera_()
{
	f32 stickX = -unk120->mCompSPos[4];
	f32 stickY = -unk120->mCompSPos[5];

	if (mTargetFreezeFrames == 0) {
		if (!SMS_CheckMarioFlag(MARIO_FLAG_HAS_FLUDD))
			mCurrentTarget.mTarget.set(gpCameraMario->unk0);
		else
			getNozzleTopPos_(&mCurrentTarget.mTarget);
	}

	if (mPosFreezeFrames == 0) {
		if (unk64 & CAMERA_FLAG_NOTICE_ACTIVE) {
			if (stickX != 0.0f) {
				rotateY_ByStickX_(stickX);
			} else {
				calcNoticeTargetYrot_(mNoticeActor->mPosition);
			}
		} else {
			rotateY_ByStickX_(stickX);
		}
		rotateX_ByStickY_(stickY);
	}

	calcPosAndAt_();
}
