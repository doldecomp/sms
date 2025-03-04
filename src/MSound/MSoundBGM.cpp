#include <MSound/MSoundBGM.hpp>

JASystem::TTrack* MSBgm::smBgmInTrack[4];
f32 MSBgm::smMainVolume = 0.75f;

void MSBgm::init() { }

void MSBgm::startBGM(u32 param) { }

void MSBgm::stopBGM(u32 param1, u32 param2) { }

void MSBgm::stopTrackBGM(u8 param1, u32 param2) { }

void MSBgm::stopTrackBGMs(u8 param1, u32 param2) { }

void MSBgm::setVolume(u32 param1, f32 param2, u32 param3, u8 param4) { }

void MSBgm::setTrackVolume(u8 param1, f32 param2, u32 param3, u8 param4) { }

void MSBgm::setSeqTrackVolume(u8 param1, u8 param2, f32 param3, u32 param4) { }

void MSBgm::setPan(u8 param1, f32 param2, u32 param3, u8 param4) { }

void MSBgm::setDolby(u8 param1, f32 param2, u32 param3, u8 param4) { }

void MSBgm::setPause(u8 param1, bool param2) { }

void MSBgm::setSeqTrackMute(u8 param1, u8 param2, bool param3) { }

void MSBgm::setSeqTRACKsMute(u8 param1, bool param2, u16 param3) { }

void MSBgm::setSeqTRACKsMuteH(JAISound* param1, bool param2, u16 param3) { }

void MSBgm::setStageBgmYoshiPercussion(bool param) { }

bool MSBgm::checkPlaying(u32 param) { return false; }

u32 MSBgm::getSceneNo(u32 param) { return 0; }

JAISound* MSBgm::getHandle(u8 param) { return nullptr; }

JAISound* MSBgm::getJASTrack(JAISound* param1, u8 param2) { return nullptr; }
