#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIData.hpp>
#include <dolphin/mtx.h>

#include <JSystem/JAudio/JAInterface/JAIConst.hpp>

JAIBasic::JAIBasic() { }

void JAIBasic::initDriver(JKRSolidHeap* heap, u32 param1, u8 param2) { }

void JAIBasic::initInterface(u8 param) { }

void JAIBasic::initInterfaceMain() { }

void JAIBasic::initHeap() { }

void JAIBasic::initReadFile() { }

void JAIBasic::checkInitListFile() { }

void JAIBasic::loadDVDFile(char* filename) { }

void JAIBasic::checkInitDataFile() { }

void JAIBasic::loadTmpDVDFile(char* filename, unsigned char** buffer) { }

void JAIBasic::checkInitDataOnMemory() { }

void JAIBasic::transInitDataFile(unsigned char* buffer, u32 size) { }

void JAIBasic::allocHeap(u32 size) { }

void JAIBasic::initBankWave() { }

void JAIBasic::deleteTmpDVDFile(unsigned char** buffer) { }

void JAIBasic::setWaveScene() { }

void JAIBasic::initAllocParameter() { }

void JAIBasic::initNullData() { }

void JAIBasic::initSeqsLoadArea() { }

void JAIBasic::startSeSequence() { }

void JAIBasic::setCameraInfo(VecPtr pos, VecPtr dir, MtxPtr mtx, u32 param) { }

void JAIBasic::initStream() { }

void JAIBasic::setRegisterTrackCallback() { }

void JAIBasic::initAudioThread(JKRSolidHeap* heap, u32 param1, u8 param2) { }

void JAIBasic::bootDSP() { }

void JAIBasic::readInitSoundData() { }

void JAIBasic::loadFirstStayWave() { }

void JAIBasic::loadSecondStayWave() { }

void JAIBasic::setSceneSetFinishCallback(s32 param1, s32 param2) { }

void JAIBasic::finishSceneSet(u32 param) { }

void JAIBasic::loadSceneWave(s32 param1, s32 param2) { }

void JAIBasic::checkSceneWaveOnMemory(s32 param1, s32 param2) { }

void JAIBasic::loadGroupWave(s32 param1, s32 param2) { }

void JAIBasic::getWaveGroupNumber(s32 param) { }

void JAIBasic::getWaveLoadStatus(s32 param) { }

void JAIBasic::checkAllWaveLoadStatus() { }

void JAIBasic::setInitFileLoadSwitch(u8 flag) { }

void JAIBasic::startFrameInterfaceWork() { }

void JAIBasic::processFrameWork() { }

void JAIBasic::checkStream() { }

void JAIBasic::checkDummyPositionBuffer() { }

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
}

void JAIBasic::startSoundActorReturnHandle(u32 id, JAIActor* actor, u32 param,
                                           u8 flag)
{
}

void JAIBasic::startSoundDirectID(u32 id, JAISound** sound, JAIActor* actor,
                                  u32 param, u8 flag)
{
}

void JAIBasic::startSoundIndirectID(u32 id, JAISound** sound, JAIActor* actor,
                                    u32 param, u8 flag)
{
}

void JAIBasic::startSoundBasic(u32 id, JAISound** sound, JAIActor* actor,
                               u32 param, u8 flag, void* data)
{
}

void JAIBasic::getPlayingSoundHandle(JAISound** sound, u32 param) { }

void JAIBasic::stopSoundHandle(JAISound* sound, u32 param) { }

u32 JAIBasic::changeIDToCategory(u32 id) { return id >> 0xc & 0xff; }

void JAIBasic::stopPlayingObjectSe(void* obj) { }

void JAIBasic::stopPlayingIDObjectSe(u32 id, void* obj) { }

void JAIBasic::stopPlayingCategorySe(u8 category) { }

void JAIBasic::stopPlayingCategoryObjectSe(u8 category, void* obj) { }

void JAIBasic::stopAllSe(void* obj) { }

void JAIBasic::stopAllSe(u8 param) { }

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

void JAIBasic::getControllerHandle(JAILinkBuffer* buffer) { }

void JAIBasic::releaseControllerHandle(JAILinkBuffer* buffer, JAISound* sound)
{
}

void JAIBasic::getStreamParameter() { }

void JAIBasic::releaseStreamParameterPointer(JAIStreamParameter* param) { }

void JAIBasic::getSeqParametermeterPointer() { }

void JAIBasic::releaseSeqParameterPointer(JAISeqParameter* param) { }

void JAIBasic::getSeParametermeterPointer() { }

void JAIBasic::releaseSeParameterPointer(JAISeParameter* param) { }

void JAIBasic::getDummyVecPointer() { }

void JAIBasic::releaseDummyVecPointer(JAIDummyVec* vec) { }

void JAIBasic::getGameFrameCounter() { }

void JAIBasic::setPauseFlagAll(u8 flag) { }

void JAIBasic::checkPlayingSoundTrack(u32 param) { }

void JAIBasic::changeSoundScene(u32 scene) { }

void JAIBasic::getMapInfoFxline(u32 param) { }

u32 JAIBasic::getMapInfoGround(u32 param) { }

void JAIBasic::getMapInfoFxParameter(u32 param) { }

void JAIBasic::allocDvdBuffer(unsigned char* buffer, u32 param1, u32 param2) { }

void JAIBasic::deallocDvdBuffer(unsigned char* buffer) { }

void JAIBasic::getSeInfoMode() { }

void JAIBasic::getSeInfoStartPointer() { }

void JAIBasic::getSeInfoCategoryMax() { }

void JAIBasic::getInfoPointerFromID(u32 id) { }

void JAIBasic::getInfoFormat(JAISoundTable* table, u32 id) { }

void JAIBasic::setSeCancelSwitch(u8 param1, u8 param2) { }

void JAIBasic::setSeCategoryVolume(u8 category, u8 volume) { }

void JAIBasic::setParameterSeqSync(JASystem::TTrack* track, u16 param) { }

void JAIBasic::getSoundInfoFromID(u32 id) { }

void JAIBasic::getSeqTrackNumber(void* param) { }

void JAIBasic::getSoundPrioity(void* param) { }

void JAIBasic::getSoundSwBit(void* param) { }

void JAIBasic::setSeExtParameter(JAISound* sound) { }

void JAIBasic::routeToTrack(u32 param) { }

JAISound* JAIBasic::makeSound(u32 param) { }

void JAIBasic::allocStreamBuffer(void* buffer, s32 size) { }

void JAIBasic::deallocStreamBuffer() { }

void JAIBasic::loadArcSeqData(u32 param, bool flag) { }
