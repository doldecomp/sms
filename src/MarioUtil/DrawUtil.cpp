#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/SunMgr.hpp>
#include <Map/JointModel.hpp>
#include <Map/PollutionManager.hpp>
#include <Map/PollutionLayer.hpp>
#include <System/TexCache.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

TSilhouette* gpSilhouetteManager;

void TSilhouette::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	unk12               = (GXColor) { 0, 0x27, 0x77, 0xff };
	unk16               = (GXColor) { 0xff, 0xff, 0xff, 0xff };
	unk1C               = 100.0f;
	unk20               = 0.9f;
	gpSilhouetteManager = this;
}

void TSilhouette::loadAfter()
{
	// TODO: ewwww floats
	unk10     = 1;
	unk12     = gpSunMgr->unk18;
	unk12.a   = 0;
	unk24     = 30.0f;
	unk28     = 650.0f;
	unk2C     = 1500.0f;
	f32 fVar1 = unk24;
	f32 fVar2 = unk28;
	f32 fVar3 = unk2C;
	f32 fVar4 = (fVar1 - fVar2) * 0.9f * 0.5f;
	f32 fVar5 = (fVar1 * fVar1 - fVar2 * fVar2) * 0.9f * 0.5f;
	f32 fVar6 = (fVar2 - fVar3) * 0.5f * 0.05f;
	unk38     = (fVar6 * -0.4f - fVar4 * -0.45f)
	        / (fVar5 * fVar6
	           - (fVar2 * fVar2 - fVar3 * fVar3) * 0.5f * 0.05f * fVar4);
	unk34 = -(fVar5 * unk38 - -0.4f) / fVar4;
	unk30 = 0.9 - (unk38 * fVar1 * fVar1 + fVar1 * unk34);
	unk3C = 8e-05f;

	if (gpPollution->getJointModelNum() > 0) {
		TPollutionLayer* pJVar9
		    = (TPollutionLayer*)gpPollution->getJointModel(0);
		ResTIMG* img = pJVar9->unk58;
		unk40        = new JUTTexture(img);
	}
	ResTIMG* pRVar8 = (ResTIMG*)JKRFileLoader::getGlbResource(
	    "/common/timg/H_marukage_xlu_i8.bti");

	unk44 = new JUTTexture(pRVar8);
	unk48 = 0.0f;
	unk4C = 0.01f;
	unk50 = 128.0f;
}

void TSilhouette::setting(MtxPtr param_1)
{
	GXSetChanAmbColor(GX_COLOR0A0, (GXColor) { unk12.r, unk12.g, unk12.b, 0 });
	Vec local_6C = *gpMarioPos;
	Vec local_60;
	PSMTXMultVec(param_1, &local_6C, &local_60);
	GXLightObj GStack_54;
	GXInitLightPos(&GStack_54, local_60.x, local_60.y, local_60.z);
	GXInitLightAttnK(&GStack_54, unk30, unk34, unk38);
	GXInitLightAttnA(&GStack_54, 1.0f, 0.0f, 0.0f);
	unk16   = unk12;
	unk16.a = unk48;
	GXInitLightColor(&GStack_54, unk16);
	GXLoadLightObjImm(&GStack_54, GX_LIGHT0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_TRUE, GX_SRC_REG, GX_SRC_REG, 1, GX_DF_NONE,
	              GX_AF_SPOT);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
	GXSetZMode(GX_TRUE, GX_GEQUAL, GX_FALSE);
}

void TSilhouette::perform(u32 param_1, JDrama::TGraphics* param_2)
{

	if ((param_1 & 1) != 0) {
		f32 fVar1 = SMS_CheckMarioFlag(1) ? unk50 : 0.0f;
		unk48 += unk4C * (fVar1 - unk48);
		unk12.a = unk48;
	}

	if ((param_1 & 8) != 0) {
		GXSetNumChans(1);
		GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 1,
		              GX_DF_NONE, GX_AF_SPOT);
		GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXSetChanMatColor(GX_COLOR0A0, unk12);
		setting(param_2->getUnkB4());
	}
	if ((param_1 & 0x80) != 0) {
		GXColor color = unk12;
		color.a       = gpSunMgr->getUnk1CAlpha();
		GXSetChanMatColor(GX_COLOR0A0, color);
		setting(param_2->getUnkB4());
	}
	if (((param_1 & 0x10) != 0) && gpPollution->getJointModelNum()) {
		Mtx afStack_80;
		C_MTXLightFrustum(afStack_80, -1.0f, 1.0f, -1.0f, 1.0f, 10.0f, 0.5f,
		                  0.5f, 0.5f, 0.5f);
		Mtx afStack_b0;
		PSMTXRotRad(afStack_b0, 0x58, 1.570796f);
		Mtx afStack_50;
		PSMTXConcat(afStack_80, afStack_b0, afStack_50);
		Mtx afStack_e0;
		PSMTXScale(afStack_e0, unk3C, unk3C, unk3C);
		Mtx afStack_110;
		PSMTXTrans(afStack_110, -gpMarioPos->x, 0.0f, -gpMarioPos->z);
		Mtx afStack_140;
		PSMTXTrans(afStack_140, 1.75f, 1.75f, 0.0f);
		PSMTXConcat(afStack_e0, afStack_110, afStack_e0);
		PSMTXConcat(afStack_50, afStack_e0, afStack_50);
		PSMTXConcat(afStack_140, afStack_50, afStack_50);
		GXLoadTexMtxImm(afStack_50, 0x1e, GX_MTX3x4);
		GXSetNumTexGens(2);
		GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0,
		                  0x7d);
		GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_POS, 0x1e, 0, 0x7d);
		unk40->load(GX_TEXMAP0);
		unk44->load(GX_TEXMAP1);
		GXSetNumChans(1);
		GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_REG, 1, GX_DF_NONE,
		              GX_AF_SPOT);
		GXSetChanCtrl(GX_COLOR1A1, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
		              GX_AF_NONE);
		GXColor color = unk12;
		GXSetChanMatColor(GX_COLOR0A0, color);
		color.a = 0x40;
		GXSetTevColor(GX_TEVREG0, color);
		GXSetNumTevStages(2);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC,
		                GX_CC_C0);
		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                GX_TEVPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA,
		                GX_CA_A0);
		GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                GX_TEVPREV);
		GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
		GXSetTevOp(GX_TEVSTAGE1, GX_MODULATE);
		GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
		               GX_LO_NOOP);
		GXSetZCompLoc(GX_TRUE);
		GXSetZMode(GX_TRUE, GX_GEQUAL, GX_FALSE);
	}
}

void TSilhouette::calcSilhouetteBorder() { }

void TTrembleModelEffect::init(J3DModel*) { }

void TTrembleModelEffect::tremble(f32, f32, f32, int) { }

void TTrembleModelEffect::clash(f32) { }

void TTrembleModelEffect::movement() { }

void TTrembleModelEffect::reset() { }

void SMS_AddDamageFogEffect(J3DModelData*, const JGeometry::TVec3<f32>&,
                            JDrama::TGraphics*)
{
}

void SMS_ResetDamageFogEffect(J3DModelData*) { }

// fabricated
struct Plane {
	f32 nx;
	f32 ny;
	f32 nz;
	f32 offset; // from origin

	// fabricated
	f32 sdf(Vec* local)
	{
		return local->x * nx + local->y * ny + local->z * nz + offset;
	}

	// fabricated
	void set(Vec* normal, Vec* point)
	{
		nx = normal->x;
		ny = normal->y;
		nz = normal->z;
		// project any point on a plane onto it's normal and you get the
		// distance from the origin
		offset = -(normal->x * point->x + normal->y * point->y
		           + normal->z * point->z);
	}

	// fabricated
	void set(Vec* p1, Vec* p2, Vec* p3)
	{
		Vec v1;
		Vec v2;
		Vec normal;

		VECSubtract(p1, p3, &v1);
		VECSubtract(p2, p3, &v2);
		VECCrossProduct(&v1, &v2, &normal);
		MsVECNormalize(&normal, &normal);

		nx = normal.x;
		ny = normal.y;
		nz = normal.z;
		// project any point on a plane onto it's normal and you get the
		// distance from the origin
		offset = -(normal.x * p3->x + normal.y * p3->y + normal.z * p3->z);
	}
};

Plane sViewPlane[6];

static void SetViewFrustumClipCheck(f32 top, f32 bottom, f32 left, f32 right,
                                    f32 near, f32 far)
{
	f32 farTop    = top * (far / near);
	f32 farBottom = bottom * (far / near);
	f32 farLeft   = left * (far / near);
	f32 farRight  = right * (far / near);

	Vec local_98;
	Vec local_8c;
	Vec local_80;
	Vec local_74;
	Vec local_68;
	Vec local_5c;
	Vec local_50;
	Vec local_44;

	local_44.x = left;
	local_44.y = top;
	local_44.z = -near;

	local_50.x = farLeft;
	local_50.y = farTop;
	local_50.z = -far;

	local_5c.x = right;
	local_5c.y = top;
	local_5c.z = -near;

	local_68.x = farRight;
	local_68.y = farTop;
	local_68.z = -far;

	local_74.x = left;
	local_74.y = bottom;
	local_74.z = -near;

	local_80.x = farLeft;
	local_80.y = farBottom;
	local_80.z = -far;

	local_8c.x = right;
	local_8c.y = bottom;
	local_8c.z = -near;

	local_98.x = farRight;
	local_98.y = farBottom;
	local_98.z = -far;

	Vec v1;
	Vec v2;
	Vec normal;

	VECSubtract(&local_50, &local_44, &v1);
	VECSubtract(&local_5c, &local_44, &v2);
	VECCrossProduct(&v1, &v2, &normal);
	MsVECNormalize(&normal, &normal);
	sViewPlane[0].set(&normal, &local_44);

	VECSubtract(&local_8c, &local_74, &v1);
	VECSubtract(&local_80, &local_74, &v2);
	VECCrossProduct(&v1, &v2, &normal);
	MsVECNormalize(&normal, &normal);
	sViewPlane[1].set(&normal, &local_74);

	VECSubtract(&local_74, &local_44, &v1);
	VECSubtract(&local_50, &local_44, &v2);
	VECCrossProduct(&v1, &v2, &normal);
	MsVECNormalize(&normal, &normal);
	sViewPlane[2].set(&normal, &local_44);

	VECSubtract(&local_68, &local_5c, &v1);
	VECSubtract(&local_8c, &local_5c, &v2);
	VECCrossProduct(&v1, &v2, &normal);
	MsVECNormalize(&normal, &normal);
	sViewPlane[3].set(&normal, &local_5c);

	VECSubtract(&local_5c, &local_44, &v1);
	VECSubtract(&local_74, &local_44, &v2);
	VECCrossProduct(&v1, &v2, &normal);
	MsVECNormalize(&normal, &normal);
	sViewPlane[4].set(&normal, &local_44);

	VECSubtract(&local_68, &local_98, &v1);
	VECSubtract(&local_80, &local_98, &v2);
	VECCrossProduct(&v1, &v2, &normal);
	MsVECNormalize(&normal, &normal);
	sViewPlane[5].set(&normal, &local_98);
}

static f32 sKeepViewClipFovy;
static f32 sKeepViewClipAspect;
static f32 sKeepViewClipNear;
static f32 sKeepViewClipFar;

void SetViewFrustumClipCheckPerspective(f32 fovy, f32 aspect, f32 clip_near,
                                        f32 clip_far)
{
	if (fovy != sKeepViewClipFovy || aspect != sKeepViewClipAspect
	    || clip_near != sKeepViewClipNear || clip_far != sKeepViewClipFar) {
		sKeepViewClipFovy   = fovy;
		sKeepViewClipAspect = aspect;
		sKeepViewClipNear   = clip_near;
		sKeepViewClipFar    = clip_far;

		//       E
		//      /|\
		//     / | \
		//    /  |  \
		//   /   |   \
		//  /____|____\
		// L     C     R
		//
		// Vertical slice of camera frustsum, E is the eye, LR is the near
		// plane. LER then is the fovy. LEC is half of it. Tan of LEC is
		// by definition LC/EC, but EC is the near plane dist, so we get
		// what we want -- LC, half the vertical span of the near plane.
		f32 tan          = tanf(fovy * (3.141593f / 180.0f / 2.0f));
		f32 vertHalfSize = clip_near * tan;
		f32 horHalfSize  = vertHalfSize * aspect;
		SetViewFrustumClipCheck(vertHalfSize, -vertHalfSize, -horHalfSize,
		                        horHalfSize, clip_near, clip_far);
	}
}

BOOL ViewFrustumClipCheck(JDrama::TGraphics* gfx, Vec* position, f32 radius)
{
	Vec local_18;
	MTXMultVec(gfx->mViewMtx, position, &local_18);

	for (int i = 0; i < 6; ++i)
		if (-radius > sViewPlane[i].sdf(&local_18))
			return false;

	return true;
}

void ViewFrustumRectClipCheck(JDrama::TGraphics*, Vec*, f32, f32) { }

int SMS_CountPolygonNumInShape(J3DShape*) { }

void SMS_CountPolygonNumInModelData(J3DModelData*) { }

void makeCollisionIndexList(u32, JGeometry::TVec3<u16>*) { }

void SMS_CreatePolygonOrderListInShape(J3DShape*, JGeometry::TVec3<u16>*) { }

void SMS_CreatePolygonOrderListInModelData(J3DModelData*,
                                           JGeometry::TVec3<u16>*)
{
}

void SMS_DrawBillboardLine(const JDrama::TGraphics*,
                           const JGeometry::TVec3<f32>*, int, int, f32,
                           const _GXColor*)
{
}

void SMS_DrawCube(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&) {
}

void SMS_SettingDrawShape(J3DModelData* param_1, u16 param_2)
{
	J3DShape* shape = param_1->getShapeNodePointer(param_2);
	GXCallDisplayList(shape->getDrawList(), 0xC0);
	J3DVertexData& data = param_1->getVertexData();
	j3dSys.unk10C       = data.getVtxPosArray();
	j3dSys.unk110       = data.getVtxNormArray();
	shape->loadVtxArray();
}

void SMS_DrawShape(J3DModelData* param_1, u16 param_2)
{
	J3DShape* shape = param_1->getShapeNodePointer(param_2);
	for (u16 i = 0; i < shape->getMtxGroupNum(); ++i)
		shape->getShapeDraw(i)->draw();
}

void SMS_MakeDLAndLock(J3DModel* param_1)
{
	param_1->prepareShapePackets();
	for (u16 i = 0; i < param_1->getModelData()->getMaterialNum(); ++i) {
		param_1->getModelData()->getMaterialNodePointer(i)->calc(
		    (MtxPtr)j3dDefaultMtx);
	}
	param_1->makeDL();
	param_1->lock();
}

void SMS_DrawInit()
{
	j3dSys.drawInit();
	SMS_ResetTexCacheRegion();
}

void SMS_ShowJoint(J3DMaterial* param_1, bool param_2)
{
	if (param_2) {
		for (; param_1 != nullptr; param_1 = param_1->getNext())
			param_1->getShape()->offFlag(1);
	} else {
		for (; param_1 != nullptr; param_1 = param_1->getNext())
			param_1->getShape()->onFlag(1);
	}
}

void MsWireInit(_GXVtxFmt) { }

void MsDrawCube(const JGeometry::TVec3<f32>&, f32, _GXColor) { }

void MsDrawAxis(MtxPtr, f32) { }

void SMS_DrawBeamAux(int) { }

void SMS_DrawConeBeam(const JGeometry::TVec3<f32>&,
                      const JGeometry::TVec3<f32>&, f32, int, const _GXColor&)
{
}

void SMS_DrawHorzCircle(const JGeometry::TVec3<f32>&, f32, int, const _GXColor&)
{
}

void SMS_CalcMatAnmAndMakeDL(J3DModel* param_1, u16 param_2)
{
	J3DMaterial* mat = param_1->getModelData()->getMaterialNodePointer(param_2);
	mat->getMaterialAnm()->calc(mat);
	j3dSys.setMatPacket(param_1->getMatPacket(param_2));
	mat->makeDisplayList();
}

void SMS_CopyMaterialToSort(J3DMaterial*, J3DModel*, u16) { }

void SMS_UnifyMaterial(J3DModel* param_1)
{
	J3DModelData* modelData = param_1->getModelData();
	J3DMaterial* unifier    = modelData->getMaterialNodePointer(0);
	for (u16 i = 0; i < modelData->getMaterialNum(); i = i + 1) {
		J3DMaterial* mat = param_1->getModelData()->getMaterialNodePointer(i);
		u32 thing        = unifier->unk18 & 0x7fffffff;
		mat->unk18       = thing;
		param_1->getMatPacket(i)->unk3C = thing;

		u16 texNo = unifier->getTevBlock()->getTexNo(0);
		mat->getTevBlock()->setTexNo(0, texNo);
	}
}
