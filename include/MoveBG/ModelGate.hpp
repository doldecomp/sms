#ifndef MOVE_BG_MODEL_GATE
#define MOVE_BG_MODEL_GATE

#include <Strategic/TakeActor.hpp>
#include <M3DUtil/MActor.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>

class SampleCtrlModelData;

// TODO: this belongs in M3DUtil/SampleCtrlModel.hpp, which only forward
// declares the class. It lives here because this batch may not edit shared
// headers; nothing else in the tree dereferences a SampleCtrlMaterial yet.
// TModelGate::perform patches the tev stage infos at indices 0, 2, 3 and 5 of
// the array at 0x3c and copies them into the gate material's matching stages,
// so the array holds at least six entries.
class SampleCtrlMaterial {
public:
	/* 0x00 */ u8 unk0[0x3C];
	/* 0x3C */ J3DTevStageInfo mTevStageInfo[6];
};

/// The magic warp gate in Delfino Plaza. Holding it (as a TTakeActor) is how
/// Mario gets pulled through: the gate owns the wind model, the swirl
/// particles and the screen blur.
class TModelGate : public TTakeActor {
public:
	TModelGate(const char* name = "<TModelGate>")
	    : TTakeActor(name)
	{
	}

	virtual MtxPtr getTakingMtx() { return nullptr; }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void loadAfter();

	void screenBlur(JDrama::TGraphics*);
	void startOpen();

	enum {
		/// Set once the gate is live (either already unlocked or opening).
		GATE_FLAG_ACTIVE = 1,
		/// Set while the swirl is fading in.
		GATE_FLAG_OPENING = 2,
	};

	enum {
		STATE_WAIT_ANIM = 0,
		STATE_SWIRL     = 1,
		STATE_HIT       = 2,
	};

public:
	// Size 0x118
	/* 0x70 */ u8 mFlags;
	// TODO: unk71/unk72/unk78/unkAC keep their placeholder names because
	// MarioParticle.cpp, MarioAutodemo.cpp and EventWatcher.cpp spell them
	// out; they are the gate index (0..4), the "center" joint index, the
	// MActor and Mario's hold offset in gate space.
	/* 0x71 */ u8 unk71;
	/* 0x72 */ u16 unk72;
	/// Yaw of the center joint, as a short angle.
	/* 0x74 */ s16 mCenterYaw;
	/* 0x76 */ u16 unk76;
	/* 0x78 */ MActor* unk78;
	/// Inverse of the center joint matrix; turns world space into gate space.
	/* 0x7C */ Mtx mInvCenterMtx;
	/// Where Mario is held, in gate space.
	/* 0xAC */ JGeometry::TVec3<f32> unkAC;
	/* 0xB8 */ u8 unkB8;
	/// Which tev-stage patch variant the swirl is currently on, 0..7.
	/* 0xB9 */ u8 mSwirlStep;
	/* 0xBA */ u8 unkBA;
	/// Frames until the next swirl step.
	/* 0xBC */ u16 mSwirlTimer;
	/// Frames per swirl step.
	/* 0xBE */ u16 mSwirlInterval;
	/* 0xC0 */ SampleCtrlModelData* mCtrlModelData;
	/* 0xC4 */ u8 mState;
	/* 0xC5 */ u8 unkC5;
	/* 0xC6 */ u8 unkC6;
	/// Frames of wind the gate still has to blow.
	/* 0xC8 */ s16 mWindTime;
	/* 0xCA */ s16 mWindTimeLeft;
	/* 0xCC */ s16 mWindFrame;
	/* 0xCE */ s16 unkCE;
	/// 0..1 blend of the swirl animation.
	/* 0xD0 */ f32 mOpenRate;
	/* 0xD4 */ f32 mOpenSpeed;
	/* 0xD8 */ f32 mCloseSpeed;
	/* 0xDC */ f32 mIdleCloseSpeed;
	/* 0xE0 */ u8 mBlurStrength;
	/* 0xE4 */ f32 mBlurAlpha;
	/* 0xE8 */ f32 mBlurAlphaRate;
	/* 0xEC */ f32 mBlurRadius;
	/* 0xF0 */ f32 mBlurNearDist;
	/* 0xF4 */ f32 mBlurFarDist;
	/* 0xF8 */ f32 mHitParticleRate;
	/* 0xFC */ f32 unkFC;
	/* 0x100 */ f32 unk100;
	/* 0x104 */ f32 unk104;
	/* 0x108 */ f32 unk108;
	/* 0x10C */ f32 unk10C;
	/* 0x110 */ f32 unk110;
	/* 0x114 */ f32 unk114;
};

#endif
