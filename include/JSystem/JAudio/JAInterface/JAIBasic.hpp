#ifndef JAIBASIC_HPP
#define JAIBASIC_HPP

#include <JSystem/JAudio/JAInterface/JAIData.hpp>
#include <dolphin/mtx.h>
#include <JSystem/JKernel/JKRSolidHeap.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JASystem/JASTrack.hpp>

class JAICamera {
public:
	JAICamera();
	JAICamera(VecPtr param_1, VecPtr param_2, MtxPtr param_3)
	    : unk0(param_1)
	    , unk4(param_2)
	    , unk8(param_3)
	{
	}

	/* 0x0 */ VecPtr unk0;
	/* 0x4 */ VecPtr unk4;
	/* 0x8 */ MtxPtr unk8;
};

class JAIActor;
class JAILinkBuffer;
class JAIDummyVec;
class JAISoundTable;

class JAIBasic {
public:
	/* 0x0 */ JAIData* unk0;
	/* 0x4 */ // vt
public:
	JAIBasic();
	~JAIBasic();

	virtual void initStream();
	virtual JAISound* makeSound(u32 param);
	virtual BOOL getMapInfoFxline(u32 param);
	virtual u32 getMapInfoGround(u32 param);
	virtual f32 getMapInfoFxParameter(u32 param);
	virtual void sendPlayingSeCommand();
	virtual void sendSeAllParameter(JAISound* sound);
	virtual void setSeExtParameter(JAISound* sound);
	virtual void setRegisterTrackCallback();
	virtual void checkStream();
	virtual void checkNextFrameSe();
	virtual void loadGroupWave(s32 param1, s32 param2);

	void initDriver(JKRSolidHeap* heap, u32 param1, u8 param2);
	void initInterface(u8 param);
	void initInterfaceMain();
	void initHeap();
	void initReadFile();
	BOOL checkInitListFile();
	void* loadDVDFile(char* filename);
	BOOL checkInitDataFile();
	void loadTmpDVDFile(char* filename, unsigned char** buffer);
	void checkInitDataOnMemory();
	void* transInitDataFile(unsigned char* buffer, u32 size);
	void* allocHeap(u32 size);
	void initBankWave();
	void deleteTmpDVDFile(unsigned char** buffer);
	void setWaveScene();
	void initAllocParameter();
	void initNullData();
	void initSeqsLoadArea();
	void startSeSequence();
	void setCameraInfo(VecPtr pos, VecPtr dir, MtxPtr mtx, u32 param);
	void initAudioThread(JKRSolidHeap* heap, u32 param1, u8 param2);
	void bootDSP();
	void readInitSoundData();
	void loadFirstStayWave();
	void loadSecondStayWave();
	void setSceneSetFinishCallback(s32 param1, s32 param2);
	static void finishSceneSet(u32 param);
	void loadSceneWave(s32 param1, s32 param2);
	BOOL checkSceneWaveOnMemory(s32 param1, s32 param2);
	void getWaveGroupNumber(s32 param);
	void getWaveLoadStatus(s32 param);
	void checkAllWaveLoadStatus();
	void setInitFileLoadSwitch(u8 flag);
	void startFrameInterfaceWork();
	void processFrameWork();
	void checkDummyPositionBuffer();
	void startSoundVec(u32 id, JAISound** sound, Vec* pos, u32 param1,
	                   u32 param2, u8 param3);
	void startSoundVecReturnHandle(u32 id, Vec* pos, u32 param1, u32 param2,
	                               u8 param3);
	void startSoundActor(u32 id, JAISound** sound, JAIActor* actor, u32 param,
	                     u8 flag);
	JAISound* startSoundActorReturnHandle(u32 id, JAIActor* actor, u32 param,
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
	JAISound* getControllerHandle(JAILinkBuffer* buffer);
	void releaseControllerHandle(JAILinkBuffer* buffer, JAISound* sound);
	JAIStreamParameter* getStreamParameter();
	void releaseStreamParameterPointer(JAIStreamParameter* param);
	JAISeqParameter* getSeqParametermeterPointer();
	void releaseSeqParameterPointer(JAISeqParameter* param);
	JAISeParameter* getSeParametermeterPointer();
	void releaseSeParameterPointer(JAISeParameter* param);
	void getDummyVecPointer();
	void releaseDummyVecPointer(JAIDummyVec* vec);
	void getGameFrameCounter();
	void setPauseFlagAll(u8 flag);
	void checkPlayingSoundTrack(u32 param);
	void changeSoundScene(u32 scene);
	void allocDvdBuffer(unsigned char* buffer, u32 param1, u32 param2);
	void deallocDvdBuffer(unsigned char* buffer);
	void getSeInfoMode();
	void getSeInfoStartPointer();
	void getSeInfoCategoryMax();
	JAISoundTable* getInfoPointerFromID(u32 id);
	u32 getInfoFormat(JAISoundTable* table, u32 id);
	void setSeCancelSwitch(u8 param1, u8 param2);
	void setSeCategoryVolume(u8 category, u8 volume);
	static u16 setParameterSeqSync(JASystem::TTrack* track, u16 param);
	JAISoundInfo* getSoundInfoFromID(u32 id);
	u8 getSeqTrackNumber(void* param);
	u8 getSoundPrioity(void* param);
	u32 getSoundSwBit(void* param);
	u32 routeToTrack(u32 param);
	void allocStreamBuffer(void* buffer, s32 size);
	void deallocStreamBuffer();
	int loadArcSeqData(u32 param, bool flag);
	void setSeqMuteFromSeStart(JAISound* sound);
	void clearSeqMuteFromSeStop(JAISound* sound);
	void checkSeMovePara();
	void releaseSeRegist(JAISound* sound);
	void checkEntriedSeq();
	void checkPlayingSeqTrack(unsigned long trackID);
	void checkPlayingSeq();
	void checkStoppedSeq();
	void checkStartedSeq();
	void checkFadeoutSeq();
	void checkReadSeq();
	void checkSeqWave();
	static void checkDvdLoadArc(unsigned long arcID);
	void stopSeq(JAISound* sound);
	void checkEntriedStream();
	void checkWaitStream();
	void checkRequestStream();
	void checkPlayingStream();

	static JAIBasic* basic;

public:
	/* 0x8 */ JAICamera* unk8;
	/* 0xC */ JKRSolidHeap* unkC;
	/* 0x10 */ u8 unk10;
	/* 0x11 */ u8 unk11;
	/* 0x12 */ u8 unk12;
	/* 0x13 */ u8 unk13;
	/* 0x14 */ u8 unk14;
	/* 0x15 */ u8 unk15;
	/* 0x16 */ char unk16[2];
	/* 0x18 */ u8* unk18;
	/* 0x1C */ struct {
		u8 flag1 : 1;
		u8 flag2 : 1;
		u8 flag3 : 1;
		u8 flag4 : 1;
		u8 flag5 : 1;
		u8 flag6 : 1;
		u8 flag7 : 1;
		u8 flag8 : 1;
	} unk1C;
	/* 0x20 */ u32 unk20;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ f32* unk28;
	/* 0x2C */ u32 unk2C;
	/* 0x30 */ u32 unk30;
	/* 0x34 */ u32 unk34;
	/* 0x38 */ JAISound* unk38;

	struct FabricatedUnk3CStruct {
		char unk0[1]; // TODO: is this just a size 0x30 str or nah?
		char unk1[0x2F];
	};
	/* 0x3C */ FabricatedUnk3CStruct* unk3C;

	// TODO: same as unk3C?
	struct FabricatedUnk40Struct {
		char unk0[1];
		char unk1[0x2F];
	};
	/* 0x40 */ FabricatedUnk40Struct* unk40;
	/* 0x44 */ u32 unk44;
	/* 0x48 */ u32 unk48;
	/* 0x4C */ u8* unk4C;

	// TODO: same as Unk54 struct?
	struct FabricatedUnk50Struct {
		void* unk0;
		char unk4[0x4];
		int unk8;
	};

	/* 0x50 */ FabricatedUnk50Struct* unk50;

	struct FabricatedUnk54Struct {
		void* unk0;
		u32 unk4;
		u32 unk8;
	};

	/* 0x54 */ FabricatedUnk54Struct* unk54;
	/* 0x58 */ u8** unk58; // TODO: wrong type
	/* 0x5C */ u32 unk5C;
	/* 0x60 */ s32* unk60;
	/* 0x64 */ s32* unk64;
	/* 0x68 */ u8** unk68; // TODO: might be pair of u8s actually
	/* 0x6C */ u32* unk6C; // TODO: wrong type
	/* 0x70 */ u32 unk70;
	/* 0x74 */ char unk74[0x4];
	/* 0x78 */ u32 unk78;
	/* 0x7C */ char unk7C[0xC];
	/* 0x88 */ u32 unk88;
	/* 0x104 */ char unk104[0x304 - 0x88];
	/* 0x304 */ JASystem::TTrack::TOuterParam* unk304;
	/* 0x308 */ char unk308[0x4];
};

#endif // JAIBASIC_HPP
