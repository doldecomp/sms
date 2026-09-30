#include "Enemy/FeetInv.hpp"
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JMath.hpp>
#include "Map/Map.hpp"
#include <Map/MapData.hpp>
#include <MarioUtil/MathUtil.hpp>

// fabricated
inline f32 getAngleBetween(const JGeometry::TVec3<f32>& a,
                           const JGeometry::TVec3<f32>& b)
{
	JGeometry::TVec3<f32> crossVec;
	crossVec.cross(a, b);

	s16 rawAngle = matan(a.dot(b), MsVECMag2(&crossVec));
	return SHORTANGLE2DEG(rawAngle);
}

// fabricated
inline f32 MsAsin(f32 x)
{
	if (x == 1.0f) {
		return 90.0f;
	} else if (x == -1.0f) {
		return -90.0f;
	} else {
		f32 cosSq  = -((x * x) - 1.0f);
		f32 cosVal = cosSq * __frsqrte(cosSq);
		return SHORTANGLE2DEG(matan(cosVal, x));
	}
}

// fabricated
inline f32 MsAcos(f32 x)
{
	if (x == 1.0f) {
		return 0.0f;
	} else if (x == -1.0f) {
		return 180.0f;
	} else {
		f32 cosSq  = -((x * x) - 1.0f);
		f32 cosVal = cosSq * __frsqrte(cosSq);
		return 90.0f - SHORTANGLE2DEG(matan(cosVal, x));
	}
}

void FeetInvCalc(J3DModel* model, u16 hipIdx, u16 kneeIdx, u16 footIdx,
                 f32 heightOffset)
{
	MtxPtr kneeMtx = model->getAnmMtx(kneeIdx);
	MtxPtr footMtx = model->getAnmMtx(footIdx);

	JGeometry::TVec3<f32> kneePos;
	kneePos.x = kneeMtx[0][3];
	kneePos.y = kneeMtx[1][3];
	kneePos.z = kneeMtx[2][3];

	JGeometry::TVec3<f32> footPos;
	footPos.x = footMtx[0][3];
	footPos.y = footMtx[1][3];
	footPos.z = footMtx[2][3];

	JGeometry::TVec3<f32> shin(footPos);
	shin -= kneePos;
	f32 shinLen = shin.length();

	const TBGCheckData* groundData;
	f32 groundY = gpMap->checkGround(footPos.x, footPos.y + shinLen, footPos.z,
	                                 &groundData)
	              + heightOffset;

	if (!(groundY < footPos.y)) {
		footPos.y = groundY;

		MtxPtr hipMtx = model->getAnmMtx(hipIdx);
		JGeometry::TVec3<f32> hipPos;
		hipPos.x = hipMtx[0][3];
		hipPos.y = hipMtx[1][3];
		hipPos.z = hipMtx[2][3];

		JGeometry::TVec3<f32> thigh(kneePos);
		thigh -= hipPos;
		f32 thighLen = thigh.length();

		JGeometry::TVec3<f32> toFoot(footPos);
		toFoot -= hipPos;
		f32 toFootLen = toFoot.length();

		f32 betweenAngleDeg = getAngleBetween(toFoot, thigh);

		f32 cosHipAngle = -((toFootLen * toFootLen)
		                    - ((thighLen * thighLen) + (shinLen * shinLen)))
		                  / (2.0f * thighLen * shinLen);
		f32 hipAngle = MsAcos(cosHipAngle);

		f32 sinRatio       = (shinLen * MsSin(hipAngle)) / toFootLen;
		f32 kneeSolveAngle = MsAsin(sinRatio);

		f32 rotAngle = kneeSolveAngle - betweenAngleDeg;
		TRotation3f rotMtx;
		MsMtxSetRotZ(rotMtx, rotAngle);
		PSMTXConcat(hipMtx, rotMtx, hipMtx);

		JGeometry::TVec3<f32> dir;
		dir.x = hipMtx[0][0];
		dir.y = hipMtx[1][0];
		dir.z = hipMtx[2][0];
		dir.setLength(thighLen);

		JGeometry::TVec3<f32> newKnee = hipPos + dir;
		kneePos                       = newKnee;

		kneeMtx[0][3] = kneePos.x;
		kneeMtx[1][3] = kneePos.y;
		kneeMtx[2][3] = kneePos.z;

		JGeometry::TVec3<f32> kneeFwd;
		kneeFwd.x = kneeMtx[0][2];
		kneeFwd.y = kneeMtx[1][2];
		kneeFwd.z = kneeMtx[2][2];

		JGeometry::TVec3<f32> kneeSide;
		kneeSide.x = kneeMtx[0][0];
		kneeSide.y = kneeMtx[1][0];
		kneeSide.z = kneeMtx[2][0];

		f32 kneeFwdLen  = kneeFwd.length();
		f32 kneeSideLen = kneeSide.length();

		JGeometry::TVec3<f32> newDir(footPos);
		newDir -= kneePos;
		newDir.setLength(kneeSideLen);

		kneeMtx[0][0] = newDir.x;
		kneeMtx[1][0] = newDir.y;
		kneeMtx[2][0] = newDir.z;

		JGeometry::TVec3<f32> oldUp;
		oldUp.x = kneeMtx[0][1];
		oldUp.y = kneeMtx[1][1];
		oldUp.z = kneeMtx[2][1];

		JGeometry::TVec3<f32> upRaw;
		upRaw.cross(newDir, oldUp);
		upRaw.setLength(kneeFwdLen);

		kneeMtx[0][2] = upRaw.x;
		kneeMtx[1][2] = upRaw.y;
		kneeMtx[2][2] = upRaw.z;

		footMtx[0][3] = footPos.x;
		footMtx[1][3] = footPos.y;
		footMtx[2][3] = footPos.z;

		JGeometry::TVec3<f32> footFwd;
		footFwd.x = footMtx[0][1];
		footFwd.y = footMtx[1][1];
		footFwd.z = footMtx[2][1];

		JGeometry::TVec3<f32> footSide;
		footSide.x = footMtx[0][0];
		footSide.y = footMtx[1][0];
		footSide.z = footMtx[2][0];

		f32 footFwdLen  = footFwd.length();
		f32 footSideLen = footSide.length();

		const JGeometry::TVec3<f32>& normal = groundData->getNormal();
		JGeometry::TVec3<f32> negNormal;
		negNormal.x = -normal.x * footFwdLen;
		negNormal.y = -normal.y * footFwdLen;
		negNormal.z = -normal.z * footFwdLen;

		footMtx[0][1] = negNormal.x;
		footMtx[1][1] = negNormal.y;
		footMtx[2][1] = negNormal.z;

		JGeometry::TVec3<f32> footZDir;
		footZDir.x = footMtx[0][2];
		footZDir.y = footMtx[1][2];
		footZDir.z = footMtx[2][2];

		JGeometry::TVec3<f32> footSideRaw;
		footSideRaw.cross(negNormal, footZDir);
		footSideRaw.setLength(footSideLen);

		footMtx[0][0] = footSideRaw.x;
		footMtx[1][0] = footSideRaw.y;
		footMtx[2][0] = footSideRaw.z;
	}
}

TMtxCalcFootInv::TMtxCalcFootInv(u16 unk1, u16 unk2, u16 unk3, u16 unk4,
                                 u16 unk5, u16 unk6, f32 unk7)
    : J3DMtxCalcSoftimageAnm(nullptr)
{
	this->unk68 = unk1;
	this->unk6A = unk2;
	this->unk6C = unk3;
	this->unk6E = unk4;
	this->unk70 = unk5;
	this->unk72 = unk6;
	this->unk74 = unk7;
}

void TMtxCalcFootInv::calc(u16 jntIdx)
{
	J3DMtxCalcAnm::calc(jntIdx);

	if (this->unk6C == jntIdx) {
		FeetInvCalc(j3dSys.mModel, this->unk68, this->unk6A, this->unk6C,
		            this->unk74);
	}
	if (this->unk72 == jntIdx) {
		FeetInvCalc(j3dSys.mModel, this->unk6E, this->unk70, this->unk72,
		            this->unk74);
	}
}
