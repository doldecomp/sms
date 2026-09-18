#include <Camera/Camera.hpp>
#include <System/MarioGamePad.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <Camera/CameraMarioData.hpp>
#include <Camera/CameraMapTool.hpp>
#include <Player/MarioAccess.hpp>

// The three accessors below belong on their classes, not here: this batch may
// not touch the shared camera headers, so they are parked as TU-local inlines
// under prefixed names. The promotions to make later are
//   Camera/Camera.hpp:          int CPolarSubCamera::getCamMode() const
//                               const TCameraMapTool* CPolarSubCamera::getUnk70() const
//   Camera/CameraMarioData.hpp: u32 TCameraMarioData::getFramesSinceMarioStatusChange() const
//                               TCameraMarioData* SMSGetCameraMario()
// Each is exactly one inline level and the five levels used in
// controlByCameraCode_ (these three, the named TCubeGeneralInfo* and
// TCameraMapTool::getCameraMode()) are its 0x28 of otherwise missing frame.
static inline int CameraCodeGetCamMode(const CPolarSubCamera* cam)
{
	return cam->mMode;
}

static inline const TCameraMapTool*
CameraCodeGetCurCamMapTool(const CPolarSubCamera* cam)
{
	return cam->unk70;
}

static inline TCameraMarioData* CameraCodeGetMarioData() { return gpCameraMario; }

static inline u32 CameraCodeGetStatusChangeFrames(const TCameraMarioData* data)
{
	return data->mFramesSinceMarioStatusChange;
}

bool CPolarSubCamera::controlByCameraCode_(int* param_1)
{
	bool result = true;
	*param_1    = -1;
	if (SMS_IsMarioOpeningDoor()) {
		if (CameraCodeGetCamMode(this) == CAMERA_MODE_DELFINO_B
		    && CameraCodeGetStatusChangeFrames(CameraCodeGetMarioData())
		           == 120) {
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
				TCubeGeneralInfo* general = &(*gpCubeCamera->unk14)[i];
				TCubeCameraInfo* info     = (TCubeCameraInfo*)general;

				TCameraMapTool* tool = info->getCameraMapTool();
				if (tool) {
					if (CameraCodeGetCamMode(this) != tool->getCameraMode()
					    || tool != CameraCodeGetCurCamMapTool(this))
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
