#include <MSound/MSRandVol.hpp>
#include <MSound/MSound.hpp>

JAISound* MSGMSound = 0;
JAIBasic* MSGBasic  = 0;

namespace MSoundSESystem {

f32 MSRandVol::getRandomVolumeNormal(u32) { return 0.0f; }

} // namespace MSoundSESystem

void MSSeCallBack::setParameterSeqSync(JASystem::TTrack* track, u16 param) { }

void MSSeCallBack::setWaterFilter(u16 param) { }

void MSSeCallBack::setWaterCameraFir(bool enabled) { }

u8 MSSeCallBack::smTrackCategory;
u8 MSSeCallBack::smPolifonic;
u8 MSSeCallBack::smWaterFilter;

void MSLoadWave::loadWaveBackword(int param1, int param2) { }

void MSLoadWave::loadWaveBackword(JASystem::WaveArcLoader::TObject* obj) { }

MSound* MSound::getMSound() { return nullptr; }

void MSound::initSound() { }

void MSound::mainLoop() { }

void MSound::exitStage() { }

void MSound::enterStage(MS_SCENE_WAVE wave, u8 param2, u8 param3) { }

void MSound::loadWave(MS_SCENE_WAVE wave) { }

void MSound::loadGroupWave(s32 param1, s32 param2) { }

void MSound::cleanUpAramWave(u8 param) { }

bool MSound::checkWaveOnAram(MS_SCENE_WAVE wave) { return false; }

bool MSound::checkSeqOnMemory(u32 id) { return false; }

void MSound::stopAllSound() { }

void MSound::fadeOutAllSound(u32 fadeTime) { }

void MSound::stopAllSeInCategory(u8 category, u32 param2) { }

void MSound::setCategoryAllVolume(u8 category, f32 volume, u32 param3,
                                  u8 param4)
{
}

void MSound::setCategoryVOLs(u16 param1, f32 volume) { }

void MSound::setCategoryVOLsDefault(u16 param) { }

void MSound::pauseOn(bool param) { }

void MSound::pauseOff(u8 param) { }

void MSound::talkModeIn(bool param) { }

void MSound::talkModeOut() { }

void MSound::demoModeIn(u16 param1, bool param2) { }

void MSound::demoModeOut(bool param) { }

void MSound::setPlayerInfo(Vec* pos, Vec* velocity, MtxPtr mtx, bool param4) { }

void MSound::setCameraInfo(Vec* pos, Vec* target, MtxPtr mtx, u32 param4) { }

f32 MSound::getDistFromCamera(Vec* pos) { return 0.0f; }

bool MSound::cameraLooksAtMario() { return false; }

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

JAISound* MSound::startSoundActorSpecial(u32 id, const Vec* pos, f32 param3,
                                         f32 param4, u32 param5,
                                         JAISound** soundPtr, u32 param7,
                                         u8 param8)
{
	return nullptr;
}

JAISound* MSound::startBeeSe(Vec* pos, u32 id) { return nullptr; }

void MSound::startMarioVoice(u32 id, s16 param2, u8 param3) { }

void MSound::stopMarioVoice(u32 id, u8 param2) { }

u8 MSound::getMarioVoiceID(u8 param) { return 0; }

bool MSound::checkMarioVoicePlaying(u8 param) { return false; }

void MSound::setRegisterTrackCallback() { }

void MSound::setSeExtParameter(JAISound* sound) { }

JAISound* MSound::makeSound(u32 id) { return nullptr; }

void MSound::playTimer(u32 time) { }

void MSound::requestShineAppearFanfare() { }

f32 MSound::getWallSound(u32 param1, f32 param2) { return 0.0f; }

u32 MSound::getBstPitch(u32 param) { return 0; }

u32 MSound::getBstSwitch(u32 param) { return 0; }

u32 MSound::getSwitch(u32 param1, u32 param2, u32 param3) { return 0; }

bool MSound::gateCheck(u32 param) { return false; }

void MSound::resetAudioAll(u16 param) { }

MSound::MSound(JKRHeap* heap1, JKRHeap* heap2, u32 param3, u8* param4,
               u8* param5, u32 param6)
{
}
