#include <Camera/Camera.hpp>
#include <System/MarioGamePad.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <Camera/CameraMarioData.hpp>
#include <Camera/CameraMapTool.hpp>
#include <Player/MarioAccess.hpp>

static const char* SMS_NO_MEMORY_MESSAGE = "メモリが足りません\n";

bool CPolarSubCamera::controlByCameraCode_(int* param_1)
{
	bool result = true;
	*param_1    = -1;
	if (SMS_IsMarioOpeningDoor()) {
		if (getMode() == CAMERA_MODE_DELFINO_B
		    && gpCameraMario->getFramesSinceMarioStatusChange() == 120) {
			changeCamModeSpecifyFrame_(CAMERA_MODE_DELFINO, 1);
			warpPosAndAt(mCurrentTarget.unk28, *gpMarioAngleY + 0x9C4);
		}
		unk120->onNeutralMarioKey();
		result = false;
	} else {
		int count = gpCubeCamera->unk10;

		JGeometry::TVec3<f32> local_24 = SMS_GetMarioPos();
		local_24.y += 75.0f;
		for (int i = 0; i < count; ++i) {
			if (gpCubeCamera->isInCube(local_24, i)) {
				TCubeGeneralInfo& generalInfo = gpCubeCamera->getInfo(i);
				TCubeCameraInfo* info = (TCubeCameraInfo*)&generalInfo;

				TCameraMapTool* tool = info->getCameraMapTool();
				if (tool) {
					if (mMode != tool->getCameraMode() || tool != unk70)
						changeCamModeSpecifyCamMapTool_(tool);
					*param_1 = tool->getCameraMode();
				} else {
					*param_1 = gpCubeCamera->getDataNo(i);
				}

				return true;
			}
		}
	}

	return result;
}
