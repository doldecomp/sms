#ifndef JAIGLOBALPARAMETER_HPP
#define JAIGLOBALPARAMETER_HPP

/**
 * JAIGlobalParameter - Global audio system parameters
 *
 * This class contains static methods and variables that control the global
 * configuration of the audio system. All members are static because they
 * represent singleton global state that is accessed from multiple parts
 * of the audio system.
 */
class JAIGlobalParameter {
public:
	// Setter methods
	static void setParamSoundOutputMode(unsigned long);
	static void setParamAudioSystemThreadPriority(unsigned char);
	static void setParamAudioDvdThreadPriority(unsigned char);
	static void setParamInitFileLoadSwitch(unsigned char);
	static void setParamInitDataPointer(void*);
	static void setParamInterfaceHeapSize(unsigned long);
	static void setParamSoundSceneMax(unsigned long);
	static void setParamSeRegistMax(unsigned long);
	static void setParamSeTrackMax(unsigned long);
	static void setParamSeqPlayTrackMax(unsigned long);
	static void setParamSeqControlBufferMax(unsigned long);
	static void setParamStreamControlBufferMax(unsigned long);
	static void setParamStreamParameterBufferMax(unsigned long);
	static void setParamAutoHeapMax(unsigned long);
	static void setParamStayHeapMax(unsigned long);
	static void setParamInputGainDown(float);
	static void setParamOutputGainUp(float);
	static void setParamDistanceMax(float);
	static void setParamMaxVolumeDistance(float);
	static void setParamMinDistanceVolume(float);
	static void setParamSeDistanceFxParameter(unsigned short);
	static void setParamInitDataLoadOffFlag(bool);
	static void setParamStreamUseOffFlag(bool);
	static void setParamStreamDecodedBufferBlocks(unsigned long);
	static void setParamStreamInsideBufferCut(bool);
	static void setParamAutoHeapRoomSize(unsigned long);
	static void setParamStayHeapSize(unsigned long);
	static void setParamSeDolbyCenterValue(unsigned char);
	static void setParamSeDolbyFrontDistanceMax(float);
	static void setParamSeDolbyBehindDistanceMax(float);
	static void setParamInitDataFileName(char*);
	static void setParamSeInfoFileName(char*);
	static void setParamSeqInfoFileName(char*);
	static void setParamStreamInfoFileName(char*);
	static void setParamWavePath(char*);
	static void setParamSequenceArchivesPath(char*);
	static void setParamStreamPath(char*);
	static void setParamAudioResPath(char*);
	static void setParamSequenceArchivesFileName(char*);
	static void setParamSeqEntryCancelFlag(bool);
	static void setParamStreamEntryCancelFlag(bool);
	static void setParamDopplarMoveTime(unsigned long);
	static void setParamDistanceParameterMoveTime(unsigned char);
	static void setParamDummyPositionLifeTime(unsigned long);
	static void setParamDummyPositionMax(unsigned long);
	static void setParamSeqMuteVolumeSePlay(unsigned char);
	static void setParamSeqMuteMoveSpeedSePlay(unsigned long);
	static void setParamAudioCameraMax(unsigned long);
	static void setParamSystemTrackMax(long);
	static void setParamSystemRootTrackMax(long);
	static void setParamPanDistanceMax(float);
	static void setParamPanAngleParameter(float);
	static void setParamDopplarParameter(float);
	static void setParamSeDistanceWaitMax(unsigned short);
	static void setParamSeDistancepitchMax(float);

	// Getter methods
	static unsigned char getParamAudioSystemThreadPriority();
	static unsigned char getParamAudioDvdThreadPriority();
	static unsigned char getParamInitFileLoadSwitch();
	static void* getParamInitDataPointer();
	static unsigned long getParamInterfaceHeapSize();
	static unsigned long getParamSeCategoryMax();
	static unsigned long getParamSoundSceneMax();
	static unsigned long getParamSeRegistMax();
	static unsigned long getParamSeTrackMax();
	static unsigned long getParamSeqTrackMax();
	static unsigned long getParamSeqControlBufferMax();
	static unsigned long getParamStreamControlBufferMax();
	static unsigned long getParamStreamParameterBufferMax();
	static unsigned long getParamAutoHeapMax();
	static unsigned long getParamStayHeapMax();
	static unsigned long getParamSeqPlayTrackMax();
	static float getParamInputGainDown();
	static float getParamOutputGainUp();
	static float getParamDistanceMax();
	static float getParamMaxVolumeDistance();
	static float getParamMinDistanceVolume();
	static unsigned long getParamSoundOutputMode();
	static unsigned short getParamSeDistanceFxParameter();
	static bool getParamInitDataLoadOffFlag();
	static bool getParamStreamUseOffFlag();
	static unsigned long getParamAutoHeapRoomSize();
	static unsigned long getParamStayHeapSize();
	static unsigned char getParamSeDolbyCenterValue();
	static float getParamSeDolbyFrontDistanceMax();
	static float getParamSeDolbyBehindDistanceMax();
	static char* getParamInitDataFileName();
	static char* getParamSeInfoFileName();
	static char* getParamSeqInfoFileName();
	static char* getParamStreamInfoFileName();
	static char* getParamWavePath();
	static char* getParamSequenceArchivesPath();
	static char* getParamStreamPath();
	static char* getParamAudioResPath();
	static char* getParamSequenceArchivesFileName();
	static bool getParamSeqEntryCancelFlag();
	static bool getParamStreamEntryCancelFlag();
	static unsigned long getParamDopplarMoveTime();
	static unsigned char getParamDistanceParameterMoveTime();
	static unsigned long getParamDummyPositionLifeTime();
	static unsigned long getParamDummyPositionMax();
	static unsigned char getParamSeqMuteVolumeSePlay();
	static unsigned long getParamSeqMuteMoveSpeedSePlay();
	static unsigned long getParamAudioCameraMax();
	static long getParamSystemTrackMax();
	static long getParamSystemRootTrackMax();

	// Global parameter variables
	static unsigned char distanceParameterMoveTime;
	static unsigned char audioSystemThreadPriority;
	static unsigned char audioDvdThreadPriority;
	static unsigned char seqMuteVolumeSePlay;
	static unsigned short seDistanceFxParameter;
	static unsigned long soundSceneMax;
	static unsigned long seRegistMax;
	static unsigned long seTrackMax;
	static unsigned long seqTrackMax;
	static unsigned long seqPlayTrackMax;
	static unsigned long seqControlBufferMax;
	static unsigned long streamControlBufferMax;
	static unsigned long streamParameterBufferMax;
	static unsigned long autoHeapMax;
	static unsigned long stayHeapMax;
	static unsigned long autoHeapRoomSize;
	static unsigned long stayHeapSize;
	static char* seInfoFileName;
	static char* seqInfoFileName;
	static char* streamInfoFileName;
	static char* initDataFileName;
	static char* wavePath;
	static char* sequenceArchivesPath;
	static char* streamPath;
	static char* sequenceArchivesFileName;
	static float inputGainDown;
	static float outputGainUp;
	static float distanceMax;
	static float maxVolumeDistance;
	static unsigned char seDolbyCenterValue;
	static float seDolbyFrontDistanceMax;
	static float seDolbyBehindDistanceMax;
	static unsigned long dopplarMoveTime;
	static unsigned long dummyPositionLifeTime;
	static unsigned long dummyPositionMax;
	static unsigned long seqMuteMoveSpeedSePlay;
	static unsigned long audioCameraMax;
	static long systemTrackMax;
	static long systemRootTrackMax;
	static float panDistanceMax;
	static float panDistance2Max;
	static float panAngleParameter;
	static float panAngleParameter2;
	static float dopplarParameter;
	static unsigned short seDistanceWaitMax;
	static float seDistancepitchMax;
	static unsigned short seDefaultFx;
	static unsigned long interfaceHeapSize;
	static char* audioResPath;
	static float minDistanceVolume;
};

#endif // JAIGLOBALPARAMETER_HPP
