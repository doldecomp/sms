#include <JSystem/JAudio/JAInterface/JAIData.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>
#include <JSystem/JAudio/JAInterface/JAISystemInterface.hpp>
#include <JSystem/JAudio/JAInterface/JAIConst.hpp>
#include <JSystem/JAudio/JASystem/JASDvdThread.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>

JAIData::JAIData() { }

void JAIData::init()
{
	mSeTable.unk78       = 0;
	mSeqTable.unk78      = 0;
	mStreamTable.unk78   = 0;
	mSeparateSoundTables = 0;
	mStreamList          = 0;
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
	param->unk1758            = 0xffffffff;
	param->unk4.mCurrentValue = 1.0f;
	param->unk4.mMoveCounter  = 0;
	param->unk1755            = 0;
	param->unk1756            = 0;

	// TODO: array? but why ops reordered tho?
	param->unk175C = 0;
	param->unk1760 = 0;
	param->unk1764 = 0;
	param->unk1768 = 0;
	param->unk1770 = 0;

	param->unk176C = 0;

	param->unk1774 = 0;
	param->unk1778 = 0;
	param->unk177C = 0;
	param->unk1780 = 0;
	param->unk1784 = 0;
	param->unk1788 = 0;
	param->unk178C = 0;

	for (u32 i = 0; i < JAIGlobalParameter::seqTrackMax; ++i) {
		param->unk754[i].mCurrentValue  = 1.0f;
		param->unk954[i].mCurrentValue  = 0.5f;
		param->unkB54[i].mCurrentValue  = 1.0f;
		param->unkD54[i].mCurrentValue  = 0.0f;
		param->unkF54[i].mCurrentValue  = 0.0f;
		param->unk1154[i].mCurrentValue = 0.0f;

		param->unk754[i].mTargetValue  = 1.0f;
		param->unk954[i].mTargetValue  = 0.5f;
		param->unkB54[i].mTargetValue  = 1.0f;
		param->unkD54[i].mTargetValue  = 0.0f;
		param->unkF54[i].mTargetValue  = 0.0f;
		param->unk1154[i].mTargetValue = 0.0f;

		param->unk754[i].mMoveCounter  = 0;
		param->unk954[i].mMoveCounter  = 0;
		param->unkB54[i].mMoveCounter  = 0;
		param->unkD54[i].mMoveCounter  = 0;
		param->unkF54[i].mMoveCounter  = 0;
		param->unk1154[i].mMoveCounter = 0;

		param->unk1830[i].flag1 = 0;
		param->unk1830[i].flag3 = 0;
		param->unk1810[i]       = 0;

		param->unk1790[i] = 0;

		param->unk1354[i][0]  = 0;
		param->unk1354[i][1]  = 0;
		param->unk1354[i][2]  = 0;
		param->unk1354[i][3]  = 0;
		param->unk1354[i][4]  = 0;
		param->unk1354[i][5]  = 0;
		param->unk1354[i][6]  = 0;
		param->unk1354[i][7]  = 0;
		param->unk1354[i][8]  = 0;
		param->unk1354[i][9]  = 0;
		param->unk1354[i][10] = 0;
		param->unk1354[i][11] = 0;
		param->unk1354[i][12] = 0;
		param->unk1354[i][13] = 0;
		param->unk1354[i][14] = 0;
		param->unk1354[i][15] = 0;
	}

	for (u32 i = 0; i < 16; ++i) {
		param->unk14[i].mCurrentValue = 0.0f;
		param->unk14[i].mMoveCounter  = 0;
	}

	for (u32 i = 0; i < JAIGlobalParameter::seqPlayTrackMax + 0xC; ++i) {
		param->unk114[i].mCurrentValue = 1.0f;
		param->unk254[i].mCurrentValue = 0.5f;
		param->unk394[i].mCurrentValue = 1.0f;
		param->unk4D4[i].mCurrentValue = 0.0f;
		param->unk614[i].mCurrentValue = 0.0f;

		param->unk114[i].mTargetValue = 1.0f;
		param->unk254[i].mTargetValue = 0.5f;
		param->unk394[i].mTargetValue = 1.0f;
		param->unk4D4[i].mTargetValue = 0.0f;
		param->unk614[i].mTargetValue = 0.0f;

		param->unk114[i].mMoveCounter = 0;
		param->unk254[i].mMoveCounter = 0;
		param->unk394[i].mMoveCounter = 0;
		param->unk4D4[i].mMoveCounter = 0;
		param->unk614[i].mMoveCounter = 0;
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
		param->unk124[i].mCurrentValue = 1.0f;
		param->unk1A4[i].mCurrentValue = 0.5f;
		param->unk224[i].mCurrentValue = 1.0f;
		param->unk2A4[i].mCurrentValue = 0.0f;
		param->unk324[i].mCurrentValue = 0.0f;
		param->unk3A4[i].mCurrentValue = dolby;

		param->unk124[i].mMoveCounter = 0;
		param->unk1A4[i].mMoveCounter = 0;
		param->unk224[i].mMoveCounter = 0;
		param->unk2A4[i].mMoveCounter = 0;
		param->unk324[i].mMoveCounter = 0;
		param->unk3A4[i].mMoveCounter = 0;
	}
	param->unk124[7].mCurrentValue = -1.0f;
	param->unk1A4[7].mCurrentValue = -1.0f;
	param->unk224[7].mCurrentValue = -1.0f;
	param->unk2A4[7].mCurrentValue = -1.0f;
	param->unk324[7].mCurrentValue = -1.0f;
	param->unk3A4[7].mCurrentValue = -1.0f;

	param->unk424 = 0;
	param->unk428 = 0;
	param->unk42C = 0;
	param->unk430 = 0;
	param->unk434 = 0;
	param->unk438 = 0;

	param->unk20 = 0;
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
	param->unk0 = 0;
	param->unk4 = 0;
	param->unk8 = 0;
	param->unkC = 0;

	for (int i = 0; i < 13; ++i) {
		param->unk14[i].mCurrentValue  = 1.0f;
		param->unk14[i].mTargetValue   = 1.0f;
		param->unk14[i].mMoveCounter   = 0;
		param->unk154[i].mCurrentValue = 1.0f;
		param->unk154[i].mTargetValue  = 1.0f;
		param->unk154[i].mMoveCounter  = 0;
		param->unk294[i].mCurrentValue = 0.5f;
		param->unk294[i].mTargetValue  = 0.5f;
		param->unk294[i].mMoveCounter  = 0;
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
	mStreamUpdate->unk0  = 0;
	mStreamUpdate->unk1  = 0;
	mStreamUpdate->unk2  = 0;
	mStreamUpdate->unk4  = 1.0f;
	mStreamUpdate->unk8  = 1.0f;
	mStreamUpdate->unkC  = 0.5f;
	mStreamUpdate->unk10 = 0;
	mStreamUpdate->unk14 = nullptr;
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
		if (param1 != unk1EC[i].mSeqNumber)
			continue;

		if (unk1EC[i].mLoadedFlag == 1)
			return (void*)0xffffffff;

		if (param2)
			*param2 = i;

		return unk1EC[i].mPointer;
	}

	for (u8 i = 0; i < mStayHeapCount; ++i) {
		if (param1 != unk1F0[i].mSeqNumber)
			continue;

		if (param2 != 0)
			*param2 = 0xff;

		return unk1F0[i].mPointer;
	}
	return nullptr;
}

u8 JAIData::checkUsefulAutoHeapPosition()
{
	int i            = 0;
	u32 smallestSize = -1;
	int smallest     = 0;

	for (; i < JAIGlobalParameter::autoHeapMax; ++i)
		if (unk1EC[i].mSeqNumber == -1)
			break;

	if (i == JAIGlobalParameter::autoHeapMax) {
		for (i = 0; i < JAIGlobalParameter::autoHeapMax; ++i) {
			if (smallestSize > unk1EC[i].mLoadOrder
			    && unk1EC[i].mUseOrder == -1) {
				smallest     = i;
				smallestSize = unk1EC[i].mLoadOrder;
			}
		}

		if (smallestSize != -1)
			i = smallest;
	}

	return i;
}

void* JAIData::getFreeAutoHeapPointer(u8 param1, u32 param2)
{
	unk1EC[param1].mSeqNumber = param2;
	void* result              = unk1EC[param1].mPointer;
	unk1EC[param1].mUseOrder  = mNextLoadOrder;
	unk1EC[param1].mLoadOrder = mNextLoadOrder;
	++mNextLoadOrder;
	return result;
}

void JAIData::releaseAutoHeapPointer(u8 param)
{
	if (param == 0xff)
		return;
	unk1EC[param].mUseOrder = -1;
}

u8* JAIData::getFreeStayHeapPointer(u32 param1, u32 param2)
{
	if (mStayHeapCount >= JAIGlobalParameter::stayHeapMax)
		return nullptr;

	u8* result;

	if ((u8*)unk1F0[mStayHeapCount].mPointer + param1
	        < (u8*)unk1F0[0].mPointer + JAIGlobalParameter::stayHeapSize
	    && mStayHeapCount < JAIGlobalParameter::stayHeapMax) {
		result = (u8*)unk1F0[mStayHeapCount].mPointer;
		unk1F0[mStayHeapCount].mSeqNumber = param2;
		u8* ptr = (u8*)unk1F0[mStayHeapCount].mPointer + (param1 & ~0x1F);
		if (param1 & 0x1F)
			ptr += 0x20;
		++mStayHeapCount;
		if (mStayHeapCount < JAIGlobalParameter::stayHeapMax)
			unk1F0[mStayHeapCount].mPointer = ptr;
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
	unk1EC[param1].mLoadedFlag = param2;
}

void JAIData::changeAutoHeapPointerToPosition(u8* ptr) { }

void JAIData::stopPlayingSeq(u32 param) { }

u8* JAIData::getAutoHeapPointer(u32 param) { return nullptr; }

void JAIData::getInfoPointer(u32 param_1, void** param_2)
{
	JAISoundTable* table;
	u32 thing;

	*param_2 = &JAIConst::nullInfoData2;
	if (mSeparateSoundTables == 0) {
		table = &mSeTable;
		switch (param_1 & JAISoundID_TypeMask) {
		case JAISoundID_Type_Se:
			thing = (u8)(param_1 >> 12);
			JAIGlobalParameter::getParamSeCategoryMax();
			break;
		case JAISoundID_Type_Sequence:
			thing = 16;
			break;
		case JAISoundID_Type_Stream:
			thing = 17;
			break;
		}
	} else {
		switch (param_1 & JAISoundID_TypeMask) {
		case JAISoundID_Type_Se:
			thing = (u8)(param_1 >> 12);
			table = &mSeTable;
			JAIGlobalParameter::getParamSeCategoryMax();
			break;
		case JAISoundID_Type_Sequence:
			table = &mSeqTable;
			thing = 0x10;
			break;
		case JAISoundID_Type_Stream:
			table = &mStreamTable;
			thing = 0x11;
			break;
		}
	}

	u32 tmp = param_1 & 0x3FF;
	if (table->unk78 && tmp < table->unk2[thing])
		*param_2 = &table->unk30[thing][tmp];
	else
		*param_2 = nullptr;
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

	unk1EC = (JAIHeapBlock*)unk1F4->allocHeap(JAIGlobalParameter::autoHeapMax
	                                          * sizeof(JAIHeapBlock));
	for (int i = 0; i < JAIGlobalParameter::autoHeapMax; ++i) {
		unk1EC[i].mLoadedFlag = 0;
		unk1EC[i].mLoadOrder  = 0;
		unk1EC[i].mSeqNumber  = -1;
		unk1EC[i].mUseOrder   = -1;
	}

	unk1F0 = (JAIHeapBlock*)unk1F4->allocHeap(JAIGlobalParameter::stayHeapMax
	                                          * sizeof(JAIHeapBlock));
	for (int i = 0; i < JAIGlobalParameter::stayHeapMax; ++i) {
		unk1F0[i].mLoadedFlag = 0;
		unk1F0[i].mLoadOrder  = 0;
		unk1F0[i].mSeqNumber  = -1;
		unk1F0[i].mUseOrder   = -1;
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
	mStreamUpdate->unk0  = 0;
	mStreamUpdate->unk1  = 0;
	mStreamUpdate->unk2  = 0;
	mStreamUpdate->unk4  = 1.0f;
	mStreamUpdate->unk8  = 1.0f;
	mStreamUpdate->unkC  = 0.5f;
	mStreamUpdate->unk10 = 0;
	mStreamUpdate->unk14 = nullptr;

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
			mFxlineConfig[i] = (JASystem::DSPInterface::
			                        FxlineConfig_*)((u8*)unk1F4->mFxSceneTable
			                                        + tmp->mSceneOffset[i]);
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
	if (!soundTable->unk78)
		size = JASystem::Dvd::checkFile(path);
	else
		size = soundTable->unk28;

	if (size == 0)
		return;

	if (!soundTable->unk78) {
		soundTable->unk78 = (u8*)unk1F4->allocHeap(size);
		JASystem::Dvd::loadFile(path, soundTable->unk78);
		soundTable->unk28 = size;
	}
	soundTable->unk0 = soundTable->unk78[3];
	// TODO: WTF???
	soundTable->unk2C = &path;

	// TODO: definitely fake, but a header struct doesn't work either
	for (u8 i = 0; i < 18; ++i) {
		soundTable->unk2[i]
		    = reinterpret_cast<u16*>(soundTable->unk78 + 6)[i * 2];
		u32 idx = reinterpret_cast<u16*>(soundTable->unk78 + 8)[i * 2];
		soundTable->unk30[i]
		    = &(reinterpret_cast<JAISoundInfo*>(soundTable->unk78 + 0x50)[idx]);
		if (i < 0x10 && soundTable->unk2[i] != 0) {
			soundTable->unk1 = i + 1;
		}
	}
}

void JAIData::reloadInfoDataWork(JAISoundTable* soundTable) { }

void JAIData::setInfoDataPointer(JAISoundTable* soundTable, u8* ptr) { }
