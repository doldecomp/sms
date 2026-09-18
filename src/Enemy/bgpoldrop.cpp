#include <Enemy/BossGessoPolDrop.hpp>
#include <Map/MapData.hpp>
#include <Map/Map.hpp>
#include <Map/PollutionManager.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <JSystem/JMath.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TBGPolDrop::TBGPolDrop(const char* name)
    : JDrama::TActor(name)
    , unk50(nullptr)
    , unk54(nullptr)
    , unk58(0)
{
	unk44.zero();
}

void TBGPolDrop::move()
{
	if (!getUnk58())
		return;

	JGeometry::TVec3<f32> local_14 = mPosition;
	local_14 += getVelocity();

	if (getUnk58() == 1) {
		unk44.y -= 0.2f;
		const TBGCheckData* checkData;
		f32 dVar3 = gpMap->checkGround(local_14.x, mPosition.y, local_14.z,
		                               &checkData);
		dVar3 += 1.0f;

		if (checkData->checkFlag(BG_CHECK_FLAG_ILLEGAL)) {
			unk58 = 0;
			return;
		}

		if (local_14.y < dVar3) {
			unk58      = 2;
			local_14.y = dVar3;
			unk44.zero();
			if (!unk50->checkCurBckFromIndex(12)) {
				unk50->setBckFromIndex(12);
				unk54->setBckFromIndex(13);
			}

			gpMarioParticleManager->emit(BGESO_JPA_MS_BOGE_ODANHIT_A, &local_14,
			                             0, nullptr);
			gpMarioParticleManager->emit(BGESO_JPA_MS_BOGE_ODANHIT_B, &local_14,
			                             0, nullptr);
			SMSGetMSound()->startSoundActor(MSD_SE_BS_GESO_GERO_LAND,
			                                &mPosition, 0, nullptr, 0, 4);
		} else if (!unk50->checkCurBckFromIndex(11)
		           || unk50->curAnmEndsNext()) {
			unk50->setBckFromIndex(11);
		}

		if (gpMap->isTouchedOneWallAndMoveXZ(&local_14.x, local_14.y,
		                                     &local_14.z, 80.0f))
			unk58 = 0;

	} else if (getUnk58() == 2 && unk50->curAnmEndsNext()) {
		unk58 = 0;
	}

	mPosition = local_14;
}

void TBGPolDrop::launch(const JGeometry::TVec3<f32>& param_1,
                        const JGeometry::TVec3<f32>& param_2)
{
	unk44     = param_2;
	mPosition = param_1;
	mScaling.set(1.0f, 1.0f, 1.0f);
	mRotation.zero();
	unk58 = 1;
}

// The rotation matrix below `perform` builds is `MsMtxSetRotX__FPA4_ff`, the
// map's weak 0x7c header inline (one out-of-line copy in MoveBG.a
// MapObjPinna.cpp); compiled out of line here the body is 124 bytes = 0x7c on
// the nose, which is what identified it. It now comes from
// <MarioUtil/MathUtil.hpp>; the TU-local copy this unit used to park was
// byte-identical, and so is the header spelling, which reaches the table
// lookups through JMASSin/JMASCos rather than JMASin/JMACos.
//
// Naming it is what closed `perform`. That function used to be 100.0% but not
// exact, with the MsGetRotFromZaxis return temporary at 0x5c against retail's
// 0x64 and every other slot (the rotation matrix at 0x70, the three
// float-to-int pairs at 0xa0/0xa8/0xb0, the r27-r31 save block at 0xbc) already
// right. The 8-byte hole at 0x68-0x6f above the temporary was the pair of
// reserved slots for the named `f32 s` and `f32 c`: the same two values held as
// locals of an *inlined callee* are trivial PODs and cost zero frame, so the
// temporary moves up by 8 and the frame stays 0xd0.
// Rejected before that (temporary's offset in brackets): getScaling() on either
// setBaseScale [0x64] but +8 of frame; getMActor2() for unk54 [0x60] +8;
// getUnk58() at all three sites [0x68] +16; getRotation()/getPosition() in the
// MsMtxSetXYZRPH arguments [0x68] +16; a named `J3DModel* model` before
// getBaseTRMtx() [0x54] -8; getScaling() plus that named model [0x58] with the
// right frame. Worth nothing: SMSGetPollution(), MsSin/MsCos over
// JMASin/JMACos, getUnk58() on the entry guard alone. Actively worse:
// getVelocity() in MsGetRotFromZaxis (-0.3), a named TVec3 for its result
// (-4.6), and simply repeating JMASin/JMACos in place of the named `s`/`c`
// (the CSE goes away: -6 and -13).

void TBGPolDrop::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!unk58)
		return;

	if (cue & CUE_MOVE)
		move();

	if (cue & CUE_CALC_ANIM) {
		MtxPtr m = unk50->getModel()->getBaseTRMtx();
		if (unk58 == 1) {
			Mtx local_60;
			MsMtxSetRotX(local_60, -90.0f);

			mRotation = MsGetRotFromZaxis(unk44);

			MsMtxSetXYZRPH(m, mPosition.x, mPosition.y, mPosition.z,
			               mRotation.x, mRotation.y, mRotation.z);

			MTXConcat(m, local_60, m);
			gpMarioParticleManager->emitAndBindToPosPtr(0x13A, &mPosition, 1,
			                                            this);
		} else {
			MTXIdentity(m);
			m[0][3] = mPosition.x;
			m[1][3] = mPosition.y;
			m[2][3] = mPosition.z;
		}

		unk50->getModel()->setBaseScale(mScaling);
		if (unk58 == 2) {
			unk54->getModel()->setBaseScale(mScaling);
			unk54->getModel()->setBaseTRMtx(m);
		}
	}

	unk50->perform(cue, graphics);
	if (unk58 == 2) {
		if (cue & CUE_CALC_ANIM)
			unk54->calcAnm();
		if (cue & CUE_ENTRY)
			gpPollution->stampModel(unk54->getModel());
	}
}
