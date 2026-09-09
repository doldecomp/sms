#ifndef JAIDATA_HPP
#define JAIDATA_HPP

#include <JSystem/JAudio/JAInterface/JAIEntrySequence.hpp>
#include <JSystem/JAudio/JAInterface/JAIEntrySe.hpp>
#include <JSystem/JAudio/JAInterface/JAIEntryStream.hpp>
#include <JSystem/JAudio/JASystem/JASDSPInterface.hpp>
#include <types.h>

class JAIBasic;
class JAISound;
struct JAISeqUpdateData;
class JAIDummyVec {
public:
	/* 0x0 */ JAIDummyVec* mPrev;
	/* 0x4 */ JAIDummyVec* mNext;
	/* 0x8 */ JAISound* mSound;
	/* 0xC */ u32 mLifeTime;
	/* 0x10 */ char unk10[0xC];
};
struct JAIStreamUpdateParameter;

// fabricated
struct JAICategoryInfo {
	/* 0x0 */ u8 mMaxPlaying;
	/* 0x1 */ u8 mMaxSameSound;
};

// fabricated
struct JAISoundInfo {
	/* 0x0 */ u32 mSwBit;
	/* 0x4 */ u8 mPriority;
	/* 0x5 */ u8 mTrackNumber;
	/* 0x6 */ u16 mOffsetNo;
	/* 0x8 */ f32 mPitch;
	/* 0xC */ u8 mVolume;
	/* 0xD */ u8 mFxmix;
};

struct FabricatedSoundTableMemoryHeader {
	/* 0x0 */ char unk0[3];
	/* 0x3 */ u8 unk3;
	/* 0x4 */ char unk4[2];
	struct Pair {
		s16 unk0;
		s16 unk2;
	};
	/* 0x6 */ Pair unk6[18];
	/* 0x50 */ JAISoundInfo unk50[];
};

struct JAISoundTable {
	/* 0x0 */ u8 unk0;
	/* 0x1 */ u8 mCategoryMax;
	/* 0x2 */ u16 mSoundMax[18];
	/* 0x28 */ u32 mDataSize;
	/* 0x2C */ void* unk2C;
	/* 0x30 */ JAISoundInfo* mCategorySoundInfos[18];
	/* 0x78 */ u8* mData;
};

struct JAIHeapBlock {
	/* 0x0 */ u8 mLoadedFlag;
	/* 0x4 */ void* mPointer;
	/* 0x8 */ u32 mSeqNumber;
	/* 0xC */ u32 mLoadOrder;
	/* 0x10 */ u32 mUseOrder;
};

struct JAIMoveParaSet;
class JAILinkBuffer {
public:
	/* 0x0 */ JAISound* mFreeHead;
	/* 0x4 */ JAISound* mUsedHead;
	/* 0x8 */ JAISound* mStorage;
};
struct JAISeqParameter;
struct JAISeParameter;
struct JAIStreamParameter;

class JAIData {
public:
	JAIData();

	void init();
	void initData();
	void initInfoDataWork(JAISoundTable* soundTable, char* param);
	void initLinkBuffer(JAILinkBuffer* linkBuffer, u32 param);
	void initSeqParaLinkBuffer();
	void initSeParaLinkBuffer();
	void initStreamParaLinkBuffer();
	void initDummyVecLink();
	void initSeqParameter(JAISeqParameter* param);
	void initSePara(JAISeParameter* param);
	void initStreamParameter(JAIStreamParameter* param);
	void initSeqTrackInfoParameter(u32 param);
	void initStreamUpdateParameter();

	void setSeMovePara(JAIMoveParaSet* moveParaSet);
	BOOL moveParameter(JAIMoveParaSet* moveParaSet);

	void* checkOnMemory(u32 param1, u8* param2);
	u8 checkUsefulAutoHeapPosition();
	void* getFreeAutoHeapPointer(u8 param1, u32 param2);
	void releaseAutoHeapPointer(u8 param);
	u8* getFreeStayHeapPointer(u32 param1, u32 param2);

	void clearAutoHeap(JAIHeapBlock* heapBlock);
	void clearAutoHeap(u32 param);
	void clearStayHeap(u32 param);

	void setAutoHeapLoadedFlag(u8 param1, u8 param2);
	void changeAutoHeapPointerToPosition(u8* ptr);
	void stopPlayingSeq(u32 param);
	u8* getAutoHeapPointer(u32 param);
	void getInfoPointer(u32 param, void** ptr);

	void reloadInfoDataWork(JAISoundTable* soundTable);
	void setInfoDataPointer(JAISoundTable* soundTable, u8* ptr);

	// from tww
	void setFxSceneMax(u8 param_1) { mFxSceneMax = param_1; }
	void setFxBufferMax(u32 param_1, u32 param_2, u32 param_3, u32 param_4)
	{
		mFxBufferMax[0] = param_1;
		mFxBufferMax[1] = param_2;
		mFxBufferMax[2] = param_3;
		mFxBufferMax[3] = param_4;
	}

public:
	struct FabricatedSeTrackParameter {
		/* 0x0 */ u8 unk0;
		/* 0x4 */ f32 mVolume;
		/* 0x8 */ f32 mPitch;
		/* 0xC */ f32 mFxmix;
		/* 0x10 */ f32 mPan;
		/* 0x14 */ f32 mDolby;
	};

	/* 0x0 */ FabricatedSeTrackParameter* unk0;
	/* 0x4 */ JAICategoryInfo** mCategoryInfoTable;

	struct FabricatedSeTrack {
		/* 0x0 */ u32 unk0;
		/* 0x4 */ u32 unk4;
		/* 0x8 */ JAISound* mSound;
	};

	/* 0x8 */ FabricatedSeTrack** mSeTrack;
	/* 0xC */ JAISoundTable mSeqTable;
	/* 0x88 */ JAISoundTable mSeTable;
	/* 0x104 */ JAISoundTable mStreamTable;
	/* 0x180 */ JAISeqUpdateData* mSeqTrackInfo;
	/* 0x184 */ JAIStreamUpdateParameter* mStreamUpdate;
	/* 0x188 */ u8 mFxSceneMax;
	/* 0x18C */ u32 mFxBufferMax[4];
	/* 0x19C */ s16* mFxBuffer[4];
	/* 0x1AC */ JASystem::DSPInterface::FxlineConfig_** mFxlineConfig;
	/* 0x1B0 */ u8 mSeparateSoundTables;
	/* 0x1B4 */ u32 mNextLoadOrder;
	/* 0x1B8 */ u32 mStayHeapCount;
	/* 0x1BC */ JAISeqParameter* mSeqParameterFreeHead;
	/* 0x1C0 */ JAISeqParameter* mSeqParameterUsedHead;
	/* 0x1C4 */ JAISeqParameter* mSeqParameterBuffer;
	/* 0x1C8 */ JAISeParameter* mSeParameterFreeHead;
	/* 0x1CC */ JAISeParameter* mSeParameterUsedHead;
	/* 0x1D0 */ JAISeParameter* mSeParameterBuffer;
	/* 0x1D4 */ JAIStreamParameter* mStreamParameterFreeHead;
	/* 0x1D8 */ JAIStreamParameter* mStreamParameterUsedHead;
	/* 0x1DC */ JAIStreamParameter* mStreamParameterBuffer;
	/* 0x1E0 */ JAISound** mDefaultSeqHandle;
	/* 0x1E4 */ JAISound** mSeRegistStorage;
	/* 0x1E8 */ JAILinkBuffer* mSeRegist;
	/* 0x1EC */ JAIHeapBlock* unk1EC;
	/* 0x1F0 */ JAIHeapBlock* unk1F0;
	/* 0x1F4 */ JAIBasic* unk1F4;

	struct FabricatedStreamEntry {
		/* 0x0 */ char unk0[0x10];
		/* 0x10 */ char mFileName[0x10];
		/* 0x20 */ void* unk20;
		/* 0x24 */ char unk24[0xC];
	};

	/* 0x1F8 */ FabricatedStreamEntry* mStreamList;
	/* 0x1FC */ JAISeqEntry unk1FC;
	/* 0x200 */ JAISeEntry unk200;
	/* 0x204 */ JAIStreamEntry unk204;
	/* 0x208 */ JAISound* mSeqControlStorage;
	/* 0x20C */ JAISound* mStreamControlStorage;
	/* 0x210 */ JAILinkBuffer mSeqControlBuffer;
	/* 0x21C */ JAILinkBuffer mStreamControlBuffer;
	/* 0x228 */ JAIDummyVec* mDummyVecFreeHead;
	/* 0x22C */ JAIDummyVec* mDummyVecUsedHead;
	/* 0x230 */ JAIDummyVec* mDummyVecBuffer;
};

#endif // JAIDATA_HPP
