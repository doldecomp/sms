#include <MSound/MSound.hpp>
#include <MSound/MSRandVol.hpp>
#include <MSound/MSHandle.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/MSoundBGM.hpp>
#include <MSound/MSModBgm.hpp>
#include <System/MSoundMainSide.hpp>
#include <JSystem/JAudio/JADebug/JADHioNode.hpp>
#include <JSystem/JAudio/JAInterface/JAIDebug.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAIAsnData.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASWaveBankMgr.hpp>
#include <JSystem/JAudio/JASystem/JASWaveBank.hpp>
#include <JSystem/JAudio/JASystem/JASBasicWaveBank.hpp>
#include <JSystem/JAudio/JASystem/JASSimpleWaveBank.hpp>
#include <JSystem/JAudio/JASystem/JASDvdThread.hpp>
#include <JSystem/JAudio/JALibrary/JALSystem.hpp>

MSound* MSGMSound  = 0;
JAIBasic* MSGBasic = 0;

u8 MSSeCallBack::smTrackCategory;
u8 MSSeCallBack::smPolifonic;
u16 MSSeCallBack::smWaterFilter;

bool MSLoadWave::loadWaveBackword(int param_1, int param_2)
{
	JASystem::TWaveBank* bank = JASystem::WaveBankMgr::getWaveBank(param_1);
	if (!bank)
		return false;

	if (bank->getType() == 'BSIC') {
		JASystem::TBasicWaveBank::TWaveGroup* grp
		    = ((JASystem::TBasicWaveBank*)bank)->getWaveGroup(param_2);

		if (!grp)
			return false;

		if (!loadWaveBackword(grp))
			return false;

		((JASystem::TBasicWaveBank*)bank)->incWaveTable(grp);
		return true;
	} else if (bank->getType() == 'SMPL') {
		JASystem::TSimpleWaveBank* obj = (JASystem::TSimpleWaveBank*)bank;
		return loadWaveBackword(obj);
	} else {
		return false;
	}
}

bool MSLoadWave::loadWaveBackword(JASystem::WaveArcLoader::TObject* obj)
{
	JASystem::Kernel::THeap* heap = obj->getHeap();
	if (!heap)
		return false;

	if (heap->unk8 != 0)
		return false;

	char filePath[256];
	strcpy(filePath, JASystem::WaveArcLoader::getCurrentDir());
	strcat(filePath, obj->getWaveArcFileName());
	u32 extent = JASystem::Dvd::checkFileExtend(filePath);
	if (!extent)
		return false;

	JASystem::Kernel::THeap* root = JASystem::WaveArcLoader::getRootHeap();
	if (!heap->selfAlloc(root, extent, (u32)root->unk8 + root->unk10 - extent))
		return false;

	u32* ptr = obj->getLoadFlagPtr();
	*ptr     = 0;
	if (JASystem::Dvd::loadToAramDvdT(0, filePath, heap->unk8, 0, extent, ptr,
	                                  nullptr)
	    == -1) {
		heap->free();
		return false;
	}

	return true;
}

void MSSeCallBack::setWaterCameraFir(bool enabled)
{
	if (enabled)
		smWaterFilter = 0x78;
	else
		smWaterFilter = 0;
}

void MSSeCallBack::setWaterFilter(u16 param) { }

u16 MSSeCallBack::setParameterSeqSync(JASystem::TTrack* track, u16 param) { }

MSound* MSound::getMSound() { return nullptr; }

JAISound* MSound::makeSound(u32 count)
{
	if (unkC != 0)
		return new (unkC, 0) MSHandle[count];
	else
		return new (JASDram, 0) MSHandle[count];
}

void MSound::setRegisterTrackCallback()
{
	JASystem::TrackMgr::registerTrackCallback(
	    &MSSeCallBack::setParameterSeqSync);
}

void MSound::loadGroupWave(s32 param_1, s32 param_2)
{
	if (param_1 == 2 && param_2 != 0x10) {
		MSLoadWave::loadWaveBackword(param_1, param_2);
		setSceneSetFinishCallback(param_1, param_2);
		unk60[param_1] = param_2;
	} else if (param_1 == 2 && param_2 == 0x10) {
		JASystem::WaveBankMgr::loadWave(param_1, param_2);
	} else {
		JASystem::WaveBankMgr::loadWave(param_1, param_2);
		setSceneSetFinishCallback(param_1, param_2);
		unk60[param_1] = param_2;
	}
}

void MSound::cleanUpAramWave(u8 param) { }

void MSound::loadWave(MS_SCENE_WAVE wave)
{
	if (wave == -1)
		return;

	u8 hi = wave >> 8;
	u8 lo = wave & 0xff;
	if (wave == 0x210)
		JASystem::WaveBankMgr::loadWave(hi, lo);
	else
		loadSceneWave(hi, lo);
}

void MSound::enterStage(MS_SCENE_WAVE wave, u8 param2, u8 param3)
{
	unkCD = param2;
	unkCE = param3;

	if (wave == -1)
		return;

	loadWave(wave);
}

void MSound::exitStage()
{
	for (u8 i = 0; i < JAIGlobalParameter::getParamSeCategoryMax(); ++i)
		if (unk0->unk88.unk2[i])
			stopAllSe(i);

	MSBgm::stopTrackBGMs(7, 0);
	if (unkC4)
		unkC4->stop(0);

	unk8[0] = JAInullCamera;

	unkAC[0] = JAICamera();
	unkAC[0] = JAInullCamera;
	unkAC[1] = JAICamera();
	unkAC[1] = JAInullCamera;

	unkCD = 0xff;
	unkCE = 0xff;
	unkC8 = 0;
}

bool MSound::checkWaveOnAram(MS_SCENE_WAVE wave)
{
	u8 hi = wave >> 8;
	u8 lo = wave & 0xff;
	if (wave == 0x210) {
		JASystem::TWaveBank* bank = JASystem::WaveBankMgr::getWaveBank(hi);
		if (!bank)
			return false;

		if (bank->getType() == 'BSIC') {
			JASystem::TBasicWaveBank::TWaveGroup* grp
			    = ((JASystem::TBasicWaveBank*)bank)->getWaveGroup(lo);

			if (!grp)
				return false;

			if (*grp->getLoadFlagPtr())
				return true;
			else
				return false;
		} else if (bank->getType() == 'SMPL') {
			JASystem::TSimpleWaveBank* obj = (JASystem::TSimpleWaveBank*)bank;
			if (*obj->getLoadFlagPtr())
				return true;
			else
				return false;
		} else {
			return false;
		}
	} else {
		return checkSceneWaveOnMemory(hi, lo);
	}
}

bool MSound::checkSeqOnMemory(u32 id) { return false; }

void MSound::setCameraInfo(Vec* param_1, Vec* param_2, MtxPtr param_3,
                           u32 param_4)
{
	if (param_1 == nullptr) {
		unk8[param_4] = JAInullCamera;
	} else {
		JAIBasic::setCameraInfo(param_1, param_2, param_3, param_4);
	}
}

void MSound::setPlayerInfo(Vec* param_1, Vec* param_2, MtxPtr param_3,
                           bool param_4)
{
	u8 i = param_4 == 1 ? 0 : 1;
	if (param_1 == 0) {
		unkAC[i] = JAICamera();
		unkAC[i] = JAInullCamera;
	} else {
		unkAC[i].unk0 = param_1;
		unkAC[i].unk4 = param_2;
		unkAC[i].unk8 = param_3;
		MSoundSESystem::MSRandPlay::createRandPlayVec(0x7865, 1);
		MSoundSESystem::MSRandPlay::registerTrans(0x7865, param_1);
		MSoundSESystem::MSRandPlay::createRandPlayVec(0x7094, 1);
		MSoundSESystem::MSRandPlay::registerTrans(0x7094, param_1);
		MSoundSESystem::MSRandPlay::createRandPlayVec(0x1950, 1);
		MSoundSESystem::MSRandPlay::registerTrans(0x1950, param_1);
	}
}

f32 MSound::getDistFromCamera(Vec* pos)
{
	return JALCalc::getDist(pos, unk8->unk0);
}

MSound::MSound(JKRHeap* param_1, JKRHeap* param_2, u32 param_3, u8* param_4,
               u8* param_5, u32 param_6)
{
	JKRSolidHeap* heap = JKRSolidHeap::create(0x151800, param_1, false);
	if (param_2 != nullptr) {
		JAInter::TDebugHeap::currentHeap = param_2;
		JADHioNode::smCurrentHeap        = param_2;
	}
	JAIGlobalParameter::setParamInitDataFileName("/AudioRes/mSound.aaf");
	JAInter::TAsnData::asnFileName = "/AudioRes/mSound.asn";
	JAIGlobalParameter::setParamWavePath("/AudioRes/Banks/");
	JAIGlobalParameter::setParamSequenceArchivesPath("/AudioRes/Seqs/");
	JAIGlobalParameter::setParamStreamPath("/AudioRes/Streams/");
	JAIGlobalParameter::setParamSystemTrackMax(0xb9);
	JAIGlobalParameter::setParamSeqPlayTrackMax(4);
	JAIGlobalParameter::setParamSeqControlBufferMax(8);
	JAIGlobalParameter::setParamSystemRootTrackMax(8);
	JAIGlobalParameter::setParamStayHeapSize(0xf000);
	JAIGlobalParameter::setParamAutoHeapMax(3);
	JAIGlobalParameter::setParamAutoHeapRoomSize(0xa2ff);
	JAIGlobalParameter::setParamStayHeapMax(1);
	JAIGlobalParameter::setParamStreamInsideBufferCut(true);
	JAIGlobalParameter::setParamInputGainDown(0.802);
	JAIGlobalParameter::setParamOutputGainUp(5.0);
	setInitFileLoadSwitch(2);

	if (param_4 != nullptr)
		JAIGlobalParameter::setParamInitDataPointer(param_4);
	if (param_5 != nullptr)
		JAInter::TAsnData::asnData = param_5;

	MSSeCallBack::smWaterFilter = nullptr;
	initDriver(heap, param_3, 1);
	initInterface(1);
	f32 fVar1 = 0.0f;
	for (u8 i = 0; i < 16; ++i) {
		if (unk0->unk88.unk2[i] != 0) {
			if (fVar1 < MSHandle::smSeCategory[i].unk4)
				fVar1 = MSHandle::smSeCategory[i].unk4;
			u8 uVar2 = (u8)(MSHandle::smSeCategory[i].unk4 * 127.0f);
			if ((u8)(MSHandle::smSeCategory[i].unk4 * 127.0f) > 0x7E)
				uVar2 = 0x7F;
			setSeCategoryVolume(i, uVar2);
		}
	}

	JAIGlobalParameter::setParamDistanceMax(fVar1);
	JAIGlobalParameter::setParamMinDistanceVolume(0.0);
	JAIGlobalParameter::setParamMaxVolumeDistance(1200.0);
	unkA8     = 3;
	MSGBasic  = JAIBasic::basic;
	MSGMSound = this;
	JALSystem::init();
	MSoundSESystem::MSoundSE::construct();
	MSBgm::init();
	unk88  = 0;
	unk98  = new u64; // TODO: wrong type & size
	f32* p = new f32; // TODO: wrong type & size
	if (p)
		*p = 0.0f;
	unk9C = p;

	unk7C = 0;
	unk80 = 0;

	unkC8 = 0;
	unkC9 = 0;
	unkCA = 0;
	unkCB = 0;
	unkCC = 0;

	unkAC[0] = JAInullCamera;
	unkAC[1] = JAInullCamera;

	unk84 = 0;
	unk94 = 0;
	unk8C = 0;
	unk90 = 0;
	unkC4 = 0;

	unkCF = 1;
	unkD0 = 1;
	unkCD = 0xFF;
	unkCE = 0xFF;

	unkA0 = 0;
	unkA4 = 0;
}

void MSound::requestShineAppearFanfare() { }

void MSound::mainLoop()
{
	if (unkCF == 0 && unkA8 == 0)
		return;

	if (unkD1 == 1) {
		MSBgm::startBGM(0x8001002D);
		unkD1 = 0;
	}

	if (unkC9 != 0) {
		MSMainProc::entranceDemoLoop(unkA4);
		++unkA4;
	}

	for (JSUListIterator<MSSetSound> it
	     = JALList<MSSetSound>::smList.getFirst();
	     it != JALList<MSSetSound>::smList.getEnd(); ++it)
		it.getObject()->frameLoopDyna();

	for (JSUListIterator<MSSetSoundGrp> it
	     = JALList<MSSetSoundGrp>::smList.getFirst();
	     it != JALList<MSSetSoundGrp>::smList.getEnd(); ++it)
		it.getObject()->frameLoopDyna();

	startFrameInterfaceWork();
	((MSModBgm*)unk98)->loop();
}

JAISound* MSound::startSoundSet(u32 id, const Vec* pos, u32 param3, f32 volume,
                                u32 param5, u32 param6, u8 param7)
{
	return nullptr;
}

JAISound* MSound::startSoundSetGrp(u32 id, const Vec* pos, u32 param3,
                                   f32 volume, u32 param5, u32 param6,
                                   u8 param7)
{
	return nullptr;
}

void MSound::initSound() { }

void MSound::pauseOn(bool param) { }

void MSound::pauseOff(u8 param) { }

void MSound::demoModeIn(u16 param1, bool param2) { }

void MSound::demoModeOut(bool param) { }

void MSound::talkModeIn(bool param) { }

void MSound::talkModeOut() { }

void MSound::setCategoryVOLsDefault(u16 param) { }

void MSound::setCategoryVOLs(u16 param1, f32 volume) { }

bool MSound::resetAudioAll(u16 param) { }

void MSound::stopAllSeInCategory(u8 category, u32 param2) { }

void MSound::setCategoryAllVolume(u8 category, f32 volume, u32 param3,
                                  u8 param4)
{
}

void MSound::fadeOutAllSound(u32 fadeTime) { }

void MSound::stopAllSound() { }

f32 MSoundSESystem::MSRandVol::getRandomVolumeNormal(u32) { return 0.0f; }

void MSound::setSeExtParameter(JAISound* sound) { }

void MSound::playTimer(u32 time) { }

void MSound::startMarioVoice(u32 id, s16 param2, u8 param3) { }

u8 MSound::getMarioVoiceID(u8 param) { return 0; }

void MSound::stopMarioVoice(u32 id, u8 param2) { }

bool MSound::checkMarioVoicePlaying(u8 param) { return false; }

u32 MSound::getWallSound(u32 param1, f32 param2)
{
	if (param2 < 15.0f) {
		if (param1 == 0x1C)
			return 0x194B;
		else
			return 0x194A;
	}

	if (param2 < 30.0f) {
		if (param1 == 0x1C)
			return 0x1802;
		else
			return 0x1949;
	}

	if (param1 == 0x1C)
		return 0x1803;
	else
		return 0x1948;
}

JAISound* MSound::startBeeSe(Vec* pos, u32 id) { return nullptr; }

JAISound* MSound::startSoundActorSpecial(u32 id, const Vec* pos, f32 param3,
                                         f32 param4, u32 param5,
                                         JAISound** soundPtr, u32 param7,
                                         u8 param8)
{
	return nullptr;
}

bool MSound::cameraLooksAtMario() { return false; }

bool MSound::gateCheck(u32 param) { return false; }

u32 MSound::getBstSwitch(u32 param)
{
	JAISoundInfo* info = MSGBasic->getSoundInfoFromID(param);
	if (!info)
		return -1;

	return info->unk0;
}

u32 MSound::getSwitch(u32 param1, u32 param2, u32 param3)
{
	u32 bstSwitch = getBstSwitch(param1);
	return (bstSwitch & param2) >> param3;
}

u32 MSound::getBstPitch(u32 param) { return 0; }
