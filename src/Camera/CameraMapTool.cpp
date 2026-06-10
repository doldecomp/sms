#include <Camera/CameraMapTool.hpp>
#include <Camera/cameralib.hpp>
#include <Camera/Camera.hpp>

TNameRefAryT<TCameraMapTool>* gpCamMapToolTable;

void TCameraMapTool::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);

	stream >> unkC.x >> unkC.y >> unkC.z >> unk18.x >> unk18.y;

	stream >> unk20;
	stream >> mCameraMode;
	stream >> unk28;
	stream >> mDemoLengthFrames;

	if (unk28 < 0)
		unk28 = 0;
}

void TCameraMapTool::calcPosAndAt(JGeometry::TVec3<f32>* param_1,
                                  JGeometry::TVec3<f32>* param_2) const
{
	param_1->set(unkC);

	if (gpCamera->isFixCameraSpecifyMode(mCameraMode)) {
		CLBPolarToCross(*param_1, param_2, 1000.0f,
		                CLBDegToShortAngle(-unk18.x),
		                CLBDegToShortAngle(unk18.y));
	} else {
		param_2->set(gpCamera->getUsualLookat());
	}
}
