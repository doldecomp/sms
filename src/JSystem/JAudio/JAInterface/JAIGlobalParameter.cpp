
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>

u8 JAIGlobalParameter::distanceParameterMoveTime   = 0x03;
u8 JAIGlobalParameter::audioSystemThreadPriority   = 0x02;
u8 JAIGlobalParameter::audioDvdThreadPriority      = 0x03;
u8 JAIGlobalParameter::seqMuteVolumeSePlay         = 0x28;
u16 JAIGlobalParameter::seDistanceFxParameter      = 0x96;
u32 JAIGlobalParameter::soundSceneMax              = 0x02;
u32 JAIGlobalParameter::seRegistMax                = 0x14;
u32 JAIGlobalParameter::seTrackMax                 = 0x20;
u32 JAIGlobalParameter::seqTrackMax                = 0x20;
u32 JAIGlobalParameter::seqPlayTrackMax            = 0x08;
u32 JAIGlobalParameter::seqControlBufferMax        = 0x10;
u32 JAIGlobalParameter::streamControlBufferMax     = 0x08;
u32 JAIGlobalParameter::streamParameterBufferMax   = 0x10;
u32 JAIGlobalParameter::autoHeapMax                = 0x02;
u32 JAIGlobalParameter::stayHeapMax                = 0x02;
u32 JAIGlobalParameter::autoHeapRoomSize           = 0xc000;
u32 JAIGlobalParameter::stayHeapSize               = 0x10000;
char* JAIGlobalParameter::seInfoFileName           = "Seqs/JaiSeInf.bst";
char* JAIGlobalParameter::seqInfoFileName          = "Seqs/JaiSqInf.bst";
char* JAIGlobalParameter::streamInfoFileName       = "Seqs/JaiStInf.bst";
char* JAIGlobalParameter::initDataFileName         = "JaiInit.aaf";
char* JAIGlobalParameter::wavePath                 = "Banks/";
char* JAIGlobalParameter::sequenceArchivesPath     = "Seqs/";
char* JAIGlobalParameter::streamPath               = "Stream/";
char* JAIGlobalParameter::sequenceArchivesFileName = "JaiArcS.hed";
f32 JAIGlobalParameter::inputGainDown              = 0.5f;
f32 JAIGlobalParameter::outputGainUp               = 1.2f;
f32 JAIGlobalParameter::distanceMax                = 5000.0f;
f32 JAIGlobalParameter::maxVolumeDistance          = 1000.0f;
f32 JAIGlobalParameter::seDolbyCenterValue         = 30.0f;
f32 JAIGlobalParameter::seDolbyFrontDistanceMax    = -300.0f;
f32 JAIGlobalParameter::seDolbyBehindDistanceMax   = 1000.0f;
u32 JAIGlobalParameter::dopplarMoveTime            = 0x0f;
u32 JAIGlobalParameter::dummyPositionMax           = 0x0a;
u32 JAIGlobalParameter::dummyPositionLifeTime      = 0x00; // Unused
u32 JAIGlobalParameter::seqMuteMoveSpeedSePlay     = 0x03;
u32 JAIGlobalParameter::audioCameraMax             = 0x01;
s32 JAIGlobalParameter::systemTrackMax             = 0x100;
s32 JAIGlobalParameter::systemRootTrackMax         = 0x10;
f32 JAIGlobalParameter::panDistanceMax             = 500.0f;
f32 JAIGlobalParameter::panDistance2Max            = 1000.0f;
f32 JAIGlobalParameter::panAngleParameter          = 12.0f;
f32 JAIGlobalParameter::panAngleParameter2         = 2.4f;
f32 JAIGlobalParameter::dopplarParameter           = 3200.0f;
u16 JAIGlobalParameter::seDistanceWaitMax          = 0x0c;
f32 JAIGlobalParameter::seDistancepitchMax         = 0.2f;
u16 JAIGlobalParameter::seDefaultFx                = 0;
u32 JAIGlobalParameter::interfaceHeapSize          = 0;
char* JAIGlobalParameter::audioResPath             = nullptr;
f32 JAIGlobalParameter::minDistanceVolume          = 0.0f;

void JAIGlobalParameter::setParamSoundOutputMode(u32) { }
void JAIGlobalParameter::setParamAudioSystemThreadPriority(u8) { }
void JAIGlobalParameter::setParamAudioDvdThreadPriority(u8) { }
void JAIGlobalParameter::setParamInitFileLoadSwitch(u8) { }
void JAIGlobalParameter::setParamInitDataPointer(void*) { }
void JAIGlobalParameter::setParamInterfaceHeapSize(u32) { }
void JAIGlobalParameter::setParamSoundSceneMax(u32) { }
void JAIGlobalParameter::setParamSeRegistMax(u32) { }
void JAIGlobalParameter::setParamSeTrackMax(u32) { }
void JAIGlobalParameter::setParamSeqPlayTrackMax(u32) { }
void JAIGlobalParameter::setParamSeqControlBufferMax(u32) { }
void JAIGlobalParameter::setParamStreamControlBufferMax(u32) { }
void JAIGlobalParameter::setParamStreamParameterBufferMax(u32) { }
void JAIGlobalParameter::setParamAutoHeapMax(u32) { }
void JAIGlobalParameter::setParamStayHeapMax(u32) { }
void JAIGlobalParameter::setParamInputGainDown(f32) { }
void JAIGlobalParameter::setParamOutputGainUp(f32) { }
void JAIGlobalParameter::setParamDistanceMax(f32) { }
void JAIGlobalParameter::setParamMaxVolumeDistance(f32) { }
void JAIGlobalParameter::setParamMinDistanceVolume(f32) { }
void JAIGlobalParameter::setParamSeDistanceFxParameter(u16) { }
void JAIGlobalParameter::setParamInitDataLoadOffFlag(bool) { }
void JAIGlobalParameter::setParamStreamUseOffFlag(bool) { }
void JAIGlobalParameter::setParamStreamDecodedBufferBlocks(u32) { }
void JAIGlobalParameter::setParamStreamInsideBufferCut(bool) { }
void JAIGlobalParameter::setParamAutoHeapRoomSize(u32) { }
void JAIGlobalParameter::setParamStayHeapSize(u32) { }
void JAIGlobalParameter::setParamSeDolbyCenterValue(u8) { }
void JAIGlobalParameter::setParamSeDolbyFrontDistanceMax(f32) { }
void JAIGlobalParameter::setParamSeDolbyBehindDistanceMax(f32) { }
void JAIGlobalParameter::setParamInitDataFileName(char*) { }
void JAIGlobalParameter::setParamSeInfoFileName(char*) { }
void JAIGlobalParameter::setParamSeqInfoFileName(char*) { }
void JAIGlobalParameter::setParamStreamInfoFileName(char*) { }
void JAIGlobalParameter::setParamWavePath(char*) { }
void JAIGlobalParameter::setParamSequenceArchivesPath(char*) { }
void JAIGlobalParameter::setParamStreamPath(char*) { }
void JAIGlobalParameter::setParamAudioResPath(char*) { }
void JAIGlobalParameter::setParamSequenceArchivesFileName(char*) { }
void JAIGlobalParameter::setParamSeqEntryCancelFlag(bool) { }
void JAIGlobalParameter::setParamStreamEntryCancelFlag(bool) { }
void JAIGlobalParameter::setParamDopplarMoveTime(u32) { }
void JAIGlobalParameter::setParamDistanceParameterMoveTime(u8) { }
void JAIGlobalParameter::setParamDummyPositionLifeTime(u32) { }
void JAIGlobalParameter::setParamDummyPositionMax(u32) { }
void JAIGlobalParameter::setParamSeqMuteVolumeSePlay(u8) { }
void JAIGlobalParameter::setParamSeqMuteMoveSpeedSePlay(u32) { }
void JAIGlobalParameter::setParamAudioCameraMax(u32) { }
void JAIGlobalParameter::setParamSystemTrackMax(s32) { }
void JAIGlobalParameter::setParamSystemRootTrackMax(s32) { }
void JAIGlobalParameter::setParamPanDistanceMax(f32) { }
void JAIGlobalParameter::setParamPanAngleParameter(f32) { }
void JAIGlobalParameter::setParamDopplarParameter(f32) { }
void JAIGlobalParameter::setParamSeDistanceWaitMax(u16) { }
void JAIGlobalParameter::setParamSeDistancepitchMax(f32) { }

u8 JAIGlobalParameter::getParamAudioSystemThreadPriority() { return 0; }
u8 JAIGlobalParameter::getParamAudioDvdThreadPriority() { return 0; }
u8 JAIGlobalParameter::getParamInitFileLoadSwitch() { return 0; }
void* JAIGlobalParameter::getParamInitDataPointer() { return nullptr; }
u32 JAIGlobalParameter::getParamInterfaceHeapSize() { return 0; }
u32 JAIGlobalParameter::getParamSeCategoryMax() { return 0; }
u32 JAIGlobalParameter::getParamSoundSceneMax() { return 0; }
u32 JAIGlobalParameter::getParamSeRegistMax() { return 0; }
u32 JAIGlobalParameter::getParamSeTrackMax() { return 0; }
u32 JAIGlobalParameter::getParamSeqTrackMax() { return 0; }
u32 JAIGlobalParameter::getParamSeqControlBufferMax() { return 0; }
u32 JAIGlobalParameter::getParamStreamControlBufferMax() { return 0; }
u32 JAIGlobalParameter::getParamStreamParameterBufferMax() { return 0; }
u32 JAIGlobalParameter::getParamAutoHeapMax() { return 0; }
u32 JAIGlobalParameter::getParamStayHeapMax() { return 0; }
u32 JAIGlobalParameter::getParamSeqPlayTrackMax() { return 0; }
f32 JAIGlobalParameter::getParamInputGainDown() { return 0.0f; }
f32 JAIGlobalParameter::getParamOutputGainUp() { return 0.0f; }
f32 JAIGlobalParameter::getParamDistanceMax() { return 0.0f; }
f32 JAIGlobalParameter::getParamMaxVolumeDistance() { return 0.0f; }
f32 JAIGlobalParameter::getParamMinDistanceVolume() { return 0.0f; }
u32 JAIGlobalParameter::getParamSoundOutputMode() { return 0; }
u16 JAIGlobalParameter::getParamSeDistanceFxParameter() { return 0; }
bool JAIGlobalParameter::getParamInitDataLoadOffFlag() { return false; }
bool JAIGlobalParameter::getParamStreamUseOffFlag() { return false; }
u32 JAIGlobalParameter::getParamAutoHeapRoomSize() { return 0; }
u32 JAIGlobalParameter::getParamStayHeapSize() { return 0; }
u8 JAIGlobalParameter::getParamSeDolbyCenterValue() { return 0; }
f32 JAIGlobalParameter::getParamSeDolbyFrontDistanceMax() { return 0.0f; }
f32 JAIGlobalParameter::getParamSeDolbyBehindDistanceMax() { return 0.0f; }
char* JAIGlobalParameter::getParamInitDataFileName() { return nullptr; }
char* JAIGlobalParameter::getParamSeInfoFileName() { return nullptr; }
char* JAIGlobalParameter::getParamSeqInfoFileName() { return nullptr; }
char* JAIGlobalParameter::getParamStreamInfoFileName() { return nullptr; }
char* JAIGlobalParameter::getParamWavePath() { return nullptr; }
char* JAIGlobalParameter::getParamSequenceArchivesPath() { return nullptr; }
char* JAIGlobalParameter::getParamStreamPath() { return nullptr; }
char* JAIGlobalParameter::getParamAudioResPath() { return nullptr; }
char* JAIGlobalParameter::getParamSequenceArchivesFileName() { return nullptr; }
bool JAIGlobalParameter::getParamSeqEntryCancelFlag() { return false; }
bool JAIGlobalParameter::getParamStreamEntryCancelFlag() { return false; }
u32 JAIGlobalParameter::getParamDopplarMoveTime() { return 0; }
u8 JAIGlobalParameter::getParamDistanceParameterMoveTime() { return 0; }
u32 JAIGlobalParameter::getParamDummyPositionLifeTime() { return 0; }
u32 JAIGlobalParameter::getParamDummyPositionMax() { return 0; }
u8 JAIGlobalParameter::getParamSeqMuteVolumeSePlay() { return 0; }
u32 JAIGlobalParameter::getParamSeqMuteMoveSpeedSePlay() { return 0; }
u32 JAIGlobalParameter::getParamAudioCameraMax() { return 0; }
s32 JAIGlobalParameter::getParamSystemTrackMax() { return 0; }
s32 JAIGlobalParameter::getParamSystemRootTrackMax() { return 0; }
