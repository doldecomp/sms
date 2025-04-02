#ifndef JPA_PARTICLE_HPP
#define JPA_PARTICLE_HPP

#include <JSystem/JParticle/JPACallback.hpp>
#include <JSystem/JParticle/JPADraw.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <JSystem/JGeometry.hpp>
#include <dolphin/gx/GXStruct.h>

class JPABaseEmitter;

// fabricated
struct JPADrawParams {
	/* 0x0 */ Vec unk0;
	/* 0x8 */ f32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ char unk18[8];
	/* 0x20 */ f32 unk20;
	/* 0x24 */ f32 unk24;
	/* 0x28 */ s32 unk28;
	/* 0x2C */ GXColor unk2C;
	/* 0x30 */ GXColor unk30;
	/* 0x34 */ u16 unk34;
	/* 0x36 */ s16 unk36;
	/* 0x38 */ char unk38[2];
	/* 0x3A */ u16 unk3A;
};

class JPABaseParticle {
public:
	/* 0x0 */ JSULink<JPABaseParticle> unk0;
	/* 0x10 */ u32 unk10;
	/* 0x14 */ JGeometry::TVec3<f32> unk14;
	/* 0x20 */ JGeometry::TVec3<f32> mLocalPosition;
	/* 0x2C */ JGeometry::TVec3<f32> mGlobalPosition;
	/* 0x38 */ JGeometry::TVec3<f32> unk38;
	/* 0x44 */ f32 unk44;
	/* 0x48 */ f32 unk48;
	/* 0x4C */ f32 unk4C;
	/* 0x50 */ JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>* unk50;
	/* 0x54 */ u32 unk54;
	/* 0x58 */ // vt

public:
	JPABaseParticle();
	~JPABaseParticle();

	virtual void init()                     = 0;
	virtual void calcVelocity()             = 0;
	virtual void setVelocity()              = 0;
	virtual bool checkCreateChildParticle() = 0;

	virtual void executeParticleCallBack(JPABaseEmitter*);
	virtual void drawParticleCallBack(JPABaseEmitter*);

	virtual JGeometry::TVec3<f32>& accessFVelVec()             = 0;
	virtual JGeometry::TVec3<f32>& accessFAccVec()             = 0;
	virtual void getBaseVelVec(JGeometry::TVec3<float>&) const = 0;
	virtual JGeometry::TVec3<f32>& accessBaseVelVec()          = 0;
	virtual void setBaseVelVec(const JGeometry::TVec3<float>&) = 0;
	virtual void addBaseVelVec(const JGeometry::TVec3<float>&) = 0;

	virtual f32 getDynamicsWeight() const   = 0;
	virtual f32 getAirResistance() const    = 0;
	virtual f32 getCurrentDragForce() const = 0;
	virtual f32 getDragForce() const        = 0;

	virtual JPADrawParams* getDrawParamPPtr() = 0;
	virtual JPADrawParams* getDrawParamCPtr() = 0;

	virtual f32 getWidth()  = 0;
	virtual f32 getHeight() = 0;

	void initBase();
	void incTimer();
	void initGlobalPosition();
	void calcGlobalPosition();
	void getCurrentPosition(JGeometry::TVec3<float>&);
	void getCurrentPositionX();
	void getCurrentPositionY();
	void getCurrentPositionZ();

	// from TWW
	JSULink<JPABaseParticle>* getLinkBufferPtr() { return &unk0; }
	void getGlobalPosition(JGeometry::TVec3<f32>& out) const
	{
		out.set(mGlobalPosition);
	}
	void getLocalPosition(JGeometry::TVec3<f32>& out) const
	{
		out.set(mLocalPosition);
	}

	// fabricated
	bool checkFlag(u32 flag) const { return (unk10 & flag) ? true : false; }
	s32 getAge() const { return unk44; } // TODO: name might be wrong
	bool isInvisibleParticle() { return checkFlag(8); }
};

class JPAParticle : public JPABaseParticle {
public:
	/* 0x5C */ JGeometry::TVec3<f32> mBaseVelocity;
	/* 0x68 */ JGeometry::TVec3<f32> unk68;
	/* 0x74 */ f32 mDynamicsWeight;
	/* 0x78 */ f32 unk78;
	/* 0x7C */ f32 mAirResistance;
	/* 0x80 */ f32 mDragForce;
	/* 0x84 */ f32 mCurrentDragForce;
	/* 0x88 */ JGeometry::TVec3<f32> mVelocity;
	/* 0x94 */ JGeometry::TVec3<f32> mAcceleration;
	/* 0xA0 */ JPADrawParams mDrawParams;

public:
	JPAParticle();
	~JPAParticle();

	virtual void init();
	virtual void calcVelocity();
	virtual void setVelocity();
	virtual bool checkCreateChildParticle();

	virtual JGeometry::TVec3<f32>& accessFVelVec() { return mVelocity; }
	virtual JGeometry::TVec3<f32>& accessFAccVec() { return mAcceleration; }
	virtual void getBaseVelVec(JGeometry::TVec3<float>&) const;
	virtual JGeometry::TVec3<f32>& accessBaseVelVec() { return mBaseVelocity; }
	virtual void setBaseVelVec(const JGeometry::TVec3<float>& v)
	{
		mBaseVelocity.set(v);
	}
	virtual void addBaseVelVec(const JGeometry::TVec3<float>& v)
	{
		mBaseVelocity.add(v);
	}

	virtual f32 getDynamicsWeight() const { return mDynamicsWeight; }
	virtual f32 getAirResistance() const { return mAirResistance; }
	virtual f32 getCurrentDragForce() const { return mCurrentDragForce; }
	virtual f32 getDragForce() const { return mDragForce; }

	virtual JPADrawParams* getDrawParamPPtr() { return &mDrawParams; }
	virtual JPADrawParams* getDrawParamCPtr() { return &mDrawParams; }

	virtual f32 getWidth()
	{
		return mDrawParams.unk10 * 2.0f * JPADraw::cb.unk4;
	}
	virtual f32 getHeight()
	{
		return mDrawParams.unk14 * 2.0f * JPADraw::cb.unk8;
	}
};

#endif
