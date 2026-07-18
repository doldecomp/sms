#include <NPC/NpcTrample.hpp>
#include <NPC/NpcBase.hpp>
#include <NPC/NpcSave.hpp>

f32 TNpcTrample::msAmpDecrease = 0.0f;

void TNpcTrample::startTrample()
{
	unk0 = TBaseNPC::mPtrSaveNormal->mSLTrampleAmplitude.get();
	unk4 = TBaseNPC::mPtrSaveNormal->mSLTrampleShakeFrames.get();
	unk6 = 0;

	msAmpDecrease = unk0 * (1.0f / unk4);
}

bool TNpcTrample::updateTrample(f32 param_1, f32* param_2)
{

	bool result = false;
	if (unk6 == 0 && unk4 == 0) {
		*param_2 = param_1;
	} else if (unk6 > 0) {
		*param_2 = param_1;
		unk4     = 0;
		unk6 -= 1;
		if (unk6 == 0)
			result = true;
	} else if (unk4 > 0) {
		unk4 -= 1;

		if (unk4 == 0) {
			*param_2 = param_1;
			unk6     = (TBaseNPC::mPtrSaveNormal->mSLTrampleToMadFrames).value;
		} else {
			unk0      = unk0 - msAmpDecrease;
			s16 phase = TBaseNPC::mPtrSaveNormal->mSLTrampleVelocity.get()
			            * (TBaseNPC::mPtrSaveNormal->mSLTrampleShakeFrames.get()
			               - unk4);

			f32 amplitude = unk0 * -JMASSin(phase);

			*param_2 = param_1 * (amplitude + 1.0f);
		}
	}

	return result;
}
