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

	f32 getNear() const { return mNear; }
	f32 getFar() const { return mFar; }

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
	virtual void perform(u32 cue, TGraphics* graphics);

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
	TLookAtCamera(const JGeometry::TVec3<f32>& position,
	              const JGeometry::TVec3<f32>& target,
	              const JGeometry::TVec3<f32>& up, f32 fovy, f32 aspect,
	              const char* name, f32 near = 50.0f, f32 far = 10000.0f)
	    : TCamera(near, far, name)
	{
		mUp       = up;
		mTarget   = target;
		mFovy     = fovy;
		mAspect   = aspect;
		mPosition = position;
	}

	virtual ~TLookAtCamera() { }
	virtual void perform(u32 cue, TGraphics* graphics);

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
	// The edges come as (left, top, bottom, right) while mField is stored
	// {left, top, right, bottom} (perform() passes mField[1], [3], [0], [2]
	// to C_MTXOrtho as top, bottom, left, right). MWCC evaluates the call's
	// arguments right to left, so retail's render-size cameras, which call
	// SMSGetGameRenderWidth() before SMSGetGameRenderHeight() and store the
	// width in mField[2], pass (..., height, width).
	TOrthoProj(f32 near, f32 far, f32 left, f32 top, f32 bottom, f32 right,
	           const char* name = "<TOrthoProj>")
	    : TCamera(near, far, name)
	{
		mField[0] = left;
		mField[1] = top;
		mField[2] = right;
		mField[3] = bottom;
	}

	virtual ~TOrthoProj() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	virtual JStage::TECameraProjection JSGGetProjectionType() const;
	virtual void JSGSetProjectionType(JStage::TECameraProjection);
	virtual void JSGGetProjectionField(f32*) const;
	virtual void JSGSetProjectionField(const f32*);

public:
	/* 0x30 */ f32 mField[4]; // TODO: maybe a TBox?
};

}; // namespace JDrama

#endif
