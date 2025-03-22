#include <JSystem/JAudio/JAInterface/JAISound.hpp>

JAISound::JAISound() { }

JAISound::~JAISound() { }

void JAISound::initMoveParameter(JAIMoveParaSet*, f32, u32) { }

void JAISound::initMultiMoveParameter(JAIMoveParaSet*, u8, u32, f32, f32, u32)
{
}

void JAISound::getSeCategoryNumber() { }

void JAISound::getDataInfoHeader() { }

u32 JAISound::getSwBit() { }

void JAISound::checkSwBit(u32) { }

u8 JAISound::getInfoPriority() { }

void JAISound::clearMainSoundPPointer() { }

void JAISound::release() { }

void JAISound::start(u32) { }

void JAISound::stop(u32) { }

void JAISound::setVolume(f32, u32, u8) { }

void JAISound::setDirectVolume(f32, u32) { }

void JAISound::setPan(f32, u32, u8) { }

void JAISound::setDirectPan(f32, u32) { }

void JAISound::setPitch(f32, u32, u8) { }

void JAISound::setDirectPitch(f32, u32) { }

void JAISound::setFxmix(f32, u32, u8) { }

void JAISound::setDirectFxmix(f32, u32) { }

void JAISound::setDolby(f32, u32, u8) { }

void JAISound::setDirectDolby(f32, u32) { }

void JAISound::setTempoProportion(f32, u32) { }

void JAISound::setPortData(u8, u16) { }

void JAISound::setPrepareFlag(u8) { }

void JAISound::checkReady() { }

void JAISound::setDistanceVolumeCommon(f32, u8) { }

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

void JAISound::getSeqInterVolume(u8) { }

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

void JAISound::getTrackPortRoute(u8, u8) { }

void JAISound::getSeInfoPointer() { }
