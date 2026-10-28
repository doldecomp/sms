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
	mTranslationOffset = nullptr;
	SDLModel* sdlMdl
	    = SMS_CreateMinimumSDLModel("/common/camera/camera_model.bmd");
	mAnmData = new MActorAnmData;
	mAnmData->init(cCameraBckVolumeName, sAddBckFileNameTable);
	mActor = new MActor(mAnmData);
	mActor->setModel(sdlMdl, 0);
	JUTNameTab* nameTab = sdlMdl->getModelData()->getJointName();
	mPositionJointIndex = nameTab->getIndex(cPositionJointName);
	mPositionJointMtx   = sdlMdl->getAnmMtx((u16)mPositionJointIndex);
	u16 idx             = nameTab->getIndex(cLookatJointName);
	mLookatJointMtx     = sdlMdl->getAnmMtx(idx);
}

bool TCameraBck::isFileExist(const char* name) const
{
	return mActor->checkAnmFileExist(name, ANM_TYPE_BCK);
}

void TCameraBck::getDemoFileName() const { }

bool TCameraBck::isNowDemo() const
{
	if (mActor->getCurAnmIdx(ANM_TYPE_BCK) != -1)
		return true;
	return false;
}

void TCameraBck::startDemo(const char* name,
                           const JGeometry::TVec3<f32>* offset)
{
	mActor->setBck(name);
	mTranslationOffset = offset;
}

int TCameraBck::getTotalDemoFrames() const
{
	int total              = 0;
	const J3DFrameCtrl* fc = mActor->getFrameCtrl(ANM_TYPE_BCK);
	if (fc != nullptr) {
		if (fc->getAttribute() != 0)
			total = -1;
		else
			total = (fc->getEnd() + 1) * 2;
	}
	return total;
}

bool TCameraBck::isDemoFinished() const
{
	bool result      = true;
	J3DFrameCtrl* fc = mActor->getFrameCtrl(ANM_TYPE_BCK);
	if (fc != nullptr) {
		result
		    = fc->checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE) ? true : false;
	}
	return result;
}

void TCameraBck::endDemo() { mActor->setBckFromIndex(-1); }

void TCameraBck::restartDemo() { }

bool TCameraBck::updateDemo(JGeometry::TVec3<f32>* pos,
                            JGeometry::TVec3<f32>* lookat,
                            JGeometry::TVec3<f32>* up, f32* out_y_scale)
{
	mActor->calcAnm();

	if (pos != nullptr)
		pos->set(mPositionJointMtx[0][3], mPositionJointMtx[1][3],
		         mPositionJointMtx[2][3]);

	if (lookat != nullptr)
		lookat->set(mLookatJointMtx[0][3], mLookatJointMtx[1][3],
		            mLookatJointMtx[2][3]);

	if (up != nullptr)
		up->set(mPositionJointMtx[0][1], mPositionJointMtx[1][1],
		        mPositionJointMtx[2][1]);

	if (out_y_scale != nullptr) {
		J3DAnmTransformKey* anm = mActor->getCurBckAnmPtr();
		if (anm != nullptr) {
			J3DTransformInfo info;
			anm->getTransform(mPositionJointIndex, &info);
			*out_y_scale = info.mScale.y;
		}
	}

	if (mTranslationOffset != nullptr) {
		if (pos != nullptr)
			pos->add(*mTranslationOffset);
		if (lookat != nullptr)
			lookat->add(*mTranslationOffset);
	}

	return isDemoFinished();
}

void TCameraBck::setFrame(f32 frame)
{
	if (isNowDemo()) {
		J3DFrameCtrl* fc = mActor->getFrameCtrl(ANM_TYPE_BCK);
		if (fc != nullptr)
			fc->setFrame(frame);
	}
}

void TCameraBck::setRate(f32) { }
