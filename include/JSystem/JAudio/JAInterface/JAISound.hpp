#ifndef JAISOUND_HPP
#define JAISOUND_HPP

#include <dolphin/types.h>
#include <dolphin/mtx.h>

struct JAIMoveParaSet;
struct JAISeqParameter;
struct JAISeParameter;
struct JAIStreamParameter;
class JAIBasic;

#define JAISoundID_TypeMask      0xC0000000
#define JAISoundID_Type_Se       0x00000000
#define JAISoundID_Type_Sequence 0x80000000
#define JAISoundID_Type_Stream   0xC0000000

// How far a sound has got. The three sound types share one ladder and the
// library compares it ordinally (>= 3, < 4). The names come from the functions
// that service each state - checkEntriedStream, checkReadSeq, checkStartedSeq,
// checkPlayingSeq, checkFadeoutSeq - but a se reads the top of the ladder
// differently, so each state says what it means for all three.
enum JAISoundState {
	// free, on the free list of its JAILinkBuffer
	SOUNDSTATE_Inactive = 0,
	// entered, and waiting for what it needs: a se for a track, a sequence for
	// its data, which it may be reading from the disc, a stream for the player
	SOUNDSTATE_Stored = 1,
	// it has what it needs and the start comes next: a se holds its track, the
	// data of a sequence is in memory, a stream holds the player
	SOUNDSTATE_Prepared = 2,
	// the start is issued. A sequence waits for its player to report active, a
	// stream waits after JAInter::StreamLib::start, and a se is on its track
	// and gets its parameters again this frame
	SOUNDSTATE_Started = 3,
	// playing
	SOUNDSTATE_Playing = 4,
	// about to be released. A sequence or a stream fades out here and stops
	// when its volume reaches zero. A se is playing and waits for the game to
	// ask for it again, which puts it back to _Playing; it is released if the
	// game does not
	SOUNDSTATE_Stopping = 5,
};

class JAISound {
public:
	struct FabricatedPositionInfo {
		/* 0x00 */ Vec unk0;
		/* 0x0C */ Vec unkC;
		/* 0x18 */ f32 unk18;
	};

	/* 0x0 */ u8 mTrack;
	/* 0x1 */ u8 mState;
	/* 0x2 */ u8 mWaitTimer;
	/* 0x3 */ u8 mRandom;
	/* 0x4 */ u8 unk4;
	/* 0x5 */ u8 unk5;
	/* 0x6 */ s16 mAdjustPrio;
	/* 0x8 */ u32 mSoundID;
	/* 0xC */ u32 unkC;
	/* 0x10 */ u32 mFadeCounter;
	/* 0x14 */ u32 mPlayGameFrameCounter;
	/* 0x18 */ u32 mActorGroundNumber;
	/* 0x1C */ FabricatedPositionInfo* unk1C;
	/* 0x20 */ const void* mActor;
	/* 0x24 */ const Vec* mActorTrans;
	/* 0x28 */ const Vec* unk28;
	/* 0x2C */ JAISound* mPrevSound;
	/* 0x30 */ JAISound* mNextSound;
	/* 0x34 */ JAISound** mMainSoundPPointer;
	// JAISeqParameter* or JAIStreamParameter* or JAISeParameter*
	/* 0x38 */ void* mCustomParameter;
	/* 0x3C */ void* mInfo;
	/* 0x40 */ // vtable

public:
	JAISound();

	virtual void setSeDistanceParameters();
	virtual void setSeDistanceVolume(u8);
	virtual void setSeDistancePan(u8);
	virtual void setSeDistancePitch(u8);
	virtual void setSeDistanceFxmix(u8);
	virtual void setSeDistanceFir(u8);
	virtual void setSeDistanceDolby(u8);
	virtual void setSePositionDopplar();
	virtual f32 setPositionDopplarCommon(u32);
	virtual f32 setDistanceVolumeCommon(f32, u8);
	virtual f32 setDistancePanCommon();

	int initMoveParameter(JAIMoveParaSet*, f32, u32);
	void initMultiMoveParameter(JAIMoveParaSet*, u8, u32, f32, f32, u32);
	u32 getSeCategoryNumber();
	void getDataInfoHeader();
	u32 getSwBit();
	u32 checkSwBit(u32);
	u8 getInfoPriority();
	void clearMainSoundPPointer();
	void release();
	void start(u32);
	void stop(u32);
	void setVolume(f32, u32, u8);
	void setDirectVolume(f32, u32);
	void setPan(f32, u32, u8);
	void setDirectPan(f32, u32);
	void setPitch(f32, u32, u8);
	void setDirectPitch(f32, u32);
	void setFxmix(f32, u32, u8);
	void setDirectFxmix(f32, u32);
	void setDolby(f32, u32, u8);
	void setDirectDolby(f32, u32);
	void setTempoProportion(f32, u32);
	void setPortData(u8, u16);
	void setPrepareFlag(u8);
	void checkReady();
	void setSeqInterVolume(u8, f32, u32);
	void setSeqInterPan(u8, f32, u32);
	void setSeqInterPitch(u8, f32, u32);
	void setSeqInterFxmix(u8, f32, u32);
	void setSeqInterDolby(u8, f32, u32);
	void setSeqTempoProportion(f32, u32);
	void setSeqPortData(u8, u16, u32);
	void setWaveReadMode(s32, s32);
	void setTrackVolume(u8, f32, u32);
	void setTrackVolumeMulti(u8, u32, f32, u32);
	void setTrackMuteSwitch(u8, u8);
	void setTrackMuteSwitchMulti(u32, u8);
	void setTrackInterruptSwitch(u8, u8);
	void setTrackPan(u8, f32, u32);
	void setTrackPanMulti(u8, u32, f32, u32);
	void setTrackPitch(u8, f32, u32);
	void setTrackPitchMulti(u8, u32, f32, u32);
	void setTrackFxmix(u8, f32, u32);
	void setTrackFxmixMulti(u8, u32, f32, u32);
	void setTrackDolby(u8, f32, u32);
	void setTrackDolbyMulti(u8, u32, f32, u32);
	void setTrackFirU7(u8, u8, u32);
	void setTrackFirMultiU7(u8, u32, u8, u32);
	void setTrackPortData(u8, u8, u16);
	void setSeInterMovePara(JAIMoveParaSet*, u32);
	void setSeInterRandomPara(f32*, u32, f32, f32);
	void setSeInterVolume(u8, f32, u32, u8);
	void setSeInterPan(u8, f32, u32, u8);
	void setSeInterFxmix(u8, f32, u32, u8);
	void setSeInterFir(u8, u8, u32, u8);
	void setSeInterDolby(u8, f32, u32, u8);
	void setSeInterPitch(u8, f32, u32, f32);
	void setSePortData(u8, u16);
	void getSePortData(u8);
	void setStreamMode(u32);
	void setStreamInterVolume(u8, f32, u32);
	void setStreamInterPitch(u8, f32, u32);
	void setStreamInterPan(u8, f32, u32);
	void setStreamPrepareFlag(u8);
	void checkStreamReady();
	void setPauseMode(u8, u8);
	void setSeqPrepareFlag(u8);
	void checkSeqReady();
	void getVolume(u8);
	void getPan(u8);
	void getPitch(u8);
	void getFxmix(u8);
	void getDolby(u8);
	void getTempoProportion();
	void getPortData(u8);
	f32 getSeqInterVolume(u8);
	void getSeqInterPan(u8);
	void getSeqInterPitch(u8);
	void getSeqInterFxmix(u8);
	void getSeqInterDolby(u8);
	void getSeqTempoProportion();
	void getSeqPortData(u8);
	void getTrackParameterU7(u8, s8);
	void getTrackParameterF32(u8, s8);
	void getPointer(u8, s8);
	void getTrackPortData(u8, u8);
	void getSeParametermeterF32(u8, u8);
	f32 getStreamInterVolume(u8);
	void getStreamInterPitch(u8);
	u32 getActorGroundNumber();
	JAISeqParameter* getSeqParameter();
	JAISeParameter* getSeParameter();
	JAIStreamParameter* getStreamParameter();
	u32 getTrackPortRoute(u8, u8);
	void* getSeInfoPointer();

	u32 getID() const { return mSoundID; }
	void setID(u32 id) { mSoundID = id; }

	u8 getStatus() const { return mState; }
	void setStatus(u8 state) { mState = state; }

	u8 getWait() const { return mWaitTimer; }
	void setWait(u8 wait) { mWaitTimer = wait; }
	void decWait() { mWaitTimer--; }

	u8 getTrack() const { return mTrack; }
	void setTrack(u8 track) { mTrack = track; }

	void setCustomParameterPointer(void* param) { mCustomParameter = param; }

	JAISound* getPrevSound() const { return mPrevSound; }
	void setPrevSound(JAISound* prev) { mPrevSound = prev; }

	JAISound* getNextSound() const { return mNextSound; }
	void setNextSound(JAISound* next) { mNextSound = next; }

	u32 getFadetime() const { return mFadeCounter; }
	void setFadetime(u32 fadeTime) { mFadeCounter = fadeTime; }

	void setMainSoundPPointer(JAISound** pp) { mMainSoundPPointer = pp; }

	u32 getPlayGameFrameCounter() const { return mPlayGameFrameCounter; }
	void incPlayGameFrameCounter() { mPlayGameFrameCounter++; }

	const void* getAct() const { return mActor; }
	const Vec* getTrans() const { return mActorTrans; }

	void setRandom(u8 random) { mRandom = random; }
	s16 getAdjustPriority() { return mAdjustPrio; }

	u32 getUnk8Lo() { return mSoundID & 0x3FF; }

	void setSeqInterVolumeU7(u8 param, u8 value, u32 moveTime)
	{
		f32 volume = value / 127.0f;
		setSeqInterVolume(param, volume, moveTime);
	}

	void setSeInterFxmixU7(u8 param, u8 value, u32 moveTime, u8 random)
	{
		f32 fxmix = value / 127.0f;
		setSeInterFxmix(param, fxmix, moveTime, random);
	}

	void setSeInterDolbyU7(u8 param, u8 value, u32 moveTime, u8 random)
	{
		f32 dolby = value / 127.0f;
		setSeInterDolby(param, dolby, moveTime, random);
	}

	static JAIBasic* interPointer;
};

#endif // JAISOUND_HPP
