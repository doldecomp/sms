#ifndef JAIBASIC_HPP
#define JAIBASIC_HPP

#include <JSystem/JKernel/JKRSolidHeap.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIActor.hpp>
#include <JSystem/JAudio/JAInterface/JAILinkBuffer.hpp>
#include <JSystem/JAudio/JAInterface/JAISeqParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAISeParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAIStreamParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAIDummyVec.hpp>
#include <JSystem/JAudio/JAInterface/JAISoundTable.hpp>
#include <JSystem/JAudio/JASystem/JASystem.hpp>

class Vec;

class JAIBasic {
public:
	static JAIBasic* basic;

	JAIBasic();
	virtual ~JAIBasic();

	void initDriver(JKRSolidHeap* heap, unsigned long param1,
	                unsigned char param2);
	void initInterface(unsigned char param);
	void initInterfaceMain();
	void initHeap();
	void initReadFile();
	void checkInitListFile();
	void loadDVDFile(char* filename);
	void checkInitDataFile();
	void loadTmpDVDFile(char* filename, unsigned char** buffer);
	void checkInitDataOnMemory();
	void transInitDataFile(unsigned char* buffer, unsigned long size);
	void allocHeap(unsigned long size);
	void initBankWave();
	void deleteTmpDVDFile(unsigned char** buffer);
	void setWaveScene();
	void initAllocParameter();
	void initNullData();
	void initSeqsLoadArea();
	void startSeSequence();
	void setCameraInfo(Vec* pos, Vec* dir, float (*mtx)[4],
	                   unsigned long param);
	void initStream();
	void setRegisterTrackCallback();
	void initAudioThread(JKRSolidHeap* heap, unsigned long param1,
	                     unsigned char param2);
	void bootDSP();
	void readInitSoundData();
	void loadFirstStayWave();
	void loadSecondStayWave();
	void setSceneSetFinishCallback(long param1, long param2);
	void finishSceneSet(unsigned long param);
	void loadSceneWave(long param1, long param2);
	void checkSceneWaveOnMemory(long param1, long param2);
	void loadGroupWave(long param1, long param2);
	void getWaveGroupNumber(long param);
	void getWaveLoadStatus(long param);
	void checkAllWaveLoadStatus();
	void setInitFileLoadSwitch(unsigned char flag);
	void startFrameInterfaceWork();
	void processFrameWork();
	void checkStream();
	void checkDummyPositionBuffer();
	void startSoundVec(unsigned long id, JAISound** sound, Vec* pos,
	                   unsigned long param1, unsigned long param2,
	                   unsigned char param3);
	void startSoundVecReturnHandle(unsigned long id, Vec* pos,
	                               unsigned long param1, unsigned long param2,
	                               unsigned char param3);
	void startSoundActor(unsigned long id, JAISound** sound, JAIActor* actor,
	                     unsigned long param, unsigned char flag);
	void startSoundActorReturnHandle(unsigned long id, JAIActor* actor,
	                                 unsigned long param, unsigned char flag);
	void startSoundDirectID(unsigned long id, JAISound** sound, JAIActor* actor,
	                        unsigned long param, unsigned char flag);
	void startSoundIndirectID(unsigned long id, JAISound** sound,
	                          JAIActor* actor, unsigned long param,
	                          unsigned char flag);
	void startSoundBasic(unsigned long id, JAISound** sound, JAIActor* actor,
	                     unsigned long param, unsigned char flag, void* data);
	void getPlayingSoundHandle(JAISound** sound, unsigned long param);
	void stopSoundHandle(JAISound* sound, unsigned long param);
	void changeIDToCategory(unsigned long id);
	void stopPlayingObjectSe(void* obj);
	void stopPlayingIDObjectSe(unsigned long id, void* obj);
	void stopPlayingCategorySe(unsigned char category);
	void stopPlayingCategoryObjectSe(unsigned char category, void* obj);
	void stopAllSe(void* obj);
	void stopAllSe(unsigned char param);
	void stopAllSe(unsigned char param, void* obj);
	void stopAllSeq(void* obj);
	void stopAllStream(void* obj);
	void stopActorSoundOneBuffer(void* actor, JAISound* sound);
	void stopIDSoundOneBuffer(unsigned long id, JAISound* sound);
	void stopIDActorSoundOneBuffer(unsigned long id, void* actor,
	                               JAISound* sound);
	void stopAllSound(void* obj);
	void getPlayingSoundLinkHeadPointer(unsigned long param);
	void stopAllSound(unsigned long param);
	void stopAllSound(unsigned long param, void* obj);
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
	void setPauseFlagAll(unsigned char flag);
	void checkPlayingSoundTrack(unsigned long param);
	void changeSoundScene(unsigned long scene);
	void getMapInfoFxline(unsigned long param);
	void getMapInfoGround(unsigned long param);
	void getMapInfoFxParameter(unsigned long param);
	void allocDvdBuffer(unsigned char* buffer, unsigned long param1,
	                    unsigned long param2);
	void deallocDvdBuffer(unsigned char* buffer);
	void getSeInfoMode();
	void getSeInfoStartPointer();
	void getSeInfoCategoryMax();
	void getInfoPointerFromID(unsigned long id);
	void getInfoFormat(JAISoundTable* table, unsigned long id);
	void setSeCancelSwitch(unsigned char param1, unsigned char param2);
	void setSeCategoryVolume(unsigned char category, unsigned char volume);
	void setParameterSeqSync(JASystem::TTrack* track, unsigned short param);
	void getSoundInfoFromID(unsigned long id);
	void getSeqTrackNumber(void* param);
	void getSoundPrioity(void* param);
	void getSoundSwBit(void* param);
	void setSeExtParameter(JAISound* sound);
	void routeToTrack(unsigned long param);
	void makeSound(unsigned long param);
	void allocStreamBuffer(void* buffer, long size);
	void deallocStreamBuffer();
	void loadArcSeqData(unsigned long param, bool flag);
};

#endif // JAIBASIC_HPP
