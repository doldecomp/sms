#ifndef GC2D_SELECT_SHINE_2_HPP
#define GC2D_SELECT_SHINE_2_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>

class J3DModel;
class J3DModelData;
class J3DAnmColor;
class J3DDrawBuffer;
class JPAEmitterManager;
class JPABaseEmitter;

class TSelectShine {
public:
	TSelectShine(J3DModelData*, J3DAnmColor*, JPAEmitterManager*,
	             JGeometry::TVec3<f32>&, s16, u8, f32, f32, f32);
	virtual ~TSelectShine() { }
	virtual void move();

	f32 makeNewPosition(f32, f32, f32, f32);

public:
	/* 0x4 */ J3DModel* mModel;
	/* 0x8 */ J3DAnmColor* mAnmColor;
	/* 0xC */ JGeometry::TVec3<f32> mPos;
	/* 0x18 */ JGeometry::TVec3<f32> mOffset;
	/* 0x24 */ u8 mIsSelected;
	/* 0x28 */ f32 mBobPhase;
	/* 0x2C */ f32 mBobHeight;
	/* 0x30 */ f32 mBobSpeed;
	/* 0x34 */ s32 mSpinAngle;
	/* 0x38 */ s8 mSpinSpeed;
	/* 0x3A */ s16 mAngleY;
	/* 0x3C */ s16 mAnmFrame;
	/* 0x3E */ u8 unk3E;
	/* 0x40 */ f32 unk40;
	/* 0x44 */ f32 unk44;
	/* 0x48 */ u8 unk48;
	/* 0x49 */ u8 unk49;
	/* 0x4A */ u8 mType;
	/* 0x4C */ JPAEmitterManager* mEmitterManager;
	/* 0x50 */ JPABaseEmitter* mEmitter0;
	/* 0x54 */ JPABaseEmitter* mEmitter1;
	/* 0x58 */ JPABaseEmitter* mEmitter2;
};

class TSelectShineManager : public JDrama::TViewObj {
public:
	TSelectShineManager(const char*);
	virtual ~TSelectShineManager() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void initData(u8*, u8, u8, JPAEmitterManager*);
	JGeometry::TVec3<f32> getPosition(s16);
	s16 getAngle(const JGeometry::TVec3<f32>&);
	void startClose();
	void startIncrease(int);
	void startDecrease(int);

	// fabricated!!!
	void updateShine(int);

	static JGeometry::TVec3<f32> cCenter;

public:
	/* 0x10 */ TSelectShine* mShines[8];
	/* 0x30 */ u8 unk30[0x20];
	/* 0x50 */ J3DDrawBuffer* mOpaBuffer;
	/* 0x54 */ J3DDrawBuffer* mXluBuffer;
	/* 0x58 */ u8 unk58[0x20];
	/* 0x78 */ f32 unk78;
	/* 0x7C */ f32 unk7C;
	/* 0x80 */ u8 unk80[8];
	/* 0x88 */ int mShineNum;
	/* 0x8C */ int mSelected;
	/* 0x90 */ f32 unk90;
	/* 0x94 */ f32 unk94;
	/* 0x98 */ int unk98;
	/* 0x9C */ int mRingAngle;
	/* 0xA0 */ f32 mRingSpeed;
	/* 0xA4 */ u8 mIsIncreasing;
	/* 0xA5 */ u8 mIsDecreasing;
	/* 0xA6 */ u8 unkA6;
	/* 0xA7 */ u8 mIsClosing;
	/* 0xA8 */ JGeometry::TVec3<f32> unkA8[8];
	/* 0x108 */ u8 unk108[0x18];
};

#endif // GC2D_SELECT_SHINE_2_HPP
