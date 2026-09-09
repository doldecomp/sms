#ifndef JAIPARAMETERS_HPP
#define JAIPARAMETERS_HPP

#include <types.h>

struct JAIMoveParaSet {
	/* 0x0 */ f32 mTargetValue;
	/* 0x4 */ f32 mCurrentValue;
	/* 0x8 */ f32 mMoveAmount;
	/* 0xC */ u32 mMoveCounter;
};

struct JAISeParameter {
	/* 0x0 */ u16 mPortData[16];
	/* 0x20 */ u16 mPortUpdate;
	/* 0x22 */ char unk22[0x124 - 0x22];
	/* 0x124 */ JAIMoveParaSet mVolume[8];
	/* 0x1A4 */ JAIMoveParaSet mPan[8];
	/* 0x224 */ JAIMoveParaSet mPitch[8];
	/* 0x2A4 */ JAIMoveParaSet mFxmix[8];
	/* 0x324 */ JAIMoveParaSet mFir[8];
	/* 0x3A4 */ JAIMoveParaSet mDolby[8];
	/* 0x424 */ f32* mVolumePointer;
	/* 0x428 */ f32* mPanPointer;
	/* 0x42C */ f32* mPitchPointer;
	/* 0x430 */ f32* mFxmixPointer;
	/* 0x434 */ f32* mFirPointer;
	/* 0x438 */ f32* mDolbyPointer;
	/* 0x43C */ JAISeParameter* mPrev;
	/* 0x440 */ JAISeParameter* mNext;
};

class MuteBit {
public:
	u8 mCurrent : 1;
	u8 mTarget : 1;
	u8 mEnable : 1;
};

struct JAISeqUpdateData;

struct JAISeqParameter {
	/* 0x0 */ u32 mSeqHandle;
	/* 0x4 */ JAIMoveParaSet mTempo;
	/* 0x14 */ JAIMoveParaSet mPortData[16];
	/* 0x114 */ JAIMoveParaSet mVolume[20];
	/* 0x254 */ JAIMoveParaSet mPan[20];
	/* 0x394 */ JAIMoveParaSet mPitch[20];
	/* 0x4D4 */ JAIMoveParaSet mFxmix[20];
	/* 0x614 */ JAIMoveParaSet mDolby[20];
	/* 0x754 */ JAIMoveParaSet mTrackVolume[32];
	/* 0x954 */ JAIMoveParaSet mTrackPan[32];
	/* 0xB54 */ JAIMoveParaSet mTrackPitch[32];
	/* 0xD54 */ JAIMoveParaSet mTrackFxmix[32];
	/* 0xF54 */ JAIMoveParaSet mTrackDolby[32];
	/* 0x1154 */ JAIMoveParaSet mTrackFir[32];
	/* 0x1354 */ u16 mTrackPortData[32][16];
	/* 0x1754 */ u8 mAutoHeapPosition;
	/* 0x1755 */ u8 mPauseMode;
	/* 0x1756 */ s16 unk1756;
	/* 0x1758 */ u32 mWaitSceneSet;
	/* 0x175C */ u32 mPortUpdate;
	/* 0x1760 */ u32 mVolumeUpdate;
	/* 0x1764 */ u32 mPanUpdate;
	/* 0x1768 */ u32 mPitchUpdate;
	/* 0x176C */ u32 mFxmixUpdate;
	/* 0x1770 */ u32 mDolbyUpdate;
	/* 0x1774 */ u32 mTrackVolumeUpdate;
	/* 0x1778 */ u32 mTrackPanUpdate;
	/* 0x177C */ u32 mTrackPitchUpdate;
	/* 0x1780 */ u32 mTrackFxmixUpdate;
	/* 0x1784 */ u32 mTrackDolbyUpdate;
	/* 0x1788 */ u32 mTrackFirUpdate;
	/* 0x178C */ u32 mTrackPortUpdate;
	/* 0x1790 */ u32 mTrackPortDataUpdate[32];
	/* 0x1810 */ u8 mTrackInterruptSwitch[32];
	/* 0x1830 */ MuteBit mMuteBits[32];
	/* 0x1850 */ JAISeqUpdateData* mUpdateData;
	/* 0x1854 */ JAISeqParameter* mPrev;
	/* 0x1858 */ JAISeqParameter* mNext;
};

struct JAIStreamUpdateParameter;

struct JAIStreamParameter {
	/* 0x0 */ u8 mPauseMode;
	/* 0x4 */ u32 mStreamMode;
	/* 0x8 */ u32 mVolumeUpdate;
	/* 0xC */ u32 mPitchUpdate;
	/* 0x10 */ u32 mPanUpdate;
	/* 0x14 */ JAIMoveParaSet mVolume[20];
	/* 0x154 */ JAIMoveParaSet mPitch[20];
	/* 0x294 */ JAIMoveParaSet mPan[20];
	/* 0x3D4 */ JAIStreamUpdateParameter* mUpdateData;
	/* 0x3D8 */ JAIStreamParameter* mPrev;
	/* 0x3DC */ JAIStreamParameter* mNext;
};

class JAISound;

// fabricated
struct JAIStreamUpdateParameter {
	/* 0x0 */ u8 unk0; // JAISeqUpdateData's pair here is the pause mode
	/* 0x1 */ u8 unk1; // and the pause volume; nothing in SMS reads either
	/* 0x2 */ u8 mPrepareFlag;
	/* 0x4 */ f32 mVolume;
	/* 0x8 */ f32 mPitch;
	/* 0xC */ f32 mPan;
	/* 0x10 */ u32 mActiveTrackFlag;
	/* 0x14 */ JAISound* mSound;
};

#endif
