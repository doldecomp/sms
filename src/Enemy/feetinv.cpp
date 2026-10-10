#include <Enemy/FeetInv.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JMath.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/MathUtil.hpp>

// fabricated
inline f32 getAngleBetween(const JGeometry::TVec3<f32>& a,
                           const JGeometry::TVec3<f32>& b)
{
	f32 dot = a.dot(b);
	JGeometry::TVec3<f32> crossVec;
	crossVec.cross(a, b);

	return MsAtan2(dot, MsVECMag2(&crossVec));
}

// fabricated
inline f32 MsAsin(f32 x)
{
	if (x == 1.0f) {
		return 90.0f;
	} else if (x == -1.0f) {
		return -90.0f;
	} else {
		f32 cosSq = -((x * x) - 1.0f);
		// TODO: the target uses an unrefined square-root estimate.
		f32 cosVal = MsSqrtf(cosSq);
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
		f32 cosSq = -((x * x) - 1.0f);
		// TODO: the target uses an unrefined square-root estimate.
		f32 cosVal = MsSqrtf(cosSq);
		f32 angle  = SHORTANGLE2DEG(matan(cosVal, x));
		return 90.0f - angle;
	}
}

// TODO: nonmatching frame layout and vector-value reloads remain.
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
	f32 footY   = footPos.y;
	f32 groundY = gpMap->checkGround(footPos.x, footY + shinLen, footPos.z,
	                                 &groundData);
	groundY += heightOffset;

	if (!(groundY < footY)) {
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
		MsMtxSetRotZ(rotMtx, -rotAngle);
		PSMTXConcat(hipMtx, rotMtx, hipMtx);

		JGeometry::TVec3<f32> dir(hipMtx[0][0], hipMtx[1][0], hipMtx[2][0]);
		dir.normalize();
		dir.scale(thighLen);

		kneePos = hipPos + dir;

		kneeMtx[0][3] = kneePos.x;
		kneeMtx[1][3] = kneePos.y;
		kneeMtx[2][3] = kneePos.z;

		JGeometry::TVec3<f32> newDir(footPos);
		f32 kneeFwdLen  = MsSqrtf(kneeMtx[0][2] * kneeMtx[0][2]
		                          + kneeMtx[1][2] * kneeMtx[1][2]
		                          + kneeMtx[2][2] * kneeMtx[2][2]);
		f32 kneeSideLen = MsSqrtf(kneeMtx[0][0] * kneeMtx[0][0]
		                          + kneeMtx[1][0] * kneeMtx[1][0]
		                          + kneeMtx[2][0] * kneeMtx[2][0]);
		newDir -= kneePos;
		newDir.normalize();
		newDir.scale(kneeSideLen);

		kneeMtx[0][0] = newDir.x;
		kneeMtx[1][0] = newDir.y;
		kneeMtx[2][0] = newDir.z;

		JGeometry::TVec3<f32> oldUp;
		oldUp.x = kneeMtx[0][1];
		oldUp.y = kneeMtx[1][1];
		oldUp.z = kneeMtx[2][1];

		JGeometry::TVec3<f32> upRaw;
		upRaw.cross2(newDir, oldUp);
		upRaw.normalize();
		upRaw.scale(kneeFwdLen);

		kneeMtx[0][2] = upRaw.x;
		kneeMtx[1][2] = upRaw.y;
		kneeMtx[2][2] = upRaw.z;

		footMtx[0][3] = footPos.x;
		footMtx[1][3] = footPos.y;
		footMtx[2][3] = footPos.z;

		JGeometry::TVec3<f32> negNormal(groundData->getNormal());
		f32 footFwdLen  = MsSqrtf(footMtx[0][1] * footMtx[0][1]
		                          + footMtx[1][1] * footMtx[1][1]
		                          + footMtx[2][1] * footMtx[2][1]);
		f32 footSideLen = MsSqrtf(footMtx[0][0] * footMtx[0][0]
		                          + footMtx[1][0] * footMtx[1][0]
		                          + footMtx[2][0] * footMtx[2][0]);

		negNormal.negate();
		negNormal.scale(footFwdLen);

		footMtx[0][1] = negNormal.x;
		footMtx[1][1] = negNormal.y;
		footMtx[2][1] = negNormal.z;

		JGeometry::TVec3<f32> footZDir(footMtx[0][2], footMtx[1][2],
		                               footMtx[2][2]);

		JGeometry::TVec3<f32> footSideRaw;
		footSideRaw.cross2(negNormal, footZDir);
		footSideRaw.normalize();
		footSideRaw.scale(footSideLen);

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
