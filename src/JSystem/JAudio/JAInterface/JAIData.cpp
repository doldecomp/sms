#include <JSystem/JAudio/JAInterface/JAIData.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>
#include <JSystem/JAudio/JAInterface/JAISystemInterface.hpp>
#include <JSystem/JAudio/JAInterface/JAIConst.hpp>
#include <JSystem/JAudio/JASystem/JASDvdThread.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include <stdint.h>

JAIData::JAIData() { }

void JAIData::init()
{
	mSeTable.mData       = nullptr;
	mSeqTable.mData      = nullptr;
	mStreamTable.mData   = nullptr;
	mSeparateSoundTables = 0;
	mStreamList          = nullptr;
	mNextLoadOrder       = 0;
	mStayHeapCount       = 0;
}

void JAIData::initLinkBuffer(JAILinkBuffer* linkBuffer, u32 param)
{
	int i;
	u32 thing = JAIGlobalParameter::audioCameraMax * 0x1C;

	linkBuffer->mFreeHead = linkBuffer->mStorage;
	linkBuffer->mUsedHead = nullptr;

	linkBuffer->mStorage[0].mPrevSound = nullptr;
	linkBuffer->mStorage[0].mNextSound = &linkBuffer->mStorage[1];
	JAISound::interPointer             = unk1F4;
	linkBuffer->mStorage[0].mState     = SOUNDSTATE_Inactive;
	linkBuffer->mStorage[0].unk1C
	    = (JAISound::FabricatedPositionInfo*)unk1F4->allocHeap(thing);
	JUT_ASSERT(linkBuffer->mStorage[0].unk1C);
	for (i = 1; i < param - 1; ++i) {
		linkBuffer->mStorage[i].mPrevSound = &linkBuffer->mStorage[i - 1];
		linkBuffer->mStorage[i].mNextSound = &linkBuffer->mStorage[i + 1];
		JAISound::interPointer             = unk1F4;
		linkBuffer->mStorage[i].mState     = SOUNDSTATE_Inactive;
		linkBuffer->mStorage[i].unk1C
		    = (JAISound::FabricatedPositionInfo*)unk1F4->allocHeap(thing);
		JUT_ASSERT(linkBuffer->mStorage[i].unk1C);
	}
	linkBuffer->mStorage[i].mPrevSound = &linkBuffer->mStorage[i - 1];
	linkBuffer->mStorage[i].mNextSound = nullptr;
	JAISound::interPointer             = unk1F4;
	linkBuffer->mStorage[i].mState     = SOUNDSTATE_Inactive;
	linkBuffer->mStorage[i].unk1C
	    = (JAISound::FabricatedPositionInfo*)unk1F4->allocHeap(thing);
	JUT_ASSERT(linkBuffer->mStorage[i].unk1C);
}

void JAIData::initSeqParaLinkBuffer()
{
	int i;

	mSeqParameterFreeHead = &mSeqParameterBuffer[0];
	mSeqParameterUsedHead = nullptr;

	mSeqParameterBuffer[0].mPrev = nullptr;
	mSeqParameterBuffer[0].mNext = mSeqParameterBuffer + 1;
	for (i = 1; i < JAIGlobalParameter::seqControlBufferMax - 1; ++i) {
		mSeqParameterBuffer[i].mPrev = &mSeqParameterBuffer[i - 1];
		mSeqParameterBuffer[i].mNext = &mSeqParameterBuffer[i + 1];
	}
	mSeqParameterBuffer[i].mPrev = &mSeqParameterBuffer[i - 1];
	mSeqParameterBuffer[i].mNext = nullptr;

	for (int i = 0; i < JAIGlobalParameter::seqControlBufferMax; ++i)
		initSeqParameter(&mSeqParameterBuffer[i]);
}

void JAIData::initDummyVecLink()
{
	int i;

	mDummyVecFreeHead = (JAIDummyVec*)mDummyVecBuffer;
	mDummyVecUsedHead = nullptr;

	mDummyVecBuffer[0].mPrev = nullptr;
	mDummyVecBuffer[0].mNext = &mDummyVecBuffer[1];
	for (i = 1; i < JAIGlobalParameter::dummyPositionMax - 1; ++i) {
		mDummyVecBuffer[i].mPrev = &mDummyVecBuffer[i - 1];
		mDummyVecBuffer[i].mNext = &mDummyVecBuffer[i + 1];
	}
	mDummyVecBuffer[i].mPrev = &mDummyVecBuffer[i - 1];
	mDummyVecBuffer[i].mNext = nullptr;
}

void JAIData::initSeqParameter(JAISeqParameter* param)
{
	param->mWaitSceneSet        = 0xffffffff;
	param->mTempo.mCurrentValue = 1.0f;
	param->mTempo.mMoveCounter  = 0;
	param->mPauseMode           = 0;
	param->unk1756              = 0;

	param->mPortUpdate   = 0;
	param->mVolumeUpdate = 0;
	param->mPanUpdate    = 0;
	param->mPitchUpdate  = 0;
	param->mDolbyUpdate  = 0;

	param->mFxmixUpdate = 0;

	param->mTrackVolumeUpdate = 0;
	param->mTrackPanUpdate    = 0;
	param->mTrackPitchUpdate  = 0;
	param->mTrackFxmixUpdate  = 0;
	param->mTrackDolbyUpdate  = 0;
	param->mTrackFirUpdate    = 0;
	param->mTrackPortUpdate   = 0;

	for (u32 i = 0; i < JAIGlobalParameter::seqTrackMax; ++i) {
		param->mTrackVolume[i].mCurrentValue = 1.0f;
		param->mTrackPan[i].mCurrentValue    = 0.5f;
		param->mTrackPitch[i].mCurrentValue  = 1.0f;
		param->mTrackFxmix[i].mCurrentValue  = 0.0f;
		param->mTrackDolby[i].mCurrentValue  = 0.0f;
		param->mTrackFir[i].mCurrentValue    = 0.0f;

		param->mTrackVolume[i].mTargetValue = 1.0f;
		param->mTrackPan[i].mTargetValue    = 0.5f;
		param->mTrackPitch[i].mTargetValue  = 1.0f;
		param->mTrackFxmix[i].mTargetValue  = 0.0f;
		param->mTrackDolby[i].mTargetValue  = 0.0f;
		param->mTrackFir[i].mTargetValue    = 0.0f;

		param->mTrackVolume[i].mMoveCounter = 0;
		param->mTrackPan[i].mMoveCounter    = 0;
		param->mTrackPitch[i].mMoveCounter  = 0;
		param->mTrackFxmix[i].mMoveCounter  = 0;
		param->mTrackDolby[i].mMoveCounter  = 0;
		param->mTrackFir[i].mMoveCounter    = 0;

		param->mMuteBits[i].mCurrent    = 0;
		param->mMuteBits[i].mEnable     = 0;
		param->mTrackInterruptSwitch[i] = 0;

		param->mTrackPortDataUpdate[i] = 0;

		for (u32 j = 0; j < 16; ++j)
			param->mTrackPortData[i][j] = 0;
	}

	for (u32 i = 0; i < 16; ++i) {
		param->mPortData[i].mCurrentValue = 0.0f;
		param->mPortData[i].mMoveCounter  = 0;
	}

	for (u32 i = 0; i < JAIGlobalParameter::seqPlayTrackMax + 0xC; ++i) {
		param->mVolume[i].mCurrentValue = 1.0f;
		param->mPan[i].mCurrentValue    = 0.5f;
		param->mPitch[i].mCurrentValue  = 1.0f;
		param->mFxmix[i].mCurrentValue  = 0.0f;
		param->mDolby[i].mCurrentValue  = 0.0f;

		param->mVolume[i].mTargetValue = 1.0f;
		param->mPan[i].mTargetValue    = 0.5f;
		param->mPitch[i].mTargetValue  = 1.0f;
		param->mFxmix[i].mTargetValue  = 0.0f;
		param->mDolby[i].mTargetValue  = 0.0f;

		param->mVolume[i].mMoveCounter = 0;
		param->mPan[i].mMoveCounter    = 0;
		param->mPitch[i].mMoveCounter  = 0;
		param->mFxmix[i].mMoveCounter  = 0;
		param->mDolby[i].mMoveCounter  = 0;
	}
}

void JAIData::initSeParaLinkBuffer()
{
	int i;

	mSeParameterFreeHead = &mSeParameterBuffer[0];
	mSeParameterUsedHead = nullptr;

	mSeParameterBuffer[0].mPrev = nullptr;
	mSeParameterBuffer[0].mNext = &mSeParameterBuffer[1];
	for (i = 1; i < JAIGlobalParameter::seRegistMax
	                        * JAIGlobalParameter::getParamSeCategoryMax()
	                    - 1;
	     ++i) {
		mSeParameterBuffer[i].mPrev = &mSeParameterBuffer[i - 1];
		mSeParameterBuffer[i].mNext = &mSeParameterBuffer[i + 1];
	}
	mSeParameterBuffer[i].mPrev = &mSeParameterBuffer[i - 1];
	mSeParameterBuffer[i].mNext = nullptr;
}

void JAIData::initSePara(JAISeParameter* param)
{
	f32 dolby = JAIGlobalParameter::seDolbyCenterValue / 127.0f;
	for (int i = 0; i < 8; ++i) {
		param->mVolume[i].mCurrentValue = 1.0f;
		param->mPan[i].mCurrentValue    = 0.5f;
		param->mPitch[i].mCurrentValue  = 1.0f;
		param->mFxmix[i].mCurrentValue  = 0.0f;
		param->mFir[i].mCurrentValue    = 0.0f;
		param->mDolby[i].mCurrentValue  = dolby;

		param->mVolume[i].mMoveCounter = 0;
		param->mPan[i].mMoveCounter    = 0;
		param->mPitch[i].mMoveCounter  = 0;
		param->mFxmix[i].mMoveCounter  = 0;
		param->mFir[i].mMoveCounter    = 0;
		param->mDolby[i].mMoveCounter  = 0;
	}
	param->mVolume[7].mCurrentValue = -1.0f;
	param->mPan[7].mCurrentValue    = -1.0f;
	param->mPitch[7].mCurrentValue  = -1.0f;
	param->mFxmix[7].mCurrentValue  = -1.0f;
	param->mFir[7].mCurrentValue    = -1.0f;
	param->mDolby[7].mCurrentValue  = -1.0f;

	param->mVolumePointer = nullptr;
	param->mPanPointer    = nullptr;
	param->mPitchPointer  = nullptr;
	param->mFxmixPointer  = nullptr;
	param->mFirPointer    = nullptr;
	param->mDolbyPointer  = nullptr;

	param->mPortUpdate = 0;
}

void JAIData::initStreamParaLinkBuffer()
{
	int i;

	mStreamParameterFreeHead = &mStreamParameterBuffer[0];
	mStreamParameterUsedHead = nullptr;

	mStreamParameterBuffer[0].mPrev = nullptr;
	mStreamParameterBuffer[0].mNext = &mStreamParameterBuffer[1];
	for (i = 1; i < JAIGlobalParameter::streamParameterBufferMax - 1; ++i) {
		mStreamParameterBuffer[i].mPrev = &mStreamParameterBuffer[i - 1];
		mStreamParameterBuffer[i].mNext = &mStreamParameterBuffer[i + 1];
	}
	mStreamParameterBuffer[i].mPrev = &mStreamParameterBuffer[i - 1];
	mStreamParameterBuffer[i].mNext = nullptr;

	for (int i = 0; i < JAIGlobalParameter::streamParameterBufferMax; ++i)
		initStreamParameter(&mStreamParameterBuffer[i]);
}

void JAIData::initStreamParameter(JAIStreamParameter* param)
{
	param->mPauseMode    = 0;
	param->mStreamMode   = 0;
	param->mVolumeUpdate = 0;
	param->mPitchUpdate  = 0;

	for (int i = 0; i < 13; ++i) {
		param->mVolume[i].mCurrentValue = 1.0f;
		param->mVolume[i].mTargetValue  = 1.0f;
		param->mVolume[i].mMoveCounter  = 0;
		param->mPitch[i].mCurrentValue  = 1.0f;
		param->mPitch[i].mTargetValue   = 1.0f;
		param->mPitch[i].mMoveCounter   = 0;
		param->mPan[i].mCurrentValue    = 0.5f;
		param->mPan[i].mTargetValue     = 0.5f;
		param->mPan[i].mMoveCounter     = 0;
	}
}

void JAIData::initSeqTrackInfoParameter(u32 param)
{
	mSeqTrackInfo[param].mSeqVolume = 1.0f;
	mSeqTrackInfo[param].mSeqPan    = 0.5f;
	mSeqTrackInfo[param].mSeqPitch  = 1.0f;
	mSeqTrackInfo[param].mSeqFxmix  = 0.0f;
	mSeqTrackInfo[param].mSeqDolby  = 0.0f;
	mSeqTrackInfo[param].mSeqTempo  = 1.0f;
	for (int i = 0; i < JAIGlobalParameter::seqTrackMax; ++i) {
		mSeqTrackInfo[param].mTrackVolume[i] = 1.0f;
		mSeqTrackInfo[param].mTrackPan[i]    = 64.0f;
		mSeqTrackInfo[param].mTrackPitch[i]  = 1.0f;
		mSeqTrackInfo[param].mTrackFxmix[i]  = 0.0f;
		mSeqTrackInfo[param].mTrackDolby[i]  = 0.0f;
		mSeqTrackInfo[param].mTrackUpdate[i] = 0;
	}
}

void JAIData::initStreamUpdateParameter()
{
	mStreamUpdate->unk0             = 0;
	mStreamUpdate->unk1             = 0;
	mStreamUpdate->mPrepareFlag     = 0;
	mStreamUpdate->mVolume          = 1.0f;
	mStreamUpdate->mPitch           = 1.0f;
	mStreamUpdate->mPan             = 0.5f;
	mStreamUpdate->mActiveTrackFlag = 0;
	mStreamUpdate->mSound           = nullptr;
}

void JAIData::setSeMovePara(JAIMoveParaSet* moveParaSet)
{
	for (u8 i = 0; i < 8; ++i) {
		if (!moveParaSet[i].mMoveCounter)
			continue;

		--moveParaSet[i].mMoveCounter;
		if (moveParaSet[i].mMoveCounter == 0)
			moveParaSet[i].mCurrentValue = moveParaSet[i].mTargetValue;
		else
			moveParaSet[i].mCurrentValue += moveParaSet[i].mMoveAmount;
	}
}

BOOL JAIData::moveParameter(JAIMoveParaSet* moveParaSet)
{
	if (moveParaSet->mMoveCounter == 0)
		return false;

	if (--moveParaSet->mMoveCounter) {
		moveParaSet->mCurrentValue -= moveParaSet->mMoveAmount;
		return true;
	}

	moveParaSet->mCurrentValue = moveParaSet->mTargetValue;
	return false;
}

void* JAIData::checkOnMemory(u32 param1, u8* param2)
{
	for (u8 i = 0; i < JAIGlobalParameter::autoHeapMax; ++i) {
		if (param1 != mAutoHeap[i].mSeqNumber)
			continue;

		if (mAutoHeap[i].mLoadedFlag == 1)
			return (void*)0xffffffff;

		if (param2)
			*param2 = i;

		return mAutoHeap[i].mPointer;
	}

	for (u8 i = 0; i < mStayHeapCount; ++i) {
		if (param1 != mStayHeap[i].mSeqNumber)
			continue;

		if (param2 != 0)
			*param2 = 0xff;

		return mStayHeap[i].mPointer;
	}
	return nullptr;
}

u8 JAIData::checkUsefulAutoHeapPosition()
{
	int i            = 0;
	u32 smallestSize = -1;
	int smallest     = 0;

	for (; i < JAIGlobalParameter::autoHeapMax; ++i)
		if (mAutoHeap[i].mSeqNumber == -1)
			break;

	if (i == JAIGlobalParameter::autoHeapMax) {
		for (i = 0; i < JAIGlobalParameter::autoHeapMax; ++i) {
			if (smallestSize > mAutoHeap[i].mLoadOrder
			    && mAutoHeap[i].mUseOrder == -1) {
				smallest     = i;
				smallestSize = mAutoHeap[i].mLoadOrder;
			}
		}

		if (smallestSize != -1)
			i = smallest;
	}

	return i;
}

void* JAIData::getFreeAutoHeapPointer(u8 param1, u32 param2)
{
	mAutoHeap[param1].mSeqNumber = param2;
	void* result                 = mAutoHeap[param1].mPointer;
	mAutoHeap[param1].mUseOrder  = mNextLoadOrder;
	mAutoHeap[param1].mLoadOrder = mNextLoadOrder;
	++mNextLoadOrder;
	return result;
}

void JAIData::releaseAutoHeapPointer(u8 param)
{
	if (param == 0xff)
		return;
	mAutoHeap[param].mUseOrder = -1;
}

u8* JAIData::getFreeStayHeapPointer(u32 param1, u32 param2)
{
	if (mStayHeapCount >= JAIGlobalParameter::stayHeapMax)
		return nullptr;

	u8* result;

	if (param1 + (uintptr_t)mStayHeap[mStayHeapCount].mPointer
	        < (uintptr_t)mStayHeap[0].mPointer
	              + JAIGlobalParameter::stayHeapSize
	    && mStayHeapCount < JAIGlobalParameter::stayHeapMax) {
		result = (u8*)mStayHeap[mStayHeapCount].mPointer;
		mStayHeap[mStayHeapCount].mSeqNumber = param2;
		u8* ptr                              = (u8*)((param1 & ~0x1F)
                        + (uintptr_t)mStayHeap[mStayHeapCount].mPointer);
		if (param1 & 0x1F)
			ptr += 0x20;
		++mStayHeapCount;
		if (mStayHeapCount < JAIGlobalParameter::stayHeapMax)
			mStayHeap[mStayHeapCount].mPointer = ptr;
	} else {
		result = nullptr;
	}
	return result;
}

void JAIData::clearAutoHeap(JAIHeapBlock* heapBlock) { }

void JAIData::clearAutoHeap(u32 param) { }

void JAIData::clearStayHeap(u32 param) { }

void JAIData::setAutoHeapLoadedFlag(u8 param1, u8 param2)
{
	mAutoHeap[param1].mLoadedFlag = param2;
}

void JAIData::changeAutoHeapPointerToPosition(u8* ptr) { }

void JAIData::stopPlayingSeq(u32 param) { }

u8* JAIData::getAutoHeapPointer(u32 param) { return nullptr; }

void JAIData::getInfoPointer(u32 sound_id, void** result)
{
	JAISoundTable* table;
	u32 category;

	*result = &JAIConst::nullInfoData2;
	if (mSeparateSoundTables == 0) {
		table = &mSeTable;
		switch (sound_id & JAISoundID_TypeMask) {
		case JAISoundID_Type_Se:
			category = (u8)(sound_id >> 12);
			JAIGlobalParameter::getParamSeCategoryMax();
			break;
		case JAISoundID_Type_Sequence:
			category = 16;
			break;
		case JAISoundID_Type_Stream:
			category = 17;
			break;
		}
	} else {
		switch (sound_id & JAISoundID_TypeMask) {
		case JAISoundID_Type_Se:
			category = (u8)(sound_id >> 12);
			table    = &mSeTable;
			JAIGlobalParameter::getParamSeCategoryMax();
			break;
		case JAISoundID_Type_Sequence:
			table    = &mSeqTable;
			category = 16;
			break;
		case JAISoundID_Type_Stream:
			table    = &mStreamTable;
			category = 17;
			break;
		}
	}

	u32 index = sound_id & JAISoundID_IndexMask;
	if (table->mData && index < table->mSoundMax[category])
		*result = &table->mCategorySoundInfos[category][index];
	else
		*result = nullptr;
}

void JAIData::initData()
{
	initInfoDataWork(&mSeTable, JAIGlobalParameter::seInfoFileName);
	if (mSeparateSoundTables == 1) {
		initInfoDataWork(&mSeqTable, JAIGlobalParameter::seqInfoFileName);
		initInfoDataWork(&mStreamTable, JAIGlobalParameter::streamInfoFileName);
	}
	if (unk1F4->mSoundSceneList) {
		JAIGlobalParameter::seTrackMax = 0;
		for (int i = 0; i < JAIGlobalParameter::soundSceneMax; ++i) {
			u32 sum = 0;
			for (int j = 0; j < JAIGlobalParameter::getParamSeCategoryMax();
			     ++j) {
				sum += unk1F4->mSoundSceneList[i][j].mMaxPlaying;
			}
			if (JAIGlobalParameter::seTrackMax < sum)
				JAIGlobalParameter::seTrackMax = sum;
		}
	}
	mDummyVecBuffer = (JAIDummyVec*)unk1F4->allocHeap(
	    JAIGlobalParameter::dummyPositionMax * sizeof(JAIDummyVec));
	initDummyVecLink();

	mSeRegist = (JAILinkBuffer*)unk1F4->allocHeap(
	    JAIGlobalParameter::getParamSeCategoryMax() * sizeof(JAILinkBuffer));

	mSeTrack = (FabricatedSeTrack**)unk1F4->allocHeap(
	    JAIGlobalParameter::getParamSeCategoryMax()
	    * sizeof(FabricatedSeTrack*));

	mSeRegistStorage = (JAISound**)unk1F4->allocHeap(
	    JAIGlobalParameter::getParamSeCategoryMax() * sizeof(JAISound*));

	for (int i = 0; i < JAIGlobalParameter::getParamSeCategoryMax(); ++i) {
		mSeRegistStorage[i]
		    = unk1F4->makeSound(JAIGlobalParameter::seRegistMax);
		mSeRegist[i].mStorage = mSeRegistStorage[i];
		initLinkBuffer(&mSeRegist[i], JAIGlobalParameter::seRegistMax);
		mSeTrack[i] = (FabricatedSeTrack*)unk1F4->allocHeap(
		    JAIGlobalParameter::seRegistMax * sizeof(FabricatedSeTrack));
		for (int j = 0; j < JAIGlobalParameter::seRegistMax; ++j) {
			mSeTrack[i][j].mSound = nullptr;
		}
	}
	unk0 = (FabricatedSeTrackParameter*)unk1F4->allocHeap(
	    JAIGlobalParameter::seTrackMax * 0x18);
	for (u32 i = 0; i < JAIGlobalParameter::seTrackMax; ++i) {
		FabricatedSeTrackParameter& s = unk0[i];

		s.mVolume = 1.0f;
		s.mPitch  = 1.0f;
		s.mFxmix  = 0.0f;
		s.mPan    = 0.5f;
		s.unk0    = 0xff;
		s.mDolby  = 0.0f;
	}

	mSeqControlStorage
	    = unk1F4->makeSound(JAIGlobalParameter::seqControlBufferMax);
	mStreamControlStorage
	    = unk1F4->makeSound(JAIGlobalParameter::streamControlBufferMax);
	mSeqControlBuffer.mStorage = mSeqControlStorage;
	initLinkBuffer(&mSeqControlBuffer, JAIGlobalParameter::seqControlBufferMax);
	mStreamControlBuffer.mStorage = mStreamControlStorage;
	initLinkBuffer(&mStreamControlBuffer,
	               JAIGlobalParameter::streamControlBufferMax);
	mSeqParameterBuffer = (JAISeqParameter*)unk1F4->allocHeap(
	    JAIGlobalParameter::seqControlBufferMax * sizeof(JAISeqParameter));
	initSeqParaLinkBuffer();
	mSeParameterBuffer = (JAISeParameter*)unk1F4->allocHeap(
	    JAIGlobalParameter::seRegistMax
	    * JAIGlobalParameter::getParamSeCategoryMax() * sizeof(JAISeParameter));
	initSeParaLinkBuffer();
	mStreamParameterBuffer = (JAIStreamParameter*)unk1F4->allocHeap(
	    JAIGlobalParameter::streamParameterBufferMax
	    * sizeof(JAIStreamParameter));
	initStreamParaLinkBuffer();
	for (int i = 0; i < JAIGlobalParameter::seRegistMax
	                        * JAIGlobalParameter::getParamSeCategoryMax();
	     ++i) {
		initSePara(&mSeParameterBuffer[i]);
	}

	mAutoHeap = (JAIHeapBlock*)unk1F4->allocHeap(JAIGlobalParameter::autoHeapMax
	                                             * sizeof(JAIHeapBlock));
	for (int i = 0; i < JAIGlobalParameter::autoHeapMax; ++i) {
		mAutoHeap[i].mLoadedFlag = 0;
		mAutoHeap[i].mLoadOrder  = 0;
		mAutoHeap[i].mSeqNumber  = -1;
		mAutoHeap[i].mUseOrder   = -1;
	}

	mStayHeap = (JAIHeapBlock*)unk1F4->allocHeap(JAIGlobalParameter::stayHeapMax
	                                             * sizeof(JAIHeapBlock));
	for (int i = 0; i < JAIGlobalParameter::stayHeapMax; ++i) {
		mStayHeap[i].mLoadedFlag = 0;
		mStayHeap[i].mLoadOrder  = 0;
		mStayHeap[i].mSeqNumber  = -1;
		mStayHeap[i].mUseOrder   = -1;
	}

	mDefaultSeqHandle = (JAISound**)unk1F4->allocHeap(
	    JAIGlobalParameter::seqPlayTrackMax * sizeof(JAISound*));
	mSeqTrackInfo = (JAISeqUpdateData*)unk1F4->allocHeap(
	    JAIGlobalParameter::seqPlayTrackMax * sizeof(JAISeqUpdateData));
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		mSeqTrackInfo[i].unk4C
		    = (JAISeqUpdateData::FabricatedUnk4CStruct*)unk1F4->allocHeap(
		        0x7BC);
		mDefaultSeqHandle[i]    = 0;
		mSeqTrackInfo[i].unk0   = 0;
		mSeqTrackInfo[i].unk1   = 0;
		mSeqTrackInfo[i].unk2   = 0;
		mSeqTrackInfo[i].unk3   = 0;
		mSeqTrackInfo[i].unk8   = 0;
		mSeqTrackInfo[i].mSound = 0;

		mSeqTrackInfo[i].mTrackVolume = (f32*)unk1F4->allocHeap(
		    JAIGlobalParameter::seqTrackMax * sizeof(f32));
		mSeqTrackInfo[i].mTrackPan = (f32*)unk1F4->allocHeap(
		    JAIGlobalParameter::seqTrackMax * sizeof(f32));
		mSeqTrackInfo[i].mTrackPitch = (f32*)unk1F4->allocHeap(
		    JAIGlobalParameter::seqTrackMax * sizeof(f32));
		mSeqTrackInfo[i].mTrackFxmix = (f32*)unk1F4->allocHeap(
		    JAIGlobalParameter::seqTrackMax * sizeof(f32));
		mSeqTrackInfo[i].mTrackDolby = (f32*)unk1F4->allocHeap(
		    JAIGlobalParameter::seqTrackMax * sizeof(f32));
		mSeqTrackInfo[i].mTrackUpdate = (u32*)unk1F4->allocHeap(
		    (JAIGlobalParameter::seqTrackMax + 1) * sizeof(u32));
		initSeqTrackInfoParameter(i);
	}
	mStreamUpdate = (JAIStreamUpdateParameter*)unk1F4->allocHeap(
	    sizeof(JAIStreamUpdateParameter));
	mStreamUpdate->unk0             = 0;
	mStreamUpdate->unk1             = 0;
	mStreamUpdate->mPrepareFlag     = 0;
	mStreamUpdate->mVolume          = 1.0f;
	mStreamUpdate->mPitch           = 1.0f;
	mStreamUpdate->mPan             = 0.5f;
	mStreamUpdate->mActiveTrackFlag = 0;
	mStreamUpdate->mSound           = nullptr;

	if (unk1F4->mSoundSceneList) {
		mCategoryInfoTable = unk1F4->mSoundSceneList;
	} else {
		mCategoryInfoTable = (JAICategoryInfo**)unk1F4->allocHeap(
		    JAIGlobalParameter::soundSceneMax * 4);
		for (int i = 0; i < JAIGlobalParameter::soundSceneMax; ++i)
			mCategoryInfoTable[i] = JAIConst::sCInfos_0;
	}

	if (unk1F4->mFxSceneTable) {
		JAIBasic::FabricatedFxSceneTable* tmp = unk1F4->mFxSceneTable;
		setFxSceneMax(tmp->mSceneMax);
		setFxBufferMax(tmp->mBufferMax[0], tmp->mBufferMax[1],
		               tmp->mBufferMax[2], tmp->mBufferMax[3]);

		void* table = unk1F4->allocHeap(
		    mFxSceneMax * sizeof(JASystem::DSPInterface::FxlineConfig_*));
		JUT_ASSERT(table);
		mFxlineConfig = (JASystem::DSPInterface::FxlineConfig_**)table;
		for (u8 i = 0; i < mFxSceneMax; ++i) {
			u8* tmp2 = (u8*)unk1F4->mFxSceneTable + tmp->mSceneOffset[(int)i];
			mFxlineConfig[i] = (JASystem::DSPInterface::FxlineConfig_*)tmp2;
		}
		for (u8 i = 0; i < 4; ++i) {
			if (!mFxBufferMax[i])
				continue;
			s16* buf = (s16*)unk1F4->allocHeap(mFxBufferMax[i] * 0xA0);
			JUT_ASSERT(buf);
			mFxBuffer[i] = buf;
			JASystem::DSPInterface::setFXLine(i, mFxBuffer[i],
			                                  &mFxlineConfig[0][i]);
		}
	}
}

void JAIData::initInfoDataWork(JAISoundTable* soundTable, char* path)
{
	u32 size;
	if (!soundTable->mData)
		size = JASystem::Dvd::checkFile(path);
	else
		size = soundTable->mDataSize;

	if (size == 0)
		return;

	if (!soundTable->mData) {
		soundTable->mData = (u8*)unk1F4->allocHeap(size);
		// TODO: fakematch?
		void* tmp = soundTable->mData;
		JASystem::Dvd::loadFile(path, tmp);
		soundTable->mDataSize = size;
	}
	soundTable->unk0 = soundTable->mData[3];
	// BUG: pointer to a local recorded in persistent storage
	soundTable->unk2C = &path;

	// TODO: you'd think structs were used here but apparently not?
	// maybe I didn't try hard enough
	for (u8 i = 0; i < 18; ++i) {
		soundTable->mSoundMax[i] = *(u16*)(&soundTable->mData[i * 4 + 6]);

		u32 idx = *(u16*)(&soundTable->mData[i * 4 + 8]);
		soundTable->mCategorySoundInfos[i]
		    = &((JAISoundInfo*)(soundTable->mData + 0x50))[idx];
		if (i < 0x10 && soundTable->mSoundMax[i] != 0) {
			soundTable->mCategoryMax = i + 1;
		}
	}
}

void JAIData::reloadInfoDataWork(JAISoundTable* soundTable)
{
	char* path = *(char**)soundTable->unk2C;
	u32 size   = JASystem::Dvd::checkFile(path);
	if (size == 0)
		return;
	if (!soundTable->mData) {
		soundTable->mData = (u8*)unk1F4->allocHeap(size);
		void* tmp         = soundTable->mData;
		JASystem::Dvd::loadFile(path, tmp);
		soundTable->mDataSize = size;
	}
}

void JAIData::setInfoDataPointer(JAISoundTable* soundTable, u8* ptr)
{
	soundTable->mData = ptr;
	soundTable->unk0  = soundTable->mData[3];
	// TODO: you'd think structs were used here but apparently not?
	// maybe I didn't try hard enough
	for (u8 i = 0; i < 18; ++i) {
		soundTable->mSoundMax[i] = *(u16*)(&soundTable->mData[i * 4 + 6]);

		u32 idx = *(u16*)(&soundTable->mData[i * 4 + 8]);
		soundTable->mCategorySoundInfos[i]
		    = &((JAISoundInfo*)(soundTable->mData + 0x50))[idx];
		if (i < 0x10 && soundTable->mSoundMax[i] != 0) {
			soundTable->mCategoryMax = i + 1;
		}
	}
}
