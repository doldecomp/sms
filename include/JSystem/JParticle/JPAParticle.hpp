#ifndef JPA_PARTICLE_HPP
#define JPA_PARTICLE_HPP

#include <JSystem/JParticle/JPACallback.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <JSystem/JGeometry.hpp>

class JPABaseEmitter;

class JPABaseParticle {
public:
	/* 0x0 */ JSULink<JPABaseParticle> unk0;
	/* 0x10 */ u32 unk10;
	/* 0x14 */ JGeometry::TVec3<f32> unk14;
	/* 0x20 */ JGeometry::TVec3<f32> unk20;
	/* 0x2C */ JGeometry::TVec3<f32> unk2C;
	/* 0x38 */ Vec unk38;
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

	virtual void* getDrawParamPPtr() = 0;
	virtual void* getDrawParamCPtr() = 0;

	virtual void getWidth()  = 0;
	virtual void getHeight() = 0;

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

	// fabricated
	bool checkFlag(u32 flag) const { return (unk10 & flag) ? true : false; }
};

class JPAParticle : public JPABaseParticle {
public:
	/* 0x5C */ JGeometry::TVec3<f32> unk5C;
	/* 0x68 */ JGeometry::TVec3<f32> unk68;
	/* 0x74 */ f32 unk74;
	/* 0x78 */ f32 unk78;
	/* 0x7C */ f32 unk7C;
	/* 0x80 */ f32 unk80;
	/* 0x84 */ f32 unk84;
	/* 0x88 */ JGeometry::TVec3<f32> unk88;
	/* 0x94 */ JGeometry::TVec3<f32> unk94;
	/* 0xA0 */ char unkA0[0xB0 - 0xA0];
	/* 0xB0 */ f32 unkB0;
	/* 0xB4 */ f32 unkB4;

public:
	JPAParticle();
	~JPAParticle();

	virtual void init();
	virtual void calcVelocity();
	virtual void setVelocity();
	virtual bool checkCreateChildParticle();

	virtual JGeometry::TVec3<f32>& accessFVelVec() { return unk88; }
	virtual JGeometry::TVec3<f32>& accessFAccVec() { return unk94; }
	virtual void getBaseVelVec(JGeometry::TVec3<float>&) const;
	virtual JGeometry::TVec3<f32>& accessBaseVelVec() { return unk5C; }
	virtual void setBaseVelVec(const JGeometry::TVec3<float>&);
	virtual void addBaseVelVec(const JGeometry::TVec3<float>&);

	virtual f32 getDynamicsWeight() const { return unk74; }
	virtual f32 getAirResistance() const { return unk7C; }
	virtual f32 getCurrentDragForce() const { return unk84; }
	virtual f32 getDragForce() const { return unk80; }

	virtual void* getDrawParamPPtr() { return &unkA0; }
	virtual void* getDrawParamCPtr() { return &unkA0; }

	virtual void getWidth() { }
	virtual void getHeight() { }
};

#endif
