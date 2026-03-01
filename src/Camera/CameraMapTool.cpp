#include <Camera/CameraMapTool.hpp>
#include <Camera/cameralib.hpp>
#include <Camera/Camera.hpp>

TNameRefAryT<TCameraMapTool>* gpCamMapToolTable;

void TCameraMapTool::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);

	stream.read(&unkC.x, 4);
	stream.read(&unkC.y, 4);
	stream.read(&unkC.z, 4);

	stream.read(&unk18.x, 4);
	stream.read(&unk18.y, 4);

	stream.read(&unk20, sizeof(unk20));
	stream.read(&unk24, sizeof(unk24));
	stream.read(&unk28, sizeof(unk28));
	stream.read(&unk2C, sizeof(unk2C));

	if (unk28 < 0)
		unk28 = 0;
}

void TCameraMapTool::calcPosAndAt(JGeometry::TVec3<f32>* param_1,
                                  JGeometry::TVec3<f32>* param_2) const
{
	param_1->set(unkC);

	if (gpCamera->isFixCameraSpecifyMode(unk24)) {
		CLBPolarToCross(*param_1, param_2, 1000.0f,
		                CLBDegToShortAngle(-unk18.x),
		                CLBDegToShortAngle(unk18.y));
	} else {
		param_2->set(gpCamera->getUsualLookat());
	}
}
