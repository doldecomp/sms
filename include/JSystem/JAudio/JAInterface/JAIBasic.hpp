#ifndef JAIBASIC_HPP
#define JAIBASIC_HPP

#include <JSystem/JAudio/JAInterface/JAIData.hpp>
#include <dolphin/mtx.h>
#include <JSystem/JKernel/JKRSolidHeap.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JASystem/JASTrack.hpp>

class JAICamera {
public:
	JAICamera()
	    : mPosition(nullptr)
	    , mPrevPosition(nullptr)
	    , nViewMtx(nullptr)
	{
	}
	JAICamera(VecPtr param_1, VecPtr param_2, MtxPtr param_3)
	    : mPosition(param_1)
	    , mPrevPosition(param_2)
	    , nViewMtx(param_3)
	{
	}

	Vec toCamSpace(Vec in) const
	{
		Vec out;
		MTXMultVec(nViewMtx, &in, &out);
		return out;
	}

	/* 0x0 */ VecPtr mPosition;
	/* 0x4 */ VecPtr mPrevPosition;
	/* 0x8 */ MtxPtr nViewMtx;
};

extern JAICamera JAInullCamera;

class JAIActor;
class JAILinkBuffer;
class JAIDummyVec;
struct JAISoundTable;

class JAIBasic {
public:
	/* 0x0 */ JAIData* unk0;
	/* 0x4 */ // vt
public:
	JAIBasic();
	~JAIBasic();

	virtual void initStream();
	virtual JAISound* makeSound(u32 param);
	virtual u16 getMapInfoFxline(u32 param);
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
	bool checkSceneWaveOnMemory(s32 param1, s32 param2);
	s32 getWaveGroupNumber(s32 param);
	s32 getWaveLoadStatus(s32 param);
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
	void checkPlayingSeqTrack(u32 trackID);
	void checkPlayingSeq();
	void checkStoppedSeq();
	void checkStartedSeq();
	void checkFadeoutSeq();
	void checkReadSeq();
	void checkSeqWave();
	static void checkDvdLoadArc(u32 arcID);
	void stopSeq(JAISound* sound);
	void checkEntriedStream();
	void checkWaitStream();
	void checkRequestStream();
	void checkPlayingStream();

	static JAIBasic* basic;

	static JAIBasic* getInterface() { return basic; }

public:
	/* 0x8 */ JAICamera* mAudioCameras;
	/* 0xC */ JKRSolidHeap* mInterfaceHeap;
	/* 0x10 */ u8 mSoundScene;
	/* 0x11 */ u8 unk11;
	/* 0x12 */ u8 unk12;
	/* 0x13 */ u8 mInitFileLoadSwitch;
	/* 0x14 */ u8 mSoundOutputMode;
	/* 0x15 */ u8 unk15;
	/* 0x16 */ char unk16[2];
	/* 0x18 */ u8* mSeCancelSwitch;
	/* 0x1C */ struct {
		u8 mFirstStayWaveLoaded : 1;
		u8 mSecondStayWaveLoaded : 1;
		u8 mInitDataLoadOff : 1;
		u8 mSeqEntryCancel : 1;
		u8 mStreamEntryCancel : 1;
		u8 mStreamUseOff : 1;
		u8 mStreamInsideBufferCut : 1;
	} unk1C;
	/* 0x20 */ u32 unk20;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ f32* mSeCategoryVolume;
	/* 0x2C */ u32 mSeqArchiveHandle;
	/* 0x30 */ u32 unk30;
	/* 0x34 */ u32 mFinishedSceneSet;
	/* 0x38 */ JAISound* mSeSequence;

	struct FabricatedFileNameEntry {
		/* 0x0 */ char mFileName[0x30];
	};

	/* 0x3C */ FabricatedFileNameEntry* mBankFileList;
	/* 0x40 */ FabricatedFileNameEntry* mWaveBankFileList;
	/* 0x44 */ u32 unk44;
	/* 0x48 */ u32 unk48;
	/* 0x4C */ u8* mInitDataPointer;

	struct FabricatedBankEntry {
		/* 0x0 */ void* mBankData;
		/* 0x4 */ char unk4[0x4];
		/* 0x8 */ int mWaveBankNumber;
	};

	/* 0x50 */ FabricatedBankEntry* mBankList;

	enum {
		WAVE_LOAD_TIMING_FIRST_STAY  = 0,
		WAVE_LOAD_TIMING_SECOND_STAY = 1,
		WAVE_LOAD_TIMING_SCENE       = 2,
	};

	struct FabricatedWaveBankEntry {
		/* 0x0 */ void* mWaveBankData;
		/* 0x4 */ u32 unk4;
		/* 0x8 */ u32 mLoadTiming;
	};

	/* 0x54 */ FabricatedWaveBankEntry* mWaveBankList;

	struct FabricatedSeqArchiveHeader {
		/* 0x0 */ u8* mData;
		/* 0x4 */ u32 mSize;
	};

	/* 0x58 */ FabricatedSeqArchiveHeader* mSeqArchiveHeader;

	struct FabricatedStreamListHeader {
		/* 0x0 */ JAIData::FabricatedStreamEntry* mData;
		/* 0x4 */ u32 mSize;
	};

	/* 0x5C */ FabricatedStreamListHeader* mStreamListHeader;
	/* 0x60 */ s32* mWaveGroupNumber;

	enum {
		WAVE_LOAD_STATUS_NOT_LOADED = 0,
		WAVE_LOAD_STATUS_LOADING    = 1,
		WAVE_LOAD_STATUS_LOADED     = 2,
	};

	/* 0x64 */ s32* mWaveLoadStatus;

	struct FabricatedSoundSceneTable {
		/* 0x0 */ u32 mSceneMax;
		/* 0x4 */ JAICategoryInfo* mSceneData[];
	};

	// Points at FabricatedSoundSceneTable::mSceneData.
	/* 0x68 */ JAICategoryInfo** mSoundSceneList;

	struct FabricatedFxSceneTable {
		/* 0x0 */ u32 mSceneMax;
		/* 0x4 */ u32 mBufferMax[4];
		/* 0x14 */ u32 mSceneOffset[];
	};

	/* 0x6C */ FabricatedFxSceneTable* mFxSceneTable;
	/* 0x70 */ u32 unk70;
	/* 0x74 */ char unk74[0x4];
	/* 0x78 */ u8* unk78;
	/* 0x7C */ JAISound* unk7C;
	/* 0x80 */ JAISound* unk80;
	/* 0x84 */ u32 unk84;
	/* 0x88 */ u8 unk88;
	/* 0x8C */ JAISound* unk8C[2];
	/* 0x94 */ u16 unk94;
	// TODO: some of the fields might actually be
	// from a derived class, MSound.
};

#endif // JAIBASIC_HPP
