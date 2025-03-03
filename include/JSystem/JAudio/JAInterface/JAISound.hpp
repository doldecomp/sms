#ifndef JAISOUND_HPP
#define JAISOUND_HPP

class JAIMoveParaSet;
class JAISeParameter;
class JAISeqParameter;
class JAIStreamParameter;

class JAISound {
public:
	JAISound();
	virtual ~JAISound();

	void initMoveParameter(JAIMoveParaSet*, float, unsigned long);
	void initMultiMoveParameter(JAIMoveParaSet*, unsigned char, unsigned long,
	                            float, float, unsigned long);
	void getSeCategoryNumber();
	void getDataInfoHeader();
	void getSwBit();
	void checkSwBit(unsigned long);
	void getInfoPriority();
	void clearMainSoundPPointer();
	void release();
	void start(unsigned long);
	void stop(unsigned long);
	void setVolume(float, unsigned long, unsigned char);
	void setDirectVolume(float, unsigned long);
	void setPan(float, unsigned long, unsigned char);
	void setDirectPan(float, unsigned long);
	void setPitch(float, unsigned long, unsigned char);
	void setDirectPitch(float, unsigned long);
	void setFxmix(float, unsigned long, unsigned char);
	void setDirectFxmix(float, unsigned long);
	void setDolby(float, unsigned long, unsigned char);
	void setDirectDolby(float, unsigned long);
	void setTempoProportion(float, unsigned long);
	void setPortData(unsigned char, unsigned short);
	void setPrepareFlag(unsigned char);
	void checkReady();
	void setDistanceVolumeCommon(float, unsigned char);
	void setDistancePanCommon();
	void setPositionDopplarCommon(unsigned long);
	void setSeqInterVolume(unsigned char, float, unsigned long);
	void setSeqInterPan(unsigned char, float, unsigned long);
	void setSeqInterPitch(unsigned char, float, unsigned long);
	void setSeqInterFxmix(unsigned char, float, unsigned long);
	void setSeqInterDolby(unsigned char, float, unsigned long);
	void setSeqTempoProportion(float, unsigned long);
	void setSeqPortData(unsigned char, unsigned short, unsigned long);
	void setWaveReadMode(long, long);
	void setTrackVolume(unsigned char, float, unsigned long);
	void setTrackVolumeMulti(unsigned char, unsigned long, float,
	                         unsigned long);
	void setTrackMuteSwitch(unsigned char, unsigned char);
	void setTrackMuteSwitchMulti(unsigned long, unsigned char);
	void setTrackInterruptSwitch(unsigned char, unsigned char);
	void setTrackPan(unsigned char, float, unsigned long);
	void setTrackPanMulti(unsigned char, unsigned long, float, unsigned long);
	void setTrackPitch(unsigned char, float, unsigned long);
	void setTrackPitchMulti(unsigned char, unsigned long, float, unsigned long);
	void setTrackFxmix(unsigned char, float, unsigned long);
	void setTrackFxmixMulti(unsigned char, unsigned long, float, unsigned long);
	void setTrackDolby(unsigned char, float, unsigned long);
	void setTrackDolbyMulti(unsigned char, unsigned long, float, unsigned long);
	void setTrackFirU7(unsigned char, unsigned char, unsigned long);
	void setTrackFirMultiU7(unsigned char, unsigned long, unsigned char,
	                        unsigned long);
	void setTrackPortData(unsigned char, unsigned char, unsigned short);
	void setSeInterMovePara(JAIMoveParaSet*, unsigned long);
	void setSeInterRandomPara(float*, unsigned long, float, float);
	void setSeInterVolume(unsigned char, float, unsigned long, unsigned char);
	void setSeInterPan(unsigned char, float, unsigned long, unsigned char);
	void setSeInterFxmix(unsigned char, float, unsigned long, unsigned char);
	void setSeInterFir(unsigned char, unsigned char, unsigned long,
	                   unsigned char);
	void setSeInterDolby(unsigned char, float, unsigned long, unsigned char);
	void setSeInterPitch(unsigned char, float, unsigned long, float);
	void setSePortData(unsigned char, unsigned short);
	void getSePortData(unsigned char);
	void setSeDistanceParameters();
	void setSeDistanceVolume(unsigned char);
	void setSeDistancePan(unsigned char);
	void setSeDistancePitch(unsigned char);
	void setSePositionDopplar();
	void setSeDistanceFxmix(unsigned char);
	void setSeDistanceFir(unsigned char);
	void setSeDistanceDolby(unsigned char);
	void setStreamMode(unsigned long);
	void setStreamInterVolume(unsigned char, float, unsigned long);
	void setStreamInterPitch(unsigned char, float, unsigned long);
	void setStreamInterPan(unsigned char, float, unsigned long);
	void setStreamPrepareFlag(unsigned char);
	void checkStreamReady();
	void setPauseMode(unsigned char, unsigned char);
	void setSeqPrepareFlag(unsigned char);
	void checkSeqReady();
	void getVolume(unsigned char);
	void getPan(unsigned char);
	void getPitch(unsigned char);
	void getFxmix(unsigned char);
	void getDolby(unsigned char);
	void getTempoProportion();
	void getPortData(unsigned char);
	void getSeqInterVolume(unsigned char);
	void getSeqInterPan(unsigned char);
	void getSeqInterPitch(unsigned char);
	void getSeqInterFxmix(unsigned char);
	void getSeqInterDolby(unsigned char);
	void getSeqTempoProportion();
	void getSeqPortData(unsigned char);
	void getTrackParameterU7(unsigned char, signed char);
	void getTrackParameterF32(unsigned char, signed char);
	void getPointer(unsigned char, signed char);
	void getTrackPortData(unsigned char, unsigned char);
	void getSeParametermeterF32(unsigned char, unsigned char);
	void getStreamInterVolume(unsigned char);
	void getStreamInterPitch(unsigned char);
	void getActorGroundNumber();
	void getSeqParameter();
	void getSeParameter();
	void getStreamParameter();
	void getTrackPortRoute(unsigned char, unsigned char);
	void getSeInfoPointer();

	void* interPointer;
};

#endif // JAISOUND_HPP
