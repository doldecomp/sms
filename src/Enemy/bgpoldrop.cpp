#include <Enemy/BossGessoPolDrop.hpp>
#include <Map/MapData.hpp>
#include <Map/Map.hpp>
#include <Map/PollutionManager.hpp>
#include <System/EmitterViewObj.hpp>
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
	if (!unk58)
		return;

	JGeometry::TVec3<f32> local_14 = mPosition;
	local_14 += unk44;

	if (unk58 == 1) {
		unk44.y -= 0.2f;
		const TBGCheckData* checkData;
		f32 dVar3 = gpMap->checkGround(local_14.x, mPosition.y, local_14.z,
		                               &checkData);
		dVar3 += 1.0f;

		if (checkData->checkFlag2(0x10)) {
			unk58 = 0;
			return;
		}

		if (local_14.y < dVar3) {
			unk58      = 2;
			local_14.y = dVar3;
			unk44.zero();
			if (!unk50->checkCurBckFromIndex(12)) {
				unk50->setBckFromIndex(12);
				unk50->setBckFromIndex(13);
			}

			gpMarioParticleManager->emit(0x9E, &local_14, 0, nullptr);
			gpMarioParticleManager->emit(0x9F, &local_14, 0, nullptr);
			if (gpMSound->gateCheck(0x2871))
				MSoundSESystem::MSoundSE::startSoundActor(0x2871, mPosition, 0,
				                                          nullptr, 0, 4);
		} else if (!unk50->checkCurBckFromIndex(11)
		           || unk50->curAnmEndsNext()) {
			unk50->setBckFromIndex(11);
		}

		if (gpMap->isTouchedOneWallAndMoveXZ(&local_14.x, local_14.y,
		                                     &local_14.z, 80.0f))
			unk58 = 0;

	} else if (unk58 == 2 && unk50->curAnmEndsNext()) {
		unk58 = 0;
	}

	mPosition = local_14;

	char trash[0x10]; // TODO: skill issue
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

void TBGPolDrop::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (!unk58)
		return;

	if (param_1 & 1)
		move();

	if (param_1 & 2) {
		MtxPtr m = unk50->getUnk4()->getBaseTRMtx();
		if (unk58 == 1) {
			Mtx local_60;
			f32 s          = JMASin(-90.0f);
			f32 c          = JMACos(-90.0f);
			local_60[0][0] = 1.0;
			local_60[0][1] = 0.0;
			local_60[0][2] = 0.0;
			local_60[0][3] = 0.0;

			local_60[1][0] = 0.0;
			local_60[1][1] = c;
			local_60[1][2] = -s;
			local_60[1][3] = 0.0;

			local_60[2][0] = 0.0;
			local_60[2][1] = s;
			local_60[2][2] = c;
			local_60[2][3] = 0.0;

			mRotation = MsGetRotFromZaxis(unk44);

			s16 r = mRotation.x * (65536.0f / 360.0f);
			s16 p = mRotation.y * (65536.0f / 360.0f);
			s16 h = mRotation.z * (65536.0f / 360.0f);
			MsMtxSetXYZRPH(m, mPosition.x, mPosition.y, mPosition.z, r, p, h);

			MTXConcat(m, local_60, m);
			gpMarioParticleManager->emitAndBindToPosPtr(0x13A, &mPosition, 1,
			                                            this);
		} else {
			MTXIdentity(m);
			m[0][3] = mPosition.x;
			m[1][3] = mPosition.y;
			m[2][3] = mPosition.z;
		}

		unk50->getUnk4()->setBaseScale(mScaling);
		if (unk58 == 2) {
			unk54->getUnk4()->setBaseScale(mScaling);
			unk54->getUnk4()->setBaseTRMtx(m);
		}
	}

	unk50->perform(param_1, param_2);
	if (unk58 == 2) {
		if (param_1 & 0x2)
			unk54->calcAnm();
		if (param_1 & 0x200)
			gpPollution->stampModel(unk54->getUnk4());
	}
}
