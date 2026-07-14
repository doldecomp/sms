#include "MarioUtil/RandomUtil.hpp"
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/SunMgr.hpp>
#include <Camera/Camera.hpp>
#include <System/MarDirector.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/J3D/J3DGraphBase/Blocks/J3DPEBlocks.hpp>
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
#include <dolphin/os/OSCache.h>
#include <stdlib.h>

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
	unk10    = 1;
	unk12    = gpSunMgr->unk18;
	unk12.a  = 0;
	unk24[0] = 30.0f;
	unk24[1] = 650.0f;
	unk24[2] = 1500.0f;

	// quadratic-fit solve for the light attenuation coefficients k0/k1/k2
	// (unk30/34/38) such that 1/atten == k0 + k1*d + k2*d^2 passes through the
	// three sample points atten = {0.9, 0.5, 0.05} at distances
	// d = {30, 650, 1500}. k0 is eliminated by subtracting adjacent equations
	// scaled by a[i]*a[i+1], leaving a 2x2 system solved via Cramer's rule.
	f32 m[3][2];
	f32 dist[3];
	f32 atten[3] = { 0.9f, 0.5f, 0.05f };

	for (int i = 0; i < 3; ++i)
		dist[i] = unk24[i];

	for (int i = 0; i < 2; ++i) {
		m[0][i]
		    = atten[i + 1]
		      * (atten[i] * (dist[i] * dist[i] - dist[i + 1] * dist[i + 1]));
		m[1][i] = atten[i + 1] * (atten[i] * (dist[i] - dist[i + 1]));
		m[2][i] = atten[i + 1] - atten[i];
	}

	unk38 = (m[2][0] * m[1][1] - m[2][1] * m[1][0])
	        / (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
	unk34 = (m[2][0] - m[0][0] * unk38) / m[1][0];
	unk30 = atten[0] - (dist[0] * dist[0] * unk38 + dist[0] * unk34);
	unk3C = 8e-05f;

	if (gpPollution->getJointModelNum() > 0) {
		const ResTIMG* img = gpPollution->getLayer(0)->getPollutionImage();
		unk40              = new JUTTexture(img);
	}
	const ResTIMG* pRVar8
	    = (const ResTIMG*)JKRGetResource("/common/timg/H_marukage_xlu_i8.bti");

	unk44 = new JUTTexture(pRVar8);
	unk48 = 0.0f;
	unk4C = 0.01f;
	unk50 = 128.0f;
}

void TSilhouette::setting(MtxPtr param_1)
{
	GXSetChanAmbColor(GX_COLOR0A0, (GXColor) { unk12.r, unk12.g, unk12.b, 0 });
	GXLightObj GStack_54;
	Vec local_60;
	Vec local_6C = SMS_GetMarioPos();
	MTXMultVec(param_1, &local_6C, &local_60);
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

void TSilhouette::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if ((cue & CUE_MOVE) != 0) {
		f32 fVar1 = SMS_CheckMarioFlag(MARIO_FLAG_OCCLUDED) ? unk50 : 0.0f;
		unk48 += unk4C * (fVar1 - unk48);
		unk12.a = unk48;
	}

	if ((cue & CUE_DRAW) != 0) {
		GXSetNumChans(1);
		GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 1,
		              GX_DF_NONE, GX_AF_SPOT);
		GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXSetChanMatColor(GX_COLOR0A0, unk12);
		setting(graphics->getViewMtx());
	}
	if ((cue & CUE_DRAW_INIT) != 0) {
		GXColor color = unk12;
		color.a       = gpSunMgr->getUnk1CAlpha();
		GXSetChanMatColor(GX_COLOR0A0, color);
		setting(graphics->getViewMtx());
	}
	if (((cue & CUE_SET_PROJECTION) != 0) && gpPollution->getJointModelNum()) {
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

void TTrembleModelEffect::init(J3DModel* model)
{
	int found = 0;
	unk0      = model;
	unk10     = 0;

	GXVtxAttrFmtList* fmt
	    = model->getModelData()->getVertexData().getVtxAttrFmtList();
	while (fmt->attr != GX_VA_NULL) {
		if (fmt->attr == GX_VA_POS) {
			if (fmt->type == GX_S16) {
				unk8  = 0;
				found = 1;
				unkA  = fmt->frac;
				unkC  = 1 << fmt->frac;
			} else if (fmt->type == GX_F32) {
				unk8  = 0;
				found = 1;
				unk8 |= 2;
				unkA = 0;
				unkC = 1;
			}
			break;
		}
	}

	if (found == 1) {
		unk4  = unk0->getModelData()->getVertexData().getVtxPosArray();
		u32 n = unk0->getModelData()->getVertexData().getVtxNum();
		unk9  = 0;
		switch (unk8 & 2) {
		case 0: {
			unk14     = new JGeometry::TVec3<s16>[n];
			unk18[0]  = new JGeometry::TVec3<s16>[n];
			unk18[1]  = new JGeometry::TVec3<s16>[n];
			unk20     = new JGeometry::TVec3<s16>[n];
			unk24     = 0;
			unk26     = 0;
			void* src = model->getModelData()->getVertexData().getVtxPosArray();
			for (u32 i = 0; i < n; ++i) {
				unk14[i]    = ((JGeometry::TVec3<s16>*)src)[i];
				unk18[0][i] = ((JGeometry::TVec3<s16>*)src)[i];
				unk18[1][i] = ((JGeometry::TVec3<s16>*)src)[i];
				unk20[i].set(0, 0, 0);
			}
			break;
		}
		case 2: {
			unk28     = new JGeometry::TVec3<f32>[n];
			unk2C[0]  = new JGeometry::TVec3<f32>[n];
			unk2C[1]  = new JGeometry::TVec3<f32>[n];
			unk34     = new JGeometry::TVec3<f32>[n];
			unk38     = 0.0f;
			unk3C     = 0.0f;
			void* src = model->getModelData()->getVertexData().getVtxPosArray();
			for (u32 i = 0; i < n; ++i) {
				unk28[i]    = ((JGeometry::TVec3<f32>*)src)[i];
				unk2C[0][i] = ((JGeometry::TVec3<f32>*)src)[i];
				unk2C[1][i] = ((JGeometry::TVec3<f32>*)src)[i];
				unk34[i].set(0.0f, 0.0f, 0.0f);
			}
			break;
		}
		}
	}
}

void TTrembleModelEffect::tremble(f32 magnitude, f32 spring, f32 damping,
                                  int duration)
{
	unk8 |= 1;
	switch (unk8 & 2) {
	case 0: {
		unk26                      = (s16)(spring * unkC);
		unk24                      = (s16)(damping * unkC);
		JGeometry::TVec3<s16>* src = (JGeometry::TVec3<s16>*)unk4;
		for (u32 i = 0; i < unk0->getModelData()->getVertexData().getVtxNum();
		     ++i) {
			unk20[i].x  = (s16)(unkC * (magnitude * (2.0f * MsRandF() - 1.0f)));
			unk20[i].y  = (s16)(unkC * (magnitude * (2.0f * MsRandF() - 1.0f)));
			unk20[i].z  = (s16)(unkC * (magnitude * (2.0f * MsRandF() - 1.0f)));
			unk14[i]    = src[i];
			unk18[0][i] = src[i];
			unk18[1][i] = src[i];
		}
		break;
	}
	case 2: {
		unk3C                      = spring;
		unk38                      = damping;
		JGeometry::TVec3<f32>* src = (JGeometry::TVec3<f32>*)unk4;
		for (u32 i = 0; i < unk0->getModelData()->getVertexData().getVtxNum();
		     ++i) {
			unk34[i].x  = magnitude * (2.0f * MsRandF() - 1.0f);
			unk34[i].y  = magnitude * (2.0f * MsRandF() - 1.0f);
			unk34[i].z  = magnitude * (2.0f * MsRandF() - 1.0f);
			unk28[i]    = src[i];
			unk2C[0][i] = src[i];
			unk2C[1][i] = src[i];
		}
		break;
	}
	}
	unk10 = duration;
	unk8 &= ~4;
}

void TTrembleModelEffect::clash(f32 magnitude)
{
	tremble(magnitude, 0.0f, 0.0f, 0);
	switch (unk8 & 2) {
	case 0:
		for (u32 i = 0; i < unk0->getModelData()->getVertexData().getVtxNum();
		     ++i) {
			JGeometry::TVec3<s16> t = unk14[i] + unk20[i];
			unk14[i]                = t;
			unk18[0][i]             = t;
			unk18[1][i]             = t;
		}
		break;

	case 2:
		for (u32 i = 0; i < unk0->getModelData()->getVertexData().getVtxNum();
		     ++i) {
			JGeometry::TVec3<f32> t = unk28[i] + unk34[i];
			unk28[i]                = t;
			unk2C[0][i]             = t;
			unk2C[1][i]             = t;
		}
		break;
	}
	unk8 |= 4;
}

void TTrembleModelEffect::movement()
{
	if (!(unk8 & 1))
		return;

	if ((unk8 & 4) != 4) {
		unk10--;
		if (unk10 <= 0) {
			reset();
			return;
		}
	}

	switch (unk8 & 2) {
	case 0: {
		JGeometry::TVec3<s16>* src = (JGeometry::TVec3<s16>*)unk4;
		for (u32 i = 0; i < unk0->getModelData()->getVertexData().getVtxNum();
		     ++i) {
			JGeometry::TVec3<s16> diff = src[i] - unk14[i];
			unk20[i].x += (s16)((diff.x * unk26) >> unkA);
			unk20[i].y += (s16)((diff.y * unk26) >> unkA);
			unk20[i].z += (s16)((diff.z * unk26) >> unkA);
			unk20[i].x = (s16)((unk24 * unk20[i].x) >> unkA);
			unk20[i].y = (s16)((unk24 * unk20[i].y) >> unkA);
			unk20[i].z = (s16)((unk24 * unk20[i].z) >> unkA);
			unk14[i] += unk20[i];
			unk18[unk9][i] = unk14[i];
		}
		DCFlushRange(unk18[unk9],
		             unk0->getModelData()->getVertexData().getVtxNum() * 6);
		unk0->getVertexBuffer()->unk4[0] = unk18[unk9];
		for (int j = 0; j < unk0->getModelData()->getShapeNum(); ++j)
			unk0->getShapePacket(0)->unk24 = unk18[unk9];
		break;
	}
	case 2: {
		JGeometry::TVec3<f32>* src = (JGeometry::TVec3<f32>*)unk4;
		for (u32 i = 0; i < unk0->getModelData()->getVertexData().getVtxNum();
		     ++i) {
			JGeometry::TVec3<f32> diff = src[i] - unk28[i];
			unk34[i].x += diff.x * unk3C;
			unk34[i].y += diff.y * unk3C;
			unk34[i].z += diff.z * unk3C;
			unk34[i] *= unk38;
			unk28[i].add(unk34[i]);
			unk2C[unk9][i] = unk28[i];
		}
		DCFlushRange(unk2C[unk9],
		             unk0->getModelData()->getVertexData().getVtxNum() * 12);
		unk0->getVertexBuffer()->unk4[0] = unk2C[unk9];
		for (int j = 0; j < unk0->getModelData()->getShapeNum(); ++j)
			unk0->getShapePacket(0)->unk24 = unk2C[unk9];
		break;
	}
	}

	unk9 = 1 - unk9;
}

void TTrembleModelEffect::reset()
{
	switch (unk8 & 2) {
	case 0: {
		JGeometry::TVec3<s16>* src = (JGeometry::TVec3<s16>*)unk4;
		for (u32 i = 0; i < unk0->getModelData()->getVertexData().getVtxNum();
		     i++) {
			unk20[i].set(0, 0, 0);
			unk14[i]    = src[i];
			unk18[0][i] = src[i];
			unk18[1][i] = src[i];
		}
		break;
	}
	case 2: {
		JGeometry::TVec3<f32>* src = (JGeometry::TVec3<f32>*)unk4;
		for (u32 i = 0; i < unk0->getModelData()->getVertexData().getVtxNum();
		     i++) {
			unk34[i].set(0.0f, 0.0f, 0.0f);
			unk28[i]    = src[i];
			unk2C[0][i] = src[i];
			unk2C[1][i] = src[i];
		}
		break;
	}
	}

	unk8 &= ~1;
	GXInvalidateVtxCache();
	unk0->getModelData()->getVertexData().mVtxPosArray = unk4;
	unk0->getVertexBuffer()->unk4[0]                   = unk4;
	unk0->getVertexBuffer()->unk4[1]                   = unk4;
	unk0->getVertexBuffer()->unk2C                     = unk4;
}

void SMS_AddDamageFogEffect(J3DModelData* param_1,
                            const JGeometry::TVec3<f32>& param_2,
                            JDrama::TGraphics* param_3)
{
	Vec local_80;
	MTXMultVec(param_3->getViewMtx(), param_2, &local_80);

	f32 startBase = -700.0f;
	f32 endBase   = 500.0f;
	f32 s         = JMASSin((s16)(gpMarDirector->unk58 * 0x888));
	f32 startOsc  = (-400.0f - startBase) * s;
	f32 endOsc    = (800.0f - endBase) * s;

	for (u16 i = 0; i < param_1->getMaterialNum(); i++) {
		J3DFog* fog
		    = param_1->getMaterialNodePointer(i)->getPEBlock()->getFog();
		fog->mStartZ = -local_80.z + startBase + startOsc;
		fog->mEndZ   = -local_80.z + endBase + endOsc;
		fog->mNearZ  = gpCamera->getNear();
		fog->mFarZ   = gpCamera->getFar();
	}
}

void SMS_ResetDamageFogEffect(J3DModelData* param_1)
{
	for (u16 i = 0; i < param_1->getMaterialNum(); i++) {
		J3DFog* fog
		    = param_1->getMaterialNodePointer(i)->getPEBlock()->getFog();
		fog->mNearZ  = gpCamera->getNear();
		fog->mFarZ   = gpCamera->getFar();
		fog->mEndZ   = fog->mFarZ;
		fog->mStartZ = fog->mEndZ - 1.0f;
	}
}

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

int SMS_CountPolygonNumInShape(J3DShape* shape)
{
	int sizeTable[4] = {
		/* GX_NONE   */ 0,
		/* GX_DIRECT */ 1,
		/* GX_INDEX8 */ 1,
		/* GX_INDEX16*/ 2,
	};

	int polyNum = 0;
	int vtxSize = 0;
	for (GXVtxDescList* desc = shape->getVtxDesc(); desc->attr != GX_VA_NULL;
	     desc++) {
		vtxSize += sizeTable[desc->type];
	}

	for (u16 i = 0; i < shape->getMtxGroupNum(); i++) {
		u8* dl = shape->getShapeDraw(i)->getDisplayList();
		u8* p  = dl;
		while (p - dl < shape->getShapeDraw(i)->getDisplayListSize()) {
			u8 op = *p;
			if (op == GX_TRIANGLEFAN || op == GX_TRIANGLESTRIP) {
				u16 n   = *(u16*)(p + 1);
				polyNum = n + polyNum;
				p += vtxSize * n;
				polyNum -= 2;
				p += 3;
			} else {
				break;
			}
		}
	}
	return polyNum;
}

void SMS_CountPolygonNumInModelData(J3DModelData*) { }

void makeCollisionIndexList(u32, JGeometry::TVec3<u16>*) { }

void SMS_CreatePolygonOrderListInShape(J3DShape*, JGeometry::TVec3<u16>*) { }

void SMS_CreatePolygonOrderListInModelData(J3DModelData*,
                                           JGeometry::TVec3<u16>*)
{
}

void SMS_DrawBillboardLine(const JDrama::TGraphics*,
                           const JGeometry::TVec3<f32>*, int, int, f32,
                           const GXColor*)
{
}

void SMS_DrawCube(const JGeometry::TVec3<f32>& param_1,
                  const JGeometry::TVec3<f32>& param_2)
{
	GXBegin(GX_QUADS, GX_VTXFMT0, 24);

	GXPosition3f32(param_1.x, param_1.y, param_1.z);
	GXPosition3f32(param_1.x, param_1.y, param_2.z);
	GXPosition3f32(param_2.x, param_1.y, param_2.z);
	GXPosition3f32(param_2.x, param_1.y, param_1.z);

	GXPosition3f32(param_1.x, param_1.y, param_1.z);
	GXPosition3f32(param_2.x, param_1.y, param_1.z);
	GXPosition3f32(param_2.x, param_2.y, param_1.z);
	GXPosition3f32(param_1.x, param_2.y, param_1.z);

	GXPosition3f32(param_1.x, param_1.y, param_1.z);
	GXPosition3f32(param_1.x, param_2.y, param_1.z);
	GXPosition3f32(param_1.x, param_2.y, param_2.z);
	GXPosition3f32(param_1.x, param_1.y, param_2.z);

	GXPosition3f32(param_2.x, param_2.y, param_2.z);
	GXPosition3f32(param_1.x, param_2.y, param_2.z);
	GXPosition3f32(param_1.x, param_2.y, param_1.z);
	GXPosition3f32(param_2.x, param_2.y, param_1.z);

	GXPosition3f32(param_2.x, param_2.y, param_2.z);
	GXPosition3f32(param_2.x, param_1.y, param_2.z);
	GXPosition3f32(param_1.x, param_1.y, param_2.z);
	GXPosition3f32(param_1.x, param_2.y, param_2.z);

	GXPosition3f32(param_2.x, param_2.y, param_2.z);
	GXPosition3f32(param_2.x, param_2.y, param_1.z);
	GXPosition3f32(param_2.x, param_1.y, param_1.z);
	GXPosition3f32(param_2.x, param_1.y, param_2.z);

	GXEnd();
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

void MsWireInit(GXVtxFmt) { }

void MsDrawCube(const JGeometry::TVec3<f32>&, f32, GXColor) { }

void MsDrawAxis(MtxPtr, f32) { }

void SMS_DrawBeamAux(int) { }

void SMS_DrawConeBeam(const JGeometry::TVec3<f32>&,
                      const JGeometry::TVec3<f32>&, f32, int, const GXColor&)
{
}

void SMS_DrawHorzCircle(const JGeometry::TVec3<f32>&, f32, int, const GXColor&)
{
}

void SMS_CalcMatAnmAndMakeDL(J3DModel* param_1, u16 param_2)
{
	J3DMaterial* mat = param_1->getModelData()->getMaterialNodePointer(param_2);

	param_1->getModelData()
	    ->getMaterialNodePointer(param_2)
	    ->getMaterialAnm()
	    ->calc(mat);
	j3dSys.setMatPacket(param_1->getMatPacket(param_2));
	mat->makeDisplayList();
}

void SMS_CopyMaterialToSort(J3DMaterial*, J3DModel*, u16) { }

void SMS_UnifyMaterial(J3DModel* param_1)
{
	J3DModelData* modelData = param_1->getModelData();
	J3DMaterial* unifier    = modelData->getMaterialNodePointer(0);
	for (u16 i = 0; i < modelData->getMaterialNum(); ++i) {
		J3DMaterial* mat = param_1->getModelData()->getMaterialNodePointer(i);
		u32 thing        = unifier->unk18 & 0x7fffffff;
		mat->unk18       = thing;
		param_1->getMatPacket(i)->unk3C = thing;
		mat->setTexNo(0, unifier->getTexNo(0));
	}
}
