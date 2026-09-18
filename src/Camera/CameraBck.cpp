#include <Camera/CameraBck.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorData.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MarioUtil/ModelUtil.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>

static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

static const char* MtxCalcTypeName[] = {
	"MActorMtxCalcType_Basic クラシックスケールＯＮ",
	"MActorMtxCalcType_Softimage クラシックスケールＯＦＦ",
	"MActorMtxCalcType_MotionBlend モーションブレンド",
	"MActorMtxCalcType_User ユーザー定義",
};

static const char* dummy[] = {
	"/common/camera/camera_demo_shine_get_inside",
	"/common/camera/camera_demo_shine_get_outside",
	"/common/camera/camera_demo_gate_in",
};

const char* cPositionJointName   = "cam_position";
const char* cLookatJointName     = "cam_interest";
const char* cCameraBckVolumeName = "/scene/map/camera";

static const char* sAddBckFileNameTable[] = {
	"/common/camera/camera_demo_shine_get_inside.bck",
	"/common/camera/camera_demo_shine_get_outside.bck",
	"/common/camera/camera_demo_gate_in.bck",
	nullptr,
};

TCameraBck::TCameraBck()
{
	unk14 = nullptr;
	SDLModel* sdlMdl
	    = SMS_CreateMinimumSDLModel("/common/camera/camera_model.bmd");
	unk4 = new MActorAnmData;
	unk4->init(cCameraBckVolumeName, sAddBckFileNameTable);
	unk0 = new MActor(unk4);
	unk0->setModel(sdlMdl, 0);
	JUTNameTab* nameTab = sdlMdl->getModelData()->getJointName();
	unk8                = nameTab->getIndex(cPositionJointName);
	unkC                = sdlMdl->getAnmMtx((u16)unk8);
	u16 idx             = nameTab->getIndex(cLookatJointName);
	unk10               = sdlMdl->getAnmMtx(idx);
}

bool TCameraBck::isFileExist(const char* name) const
{
	return unk0->checkAnmFileExist(name, ANM_TYPE_BCK);
}

void TCameraBck::getDemoFileName() const { }

void TCameraBck::isNowDemo() const { }

void TCameraBck::startDemo(const char* name,
                           const JGeometry::TVec3<f32>* offset)
{
	unk0->setBck(name);
	unk14 = offset;
}

int TCameraBck::getTotalDemoFrames() const
{
	int total              = 0;
	const J3DFrameCtrl* fc = unk0->getFrameCtrl(ANM_TYPE_BCK);
	if (fc != nullptr) {
		if (fc->getAttribute() != 0)
			total = -1;
		else
			total = (fc->getEnd() + 1) * 2;
	}
	return total;
}

void TCameraBck::isDemoFinished() const { }

void TCameraBck::endDemo() { unk0->setBckFromIndex(-1); }

void TCameraBck::restartDemo() { }

// TODO: promote these to TCameraBck members in CameraBck.hpp; parked here
// because a header batch owns the shared headers this batch.
static inline MActor* CameraBck_getMActor(TCameraBck* self)
{
	return self->unk0;
}

static inline MtxPtr CameraBck_getPosMtx(TCameraBck* self)
{
	return self->unkC;
}

static inline MtxPtr CameraBck_getLookatMtx(TCameraBck* self)
{
	return self->unk10;
}

static inline u32 CameraBck_getFrame(TCameraBck* self)
{
	return self->unk8;
}

static inline const JGeometry::TVec3<f32>* CameraBck_getOffset(
    TCameraBck* self)
{
	return self->unk14;
}

bool TCameraBck::updateDemo(JGeometry::TVec3<f32>* pos,
                            JGeometry::TVec3<f32>* lookat,
                            JGeometry::TVec3<f32>* up, f32* out_y_scale)
{

	CameraBck_getMActor(this)->calcAnm();

	if (pos != nullptr) {
		MtxPtr mtx = CameraBck_getPosMtx(this);
		pos->set(mtx[0][3], mtx[1][3], mtx[2][3]);
	}

	if (lookat != nullptr)
		lookat->set(unk10[0][3], unk10[1][3], unk10[2][3]);

	if (up != nullptr)
		up->set(unkC[0][1], unkC[1][1], unkC[2][1]);

	if (out_y_scale != nullptr) {
		J3DAnmTransformKey* anm = CameraBck_getMActor(this)->getBckAnm();
		if (anm != nullptr) {
			J3DTransformInfo info;
			anm->getTransform((u16)CameraBck_getFrame(this), &info);
			f32 scaleY   = info.mScale.y;
			*out_y_scale = scaleY;
		}
	}

	if (CameraBck_getOffset(this) != nullptr) {
		if (pos != nullptr)
			*pos += *CameraBck_getOffset(this);
		if (lookat != nullptr)
			*lookat += *CameraBck_getOffset(this);
	}

	// TODO: one instruction from exact. Retail materialises checkState()'s
	// bool straight into `result`'s register (r31, already 1), so its
	// `li 1` is elided and only `li r31, 0` survives; the second
	// cmpwi/li-1/li-0 block then re-normalises the same register. Ours
	// materialises into r0 and copies. Rejected: `result = checkState()`
	// (plain, ternary, or followed by `result = result ? true : false`),
	// `return result ? true : false` (neg/subic/subfe conversion),
	// `BOOL result`/`BOOL finished` (same conversion), an empty then with
	// `result = false` in the else, early `return true` plus a ternary
	// return, and one pure `fc != nullptr ? ... : true` ternary (all move
	// the value into r3/r0 instead). Likely a coalescing difference that
	// needs the assignment and the normalisation to be one statement.
	bool result = true;
	J3DFrameCtrl* fc
	    = CameraBck_getMActor(this)->getFrameCtrl(ANM_TYPE_BCK);
	if (fc != nullptr) {
		if (fc->checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE))
			result = true;
		else
			result = false;
	}

	return result;
}

void TCameraBck::setFrame(f32 frame)
{
	if (unk0->getCurAnmIdx(ANM_TYPE_BCK) != -1 ? true : false) {
		J3DFrameCtrl* fc = unk0->getFrameCtrl(ANM_TYPE_BCK);
		if (fc != nullptr)
			fc->setFrame(frame);
	}
}
