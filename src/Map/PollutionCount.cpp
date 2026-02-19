#include <Map/PollutionCount.hpp>
#include <Map/PollutionLayer.hpp>
#include <Map/PollutionManager.hpp>
#include <MarioUtil/ReinitGX.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <System/DrawSyncManager.hpp>
#include <JSystem/ResTIMG.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DDrawBuffer.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static void drawBlack(u16 param_1, u16 param_2)
{

	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U16, 1);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetCurrentMtx(0);
	GXSetCullMode(GX_CULL_NONE);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0x0, 0x0, 0x0, 0xff });
	GXSetNumTexGens(0);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
	GXSetNumTevStages(1);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
	GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition2u16(0, 1);
	GXPosition2u16(param_1 << 1, 1);
	GXPosition2u16(param_1 << 1, (param_2 << 1) + 1);
	GXPosition2u16(0, (param_2 << 1) + 1);
	GXEnd();
}

void TPollutionCounterBase::setCallback(int param_1) const
{
	TDrawSyncManager::smInstance->pushBreakPoint();
	GXSetDrawSync(getCounterNo(param_1));
	TDrawSyncManager::smInstance->pushBreakPoint();
	GXSetDrawSync(0);
}

void TPollutionCounterBase::drawSyncCallback(u16 param_1)
{
	int token = getTokenNo(param_1);
	u32 discard;
	GXReadPixMetric(&discard, &discard, &discard, &discard, unkC[token],
	                &discard);
	*unkC[token] -= unk10[token] * 4;
}

void TPollutionCounterBase::initCounters(int param_1)
{
	unk4  = param_1;
	unkC  = new u32*[unk4];
	unk10 = new u32[param_1];
	for (int i = 0; i < param_1; ++i) {
		unkC[i]  = nullptr;
		unk10[i] = 0;
	}
}

void loadPollutionLayer(const u8* param_1, u16 param_2, u16 param_3,
                        GXTexMapID param_4)
{
	GXTexObj GStack_40;
	GXInitTexObj(&GStack_40, (u8*)param_1, param_2, param_3, GX_TF_I8, GX_CLAMP,
	             GX_CLAMP, 0);
	GXInitTexObjLOD(&GStack_40, GX_NEAR, GX_NEAR, 0.0f, 1.0f, 0.0f, GX_FALSE,
	                GX_FALSE, GX_ANISO_1);
	GXLoadTexObj(&GStack_40, param_4);
}

static void initDrawObjGX()
{
	Mtx local_34;

	local_34[2][3] = 0.0f;
	local_34[1][3] = 0.0f;
	local_34[0][3] = 0.0f;
	local_34[1][2] = 0.0f;
	local_34[0][2] = 0.0f;
	local_34[2][1] = 0.0f;
	local_34[0][1] = 0.0f;
	local_34[2][0] = 0.0f;
	local_34[1][0] = 0.0f;
	local_34[2][2] = 1.0f;
	local_34[1][1] = 1.0f;
	local_34[0][0] = 1.0f;
	GXLoadPosMtxImm(local_34, 0);
	GXSetCurrentMtx(0);
	GXSetNumChans(0);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, 0xff });
	GXSetNumTexGens(1);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetCullMode(GX_CULL_NONE);
	GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
	GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
}

static void initCountObjDegree(f32, f32) { }

static void drawShape(J3DShape* shape)
{
	GXCallDisplayList(shape->getDrawList(), 0xC0);
	shape->loadVtxArray();
	for (u16 k = 0; k < shape->getMtxGroupNum(); ++k) {
		shape->getShapeDraw(k)->draw();
	}
}

void TPollutionCounterObj::draw(int param_1) const
{
	if (unk14 == nullptr)
		return;

	if (unk14[param_1] == nullptr)
		return;

	const ResTIMG* pRVar6 = unk14[param_1]->unk34->getUnk58();
	loadPollutionLayer((u8*)pRVar6 + pRVar6->imageDataOffset, pRVar6->width,
	                   pRVar6->height, GX_TEXMAP0);
	j3dSys.setVtxPos(unk14[param_1]->unk34->getModelData()->getVtxPosArray());
	GXClearPixMetric();
	for (int j = 0; j < unk14[param_1]->getShapeNum(); ++j) {
		drawShape(unk14[param_1]->getShape(j));
	}
}

// TODO: hack
static inline void setEular(TRotation3f& rot, s16 yaw, s16 pitch, s16 roll)
{
	rot.setEular(yaw, pitch, roll);
}

void TPollutionCounterObj::countObjDegree() const
{
	ReInitializeGX();
	initDrawObjGX();
	for (u16 i = 0; i < unk8; ++i) {
		J3DJoint* pJVar5 = unk14[i]->getJoint();

		const Vec& minVec = pJVar5->getMin();
		f32 fVar1         = minVec.z;
		f32 fVar2         = minVec.x;

		TPosition3f local_AC;
		TPosition3f local_7C;

		local_7C.identity();

		local_AC.identity();
		local_AC.setScale(0.03125f, 0.03125f, 0.0f);

		MTXConcat(local_7C.mMtx, local_AC.mMtx, local_7C.mMtx);
		setEular(local_AC, DEG2SHORTANGLE(-90.0f), 0, 0);
		MTXConcat(local_7C.mMtx, local_AC.mMtx, local_7C.mMtx);
		local_7C.setTrans(-fVar1 / 32, -fVar2 / 32, 0);
		GXLoadPosMtxImm(local_7C.mMtx, 0);
		draw(i);
		setCallback(i);
	}
}

void TPollutionCounterObj::registerPollutionObj(TPollutionObj* param_1,
                                                u32* param_2)
{
	unk14[unk8] = param_1;
	for (int i = 0; i < param_1->getShapeNum(); ++i) {
		J3DShape* shape = param_1->getShape(i);
		unk10[unk8] += SMS_CountPolygonNumInShape(shape);
	}
	unkC[unk8] = param_2;
	*param_2   = 0xffffffff;
	++unk8;
}

void TPollutionCounterObj::init(int param_1)
{
	TPollutionCounterBase::initCounters(param_1);

	unk14 = new TPollutionObj*[param_1];
	for (int i = 0; i < param_1; ++i) {
		unk14[i] = nullptr;
		unk10[i] = nullptr;
	}
}

TPollutionCounterObj::TPollutionCounterObj()
    : unk14(nullptr)
{
}

static void drawTex(u16 param_1, u16 param_2)
{
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U16, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U16, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetNumTexGens(1);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition2u16(0, 0);
	GXTexCoord2u16(0, 0);
	GXPosition2u16(param_1, 0);
	GXTexCoord2u16(1, 0);
	GXPosition2u16(param_1, param_2);
	GXTexCoord2u16(1, 1);
	GXPosition2u16(0, param_2);
	GXTexCoord2u16(0, 1);
	GXEnd();
}

static void initGXforPollutionLayer(int param_1, u16 param_2, u8 param_3,
                                    u8 param_4)
{
	if ((param_2 & 2) != 0) {
		param_4 = 2;
	}
	GXSetCurrentMtx(0);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
	GXSetNumChans(0);
	GXSetTevColor(GX_TEVREG0, (GXColor) { param_4, param_4, param_4, param_4 });
	GXSetTevColor(GX_TEVREG1, (GXColor) { param_3, param_3, param_3, param_3 });
	if (param_1 == 7) {
		GXSetNumTevStages(2);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_C1, GX_CC_C0,
		                GX_CC_ZERO);
		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_COMP_R8_GT, GX_TB_ZERO,
		                GX_CS_SCALE_1, 1, GX_TEVPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_A1, GX_CA_A0,
		                GX_CA_ZERO);
		GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_R8_GT, GX_TB_ZERO,
		                GX_CS_SCALE_1, 1, GX_TEVPREV);
		GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
		GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO,
		                GX_CC_TEXC);
		GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                GX_TEVPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_ZERO, GX_CA_ZERO,
		                GX_CA_TEXA);
		GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                GX_TEVPREV);
	} else if ((param_2 & 2) != 0) {
		GXSetNumTevStages(1);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO,
		                GX_CC_TEXC);
		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                GX_TEVPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_ZERO,
		                GX_CA_TEXA);
		GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                GX_TEVPREV);
	} else {
		GXSetNumTevStages(2);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_TEXC, GX_CC_C0,
		                GX_CC_ZERO);
		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_COMP_R8_GT, GX_TB_ZERO,
		                GX_CS_SCALE_1, 1, GX_TEVPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A1, GX_CA_TEXA, GX_CA_A0,
		                GX_CA_ZERO);
		GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_R8_GT, GX_TB_ZERO,
		                GX_CS_SCALE_1, 1, GX_TEVPREV);
		GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
		GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO,
		                GX_CC_TEXC);
		GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                GX_TEVPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_ZERO, GX_CA_ZERO,
		                GX_CA_TEXA);
		GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                GX_TEVPREV);
	}
	GXSetCullMode(GX_CULL_NONE);
	GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
	GXSetZMode(0, GX_ALWAYS, 0);
	GXSetAlphaUpdate(0);
}

void TPollutionCounterLayer::drawPollutionLayer(int param_1) const { }

static void makeWorldToPollutionMtx(f32 param_1, f32 param_2, f32 param_3,
                                    TPosition3f* mtx)
{
	for (int j = 0; j < 4; ++j)
		for (int i = 0; i < 3; ++i)
			mtx->mMtx[i][j] = 0.0f;

	mtx->mMtx[0][0] = param_1;
	mtx->mMtx[0][3] = -param_3 * param_1;
	mtx->mMtx[1][2] = param_1;
	mtx->mMtx[1][3] = -param_2 * param_1;
}

void TPollutionCounterLayer::drawJointObjStamp(int param_1) const
{
	for (int i = 0; i < unkD4; ++i) {
		const TPollutionJointObjTaskInfo& info = unkD8[i];
		if (info.unk1 != param_1)
			continue;

		TPollutionLayer* pTVar2
		    = (TPollutionLayer*)gpPollution->getJointModel(info.unk1);

		initDrawObjGX();
		GXSetNumChans(1);
		GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
		              GX_AF_NONE);
		GXSetChanCtrl(GX_COLOR1A1, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
		              GX_AF_NONE);

		if (unkD8[i].unk0 == 0) {
			GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0, 0, 0, 0xff });
		} else {
			GXSetChanMatColor(GX_COLOR0A0,
			                  (GXColor) { 0xff, 0xff, 0xff, 0xff });
		}
		GXSetNumTevStages(1);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		              GX_COLOR0A0);
		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_RASC, GX_CC_ZERO, GX_CC_ZERO,
		                GX_CC_ZERO);
		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                GX_TEVPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_RASA, GX_CA_ZERO, GX_CA_ZERO,
		                GX_CA_ZERO);
		GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                GX_TEVPREV);

		TPosition3f local_6c;
		makeWorldToPollutionMtx(pTVar2->unk5C.mInverseVerticalScale,
		                        pTVar2->unk38, pTVar2->unk40, &local_6c);
		GXLoadPosMtxImm(local_6c, 0);

		j3dSys.setVtxPos(pTVar2->getModelData()->getVtxPosArray());
		for (int j = 0; j < unkD8[i].unk4->getShapeNum(); ++j) {
			drawShape(unkD8[i].unk4->getShape(j));
		}
	}
}

static void doTask(u16 param_1, int param_2, TPollutionLayerTaskInfo* param_3,
                   ResTIMG* param_4, u16 param_5, u16 param_6, s32 param_7)
{
	JUTTexture texture(param_4);
	texture.load(GX_TEXMAP0);

	int sVar10 = 1;
	int sVar9  = 1;
	if (param_5 <= param_6) {
		sVar10 = param_6 / param_5;
	} else {
		sVar9 = param_5 / param_6;
	}

	for (u16 i = 0; i < param_2; ++i, ++param_3) {
		if (param_1 != param_3->unk0)
			continue;
		int iVar5 = param_3->unk8 - param_7;
		int iVar7 = param_3->unk8 + param_7;
		if (iVar5 < 0) {
			iVar5 = 0;
		}
		if (iVar7 > 0xff) {
			iVar7 = 0xff;
		}

		GXSetTevColor(GX_TEVREG0, (GXColor) { iVar5, iVar5, iVar5, iVar5 });
		GXSetTevColor(GX_TEVREG1, (GXColor) { iVar7, iVar7, iVar7, iVar7 });
		s16 x1 = param_3->unk2 - (param_3->unk6 / 2);
		s16 y1 = param_3->unk4 - (param_3->unk6 / 2);
		s16 x2 = param_3->unk2 + (param_3->unk6 / 2);
		s16 y2 = param_3->unk4 + (param_3->unk6 / 2);
		u16 s1 = x1 * sVar10;
		u16 s2 = x2 * sVar10;
		u16 t1 = y1 * sVar9;
		u16 t2 = y2 * sVar9;
		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition2s16(x1, y1);
		GXTexCoord2u16(0, 0);
		GXTexCoord2u16(s1, t1);
		GXPosition2s16(x2, y1);
		GXTexCoord2u16(1, 0);
		GXTexCoord2u16(s2, t1);
		GXPosition2s16(x2, y2);
		GXTexCoord2u16(1, 1);
		GXTexCoord2u16(s2, t2);
		GXPosition2s16(x1, y2);
		GXTexCoord2u16(0, 1);
		GXTexCoord2u16(s1, t2);
		GXEnd();
	}
}

static void initGXforStamp(const u8* param_1, u16 param_2, u16 param_3,
                           int param_4)
{
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_S16, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U16, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_S16, param_4);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
	GXSetNumTexGens(2);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
	GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x3c, 0, 0x7d);
	GXTexObj GStack_34;
	GXInitTexObj(&GStack_34, (void*)param_1, param_2, param_3, GX_TF_I8,
	             GX_CLAMP, GX_CLAMP, 0);
	GXInitTexObjLOD(&GStack_34, GX_NEAR, GX_NEAR, 0.0f, 1.0f, 0.0f, 0, 0,
	                GX_ANISO_1);
	GXLoadTexObj(&GStack_34, GX_TEXMAP1);
	GXSetNumTevStages(3);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_A0, GX_CA_KONST,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_RGB8_GT, GX_TB_ZERO,
	                GX_CS_SCALE_1, 1, GX_TEVREG2);
	GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
	GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_A1, GX_CA_TEXA, GX_CA_A2, GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_COMP_RGB8_GT, GX_TB_ZERO,
	                GX_CS_SCALE_1, 1, GX_TEVPREV);
	GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_TEXA, GX_CA_APREV,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
}

// TODO: size doesn't match MAP, may be wrong? Does it really draw a tex stamp?
void TPollutionCounterLayer::drawTexStamp(int param_1) const
{
	const TPollutionLayer* layer2 = unk14[param_1];

	int uVar1 = layer2->unk5C.unk8 >= layer2->unk5C.unkC
	                ? uVar1 = layer2->unk5C.unk8
	                : layer2->unk5C.unkC;

	initGXforStamp(layer2->unk5C.mMap, layer2->unk58->width,
	               layer2->unk58->height, uVar1);

	for (int i = 0; i < unk1A; ++i) {
		TPollutionTexStamp& stamp = unk1C[i];
		setTevColorInByStampType(unk1C[i].unk0);
		doTask(param_1, stamp.unk8, stamp.unk10, stamp.unk4,
		       unk14[param_1]->getUnk58()->width,
		       unk14[param_1]->getUnk58()->height, unk14[param_1]->getUnk48());
	}
}

void TPollutionCounterLayer::drawRevivalTexStamp(int param_1) const
{
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_S16, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U16, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetNumTexGens(0);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
	GXSetNumChans(1);
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, 0x14 });
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ONE, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_RASA, GX_CA_ZERO, GX_CA_ZERO,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetCullMode(GX_CULL_NONE);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0xff);
	GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);

	for (int i = 0; i < unk22; ++i) {
		TPollutionRevivalTexStamp& stamp = unk24[i];

		if (stamp.unk18 > 0) {
			++stamp.unk14;
			if (stamp.unk14 >= stamp.unk18) {
				stamp.unk14 = 0;
			} else {
				continue;
			}
		}

		if (stamp.unk4 != param_1)
			continue;

		if (!(stamp.unk0 == 0 ? true : false))
			continue;

		JUTTexture texture(stamp.unk10);
		texture.load(GX_TEXMAP0);
		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition2s16(stamp.unk8, stamp.unkA);
		GXTexCoord2u16(0, 0);
		GXPosition2s16(stamp.unk8 + stamp.unkC, stamp.unkA);
		GXTexCoord2u16(1, 0);
		GXPosition2s16(stamp.unk8 + stamp.unkC, stamp.unkA + stamp.unkE);
		GXTexCoord2u16(1, 1);
		GXPosition2s16(stamp.unk8, stamp.unkA + stamp.unkE);
		GXTexCoord2u16(0, 1);
		GXEnd();
	}
}

void TPollutionCounterLayer::cleanProhibitArea(int param_1) const
{

	GXSetNumTexGens(1);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U16, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U16, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetTevColor(GX_TEVREG1, (GXColor) { 0, 0, 0, 200 });
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR_NULL);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_A1, GX_CA_KONST,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_RGB8_GT, GX_TB_ZERO,
	                GX_CS_SCALE_1, 1, GX_TEVPREV);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ZERO, GX_LO_NOOP);
	GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_GREATER, 0);

	u32 w = unk14[param_1]->getUnk58()->width;
	u32 h = unk14[param_1]->getUnk58()->height;

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition2u16(0, 0);
	GXTexCoord2u16(0, 0);
	GXPosition2u16(w, 0);
	GXTexCoord2u16(1, 0);
	GXPosition2u16(w, h);
	GXTexCoord2u16(1, 1);
	GXPosition2u16(0, h);
	GXTexCoord2u16(0, 1);
	GXEnd();
}

void TPollutionCounterLayer::drawModelStamp(int) { }

void TPollutionCounterLayer::countTexDegree(int param_1)
{
	if (unk178[param_1] == 0)
		return;

	ReInitializeGX();

	// TODO: drawPollutionLayer or nah? size doesn't match
	const TPollutionLayer* layer = unk14[param_1];
	ResTIMG* img                 = layer->unk58;
	drawBlack(img->width, img->height);
	loadPollutionLayer((u8*)img + img->imageDataOffset, img->width, img->height,
	                   GX_TEXMAP0);
	initGXforPollutionLayer(layer->unk30, layer->unk32, layer->unk85,
	                        layer->unk84);

	GXClearPixMetric();
	drawTex(unk14[param_1]->unk58->width, unk14[param_1]->unk58->height);
	setCallback(param_1);
	if (unk28 != 0) {
		j3dSys.unk4C = 7;
		unk2C[param_1]->draw();
		unk2C[param_1]->frameInit();
	}

	ReInitializeGX();
	drawJointObjStamp(param_1);
	drawTexStamp(param_1);
	drawRevivalTexStamp(param_1);
	if (unk30[param_1] != 0)
		cleanProhibitArea(param_1);
}

void TPollutionCounterLayer::resetTask()
{
	for (int i = 0; i < unk1A; ++i)
		unk1C[i].unk8 = 0;

	for (int i = 0; i < unk8; ++i)
		unk30[i] = nullptr;

	unk28 = 0;
	unkD4 = 0;
}

void TPollutionCounterLayer::setTevColorInByStampType(u16 type) const
{
	if (type == 0) {
		GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO,
		                GX_CC_ZERO);
	} else {
		GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ONE, GX_CC_ZERO, GX_CC_ZERO,
		                GX_CC_ZERO);
	}
}

void TPollutionCounterLayer::pushJointObjStampTask(u8 param_1, u8 param_2,
                                                   TPollutionObj* param_3)
{
	if (unkD4 >= 0x14)
		return;

	unkD8[unkD4].unk0 = param_1;
	unkD8[unkD4].unk1 = param_2;
	unkD8[unkD4].unk4 = param_3;
	++unkD4;
}

void TPollutionCounterLayer::calcViewMtx()
{
	JGeometry::SMatrix34C<f32> afStack_68;
	afStack_68.set(j3dSys.mViewMtx);
	J3DDrawBuffer* buf1 = j3dSys.mDrawBuffer[1];
	J3DDrawBuffer* buf0 = j3dSys.mDrawBuffer[0];
	for (int i = 0; i < unk8; ++i) {
		TPollutionLayer* iVar3
		    = (TPollutionLayer*)gpPollution->getJointModel(i);

		TPosition3f local_a4;
		makeWorldToPollutionMtx(iVar3->unk5C.mInverseVerticalScale,
		                        iVar3->unk38, iVar3->unk40, &local_a4);
		MtxPtr ptr = local_a4;
		MTXCopy(ptr, j3dSys.mViewMtx);
		j3dSys.mDrawBuffer[0] = unk2C[i];
		j3dSys.mDrawBuffer[1] = unk2C[i];
		for (int j = 0; j < unk28; ++j) {
			if (unk34[j].unk0 != i)
				continue;
			unk34[j].unk4->viewCalc();
			unk34[j].unk4->entry();
		}
	}
	MTXCopy(afStack_68, j3dSys.mViewMtx);
	j3dSys.mDrawBuffer[0] = buf0;
	j3dSys.mDrawBuffer[1] = buf1;
}

void TPollutionCounterLayer::pushModelStampTask(u8 param_1, J3DModel* param_2)
{
	if (unk28 >= 0x14)
		return;

	unk34[unk28].unk0 = param_1;
	unk34[unk28].unk4 = param_2;
	++unk30[param_1];
	++unk28;
}

int TPollutionCounterLayer::registerRevivalTexStamp(int param_1, s16 param_2,
                                                    s16 param_3, s16 param_4,
                                                    s16 param_5, int param_6,
                                                    ResTIMG* param_7)
{
	unk24[unk22].registerTex(param_1, param_2, param_3, param_4, param_5,
	                         param_6, param_7);
	return unk22++;
}

int TPollutionCounterLayer::registerTexStamp(u16 param_1, u16 param_2,
                                             ResTIMG* param_3)
{
	if (unk18 == 0)
		return 0;

	unk1C[unk1A].registerTexStamp(param_1, param_2, param_3);
	return unk1A++;
}

void TPollutionCounterLayer::registerLayer(const TPollutionLayer* param_1,
                                           u32* param_2)
{
	unk14[unk8] = param_1;
	unk10[unk8] = 2;
	unkC[unk8]  = param_2;
	*param_2    = -1;
	++unk8;
}

bool TPollutionCounterLayer::stampIsCleanType(u16 param_1) const
{
	return unk1C[param_1].unk0 == 0 ? true : false;
}

void TPollutionCounterLayer::onLayer(int) { }

void TPollutionCounterLayer::offLayer(int param_1)
{
	unk178[param_1] = 0;
	*unkC[param_1]  = 0;
}

void TPollutionCounterLayer::init(int param_1, u16 param_2, u16 param_3)
{
	TPollutionCounterBase::initCounters(param_1);

	unk14 = new const TPollutionLayer*[unk4];
	for (int i = 0; i < unk4; ++i)
		unk14[i] = nullptr;

	unk18  = param_2;
	unk1C  = new TPollutionTexStamp[unk18];
	unk20  = param_3;
	unk24  = new TPollutionRevivalTexStamp[unk20];
	unk2C  = new J3DDrawBuffer*[param_1];
	unk30  = new u16[param_1];
	unk178 = new u8[param_1];
	for (int i = 0; i < param_1; ++i) {
		unk2C[i]  = new J3DDrawBuffer(0x10);
		unk30[i]  = 0;
		unk178[i] = 1;
	}
}

TPollutionCounterLayer::TPollutionCounterLayer()
    : unk14(nullptr)
    , unk18(0)
    , unk1A(0)
    , unk1C(nullptr)
    , unk20(0)
    , unk22(0)
    , unk24(nullptr)
    , unk28(0)
    , unk2C(nullptr)
    , unk30(nullptr)
    , unkD4(0)
{
}

void TPollutionTexStamp::pushTask(u8 param_1, u16 param_2, u16 param_3,
                                  u16 param_4, s16 param_5)
{
	if (unk8 >= unkC)
		return;

	unk10[unk8].unk2 = param_3;
	unk10[unk8].unk4 = param_4;
	unk10[unk8].unk6 = param_2;
	unk10[unk8].unk0 = param_1;
	unk10[unk8].unk8 = param_5;
	++unk8;
}

void TPollutionTexStamp::registerTexStamp(u16 param_1, u16 param_2,
                                          ResTIMG* param_3)
{
	unk0  = param_1;
	unk4  = param_3;
	unkC  = param_2;
	unk10 = new TPollutionLayerTaskInfo[unkC];
}

TPollutionTexStamp::TPollutionTexStamp()
    : unk0(0)
    , unk2(0)
    , unk4(0)
    , unk8(0)
    , unkC(0)
    , unk10(nullptr)
{
}

void TPollutionRevivalTexStamp::registerTex(int param_1, s16 param_2,
                                            s16 param_3, s16 param_4,
                                            s16 param_5, int param_6,
                                            ResTIMG* param_7)
{
	unk4  = param_1;
	unk8  = param_2;
	unkA  = param_3;
	unkC  = param_4;
	unkE  = param_5;
	unk10 = param_7;
	unk0  = 0;
	unk18 = param_6;
}

TPollutionRevivalTexStamp::TPollutionRevivalTexStamp()
    : unk0(0)
    , unk4(0)
    , unk8(0)
    , unkA(0)
    , unkC(0)
    , unkE(0)
    , unk10(0)
    , unk14(0)
    , unk18(0)
{
}
