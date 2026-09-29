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
	return std::abs(SHORTANGLE2DEG(rawAngle));
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
		f32 cosVal = (f32)((f64)cosSq * __frsqrte(cosSq));
		return SHORTANGLE2DEG(matan(cosVal, x));
	}
}

void FeetInvCalc(J3DModel* model, u16 hipIdx, u16 kneeIdx, u16 footIdx,
                 f32 heightOffset)
{
	TPosition3f& kneeMtx
	    = *reinterpret_cast<TPosition3f*>(model->getAnmMtx(kneeIdx));
	TPosition3f& footMtx
	    = *reinterpret_cast<TPosition3f*>(model->getAnmMtx(footIdx));

	JGeometry::TVec3<f32> kneePos, footPos;
	kneeMtx.getTrans(kneePos);
	footMtx.getTrans(footPos);

	JGeometry::TVec3<f32> shin(footPos);
	shin -= kneePos;
	f32 shinLen = shin.length();

	const TBGCheckData* groundData;
	f32 groundY = gpMap->checkGround(footPos.x, footPos.y + shinLen, footPos.z,
	                                 &groundData)
	              + heightOffset;

	if (!(groundY < footPos.y)) {
		footPos.y = groundY;

		TPosition3f& hipMtx
		    = *reinterpret_cast<TPosition3f*>(model->getAnmMtx(hipIdx));
		JGeometry::TVec3<f32> hipPos;
		hipMtx.getTrans(hipPos);

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
		f32 hipAngle = 90.0f - MsAsin(cosHipAngle);

		f32 sinRatio       = (shinLen * MsSin(hipAngle)) / toFootLen;
		f32 kneeSolveAngle = MsAsin(sinRatio);

		f32 rotAngle = kneeSolveAngle - betweenAngleDeg;
		TRotation3f rotMtx;
		MsMtxSetRotZ(rotMtx, rotAngle);
		PSMTXConcat(hipMtx, rotMtx, hipMtx);

		JGeometry::TVec3<f32> dir;
		hipMtx.getXDir(dir);
		dir.setLength(thighLen);

		JGeometry::TVec3<f32> newKnee(hipPos);
		newKnee.add(dir);
		kneePos = newKnee;
		kneeMtx.setTrans(kneePos);

		JGeometry::TVec3<f32> kneeFwd;
		kneeMtx.getZDir(kneeFwd);
		JGeometry::TVec3<f32> kneeSide;
		kneeMtx.getYDir(kneeFwd);
		f32 kneeFwdLen  = kneeFwd.length();
		f32 kneeSideLen = kneeSide.length();

		JGeometry::TVec3<f32> newDir(footPos);
		newDir -= kneePos;
		newDir.setLength(kneeSideLen);
		kneeMtx.setXDir(newDir);

		JGeometry::TVec3<f32> kneeYDir;
		kneeMtx.getXDir(kneeYDir);
		JGeometry::TVec3<f32> upRaw;
		upRaw.cross(newDir, kneeYDir);
		upRaw.setLength(kneeFwdLen);

		kneeMtx.setYDir(upRaw);

		footMtx.setTrans(footPos);

		JGeometry::TVec3<f32> footFwd;
		footMtx.getYDir(footFwd);
		JGeometry::TVec3<f32> footSide;
		footMtx.getXDir(footSide);
		f32 footFwdLen  = footFwd.length();
		f32 footSideLen = footSide.length();

		const JGeometry::TVec3<f32>& normal = groundData->getNormal();
		JGeometry::TVec3<f32> negNormal     = normal * -footFwdLen;

		footMtx.setYDir(negNormal);

		JGeometry::TVec3<f32> footZDir;
		footMtx.getZDir(footZDir);
		JGeometry::TVec3<f32> footUpRaw;
		footUpRaw.cross(negNormal, footZDir);
		footUpRaw.setLength(footSideLen);

		footMtx.setYDir(footUpRaw);
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
