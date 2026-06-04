#include <Camera/CameraOption.hpp>
#include <Camera/Camera.hpp>
#include <Camera/CameraMapTool.hpp>
#include <Camera/CubeMapTool.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <Camera/cameralib.hpp>
#include <Player/MarioAccess.hpp>
#include <JSystem/JMath.hpp>

static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

TCameraOption* gpCameraOption;
const char* cLoadCamName = "ロードカメラ";

void CPolarSubCamera::chaseOptionCamera_(f32 param_1)
{
	CLBChaseConstantSpecifyFrame<f32>(&mPosition.x, unk80.unk0.x, param_1);
	CLBChaseConstantSpecifyFrame<f32>(&mPosition.y, unk80.unk0.y, param_1);
	CLBChaseConstantSpecifyFrame<f32>(&mPosition.z, unk80.unk0.z, param_1);
	CLBChaseConstantSpecifyFrame<f32>(&mTarget.x, unk80.unkC.x, param_1);
	CLBChaseConstantSpecifyFrame<f32>(&mTarget.y, unk80.unkC.y, param_1);
	CLBChaseConstantSpecifyFrame<f32>(&mTarget.z, unk80.unkC.z, param_1);
}

void CPolarSubCamera::ctrlOptionCamera_()
{
	JGeometry::TVec3<f32> probe;

	if (gpCameraOption->unkA > 0) {
		chaseOptionCamera_(gpCameraOption->unkA);
		gpCameraOption->unkA--;
	} else if (gpCameraOption->unkE > 0) {
		chaseOptionCamera_(gpCameraOption->unkE);
		gpCameraOption->unkE--;
	} else if (!(gpCameraOption->unk0 & 0x2)) {
		probe = *gpMarioPos;
		probe.y += 75.0f;
		int cubeNo = gpCubeCamera->getInCubeNo(probe);
		if (cubeNo >= 0) {
			TCubeCameraInfo* info
			    = (TCubeCameraInfo*)&(*gpCubeCamera->unk14)[cubeNo];
			TCameraMapTool* tool = info->unk38;
			if (tool != nullptr && tool != unk70) {
				gpCameraOption->unk0 ^= 0x1;
				unk70 = tool;
				unk70->calcPosAndAt(&unk80.unk0, &unk80.unkC);
				gpCameraOption->unk12 = gpCameraOption->unk10;
			}
		}

		if (gpCameraOption->unk12 > 0) {
			chaseOptionCamera_(gpCameraOption->unk12);
			gpCameraOption->unk12--;
		} else if (gpCameraOption->unk16 > 0) {
			chaseOptionCamera_(gpCameraOption->unk16);
			gpCameraOption->unk16--;
		}
	}

	unk124.x = mPosition.x;
	unk124.y = mPosition.y;
	unk124.z = mPosition.z;
	unk148.x = mTarget.x;
	unk148.y = mTarget.y;
	unk148.z = mTarget.z;
	mFovy    = gpCameraOption->mFovYunk4;
}

TCameraOption::TCameraOption(JGeometry::TVec3<f32> param1,
                             JGeometry::TVec3<f32>* param2)
{
	unk0      = 2;
	mFovYunk4 = 40.0f;
	unk8      = 300;
	unkA      = 300;
	unkC      = 120;
	unkE      = 0;
	unk10     = 80;
	unk12     = 0;
	unk14     = 60;
	unk16     = 0;
	unk18.set(0.0f, 0.0f, 0.0f);
	unk24.set(0.0f, 0.0f, 0.0f);
	unk30.set(0.0f, 0.0f, 0.0f);
	unk3C = param2;

	s16 v1 = CLBRoundf<s16>(DEG2SHORTANGLE(-73.0f));
	s16 v2 = CLBRoundf<s16>(DEG2SHORTANGLE(54.0f));
	CLBPolarToCross(param1, &unk18, 1000.0f, v2, v1);
	param2->set(unk18);

	// TODO: inline doesn't work?!
	TCameraMapTool* tool = (TCameraMapTool*)gpCamMapToolTable->searchF(
	    JDrama::TNameRef::calcKeyCode(cLoadCamName), cLoadCamName);

	if (tool != nullptr) {
		JGeometry::TVec3<f32> origin;
		tool->calcPosAndAt(&origin, &unk24);
		s16 a = CLBRoundf<s16>(DEG2SHORTANGLE(tool->getThing()));
		s16 b = CLBRoundf<s16>(DEG2SHORTANGLE(60.0f));
		CLBPolarToCross(origin, &unk30, 1000.0f, b, a);
	}
}

void TCameraOption::moveToLoadFromTitle()
{
	unk3C->set(unk24);
	unkE = unkC;
	unk0 &= ~0x2;
}

void TCameraOption::moveToTitleFromLoad() { }

void TCameraOption::moveToUp()
{
	unk3C->set(unk30);
	unk16 = unk14;
}

void TCameraOption::moveToDown()
{
	unk3C->set(unk24);
	unk16 = unk14;
}
