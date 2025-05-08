#ifndef JDR_CAMERA_HPP
#define JDR_CAMERA_HPP

#include <JSystem/JStage/JSGCamera.hpp>
#include <JSystem/JDrama/JDRPlacement.hpp>

namespace JDrama {

class TCamera : public TPlacement, public JStage::TCamera {
public:
	TCamera(float near, float far, const char* name)
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
	virtual float JSGGetProjectionNear() const;
	virtual void JSGSetProjectionNear(float);
	virtual float JSGGetProjectionFar() const;
	virtual void JSGSetProjectionFar(float);

public:
	/* 0x24 */ TFlagT<u16> mFlag;
	/* 0x28 */ float mNear;
	/* 0x2C */ float mFar;
};

class TPolarCamera : public TCamera {
public:
	TPolarCamera()
	    : TCamera(50.0f, 10000.0f, "<TPolarCamera>")
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
	virtual float JSGGetProjectionFovy() const;
	virtual void JSGSetProjectionFovy(float);
	virtual float JSGGetProjectionAspect() const;
	virtual void JSGSetProjectionAspect(float);

public:
	/* 0x30 */ float mFovy;
	/* 0x34 */ float mAspect;
	/* 0x38 */ float unk38;
	/* 0x3C */ float unk3C;
	/* 0x40 */ float unk40;
	/* 0x44 */ float unk44;
};

class TLookAtCamera : public TCamera {
public:
	TLookAtCamera();

	virtual ~TLookAtCamera() { }
	virtual void perform(u32, TGraphics*);

	virtual JStage::TECameraProjection JSGGetProjectionType() const;
	virtual void JSGSetProjectionType(JStage::TECameraProjection);
	virtual float JSGGetProjectionFovy() const;
	virtual void JSGSetProjectionFovy(float);
	virtual float JSGGetProjectionAspect() const;
	virtual void JSGSetProjectionAspect(float);
	virtual void JSGGetViewPosition(Vec*) const;
	virtual void JSGSetViewPosition(const Vec&);
	virtual void JSGGetViewUpVector(Vec*) const;
	virtual void JSGSetViewUpVector(const Vec&);
	virtual void JSGGetViewTargetPosition(Vec*) const;
	virtual void JSGSetViewTargetPosition(const Vec&);

public:
	/* 0x30 */ JGeometry::TVec3<f32> mUp;
	/* 0x3C */ JGeometry::TVec3<f32> mTarget;
	/* 0x48 */ float mFovy;
	/* 0x4C */ float mAspect;
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
	virtual void JSGGetProjectionField(float*) const;
	virtual void JSGSetProjectionField(const float*);

public:
	/* 0x30 */ float mField[4]; // TODO: maybe a TBox?
};

}; // namespace JDrama

#endif
