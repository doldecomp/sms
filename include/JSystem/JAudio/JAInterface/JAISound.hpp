#ifndef JAISOUND_HPP
#define JAISOUND_HPP

#include <types.h>

class JAIMoveParaSet;
class JAISeParameter;
class JAISeqParameter;
class JAIStreamParameter;

class JAISound {
public:
	JAISound();
	~JAISound();

	virtual void setSeDistanceParameters();
	virtual void setSeDistanceVolume(u8);
	virtual void setSeDistancePan(u8);
	virtual void setSeDistancePitch(u8);
	virtual void setSeDistanceFxmix(u8);
	virtual void setSeDistanceFir(u8);
	virtual void setSeDistanceDolby(u8);
	virtual void setSePositionDopplar();
	virtual void setPositionDopplarCommon(u32);
	virtual void setDistanceVolumeCommon(f32, u8);
	virtual void setDistancePanCommon();

	void initMoveParameter(JAIMoveParaSet*, f32, u32);
	void initMultiMoveParameter(JAIMoveParaSet*, u8, u32, f32, f32, u32);
	void getSeCategoryNumber();
	void getDataInfoHeader();
	void getSwBit();
	void checkSwBit(u32);
	void getInfoPriority();
	void clearMainSoundPPointer();
	void release();
	void start(u32);
	void stop(u32);
	void setVolume(f32, u32, u8);
	void setDirectVolume(f32, u32);
	void setPan(f32, u32, u8);
	void setDirectPan(f32, u32);
	void setPitch(f32, u32, u8);
	void setDirectPitch(f32, u32);
	void setFxmix(f32, u32, u8);
	void setDirectFxmix(f32, u32);
	void setDolby(f32, u32, u8);
	void setDirectDolby(f32, u32);
	void setTempoProportion(f32, u32);
	void setPortData(u8, u16);
	void setPrepareFlag(u8);
	void checkReady();
	void setSeqInterVolume(u8, f32, u32);
	void setSeqInterPan(u8, f32, u32);
	void setSeqInterPitch(u8, f32, u32);
	void setSeqInterFxmix(u8, f32, u32);
	void setSeqInterDolby(u8, f32, u32);
	void setSeqTempoProportion(f32, u32);
	void setSeqPortData(u8, u16, u32);
	void setWaveReadMode(s32, s32);
	void setTrackVolume(u8, f32, u32);
	void setTrackVolumeMulti(u8, u32, f32, u32);
	void setTrackMuteSwitch(u8, u8);
	void setTrackMuteSwitchMulti(u32, u8);
	void setTrackInterruptSwitch(u8, u8);
	void setTrackPan(u8, f32, u32);
	void setTrackPanMulti(u8, u32, f32, u32);
	void setTrackPitch(u8, f32, u32);
	void setTrackPitchMulti(u8, u32, f32, u32);
	void setTrackFxmix(u8, f32, u32);
	void setTrackFxmixMulti(u8, u32, f32, u32);
	void setTrackDolby(u8, f32, u32);
	void setTrackDolbyMulti(u8, u32, f32, u32);
	void setTrackFirU7(u8, u8, u32);
	void setTrackFirMultiU7(u8, u32, u8, u32);
	void setTrackPortData(u8, u8, u16);
	void setSeInterMovePara(JAIMoveParaSet*, u32);
	void setSeInterRandomPara(f32*, u32, f32, f32);
	void setSeInterVolume(u8, f32, u32, u8);
	void setSeInterPan(u8, f32, u32, u8);
	void setSeInterFxmix(u8, f32, u32, u8);
	void setSeInterFir(u8, u8, u32, u8);
	void setSeInterDolby(u8, f32, u32, u8);
	void setSeInterPitch(u8, f32, u32, f32);
	void setSePortData(u8, u16);
	void getSePortData(u8);
	void setStreamMode(u32);
	void setStreamInterVolume(u8, f32, u32);
	void setStreamInterPitch(u8, f32, u32);
	void setStreamInterPan(u8, f32, u32);
	void setStreamPrepareFlag(u8);
	void checkStreamReady();
	void setPauseMode(u8, u8);
	void setSeqPrepareFlag(u8);
	void checkSeqReady();
	void getVolume(u8);
	void getPan(u8);
	void getPitch(u8);
	void getFxmix(u8);
	void getDolby(u8);
	void getTempoProportion();
	void getPortData(u8);
	void getSeqInterVolume(u8);
	void getSeqInterPan(u8);
	void getSeqInterPitch(u8);
	void getSeqInterFxmix(u8);
	void getSeqInterDolby(u8);
	void getSeqTempoProportion();
	void getSeqPortData(u8);
	void getTrackParameterU7(u8, s8);
	void getTrackParameterF32(u8, s8);
	void getPointer(u8, s8);
	void getTrackPortData(u8, u8);
	void getSeParametermeterF32(u8, u8);
	void getStreamInterVolume(u8);
	void getStreamInterPitch(u8);
	void getActorGroundNumber();
	void getSeqParameter();
	void getSeParameter();
	void getStreamParameter();
	void getTrackPortRoute(u8, u8);
	void getSeInfoPointer();

public:
	/* 0x0 */ void* interPointer;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ u32 unk8;
};

#endif // JAISOUND_HPP
