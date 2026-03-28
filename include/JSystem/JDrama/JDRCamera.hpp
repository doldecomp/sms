#ifndef JDR_CAMERA_HPP
#define JDR_CAMERA_HPP

#include <JSystem/JStage/JSGCamera.hpp>
#include <JSystem/JDrama/JDRPlacement.hpp>

namespace JDrama {

class TCamera : public TPlacement, public JStage::TCamera {
public:
	TCamera(f32 near, f32 far, const char* name)
	    : TPlacement(name)
	    , mFlag(0)
	    , mNear(near)
	    , mFar(far)
	{
	}

	virtual ~TCamera();
	virtual int getType() const { return 2; }

	virtual u32 JSGGetFlag() const;
	virtual void JSGSetFlag(u32);
	virtual f32 JSGGetProjectionNear() const;
	virtual void JSGSetProjectionNear(f32);
	virtual f32 JSGGetProjectionFar() const;
	virtual void JSGSetProjectionFar(f32);

public:
	/* 0x24 */ TFlagT<u16> mFlag;
	/* 0x28 */ f32 mNear;
	/* 0x2C */ f32 mFar;
};

class TPolarCamera : public TCamera {
public:
	TPolarCamera(const char* name = "<TPolarCamera>")
	    : TCamera(50.0f, 10000.0f, name)
	    , mFovy(45.0f)
	    , mAspect(1.3333334f)
	    , unk38(0.0f)
	    , unk3C(0.0f)
	    , unk40(0.0f)
	    , unk44(1200.0f)
	{
	}
	virtual ~TPolarCamera() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, TGraphics*);

	virtual JStage::TECameraProjection JSGGetProjectionType() const;
	virtual void JSGSetProjectionType(JStage::TECameraProjection);
	virtual f32 JSGGetProjectionFovy() const;
	virtual void JSGSetProjectionFovy(f32);
	virtual f32 JSGGetProjectionAspect() const;
	virtual void JSGSetProjectionAspect(f32);

public:
	/* 0x30 */ f32 mFovy;
	/* 0x34 */ f32 mAspect;
	/* 0x38 */ f32 unk38;
	/* 0x3C */ f32 unk3C;
	/* 0x40 */ f32 unk40;
	/* 0x44 */ f32 unk44;
};

class TLookAtCamera : public TCamera {
public:
	TLookAtCamera();

	virtual ~TLookAtCamera() { }
	virtual void perform(u32, TGraphics*);

	virtual JStage::TECameraProjection JSGGetProjectionType() const;
	virtual void JSGSetProjectionType(JStage::TECameraProjection);
	virtual f32 JSGGetProjectionFovy() const;
	virtual void JSGSetProjectionFovy(f32);
	virtual f32 JSGGetProjectionAspect() const;
	virtual void JSGSetProjectionAspect(f32);
	virtual void JSGGetViewPosition(Vec*) const;
	virtual void JSGSetViewPosition(const Vec&);
	virtual void JSGGetViewUpVector(Vec*) const;
	virtual void JSGSetViewUpVector(const Vec&);
	virtual void JSGGetViewTargetPosition(Vec*) const;
	virtual void JSGSetViewTargetPosition(const Vec&);

	f32 getFovy() const { return mFovy; }
	f32 getAspect() const { return mAspect; }

public:
	/* 0x30 */ JGeometry::TVec3<f32> mUp;
	/* 0x3C */ JGeometry::TVec3<f32> mTarget;
	/* 0x48 */ f32 mFovy;
	/* 0x4C */ f32 mAspect;
};

class TOrthoProj : public TCamera {
public:
	TOrthoProj(f32 a, f32 b, f32 c, f32 d)
	    : TCamera(-1.0f, 1.0f, "<TOrthoProj>")
	{
		mField[0] = a;
		mField[1] = b;
		mField[2] = c;
		mField[3] = d;
	}

	virtual ~TOrthoProj() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);

	virtual JStage::TECameraProjection JSGGetProjectionType() const;
	virtual void JSGSetProjectionType(JStage::TECameraProjection);
	virtual void JSGGetProjectionField(f32*) const;
	virtual void JSGSetProjectionField(const f32*);

public:
	/* 0x30 */ f32 mField[4]; // TODO: maybe a TBox?
};

}; // namespace JDrama

#endif
