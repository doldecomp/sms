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
const char* cLoadCamName = "左サイドカメラ";

// Parked here, not in <Camera/CameraOption.hpp>, because a header batch is
// running: the house global-accessor idiom (cf. SMSGetCamera,
// SMSGetMarDirector, SMSGetPollution). Promotion to the header is a header
// item. Measured: each read routed through this level is +4 bytes of
// low-region frame in ctrlOptionCamera_ with no instruction change, and the
// four guard reads plus the four chaseOptionCamera_ arguments - eight sites -
// are exactly the 32 bytes retail's frame has over the raw spelling. The
// in-place decrements and the unk0/unk10/mFovY accesses stay raw.
static inline TCameraOption* SMSGetCameraOption() { return gpCameraOption; }

void CPolarSubCamera::chaseOptionCamera_(f32 param_1)
{
	CLBChaseConstantSpecifyFrame<f32>(&mPosition.x, mCurrentTarget.mPosition.x,
	                                  param_1);
	CLBChaseConstantSpecifyFrame<f32>(&mPosition.y, mCurrentTarget.mPosition.y,
	                                  param_1);
	CLBChaseConstantSpecifyFrame<f32>(&mPosition.z, mCurrentTarget.mPosition.z,
	                                  param_1);
	CLBChaseConstantSpecifyFrame<f32>(&mTarget.x, mCurrentTarget.mTarget.x,
	                                  param_1);
	CLBChaseConstantSpecifyFrame<f32>(&mTarget.y, mCurrentTarget.mTarget.y,
	                                  param_1);
	CLBChaseConstantSpecifyFrame<f32>(&mTarget.z, mCurrentTarget.mTarget.z,
	                                  param_1);
}

void CPolarSubCamera::ctrlOptionCamera_()
{
	JGeometry::TVec3<f32> probe;

	if (SMSGetCameraOption()->unkA > 0) {
		chaseOptionCamera_(SMSGetCameraOption()->unkA);
		gpCameraOption->unkA--;
	} else if (SMSGetCameraOption()->unkE > 0) {
		chaseOptionCamera_(SMSGetCameraOption()->unkE);
		gpCameraOption->unkE--;
	} else if (!(gpCameraOption->unk0 & 0x2)) {
		probe = *gpMarioPos;
		probe.y += 75.0f;
		int cubeNo = gpCubeCamera->getInCubeNo(probe);
		if (cubeNo >= 0) {
			TCubeCameraInfo* info
			    = (TCubeCameraInfo*)(*gpCubeCamera->unk14)[cubeNo];
			TCameraMapTool* tool = info->unk38;
			if (tool != nullptr && tool != unk70) {
				gpCameraOption->unk0 ^= 0x1;
				unk70 = tool;
				unk70->calcPosAndAt(&mCurrentTarget.mPosition,
				                    &mCurrentTarget.mTarget);
				gpCameraOption->unk12 = gpCameraOption->unk10;
			}
		}

		if (SMSGetCameraOption()->unk12 > 0) {
			chaseOptionCamera_(SMSGetCameraOption()->unk12);
			gpCameraOption->unk12--;
		} else if (SMSGetCameraOption()->unk16 > 0) {
			chaseOptionCamera_(SMSGetCameraOption()->unk16);
			gpCameraOption->unk16--;
		}
	}

	unk124.set(mPosition);
	unk148.set(mTarget);
	mFovy    = gpCameraOption->mFovY;
}

TCameraOption::TCameraOption(JGeometry::TVec3<f32> param1,
                             JGeometry::TVec3<f32>* param2)
{
	unk0  = 2;
	mFovY = 40.0f;
	unk8  = 300;
	unkA  = 300;
	unkC  = 120;
	unkE  = 0;
	unk10 = 80;
	unk12 = 0;
	unk14 = 60;
	unk16 = 0;
	unk18.set(0.0f, 0.0f, 0.0f);
	unk24.set(0.0f, 0.0f, 0.0f);
	unk30.set(0.0f, 0.0f, 0.0f);
	unk3C = param2;

	s16 v1 = CLBRoundf<s16>(DEG2SHORTANGLE(-73.0f));
	s16 v2 = CLBRoundf<s16>(DEG2SHORTANGLE(54.0f));
	CLBPolarToCross(param1, &unk18, 1000.0f, v2, v1);
	param2->set(unk18);

	TCameraMapTool* tool = (TCameraMapTool*)gpCamMapToolTable->searchF(
	    JDrama::TNameRef::calcKeyCode(cLoadCamName), cLoadCamName);

	if (tool != nullptr) {
		JGeometry::TVec3<f32> origin;
		tool->calcPosAndAt(&origin, &unk24);
		f32 yaw = tool->getYaw();
		s16 a   = CLBRoundf<s16>(DEG2SHORTANGLE(yaw));
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

// UNUSED (map 0x34). The exact mirror of moveToLoadFromTitle, which is
// also 0x34: it moves the option camera back to the title-screen pose
// (unk18, the polar position the constructor builds) and re-arms the
// "no cube tracking" bit that moveToLoadFromTitle clears.
void TCameraOption::moveToTitleFromLoad()
{
	unk3C->set(unk18);
	unkE = unkC;
	unk0 |= 0x2;
}

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
