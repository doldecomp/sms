#include <MarioUtil/MtxUtil.hpp>

#include <printf.h>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>

void TMultiMtxEffect::setup(J3DModel* model, const char* prmLocation)
{
	mModel        = model;
	mMtxEffectTbl = new TMtxEffectBase*[mNumBones << 2];

	for (u16 i = 0; i < mNumBones; ++i) {
		char* path = new char[0x40];
		snprintf(path, 0x40, "/%s/MtxEffect%d.prm", prmLocation, mBoneIDs[i]);

		switch (mMtxEffectType[i]) {
		case TMTX_EFFECT_TIME_LAG:
			TMtxTimeLag* timeLag = new TMtxTimeLag(path);
			model->getModelData()
			    ->getJointNodePointer(mBoneIDs[i])
			    ->setCallBack(TMtxTimeLagCallBack);
			model->getModelData()
			    ->getJointNodePointer(mBoneIDs[i])
			    ->setCallBackUserData(timeLag);
			mMtxEffectTbl[i] = timeLag;
			break;
		case TMTX_EFFECT_SWING_RZ:
			TMtxSwingRZ* swingRz = new TMtxSwingRZ(path);
			model->getModelData()
			    ->getJointNodePointer(mBoneIDs[i])
			    ->setCallBack(TMtxSwingRZCallBack);
			model->getModelData()
			    ->getJointNodePointer(mBoneIDs[i])
			    ->setCallBackUserData(swingRz);
			mMtxEffectTbl[i] = swingRz;
			break;
		case TMTX_EFFECT_SWING_RZ_REVERSE_XZ:
			TMtxSwingRZ* swingRzReverse = new TMtxSwingRZ(path);
			model->getModelData()
			    ->getJointNodePointer(mBoneIDs[i])
			    ->setCallBack(TMtxSwingRZReverseXZCallBack);
			model->getModelData()
			    ->getJointNodePointer(mBoneIDs[i])
			    ->setCallBackUserData(swingRzReverse);
			mMtxEffectTbl[i] = swingRzReverse;
			break;
		}
	}
	for (u16 i = 0; i < mNumBones; ++i) {
		mMtxEffectTbl[i]->mFlags |= 2;
	}
}

void TMultiMtxEffect::setUserArea()
{
	for (u16 i = 0; i < mNumBones; i++) {
		mModel->getModelData()
		    ->getJointNodePointer(mBoneIDs[i])
		    ->setCallBackUserData(mMtxEffectTbl[i]);
	}
}
