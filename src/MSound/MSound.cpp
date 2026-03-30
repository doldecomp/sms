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
#include <JSystem/JAudio/JAInterface/JAIConst.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASWaveBankMgr.hpp>
#include <JSystem/JAudio/JASystem/JASWaveBank.hpp>
#include <JSystem/JAudio/JASystem/JASBasicWaveBank.hpp>
#include <JSystem/JAudio/JASystem/JASSimpleWaveBank.hpp>
#include <JSystem/JAudio/JASystem/JASDvdThread.hpp>
#include <JSystem/JAudio/JASystem/JASDriverIF.hpp>
#include <JSystem/JAudio/JASystem/JASAudioThread.hpp>
#include <JSystem/JAudio/JALibrary/JALSystem.hpp>
#include <math.h>

MSound* MSGMSound  = 0;
JAIBasic* MSGBasic = 0;

u16 MSSeCallBack::smTrackCategory[32] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

u8 MSSeCallBack::smPolifonic[16] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

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

void MSSeCallBack::setWaterFilter(u16 param_1) { }

u16 MSSeCallBack::setParameterSeqSync(JASystem::TTrack* param_1, u16 param_2)
{
	switch (param_2) {
	case 15:
		return MSGMSound->unk94;
		break;

	case 20:
		for (u16 i = 0; i < 2; ++i) {
			for (u16 j = 0; j < 16; ++j) {
				JASystem::TTrack* pTVar1 = param_1->unk2C4[i]->unk2C4[j];
				if (pTVar1 == nullptr)
					break;

				pTVar1->readPortAppDirect(9, &smTrackCategory[i << 4 | j]);

				smPolifonic[smTrackCategory[i << 4 | j]] += 1;
			}
		}

		for (u16 uVar3 = 0; uVar3 < JAIGlobalParameter::getParamSeCategoryMax();
		     ++uVar3) {
			(void)uVar3;
		}
		return 0;

	case 12: {
		u16 local_26;
		u16 local_28;
		param_1->unk2C0->readPortAppDirect(8, &local_26);
		param_1->readPortAppDirect(0xf, &local_28);
		u16 uVar4 = local_26 > smWaterFilter ? smWaterFilter : local_26;

		u16 local_2c;
		if (uVar4 != local_28) {
			u16 local_2a;
			param_1->readPortAppDirect(14, &local_2a);
			if (local_2a > uVar4) {
				local_2c = 0xFFEC;
			} else {
				local_2c = 0x14;
			}
			param_1->writePortAppDirect(13, local_2c);
			param_1->writePortAppDirect(15, uVar4);
			local_28 = uVar4;
		} else {
			param_1->readPortAppDirect(13, &local_2c);
		}

		if (local_2c != 0) {
			u16 local_2a;
			param_1->readPortAppDirect(14, &local_2a);
			local_2a += local_2c;
			if (local_2a > 0x7FFF || local_2a == 0) {
				local_2c = 0;
				local_2a = 0;
				param_1->writePortAppDirect(13, local_2a);
			} else if (local_28 != 0 && local_2a > local_28) {
				local_2a = local_28;
				local_2c = 0;
				param_1->writePortAppDirect(13, local_2c);
			}

			param_1->writePortAppDirect(14, local_2a);
			return 0x7f - local_2a;
		}

		return 0xff;
	}

	case 13: {
		u16 local_26;
		param_1->unk2C0->readPortAppDirect(8, &local_26);
		u16 uVar3 = local_26 > smWaterFilter ? smWaterFilter : local_26;
		param_1->writePortAppDirect(0xe, uVar3);
		param_1->writePortAppDirect(0xf, uVar3);
		return 0x7f - uVar3;
	}

	case 0: {
		u16 uVar3 = JAIBasic::setParameterSeqSync(param_1, param_2);

		param_1->unk3C1 = 74;
		if (param_1->unk2C0->unk2C4[15] == param_1)
			param_1->unk3C2 = 1;

		return uVar3;
	}

	case 1:
		break;

	case 30: {
		u16 uVar3 = JAIBasic::setParameterSeqSync(param_1, 0);
		param_1->setPanSwitchParent(1, 0);
		param_1->setPanSwitchParent(1, 1);
		param_1->setPanSwitchParent(1, 2);
		param_1->writeRegDirect(8, 0);
		param_1->writeRegDirect(10, 0);
		param_1->writeRegDirect(11, 0);
		param_1->writeRegDirect(12, 0x7fff);
		JASystem::TTrack* pTVar1 = param_1->unk2C0;
		if (pTVar1 != nullptr) {
			pTVar1->setPanSwitchExt(1, 0);
			pTVar1->setPanSwitchExt(1, 1);
			pTVar1->setPanSwitchExt(1, 2);
			pTVar1->writeRegDirect(8, 0);
			pTVar1->writeRegDirect(10, 0);
			pTVar1->writeRegDirect(11, 0x7fff);
			pTVar1->writeRegDirect(12, 0);
			if (pTVar1->mOuterParam != nullptr) {
				pTVar1->mOuterParam->onSwitch(8);
				pTVar1->mOuterParam->onSwitch(4);
				pTVar1->mOuterParam->onSwitch(0x10);
			}
		}
		param_1->mChannelUpdater.unk4E[0] = 0xffff;
		return uVar3;
	}

	case 40:
		MSGMSound->unkD1 = 1;
		return 0;

	case 110: {
		u8 a = MSGMSound->unkCD;
		u8 b = MSGMSound->unkCE;
		if (a == 8 && b == 6)
			return 0xffff;
		return MSGMSound->unkCD;
	}

	case 120:
		static bool ukuleleFlag = 0;
		ukuleleFlag ^= true;
		return ukuleleFlag;

	case 121:
		return ukuleleFlag;

	case 122:
	case 127:
		break;
	}

	return JAIBasic::setParameterSeqSync(param_1, param_2);
}

MSound* MSound::getMSound() { return MSGMSound; }

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

void MSound::cleanUpAramWave(u8 param_1) { }

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

void MSound::enterStage(MS_SCENE_WAVE wave, u8 param_2, u8 param3)
{
	unkCD = param_2;
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

	unkCD    = 0xff;
	unkCE    = 0xff;
	unkC8[0] = 0;
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

	for (int i = 0; i < 5; ++i)
		unkC8[i] = 0;

	unkAC[0] = JAInullCamera;
	unkAC[1] = JAInullCamera;

	unk84    = 0;
	unk94    = 0;
	unk8C[0] = 0;
	unk8C[1] = 0;
	unkC4    = 0;

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

	if (unkC8[1] != 0) {
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

void MSound::startSoundSet(u32 param_1, const Vec* param_2, u32 param_3,
                           f32 param_4, u32 param_5, u32 param_6, u8 param_7)
{
	if (gateCheck(param_1))
		MSSetSound::startSoundSet(param_1, param_2, param_3, param_4, param_5,
		                          param_6, param_7);
}

void MSound::startSoundSetGrp(u32 param_1, const Vec* param_2, u32 param_3,
                              f32 param_4, u32 param_5, u32 param_6, u8 param_7)
{
	if (gateCheck(param_1))
		MSSetSoundGrp::startSoundSetGrp(param_1, param_2, param_3, param_4,
		                                param_5, param_6, param_7);
}

void MSound::initSound()
{
	unkA8 |= 0x2;
	for (u8 i = 0; i < 16; ++i) {
		if (MSGMSound->unk0->unk88.unk2[i] != 0 && JAIBasic::basic != nullptr) {
			u8 uVar1 = MSHandle::smSeCategory[i].unk8 * 127.0f;
			if (uVar1 >= 127)
				uVar1 = 127;
			JAIBasic::basic->setSeCategoryVolume(i, uVar1);
		}
	}

	if (unk7C != nullptr) {
		unk7C->stop(1);
		unk7C = nullptr;
	}

	if (unk80 != nullptr) {
		unk80->stop(1);
		unk80 = nullptr;
	}
}

void MSound::pauseOn(bool param_1)
{
	if (param_1)
		if (!(unkA8 & 2) ? false : true)
			MSoundSESystem::MSoundSE::startSoundSystemSE(0x4802, 0, nullptr, 0);

	for (u8 i = 0; i < 16; ++i)
		if (i != 4 && MSGMSound->unk0->unk88.unk2[i] != 0)
			MSGMSound->setSeCategoryVolume(i, 0);

	if (param_1) {
		for (u8 i = 0; i < 3; ++i)
			if (0x7 >> i & 1)
				MSBgm::setTrackVolume(i, 0.0f, 60, 3);
	} else {
		for (u8 i = 0; i < 3; ++i)
			if (0x7 >> i & 1)
				MSBgm::setTrackVolume(i, 0.0f, 0, 3);
	}
}

void MSound::pauseOff(u8 param_1)
{
	switch (param_1) {
	case 0:
		if (!(unkA8 & 2) ? false : true)
			MSoundSESystem::MSoundSE::startSoundSystemSE(0x4803, 0, nullptr, 0);
		// FALLTHROUGH!!!

	case 2:
		for (u8 i = 0; i < 16; ++i) {
			if (i != 4 && MSGMSound->unk0->unk88.unk2[i] != 0
			    && JAIBasic::basic != nullptr) {
				u8 uVar1 = MSHandle::smSeCategory[i].unk8 * 127.0f;
				if (uVar1 >= 127)
					uVar1 = 127;
				JAIBasic::basic->setSeCategoryVolume(i, uVar1);
			}
		}
		for (u8 i = 0; i < 3; ++i)
			if (0x7 >> i & 1)
				MSBgm::setTrackVolume(i, 1.0f, 10, 3);
		break;

	case 1:
		if (!(unkA8 & 2) ? false : true)
			MSoundSESystem::MSoundSE::startSoundSystemSE(0x481B, 0, nullptr, 0);

		for (u8 i = 0; i < 16; ++i)
			if (MSGMSound->unk0->unk88.unk2[i] != 0)
				MSGMSound->setSeCategoryVolume(i, 0);

		for (u8 i = 0; i < 3; ++i)
			if (0x7 >> i & 1)
				MSBgm::setTrackVolume(i, 0.0f, 15, 3);
		break;
	}
}

void MSound::demoModeIn(u16 param_1, bool param_2)
{
	for (u8 i = 0; i < 16; ++i) {
		if (param_1 >> i & 1)
			if (MSGMSound->unk0->unk88.unk2[i] != 0)
				MSGMSound->setSeCategoryVolume(i, 0);
	}

	if (param_2) {
		for (u8 i = 0; i < 3; ++i)
			if (0x7 >> i & 1)
				MSBgm::setTrackVolume(i, 0.0f, 15, 3);
	}
}

void MSound::demoModeOut(bool param_1)
{
	for (u8 i = 0; i < 16; ++i) {
		if (MSGMSound->unk0->unk88.unk2[i] != 0 && JAIBasic::basic != nullptr) {
			u8 uVar1 = MSHandle::smSeCategory[i].unk8 * 127.0f;
			if (uVar1 >= 127)
				uVar1 = 127;
			JAIBasic::basic->setSeCategoryVolume(i, uVar1);
		}
	}

	if (param_1) {
		for (u8 i = 0; i < 3; ++i)
			if (0x7 >> i & 1)
				MSBgm::setTrackVolume(i, 1.0f, 15, 3);
	}
}

void MSound::talkModeIn(bool param_1)
{
	if (param_1 && (!(unkA8 & 2) ? false : true)) {
		MSoundSESystem::MSoundSE::startSoundSystemSE(0x4804, 0, nullptr, 0);
	}

	for (u8 i = 0; i < 16; ++i)
		if (MSGMSound->unk0->unk88.unk2[i] != 0)
			if ((0x44 >> i) & 1)
				MSGMSound->setSeCategoryVolume(i, 0);

	for (u8 i = 0; i < 3; ++i)
		if (0x7 >> i & 1)
			MSBgm::setTrackVolume(i, 0.6f, 30, 3);
}

void MSound::talkModeOut()
{
	if (!(unkA8 & 2) ? false : true) {
		MSoundSESystem::MSoundSE::startSoundSystemSE(0x4805, 0, nullptr, 0);
	}

	for (u8 i = 0; i < 16; ++i) {
		if (MSGMSound->unk0->unk88.unk2[i] != 0)
			if (0x1FF >> i & 1)
				if (JAIBasic::basic != nullptr) {
					u8 uVar1 = MSHandle::smSeCategory[i].unk8 * 127.0f;
					if (uVar1 >= 127)
						uVar1 = 127;
					JAIBasic::basic->setSeCategoryVolume(i, uVar1);
				}
	}

	for (u8 i = 0; i < 3; ++i)
		if (0x7 >> i & 1)
			MSBgm::setTrackVolume(i, 1.0f, 15, 3);
}

void MSound::setCategoryVOLsDefault(u16 param_1) { }

void MSound::setCategoryVOLs(u16 param_1, f32 param_2)
{
	u8 uVar2 = param_2 * 127.0f;

	if (uVar2 > 127)
		uVar2 = 127;

	for (u8 i = 0; i < 16; ++i) {
		if (MSGMSound->unk0->unk88.unk2[i] != 0 && param_1 >> i & 1)
			MSGMSound->setSeCategoryVolume(i, uVar2);
	}
}

bool MSound::resetAudioAll(u16 param_1)
{
	if (!unkD0)
		return true;

	f32 dVar2 = JAIGlobalParameter::getParamOutputGainUp();
	if (dVar2 <= 0.002f) {
		JASystem::Driver::setMixerLevel(0.802f, 0.0f);
		JASystem::AudioThread::stop();
		unkA8 = 0;
		unkD0 = 0;
		return true;
	}

	f32 fVar3 = dVar2 * std::powf(0.00020000001f, 1.0f / param_1);
	JASystem::Driver::setMixerLevel(0.802f, fVar3);
	JAIGlobalParameter::setParamOutputGainUp(fVar3);
	return false;
}

void MSound::stopAllSeInCategory(u8 category, u32 param_2) { }

void MSound::setCategoryAllVolume(u8 category, f32 volume, u32 param3,
                                  u8 param4)
{
}

void MSound::fadeOutAllSound(u32 param_1)
{
	unkA8 &= 1;

	for (u8 i = 0; i < JAIGlobalParameter::getParamSeCategoryMax(); ++i) {
		if (unk0->unk88.unk2[i] != 0 && i != 4) {
			for (JAISound* sound = unk0->unk1E8[i].unk4; sound != nullptr;
			     sound           = sound->unk30)
                sound->setVolume(0.0f, param_1, 2);
		}
	}

	for (u8 i = 0; i < 3; ++i)
		if (0x7 >> i & 1)
			MSBgm::setTrackVolume(i, 0.0f, param_1, 3);

	if (unkC4)
		unkC4->stop(param_1);
}

void MSound::stopAllSound()
{
	for (u8 i = 0; i < JAIGlobalParameter::getParamSeCategoryMax(); ++i) {
		if (unk0->unk88.unk2[i] != 0)
			JAIBasic::stopAllSe(i);
	}

	MSBgm::stopTrackBGMs(7, 0);
	if (unkC4)
		unkC4->stop(0);
}

void MSound::setSeExtParameter(JAISound* sound)
{
	if (sound != nullptr) {
		u32 id               = sound->unk8;
		JAISoundTable* table = JAIBasic::getInfoPointerFromID(id);
		JAISoundInfo* ptr    = (JAISoundInfo*)sound->unk3C;
		JAIBasic::getInfoFormat(table, id);
		f32 dVar5
		    = (ptr->unk0 & 0xC00000)
		          ? MSoundSESystem::MSRandVol::getRandomVolumeNormal(ptr->unk0)
		          : 1.0f;
		f32 fVar1 = dVar5 * (ptr->unkC / 127.0f);
		if (fVar1 > 1.0f)
			fVar1 = 1.0f;
		sound->setVolume(fVar1, 0, 1);
		sound->setPitch(ptr->unk8, 0, 1);
	}
}

void MSound::playTimer(u32 time)
{
	if (!(unkA8 & 1) ? false : true) {
		MSoundSESystem::MSoundSE::startSoundActorInner(
		    0x403A, nullptr, (JAIActor*)0xffffffff, 0, 4);

		if (time > 0x7530) {
			unk94 = 0x6e;
			return;
		}

		if (time > 0x3A98) {
			unk94 = 0x32;
			return;
		}

		if (time > 0x2710) {
			unk94 = 0x23;
			return;
		}

		if (time > 0x1388) {
			unk94 = 0x19;
			return;
		}

		if (time > 0x7D0) {
			unk94 = 10;
			return;
		}

		if (time > 0x3E8) {
			unk94 = 3;
			return;
		}

		unk94 = 0;
	}
}

u32 MSound::startMarioVoice(u32 param_1, s16 param_2, u8 param_3)
{
	if (((param_3 & 0x1) ? true : false) == 1)
		return 0;

	bool iVar6 = param_3 & 0x2 ? true : false;
	u32 iVar3  = param_3 & 0x2 ? true : false;

	bool r3 = 1;

	if (param_3 == 6) {
		switch (param_1) {
		case 0x78ab:
		case 0x78b1:
		case 0x78b6:
		case 0x78b9:
		case 0x78bf:
			JAIActor local_38(unkAC[iVar3].unk0, unkAC[iVar3].unk0,
			                  unkAC[iVar3].unk0, 0);
			MSoundSESystem::MSoundSE::startSoundActorInner(
			    0x898C, unk8C + iVar6, &local_38, 1, 4);
			return 0x898C;
		}

		return -1;
	}

	if (unk8C[iVar6] != nullptr) {
		switch (param_1) {
		case 0x78CF:
		case 0x78A8:
		case 0x7865:
		case 0x7852:
		case 0x78C7:
		case 0x78D9:
		case 0x78D3:
		case 0x78E0:
			r3 = 0;
			break;
		}

		if (!r3) {
			if (unk8C[0])
				return unk8C[0]->unk8;
			return -1;
		}
	}

	switch (param_1) {
	case 0xffff0003:
		if (param_3 != 2 && JALCalc::getRandom_0_1() < 0.5f)
			param_1 = 0x7806;
		if (param_1 + 0x10000 == 3)
			param_1 = 0x7817;
		break;

	case 0x7094:
	case 0x7849:
	case 0x7844:
	case 0x784F:
	case 0x786B:
	case 0x789E:
	case 0x78C1:
	case 0x78a3:
	case 0x78c4:
		break;

	case 0x7830:
		if (param_3 != 2 && JALCalc::getRandom_0_1() < 0.25f)
			param_1 = 0x7818;
		break;

	case 0x7833:
		if (param_3 != 2 && JALCalc::getRandom_0_1() < 0.25f)
			param_1 = 0x781C;
		break;

	case 0x783B:
		if (param_3 != 2 && JALCalc::getRandom_0_1() < 0.25f)
			param_1 = 0x781C;
		break;

	case 0x7852:
		if (param_2 <= 2)
			param_1 = 0x78EE;
		break;

	case 0x785D:
		if (param_3 != 2 && JALCalc::getRandom_0_1() < 0.15f)
			param_1 = 0x781B;
		break;

	case 0x7865:
		if (!(unkA8 & 2) ? false : true)
			MSoundSESystem::MSRandPlay::startSeRandPlay(0x7865, 0);
		if (unk8C[0] != nullptr)
			return unk8C[0]->unk8;
		return -1;
		break;

	case 0x7881:
		if (param_2 <= 2)
			param_1 = 0x78EE;
		break;

	case 0x7884:
		if (param_2 <= 2)
			param_1 = 0x78FB;
		break;

	case 0x788F:
		if (param_2 <= 2)
			param_1 = 0x78FE;
		break;

	case 0x7899:
		if (param_2 <= 2)
			param_1 = 0x78FB;
		break;

	case 0x78AB:
		if (param_2 <= 2) {
			param_1 = 0x7901;
			break;
		}
		if (param_3 != 2 && JALCalc::getRandom_0_1() < 0.3f)
			param_1 = 0x7807;
		break;

	case 0x78B1:
		if (param_2 <= 2) {
			param_1 = 0x7901;
			break;
		}
		if (param_3 != 2 && JALCalc::getRandom_0_1() < 0.7f)
			param_1 = 0x7803;
		break;

	case 0x78B6:
		if (param_2 <= 2) {
			param_1 = 0x7901;
			break;
		}
		if (param_3 != 2 && JALCalc::getRandom_0_1() < 0.5f)
			param_1 = 0x7800;
		break;

	case 0x78B9:
		if (param_2 <= 2) {
			param_1 = 0x7901;
			break;
		}
		if (param_3 != 2 && JALCalc::getRandom_0_1() < 0.5f)
			param_1 = 0x780A;
		break;

	case 0x78BF:
		if (param_2 <= 2)
			param_1 = 0x7906;
		break;

	case 0x78C7:
		if (param_2 <= 2) {
			param_1 = 0x78EE;
			break;
		}
		if (param_3 != 2 && JALCalc::getRandom_0_1() < 0.5f)
			param_1 = 0x780E;
		if (param_3 != 2 && JALCalc::getRandom_0_1() < 0.5f)
			param_1 = 0x7813;
		break;

	case 0x78E5:
		if (param_2 <= 2)
			param_1 = 0x790B;
		break;

	case -2:
		if (param_2 <= 2)
			param_1 = 0x790E;
		else
			param_1 = 0x78E5;
		break;
	}

	JAIActor local_48(unkAC[iVar3].unk0, unkAC[iVar3].unk0, unkAC[iVar3].unk0,
	                  0);
	MSoundSESystem::MSoundSE::startSoundActorInner(param_1, unk8C + iVar6,
	                                               &local_48, 1, 4);
	if (unk8C[iVar6] != nullptr) {
		if (param_3 == 2) {
			unk8C[iVar6]->setPortData(11, 1);
			unk8C[iVar6]->setPitch(1.2f, 0, 0);
			unk8C[iVar6]->setVolume(0.9f, 0, 0);
		} else {
			unk8C[iVar6]->setPortData(11, 0);
		}
	}

	u8 iVar62 = param_3 & 0x2 ? 1 : 0;
	if (unk8C[iVar62] != nullptr)
		return unk8C[iVar62]->unk8;

	return -1;
}

u32 MSound::getMarioVoiceID(u8 param_1)
{
	u8 iVar1 = param_1 & 2 ? 1 : 0;
	if (unk8C[iVar1])
		return unk8C[iVar1]->unk8;

	return -1;
}

void MSound::stopMarioVoice(u32 id, u8 param_2)
{
	u8 iVar1 = param_2 & 2 ? 1 : 0;
	if (unk8C[iVar1] != nullptr) {
		if (id != 0xffffffff) {
			if (id == unk8C[iVar1]->unk8)
				unk8C[iVar1]->stop(1);
		} else {
			unk8C[iVar1]->stop(1);
		}
	}
}

void* MSound::checkMarioVoicePlaying(u8 param_1)
{
	u8 iVar1 = param_1 & 2 ? 1 : 0;
	return unk8C[iVar1];
}

u32 MSound::getWallSound(u32 param_1, f32 param_2)
{
	if (param_2 < 15.0f) {
		if (param_1 == 0x1C)
			return 0x194B;
		else
			return 0x194A;
	}

	if (param_2 < 30.0f) {
		if (param_1 == 0x1C)
			return 0x1802;
		else
			return 0x1949;
	}

	if (param_1 == 0x1C)
		return 0x1803;
	else
		return 0x1948;
}

void MSound::startBeeSe(Vec* param_1, u32 param_2)
{
	if (param_2 > 3) {
		JAISound* sound = !(!(unkA8 & 1) ? false : true)
		                      ? nullptr
		                      : MSoundSESystem::MSoundSE::startSoundActor(
		                            0x2106, param_1, 0, nullptr, 0, 4);

		if (sound != nullptr)
			sound->setVolume(JALCalc::linearTransform(param_2, 3.0f, 50.0f,
			                                          0.0f, 1.0f, false),
			                 0, 0);
	}

	if (param_2 > 2) {
		if (!(unkA8 & 1) ? false : true)
			MSoundSESystem::MSoundSE::startSoundActor(0x2107, param_1, 0,
			                                          nullptr, 0, 4);
	} else if (param_2 == 2) {
		if (!(unkA8 & 1) ? false : true)
			MSoundSESystem::MSoundSE::startSoundActor(0x2108, param_1, 0,
			                                          nullptr, 0, 4);
	} else if (param_2 == 1) {
		if (!(unkA8 & 1) ? false : true)
			MSoundSESystem::MSoundSE::startSoundActor(0x2109, param_1, 0,
			                                          nullptr, 0, 4);
	}
}

void MSound::startSoundActorSpecial(u32 param_1, const Vec* param_2,
                                    f32 param_3, f32 param_4, u32 param_5,
                                    JAISound** param_6, u32 param_7, u8 param_8)
{
	if (gateCheck(param_1) && !JALSystem::gateCheckFunc(param_1, param_3)
	    && !JALSystem::gateCheckFunc(param_1, param_4)) {
		JAIActor actor(param_2, param_2, param_2, param_5);
		JAISound* sound = MSoundSESystem::MSoundSE::startSoundActorInner(
		    param_1, param_6, &actor, param_7, param_8);
		if (sound != nullptr) {
			switch (param_1) {
			case 0x212F:
				f32 local_40 = 1.0f;
				f32 local_44 = 1.0f;
				if (JALSeModData<JALSeModVolFunk>::calc(param_1, param_3,
				                                        &local_40))
					sound->setVolume(local_40, 0, 0);
				if (JALSeModDataGrp<JALSeModPitFGrp>::calcGrp(param_1, param_4,
				                                              &local_44))
					sound->setPitch(local_44, 0, 0);
				break;
			}
		}
	}
}

bool MSound::cameraLooksAtMario()
{
	for (u8 i = 0; i < 5; ++i) {
		u32 i2 = i;
		if (i2 == 0) {
			if (unkC8[i2] == 0)
				return false;
		} else if (unkC8[i2] == 1) {
			return false;
		}
	}

	return true;
}

bool MSound::gateCheck(u32 param_1)
{
	if (!(unkA8 & 1)) {
		u8 tmp = (param_1 >> 11 & 1) | (param_1 >> 24 & 0xC0);
		if (tmp == 0)
			return false;
	}

	if (!(unkA8 & 2)) {
		u8 tmp = (param_1 >> 11 & 1) | (param_1 >> 24 & 0xC0);
		if (tmp == 1)
			return false;
	}

	return true;
}

u32 MSound::getBstSwitch(u32 param_1)
{
	JAISoundInfo* info = MSGBasic->getSoundInfoFromID(param_1);
	if (!info)
		return -1;

	return info->unk0;
}

u32 MSound::getSwitch(u32 param_1, u32 param_2, u32 param3)
{
	u32 bstSwitch = getBstSwitch(param_1);
	return (bstSwitch & param_2) >> param3;
}

u32 MSound::getBstPitch(u32 param_1) { return 0; }
