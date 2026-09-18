#include <Enemy/FeetInv.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JMath.hpp>

// fabricated: the acos/asin pair below is a pattern this binary uses in several
// TUs (compare TSpineEnemy::calcTurnSpeedToReach in enemy.cpp, which spells the
// acos form out by hand). Both go through matan() and a refinement-free
// frsqrte, whose result has to pass through a volatile float.
static inline f32 FeetInvAsinDeg(f32 cosine)
{
	if (cosine == 1.0f)
		return 90.0f;

	if (cosine == -1.0f)
		return -90.0f;

	f32 sine       = -(cosine * cosine - 1.0f);
	volatile f32 f = sine * __frsqrte(sine);
	return matan(f, cosine) * (360.0f / 65536.0f);
}

static inline f32 FeetInvAcosDeg(f32 cosine)
{
	if (cosine == 1.0f)
		return 0.0f;

	if (cosine == -1.0f)
		return 180.0f;

	f32 sine       = -(cosine * cosine - 1.0f);
	volatile f32 f = sine * __frsqrte(sine);
	f32 asine      = matan(f, cosine) * (360.0f / 65536.0f);
	return 90.0f - asine;
}

// fabricated: the length of one column of a joint matrix, i.e. the scale the
// animation baked into that local axis.
static inline f32 FeetInvAxisLength(MtxPtr mtx, int axis)
{
	return MsSqrtf(mtx[0][axis] * mtx[0][axis] + mtx[1][axis] * mtx[1][axis]
	               + mtx[2][axis] * mtx[2][axis]);
}

// fabricated: unsigned angle in degrees between two vectors, from the ratio of
// the cross product's length to the dot product.
static inline f32 FeetInvAngleBetween(const JGeometry::TVec3<f32>& a,
                                      const JGeometry::TVec3<f32>& b)
{
	f32 dot = a.dot(b);
	JGeometry::TVec3<f32> axis;
	axis.cross(a, b);
	return MsAtan2(dot, MsVECMag2(axis));
}

/**
 * @brief Bends one leg of an already animated skeleton so that its foot rests
 * on the ground, i.e. a two-bone inverse kinematics solve in the plane of the
 * leg.
 *
 * @param model model whose joint matrices are adjusted in place
 * @param jnt_hip joint at the top of the leg, which stays where it is
 * @param jnt_knee joint between the two bones, which is what actually moves
 * @param jnt_foot joint at the bottom of the leg, which is put on the ground
 * @param height how far above the ground the foot joint should sit
 */
void FeetInvCalc(J3DModel* model, u16 jnt_hip, u16 jnt_knee, u16 jnt_foot,
                 f32 height)
{
	// TODO: 92.3%. Every instruction is accounted for and in the right
	// order; what is left is register allocation. In four places (hipPos,
	// kneePos, shin and the ground normal) the original keeps the vector's
	// y and z components in registers and reuses them a few statements
	// later, while reloading x; our build reloads all three, which costs
	// two loads per site and renumbers the surrounding float registers.
	// Neither an inline wrapper around the column stores, spelling the
	// lengths as declare-then-assign, nor writing the axis lengths out
	// instead of through FeetInvAxisLength() changes that choice.
	// The frame is also 0x68 bytes short of the original's 0x280. A
	// temporary volatile pad of that size reproduces the prologue exactly
	// and leaves every instruction unchanged, so the missing stack is
	// unreferenced locals, not a missing computation; one of those holes is
	// a 4-byte slot the original reserves between toKnee and toFoot.
	// Re-measured in closure batch 87: **every** `r1` displacement in the
	// original is exactly 0x68 higher than ours, the LR and the four FPR/four
	// GPR save slots included, so all 104 bytes sit *below* every named local,
	// i.e. they are inline-expansion temporaries and none of them can be
	// recovered as a body local. The diff is 343 operand-only differences plus
	// 31 pairs of the same instruction scheduled one slot apart and no missing
	// or extra computation. The four "reload .x, keep .y/.z in callee-saved
	// FPRs" sites are the pattern docs/catalog/frame-gaps.md records for
	// TMapCollisionBase::updateTrans and CPolarSubCamera::updateDemoCamera_
	// (closure batch 83, still unexplained), so this unit is blocked on the
	// same research item rather than on anything local to it.

	MtxPtr kneeMtx = model->getAnmMtx(jnt_knee);
	MtxPtr footMtx = model->getAnmMtx(jnt_foot);

	JGeometry::TVec3<f32> kneePos;
	kneePos.x = kneeMtx[0][3];
	kneePos.y = kneeMtx[1][3];
	kneePos.z = kneeMtx[2][3];

	JGeometry::TVec3<f32> footPos;
	footPos.x = footMtx[0][3];
	footPos.y = footMtx[1][3];
	footPos.z = footMtx[2][3];

	JGeometry::TVec3<f32> kneeToFoot = footPos;
	kneeToFoot -= kneePos;
	f32 f31 = kneeToFoot.length();

	const TBGCheckData* ground;
	f32 footY   = footPos.y;
	f32 groundY = gpMap->checkGround(footPos.x, footY + f31, footPos.z, &ground)
	              + height;

	if (groundY < footY)
		return;

	footPos.y = groundY;

	MtxPtr hipMtx = model->getAnmMtx(jnt_hip);
	JGeometry::TVec3<f32> hipPos;
	hipPos.x = hipMtx[0][3];
	hipPos.y = hipMtx[1][3];
	hipPos.z = hipMtx[2][3];

	JGeometry::TVec3<f32> toKnee = kneePos;
	toKnee -= hipPos;
	f32 f30 = toKnee.length();

	JGeometry::TVec3<f32> toFoot = footPos;
	toFoot -= hipPos;
	f32 f29 = toFoot.length();

	f32 f28 = FeetInvAngleBetween(toFoot, toKnee);

	f32 knee = FeetInvAcosDeg((f30 * f30 + f31 * f31 - f29 * f29)
	                          / (2.0f * f30 * f31));
	f32 hip  = FeetInvAsinDeg(f31 * JMASin(knee) / f29);

	f32 rz    = -(hip - f28);
	f32 sinRz = JMASin(rz);
	f32 cosRz = JMACos(rz);

	Mtx rot;
	rot[0][0] = cosRz;
	rot[0][1] = -sinRz;
	rot[0][2] = 0.0f;
	rot[0][3] = 0.0f;
	rot[1][0] = sinRz;
	rot[1][1] = cosRz;
	rot[1][2] = 0.0f;
	rot[1][3] = 0.0f;
	rot[2][0] = 0.0f;
	rot[2][1] = 0.0f;
	rot[2][2] = 1.0f;
	rot[2][3] = 0.0f;
	MTXConcat(hipMtx, rot, hipMtx);

	JGeometry::TVec3<f32> thigh(hipMtx[0][0], hipMtx[1][0], hipMtx[2][0]);
	thigh.normalize();
	thigh.scale(f30);

	kneePos = hipPos + thigh;
	kneeMtx[0][3] = kneePos.x;
	kneeMtx[1][3] = kneePos.y;
	kneeMtx[2][3] = kneePos.z;

	JGeometry::TVec3<f32> shin = footPos;
	f32 kneeZLen = FeetInvAxisLength(kneeMtx, 2);
	f32 kneeXLen = FeetInvAxisLength(kneeMtx, 0);

	shin -= kneePos;
	shin.normalize();
	shin.scale(kneeXLen);
	kneeMtx[0][0] = shin.x;
	kneeMtx[1][0] = shin.y;
	kneeMtx[2][0] = shin.z;

	JGeometry::TVec3<f32> kneeY(kneeMtx[0][1], kneeMtx[1][1], kneeMtx[2][1]);
	JGeometry::TVec3<f32> kneeZ;
	kneeZ.cross(shin, kneeY);
	kneeZ.normalize();
	kneeZ.scale(kneeZLen);
	kneeMtx[0][2] = kneeZ.x;
	kneeMtx[1][2] = kneeZ.y;
	kneeMtx[2][2] = kneeZ.z;

	footMtx[0][3] = footPos.x;
	footMtx[1][3] = footPos.y;
	footMtx[2][3] = footPos.z;

	JGeometry::TVec3<f32> up = ground->getNormal();
	f32 footYLen = FeetInvAxisLength(footMtx, 1);
	f32 footXLen = FeetInvAxisLength(footMtx, 0);

	up.negate();
	up.scale(footYLen);
	footMtx[0][1] = up.x;
	footMtx[1][1] = up.y;
	footMtx[2][1] = up.z;

	JGeometry::TVec3<f32> footZ(footMtx[0][2], footMtx[1][2], footMtx[2][2]);
	JGeometry::TVec3<f32> footX;
	footX.cross(up, footZ);
	footX.normalize();
	footX.scale(footXLen);
	footMtx[0][0] = footX.x;
	footMtx[1][0] = footX.y;
	footMtx[2][0] = footX.z;
}

TMtxCalcFootInv::TMtxCalcFootInv(u16 param_1, u16 param_2, u16 param_3,
                                 u16 param_4, u16 param_5, u16 param_6,
                                 f32 param_7)
    : J3DMtxCalcSoftimageAnm(nullptr)
    , unk68(param_1)
    , unk6A(param_2)
    , unk6C(param_3)
    , unk6E(param_4)
    , unk70(param_5)
    , unk72(param_6)
    , unk74(param_7)
{
}

void TMtxCalcFootInv::calc(u16 jnt_idx)
{
	J3DMtxCalcAnm::calc(jnt_idx);

	if (unk6C == jnt_idx)
		FeetInvCalc(j3dSys.getModel(), unk68, unk6A, unk6C, unk74);

	if (unk72 == jnt_idx)
		FeetInvCalc(j3dSys.getModel(), unk6E, unk70, unk72, unk74);
}
