
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
void JAIGlobalParameter::setParamAudioSystemThreadPriority(u8 value)
{
	audioSystemThreadPriority = value;
}
void JAIGlobalParameter::setParamAudioDvdThreadPriority(u8 value)
{
	audioDvdThreadPriority = value;
}
void JAIGlobalParameter::setParamInitFileLoadSwitch(u8) { }
void JAIGlobalParameter::setParamInitDataPointer(void*) { }
void JAIGlobalParameter::setParamInterfaceHeapSize(u32 value)
{
	interfaceHeapSize = value;
}
void JAIGlobalParameter::setParamSoundSceneMax(u32 value)
{
	soundSceneMax = value;
}
void JAIGlobalParameter::setParamSeRegistMax(u32 value) { seRegistMax = value; }
void JAIGlobalParameter::setParamSeTrackMax(u32 value) { seTrackMax = value; }
void JAIGlobalParameter::setParamSeqPlayTrackMax(u32 value)
{
	seqPlayTrackMax     = value;
	seqControlBufferMax = value * 2;
}
void JAIGlobalParameter::setParamSeqControlBufferMax(u32 value)
{
	seqControlBufferMax = value;
}
void JAIGlobalParameter::setParamStreamControlBufferMax(u32 value)
{
	streamControlBufferMax = value;
}
void JAIGlobalParameter::setParamStreamParameterBufferMax(u32 value)
{
	streamParameterBufferMax = value;
}
void JAIGlobalParameter::setParamAutoHeapMax(u32 value) { autoHeapMax = value; }
void JAIGlobalParameter::setParamStayHeapMax(u32 value) { stayHeapMax = value; }
void JAIGlobalParameter::setParamInputGainDown(f32 value)
{
	inputGainDown = value;
}
void JAIGlobalParameter::setParamOutputGainUp(f32 value)
{
	outputGainUp = value;
}
void JAIGlobalParameter::setParamDistanceMax(f32 value) { distanceMax = value; }
void JAIGlobalParameter::setParamMaxVolumeDistance(f32 value)
{
	maxVolumeDistance = value;
}
void JAIGlobalParameter::setParamMinDistanceVolume(f32 value)
{
	minDistanceVolume = value;
}
void JAIGlobalParameter::setParamSeDistanceFxParameter(u16 value)
{
	seDistanceFxParameter = value;
}
void JAIGlobalParameter::setParamInitDataLoadOffFlag(bool) { }
void JAIGlobalParameter::setParamStreamUseOffFlag(bool) { }
void JAIGlobalParameter::setParamStreamDecodedBufferBlocks(u32) { }
void JAIGlobalParameter::setParamStreamInsideBufferCut(bool) { }
void JAIGlobalParameter::setParamAutoHeapRoomSize(u32 value)
{
	autoHeapRoomSize = value;
}
void JAIGlobalParameter::setParamStayHeapSize(u32 value)
{
	stayHeapSize = value;
}
void JAIGlobalParameter::setParamSeDolbyCenterValue(u8 value)
{
	seDolbyCenterValue = value;
}
void JAIGlobalParameter::setParamSeDolbyFrontDistanceMax(f32 value)
{
	seDolbyFrontDistanceMax = value;
}
void JAIGlobalParameter::setParamSeDolbyBehindDistanceMax(f32 value)
{
	seDolbyBehindDistanceMax = value;
}
void JAIGlobalParameter::setParamInitDataFileName(char* value)
{
	initDataFileName = value;
}
void JAIGlobalParameter::setParamSeInfoFileName(char* value)
{
	seInfoFileName = value;
}
void JAIGlobalParameter::setParamSeqInfoFileName(char* value)
{
	seqInfoFileName = value;
}
void JAIGlobalParameter::setParamStreamInfoFileName(char* value)
{
	streamInfoFileName = value;
}
void JAIGlobalParameter::setParamWavePath(char* value) { wavePath = value; }
void JAIGlobalParameter::setParamSequenceArchivesPath(char* value)
{
	sequenceArchivesPath = value;
}
void JAIGlobalParameter::setParamStreamPath(char* value) { streamPath = value; }
void JAIGlobalParameter::setParamAudioResPath(char* value)
{
	audioResPath = value;
}
void JAIGlobalParameter::setParamSequenceArchivesFileName(char* value)
{
	sequenceArchivesFileName = value;
}
void JAIGlobalParameter::setParamSeqEntryCancelFlag(bool) { }
void JAIGlobalParameter::setParamStreamEntryCancelFlag(bool) { }
void JAIGlobalParameter::setParamDopplarMoveTime(u32 value)
{
	dopplarMoveTime = value;
}
void JAIGlobalParameter::setParamDistanceParameterMoveTime(u8 value)
{
	distanceParameterMoveTime = value;
}
void JAIGlobalParameter::setParamDummyPositionLifeTime(u32 value)
{
	dummyPositionLifeTime = value;
}
void JAIGlobalParameter::setParamDummyPositionMax(u32 value)
{
	dummyPositionMax = value;
}
void JAIGlobalParameter::setParamSeqMuteVolumeSePlay(u8 value)
{
	seqMuteVolumeSePlay = value;
}
void JAIGlobalParameter::setParamSeqMuteMoveSpeedSePlay(u32 value)
{
	seqMuteMoveSpeedSePlay = value;
}
void JAIGlobalParameter::setParamAudioCameraMax(u32 value)
{
	audioCameraMax = value;
}
void JAIGlobalParameter::setParamSystemTrackMax(s32 value)
{
	systemTrackMax = value;
}
void JAIGlobalParameter::setParamSystemRootTrackMax(s32 value)
{
	systemRootTrackMax = value;
}
void JAIGlobalParameter::setParamPanDistanceMax(f32 value)
{
	panDistanceMax = value;
}
void JAIGlobalParameter::setParamPanAngleParameter(f32 value)
{
	panAngleParameter = value;
}
void JAIGlobalParameter::setParamDopplarParameter(f32 value)
{
	dopplarParameter = value;
}
void JAIGlobalParameter::setParamSeDistanceWaitMax(u16 value)
{
	seDistanceWaitMax = value;
}
void JAIGlobalParameter::setParamSeDistancepitchMax(f32 value)
{
	seDistancepitchMax = value;
}

u8 JAIGlobalParameter::getParamAudioSystemThreadPriority()
{
	return audioSystemThreadPriority;
}
u8 JAIGlobalParameter::getParamAudioDvdThreadPriority()
{
	return audioDvdThreadPriority;
}
u8 JAIGlobalParameter::getParamInitFileLoadSwitch() { return 0; }
void* JAIGlobalParameter::getParamInitDataPointer() { return nullptr; }
u32 JAIGlobalParameter::getParamInterfaceHeapSize()
{
	return interfaceHeapSize;
}
u32 JAIGlobalParameter::getParamSeCategoryMax() { return 0; }
u32 JAIGlobalParameter::getParamSoundSceneMax() { return soundSceneMax; }
u32 JAIGlobalParameter::getParamSeRegistMax() { return seRegistMax; }
u32 JAIGlobalParameter::getParamSeTrackMax() { return seTrackMax; }
u32 JAIGlobalParameter::getParamSeqTrackMax() { return seqTrackMax; }
u32 JAIGlobalParameter::getParamSeqControlBufferMax()
{
	return seqControlBufferMax;
}
u32 JAIGlobalParameter::getParamStreamControlBufferMax()
{
	return streamControlBufferMax;
}
u32 JAIGlobalParameter::getParamStreamParameterBufferMax()
{
	return streamParameterBufferMax;
}
u32 JAIGlobalParameter::getParamAutoHeapMax() { return autoHeapMax; }
u32 JAIGlobalParameter::getParamStayHeapMax() { return stayHeapMax; }
u32 JAIGlobalParameter::getParamSeqPlayTrackMax() { return seqPlayTrackMax; }
f32 JAIGlobalParameter::getParamInputGainDown() { return inputGainDown; }
f32 JAIGlobalParameter::getParamOutputGainUp() { return outputGainUp; }
f32 JAIGlobalParameter::getParamDistanceMax() { return distanceMax; }
f32 JAIGlobalParameter::getParamMaxVolumeDistance()
{
	return maxVolumeDistance;
}
f32 JAIGlobalParameter::getParamMinDistanceVolume()
{
	return minDistanceVolume;
}
u32 JAIGlobalParameter::getParamSoundOutputMode() { return 0; }
u16 JAIGlobalParameter::getParamSeDistanceFxParameter()
{
	return seDistanceFxParameter;
}
bool JAIGlobalParameter::getParamInitDataLoadOffFlag() { return false; }
bool JAIGlobalParameter::getParamStreamUseOffFlag() { return false; }
u32 JAIGlobalParameter::getParamAutoHeapRoomSize() { return autoHeapRoomSize; }
u32 JAIGlobalParameter::getParamStayHeapSize() { return stayHeapSize; }
u8 JAIGlobalParameter::getParamSeDolbyCenterValue()
{
	return seDolbyCenterValue;
}
f32 JAIGlobalParameter::getParamSeDolbyFrontDistanceMax()
{
	return seDolbyFrontDistanceMax;
}
f32 JAIGlobalParameter::getParamSeDolbyBehindDistanceMax()
{
	return seDolbyBehindDistanceMax;
}
char* JAIGlobalParameter::getParamInitDataFileName()
{
	return initDataFileName;
}
char* JAIGlobalParameter::getParamSeInfoFileName() { return seInfoFileName; }
char* JAIGlobalParameter::getParamSeqInfoFileName() { return seqInfoFileName; }
char* JAIGlobalParameter::getParamStreamInfoFileName()
{
	return streamInfoFileName;
}
char* JAIGlobalParameter::getParamWavePath() { return wavePath; }
char* JAIGlobalParameter::getParamSequenceArchivesPath()
{
	return sequenceArchivesPath;
}
char* JAIGlobalParameter::getParamStreamPath() { return streamPath; }
char* JAIGlobalParameter::getParamAudioResPath() { return audioResPath; }
char* JAIGlobalParameter::getParamSequenceArchivesFileName()
{
	return sequenceArchivesFileName;
}
bool JAIGlobalParameter::getParamSeqEntryCancelFlag() { return false; }
bool JAIGlobalParameter::getParamStreamEntryCancelFlag() { return false; }
u32 JAIGlobalParameter::getParamDopplarMoveTime() { return dopplarMoveTime; }
u8 JAIGlobalParameter::getParamDistanceParameterMoveTime()
{
	return distanceParameterMoveTime;
}
u32 JAIGlobalParameter::getParamDummyPositionLifeTime()
{
	return dummyPositionLifeTime;
}
u32 JAIGlobalParameter::getParamDummyPositionMax() { return dummyPositionMax; }
u8 JAIGlobalParameter::getParamSeqMuteVolumeSePlay()
{
	return seqMuteVolumeSePlay;
}
u32 JAIGlobalParameter::getParamSeqMuteMoveSpeedSePlay()
{
	return seqMuteMoveSpeedSePlay;
}
u32 JAIGlobalParameter::getParamAudioCameraMax() { return audioCameraMax; }
s32 JAIGlobalParameter::getParamSystemTrackMax() { return systemTrackMax; }
s32 JAIGlobalParameter::getParamSystemRootTrackMax()
{
	return systemRootTrackMax;
}
