#ifndef JPA_FIELD_HPP
#define JPA_FIELD_HPP

#include <JSystem/JSupport/JSUList.hpp>
#include <JSystem/JGeometry.hpp>

class JPAParticle;
class JPADataBlock;

class JPABaseField {
public:
	/* 0x0 */ JSULink<JPABaseField> unk0;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ JGeometry::TVec3<f32> unk18;
	/* 0x24 */ JGeometry::TVec3<f32> unk24;
	/* 0x30 */ f32 unk30;
	/* 0x34 */ f32 unk34;
	/* 0x38 */ char unk38[4];
	/* 0x3C */ f32 mMaxDistance;
	/* 0x40 */ f32 mFadeInEnd;
	/* 0x44 */ f32 mFadeOutStart;
	/* 0x48 */ f32 mFadeInStart;
	/* 0x4C */ f32 mFadeOutEnd;
	/* 0x50 */ u8 unk50;
	/* 0x51 */ u8 unk51;
	/* 0x52 */ u8 unk52;
	/* 0x53 */ u8 unk53;
	/* 0x54 */ volatile u16 unk54;
	/* 0x58 */ JGeometry::TVec3<f32> unk58;
	/* 0x64 */ JGeometry::TVec3<f32> unk64;
	/* 0x70 */ JGeometry::TVec3<f32> unk70;
	/* 0x7C */ JGeometry::TVec3<f32> unk7C;
	/* 0x88 */ f32 mMaxDistanceSq;
	/* 0x8C */ // vt

public:
	JPABaseField();

	virtual ~JPABaseField();
	virtual void set();
	virtual void affect(JPAParticle*);
	virtual bool checkMaxDistance(JGeometry::TVec3<float>&,
	                              JGeometry::TVec3<float>&);

	void calcMaxDistance();
	f32 calcFieldFadeScale(float);
	void calcFieldVelocity(JPAParticle*);
	void loadFieldBlock(JPADataBlock*);

	// from tww
	JSULink<JPABaseField>* getLinkBufferPtr() { return &unk0; }

	bool checkStatus(u32 flag) { return unk54 & flag ? true : false; }
	enum {
		STATUS_AIR_CONE                 = 0x1,
		STATUS_USE_GLOBAL_COORDS        = 0x2,
		STATUS_AIR_CLAMP_VELOCITY       = 0x4,
		STATUS_FADE_ENABLE_START_CUTOFF = 0x8,
		STATUS_FADE_ENABLE_END_CUTOFF   = 0x10,
		STATUS_FADE_ENABLE_FADE_IN      = 0x20,
		STATUS_FADE_ENABLE_FADE_OUT     = 0x40,
		STATUS_LIMIT_DISTANCE           = 0x80,
		STATUS_NEWTON_CONSTANT_FORCE    = 0x100,
	};

	// fabricated
	JGeometry::TVec3<f32>& getUnk18() { return unk18; }
};

class JPAGravityField : public JPABaseField {
public:
	JPAGravityField();

	virtual ~JPAGravityField();
	virtual void set();
};

class JPAAirField : public JPABaseField {
public:
	JPAAirField();

	virtual ~JPAAirField() { }
	virtual void set();
	virtual void affect(JPAParticle*);
};

class JPAMagnetField : public JPABaseField {
public:
	JPAMagnetField();

	virtual ~JPAMagnetField();
	virtual void set();
	virtual void affect(JPAParticle*);
};

class JPANewtonField : public JPABaseField {
public:
	JPANewtonField();

	virtual ~JPANewtonField();
	virtual void set();
	virtual void affect(JPAParticle*);
};

class JPAVortexField : public JPABaseField {
public:
	JPAVortexField();

	virtual ~JPAVortexField();
	virtual void set();
	virtual void affect(JPAParticle*);
	virtual bool checkMaxDistance(JGeometry::TVec3<float>&,
	                              JGeometry::TVec3<float>&);
};

class JPAConvectionField : public JPABaseField {
public:
	JPAConvectionField();

	virtual ~JPAConvectionField();
	virtual void set();
	virtual void affect(JPAParticle*);
	virtual bool checkMaxDistance(JGeometry::TVec3<float>&,
	                              JGeometry::TVec3<float>&);
};

class JPARandomField : public JPABaseField {
public:
	JPARandomField();

	virtual ~JPARandomField();
	virtual void affect(JPAParticle*);
};

class JPADragField : public JPABaseField {
public:
	JPADragField();

	virtual ~JPADragField();
	virtual void affect(JPAParticle*);
};

class JPAFieldManager {
public:
	/* 0x0 */ JSUList<JPABaseField> unk0;
	/* 0xC */ JSUList<JPABaseField>* unkC;

public:
	JPAFieldManager();
	~JPAFieldManager();

	void deleteField(JPABaseField*);
	void deleteAllField();
	void calcFieldParams();
	void affectField(JPAParticle*);

	JPABaseField* setField(u8);
	JPAGravityField* setGravityField();
	JPAAirField* setAirField();
	JPAMagnetField* setMagnetField();
	JPANewtonField* setNewtonField();
	JPAVortexField* setVortexField();
	JPAConvectionField* setConvectionField();
	JPARandomField* setRandomField();
	JPADragField* setDragField();
};

#endif
