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
#include <stdio.h>

#include <JSystem/JAudio/JAInterface/JAIConst.hpp>

JAIBasic* JAIBasic::basic;

JAICamera JAInullCamera(&JAIConst::camTrans, &JAIConst::camPreTrans,
                        JAIConst::camMtx);

JAIBasic::JAIBasic()
{
	basic                  = this;
	JAISound::interPointer = this;

	unk1C.flag3 = 0;
	unk1C.flag1 = 0;
	unk1C.flag2 = 0;
	unk1C.flag4 = 0;
	unk1C.flag5 = 0;
	unk1C.flag6 = 0;
	unk1C.flag7 = 0;

	unk15 = 0;
	unk10 = 0;
	unk11 = 0;
	unk24 = 0;
	unk8  = nullptr;
	unk38 = nullptr;
	unk20 = 0;
	unk30 = 0;
	unk13 = 3;
	unk4C = nullptr;
	unk3C = nullptr;
	unk40 = nullptr;
	unk44 = 0;
	unk48 = 0;
	unk50 = 0;
	unk54 = nullptr;
	unk58 = nullptr;
	unk5C = 0;
	unk68 = nullptr;
	unk6C = nullptr;
	unk34 = -1;
	unk70 = 0;
	unkC  = nullptr;
	unk78 = 0;
}

void JAIBasic::startSeSequence() { }

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
	if (!unk1C.flag6)
		initStream();
	strcat(archivesPath, JAIGlobalParameter::sequenceArchivesPath);
	strcat(archivesPath, JAIGlobalParameter::sequenceArchivesFileName);
	if (!unk58) {
		JASystem::Vload::initHeader(archivesPath);
	} else {
		JASystem::Vload::initHeaderM(archivesPath, *unk58, nullptr);
	}
	JASystem::TrackMgr::initRegistTrack();
	unk2C = JASystem::Vload::getArchiveHandle(
	    JAIGlobalParameter::sequenceArchivesFileName);
	unk38 = 0;
	startSoundDirectID(0x80000800, &unk38, nullptr, 1, 4);
	if (!unk1C.flag3)
		checkEntriedSeq();
}

void JAIBasic::setCameraInfo(VecPtr pos, VecPtr dir, MtxPtr mtx, u32 id)
{
	if (JAIGlobalParameter::audioCameraMax <= id)
		return;

	unk8[id].unk0 = pos;
	unk8[id].unk4 = dir;
	unk8[id].unk8 = mtx;
}

void JAIBasic::initStream() { JAInter::StreamLib::init(unk1C.flag7); }

void JAIBasic::setRegisterTrackCallback()
{
	JASystem::TrackMgr::registerTrackCallback(&JAIBasic::setParameterSeqSync);
}

void JAIBasic::initAudioThread(JKRSolidHeap* heap, u32 param1, u8 param2) { }

void JAIBasic::bootDSP() { }

void JAIBasic::initReadFile()
{
	switch (unk13) {
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
		if (unk4C)
			checkInitDataOnMemory();
		break;
	}

	initBankWave();
	if (unk13 != 4 && unk4C != 0)
		deleteTmpDVDFile(&unk4C);
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

		unk3C = (FabricatedUnk3CStruct*)((u8*)file + header->unk2);
		unk40 = (FabricatedUnk40Struct*)((u8*)file + header->unk4);

		strcpy(JAIGlobalParameter::seInfoFileName,
		       (char*)((u8*)file + header->unk6));

		if (header->unk8) {
			strcpy(JAIGlobalParameter::seqInfoFileName,
			       (char*)((u8*)file + header->unk8));
			strcpy(JAIGlobalParameter::streamInfoFileName,
			       (char*)((u8*)file + header->unkA));
			unk0->unk1B0 = 1;
		} else {
			unk0->unk1B0 = 0;
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

	loadTmpDVDFile(JAIGlobalParameter::initDataFileName, &unk4C);
	if (unk4C) {
		checkInitDataOnMemory();
		return 1;
	} else {
		return 0;
	}
}

void JAIBasic::checkInitDataOnMemory()
{
	JAIData* data = unk0;

	bool shouldContinue = true;
	u32 i               = 0;
	while (shouldContinue) {
		u32 command = unk4C[i];
		++i;
		switch (command) {
		case 0:
			shouldContinue = false;
			break;
		case 1:
			if (unk4C[i] == 0) {
				u32 offset        = unk4C[i + 1];
				data->unk88.unk28 = unk4C[i + 2];
				data->unk88.unk78 = (u8*)transInitDataFile(
				    ((u8*)unk4C) + offset, data->unk88.unk28);
				data->unk1B0 = 0;
				i += 4;
			} else {
				// TODO: should this all be done via header structs? probably
				u32 offset        = unk4C[i + 1];
				data->unk88.unk28 = unk4C[i + 2];
				data->unk88.unk78 = (u8*)transInitDataFile(
				    ((u8*)unk4C) + offset, data->unk88.unk28);

				u8* buffer1      = ((u8*)unk4C) + unk4C[i + 3];
				data->unkC.unk28 = unk4C[i + 4];
				data->unkC.unk78
				    = (u8*)transInitDataFile(buffer1, data->unk88.unk28);

				u8* buffer         = ((u8*)unk4C) + unk4C[i + 5];
				data->unk104.unk28 = unk4C[i + 6];
				data->unk104.unk78
				    = (u8*)transInitDataFile(buffer, data->unk104.unk28);
				data->unk1B0 = 0;

				i += 7;
			}
			break;
		case 2: {
			u32 uVar7 = 0;
			while (unk4C[i + uVar7] != 0)
				uVar7 += 3;

			unk50 = (FabricatedUnk50Struct*)transInitDataFile(
			    (u8*)&unk4C[i], (uVar7 / 3) * 0xC + 4);
			// TODO: lots more stuff in here but I'm sick of it tbh
			break;
		}
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

	if (unk54) {
		for (int i = 0; unk54[i].unk0; ++i) {
			void* data = unk54[i].unk0;
			if (data) {
				JASystem::WaveBankMgr::registWaveBankWS(i, data);
				unk60[i] = -1;
				unk64[i] = 0;
			}
		}
	}

	if (unk40) {
		for (int i = 0; unk40[i].unk0[0] != 0; ++i) {
			void* data = loadDVDFile(unk40[i].unk0);
			if (data)
				JASystem::WaveBankMgr::registWaveBankWS(i, data);
		}
	}

	JASystem::BankMgr::init(0x100);

	if (unk50) {
		int i;

		for (i = 0; unk50[i].unk0; ++i) {
			void* data = unk50[i].unk0;
			if (data)
				JASystem::BankMgr::registBankBNK(i, data);
		}

		for (i = 0; unk50[i].unk0; ++i)
			JASystem::BankMgr::assignWaveBank(i, unk50[i].unk8);
	}

	if (unk3C) {
		for (int i = 0; unk3C[i].unk0[0] != 0; ++i) {
			void* file = loadDVDFile(unk3C[i].unk0);
			if (file)
				JASystem::BankMgr::registBankBNK(i, file);
			loadGroupWave(i, 0);
		}
	}
}

void JAIBasic::setWaveScene()
{
	if (unk54 && !unk1C.flag3) {
		for (int i = 0; unk54[i].unk0; ++i)
			if (unk54[i].unk8 == 0)
				loadGroupWave(i, 0);

		unk1C.flag1 = true;
	}
}

void JAIBasic::readInitSoundData() { }

void JAIBasic::loadFirstStayWave() { }

void JAIBasic::loadSecondStayWave() { }

void JAIBasic::setSceneSetFinishCallback(s32 param1, s32 param2)
{
	u32 id        = (param1 << 16) + param2;
	unk34         = -1;
	unk64[param1] = 1;
	JASystem::Dvd::checkPassDvdT(id, nullptr, &finishSceneSet);
}

void JAIBasic::finishSceneSet(u32 param)
{
	basic->unk34              = param;
	basic->unk64[param >> 16] = 2;
}

void JAIBasic::loadSceneWave(s32 param1, s32 param2)
{
	if (unk54 && unk54[param1].unk8 == 2 && unk60[param1] != param2) {
		if (unk60[param1] != -1)
			JASystem::WaveBankMgr::eraseWave(param1, unk60[param1]);
		loadGroupWave(param1, param2);
	}
}

BOOL JAIBasic::checkSceneWaveOnMemory(s32 param1, s32 param2)
{
	if (param2 == unk60[param1] && unk64[param1] == 2)
		return 1;
	return 0;
}

void JAIBasic::loadGroupWave(s32 param1, s32 param2)
{
	JASystem::WaveBankMgr::loadWave(param1, param2);
	setSceneSetFinishCallback(param1, param2);
	unk60[param1] = param2;
}

void JAIBasic::getWaveGroupNumber(s32 param) { }

void JAIBasic::getWaveLoadStatus(s32 param) { }

void JAIBasic::checkAllWaveLoadStatus() { }

void JAIBasic::initNullData()
{
	JAInullCamera.unk0->x = 0.0f;
	JAInullCamera.unk0->y = 0.0f;
	JAInullCamera.unk0->z = -50.0f;

	JAInullCamera.unk4->x = 0.0f;
	JAInullCamera.unk4->y = 0.0f;
	JAInullCamera.unk4->z = -50.0f;

	Vec up;
	up.x       = 0.0f;
	up.y       = 1.0f;
	up.z       = 0.0f;
	Vec target = JAIConst::dummyZeroVec;
	C_MTXLookAt(JAIConst::camMtx, JAInullCamera.unk0, &up, &target);
	for (int i = 0; i < JAIGlobalParameter::audioCameraMax; ++i)
		setCameraInfo(JAInullCamera.unk0, JAInullCamera.unk4, JAIConst::camMtx,
		              i);
}

void JAIBasic::initDriver(JKRSolidHeap* heap, u32 param_2, u8 param_3)
{
	s32 uVar1 = 1;
	if (param_3 & 1)
		uVar1 |= 2;

	JASystem::AudioThread::setPriority(
	    JAIGlobalParameter::audioSystemThreadPriority,
	    JAIGlobalParameter::audioDvdThreadPriority);
	JASystem::AudioThread::start(heap, param_2, uVar1);
	JASystem::TrackMgr::init(JAIGlobalParameter::systemTrackMax,
	                         JAIGlobalParameter::systemRootTrackMax);
	JASystem::TrackMgr::reset();
}

void JAIBasic::initInterface(u8 param)
{
	unk12 = param;
	initInterfaceMain();
}

void JAIBasic::initAllocParameter()
{
	unk0->initData();
	unk18 = (u8*)allocHeap(JAIGlobalParameter::getParamSeCategoryMax()
	                       * sizeof(*unk18));
	unk28 = (f32*)allocHeap(JAIGlobalParameter::getParamSeCategoryMax()
	                        * sizeof(*unk28));
	unk8  = (JAICamera*)allocHeap(JAIGlobalParameter::audioCameraMax
	                              * sizeof(JAICamera));
	for (int i = 0; i < JAIGlobalParameter::getParamSeCategoryMax(); ++i) {
		unk18[i] = 0;
		unk28[i] = 1.0f;
	}
}

void JAIBasic::initSeqsLoadArea()
{
	for (int i = 0; i < JAIGlobalParameter::autoHeapMax; ++i) {
		unk0->unk1EC[i].unk4 = allocHeap(JAIGlobalParameter::autoHeapRoomSize);
	}
	unk0->unk1F0->unk4 = allocHeap(JAIGlobalParameter::stayHeapSize);
}

void JAIBasic::setInitFileLoadSwitch(u8 flag) { unk13 = flag; }

void JAIBasic::startFrameInterfaceWork() { processFrameWork(); }

void JAIBasic::processFrameWork()
{
	checkDummyPositionBuffer();

	if (unk38->unk1 == 3 && !unk1C.flag2 && unk54) {
		for (int i = 0; unk54[i].unk0; ++i)
			if (unk54[i].unk8 == 1)
				loadGroupWave(i, 0);

		unk1C.flag2 = true;
	}

	if (unk38->unk1 >= 4)
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
	if (!unk1C.flag6)
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
	JAIDummyVec* it = unk0->unk22C;
	while (it) {
		JAIDummyVec* next = it->unk4;
		--it->unkC;
		if (it->unkC == 0 || !it->unk8) {
			if (it->unk8)
				it->unk8->stop(0);
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
		startSoundBasic((id & 0xFFFFFC00) + *(u16*)((u8*)ptr + 6), sound, actor,
		                param, flag, ptr);
}

void JAIBasic::startSoundBasic(u32 id, JAISound** sound, JAIActor* actor,
                               u32 param, u8 flag, void* data)
{
}

void JAIBasic::getPlayingSoundHandle(JAISound** sound, u32 param) { }

#pragma dont_inline on
void JAIBasic::stopSoundHandle(JAISound* sound, u32 param) { }
#pragma dont_inline off

u32 JAIBasic::changeIDToCategory(u32 id) { return id >> 0xc & 0xff; }

void JAIBasic::stopPlayingObjectSe(void* obj) { }

void JAIBasic::stopPlayingIDObjectSe(u32 id, void* obj) { }

void JAIBasic::stopPlayingCategorySe(u8 category) { }

void JAIBasic::stopPlayingCategoryObjectSe(u8 category, void* obj) { }

void JAIBasic::stopAllSe(void* obj) { }

void JAIBasic::stopAllSe(u8 param)
{
	JAISound* sound = unk0->unk1E8[param].unk4;
	while (sound) {
		JAISound* next = sound->unk30;
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
	JAISound** var2 = &buffer->unk4;
	if (buffer->unk0) {
		JAISound* var1 = buffer->unk0;
		buffer->unk0   = var1->unk30;
		if (*var2) {
			var1->unk30    = *var2;
			(*var2)->unk2C = var1;
		} else {
			var1->unk30 = nullptr;
		}
		var1->unk2C = nullptr;
		*var2       = var1;
		var1->unk38 = nullptr;
		return var1;
	}
	return nullptr;
}

void JAIBasic::releaseControllerHandle(JAILinkBuffer* buffer, JAISound* sound)
{
	JAISound** ptr = &buffer->unk4;

	sound->unk38 = 0;
	sound->unk34 = nullptr;
	if (buffer->unk4 != sound) {
		sound->unk2C->unk30 = sound->unk30;
		if (sound->unk30)
			sound->unk30->unk2C = sound->unk2C;
	} else {
		*ptr = sound->unk30;
		if (sound->unk30)
			sound->unk30->unk2C = nullptr;
	}
	sound->unk30 = buffer->unk0;
	if (sound->unk30)
		sound->unk30->unk2C = sound;
	buffer->unk0 = sound;
}

JAIStreamParameter* JAIBasic::getStreamParameter()
{
	JAIStreamParameter* var1;
	JAIStreamParameter** var2;

	var2 = &unk0->unk1D8;
	if (unk0->unk1D4->unk3DC != nullptr) {
		var1         = unk0->unk1D4;
		unk0->unk1D4 = var1->unk3DC;
		if (*var2 != nullptr) {
			var1->unk3DC    = *var2;
			(*var2)->unk3D8 = var1;
		} else {
			var1->unk3DC = nullptr;
		}
		var1->unk3D8 = nullptr;
		*var2        = var1;
		return var1;
	}
	return nullptr;
}

void JAIBasic::releaseStreamParameterPointer(JAIStreamParameter* param)
{
	JAIStreamParameter** start = &unk0->unk1D8;
	JAIStreamParameter** end   = &unk0->unk1D4;

	if (*start != param) {
		param->unk3D8->unk3DC = param->unk3DC;
		if (param->unk3DC)
			param->unk3DC->unk3D8 = param->unk3D8;
	} else {
		*start = param->unk3DC;
		if (param->unk3DC)
			param->unk3DC->unk3D8 = nullptr;
	}
	param->unk3DC = *end;
	if (param->unk3DC)
		param->unk3DC->unk3D8 = param;
	*end = param;
}

JAISeqParameter* JAIBasic::getSeqParametermeterPointer()
{
	JAISeqParameter** var2 = &unk0->unk1C0;
	if (unk0->unk1BC->unk1858) {
		JAISeqParameter* var1 = unk0->unk1BC;
		unk0->unk1BC          = var1->unk1858;
		if (*var2) {
			var1->unk1858    = *var2;
			(*var2)->unk1854 = var1;
		} else {
			var1->unk1858 = nullptr;
		}
		var1->unk1854 = nullptr;
		*var2         = var1;
		return var1;
	}
	return nullptr;
}

void JAIBasic::releaseSeqParameterPointer(JAISeqParameter* param)
{
	JAISeqParameter** start = &unk0->unk1C0;
	JAISeqParameter** end   = &unk0->unk1BC;

	if (*start != param) {
		param->unk1854->unk1858 = param->unk1858;
		if (param->unk1858)
			param->unk1858->unk1854 = param->unk1854;
	} else {
		*start = param->unk1858;
		if (param->unk1858)
			param->unk1858->unk1854 = nullptr;
	}
	param->unk1858 = *end;
	if (param->unk1858)
		param->unk1858->unk1854 = param;
	*end = param;
}

JAISeParameter* JAIBasic::getSeParametermeterPointer()
{
	JAISeParameter* var1;
	JAISeParameter** var2 = &unk0->unk1CC;
	if (unk0->unk1C8) {
		var1         = unk0->unk1C8;
		unk0->unk1C8 = var1->unk440;
		if (*var2 != nullptr) {
			var1->unk440    = *var2;
			(*var2)->unk43C = var1;
		} else {
			var1->unk440 = nullptr;
		}
		var1->unk43C = nullptr;
		*var2        = var1;
		unk0->initSePara(var1);
	} else {
		var1 = nullptr;
	}
	return var1;
}

void JAIBasic::releaseSeParameterPointer(JAISeParameter* param)
{
	if (!param)
		return;

	JAISeParameter** start = &unk0->unk1CC;
	JAISeParameter** end   = &unk0->unk1C8;

	if (*start != param) {
		param->unk43C->unk440 = param->unk440;
		if (param->unk440)
			param->unk440->unk43C = param->unk43C;
	} else {
		*start = param->unk440;
		if (param->unk440)
			param->unk440->unk43C = nullptr;
	}
	param->unk440 = *end;
	if (param->unk440)
		param->unk440->unk43C = param;
	*end = param;
}

void JAIBasic::getDummyVecPointer() { }

void JAIBasic::releaseDummyVecPointer(JAIDummyVec* vec)
{
	JAIDummyVec** start = &unk0->unk22C;
	JAIDummyVec** end   = &unk0->unk228;

	if (*start != vec) {
		vec->unk0->unk4 = vec->unk4;
		if (vec->unk4)
			vec->unk4->unk0 = vec->unk0;
	} else {
		*start = vec->unk4;
		if (vec->unk4)
			vec->unk4->unk0 = nullptr;
	}
	vec->unk4 = *end;
	if (vec->unk4)
		vec->unk4->unk0 = vec;
	*end = vec;
}

void JAIBasic::getGameFrameCounter() { }

void JAIBasic::setPauseFlagAll(u8 flag) { }

void JAIBasic::checkPlayingSoundTrack(u32 param) { }

void JAIBasic::changeSoundScene(u32 scene) { }

BOOL JAIBasic::getMapInfoFxline(u32 param)
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
	if (unk0->unk1B0 == 0) {
		_dinfo = &unk0->unk88;
	} else {
		switch (id & 0xC0000000) {
		case 0x00000000:
			_dinfo = &unk0->unk88;
			break;
		case 0x80000000:
			_dinfo = &unk0->unkC;
			break;
		case 0xC0000000:
			_dinfo = &unk0->unk104;
			break;
		}
	}
	return _dinfo;
}

u32 JAIBasic::getInfoFormat(JAISoundTable* table, u32 id)
{
	u32 result = 0;
	switch (id & 0xC0000000) {
	case 0x00000000:
		result = ((u8*)table->unk78)[0];
		break;
	case 0x80000000:
		result = ((u8*)table->unk78)[1];
		break;
	case 0xC0000000:
		result = ((u8*)table->unk78)[2];
		break;
	}
	return result;
}

void JAIBasic::setSeCancelSwitch(u8 param1, u8 param2) { }

void JAIBasic::setSeCategoryVolume(u8 category, u8 volume)
{
	unk28[category] = volume / 127.0f;
}

u16 JAIBasic::setParameterSeqSync(JASystem::TTrack* param_1, u16 param_2)
{
	u16 result = 0;

	switch (param_2) {
	case 0:
		for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
			if (basic->unk0->unk180[i].unk48 == nullptr)
				continue;

			JASystem::TTrack* track = JASystem::TrackMgr::handleToSeq(
			    basic->unk0->unk180[i].unk48->getSeqParameter()->unk0);
			if (track != param_1->unk2C0)
				continue;

			u32 route = basic->routeToTrack(param_1->unk308);
			JAISoundInfo* info
			    = basic->getSoundInfoFromID(basic->unk0->unk180[i].unk48->unk8);

			JAISystemInterface::outerInit(&basic->unk0->unk180[i], param_1,
			                              route, info->unk0 >> 8, param_2 & 1);
			result = 0;
			basic->unk0->unk180[i].unk4 |= 1 << route;
			i = JAIGlobalParameter::seqPlayTrackMax;
		}
		break;
	case 1: {
		u32 uVar8                            = param_1->unk308;
		JASystem::TTrack::TOuterParam* outer = param_1->mOuterParam;
		JAIData::FabricatedUnk0Struct* params
		    = &basic->unk0->unk0[uVar8 & 0xff];

		outer->setParam(1, params->unk4);
		outer->setParam(8, params->unk10);
		outer->setParam(2, params->unk8);
		outer->setParam(4, params->unkC);
		f32 thing;
		if (basic->unk14 != 2)
			thing = 0.0f;
		else
			thing = params->unk14;
		outer->setParam(16, thing);
		break;
	}
	case 0x7F:
		param_1->writePortApp(0, basic->unk10);
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

u8 JAIBasic::getSeqTrackNumber(void* param) { return *((u8*)param + 5); }

u8 JAIBasic::getSoundPrioity(void* param) { return *((u8*)param + 4); }

u32 JAIBasic::getSoundSwBit(void* param) { return *((u32*)param); }

void JAIBasic::setSeExtParameter(JAISound* sound)
{
	if (!sound)
		return;
	JAISoundTable* table = getInfoPointerFromID(sound->unk8);
	u8 format            = getInfoFormat(table, sound->unk8);

	if (format & 4)
		sound->setVolume(*((u8*)sound->unk3C + 12) / 127.0f, 0, 1);
	if (format & 8)
		sound->setFxmix(*((u8*)sound->unk3C + 13) / 127.0f, 0, 1);
	if (format & 2)
		sound->setPitch(*(f32*)((u8*)sound->unk3C + 8), 0, 1);
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
		unkC = JKRSolidHeap::create(JAIGlobalParameter::interfaceHeapSize,
		                            JASDram, 0);
	}
}

void* JAIBasic::allocHeap(u32 size)
{
	if (JAIGlobalParameter::interfaceHeapSize)
		return new (unkC, 0x20) u8[size];
	else
		return JASystem::Kernel::allocFromSysDram(size);
}

JAISound* JAIBasic::makeSound(u32 param)
{
	if (unkC)
		return new (unkC, 0) JAISound[param];
	else
		return new (JASDram, 0) JAISound[param];
}

void* JAIBasic::loadDVDFile(char* filename)
{
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
	u32 uVar1   = param_1 & 0x3ff;
	u32 uVar2   = JASystem::Vload::checkSize(uVar1);
	void* iVar3 = unk0->checkOnMemory(uVar1, nullptr);

	u32 uVar6 = getSoundInfoFromID(param_1)->unk0;

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
			JASystem::Vload::loadFileAsync(unk2C + uVar1, puVar4, 0, uVar2,
			                               &checkDvdLoadArc, id);
			return 1;
		} else {
			JASystem::Vload::loadFile(unk2C + uVar1, puVar4, 0, uVar2);
			return 2;
		}
	}
	return 0;
}
