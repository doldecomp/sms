#include <Camera/CameraMapTool.hpp>
#include <Camera/cameralib.hpp>
#include <Camera/Camera.hpp>

TNameRefAryT<TCameraMapTool>* gpCamMapToolTable;

void TCameraMapTool::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);

	stream >> mPosition.x >> mPosition.y >> mPosition.z >> mPitchYaw.x
	    >> mPitchYaw.y;

	stream >> unk20;
	stream >> mCameraMode;
	stream >> unk28;
	stream >> mDemoLengthFrames;

	if (unk28 < 0)
		unk28 = 0;
}

void TCameraMapTool::calcPosAndAt(JGeometry::TVec3<f32>* pos,
                                  JGeometry::TVec3<f32>* at) const
{
	pos->set(mPosition);

	if (gpCamera->isFixCameraSpecifyMode(mCameraMode)) {
		CLBPolarToCross(*pos, at, 1000.0f, CLBDegToShortAngle(-mPitchYaw.x),
		                CLBDegToShortAngle(mPitchYaw.y));
	} else {
		at->set(gpCamera->getUsualLookat());
	}
}
