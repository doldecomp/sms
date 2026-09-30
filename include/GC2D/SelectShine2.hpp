#ifndef GC2D_SELECT_SHINE_2_HPP
#define GC2D_SELECT_SHINE_2_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>

class J3DAnmColor;
class J3DDrawBuffer;
class J3DModel;
class J3DModelData;
class JPABaseEmitter;

// One rotating shine sprite in the file select screen.
class TSelectShine {
public:
	TSelectShine(J3DModelData* model_data, J3DAnmColor* anm_color,
	             JPAEmitterManager* emitter_manager,
	             JGeometry::TVec3<f32>& position, s16 angle, u8 kind,
	             f32 param_7, f32 param_8, f32 param_9);
	virtual ~TSelectShine() { }
	virtual void move();

	void makeNewPosition(f32, f32, f32, f32);

public:
	/* 0x04 */ J3DModel* mModel;
	/* 0x08 */ J3DAnmColor* mAnmColor;
	/* 0x0C */ JGeometry::TVec3<f32> mPosition;
	/* 0x18 */ JGeometry::TVec3<f32> unk18;
	/* 0x24 */ u8 mIsSpinning;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ f32 unk2C;
	/* 0x30 */ f32 unk30;
	/* 0x34 */ s32 unk34;
	/* 0x38 */ s8 unk38;
	/* 0x3A */ s16 unk3A;
	/* 0x3C */ s16 unk3C;
	/* 0x3E */ s8 unk3E;
	/* 0x40 */ f32 unk40;
	/* 0x44 */ f32 unk44;
	/* 0x48 */ u8 unk48;
	/* 0x49 */ u8 unk49;
	/* 0x4A */ u8 mKind;
	/* 0x4C */ JPAEmitterManager* mEmitterManager;
	/* 0x50 */ JPABaseEmitter* unk50;
	/* 0x54 */ JPABaseEmitter* unk54;
	/* 0x58 */ JPABaseEmitter* unk58;
};

class TSelectShineManager : public JDrama::TViewObj {
public:
	TSelectShineManager(const char*);
	virtual ~TSelectShineManager() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void initData(u8*, u8, u8, JPAEmitterManager*);
	void startClose();
	void startIncrease(int);
	void startDecrease(int);

	JGeometry::TVec3<f32> getPosition(s16);
	s16 getAngle(const JGeometry::TVec3<f32>&);

	// fabricated: one inline layer that the target shows around getAngle
	s16 getShineAngle(JGeometry::TVec3<f32> pos) { return getAngle(pos); }

	// fabricated: one inline layer that the target shows around getPosition
	JGeometry::TVec3<f32> getShinePosition(int index)
	{
		return getPosition(mAngle + index * 40);
	}

	static JGeometry::TVec3<f32> cCenter;

public:
	/* 0x10 */ TSelectShine* mShines[8];
	/* 0x30 */ u8 unk30[0x20];
	/* 0x50 */ J3DDrawBuffer* mDrawBuffer1;
	/* 0x54 */ J3DDrawBuffer* mDrawBuffer2;
	/* 0x58 */ u8 unk58[0x20];
	/* 0x78 */ f32 unk78;
	/* 0x7C */ f32 unk7C;
	/* 0x80 */ u8 unk80[8];
	/* 0x88 */ int mShineNum;
	/* 0x8C */ int mCurrent;
	/* 0x90 */ f32 unk90;
	/* 0x94 */ f32 unk94;
	/* 0x98 */ int unk98;
	/* 0x9C */ int mAngle;
	/* 0xA0 */ f32 mAngleSpeed;
	/* 0xA4 */ u8 unkA4;
	/* 0xA5 */ u8 unkA5;
	/* 0xA6 */ u8 unkA6;
	/* 0xA7 */ u8 unkA7;
	/* 0xA8 */ JGeometry::TVec3<f32> unkA8[8];
	/* 0x108 */ u8 unk108[0x18];
};

#endif // GC2D_SELECT_SHINE_2_HPP
