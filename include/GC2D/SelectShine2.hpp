#ifndef GC2D_SELECT_SHINE_2_HPP
#define GC2D_SELECT_SHINE_2_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <dolphin/mtx.h>

class J3DAnmColor;
class J3DDrawBuffer;
class J3DModel;
class J3DModelData;
class JPAEmitterManager;

/**
 * @brief One shine icon on the episode select screen.
 *
 * @details The shines sit on a wide circle around TSelectShineManager::cCenter
 * and bob up and down along a four-segment quadratic Bezier curve
 * (makeNewPosition). The colour animation frame doubles as a fade: frame 0 is
 * fully opaque, the last frame is fully transparent, so the frame is driven
 * from the distance to the camera and forced up or down while a shine is
 * appearing or disappearing.
 */
class TSelectShine {
public:
	TSelectShine(J3DModelData* model_data, J3DAnmColor* anm_color,
	             JPAEmitterManager* emitter_manager,
	             JGeometry::TVec3<f32>& position, s16 angle, u8 type,
	             f32 bound_phase, f32 bound_speed, f32 bound_height);
	virtual ~TSelectShine() { }
	virtual void move();

	f32 makeNewPosition(f32 t, f32 start, f32 middle, f32 end);

	J3DModel* getModel() { return mModel; }

	// perform() loads mShines[i] once and keeps it in a non-volatile register
	// across both matrix calls, which only happens when `this` comes from an
	// inlined member. No map symbol names it, so it was a header inline.
	void setAngle(s16 new_angle)
	{
		MtxPtr mtx = getModel()->getBaseTRMtx();
		Mtx rot;
		MTXRotRad(rot, 'y', 0.017453292f * (f32)(new_angle - mAngle));
		MTXConcat(mtx, rot, mtx);
		mAngle = new_angle;
	}

	// Type 2 has no emitters at all and type 1 only the third one, so both
	// guards are needed at every site. No map symbol names these, so they were
	// header inlines (or copy-paste); the shape is identical at all six sites.
	void stopEmitters()
	{
		if (mType == 2)
			return;
		if (mType == 0) {
			mEmitter1->stopCreateParticle();
			mEmitter0->stopCreateParticle();
		}
		mEmitter2->stopCreateParticle();
	}

	void playEmitters()
	{
		if (mType == 2)
			return;
		if (mType == 0) {
			mEmitter1->playCreateParticle();
			mEmitter0->playCreateParticle();
		}
		mEmitter2->playCreateParticle();
	}

public:
	/* 0x04 */ J3DModel* mModel;
	/* 0x08 */ J3DAnmColor* mAnmColor;
	/* 0x0C */ JGeometry::TVec3<f32> mPosition;
	/* 0x18 */ JGeometry::TVec3<f32> mOffset;
	/// Set while this shine is the selected one, which makes it spin.
	/* 0x24 */ bool mSpinning;
	/// Phase of the bobbing curve, in [0, 4).
	/* 0x28 */ f32 mBoundTimer;
	/* 0x2C */ f32 mBoundHeight;
	/* 0x30 */ f32 mBoundSpeed;
	/// Spin angle in degrees, wrapped into [0, 360].
	/* 0x34 */ int mSpinAngle;
	/* 0x38 */ s8 mSpinSpeed;
	/* 0x39 */ u8 unk39;
	/// Facing angle in degrees.
	/* 0x3A */ s16 mAngle;
	/* 0x3C */ s16 mAnmFrame;
	/* 0x3E */ u8 unk3E;
	/* 0x40 */ f32 unk40;
	/* 0x44 */ f32 unk44;
	/* 0x48 */ bool mAppearing;
	/* 0x49 */ bool mDisappearing;
	/// 0: full shine (three emitters), 1: empty shine (one emitter), 2: none.
	/* 0x4A */ u8 mType;
	/* 0x4C */ JPAEmitterManager* mEmitterManager;
	/* 0x50 */ JPABaseEmitter* mEmitter0;
	/* 0x54 */ JPABaseEmitter* mEmitter1;
	/* 0x58 */ JPABaseEmitter* mEmitter2;
};

class TSelectShineManager : public JDrama::TViewObj {
public:
	TSelectShineManager(const char* name);
	virtual ~TSelectShineManager() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void initData(u8* shine_states, u8 shine_num, u8 index,
	              JPAEmitterManager* emitter_manager);
	JGeometry::TVec3<f32> getPosition(s16 angle);
	s16 getAngle(const JGeometry::TVec3<f32>& position);
	void startClose();
	void startIncrease(int count);
	void startDecrease(int count);

	static JGeometry::TVec3<f32> cCenter;

public:
	/* 0x10 */ TSelectShine* mShines[8];
	// TODO: untouched by this TU; the eight words at 0x30 and the eight at
	// 0x58 are not written by the constructor either.
	/* 0x30 */ u8 unk30[0x20];
	/* 0x50 */ J3DDrawBuffer* mOpaBuffer;
	/* 0x54 */ J3DDrawBuffer* mXluBuffer;
	/* 0x58 */ u8 unk58[0x20];
	/* 0x78 */ f32 unk78;
	/* 0x7C */ f32 unk7C;
	/* 0x80 */ u8 unk80[0x8];
	/* 0x88 */ int mShineNum;
	/* 0x8C */ int mIndex;
	/* 0x90 */ f32 unk90;
	/* 0x94 */ f32 unk94;
	/* 0x98 */ int unk98;
	/// Scroll offset in degrees; the resting value is -40 per shine index.
	/* 0x9C */ int mScroll;
	/* 0xA0 */ f32 mScrollSpeed;
	/* 0xA4 */ bool mIncreasing;
	/* 0xA5 */ bool mDecreasing;
	/* 0xA6 */ u8 unkA6;
	/* 0xA7 */ bool mClosed;
	/* 0xA8 */ JGeometry::TVec3<f32> mPositions[8];
	// TODO: sizeof is 0x120 (TSelectDir::rsetup news 0x120), but the
	// constructor's last write is the __construct_array over mPositions,
	// which ends at 0x108, and no TU in the map reads past it. The size is
	// the only evidence for this tail.
	/* 0x108 */ u8 unk108[0x18];
};

#endif // GC2D_SELECT_SHINE_2_HPP
