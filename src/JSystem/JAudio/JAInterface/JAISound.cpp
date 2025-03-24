#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>

JAIBasic* JAISound::interPointer;

JAISound::JAISound()
{
	unk38 = 0;
	unk1  = 0;
	unk2  = 10;
	unk20 = 0;
}

int JAISound::initMoveParameter(JAIMoveParaSet* param_1, f32 param_2,
                                u32 param_3)
{
	if (param_1->unkC == 0 && param_1->unk4 == param_2)
		return 2;

	if (param_1->unkC != 0 && param_1->unk0 == param_2)
		return 2;

	param_1->unk0 = param_2;
	if (param_3 == 0) {
		param_1->unk4 = param_2;
		return 0;
	}

	if (param_3 == 1)
		param_1->unk8 = param_1->unk4 - param_1->unk0;
	else
		param_1->unk8 = (param_1->unk4 - param_1->unk0) / (f32)param_3;
	param_1->unkC = param_3;

	return 1;
}

void JAISound::initMultiMoveParameter(JAIMoveParaSet* param_1, u8 param_2,
                                      u32 param_3, f32 param_4, f32 param_5,
                                      u32 param_6)
{
}

u32 JAISound::getSeCategoryNumber()
{
	return interPointer->changeIDToCategory(unk8);
}

void JAISound::getDataInfoHeader() { }

u32 JAISound::getSwBit() { return interPointer->getSoundSwBit(unk3C); }

u32 JAISound::checkSwBit(u32 bit)
{
	return bit & interPointer->getSoundSwBit(unk3C);
}

u8 JAISound::getInfoPriority() { return interPointer->getSoundPrioity(unk3C); }

void JAISound::clearMainSoundPPointer()
{
	if (unk34 == nullptr)
		return;
	*unk34 = nullptr;
}

void JAISound::release()
{
	*unk34 = nullptr;
	unk34  = nullptr;
}

void JAISound::start(u32) { }

void JAISound::stop(u32 param_1)
{
	interPointer->stopSoundHandle(this, param_1);
}

void JAISound::setVolume(f32 param_1, u32 param_2, u8 param_3)
{
	switch (unk8 & 0xC0000000) {
	case 0x80000000:
		setSeqInterVolume(param_3, param_1, param_2);
		break;
	case 0x00000000:
		setSeInterVolume(param_3, param_1, param_2, 0);
		break;
	case 0xC0000000:
		setStreamInterVolume(param_3, param_1, param_2);
		break;
	}
}

void JAISound::setDirectVolume(f32, u32) { }

void JAISound::setPan(f32 param_1, u32 param_2, u8 param_3)
{
	switch (unk8 & 0xC0000000) {
	case 0x80000000:
		setSeqInterPan(param_3, param_1, param_2);
		break;
	case 0x00000000:
		setSeInterPan(param_3, param_1, param_2, 0);
		break;
	case 0xC0000000:
		setStreamInterPan(param_3, param_1, param_2);
		break;
	}
}

void JAISound::setDirectPan(f32, u32) { }

void JAISound::setPitch(f32 param_1, u32 param_2, u8 param_3)
{
	switch (unk8 & 0xC0000000) {
	case 0x80000000:
		setSeqInterPitch(param_3, param_1, param_2);
		break;
	case 0x00000000:
		setSeInterPitch(param_3, param_1, param_2, 0.0f);
		break;
	case 0xC0000000:
		setStreamInterPitch(param_3, param_1, param_2);
		break;
	}
}

void JAISound::setDirectPitch(f32, u32) { }

void JAISound::setFxmix(f32 param_1, u32 param_2, u8 param_3)
{
	switch (unk8 & 0xC0000000) {
	case 0x80000000:
		setSeqInterFxmix(param_3, param_1, param_2);
		break;
	case 0x00000000:
		setSeInterFxmix(param_3, param_1, param_2, 0.0f);
		break;
	case 0xC0000000:
		break;
	}
}

void JAISound::setDirectFxmix(f32, u32) { }

void JAISound::setDolby(f32 param_1, u32 param_2, u8 param_3)
{
	switch (unk8 & 0xC0000000) {
	case 0x80000000:
		setSeqInterDolby(param_3, param_1, param_2);
		break;
	case 0x00000000:
		setSeInterDolby(param_3, param_1, param_2, 0.0f);
		break;
	case 0xC0000000:
		break;
	}
}

void JAISound::setDirectDolby(f32, u32) { }

void JAISound::setTempoProportion(f32 param_1, u32 param_2)
{
	switch (unk8 & 0xC0000000) {
	case 0x80000000:
		setSeqTempoProportion(param_1, param_2);
		break;
	case 0x00000000:
		break;
	case 0xC0000000:
		break;
	}
}

void JAISound::setPortData(u8 param_1, u16 param_2)
{
	switch (unk8 & 0xC0000000) {
	case 0x80000000:
		setSeqPortData(param_1, param_2, 0);
		break;
	case 0x00000000:
		setSePortData(param_1, param_2);
		break;
	case 0xC0000000:
		break;
	}
}

void JAISound::setPrepareFlag(u8) { }

void JAISound::checkReady() { }

void JAISound::setDistanceVolumeCommon(f32 param_1, u8 param_2) { }

void JAISound::setDistancePanCommon() { }

void JAISound::setPositionDopplarCommon(u32) { }

void JAISound::setSeqInterVolume(u8, f32, u32) { }

void JAISound::setSeqInterPan(u8, f32, u32) { }

void JAISound::setSeqInterPitch(u8, f32, u32) { }

void JAISound::setSeqInterFxmix(u8, f32, u32) { }

void JAISound::setSeqInterDolby(u8, f32, u32) { }

void JAISound::setSeqTempoProportion(f32, u32) { }

void JAISound::setSeqPortData(u8, u16, u32) { }

void JAISound::setWaveReadMode(s32, s32) { }

void JAISound::setTrackVolume(u8, f32, u32) { }

void JAISound::setTrackVolumeMulti(u8, u32, f32, u32) { }

void JAISound::setTrackMuteSwitch(u8, u8) { }

void JAISound::setTrackMuteSwitchMulti(u32, u8) { }

void JAISound::setTrackInterruptSwitch(u8, u8) { }

void JAISound::setTrackPan(u8, f32, u32) { }

void JAISound::setTrackPanMulti(u8, u32, f32, u32) { }

void JAISound::setTrackPitch(u8, f32, u32) { }

void JAISound::setTrackPitchMulti(u8, u32, f32, u32) { }

void JAISound::setTrackFxmix(u8, f32, u32) { }

void JAISound::setTrackFxmixMulti(u8, u32, f32, u32) { }

void JAISound::setTrackDolby(u8, f32, u32) { }

void JAISound::setTrackDolbyMulti(u8, u32, f32, u32) { }

void JAISound::setTrackFirU7(u8, u8, u32) { }

void JAISound::setTrackFirMultiU7(u8, u32, u8, u32) { }

void JAISound::setTrackPortData(u8, u8, u16) { }

void JAISound::setSeInterMovePara(JAIMoveParaSet*, u32) { }

void JAISound::setSeInterRandomPara(f32*, u32, f32, f32) { }

void JAISound::setSeInterVolume(u8, f32, u32, u8) { }

void JAISound::setSeInterPan(u8, f32, u32, u8) { }

void JAISound::setSeInterFxmix(u8, f32, u32, u8) { }

void JAISound::setSeInterFir(u8, u8, u32, u8) { }

void JAISound::setSeInterDolby(u8, f32, u32, u8) { }

void JAISound::setSeInterPitch(u8, f32, u32, f32) { }

void JAISound::setSePortData(u8, u16) { }

void JAISound::getSePortData(u8) { }

void JAISound::setSeDistanceParameters() { }

void JAISound::setSeDistanceVolume(u8) { }

void JAISound::setSeDistancePan(u8) { }

void JAISound::setSeDistancePitch(u8) { }

void JAISound::setSePositionDopplar() { }

void JAISound::setSeDistanceFxmix(u8) { }

void JAISound::setSeDistanceFir(u8) { }

void JAISound::setSeDistanceDolby(u8) { }

void JAISound::setStreamMode(u32) { }

void JAISound::setStreamInterVolume(u8, f32, u32) { }

void JAISound::setStreamInterPitch(u8, f32, u32) { }

void JAISound::setStreamInterPan(u8, f32, u32) { }

void JAISound::setStreamPrepareFlag(u8) { }

void JAISound::checkStreamReady() { }

void JAISound::setPauseMode(u8, u8) { }

void JAISound::setSeqPrepareFlag(u8) { }

void JAISound::checkSeqReady() { }

void JAISound::getVolume(u8) { }

void JAISound::getPan(u8) { }

void JAISound::getPitch(u8) { }

void JAISound::getFxmix(u8) { }

void JAISound::getDolby(u8) { }

void JAISound::getTempoProportion() { }

void JAISound::getPortData(u8) { }

f32 JAISound::getSeqInterVolume(u8) { }

void JAISound::getSeqInterPan(u8) { }

void JAISound::getSeqInterPitch(u8) { }

void JAISound::getSeqInterFxmix(u8) { }

void JAISound::getSeqInterDolby(u8) { }

void JAISound::getSeqTempoProportion() { }

void JAISound::getSeqPortData(u8) { }

void JAISound::getTrackParameterU7(u8, s8) { }

void JAISound::getTrackParameterF32(u8, s8) { }

void JAISound::getPointer(u8, s8) { }

void JAISound::getTrackPortData(u8, u8) { }

void JAISound::getSeParametermeterF32(u8, u8) { }

void JAISound::getStreamInterVolume(u8) { }

void JAISound::getStreamInterPitch(u8) { }

void JAISound::getActorGroundNumber() { }

JAISeqParameter* JAISound::getSeqParameter() { return (JAISeqParameter*)unk38; }

JAISeParameter* JAISound::getSeParameter() { return (JAISeParameter*)unk38; }

JAIStreamParameter* JAISound::getStreamParameter()
{
	return (JAIStreamParameter*)unk38;
}

u32 JAISound::getTrackPortRoute(u8 param_1, u8 param_2)
{
	if (unk8 & 0x800)
		return (param_1 >> 4) + 0x20000000 + ((param_1 & 0xF) << 4)
		       + (param_2 << 16);
	return (param_1 & 0xf) + 0x10000000 + (param_2 << 16);
}

void JAISound::getSeInfoPointer() { }
