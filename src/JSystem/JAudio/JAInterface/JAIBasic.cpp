#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIData.hpp>
#include <JSystem/JAudio/JAInterface/JAIInter.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>
#include <JSystem/JAudio/JAInterface/JAISystemInterface.hpp>
#include <JSystem/JAudio/JASystem/JASTrackMgr.hpp>
#include <JSystem/JAudio/JASystem/JASDvdThread.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASAudioThread.hpp>
#include <JSystem/JAudio/JASystem/JASDriverIF.hpp>
#include <JSystem/JAudio/JASystem/JASVload.hpp>
#include <JSystem/JAudio/JASystem/JASWaveBankMgr.hpp>
#include <JSystem/JAudio/JASystem/JASWaveArcLoader.hpp>
#include <JSystem/JAudio/JASystem/JASBankMgr.hpp>
#include <dolphin/mtx.h>
#include <stdint.h>
#include <stdio.h>

#include <JSystem/JAudio/JAInterface/JAIConst.hpp>

JAIBasic* JAIBasic::basic;

JAICamera JAInullCamera(&JAIConst::camTrans, &JAIConst::camPreTrans,
                        JAIConst::camMtx);

JAIBasic::JAIBasic()
{
	basic                  = this;
	JAISound::interPointer = this;

	unk1C.mInitDataLoadOff       = false;
	unk1C.mFirstStayWaveLoaded   = false;
	unk1C.mSecondStayWaveLoaded  = false;
	unk1C.mSeqEntryCancel        = false;
	unk1C.mStreamEntryCancel     = false;
	unk1C.mStreamUseOff          = false;
	unk1C.mStreamInsideBufferCut = false;

	unk15               = 0;
	mSoundScene         = 0;
	unk11               = 0;
	unk24               = 0;
	mAudioCameras       = nullptr;
	mSeSequence         = nullptr;
	unk20               = 0;
	unk30               = 0;
	mInitFileLoadSwitch = 3;
	mInitDataPointer    = nullptr;
	mBankFileList       = nullptr;
	mWaveBankFileList   = nullptr;
	unk44               = 0;
	unk48               = 0;
	mBankList           = nullptr;
	mWaveBankList       = nullptr;
	mSeqArchiveHeader   = nullptr;
	mStreamListHeader   = nullptr;
	mSoundSceneList     = nullptr;
	mFxSceneTable       = nullptr;
	mFinishedSceneSet   = 0xffffffff;
	unk70               = 0;
	mInterfaceHeap      = nullptr;
	unk78               = nullptr;
}

void JAIBasic::startSeSequence()
{
	mSeSequence = nullptr;
	startSoundDirectID(0x80000800, &mSeSequence, nullptr, 1, 4);
}

void JAIBasic::initInterfaceMain()
{
	unk0 = (JAIData*)JASystem::Kernel::allocFromSysDram(sizeof(JAIData));
	unk0->init();
	unk0->unk1F4      = this;
	unk0->unk204.unk0 = this;
	unk0->unk1FC.unk0 = this;
	unk0->unk200.unk0 = this;
	setRegisterTrackCallback();
	JASystem::Driver::setMixerLevel(JAIGlobalParameter::inputGainDown,
	                                JAIGlobalParameter::outputGainUp);
	initHeap();

	char archivesPath[100];
	archivesPath[0] = 0;
	if (JAIGlobalParameter::audioResPath) {
		strcat(archivesPath, JAIGlobalParameter::audioResPath);

		{
			char* str = (char*)JASDram->alloc(
			    strlen(JAIGlobalParameter::audioResPath)
			        + strlen(JAIGlobalParameter::wavePath) + 1,
			    0);
			sprintf(str, "%s%s%c", JAIGlobalParameter::audioResPath,
			        JAIGlobalParameter::wavePath, 0);
			JAIGlobalParameter::wavePath = str;
		}

		{
			char* str2 = (char*)JASDram->alloc(
			    strlen(JAIGlobalParameter::audioResPath)
			        + strlen(JAIGlobalParameter::streamPath) + 1,
			    0);
			sprintf(str2, "%s%s%c", JAIGlobalParameter::audioResPath,
			        JAIGlobalParameter::streamPath, 0);
			JAIGlobalParameter::streamPath = str2;
		}
	}

	initReadFile();
	setWaveScene();
	initAllocParameter();
	initNullData();
	initSeqsLoadArea();
	if (!unk1C.mStreamUseOff)
		initStream();
	strcat(archivesPath, JAIGlobalParameter::sequenceArchivesPath);
	strcat(archivesPath, JAIGlobalParameter::sequenceArchivesFileName);
	if (!mSeqArchiveHeader) {
		JASystem::Vload::initHeader(archivesPath);
	} else {
		JASystem::Vload::initHeaderM(archivesPath, mSeqArchiveHeader->mData,
		                             nullptr);
	}
	JASystem::TrackMgr::initRegistTrack();
	mSeqArchiveHandle = JASystem::Vload::getArchiveHandle(
	    JAIGlobalParameter::sequenceArchivesFileName);
	startSeSequence();
	if (!unk1C.mInitDataLoadOff)
		checkEntriedSeq();
}

void JAIBasic::setCameraInfo(VecPtr pos, VecPtr prev_pos, MtxPtr view_mtx,
                             u32 id)
{
	if (JAIGlobalParameter::audioCameraMax <= id)
		return;

	mAudioCameras[id].mPosition     = pos;
	mAudioCameras[id].mPrevPosition = prev_pos;
	mAudioCameras[id].nViewMtx      = view_mtx;
}

void JAIBasic::initStream()
{
	JAInter::StreamLib::init(unk1C.mStreamInsideBufferCut);
}

void JAIBasic::setRegisterTrackCallback()
{
	JASystem::TrackMgr::registerTrackCallback(&JAIBasic::setParameterSeqSync);
}

void JAIBasic::initAudioThread(JKRSolidHeap* heap, u32 param1, u8 param2)
{
	JKRSolidHeap* rootHeap = heap;
	s32 uVar1              = 1;
	if (param2 & 1)
		uVar1 |= 2;

	JASystem::AudioThread::setPriority(
	    JAIGlobalParameter::audioSystemThreadPriority,
	    JAIGlobalParameter::audioDvdThreadPriority);
	JASystem::AudioThread::start(rootHeap, param1, uVar1);
	JASystem::TrackMgr::init(JAIGlobalParameter::systemTrackMax,
	                         JAIGlobalParameter::systemRootTrackMax);
	JASystem::TrackMgr::reset();
}

void JAIBasic::bootDSP() { }

void JAIBasic::initReadFile()
{
	switch (mInitFileLoadSwitch) {
	case 0:
		break;
	case 1:
		checkInitListFile();
		break;
	case 2:
		checkInitDataFile();
		break;
	case 3:
		if (!checkInitListFile())
			checkInitDataFile();
		break;
	case 4:
		if (mInitDataPointer)
			checkInitDataOnMemory();
		break;
	}

	initBankWave();
	if (mInitFileLoadSwitch != 4 && mInitDataPointer != 0)
		deleteTmpDVDFile(&mInitDataPointer);
}

struct FabricatedFileHeader {
	/* 0x0 */ u16 unk0;
	/* 0x2 */ u16 unk2;
	/* 0x4 */ u16 unk4;
	/* 0x6 */ u16 unk6;
	/* 0x8 */ u16 unk8;
	/* 0xA */ u16 unkA;
};

BOOL JAIBasic::checkInitListFile()
{
	if (!JAISystemInterface::checkFileExsistence("JaiInit.bld"))
		return 0;

	void* file                   = loadDVDFile("JaiInit.bld");
	FabricatedFileHeader* header = (FabricatedFileHeader*)file;

	if (file) {
		strcpy(JAIGlobalParameter::sequenceArchivesFileName,
		       (char*)((u8*)file + header->unk0));

		mBankFileList = (FabricatedFileNameEntry*)((u8*)file + header->unk2);
		mWaveBankFileList
		    = (FabricatedFileNameEntry*)((u8*)file + header->unk4);

		strcpy(JAIGlobalParameter::seInfoFileName,
		       (char*)((u8*)file + header->unk6));

		if (header->unk8) {
			strcpy(JAIGlobalParameter::seqInfoFileName,
			       (char*)((u8*)file + header->unk8));
			strcpy(JAIGlobalParameter::streamInfoFileName,
			       (char*)((u8*)file + header->unkA));
			unk0->mSeparateSoundTables = 1;
		} else {
			unk0->mSeparateSoundTables = 0;
		}

		return 1;
	} else {
		return 0;
	}
}

BOOL JAIBasic::checkInitDataFile()
{
	if (!JAISystemInterface::checkFileExsistence(
	        JAIGlobalParameter::initDataFileName))
		return 0;

	loadTmpDVDFile(JAIGlobalParameter::initDataFileName, &mInitDataPointer);
	if (mInitDataPointer) {
		checkInitDataOnMemory();
		return 1;
	} else {
		return 0;
	}
}

// fabricated
struct JAIInitDataBlob {
	/* 0x0 */ u32 offset;
	/* 0x4 */ u32 size;
	/* 0x8 */ u32 unk8;
};

// fabricated
enum JAIInitDataCommand {
	JAIINITDATA_End              = 0,
	JAIINITDATA_SoundTables      = 1,
	JAIINITDATA_BankList         = 2,
	JAIINITDATA_WaveBankList     = 3,
	JAIINITDATA_SeqArchiveHeader = 4,
	JAIINITDATA_StreamList       = 5,
	JAIINITDATA_SoundSceneList   = 6,
	JAIINITDATA_FxSceneTable     = 7,
	JAIINITDATA_Unk78            = 8,
};

void JAIBasic::checkInitDataOnMemory()
{
	u32 i              = 0;
	u32 shouldContinue = 1;
	JAIData* data      = unk0;
	u8 j;
	u32 n;
	u32 size;

	while (shouldContinue) {
		switch (((u32*)mInitDataPointer)[i++]) {
		case JAIINITDATA_End:
			shouldContinue = 0;
			break;

		case JAIINITDATA_SoundTables:
			if (((u32*)mInitDataPointer)[i + 2] != 0) {
				u8* buffer = mInitDataPointer + ((u32*)mInitDataPointer)[i++];
				data->mSeTable.mDataSize = ((u32*)mInitDataPointer)[i++];
				data->mSeTable.mData
				    = (u8*)transInitDataFile(buffer, data->mSeTable.mDataSize);

				buffer = mInitDataPointer + ((u32*)mInitDataPointer)[i++];
				data->mSeqTable.mDataSize = ((u32*)mInitDataPointer)[i++];
				data->mSeqTable.mData
				    = (u8*)transInitDataFile(buffer, data->mSeqTable.mDataSize);

				data->mStreamTable.mData
				    = mInitDataPointer + ((u32*)mInitDataPointer)[i++];
				data->mStreamTable.mDataSize = ((u32*)mInitDataPointer)[i++];
				data->mStreamTable.mData     = (u8*)transInitDataFile(
                    buffer, data->mStreamTable.mDataSize);

				data->mSeparateSoundTables = 1;
			} else {
				u8* buffer = mInitDataPointer + ((u32*)mInitDataPointer)[i++];
				data->mSeTable.mDataSize = ((u32*)mInitDataPointer)[i++];
				data->mSeTable.mData
				    = (u8*)transInitDataFile(buffer, data->mSeTable.mDataSize);
				++i;
				data->mSeparateSoundTables = 0;
			}
			break;

		case JAIINITDATA_BankList: {
			u8* buffer = (u8*)&((u32*)mInitDataPointer)[i];
			j          = 0;
			n          = 0;
			while (((u32*)mInitDataPointer)[i + n] != 0)
				n += 3;

			size      = (n / 3) * sizeof(FabricatedBankEntry) + 4;
			mBankList = (FabricatedBankEntry*)transInitDataFile(buffer, size);

			while (((u32*)mInitDataPointer)[i] != 0) {
				mBankList[j].mBankData
				    = (void*)(mInitDataPointer
				              + (uintptr_t)mBankList[j].mBankData);
				++j;
				i += 3;
			}
			++i;
			break;
		}

		case JAIINITDATA_WaveBankList: {
			j          = 0;
			n          = 0;
			u8* buffer = (u8*)&((u32*)mInitDataPointer)[i];
			while (((u32*)mInitDataPointer)[i + n] != 0)
				n += 3;

			size = (n / 3) * sizeof(FabricatedWaveBankEntry) + 4;
			mWaveBankList
			    = (FabricatedWaveBankEntry*)transInitDataFile(buffer, size);

			while (((u32*)mInitDataPointer)[i] != 0) {
				mWaveBankList[j].mWaveBankData
				    = (void*)(mInitDataPointer
				              + (uintptr_t)mWaveBankList[j].mWaveBankData);
				++j;
				i += 3;
			}
			mWaveGroupNumber = (s32*)allocHeap(j * sizeof(*mWaveGroupNumber));
			mWaveLoadStatus  = (s32*)allocHeap(j * sizeof(*mWaveLoadStatus));
			++i;
			break;
		}

		case JAIINITDATA_SeqArchiveHeader: {
			u8* buffer        = (u8*)&((u32*)mInitDataPointer)[i];
			mSeqArchiveHeader = (FabricatedSeqArchiveHeader*)transInitDataFile(
			    buffer, sizeof(FabricatedSeqArchiveHeader));
			u8* dataBuffer = mInitDataPointer + ((u32*)mInitDataPointer)[i];
			mSeqArchiveHeader->mData = (u8*)transInitDataFile(
			    dataBuffer, ((u32*)mInitDataPointer)[i + 1]);
			i += 3;
			break;
		}

		case JAIINITDATA_StreamList: {
			u8* buffer        = (u8*)&((u32*)mInitDataPointer)[i];
			mStreamListHeader = (FabricatedStreamListHeader*)transInitDataFile(
			    buffer, sizeof(FabricatedStreamListHeader));
			u8* dataBuffer = mInitDataPointer + ((u32*)mInitDataPointer)[i];
			mStreamListHeader->mData
			    = (JAIData::FabricatedStreamEntry*)transInitDataFile(
			        dataBuffer, ((u32*)mInitDataPointer)[i + 1]);
			i += 3;
			unk0->mStreamList = mStreamListHeader->mData;
			break;
		}

		case JAIINITDATA_SoundSceneList: {
			JAIInitDataBlob* blob
			    = (JAIInitDataBlob*)&((u32*)mInitDataPointer)[i];
			u8* dataBuffer = mInitDataPointer + blob->offset;
			FabricatedSoundSceneTable* table
			    = (FabricatedSoundSceneTable*)transInitDataFile(dataBuffer,
			                                                    blob->size);
			JAIGlobalParameter::soundSceneMax = table->mSceneMax;
			mSoundSceneList                   = table->mSceneData;
			for (u32 scene = 0; scene < JAIGlobalParameter::soundSceneMax;
			     ++scene)
				((u8**)mSoundSceneList)[scene] += (uintptr_t)table;
			i += 3;
			break;
		}

		case JAIINITDATA_FxSceneTable: {
			JAIInitDataBlob* blob
			    = (JAIInitDataBlob*)&((u32*)mInitDataPointer)[i];
			u8* dataBuffer = mInitDataPointer + blob->offset;
			mFxSceneTable  = (FabricatedFxSceneTable*)transInitDataFile(
                dataBuffer, blob->size);
			i += 3;
			break;
		}

		case JAIINITDATA_Unk78: {
			JAIInitDataBlob* blob
			    = (JAIInitDataBlob*)&((u32*)mInitDataPointer)[i];
			u8* dataBuffer = mInitDataPointer + blob->offset;
			unk78          = (u8*)transInitDataFile(dataBuffer,
			                                        (blob->size & 0xFFF0) + 0x10);
			i += 3;
			break;
		}

		default:
			while (((u32*)mInitDataPointer)[i++] != 0)
				;
			break;
		}
	}
}

void* JAIBasic::transInitDataFile(u8* buffer, u32 size)
{
	u8* data = (u8*)allocHeap(size);
	if (data != nullptr)
		for (u32 i = 0; i < size; ++i)
			data[i] = buffer[i];

	return data;
}

void JAIBasic::initBankWave()
{
	JASystem::WaveArcLoader::setCurrentDir(JAIGlobalParameter::wavePath);
	JASystem::WaveBankMgr::init(0x100);
	JASystem::WaveArcLoader::init();

	if (mWaveBankList) {
		for (int i = 0; mWaveBankList[i].mWaveBankData; ++i) {
			void* data = mWaveBankList[i].mWaveBankData;
			if (data) {
				JASystem::WaveBankMgr::registWaveBankWS(i, data);
				mWaveGroupNumber[i] = -1;
				mWaveLoadStatus[i]  = WAVE_LOAD_STATUS_NOT_LOADED;
			}
		}
	}

	if (mWaveBankFileList) {
		for (int i = 0; mWaveBankFileList[i].mFileName[0] != 0; ++i) {
			void* data = loadDVDFile(mWaveBankFileList[i].mFileName);
			if (data)
				JASystem::WaveBankMgr::registWaveBankWS(i, data);
		}
	}

	JASystem::BankMgr::init(0x100);

	if (mBankList) {
		int i;

		for (i = 0; mBankList[i].mBankData; ++i) {
			void* data = mBankList[i].mBankData;
			if (data)
				JASystem::BankMgr::registBankBNK(i, data);
		}

		for (i = 0; mBankList[i].mBankData; ++i)
			JASystem::BankMgr::assignWaveBank(i, mBankList[i].mWaveBankNumber);
	}

	if (mBankFileList) {
		for (int i = 0; mBankFileList[i].mFileName[0] != 0; ++i) {
			void* file = loadDVDFile(mBankFileList[i].mFileName);
			if (file)
				JASystem::BankMgr::registBankBNK(i, file);
			loadGroupWave(i, 0);
		}
	}
}

void JAIBasic::setWaveScene()
{
	if (mWaveBankList && !unk1C.mInitDataLoadOff) {
		for (int i = 0; mWaveBankList[i].mWaveBankData; ++i)
			if (mWaveBankList[i].mLoadTiming == WAVE_LOAD_TIMING_FIRST_STAY)
				loadGroupWave(i, 0);

		unk1C.mFirstStayWaveLoaded = true;
	}
}

void JAIBasic::readInitSoundData() { }

void JAIBasic::loadFirstStayWave()
{
	if (mWaveBankList && !unk1C.mInitDataLoadOff) {
		for (int i = 0; mWaveBankList[i].mWaveBankData; ++i)
			if (mWaveBankList[i].mLoadTiming == WAVE_LOAD_TIMING_FIRST_STAY)
				loadGroupWave(i, 0);

		unk1C.mFirstStayWaveLoaded = true;
	}
}

void JAIBasic::loadSecondStayWave()
{
	if (mWaveBankList) {
		for (int i = 0; mWaveBankList[i].mWaveBankData; ++i)
			if (mWaveBankList[i].mLoadTiming == WAVE_LOAD_TIMING_SECOND_STAY)
				loadGroupWave(i, 0);

		unk1C.mSecondStayWaveLoaded = true;
	}
}

void JAIBasic::setSceneSetFinishCallback(s32 bank_id, s32 param2)
{
	u32 id                   = (bank_id << 16) + param2;
	mFinishedSceneSet        = 0xffffffff;
	mWaveLoadStatus[bank_id] = WAVE_LOAD_STATUS_LOADING;
	JASystem::Dvd::checkPassDvdT(id, nullptr, &finishSceneSet);
}

void JAIBasic::finishSceneSet(u32 id)
{
	basic->mFinishedSceneSet         = id;
	basic->mWaveLoadStatus[id >> 16] = WAVE_LOAD_STATUS_LOADED;
}

void JAIBasic::loadSceneWave(s32 bank_id, s32 group_no)
{
	if (mWaveBankList) {
		// Won't match any other way :/
		uintptr_t entry = bank_id * sizeof(FabricatedWaveBankEntry);
		entry           = (uintptr_t)mWaveBankList + entry;
		if (((FabricatedWaveBankEntry*)entry)->mLoadTiming
		    == WAVE_LOAD_TIMING_SCENE) {
			s32 current = mWaveGroupNumber[bank_id];
			if (group_no != current) {
				if (current != -1)
					JASystem::WaveBankMgr::eraseWave(bank_id,
					                                 mWaveGroupNumber[bank_id]);
				loadGroupWave(bank_id, group_no);
			}
		}
	}
}

bool JAIBasic::checkSceneWaveOnMemory(s32 bank_id, s32 group_no)
{
	if (group_no == mWaveGroupNumber[bank_id]
	    && mWaveLoadStatus[bank_id] == WAVE_LOAD_STATUS_LOADED)
		return true;
	return false;
}

void JAIBasic::loadGroupWave(s32 bank_id, s32 group_no)
{
	JASystem::WaveBankMgr::loadWave(bank_id, group_no);
	setSceneSetFinishCallback(bank_id, group_no);
	mWaveGroupNumber[bank_id] = group_no;
}

s32 JAIBasic::getWaveGroupNumber(s32 bank_id)
{
	return mWaveGroupNumber[bank_id];
}

s32 JAIBasic::getWaveLoadStatus(s32 bank_id)
{
	return mWaveLoadStatus[bank_id];
}

void JAIBasic::checkAllWaveLoadStatus() { }

void JAIBasic::initNullData()
{
	JAInullCamera.mPosition->x = 0.0f;
	JAInullCamera.mPosition->y = 0.0f;
	JAInullCamera.mPosition->z = -50.0f;

	JAInullCamera.mPrevPosition->x = 0.0f;
	JAInullCamera.mPrevPosition->y = 0.0f;
	JAInullCamera.mPrevPosition->z = -50.0f;

	Vec up;
	up.x       = 0.0f;
	up.y       = 1.0f;
	up.z       = 0.0f;
	Vec target = JAIConst::dummyZeroVec;
	C_MTXLookAt(JAIConst::camMtx, JAInullCamera.mPosition, &up, &target);
	for (int i = 0; i < JAIGlobalParameter::audioCameraMax; ++i)
		setCameraInfo(JAInullCamera.mPosition, JAInullCamera.mPrevPosition,
		              JAIConst::camMtx, i);
}

void JAIBasic::initDriver(JKRSolidHeap* heap, u32 param_2, u8 param_3)
{
	initAudioThread(heap, param_2, param_3);
}

void JAIBasic::initInterface(u8 param)
{
	unk12 = param;
	initInterfaceMain();
}

void JAIBasic::initAllocParameter()
{
	unk0->initData();
	mSeCancelSwitch = (u8*)allocHeap(JAIGlobalParameter::getParamSeCategoryMax()
	                                 * sizeof(*mSeCancelSwitch));
	mSeCategoryVolume
	    = (f32*)allocHeap(JAIGlobalParameter::getParamSeCategoryMax()
	                      * sizeof(*mSeCategoryVolume));
	mAudioCameras = (JAICamera*)allocHeap(JAIGlobalParameter::audioCameraMax
	                                      * sizeof(JAICamera));
	for (int i = 0; i < JAIGlobalParameter::getParamSeCategoryMax(); ++i) {
		mSeCancelSwitch[i]   = 0;
		mSeCategoryVolume[i] = 1.0f;
	}
}

void JAIBasic::initSeqsLoadArea()
{
	for (int i = 0; i < JAIGlobalParameter::autoHeapMax; ++i) {
		unk0->unk1EC[i].mPointer
		    = allocHeap(JAIGlobalParameter::autoHeapRoomSize);
	}
	unk0->unk1F0->mPointer = allocHeap(JAIGlobalParameter::stayHeapSize);
}

void JAIBasic::setInitFileLoadSwitch(u8 flag) { mInitFileLoadSwitch = flag; }

void JAIBasic::startFrameInterfaceWork() { processFrameWork(); }

void JAIBasic::processFrameWork()
{
	checkDummyPositionBuffer();

	if (mSeSequence->mState == SOUNDSTATE_Started
	    && !unk1C.mSecondStayWaveLoaded)
		loadSecondStayWave();

	if (mSeSequence->mState >= SOUNDSTATE_Playing)
		checkNextFrameSe();

	sendPlayingSeCommand();
	checkEntriedSeq();
	checkFadeoutSeq();
	checkStoppedSeq();
	checkSeMovePara();
	checkPlayingSeq();
	checkStartedSeq();
	checkReadSeq();
	checkSeqWave();
	if (!unk1C.mStreamUseOff)
		checkStream();
	++unk20;
}

void JAIBasic::checkStream()
{
	checkPlayingStream();
	checkRequestStream();
	checkWaitStream();
	checkEntriedStream();
}

void JAIBasic::checkDummyPositionBuffer()
{
	JAIDummyVec* it = unk0->mDummyVecUsedHead;
	while (it) {
		JAIDummyVec* next = it->mNext;
		--it->mLifeTime;
		if (it->mLifeTime == 0 || !it->mSound) {
			if (it->mSound)
				it->mSound->stop(0);
			releaseDummyVecPointer(it);
		}
		it = next;
	}
}

void JAIBasic::startSoundVec(u32 id, JAISound** sound, Vec* pos, u32 param1,
                             u32 param2, u8 param3)
{
}

void JAIBasic::startSoundVecReturnHandle(u32 id, Vec* pos, u32 param1,
                                         u32 param2, u8 param3)
{
}

void JAIBasic::startSoundActor(u32 id, JAISound** sound, JAIActor* actor,
                               u32 param, u8 flag)
{
	u32 format = getInfoFormat(getInfoPointerFromID(id), id);
	if (format & 1)
		startSoundIndirectID(id, sound, actor, param, flag);
	else
		startSoundDirectID(id, sound, actor, param, flag);
}

JAISound* JAIBasic::startSoundActorReturnHandle(u32 id, JAIActor* actor,
                                                u32 param, u8 flag)
{
	JAISound* sound = nullptr;
	startSoundActor(id, &sound, actor, param, flag);
	JAISound* result = sound;
	if (result)
		sound->release();
	return result;
}

void JAIBasic::startSoundDirectID(u32 id, JAISound** sound, JAIActor* actor,
                                  u32 param, u8 flag)
{
	void* ptr;
	unk0->getInfoPointer(id, &ptr);
	if (ptr)
		startSoundBasic(id, sound, actor, param, flag, ptr);
}

void JAIBasic::startSoundIndirectID(u32 id, JAISound** sound, JAIActor* actor,
                                    u32 param, u8 flag)
{
	void* ptr;
	unk0->getInfoPointer(id, &ptr);
	if (ptr)
		startSoundBasic((id & 0xFFFFFC00) + ((JAISoundInfo*)ptr)->mOffsetNo,
		                sound, actor, param, flag, ptr);
}

void JAIBasic::startSoundBasic(u32 id, JAISound** sound, JAIActor* actor,
                               u32 param, u8 flag, void* data)
{
	switch (id & JAISoundID_TypeMask) {
	case JAISoundID_Type_Sequence:
		if (unk1C.mSeqEntryCancel != true
		    && (mSeSequence == nullptr
		        || (mSeSequence->mSoundID & JAISoundID_IndexMask)
		               != (id & JAISoundID_IndexMask))) {
			if (sound == nullptr) {
				u8 num = getSeqTrackNumber(data);
				sound  = &unk0->mDefaultSeqHandle[num];
			}

			unk0->unk1FC.storeBuffer(sound, actor, id, param, flag, data);
		}
		break;

	case JAISoundID_Type_Se:
		if (mSeCancelSwitch[id >> 12] == 0) {
			unk0->unk200.storeBuffer(sound, actor, id, param, flag, data);
		} else if (sound != nullptr) {
			*sound = nullptr;
		}
		break;

	case JAISoundID_Type_Stream:
		if (unk1C.mStreamUseOff == false && unk1C.mStreamEntryCancel != true)
			unk0->unk204.storeBuffer(sound, actor, id, param, flag, data);
		break;
	}
}

void JAIBasic::getPlayingSoundHandle(JAISound** sound, u32 param) { }

#pragma dont_inline on
void JAIBasic::stopSoundHandle(JAISound* sound, u32 param)
{
	if (sound) {
		switch (sound->mSoundID & JAISoundID_TypeMask) {
		case JAISoundID_Type_Sequence:
			if (sound->mState < SOUNDSTATE_Playing || param == 0) {
				if (sound->mState >= SOUNDSTATE_Started)
					JAISystemInterface::stopSeq(
					    sound->getSeqParameter()->mSeqHandle);
				else if (sound->mState >= SOUNDSTATE_Stored)
					unk0->releaseAutoHeapPointer(
					    sound->getSeqParameter()->mAutoHeapPosition);

				sound->clearMainSoundPPointer();
				stopSeq(sound);
			} else {
				unk0->mSeqTrackInfo[sound->mTrack].unk8 |= 0x2;
				unk0->mSeqTrackInfo[sound->mTrack].mSound->mFadeCounter = param;
			}
			break;

		case JAISoundID_Type_Se:
			if (sound->mState) {
				if (param == 0 || sound->mState == SOUNDSTATE_Stored) {
					releaseSeRegist(sound);
				} else {
					sound->mFadeCounter = param;
					sound->setSeInterVolume(6, 0.0f, param, 0);
				}
			}
			break;

		case JAISoundID_Type_Stream:
			if (param == 0) {
				JAInter::StreamLib::stop();
				sound->mState               = SOUNDSTATE_Inactive;
				unk0->mStreamUpdate->mSound = nullptr;
				sound->clearMainSoundPPointer();
				releaseStreamParameterPointer(
				    (JAIStreamParameter*)sound->mCustomParameter);
				releaseControllerHandle(&unk0->mStreamControlBuffer, sound);
				JASystem::Dvd::unpauseDvdT();
			} else {
				if (sound->getStreamParameter()->mUpdateData != nullptr) {
					sound->getStreamParameter()->mUpdateData->mActiveTrackFlag
					    |= 0x2;
					sound->mFadeCounter = param;
				}
			}
			break;
		}
	}
}
#pragma dont_inline off

u32 JAIBasic::changeIDToCategory(u32 id) { return id >> 0xc & 0xff; }

void JAIBasic::stopPlayingObjectSe(void* obj) { }

void JAIBasic::stopPlayingIDObjectSe(u32 id, void* obj) { }

void JAIBasic::stopPlayingCategorySe(u8 category) { }

void JAIBasic::stopPlayingCategoryObjectSe(u8 category, void* obj) { }

void JAIBasic::stopAllSe(void* obj) { }

void JAIBasic::stopAllSe(u8 param)
{
	JAISound* sound = unk0->mSeRegist[param].mUsedHead;
	while (sound) {
		JAISound* next = sound->mNextSound;
		stopSoundHandle(sound, 0);
		sound = next;
	}
}

void JAIBasic::stopAllSe(u8 param, void* obj) { }

void JAIBasic::stopAllSeq(void* obj) { }

void JAIBasic::stopAllStream(void* obj) { }

void JAIBasic::stopActorSoundOneBuffer(void* actor, JAISound* sound) { }

void JAIBasic::stopIDSoundOneBuffer(u32 id, JAISound* sound) { }

void JAIBasic::stopIDActorSoundOneBuffer(u32 id, void* actor, JAISound* sound)
{
}

void JAIBasic::stopAllSound(void* obj) { }

void JAIBasic::getPlayingSoundLinkHeadPointer(u32 param) { }

void JAIBasic::stopAllSound(u32 param) { }

void JAIBasic::stopAllSound(u32 param, void* obj) { }

void JAIBasic::deleteObject(void* obj) { }

void JAIBasic::releaseSoundHandle(JAISound* sound) { }

JAISound* JAIBasic::getControllerHandle(JAILinkBuffer* buffer)
{
	JAISound** var2 = &buffer->mUsedHead;
	if (buffer->mFreeHead) {
		JAISound* result  = buffer->mFreeHead;
		buffer->mFreeHead = result->mNextSound;
		if (*var2) {
			result->mNextSound  = *var2;
			(*var2)->mPrevSound = result;
		} else {
			result->mNextSound = nullptr;
		}
		result->mPrevSound = nullptr;
		*var2              = result;
		result->setCustomParameterPointer(nullptr);
		return result;
	}
	return nullptr;
}

void JAIBasic::releaseControllerHandle(JAILinkBuffer* buffer, JAISound* sound)
{
	JAISound** ptr = &buffer->mUsedHead;

	sound->setCustomParameterPointer(nullptr);
	sound->mMainSoundPPointer = nullptr;
	if (buffer->mUsedHead != sound) {
		sound->mPrevSound->mNextSound = sound->mNextSound;
		if (sound->mNextSound)
			sound->mNextSound->mPrevSound = sound->mPrevSound;
	} else {
		*ptr = sound->mNextSound;
		if (sound->mNextSound)
			sound->mNextSound->mPrevSound = nullptr;
	}
	sound->mNextSound = buffer->mFreeHead;
	if (sound->mNextSound)
		sound->mNextSound->mPrevSound = sound;
	buffer->mFreeHead = sound;
}

JAIStreamParameter* JAIBasic::getStreamParameter()
{
	JAIStreamParameter** free = &unk0->mStreamParameterFreeHead;
	JAIStreamParameter** used = &unk0->mStreamParameterUsedHead;

	if ((*free)->mNext != nullptr) {
		JAIStreamParameter* result = *free;

		*free = result->mNext;
		if (*used != nullptr) {
			result->mNext  = *used;
			(*used)->mPrev = result;
		} else {
			result->mNext = nullptr;
		}
		result->mPrev = nullptr;
		*used         = result;
		return result;
	}
	return nullptr;
}

void JAIBasic::releaseStreamParameterPointer(JAIStreamParameter* param)
{
	JAIStreamParameter** used = &unk0->mStreamParameterUsedHead;
	JAIStreamParameter** free = &unk0->mStreamParameterFreeHead;

	if (*used != param) {
		param->mPrev->mNext = param->mNext;
		if (param->mNext)
			param->mNext->mPrev = param->mPrev;
	} else {
		*used = param->mNext;
		if (param->mNext)
			param->mNext->mPrev = nullptr;
	}
	param->mNext = *free;
	if (param->mNext)
		param->mNext->mPrev = param;
	*free = param;
}

JAISeqParameter* JAIBasic::getSeqParametermeterPointer()
{
	JAISeqParameter** free = &unk0->mSeqParameterFreeHead;
	JAISeqParameter** used = &unk0->mSeqParameterUsedHead;

	if ((*free)->mNext) {
		JAISeqParameter* result = *free;

		*free = result->mNext;
		if (*used) {
			result->mNext  = *used;
			(*used)->mPrev = result;
		} else {
			result->mNext = nullptr;
		}
		result->mPrev = nullptr;
		*used         = result;
		return result;
	}
	return nullptr;
}

void JAIBasic::releaseSeqParameterPointer(JAISeqParameter* param)
{
	JAISeqParameter** used = &unk0->mSeqParameterUsedHead;
	JAISeqParameter** free = &unk0->mSeqParameterFreeHead;

	if (*used != param) {
		param->mPrev->mNext = param->mNext;
		if (param->mNext)
			param->mNext->mPrev = param->mPrev;
	} else {
		*used = param->mNext;
		if (param->mNext)
			param->mNext->mPrev = nullptr;
	}
	param->mNext = *free;
	if (param->mNext)
		param->mNext->mPrev = param;
	*free = param;
}

JAISeParameter* JAIBasic::getSeParametermeterPointer()
{
	JAISeParameter* result;
	JAISeParameter** free = &unk0->mSeParameterFreeHead;
	JAISeParameter** used = &unk0->mSeParameterUsedHead;

	if (*free) {
		result = *free;
		*free  = result->mNext;
		if (*used != nullptr) {
			result->mNext  = *used;
			(*used)->mPrev = result;
		} else {
			result->mNext = nullptr;
		}
		result->mPrev = nullptr;
		*used         = result;
		unk0->initSePara(result);
	} else {
		result = nullptr;
	}
	return result;
}

void JAIBasic::releaseSeParameterPointer(JAISeParameter* param)
{
	if (!param)
		return;

	JAISeParameter** used = &unk0->mSeParameterUsedHead;
	JAISeParameter** free = &unk0->mSeParameterFreeHead;

	if (*used != param) {
		param->mPrev->mNext = param->mNext;
		if (param->mNext)
			param->mNext->mPrev = param->mPrev;
	} else {
		*used = param->mNext;
		if (param->mNext)
			param->mNext->mPrev = nullptr;
	}
	param->mNext = *free;
	if (param->mNext)
		param->mNext->mPrev = param;
	*free = param;
}

void JAIBasic::getDummyVecPointer() { }

void JAIBasic::releaseDummyVecPointer(JAIDummyVec* vec)
{
	JAIDummyVec** used = &unk0->mDummyVecUsedHead;
	JAIDummyVec** free = &unk0->mDummyVecFreeHead;

	if (*used != vec) {
		vec->mPrev->mNext = vec->mNext;
		if (vec->mNext)
			vec->mNext->mPrev = vec->mPrev;
	} else {
		*used = vec->mNext;
		if (vec->mNext)
			vec->mNext->mPrev = nullptr;
	}
	vec->mNext = *free;
	if (vec->mNext)
		vec->mNext->mPrev = vec;
	*free = vec;
}

void JAIBasic::getGameFrameCounter() { }

void JAIBasic::setPauseFlagAll(u8 flag) { }

void JAIBasic::checkPlayingSoundTrack(u32 param) { }

void JAIBasic::changeSoundScene(u32 scene) { }

u16 JAIBasic::getMapInfoFxline(u32 param)
{
	if (!param)
		return 0;
	else
		return 1;
}

u32 JAIBasic::getMapInfoGround(u32 param)
{
	if (!param)
		return 0;
	else
		return 1;
}

f32 JAIBasic::getMapInfoFxParameter(u32 param)
{
	if (!param)
		return 0.0f;
	else
		return 1.0f;
}

void JAIBasic::allocDvdBuffer(u8* buffer, u32 param1, u32 param2) { }

void JAIBasic::deallocDvdBuffer(u8* buffer) { }

void JAIBasic::getSeInfoMode() { }

void JAIBasic::getSeInfoStartPointer() { }

void JAIBasic::getSeInfoCategoryMax() { }

JAISoundTable* JAIBasic::getInfoPointerFromID(u32 id)
{
	static JAISoundTable* _dinfo;
	if (unk0->mSeparateSoundTables == 0) {
		_dinfo = &unk0->mSeTable;
	} else {
		switch (id & JAISoundID_TypeMask) {
		case JAISoundID_Type_Se:
			_dinfo = &unk0->mSeTable;
			break;
		case JAISoundID_Type_Sequence:
			_dinfo = &unk0->mSeqTable;
			break;
		case JAISoundID_Type_Stream:
			_dinfo = &unk0->mStreamTable;
			break;
		}
	}
	return _dinfo;
}

u32 JAIBasic::getInfoFormat(JAISoundTable* table, u32 id)
{
	u32 result = 0;
	switch (id & JAISoundID_TypeMask) {
	case JAISoundID_Type_Se:
		result = ((u8*)table->mData)[0];
		break;
	case JAISoundID_Type_Sequence:
		result = ((u8*)table->mData)[1];
		break;
	case JAISoundID_Type_Stream:
		result = ((u8*)table->mData)[2];
		break;
	}
	return result;
}

void JAIBasic::setSeCancelSwitch(u8 param1, u8 param2) { }

void JAIBasic::setSeCategoryVolume(u8 category, u8 volume)
{
	mSeCategoryVolume[category] = volume / 127.0f;
}

u16 JAIBasic::setParameterSeqSync(JASystem::TTrack* param_1, u16 param_2)
{
	u16 result = 0;

	switch (param_2) {
	case 0:
		for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
			if (basic->unk0->mSeqTrackInfo[i].mSound == nullptr)
				continue;

			JASystem::TTrack* track
			    = JASystem::TrackMgr::handleToSeq(basic->unk0->mSeqTrackInfo[i]
			                                          .mSound->getSeqParameter()
			                                          ->mSeqHandle);
			if (track != param_1->getParent())
				continue;

			u32 uVar8 = param_1->unk308;
			u32 route = basic->routeToTrack(uVar8);

			JAISystemInterface::outerInit(
			    &basic->unk0->mSeqTrackInfo[i], param_1, route,
			    basic->getSoundInfoFromID(
			             basic->unk0->mSeqTrackInfo[i].mSound->mSoundID)
			            ->mSwBit
			        >> 8,
			    param_2 & 1);
			result = 0;
			basic->unk0->mSeqTrackInfo[i].unk4 |= 1 << route;
			i = JAIGlobalParameter::seqPlayTrackMax;
		}
		break;
	case 1: {
		u8 index                             = param_1->unk308;
		JASystem::TTrack::TOuterParam* outer = param_1->getOuterParam();
		JAIData::FabricatedSeTrackParameter* seTrackUpdate = basic->unk0->unk0;

		outer->setParam(JASystem::TTrack::UPDATE_Volume,
		                seTrackUpdate[index].mVolume);
		outer->setParam(JASystem::TTrack::UPDATE_Pan,
		                seTrackUpdate[index].mPan);
		outer->setParam(JASystem::TTrack::UPDATE_Pitch,
		                seTrackUpdate[index].mPitch);
		outer->setParam(JASystem::TTrack::UPDATE_Fxmix,
		                seTrackUpdate[index].mFxmix);
		f32 thing;
		if (basic->mSoundOutputMode != 2)
			thing = 0.0f;
		else
			thing = seTrackUpdate[index].mDolby;
		outer->setParam(JASystem::TTrack::UPDATE_Dolby, thing);
		break;
	}
	case 0x7F:
		param_1->writePortApp(0, basic->mSoundScene);
		break;
	}
	return result;
}

JAISoundInfo* JAIBasic::getSoundInfoFromID(u32 id)
{
	static JAISoundInfo* _info;
	unk0->getInfoPointer(id, (void**)&_info);
	return _info;
}

u8 JAIBasic::getSeqTrackNumber(void* param)
{
	return ((JAISoundInfo*)param)->mTrackNumber;
}

u8 JAIBasic::getSoundPrioity(void* param)
{
	return ((JAISoundInfo*)param)->mPriority;
}

u32 JAIBasic::getSoundSwBit(void* param)
{
	return ((JAISoundInfo*)param)->mSwBit;
}

void JAIBasic::setSeExtParameter(JAISound* sound)
{
	if (!sound)
		return;
	JAISoundTable* table = getInfoPointerFromID(sound->mSoundID);
	u8 format            = getInfoFormat(table, sound->mSoundID);

	if (format & 4)
		sound->setVolume(((JAISoundInfo*)sound->mInfo)->mVolume / 127.0f, 0, 1);
	if (format & 8)
		sound->setFxmix(((JAISoundInfo*)sound->mInfo)->mFxmix / 127.0f, 0, 1);
	if (format & 2)
		sound->setPitch(((JAISoundInfo*)sound->mInfo)->mPitch, 0, 1);
}

u32 JAIBasic::routeToTrack(u32 param)
{
	u32 uVar2 = 0;
	if ((param & 0xf0000000) == 0x00000000)
		return 0;
	if ((param & 0xf0000000) == 0x10000000)
		uVar2 = 0xf;
	else if ((param & 0xf0000000) == 0x20000000)
		uVar2 = 0xff;
	return param & uVar2;
}

void JAIBasic::initHeap()
{
	if (JAIGlobalParameter::interfaceHeapSize != 0) {
		mInterfaceHeap = JKRSolidHeap::create(
		    JAIGlobalParameter::interfaceHeapSize, JASDram, 0);
	}
}

void* JAIBasic::allocHeap(u32 size)
{
	void* result;
	if (JAIGlobalParameter::interfaceHeapSize)
		result = new (mInterfaceHeap, 0x20) u8[size];
	else
		result = JASystem::Kernel::allocFromSysDram(size);
	return result;
}

JAISound* JAIBasic::makeSound(u32 param)
{
	if (mInterfaceHeap)
		return new (mInterfaceHeap, 0) JAISound[param];
	else
		return new (JASDram, 0) JAISound[param];
}

void* JAIBasic::loadDVDFile(char* filename)
{
	// TODO: the frame is 8 bytes short here; every instruction matches.
	u32 size = JASystem::Dvd::checkFile(filename);
	if (!size) {
		return nullptr;
	} else {
		void* result = allocHeap(size);
		JASystem::Dvd::loadFile(filename, result);
		return result;
	}
}

void JAIBasic::loadTmpDVDFile(char* filename, u8** buffer)
{
	u32 size = JASystem::Dvd::checkFile(filename);
	if (!size) {
		*buffer = nullptr;
	} else {
		*buffer = (u8*)JASDram->alloc(size, ~0x1F);
		JASystem::Dvd::loadFile(filename, *buffer);
	}
}

void JAIBasic::deleteTmpDVDFile(u8** buffer)
{
	if (*buffer)
		JASDram->freeTail();
}

void JAIBasic::allocStreamBuffer(void* buffer, s32 size) { }

void JAIBasic::deallocStreamBuffer() { }

int JAIBasic::loadArcSeqData(u32 param_1, bool param_2)
{
	u32 uVar1   = param_1 & JAISoundID_IndexMask;
	u32 uVar2   = JASystem::Vload::checkSize(uVar1);
	void* iVar3 = unk0->checkOnMemory(uVar1, nullptr);

	u32 uVar6 = getSoundInfoFromID(param_1)->mSwBit;

	u8* puVar4 = (u8*)iVar3;

	if (puVar4 == 0) {
		u8 unaff_r28;
		if ((uVar6 & 0x10) != 0) {
			puVar4    = unk0->getFreeStayHeapPointer(uVar2, uVar1);
			unaff_r28 = 0xff;
		}

		if ((puVar4 == nullptr) || ((uVar6 & 0x20) != 0)) {
			unaff_r28 = unk0->checkUsefulAutoHeapPosition();

			if (unaff_r28 >= JAIGlobalParameter::autoHeapMax)
				return 0xffffffff;

			if (uVar2 >= JAIGlobalParameter::autoHeapRoomSize)
				return 0xffffffff;

			puVar4 = (u8*)unk0->getFreeAutoHeapPointer(unaff_r28, uVar1);
		}

		if ((uVar6 & 0x40) == 0) {
			unk0->setAutoHeapLoadedFlag(unaff_r28, '\x01');
			u32 id = param_2 | 0xFE | uVar1 << 16 | unaff_r28 << 8;
			JASystem::Vload::loadFileAsync(mSeqArchiveHandle + uVar1, puVar4, 0,
			                               uVar2, &checkDvdLoadArc, id);
			return 1;
		} else {
			JASystem::Vload::loadFile(mSeqArchiveHandle + uVar1, puVar4, 0,
			                          uVar2);
			return 2;
		}
	}
	return 0;
}
