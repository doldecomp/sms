#ifndef MSOUND_HPP
#define MSOUND_HPP

#include <dolphin/types.h>
#include <dolphin/mtx.h>

#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JAudio/JASystem/JASTrackMgr.hpp>
#include <JSystem/JAudio/JASystem/JASWaveArcLoader.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>

class JAIActor;
class JAICamera;
class JAIAnimeFrameSoundData;

enum MS_SCENE_WAVE {
	MS_WAVE_UNK0         = 0,
	MS_WAVE_DEFAULT      = 256,
	MS_WAVE_DOLPIC       = 513,
	MS_WAVE_BIANCO       = 514,
	MS_WAVE_MANMA        = 515,
	MS_WAVE_PINNAPACO_S  = 516,
	MS_WAVE_PINNAPACO    = 516,
	MS_WAVE_MARE_SEA     = 517,
	MS_WAVE_MONTEVILLAGE = 518,
	MS_WAVE_SHILENA      = 519,
	MS_WAVE_RICO         = 520,
	MS_WAVE_CLEAR        = 521,
	MS_WAVE_UNK528       = 528,
};

class MSSeCallBack {
public:
	static u16 setParameterSeqSync(JASystem::TTrack*, u16);
	static void setWaterFilter(u16);
	static void setWaterCameraFir(bool);
	static u8 smTrackCategory;
	static u8 smPolifonic;
	static u16 smWaterFilter;
};

class MSLoadWave {
public:
	static bool loadWaveBackword(int, int);
	static bool loadWaveBackword(JASystem::WaveArcLoader::TObject*);
};

class MSMarioPosVolume {
public:
	static f32 getDistFromMario(const Vec&);
};

class MSound : public JAIBasic {
public:
	MSound(JKRHeap*, JKRHeap*, u32, u8*, u8*, u32);
	~MSound() { }

	virtual JAISound* makeSound(u32);
	virtual u32 getMapInfoGround(u32) { return 0; }
	virtual void setSeExtParameter(JAISound*);
	virtual void setRegisterTrackCallback();
	virtual void loadGroupWave(s32, s32);

	static MSound* getMSound();

	void initSound();
	void mainLoop();
	void exitStage();
	void enterStage(MS_SCENE_WAVE, u8, u8);
	void loadWave(MS_SCENE_WAVE);
	void cleanUpAramWave(u8);
	bool checkWaveOnAram(MS_SCENE_WAVE);
	bool checkSeqOnMemory(u32);

	void stopAllSound();
	void fadeOutAllSound(u32);
	void stopAllSeInCategory(u8, u32);
	void setCategoryAllVolume(u8, f32, u32, u8);
	void setCategoryVOLs(u16, f32);
	void setCategoryVOLsDefault(u16);

	void pauseOn(bool);
	void pauseOff(u8);
	void talkModeIn(bool);
	void talkModeOut();
	void demoModeIn(u16, bool);
	void demoModeOut(bool);

	void setPlayerInfo(Vec*, Vec*, MtxPtr, bool);
	void setCameraInfo(Vec*, Vec*, MtxPtr, u32);
	f32 getDistFromCamera(Vec*);
	bool cameraLooksAtMario();

	JAISound* startSoundSet(u32, const Vec*, u32, f32, u32, u32, u8);
	JAISound* startSoundSetGrp(u32, const Vec*, u32, f32, u32, u32, u8);
	JAISound* startSoundActorSpecial(u32, const Vec*, f32, f32, u32, JAISound**,
	                                 u32, u8);
	JAISound* startBeeSe(Vec*, u32);

	void startMarioVoice(u32, s16, u8);
	void stopMarioVoice(u32, u8);
	u8 getMarioVoiceID(u8);
	bool checkMarioVoicePlaying(u8);

	void playTimer(u32);
	void requestShineAppearFanfare();

	u32 getWallSound(u32, f32);
	u32 getBstPitch(u32);
	static u32 getBstSwitch(u32);
	static u32 getSwitch(u32, u32, u32);
	bool gateCheck(u32);

	bool resetAudioAll(u16);
};

extern MSound* MSGMSound;
extern JAIBasic* MSGBasic;
extern MSound* gpMSound;

#endif // MSOUND_HPP
