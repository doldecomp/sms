#ifndef CAMERA_CAMERA_MAP_TOOL_HPP
#define CAMERA_CAMERA_MAP_TOOL_HPP

#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JGeometry.hpp>
#include <Strategic/NameRefAry.hpp>

class TCameraMapTool : public JDrama::TNameRef {
public:
	struct TPitchYaw {
		f32 pitch;
		f32 yaw;
	};

	TCameraMapTool(const char* name = "<TCameraMapTool>")
	    : JDrama::TNameRef(name)
	{
	}

	void calcPosAndAt(JGeometry::TVec3<f32>*, JGeometry::TVec3<f32>*) const;
	void load(JSUMemoryInputStream&);

	// Fabricated
	f32 getYaw() const { return mPitchYaw.yaw; }
	int getCameraMode() const { return mCameraMode; }
	int getDemoLengthFrames() const { return mDemoLengthFrames; }

public:
	/* 0xC */ JGeometry::TVec3<f32> mPosition;
	/* 0x18 */ TPitchYaw mPitchYaw;
	/* 0x20 */ u32 unk20;
	/* 0x24 */ s32 mCameraMode;
	/* 0x28 */ s32 unk28;
	/* 0x2C */ u32 mDemoLengthFrames;
};

extern TNameRefAryT<TCameraMapTool>* gpCamMapToolTable;

#endif
