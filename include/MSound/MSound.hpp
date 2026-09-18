#ifndef MSOUND_HPP
#define MSOUND_HPP

#include <dolphin/types.h>
#include <dolphin/mtx.h>

#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JAudio/JASystem/JASTrackMgr.hpp>
#include <JSystem/JAudio/JASystem/JASWaveArcLoader.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <MSound/MSSceneWave.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>

class JAIActor;
class JAICamera;
class JAIAnimeFrameSoundData;
class MSModBgm;
class MSBgmXFade;

enum MSSeSwBit {
	MSSeSwBit_AnimeSpeed                 = 0x00100000,
	MSSeSwBit_AnimeSpeedShift            = 20,
	MSSeSwBit_ModDistanceVolume          = 0x00200000,
	MSSeSwBit_RandomVolumePSlopeMask     = 0x00C00000,
	MSSeSwBit_RandomVolumePSlopeShift    = 22,
	MSSeSwBit_RandomVolumeCSlopeMask     = 0x03000000,
	MSSeSwBit_RandomVolumeCSlopeShift    = 24,
	MSSeSwBit_RandomVolumeAmplitudeMask  = 0x0C000000,
	MSSeSwBit_RandomVolumeAmplitudeShift = 26,
	MSSeSwBit_RandomIDWeightMask         = 0x70000000,
	MSSeSwBit_RandomIDWeightShift        = 28,
	MSSeSwBit_RandomID                   = 0x80000000,
};

enum MSBgmSwBit {
	MSBgmSwBit_YoshiPercussion = 0x10000000,
};

class MSSeCallBack {
public:
	static u16 setParameterSeqSync(JASystem::TTrack*, u16);
	static void setWaterFilter(u16);
	static void setWaterCameraFir(bool);
	static u16 smTrackCategory[];
	static u8 smPolifonic[];
	static u16 smWaterFilter;
};

namespace MSLoadWave {
bool loadWaveBackword(int, int);
}

namespace MSMarioPosVolume {
f32 getDistFromMario(const Vec&);
}

// JAIGlobalParameter carries nothing but statics, and MSound inherits them:
// TOptionSoundUnit::adjust reads `setting.mOutputMode` into a register, calls
// the weak SMSGetMSound() and throws the result away, then calls
// setParamSoundOutputMode with the pre-loaded argument -- i.e. a discarded
// receiver in front of an inherited static member
// (TOptionControl::checkInput, 0x801838E8).
class MSound : public JAIBasic, public JAIGlobalParameter {
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
	void fadeOutAllSound(u32 fadeout);
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
#if defined(VERSION_GMSE01)
	f32 getDistPowFromCamera(const Vec&);
#endif
	bool cameraLooksAtMario();

	void startSoundSet(u32, const Vec*, u32, f32, u32, u32, u8);
	void startSoundSetGrp(u32, const Vec*, u32, f32, u32, u32, u8);
	void startSoundActorSpecial(u32 id, const Vec* position, f32, f32,
	                            u32 ground_no, JAISoundHandle* out_handle, u32,
	                            u8);
	void startBeeSe(Vec*, u32);

	u32 startMarioVoice(u32, s16, u8);
	void stopMarioVoice(u32, u8);
	u32 getMarioVoiceID(u8);
	JAISound* checkMarioVoicePlaying(u8);

	void playTimer(u32);
	void requestShineAppearFanfare();

	u32 getWallSound(u32, f32);
	u32 getBstPitch(u32);
	static u32 getBstSwitch(u32 sound_id);
	static u32 getSwitch(u32 sound_id, u32 mask, u32 offset);
	bool gateCheck(u32);

	bool resetAudioAll(u16);

	// real
	void startSoundSystemSE(u32 id, u32 param_2, JAISoundHandle* out_handle,
	                        u32 fade)
	{
		if (gateCheck(id))
			MSoundSESystem::MSoundSE::startSoundSystemSE(id, param_2,
			                                             out_handle, fade);
	}

	// Fabricated, very likely due to real startSoundSystemSE.
	//
	// Header round 13 re-measured batch 74's "bind the callee's result"
	// proposal (`JAISound* sound = nullptr; if (gateCheck(id)) sound =
	// MSoundSE::startSoundActor(...); return sound;`) and REJECTED it.
	// Whole-tree: 53 functions improve, 37 of them to byte-exact (TDebuTelesa::
	// receiveMessage, THamuKuri::behaveToFindMario, TSmallEnemy::
	// setAfterDeadEffect, TMapObjBase::startSound, TMario::stayWall, ...), but
	// three byte-exact functions regress, and two of them cannot be recovered:
	//
	//   TMapEventSink::control  100 -> 99.95 (frame 0x48 -> 0x50)
	//   TNerveKazekunPreAttack::execute  100 -> 99.77
	//   TBGPolDrop::move  100 -> 99.80 (recoverable, see below)
	//
	// The slot arithmetic is decisive. TMapEventSink::control's vector
	// temporary sits at r1+0x28 + 4 * (number of 4-byte bindings this inline
	// makes at the site), measured: 0x28 with the call spelled raw, 0x2c with
	// one call-site binding, 0x30 with this wrapper as written (`this` +
	// `position`), 0x34 with the result local added. Retail's temporary is at
	// 0x30 in a byte-exact function, so retail's wrapper makes exactly *two*
	// bindings here -- there is no room for a third, and no compensating error
	// is possible in a function that already matches byte for byte. The same
	// reading holds for TNerveKazekunPreAttack::execute (0x68 / 0x6c / 0x70).
	//
	// A single body cannot serve both groups, so the 4 bytes the 37 improving
	// sites want are *not* in this function; they must come from a second
	// inlined level between those sites and this one (a per-class sound helper
	// that inlined everywhere and so left no map symbol, in the shape of the
	// emitted TMario::startSoundActor(u32) and TMapObjBase::startSound(u16)).
	// That is the lead to chase, not this body.
	//
	// Call-site spellings tried at the three regressing sites, all with the
	// result-binding body in place:
	//   raw `if (gpMSound->gateCheck(id)) MSoundSE::startSoundActor(...)`
	//       -- 4 bytes short at all three (and it moves the position argument's
	//          evaluation after the gate, losing r30 at TMapEventSink);
	//   raw + `const Vec* pos` named before the gate -- restores the argument
	//       order (r30 back) but still 4 short at TMapEventSink; actively worse
	//       at Kazekun (98.7) and TBGPolDrop (97.3), which evaluate the address
	//       after the gate;
	//   raw + `JAISound* sound = nullptr;` named at the site -- recovers
	//       TBGPolDrop::move to byte-exact, but adds nothing at TMapEventSink:
	//       a call-site named pointer is register-allocated, whereas an inlined
	//       wrapper's binding always takes a slot. That asymmetry is why no raw
	//       spelling can imitate the wrapper's frame;
	//   `gpMSound->` vs `SMSGetMSound()->` receiver -- codegen-identical here.
	// See docs/catalog/frame-gaps.md, "Closure batch 74", for the site table.
	void startSoundActor(u32 id, const Vec* position, u32 ground_no,
	                     JAISoundHandle* out_handle, u32 fade, u8 camera_idx)
	{
		if (gateCheck(id))
			MSoundSESystem::MSoundSE::startSoundActor(
			    id, position, ground_no, out_handle, fade, camera_idx);
	}

	void startSoundActorWithInfo(u32 id, const Vec* position, Vec* param_3,
	                             f32 param_4, u32 param_5, u32 ground_no,
	                             JAISoundHandle* out_handle, u32 fade,
	                             u8 camera_idx)
	{
		if (gateCheck(id))
			MSoundSESystem::MSoundSE::startSoundActorWithInfo(
			    id, position, param_3, param_4, param_5, ground_no, out_handle,
			    fade, camera_idx);
	}

	void startSeRandPlay(u32 id, u32 param_2)
	{
		if (gateCheck(id))
			MSoundSESystem::MSRandPlay::startSeRandPlay(id, param_2);
	}

	void startForceJumpSound(Vec* position, u32 param_2, f32 param_3,
	                         u32 param_4)
	{
		u32 id;
		switch (param_2 & 0xff) {
		case 0x15:
		case 0x17:
		case 0x1D:
			id = MSD_SE_MA_STALL_JUMP;
			break;

		case 0x1E:
		default:
			if (param_4 < 0x1770)
				id = MSD_SE_MA_ROPE_JUMP_A;
			else if (param_4 < 0x2EE0)
				id = MSD_SE_MA_ROPE_JUMP_B;
			else
				id = MSD_SE_MA_ROPE_JUMP_C;
			break;
		}

		if (gateCheck(id))
			startSoundActor(id, position, 0, nullptr, 0, 4);
	}

	bool checkUnkA8(u32 flag) { return !(unkA8 & flag) ? false : true; }

	// Fabricated name. The inline level it adds is what gives
	// TNerveBossHanachanSnort::execute its 0x40 frame (raw gpMSound->unk98 is
	// 0x28; this level is +8 per site and the SMSGetMSound() level another +8
	// over the two sites).
	MSModBgm* getModBgm() { return unk98; }

public:
#if defined(VERSION_GMSE01)
	/* 0x98 */ u8 mWaterFilterOverride;
	/* 0x9A */ u16 mTimerParameter;
#endif
	// The following legacy names/offsets describe the Japanese layout.
	// In GMSE01 these fields are four bytes later.
	/* 0x98 */ MSModBgm* unk98;
	/* 0x9C */ MSBgmXFade* unk9C;
	/* 0xA0 */ u32 unkA0;
	/* 0xA4 */ u32 unkA4;
	/* 0xA8 */ u8 unkA8;
	/* 0xAC */ JAICamera unkAC[2];
	/* 0xC4 */ JAISound* unkC4;
	/* 0xC8 */ u8 unkC8[5];
	/* 0xCD */ u8 unkCD;
	/* 0xCE */ u8 unkCE;
	/* 0xCF */ u8 unkCF;
	/* 0xD0 */ u8 unkD0;
	/* 0xD1 */ u8 unkD1;
	/* 0xD2 */ char unkD2[0x304 - 0xD2];
	/* 0x304 */ JASystem::TTrack::TOuterParam* unk304;
	/* 0x308 */ char unk308[0x4];
};

extern MSound* MSGMSound;
extern JAIBasic* MSGBasic;
extern MSound* gpMSound;

// real
inline MSound* SMSGetMSound() { return gpMSound; }

#endif // MSOUND_HPP
