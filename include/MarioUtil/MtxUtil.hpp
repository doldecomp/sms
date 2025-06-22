#ifndef MTX_UTIL_HPP
#define MTX_UTIL_HPP

#include <JSystem/JGeometry.hpp>

class J3DNode;
class J3DModel;
class THitActor;

void MtxToQuat(MtxPtr, Quaternion*);

class TMtxEffectBase {
public:
	void getParams();
};

class TMtxTimeLag {
public:
	void getParams();
	struct TDeParams {
		TDeParams(const char*);
	};
	void calc(MtxPtr);
};

void TMtxTimeLagCallBack(J3DNode*, int);

class TMtxSwingRZ {
public:
	void getParams();
	void calcLocalXY(MtxPtr, Vec*, Vec*);
	void calc(MtxPtr);
};

void TMtxSwingRZCallBack(J3DNode*, int);

class TMtxSwingRZReverseXZ {
public:
	void calc(MtxPtr);
};

void TMtxSwingRZReverseXZCallBack(J3DNode*, int);

class TMultiMtxEffect {
public:
	void setup(J3DModel*, const char*);
	void setUserArea();
	void add();
	void remove();
};

void SMS_MakeJointsToArc(J3DModel*, const JGeometry::TVec3<f32>&,
                         const JGeometry::TVec3<f32>&,
                         const JGeometry::TVec3<f32>&);
void SMS_MtxLookAt(MtxPtr, const JGeometry::TVec3<f32>&,
                   const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&);
void SMS_GetLightPerspectiveForEffectMtx(MtxPtr);

class TRopePoint {
public:
	TRopePoint();
};

class TRope {
public:
	TRope(u16, const JGeometry::TVec3<f32>&, f32, f32, f32, f32);
	void collision();
	void constraintHead(const JGeometry::TVec3<f32>&);
	void constraintTail(const JGeometry::TVec3<f32>&);
	void moveHead(const JGeometry::TVec3<f32>&);
	void moveHeadAndTail(const JGeometry::TVec3<f32>&,
	                     const JGeometry::TVec3<f32>&);
};

void SMS_GetActorMtx(const THitActor&, MtxPtr);

#endif
