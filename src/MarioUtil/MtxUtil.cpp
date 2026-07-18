#include <MarioUtil/MtxUtil.hpp>

#include <printf.h>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <Strategic/HitActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Camera/Camera.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>

void MtxToQuat(MtxPtr m, Quaternion* quat)
{
	f32 q[4];
	f32 s = m[0][0] + m[1][1] + m[2][2] + 1.0f;
	if (s >= 1.0f) {
		f32 root = 2.0f * MsSqrtf(s);
		q[3]     = 0.25f * root;
		q[0]     = (m[2][1] - m[1][2]) * (1.0f / root);
		q[1]     = (m[0][2] - m[2][0]) * (1.0f / root);
		q[2]     = (m[1][0] - m[0][1]) * (1.0f / root);
	} else {
		int i;
		if (m[0][0] > m[1][1])
			i = 0;
		else
			i = 1;
		if (m[2][2] > m[i][i])
			i = 2;
		int j = (i + 1) % 3;
		int k = (j + 1) % 3;

		f32 root = 2.0f * MsSqrtf(1.0f + (m[i][i] - m[j][j] - m[k][k]));
		q[i]     = 0.25f * root;
		q[j]     = (m[i][j] + m[j][i]) * (1.0f / root);
		q[k]     = (m[i][k] + m[k][i]) * (1.0f / root);
		q[3]     = (m[k][j] - m[j][k]) * (1.0f / root);
	}
	quat->x = q[0];
	quat->y = q[1];
	quat->z = q[2];
	quat->w = q[3];
}

TMtxTimeLag::TDeParams::TDeParams(const char* path)
    : TParams(path)
    , PARAM_INIT(mPosAccel, 0.0f)
    , PARAM_INIT(mPosBrake, 0.0f)
    , PARAM_INIT(mPosLimit, 0.0f)
    , PARAM_INIT(mQuatAccel, 0.0f)
    , PARAM_INIT(mQuatBrake, 0.0f)
{
	TParams::load(mPrmPath);
}

void TMtxTimeLag::calc(MtxPtr mtx)
{
	if (checkFlag(2)) {
		offFlag(2);
		Vec v = { 0.0f, 0.0f, 0.0f };
		unk08 = v;

		Vec v2;
		v2.x  = mtx[0][3];
		v2.y  = mtx[1][3];
		v2.z  = mtx[2][3];
		unk14 = v2;

		Quaternion q = { 0.0f, 0.0f, 0.0f, 0.0f };
		unk20        = q;

		MtxToQuat(mtx, &q);
		unk30 = q;
	} else {
		Vec trans;

		trans.x = mtx[0][3];
		trans.y = mtx[1][3];
		trans.z = mtx[2][3];

		f32 posAccel = mParams.mPosAccel.get();
		f32 posBrake = mParams.mPosBrake.get();

		unk08.x += posAccel * (trans.x - unk14.x);
		unk08.y += posAccel * (trans.y - unk14.y);
		unk08.z += posAccel * (trans.z - unk14.z);

		unk08.x *= posBrake;
		unk08.y *= posBrake;
		unk08.z *= posBrake;

		unk14.x += unk08.x;
		unk14.y += unk08.y;
		unk14.z += unk08.z;

		f32 posLimit = mParams.mPosLimit.get();
		if (unk14.x < trans.x - posLimit)
			unk14.x = trans.x - posLimit;
		if (unk14.x > trans.x + posLimit)
			unk14.x = trans.x + posLimit;
		if (unk14.y < trans.y - posLimit)
			unk14.y = trans.y - posLimit;
		if (unk14.y > trans.y + posLimit)
			unk14.y = trans.y + posLimit;
		if (unk14.z < trans.z - posLimit)
			unk14.z = trans.z - posLimit;
		if (unk14.z > trans.z + posLimit)
			unk14.z = trans.z + posLimit;

		f32 len0 = MsSqrtf(mtx[0][0] * mtx[0][0] + mtx[1][0] * mtx[1][0]
		                   + mtx[2][0] * mtx[2][0]);
		f32 len1 = MsSqrtf(mtx[0][1] * mtx[0][1] + mtx[1][1] * mtx[1][1]
		                   + mtx[2][1] * mtx[2][1]);
		f32 len2 = MsSqrtf(mtx[0][2] * mtx[0][2] + mtx[1][2] * mtx[1][2]
		                   + mtx[2][2] * mtx[2][2]);

		Mtx rot;
		f32 inv0  = 1.0f / len0;
		f32 inv1  = 1.0f / len1;
		f32 inv2  = 1.0f / len2;
		rot[0][0] = mtx[0][0] * inv0;
		rot[1][0] = mtx[1][0] * inv0;
		rot[2][0] = mtx[2][0] * inv0;
		rot[0][1] = mtx[0][1] * inv1;
		rot[1][1] = mtx[1][1] * inv1;
		rot[2][1] = mtx[2][1] * inv1;
		rot[0][2] = mtx[0][2] * inv2;
		rot[1][2] = mtx[1][2] * inv2;
		rot[2][2] = mtx[2][2] * inv2;

		Quaternion tmp;
		MtxToQuat(rot, &tmp);

		Quaternion newQuat;
		newQuat.x = tmp.x;
		newQuat.y = tmp.y;
		newQuat.z = tmp.z;
		newQuat.w = tmp.w;

		f32 dot = unk30.x * newQuat.x + unk30.y * newQuat.y
		          + unk30.z * newQuat.z + unk30.w * newQuat.w;
		if (dot < 0.0f) {
			newQuat.x = -newQuat.x;
			newQuat.y = -newQuat.y;
			newQuat.z = -newQuat.z;
			newQuat.w = -newQuat.w;
		}

		f32 quatAccel = mParams.mQuatAccel.get();
		unk20.x += quatAccel * (newQuat.x - unk30.x);
		unk20.y += quatAccel * (newQuat.y - unk30.y);
		unk20.z += quatAccel * (newQuat.z - unk30.z);
		unk20.w += quatAccel * (newQuat.w - unk30.w);

		f32 quatBrake = mParams.mQuatBrake.get();
		unk20.x *= quatBrake;
		unk20.y *= quatBrake;
		unk20.z *= quatBrake;
		unk20.w *= quatBrake;

		unk30.x += unk20.x;
		unk30.y += unk20.y;
		unk30.z += unk20.z;
		unk30.w += unk20.w;

		if (mFlags & 1)
			MTXQuat(mtx, &unk30);
		if (mFlags & 1) {
			mtx[0][0] *= len0;
			mtx[1][0] *= len0;
			mtx[2][0] *= len0;
			mtx[0][1] *= len1;
			mtx[1][1] *= len1;
			mtx[2][1] *= len1;
			mtx[0][2] *= len2;
			mtx[1][2] *= len2;
			mtx[2][2] *= len2;
			mtx[0][3] = unk14.x;
			mtx[1][3] = unk14.y;
			mtx[2][3] = unk14.z;
		}
	}
}

int TMtxTimeLagCallBack(J3DNode* node, int param)
{
	if (param == 0)
		((TMtxTimeLag*)node->mCallBackUserData)->calc(J3DSys::mCurrentMtx);
	return 1;
}

void TMtxSwingRZ::calcLocalXY(MtxPtr mtx, Vec* vecX, Vec* vecY)
{
	if (checkFlag(2)) {
		offFlag(2);

		Vec v = { 0.0f, 0.0f, 0.0f };
		unk14 = v;

		Vec vec;
		vec.x = mtx[0][3];
		vec.y = mtx[1][3] - mParams.mL.get();
		vec.z = mtx[2][3];
		unk08 = vec;

		vecX->x = mtx[0][0];
		vecX->y = mtx[1][0];
		vecX->z = mtx[2][0];

		vecY->x = mtx[0][1];
		vecY->y = mtx[1][1];
		vecY->z = mtx[2][1];
	} else {
		VECAdd(&unk14, (Vec*)&mParams.mAcc.get(), &unk14);
		VECScale(&unk14, &unk14, mParams.mBrake.get());
		VECAdd(&unk14, &unk08, &unk08);

		Vec trans;
		trans.x = mtx[0][3];
		trans.y = mtx[1][3];
		trans.z = mtx[2][3];

		Vec diff;
		diff.x = unk08.x - trans.x;
		diff.y = unk08.y - trans.y;
		diff.z = unk08.z - trans.z;
		VECNormalize(&diff, &diff);
		VECScale(&diff, &diff, mParams.mL.get());

		Vec point;
		point.x = trans.x + diff.x;
		point.y = trans.y + diff.y;
		point.z = trans.z + diff.z;

		Vec corr;
		VECSubtract(&point, &unk08, &corr);
		VECAdd(&unk14, &corr, &unk14);
		unk08 = point;

		Vec zAxis;
		zAxis.x = mtx[0][2];
		zAxis.y = mtx[1][2];
		zAxis.z = mtx[2][2];

		vecX->x = unk08.x - trans.x;
		vecX->y = unk08.y - trans.y;
		vecX->z = unk08.z - trans.z;
		VECNormalize(vecX, vecX);
		VECCrossProduct(&zAxis, vecX, vecY);
		VECNormalize(vecX, vecX);
		VECNormalize(vecY, vecY);
		VECCrossProduct(vecY, &zAxis, vecX);
	}
}

void TMtxSwingRZ::calc(MtxPtr mtx)
{
	Vec vecX;
	Vec vecY;
	calcLocalXY(mtx, &vecX, &vecY);
	if (mFlags & 1) {
		mtx[0][0] = vecX.x;
		mtx[1][0] = vecX.y;
		mtx[2][0] = vecX.z;
		mtx[0][1] = vecY.x;
		mtx[1][1] = vecY.y;
		mtx[2][1] = vecY.z;
	}
}

int TMtxSwingRZCallBack(J3DNode* node, int param)
{
	if (param == 0)
		((TMtxSwingRZ*)node->mCallBackUserData)->calc(J3DSys::mCurrentMtx);
	return 1;
}

void TMtxSwingRZReverseXZ::calc(MtxPtr mtx)
{
	Vec vecX;
	Vec vecY;
	calcLocalXY(mtx, &vecX, &vecY);
	if (mFlags & 1) {
		mtx[0][0] = -vecX.x;
		mtx[1][0] = -vecX.y;
		mtx[2][0] = -vecX.z;
		mtx[0][1] = -vecY.x;
		mtx[1][1] = -vecY.y;
		mtx[2][1] = -vecY.z;
	}
}

int TMtxSwingRZReverseXZCallBack(J3DNode* node, int param)
{
	if (param == 0)
		((TMtxSwingRZReverseXZ*)node->mCallBackUserData)
		    ->calc(J3DSys::mCurrentMtx);
	return 1;
}

void TMultiMtxEffect::setup(J3DModel* model, const char* prmLocation)
{
	mModel        = model;
	mMtxEffectTbl = new TMtxEffectBase*[mNumBones];

	for (u16 i = 0; i < mNumBones; ++i) {
		char* path = new char[0x40];
		snprintf(path, 0x40, "/%s/MtxEffect%d.prm", prmLocation, mBoneIDs[i]);

		switch (mMtxEffectType[i]) {
		case TMTX_EFFECT_TIME_LAG: {
			TMtxTimeLag* timeLag = new TMtxTimeLag(path);
			model->getModelData()
			    ->getJointNodePointer(mBoneIDs[i])
			    ->setCallBack(TMtxTimeLagCallBack);
			model->getModelData()
			    ->getJointNodePointer(mBoneIDs[i])
			    ->setCallBackUserData(timeLag);
			mMtxEffectTbl[i] = timeLag;
			break;
		}
		case TMTX_EFFECT_SWING_RZ: {
			TMtxSwingRZ* swingRz = new TMtxSwingRZ(path);
			model->getModelData()
			    ->getJointNodePointer(mBoneIDs[i])
			    ->setCallBack(TMtxSwingRZCallBack);
			model->getModelData()
			    ->getJointNodePointer(mBoneIDs[i])
			    ->setCallBackUserData(swingRz);
			mMtxEffectTbl[i] = swingRz;
			break;
		}
		case TMTX_EFFECT_SWING_RZ_REVERSE_XZ: {
			TMtxSwingRZReverseXZ* swingRzReverse
			    = new TMtxSwingRZReverseXZ(path);
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
	}
	for (int i = 0; i < mNumBones; ++i)
		mMtxEffectTbl[i]->onFlag(2);
}

void TMultiMtxEffect::setUserArea()
{
	for (u16 i = 0; i < mNumBones; i++) {
		mModel->getModelData()
		    ->getJointNodePointer(mBoneIDs[i])
		    ->setCallBackUserData(mMtxEffectTbl[i]);
	}
}

void TMultiMtxEffect::add() { }

void TMultiMtxEffect::remove() { }

void SMS_MakeJointsToArc(J3DModel* model, const JGeometry::TVec3<f32>& start,
                         const JGeometry::TVec3<f32>& upDir,
                         const JGeometry::TVec3<f32>& end)
{
	model->calc();

	JGeometry::TVec3<f32> dir = end - start;
	f32 mag                   = VECMag(dir);
	dir.scale(1.0f / mag);

	JGeometry::TVec3<f32> up = upDir;
	up.normalize();

	int jointNum = model->getModelData()->getJointNum();
	for (u16 i = 0; i < jointNum; ++i) {
		f32 t = (f32)i / (f32)(jointNum - 1);

		JGeometry::TVec3<f32> a = dir * t;
		JGeometry::TVec3<f32> b = up * (1.0f - t);
		JGeometry::TVec3<f32> c = b + a;
		c.normalize();

		MtxPtr jm = model->getAnmMtx(i);

		f32 dist = (f32)i * (mag / (f32)(jointNum - 1));

		JGeometry::TVec3<f32> zAxis(jm[0][2], jm[1][2], jm[2][2]);
		JGeometry::TVec3<f32> side;
		side.cross(zAxis, c);
		JGeometry::TVec3<f32> fwd;
		fwd.cross(c, side);
		side.normalize();
		fwd.normalize();

		jm[0][0] = c.x;
		jm[1][0] = c.y;
		jm[2][0] = c.z;
		jm[0][1] = side.x;
		jm[1][1] = side.y;
		jm[2][1] = side.z;
		jm[0][2] = fwd.x;
		jm[1][2] = fwd.y;
		jm[2][2] = fwd.z;
		jm[0][3] = start.x + c.x * dist;
		jm[1][3] = start.y + c.y * dist;
		jm[2][3] = start.z + c.z * dist;
	}
}

void SMS_MtxLookAt(MtxPtr, const JGeometry::TVec3<f32>&,
                   const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&)
{
}

void SMS_GetLightPerspectiveForEffectMtx(MtxPtr mtx)
{
	C_MTXPerspective(mtx, gpCamera->getFovy(), gpCamera->getAspect(),
	                 gpCamera->getNear(), gpCamera->getFar());
	mtx[2][0] = 0.0f;
	mtx[2][1] = 0.0f;
	mtx[2][2] = -1.0f;
	mtx[2][3] = 0.0f;
	mtx[3][0] = 0.0f;
	mtx[3][1] = 0.0f;
	mtx[3][2] = 0.0f;
	mtx[3][3] = 1.0f;
}

TRopePoint::TRopePoint() { }

TRope::TRope(u16 count, const JGeometry::TVec3<f32>& pos, f32 p1, f32 p2,
             f32 p3, f32 p4)
{
	mNumPoints = count;
	mPoints    = new TRopePoint[count];
	unk8       = p3;
	unkC       = p4;
	unk10      = p2;
	for (int i = 0; i < count; ++i) {
		mPoints[i].unkC = pos;
		mPoints[i].unk0 = mPoints[i].unkC;
		mPoints[i].unk18.set(0.0f, 0.0f, 0.0f);
		mPoints[i].unk24 = p1;
		mPoints[i].unk28 = 0;
	}
}

void TRope::collision()
{
	for (int i = 0; i < mNumPoints; ++i) {
		if (mPoints[i].unk28 & 1)
			continue;

		JGeometry::TVec3<f32>& pos = mPoints[i].unkC;

		const TBGCheckData* data;
		f32 roofY = gpMap->checkRoof(pos.x, pos.y + unk10, pos.z, &data);
		if (data != nullptr) {
			if (!data->isIllegalData()) {
				if (pos.y + unk10 > roofY)
					pos.y = roofY - 2.0f * unk10;
			}
		}

		f32 groundY = gpMap->checkGround(pos.x, pos.y + unk10, pos.z, &data);
		groundY += 3.0f;
		if (!data->isIllegalData()) {
			if (pos.y < groundY)
				pos.y = groundY;
		}

		f32 r = unk10;
		gpMap->isTouchedOneWallAndMoveXZ(&pos.x, pos.y, &pos.z, r);
	}
}

void TRope::constraintHead(const JGeometry::TVec3<f32>& param)
{
	mPoints[0].unkC = param;
	for (int i = 0; i < mNumPoints - 1; ++i) {
		TRopePoint& next            = mPoints[i + 1];
		TRopePoint& cur             = mPoints[i];
		JGeometry::TVec3<f32> delta = next.unkC;
		delta -= cur.unkC;
		if (delta.isZero())
			delta.set(0.0f, 1.0f, 0.0f);
		VECNormalize(&delta, &delta);
		delta.scale(cur.unk24);
		next.unkC = cur.unkC;
		next.unkC.add(delta);
	}
	collision();
}

void TRope::constraintTail(const JGeometry::TVec3<f32>& param)
{
	mPoints[mNumPoints - 1].unkC = param;
	for (int i = mNumPoints - 1; i > 0; --i) {
		TRopePoint& cur  = mPoints[i];
		TRopePoint& prev = mPoints[i - 1];

		if (!cur.unkC.epsilonEquals(prev.unkC)) {
			JGeometry::TVec3<f32> delta = prev.unkC;
			delta -= cur.unkC;
			VECNormalize(&delta, &delta);
			delta.scale(cur.unk24);
			prev.unkC = cur.unkC;
			prev.unkC += delta;
		}
	}
	collision();
}

void TRope::moveHead(const JGeometry::TVec3<f32>& param)
{
	for (int i = 0; i < mNumPoints; ++i) {
		mPoints[i].unkC.y += unkC;
		mPoints[i].unkC += mPoints[i].unk18;
	}
	constraintHead(param);
	for (int i = 0; i < mNumPoints; ++i) {
		f32 scale               = unk8;
		JGeometry::TVec3<f32> v = mPoints[i].unkC - mPoints[i].unk0;
		mPoints[i].unk18        = v * scale;
		mPoints[i].unk0         = mPoints[i].unkC;
	}
}

void TRope::moveHeadAndTail(const JGeometry::TVec3<f32>&,
                            const JGeometry::TVec3<f32>&)
{
}

void SMS_GetActorMtx(const THitActor& actor, MtxPtr mtx)
{
	MsMtxSetTRS(mtx, actor.mPosition.x, actor.mPosition.y, actor.mPosition.z,
	            actor.mRotation.x, actor.mRotation.y, actor.mRotation.z,
	            actor.mScaling.x, actor.mScaling.y, actor.mScaling.z);
}
