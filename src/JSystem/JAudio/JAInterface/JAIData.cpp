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
	unk88.unk78    = 0;
	unkC.unk78     = 0;
	unk104.unk78   = 0;
	unk1B0         = 0;
	mStreamList    = 0;
	mNextLoadOrder = 0;
	mStayHeapCount = 0;
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

	unk1BC = &unk1C4[0];
	unk1C0 = nullptr;

	unk1C4[0].unk1854 = nullptr;
	unk1C4[0].unk1858 = unk1C4 + 1;
	for (i = 1; i < JAIGlobalParameter::seqControlBufferMax - 1; ++i) {
		unk1C4[i].unk1854 = &unk1C4[i - 1];
		unk1C4[i].unk1858 = &unk1C4[i + 1];
	}
	unk1C4[i].unk1854 = &unk1C4[i - 1];
	unk1C4[i].unk1858 = nullptr;

	for (int i = 0; i < JAIGlobalParameter::seqControlBufferMax; ++i)
		initSeqParameter(&unk1C4[i]);
}

void JAIData::initDummyVecLink()
{
	int i;

	unk228 = (JAIDummyVec*)unk230;
	unk22C = nullptr;

	unk230[0].unk0 = nullptr;
	unk230[0].unk4 = &unk230[1];
	for (i = 1; i < JAIGlobalParameter::dummyPositionMax - 1; ++i) {
		unk230[i].unk0 = &unk230[i - 1];
		unk230[i].unk4 = &unk230[i + 1];
	}
	unk230[i].unk0 = &unk230[i - 1];
	unk230[i].unk4 = nullptr;
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

	unk1C8 = &unk1D0[0];
	unk1CC = nullptr;

	unk1D0[0].unk43C = nullptr;
	unk1D0[0].unk440 = &unk1D0[1];
	for (i = 1; i < JAIGlobalParameter::seRegistMax
	                        * JAIGlobalParameter::getParamSeCategoryMax()
	                    - 1;
	     ++i) {
		unk1D0[i].unk43C = &unk1D0[i - 1];
		unk1D0[i].unk440 = &unk1D0[i + 1];
	}
	unk1D0[i].unk43C = &unk1D0[i - 1];
	unk1D0[i].unk440 = nullptr;
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

	unk1D4 = &unk1DC[0];
	unk1D8 = nullptr;

	unk1DC[0].unk3D8 = nullptr;
	unk1DC[0].unk3DC = &unk1DC[1];
	for (i = 1; i < JAIGlobalParameter::streamParameterBufferMax - 1; ++i) {
		unk1DC[i].unk3D8 = &unk1DC[i - 1];
		unk1DC[i].unk3DC = &unk1DC[i + 1];
	}
	unk1DC[i].unk3D8 = &unk1DC[i - 1];
	unk1DC[i].unk3DC = nullptr;

	for (int i = 0; i < JAIGlobalParameter::streamParameterBufferMax; ++i)
		initStreamParameter(&unk1DC[i]);
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
	unk180[param].mSeqVolume = 1.0f;
	unk180[param].mSeqPan    = 0.5f;
	unk180[param].mSeqPitch  = 1.0f;
	unk180[param].mSeqFxmix  = 0.0f;
	unk180[param].mSeqDolby  = 0.0f;
	unk180[param].mSeqTempo  = 1.0f;
	for (int i = 0; i < JAIGlobalParameter::seqTrackMax; ++i) {
		unk180[param].mTrackVolume[i] = 1.0f;
		unk180[param].mTrackPan[i]    = 64.0f;
		unk180[param].mTrackPitch[i]  = 1.0f;
		unk180[param].mTrackFxmix[i]  = 0.0f;
		unk180[param].mTrackDolby[i]  = 0.0f;
		unk180[param].mTrackUpdate[i] = 0;
	}
}

void JAIData::initStreamUpdateParameter()
{
	unk184->unk0  = 0;
	unk184->unk1  = 0;
	unk184->unk2  = 0;
	unk184->unk4  = 1.0f;
	unk184->unk8  = 1.0f;
	unk184->unkC  = 0.5f;
	unk184->unk10 = 0;
	unk184->unk14 = nullptr;
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
	if (unk1B0 == 0) {
		table = &unk88;
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
			table = &unk88;
			JAIGlobalParameter::getParamSeCategoryMax();
			break;
		case JAISoundID_Type_Sequence:
			table = &unkC;
			thing = 0x10;
			break;
		case JAISoundID_Type_Stream:
			table = &unk104;
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
	initInfoDataWork(&unk88, JAIGlobalParameter::seInfoFileName);
	if (unk1B0 == 1) {
		initInfoDataWork(&unkC, JAIGlobalParameter::seqInfoFileName);
		initInfoDataWork(&unk104, JAIGlobalParameter::streamInfoFileName);
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
	unk230 = (JAIDummyVec*)unk1F4->allocHeap(
	    JAIGlobalParameter::dummyPositionMax * sizeof(JAIDummyVec));
	initDummyVecLink();

	unk1E8 = (JAILinkBuffer*)unk1F4->allocHeap(
	    JAIGlobalParameter::getParamSeCategoryMax() * sizeof(JAILinkBuffer));

	unk8 = (FabricatedUnk8Struct**)unk1F4->allocHeap(
	    JAIGlobalParameter::getParamSeCategoryMax()
	    * sizeof(FabricatedUnk8Struct*));

	unk1E4 = (JAISound**)unk1F4->allocHeap(
	    JAIGlobalParameter::getParamSeCategoryMax() * sizeof(JAISound*));

	for (int i = 0; i < JAIGlobalParameter::getParamSeCategoryMax(); ++i) {
		unk1E4[i]          = unk1F4->makeSound(JAIGlobalParameter::seRegistMax);
		unk1E8[i].mStorage = unk1E4[i];
		initLinkBuffer(&unk1E8[i], JAIGlobalParameter::seRegistMax);
		unk8[i] = (FabricatedUnk8Struct*)unk1F4->allocHeap(
		    JAIGlobalParameter::seRegistMax * sizeof(FabricatedUnk8Struct));
		for (int j = 0; j < JAIGlobalParameter::seRegistMax; ++j) {
			unk8[i][j].unk8 = nullptr;
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

	unk208 = unk1F4->makeSound(JAIGlobalParameter::seqControlBufferMax);
	unk20C = unk1F4->makeSound(JAIGlobalParameter::streamControlBufferMax);
	unk210.mStorage = unk208;
	initLinkBuffer(&unk210, JAIGlobalParameter::seqControlBufferMax);
	unk21C.mStorage = unk20C;
	initLinkBuffer(&unk21C, JAIGlobalParameter::streamControlBufferMax);
	unk1C4 = (JAISeqParameter*)unk1F4->allocHeap(
	    JAIGlobalParameter::seqControlBufferMax * sizeof(JAISeqParameter));
	initSeqParaLinkBuffer();
	unk1D0 = (JAISeParameter*)unk1F4->allocHeap(
	    JAIGlobalParameter::seRegistMax
	    * JAIGlobalParameter::getParamSeCategoryMax() * sizeof(JAISeParameter));
	initSeParaLinkBuffer();
	unk1DC = (JAIStreamParameter*)unk1F4->allocHeap(
	    JAIGlobalParameter::streamParameterBufferMax
	    * sizeof(JAIStreamParameter));
	initStreamParaLinkBuffer();
	for (int i = 0; i < JAIGlobalParameter::seRegistMax
	                        * JAIGlobalParameter::getParamSeCategoryMax();
	     ++i) {
		initSePara(&unk1D0[i]);
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

	unk1E0 = (JAISound**)unk1F4->allocHeap(JAIGlobalParameter::seqPlayTrackMax
	                                       * sizeof(JAISound*));
	unk180 = (JAISeqUpdateData*)unk1F4->allocHeap(
	    JAIGlobalParameter::seqPlayTrackMax * sizeof(JAISeqUpdateData));
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		unk180[i].unk4C
		    = (JAISeqUpdateData::FabricatedUnk4CStruct*)unk1F4->allocHeap(
		        0x7BC);
		unk1E0[i]        = 0;
		unk180[i].unk0   = 0;
		unk180[i].unk1   = 0;
		unk180[i].unk2   = 0;
		unk180[i].unk3   = 0;
		unk180[i].unk8   = 0;
		unk180[i].mSound = 0;

		unk180[i].mTrackVolume = (f32*)unk1F4->allocHeap(
		    JAIGlobalParameter::seqTrackMax * sizeof(f32));
		unk180[i].mTrackPan = (f32*)unk1F4->allocHeap(
		    JAIGlobalParameter::seqTrackMax * sizeof(f32));
		unk180[i].mTrackPitch = (f32*)unk1F4->allocHeap(
		    JAIGlobalParameter::seqTrackMax * sizeof(f32));
		unk180[i].mTrackFxmix = (f32*)unk1F4->allocHeap(
		    JAIGlobalParameter::seqTrackMax * sizeof(f32));
		unk180[i].mTrackDolby = (f32*)unk1F4->allocHeap(
		    JAIGlobalParameter::seqTrackMax * sizeof(f32));
		unk180[i].mTrackUpdate = (u32*)unk1F4->allocHeap(
		    (JAIGlobalParameter::seqTrackMax + 1) * sizeof(u32));
		initSeqTrackInfoParameter(i);
	}
	unk184 = (JAIStreamUpdateParameter*)unk1F4->allocHeap(
	    sizeof(JAIStreamUpdateParameter));
	unk184->unk0  = 0;
	unk184->unk1  = 0;
	unk184->unk2  = 0;
	unk184->unk4  = 1.0f;
	unk184->unk8  = 1.0f;
	unk184->unkC  = 0.5f;
	unk184->unk10 = 0;
	unk184->unk14 = nullptr;

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
