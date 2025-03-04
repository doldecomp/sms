#ifndef JAIBASIC_HPP
#define JAIBASIC_HPP

#include <dolphin/mtx.h>
#include <JSystem/JKernel/JKRSolidHeap.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JASystem/JASTrack.hpp>

class JAIActor;
class JAILinkBuffer;
class JAIDummyVec;
class JAISoundTable;

class JAIBasic {
public:
	JAIBasic();
	virtual ~JAIBasic();

	void initDriver(JKRSolidHeap* heap, u32 param1, u8 param2);
	void initInterface(u8 param);
	void initInterfaceMain();
	void initHeap();
	void initReadFile();
	void checkInitListFile();
	void loadDVDFile(char* filename);
	void checkInitDataFile();
	void loadTmpDVDFile(char* filename, unsigned char** buffer);
	void checkInitDataOnMemory();
	void transInitDataFile(unsigned char* buffer, u32 size);
	void allocHeap(u32 size);
	void initBankWave();
	void deleteTmpDVDFile(unsigned char** buffer);
	void setWaveScene();
	void initAllocParameter();
	void initNullData();
	void initSeqsLoadArea();
	void startSeSequence();
	void setCameraInfo(VecPtr pos, VecPtr dir, MtxPtr mtx, u32 param);
	void initStream();
	void setRegisterTrackCallback();
	void initAudioThread(JKRSolidHeap* heap, u32 param1, u8 param2);
	void bootDSP();
	void readInitSoundData();
	void loadFirstStayWave();
	void loadSecondStayWave();
	void setSceneSetFinishCallback(s32 param1, s32 param2);
	void finishSceneSet(u32 param);
	void loadSceneWave(s32 param1, s32 param2);
	void checkSceneWaveOnMemory(s32 param1, s32 param2);
	void loadGroupWave(s32 param1, s32 param2);
	void getWaveGroupNumber(s32 param);
	void getWaveLoadStatus(s32 param);
	void checkAllWaveLoadStatus();
	void setInitFileLoadSwitch(u8 flag);
	void startFrameInterfaceWork();
	void processFrameWork();
	void checkStream();
	void checkDummyPositionBuffer();
	void startSoundVec(u32 id, JAISound** sound, Vec* pos, u32 param1,
	                   u32 param2, u8 param3);
	void startSoundVecReturnHandle(u32 id, Vec* pos, u32 param1, u32 param2,
	                               u8 param3);
	void startSoundActor(u32 id, JAISound** sound, JAIActor* actor, u32 param,
	                     u8 flag);
	void startSoundActorReturnHandle(u32 id, JAIActor* actor, u32 param,
	                                 u8 flag);
	void startSoundDirectID(u32 id, JAISound** sound, JAIActor* actor,
	                        u32 param, u8 flag);
	void startSoundIndirectID(u32 id, JAISound** sound, JAIActor* actor,
	                          u32 param, u8 flag);
	void startSoundBasic(u32 id, JAISound** sound, JAIActor* actor, u32 param,
	                     u8 flag, void* data);
	void getPlayingSoundHandle(JAISound** sound, u32 param);
	void stopSoundHandle(JAISound* sound, u32 param);
	u32 changeIDToCategory(u32 id);
	void stopPlayingObjectSe(void* obj);
	void stopPlayingIDObjectSe(u32 id, void* obj);
	void stopPlayingCategorySe(u8 category);
	void stopPlayingCategoryObjectSe(u8 category, void* obj);
	void stopAllSe(void* obj);
	void stopAllSe(u8 param);
	void stopAllSe(u8 param, void* obj);
	void stopAllSeq(void* obj);
	void stopAllStream(void* obj);
	void stopActorSoundOneBuffer(void* actor, JAISound* sound);
	void stopIDSoundOneBuffer(u32 id, JAISound* sound);
	void stopIDActorSoundOneBuffer(u32 id, void* actor, JAISound* sound);
	void stopAllSound(void* obj);
	void getPlayingSoundLinkHeadPointer(u32 param);
	void stopAllSound(u32 param);
	void stopAllSound(u32 param, void* obj);
	void deleteObject(void* obj);
	void releaseSoundHandle(JAISound* sound);
	void getControllerHandle(JAILinkBuffer* buffer);
	void releaseControllerHandle(JAILinkBuffer* buffer, JAISound* sound);
	void getStreamParameter();
	void releaseStreamParameterPointer(JAIStreamParameter* param);
	void getSeqParametermeterPointer();
	void releaseSeqParameterPointer(JAISeqParameter* param);
	void getSeParametermeterPointer();
	void releaseSeParameterPointer(JAISeParameter* param);
	void getDummyVecPointer();
	void releaseDummyVecPointer(JAIDummyVec* vec);
	void getGameFrameCounter();
	void setPauseFlagAll(u8 flag);
	void checkPlayingSoundTrack(u32 param);
	void changeSoundScene(u32 scene);
	void getMapInfoFxline(u32 param);
	void getMapInfoGround(u32 param);
	void getMapInfoFxParameter(u32 param);
	void allocDvdBuffer(unsigned char* buffer, u32 param1, u32 param2);
	void deallocDvdBuffer(unsigned char* buffer);
	void getSeInfoMode();
	void getSeInfoStartPointer();
	void getSeInfoCategoryMax();
	void getInfoPointerFromID(u32 id);
	void getInfoFormat(JAISoundTable* table, u32 id);
	void setSeCancelSwitch(u8 param1, u8 param2);
	void setSeCategoryVolume(u8 category, u8 volume);
	void setParameterSeqSync(JASystem::TTrack* track, u16 param);
	void getSoundInfoFromID(u32 id);
	void getSeqTrackNumber(void* param);
	void getSoundPrioity(void* param);
	void getSoundSwBit(void* param);
	void setSeExtParameter(JAISound* sound);
	void routeToTrack(u32 param);
	void makeSound(u32 param);
	void allocStreamBuffer(void* buffer, s32 size);
	void deallocStreamBuffer();
	void loadArcSeqData(u32 param, bool flag);
	void checkNextFrameSe();
	void sendPlayingSeCommand();
	void setSeqMuteFromSeStart(JAISound* sound);
	void clearSeqMuteFromSeStop(JAISound* sound);
	void checkSeMovePara();
	void sendSeAllParameter(JAISound* sound);
	void releaseSeRegist(JAISound* sound);
	void checkEntriedSeq();
	void checkPlayingSeqTrack(unsigned long trackID);
	void checkPlayingSeq();
	void checkStoppedSeq();
	void checkStartedSeq();
	void checkFadeoutSeq();
	void checkReadSeq();
	void checkSeqWave();
	void checkDvdLoadArc(unsigned long arcID);
	void stopSeq(JAISound* sound);
	void checkEntriedStream();
	void checkWaitStream();
	void checkRequestStream();
	void checkPlayingStream();
};

#endif // JAIBASIC_HPP
