#include "JSystem/JAudio/JASystem/JASTrack.hpp"

namespace JASystem {

TSeqParser* TTrack::sParser                = nullptr;
u8 TTrack::sUpdateSyncMode                 = 0;
u16 (*TTrack::sCallBackFunc)(TTrack*, u16) = nullptr;
u32 TTrack::sOscTable                      = 0;

TTrack::TTrack() { }

TTrack::~TTrack() { }

void TTrack::reset() { }

void TTrack::initTrack(void* data, u32 size, TTrack* parent) { }

void TTrack::initTimed() { }

void TTrack::openTrack(u8 param) { }

void TTrack::closeTrack() { }

void TTrack::startTrack(TTrack* parent, u8 param1, u8 param2, u32 param3) { }

void TTrack::startSeq() { }

void TTrack::stopSeq() { }

void TTrack::stopSeqMain() { }

void TTrack::allNoteOff() { }

void TTrack::setSeqData(u8* data, s32 size, Player::SEQ_PLAYMODE mode) { }

void TTrack::updateSeq(u32 param, bool flag) { }

void TTrack::updateTrack(u32 param) { }

void TTrack::updateTrackAll() { }

void TTrack::updateTempo() { }

void TTrack::mainProc() { }

void TTrack::noteOn(u8 note, s32 velocity, s32 length, s32 delay) { }

void TTrack::noteOff(u8 note, u16 release) { }

void TTrack::gateOn(u8 note, s32 velocity, s32 length, s32 delay) { }

void TTrack::checkNoteStop(s32 param) { }

void TTrack::releaseChannelAll() { }

void TTrack::flushAll() { }

void TTrack::flushRelease() { }

void TTrack::muteTrack(u8 flag) { }

void TTrack::muteChildTracks(u16 flags) { }

void TTrack::pauseTrack(u8 flag) { }

void TTrack::pauseTrackAll() { }

void TTrack::unPauseTrack(u8 flag) { }

void TTrack::unPauseTrackAll() { }

void TTrack::setInterrupt(u16 interrupt) { }

void TTrack::tryInterrupt() { }

void TTrack::setInnerMemory(u8 param) { }

void TTrack::setBankNumber(u8 bank) { }

void TTrack::assignExtBuffer(TOuterParam* buffer) { }

void TTrack::setPanSwitchExt(u8 param1, u8 param2) { }

void TTrack::setPanSwitchParent(u8 param1, u8 param2) { }

void TTrack::setTrackExtPanPower(f32 power) { }

void TTrack::setExtFirFilterD(s16* filter) { }

void TTrack::routeTrack(u32 route) { }

void TTrack::writePortApp(u32 port, u16 value) { }

void TTrack::readPortApp(u32 port, u16* value) { }

void TTrack::writePortAppDirect(u32 port, u16 value) { }

void TTrack::readPortAppDirect(u32 port, u16* value) { }

void TTrack::checkExportApp(u32 port) { }

void TTrack::checkImportApp(u32 port) { }

void TTrack::loadTbl(u32 param1, u32 param2, u32 param3) { }

void TTrack::readReg32(u8 reg) { }

void TTrack::exchangeRegisterValue(u8 reg) { }

void TTrack::readRegDirect(u8 reg) { }

void TTrack::writeRegDirect(u8 reg, u16 value) { }

void TTrack::writeTimeParam(u8 param) { }

void TTrack::writeRegParam(u8 param) { }

void TTrack::seqTimeToDspTime(s32 time, u8 param) { }

void TTrack::panCalc(f32 param1, f32 param2, f32 param3, u8 param4) { }

void TTrack::rootCallback(void* param) { }

void TTrack::updateSyncSw(u8 param) { }

void TTrack::incSelfOsc() { }

void TTrack::oscSetupSimple(u8 param) { }

void TTrack::oscSetupSimpleEnv(u8 param, u32 env) { }

void TTrack::oscSetupFull(u8 param1, u32 param2, u32 param3) { }

void TTrack::oscUpdateParam(u8 param, f32 value) { }

u8 TTrack::getBank() const { return 0; }

u8 TTrack::getProgram() const { return 0; }

f32 TTrack::getVolume() const { return 0.0f; }

f32 TTrack::getPitch() const { return 0.0f; }

f32 TTrack::getPan() const { return 0.0f; }

f32 TTrack::getFxmix() const { return 0.0f; }

f32 TTrack::getDolby() const { return 0.0f; }

u8 TTrack::getNoteStatus(u8 note) { return 0; }

void TTrack::registerTrackCallback(u16 (*callback)(TTrack*, u16)) { }

} // namespace JASystem
