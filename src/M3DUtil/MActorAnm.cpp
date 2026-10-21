#include <M3DUtil/MActorAnm.hpp>
#include <M3DUtil/MActorData.hpp>
#include <M3DUtil/MotionBlendCtrl.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DCluster.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>

void MActorAnmBase::checkUseMaterialIDInit(u16*) { }

void MActorAnmBase::checkUseMaterialID(u16*) { }

void MActorAnmBck::changeMtxCalcType(u8 type)
{
	mMtxCalcType = type;
	switch (type) {
	case MACTOR_MTX_CALC_SOFTIMAGE:
		if (mMtxCalcSoftimage == nullptr)
			mMtxCalcSoftimage = new J3DMtxCalcSoftimageAnm(nullptr);
		break;
	case MACTOR_MTX_CALC_BASIC:
		if (mMtxCalcBasic == nullptr)
			mMtxCalcBasic = new J3DMtxCalcBasicAnm(nullptr);
		break;
	}
}

void MActorAnmBck::initSimpleMotionBlend(int param_1)
{
	if (!mMotionBlendCtrl) {
		bool thing = false;
		if (mMtxCalcType == MACTOR_MTX_CALC_BASIC)
			thing = true;
		mMotionBlendCtrl = new TMotionBlendCtrl(thing, param_1);
	}
	mMtxCalcType = MACTOR_MTX_CALC_MOTION_BLEND;
}

void MActorAnmBck::initNormalMotionBlend()
{
	if (!mMotionBlendCtrl) {
		bool thing = false;
		if (mMtxCalcType == MACTOR_MTX_CALC_BASIC)
			thing = true;
		mMotionBlendCtrl = new TMotionBlendCtrl(thing);
	}
	mMtxCalcType = MACTOR_MTX_CALC_MOTION_BLEND;
}

f32 MActorAnmBck::getMotionBlendRatio() const
{
	if (!mMotionBlendCtrl)
		return 0.0f;
	else
		return mMotionBlendCtrl->getMotionBlendRatio();
}

void MActorAnmBck::setMotionBlendRatio(f32 ratio)
{
	if (mMotionBlendCtrl)
		mMotionBlendCtrl->setMotionBlendRatio(ratio);
}

J3DAnmTransform* MActorAnmBck::getOldMotionBlendAnmPtr() const
{
	if (!mMotionBlendCtrl)
		return nullptr;
	else
		return mMotionBlendCtrl->getOldMotionBlendAnmPtr();
}

void MActorAnmBck::setOldMotionBlendAnmPtr(J3DAnmTransform* anm_ptr)
{
	if (mMotionBlendCtrl)
		mMotionBlendCtrl->setOldMotionBlendAnmPtr(anm_ptr);
}

f32 MActorAnmBck::getOldMotionBlendFrame() const
{
	if (mMotionBlendCtrl)
		return mMotionBlendCtrl->getOldMotionBlendFrame();
	else
		return 0.0f;
}

void MActorAnmBck::setModel(J3DModel* model)
{
	u8 type = MACTOR_MTX_CALC_BASIC;
	mModel  = model;
	if (model->getModelData()->getFlag() & J3DMLF_MtxCalcSoftImage)
		type = MACTOR_MTX_CALC_SOFTIMAGE;
	changeMtxCalcType(type);
}

void MActorAnmBck::updateIn()
{
	J3DJoint* joint = mModel->getModelData()->getJointNodePointer(mJointIndex);
	mCurAnmPtr->setFrame(mFrameCtrl.getFrame());
	switch (getMtxCalcType()) {
	case MACTOR_MTX_CALC_BASIC:
		mMtxCalcBasic->setAnmTransform(mCurAnmPtr);
		joint->setMtxCalc(mMtxCalcBasic);
		break;
	case MACTOR_MTX_CALC_SOFTIMAGE:
		mMtxCalcSoftimage->setAnmTransform(mCurAnmPtr);
		joint->setMtxCalc(mMtxCalcSoftimage);
		break;
	case MACTOR_MTX_CALC_MOTION_BLEND:
		joint->setMtxCalc(mMotionBlendCtrl->getMtxCalc());
		mMotionBlendCtrl->execSimpleMotionBlend();
		break;
	case MACTOR_MTX_CALC_USER:
		joint->setMtxCalc(mUserMtxCalc);
		break;
	}
}

void MActorAnmBck::updateOut()
{
	mModel->getModelData()
	    ->getJointNodePointer(mJointIndex)
	    ->setMtxCalc(nullptr);
}

void MActorAnmBck::setAnmFromIndex(int param_1, u16*)
{
	if (getMtxCalcType() == MACTOR_MTX_CALC_MOTION_BLEND && mCurAnmIdx != -1)
		mMotionBlendCtrl->keepCurAnm(getData()->getAnmPtr(mCurAnmIdx),
		                             mFrameCtrl.getFrame());

	setFrameCtrl(param_1);

	if (getMtxCalcType() == MACTOR_MTX_CALC_MOTION_BLEND)
		mMotionBlendCtrl->setNewAnm(getData()->getAnmPtr(mCurAnmIdx));
}

void MActorAnmBtp::setTexNoAnmFullPtr()
{
	int count  = mAnmData->getAnmNum();
	mTexNoAnms = new J3DTexNoAnm*[count];
	for (int i = 0; i < count; ++i) {
		J3DAnmTexPattern* anm = getData()->getAnmPtr(i);

		int materials = anm->getUpdateMaterialNum();
		mTexNoAnms[i] = new J3DTexNoAnm[materials];

		for (int j = 0; j < materials; ++j) {
			mTexNoAnms[i][j].setAnmIndex(j);
			mTexNoAnms[i][j].setAnmTexPattern(getData()->getAnmPtr(i));
		}
	}
}

void MActorAnmBtp::checkUseMaterialIDInit(u16* param_1)
{
	for (int i = 0; i < getData()->getAnmNum(); ++i) {
		J3DAnmTexPattern* anm = getData()->getAnmPtr(i);
		for (u16 j = 0; j < anm->getUpdateMaterialNum(); ++j) {
			u16 id = anm->getUpdateMaterialID(j);
			for (u16 k = 0; k < mModel->getModelData()->getMaterialNum(); ++k) {
				if (strcmp(
				        anm->getUpdateMaterialName()->getName(j),
				        mModel->getModelData()->getMaterialName()->getName(k))
				    == 0) {
					param_1[k] = id;
					break;
				}
			}
		}
	}
}

void MActorAnmBtp::checkUseMaterialID(u16* param_1)
{
	if (param_1 == nullptr || mCurAnmPtr == nullptr)
		return;

	for (u16 i = 0; i < mCurAnmPtr->getUpdateMaterialNum(); ++i) {
		u32 id = mCurAnmPtr->getUpdateMaterialID(i);
		if (id != 0xffff)
			param_1[id] = id;
	}
}

void MActorAnmBtp::updateIn()
{
	mCurAnmPtr->setFrame(mFrameCtrl.getFrame());
	mModel->getModelData()->setTexNoAnimator(mCurAnmPtr,
	                                         mTexNoAnms[mCurAnmIdx]);
}

void MActorAnmBtp::updateOut()
{
	mModel->getModelData()->removeTexNoAnimator(mCurAnmPtr);
}

void MActorAnmBtk::checkUseMaterialIDInit(u16* param_1)
{
	for (int i = 0; i < getData()->getAnmNum(); ++i) {
		J3DAnmTextureSRTKey* anm = getData()->getAnmPtr(i);
		for (u16 j = 0; j < anm->getUpdateMaterialNum(); ++j) {
			for (u16 k = 0; k < mModel->getModelData()->getMaterialNum(); ++k) {
				if (strcmp(
				        anm->getUpdateMaterialName()->getName(j),
				        mModel->getModelData()->getMaterialName()->getName(k))
				    == 0) {
					param_1[k] = j;
					break;
				}
			}
		}
	}
}

void MActorAnmBtk::checkUseMaterialID(u16* param_1)
{
	if (param_1 == nullptr || mCurAnmPtr == nullptr)
		return;

	for (u16 i = 0; i < mCurAnmPtr->getUpdateMaterialNum(); ++i) {
		u32 id = mCurAnmPtr->getUpdateMaterialID(i);
		if (id != 0xffff)
			param_1[id] = id;
	}
}

void MActorAnmBtk::setTexMtxAnmKeyPtr()
{
	int count   = mAnmData->getAnmNum();
	mTexMtxAnms = new J3DTexMtxAnm*[count];
	for (int i = 0; i < count; ++i) {
		J3DAnmTextureSRTKey* anm = getData()->getAnmPtr(i);

		int materials  = anm->getUpdateMaterialNum();
		mTexMtxAnms[i] = new J3DTexMtxAnm[materials];

		for (int j = 0; j < materials; ++j) {
			mTexMtxAnms[i][j].setAnmIndex(j);
			mTexMtxAnms[i][j].setAnmTransform(getData()->getAnmPtr(i));
		}
	}
}

void MActorAnmBtk::updateIn()
{
	mCurAnmPtr->setFrame(mFrameCtrl.getFrame());
	J3DTexMtxAnm* anm = mTexMtxAnms[mCurAnmIdx];
	mModel->getModelData()->setTexMtxAnimator(mCurAnmPtr, anm, anm);
}

void MActorAnmBtk::updateOut()
{
	mModel->getModelData()->removeTexMtxAnimator(mCurAnmPtr);
}

void MActorAnmBpk::checkUseMaterialIDInit(u16* param_1)
{
	for (int i = 0; i < getData()->getAnmNum(); ++i) {
		J3DAnmColorKey* anm = getData()->getAnmPtr(i);
		for (u16 j = 0; j < anm->getUpdateMaterialNum(); ++j) {
			for (u16 k = 0; k < mModel->getModelData()->getMaterialNum(); ++k) {
				if (strcmp(
				        anm->getUpdateMaterialName()->getName(j),
				        mModel->getModelData()->getMaterialName()->getName(k))
				    == 0) {
					param_1[k] = j;
					break;
				}
			}
		}
	}
}

void MActorAnmBpk::checkUseMaterialID(u16* param_1)
{
	if (param_1 == nullptr || mCurAnmPtr == nullptr)
		return;

	for (u16 i = 0; i < mCurAnmPtr->getUpdateMaterialNum(); ++i) {
		u32 id = mCurAnmPtr->getUpdateMaterialID(i);
		if (id != 0xffff)
			param_1[id] = id;
	}
}

void MActorAnmBpk::setMatColorAnmKeyPtr()
{
	int count     = mAnmData->getAnmNum();
	mMatColorAnms = new J3DMatColorAnm*[count];
	for (int i = 0; i < count; ++i) {
		J3DAnmColorKey* anm = getData()->getAnmPtr(i);

		int materials    = anm->getUpdateMaterialNum();
		mMatColorAnms[i] = new J3DMatColorAnm[materials];

		for (int j = 0; j < materials; ++j) {
			mMatColorAnms[i][j].setAnmIndex(j);
			mMatColorAnms[i][j].setAnmColor(getData()->getAnmPtr(i));
		}
	}
}

void MActorAnmBpk::updateIn()
{
	mCurAnmPtr->setFrame(mFrameCtrl.getFrame());
	mModel->getModelData()->setMatColorAnimator(mCurAnmPtr,
	                                            mMatColorAnms[mCurAnmIdx]);
}

void MActorAnmBpk::updateOut()
{
	mModel->getModelData()->removeMatColorAnimator(mCurAnmPtr);
}

void MActorAnmBrk::setTevColorAnmKeyPtr()
{
	int count     = mAnmData->getAnmNum();
	mTevColorAnms = new J3DTevColorAnm*[count];
	for (int i = 0; i < count; ++i) {
		J3DAnmTevRegKey* anm = getData()->getAnmPtr(i);

		int materials    = anm->getCRegUpdateMaterialNum();
		mTevColorAnms[i] = new J3DTevColorAnm[materials];

		for (int j = 0; j < materials; ++j) {
			mTevColorAnms[i][j].setAnmIndex(j);
			mTevColorAnms[i][j].setAnmTevReg(getData()->getAnmPtr(i));
		}
	}
}

void MActorAnmBrk::setTevKColorAnmKeyPtr()
{
	int count      = mAnmData->getAnmNum();
	mTevKColorAnms = new J3DTevKColorAnm*[count];
	for (int i = 0; i < count; ++i) {
		J3DAnmTevRegKey* anm = getData()->getAnmPtr(i);

		int materials     = anm->getKRegUpdateMaterialNum();
		mTevKColorAnms[i] = new J3DTevKColorAnm[materials];

		for (int j = 0; j < materials; ++j) {
			mTevKColorAnms[i][j].setAnmIndex(j);
			mTevKColorAnms[i][j].setAnmTevReg(getData()->getAnmPtr(i));
		}
	}
}

void MActorAnmBrk::updateIn()
{
	mCurAnmPtr->setFrame(mFrameCtrl.getFrame());
	mModel->getModelData()->setTevRegAnimator(
	    mCurAnmPtr, mTevColorAnms[mCurAnmIdx], mTevKColorAnms[mCurAnmIdx]);
}

void MActorAnmBrk::updateOut()
{
	mModel->getModelData()->removeTevRegAnimator(mCurAnmPtr);
}

void MActorAnmBrk::checkUseMaterialIDInit(u16* param_1)
{
	for (u16 i = 0; i < mModel->getModelData()->getMaterialNum(); ++i)
		if (param_1[i] == 0x32)
			param_1[i] = i;
}

void MActorAnmBrk::checkUseMaterialID(u16* param_1)
{
	for (u16 i = 0; i < mModel->getModelData()->getMaterialNum(); ++i)
		if (param_1[i] == 0x32)
			param_1[i] = i;
}

void MActorAnmBlk::updateIn()
{
	if (mFrameCtrl.getFrame() + 1.0f >= mCurAnmPtr->getFrameMax())
		mFrameCtrl.setFrame(0.0f);
	mCurAnmPtr->setFrame(mFrameCtrl.getFrame());
	mDeformData->setAnm(mCurAnmPtr);
}

void MActorAnmBlk::updateOut() { mDeformData->setAnm(nullptr); }

void MActorAnmBlk::setAnmFromIndex(int param_1, u16* param_2)
{
	setFrameCtrl(param_1);
}
