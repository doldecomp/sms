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
	// The return type is real: MSoundSE::startSoundActor returns JAISound*,
	// and handing it straight back is codegen-neutral (measured whole-tree:
	// not one function moved), so this body still binds exactly *two* 4-byte
	// values at a call site, `this` (the gpMSound load) and `position` (the
	// address computation). Constant arguments bind nothing.
	//
	// That count is pinned by a byte-exact caller and must not grow. At a site
	// with no low-region slack each value an inlined callee binds pushes the
	// caller's next temporary up by 4, so a byte-exact caller *counts* the
	// bindings: TMapEventSink::control's vector temporary sits at r1+0x30 =
	// 0x28 + two bindings, and TNerveKazekunPreAttack::execute reads the same
	// way (0x68 / 0x6c / 0x70). Binding the result here (batch 74's proposal)
	// would make 0x34 and break both; that is why it was rejected in header
	// round 13. Eleven sites need this exact two-binding body.
	JAISound* startSoundActor(u32 id, const Vec* position, u32 ground_no,
	                          JAISoundHandle* out_handle, u32 fade,
	                          u8 camera_idx)
	{
		if (gateCheck(id))
			return MSoundSESystem::MSoundSE::startSoundActor(
			    id, position, ground_no, out_handle, fade, camera_idx);
		return nullptr;
	}

	// Fabricated. The two-argument form is the second inlined level batch 82
	// went looking for: the 4 bytes that ~37 sound sites are short of live
	// here, not in the six-argument body, so spelling the common
	// `(id, position, 0, nullptr, 0, 4)` call short pays for them at exactly
	// the sites that use it and leaves the eleven six-argument sites alone.
	// 26 functions in 20 units go byte-exact this way with nothing regressing
	// (DebuTelesa and WoodBarrel become 100/100 units).
	//
	// The slot is the *binding*, not the level: measured in a scratch TU, a
	// plain forwarder, a `const Vec&`/`const TVec3<f32>&` parameter and a
	// member wrapper that binds only `this` are all worth +0, and so is a
	// bound-but-dead local; only binding the callee's JAISound* and keeping it
	// live by returning it reserves the 4 bytes (a dead 4-byte *non-trivial*
	// local in the same place is codegen-identical, which is how the size was
	// confirmed). Hence `JAISound* sound = ...; return sound;` rather than a
	// direct `return`, which is worth nothing.
	//
	// Apply it per site, never per file: a function with several sound sites
	// in mutually exclusive branches pays per expansion and overshoots
	// (TBellDolpic::control +0x10 for four sites, TRoulette::switchStop,
	// TDptMonteFence::touchPlayer and TMapObjBase::startSound +8 for two,
	// TFireWanwan::receiveMessage and TNerveKazekunHitWater::execute +8 for
	// one). Those want the binding one level deeper and stay open.
	// See docs/catalog/frame-gaps.md, "Closure batch 82", for the site table.
	//
	// Round 14 measured the overshoot exactly: one expansion of this overload
	// is +8 of frame, not +4, and N expansions are +8N (TBellDolpic::control
	// 0x48 -> 0x68 for its four sites, where retail wants 0x58). So the
	// multi-site functions that need a flat +8 -- TDptMonteFence::touchPlayer
	// and TRoulette::switchStop (two sites, +8 wanted),
	// TMapObjBase::startSound (two, +8), TBellDolpic::control (four, +0x10),
	// TTelesa::changeByJuice -- need a carrier that *saturates per function*,
	// which nothing measured so far does. Ruled out for them: this overload at
	// every site (overshoots by 8 per extra site); this overload at exactly one
	// of two sites (lands the frame -- touchPlayer goes to 100% -- but two
	// different spellings of the same call one line apart is not a spelling a
	// developer would write, and which site gets it is unknowable); a per-class
	// wrapper (the map lists no sound helper for TBellDolpic, TDptMonteFence,
	// TMonumentShine or TRoulette, and a member wrapper binding only `this` is
	// +0 anyway); a caller-level `MSound*` local (retail re-reads
	// gpMSound@sda21 before each gateCheck, so the receiver is re-spelled).
	// The carrier is therefore still unidentified and these sites keep the
	// six-argument spelling.
	JAISound* startSoundActor(u32 id, const Vec* position)
	{
		JAISound* sound = startSoundActor(id, position, 0, nullptr, 0, 4);
		return sound;
	}

	// Fabricated, the two-argument form's sibling for the sites that pass a
	// real handle: TMareGate::control hands &gpMSound->unk7C in and is 8 bytes
	// of frame short with every instruction exact, which is the same single
	// binding the two-argument overload supplies.
	JAISound* startSoundActor(u32 id, const Vec* position,
	                          JAISoundHandle* out_handle)
	{
		JAISound* sound = startSoundActor(id, position, 0, out_handle, 0, 4);
		return sound;
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

	// fabricated. The named local is the binding closure batch 110 measured
	// at TSunMgr::perform, where the BGM handle is read through a level that
	// binds its result (+8 at each of the three sites).
	JAISound* getBgm()
	{
		JAISound* bgm = unk7C;
		return bgm;
	}

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
