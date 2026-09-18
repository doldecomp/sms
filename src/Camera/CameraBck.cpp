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

// UNUSED 0x70 in the map, and our body compiles to exactly 0x70. updateDemo
// inlines it, which is what puts the whole result in r31: `finished`'s `true`
// initialiser is hoisted above the getFrameCtrl call, checkState()'s own
// ternary reuses that register (so only `li r31, 0` survives) and the outer
// ternary re-normalises it in place.
bool TCameraBck::isDemoFinished() const
{
	bool finished    = true;
	J3DFrameCtrl* fc = unk0->getFrameCtrl(ANM_TYPE_BCK);
	if (fc != nullptr)
		finished = fc->checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE)
		    ? true
		    : false;
	return finished;
}

void TCameraBck::endDemo() { unk0->setBckFromIndex(-1); }

void TCameraBck::restartDemo() { }

bool TCameraBck::updateDemo(JGeometry::TVec3<f32>* pos,
                            JGeometry::TVec3<f32>* lookat,
                            JGeometry::TVec3<f32>* up, f32* out_y_scale)
{

	getMActor()->calcAnm();

	if (pos != nullptr) {
		MtxPtr mtx = getPosMtx();
		pos->set(mtx[0][3], mtx[1][3], mtx[2][3]);
	}

	if (lookat != nullptr)
		lookat->set(unk10[0][3], unk10[1][3], unk10[2][3]);

	if (up != nullptr)
		up->set(unkC[0][1], unkC[1][1], unkC[2][1]);

	if (out_y_scale != nullptr) {
		J3DAnmTransformKey* anm = unk0->getBckAnm();
		if (anm != nullptr) {
			J3DTransformInfo info;
			anm->getTransform((u16)getFrame(), &info);
			f32 scaleY   = info.mScale.y;
			*out_y_scale = scaleY;
		}
	}

	if (getOffset() != nullptr) {
		if (pos != nullptr)
			*pos += *getOffset();
		if (lookat != nullptr)
			*lookat += *getOffset();
	}

	return isDemoFinished();
}

void TCameraBck::setFrame(f32 frame)
{
	if (unk0->getCurAnmIdx(ANM_TYPE_BCK) != -1 ? true : false) {
		J3DFrameCtrl* fc = unk0->getFrameCtrl(ANM_TYPE_BCK);
		if (fc != nullptr)
			fc->setFrame(frame);
	}
}
