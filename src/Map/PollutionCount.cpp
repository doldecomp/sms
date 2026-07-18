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
	GXSetCurrentMtx(GX_PNMTX0);
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
	GXReadPixMetric(&discard, &discard, &discard, &discard, mCounters[token],
	                &discard);
	*mCounters[token] -= mPolygonCount[token] * 4;
}

void TPollutionCounterBase::initCounters(int max_counters)
{
	mCounterCapacity = max_counters;
	mCounters        = new u32*[mCounterCapacity];
	mPolygonCount    = new u32[max_counters];
	for (int i = 0; i < max_counters; ++i) {
		mCounters[i]     = nullptr;
		mPolygonCount[i] = 0;
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
	GXLoadPosMtxImm(local_34, GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
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

static void initCountObjDegree(f32 fVar1, f32 fVar2)
{
	TPosition3f local_7C;
	TPosition3f local_AC;

	local_7C.identity();

	local_AC.identity();
	local_AC.setScale(0.03125f, 0.03125f, 0.0f);

	MTXConcat(local_7C.mMtx, local_AC.mMtx, local_7C.mMtx);
	local_AC.setEular((s16)DEG2SHORTANGLE(-90.0f), 0, 0);
	MTXConcat(local_7C.mMtx, local_AC.mMtx, local_7C.mMtx);
	local_7C.setTrans(-fVar1 / 32, -fVar2 / 32, 0);
	GXLoadPosMtxImm(local_7C.mMtx, GX_PNMTX0);
}

static void drawShape(J3DShape* shape)
{
	GXCallDisplayList(shape->getDrawList(), 0xC0);
	shape->loadVtxArray();
	for (u16 k = 0; k < shape->getMtxGroupNum(); ++k)
		shape->getShapeDraw(k)->draw();
}

void TPollutionCounterObj::draw(int index) const
{
	if (mObjects == nullptr)
		return;

	if (mObjects[index] == nullptr)
		return;

	const ResTIMG* pRVar6 = mObjects[index]->mLayer->getPollutionImage();
	loadPollutionLayer((u8*)pRVar6 + pRVar6->imageDataOffset, pRVar6->width,
	                   pRVar6->height, GX_TEXMAP0);
	j3dSys.setVtxPos(mObjects[index]->mLayer->getModelData()->getVtxPosArray());
	GXClearPixMetric();
	for (int j = 0; j < mObjects[index]->getShapeNum(); ++j)
		drawShape(mObjects[index]->getShape(j));
}

void TPollutionCounterObj::countObjDegree() const
{
	ReInitializeGX();
	initDrawObjGX();
	for (u16 i = 0; i < mCounterNum; ++i) {
		const Vec& minVec = mObjects[i]->getJoint()->getMin();
		initCountObjDegree(minVec.x, minVec.z);
		draw(i);
		setCallback(i);
	}
}

void TPollutionCounterObj::registerPollutionObj(TPollutionObj* obj,
                                                u32* counter_ptr)
{
	mObjects[mCounterNum] = obj;
	for (int i = 0; i < obj->getShapeNum(); ++i) {
		J3DShape* shape = obj->getShape(i);
		mPolygonCount[mCounterNum] += SMS_CountPolygonNumInShape(shape);
	}
	mCounters[mCounterNum] = counter_ptr;
	*counter_ptr           = -1;
	++mCounterNum;
}

void TPollutionCounterObj::init(int max_counters)
{
	TPollutionCounterBase::initCounters(max_counters);

	mObjects = new TPollutionObj*[max_counters];
	for (int i = 0; i < max_counters; ++i) {
		mObjects[i]      = nullptr;
		mPolygonCount[i] = 0;
	}
}

TPollutionCounterObj::TPollutionCounterObj()
    : mObjects(nullptr)
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

static void initGXforPollutionLayer(int pol_type, u16 flags, u8 threshold,
                                    u8 step)
{
	if (flags & TPollutionLayer::FLAG_DECAY)
		step = 2;

	GXSetCurrentMtx(GX_PNMTX0);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
	GXSetNumChans(0);
	GXSetTevColor(GX_TEVREG0, (GXColor) { step, step, step, step });
	GXSetTevColor(GX_TEVREG1,
	              (GXColor) { threshold, threshold, threshold, threshold });
	if (pol_type == POLLUTION_TYPE_UNK7) {
		// Basically, if pollution at a position is above the `threshold`, it
		// gets increased further by the `step`. Dirty stuff gets dirtier.

		GXSetNumTevStages(2);

		// clang-format off
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_C1, GX_CC_C0, GX_CC_ZERO);
		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_COMP_R8_GT, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_A1, GX_CA_A0, GX_CA_ZERO);
		GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_R8_GT, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);

		GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
		GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
		GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
		GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
		// clang-format on
	} else if (flags & TPollutionLayer::FLAG_DECAY) {
		// All pollution uniformly decayed by `step` every frame.

		GXSetNumTevStages(1);

		// clang-format off
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
		GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
		// clang-format on
	} else {
		// If pollution at a position is below the threshold, decrease it
		// further by the `step`. Clan-ish stuff gets cleaner.

		GXSetNumTevStages(2);

		// clang-format off
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_COMP_R8_GT, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A1, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
		GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_R8_GT, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);

		GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
		GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
		GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
		GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
		// clang-format on
	}
	GXSetCullMode(GX_CULL_NONE);
	GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
	GXSetZMode(0, GX_ALWAYS, 0);
	GXSetAlphaUpdate(0);
}

void TPollutionCounterLayer::drawPollutionLayer(int layer_index) const
{
	const TPollutionLayer* layer = mLayers[layer_index];
	const ResTIMG* img           = layer->mPollutionImage;
	drawBlack(img->width, img->height);
	loadPollutionLayer((u8*)img + img->imageDataOffset, img->width, img->height,
	                   GX_TEXMAP0);
	initGXforPollutionLayer(layer->mPollutionType, layer->mFlags,
	                        layer->mPerFrameChangeThreshold,
	                        layer->mPerFrameChangeDelta);

	GXClearPixMetric();
	drawTex(mLayers[layer_index]->mPollutionImage->width,
	        mLayers[layer_index]->mPollutionImage->height);
	setCallback(layer_index);
}

static void makeWorldToPollutionMtx(f32 scale, f32 x, f32 z, TPosition3f* mtx)
{
	mtx->zero();

	mtx->mMtx[0][0] = scale;
	mtx->mMtx[0][3] = -z * scale;
	mtx->mMtx[1][2] = scale;
	mtx->mMtx[1][3] = -x * scale;
}

void TPollutionCounterLayer::drawJointObjStamp(int layer_index) const
{
	for (int i = 0; i < mJointObjStampTaskNum; ++i) {
		const TPollutionJointObjTaskInfo& info = mJointObjStampTaskQueue[i];
		if (info.mLayerIdx != layer_index)
			continue;

		TPollutionLayer* layer = gpPollution->getLayer(info.mLayerIdx);

		initDrawObjGX();
		GXSetNumChans(1);
		GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
		              GX_AF_NONE);
		GXSetChanCtrl(GX_COLOR1A1, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
		              GX_AF_NONE);

		if (mJointObjStampTaskQueue[i].unk0 == 0) {
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
		makeWorldToPollutionMtx(layer->mPos.mInverseTexelScale, layer->mMinX,
		                        layer->mMinZ, &local_6c);
		GXLoadPosMtxImm(local_6c, GX_PNMTX0);

		j3dSys.setVtxPos(layer->getModelData()->getVtxPosArray());
		for (int j = 0; j < mJointObjStampTaskQueue[i].mJointObj->getShapeNum();
		     ++j)
			drawShape(mJointObjStampTaskQueue[i].mJointObj->getShape(j));
	}
}

static void doTask(u16 target_layer, int task_num,
                   TPollutionLayerTaskInfo* tasks, ResTIMG* stamp_tex,
                   u16 pollution_tex_width, u16 pollution_tex_height,
                   s32 param_7)
{
	JUTTexture texture(stamp_tex);
	texture.load(GX_TEXMAP0);

	int sVar10 = 1;
	int sVar9  = 1;
	if (pollution_tex_width <= pollution_tex_height) {
		sVar10 = pollution_tex_height / pollution_tex_width;
	} else {
		sVar9 = pollution_tex_width / pollution_tex_height;
	}

	for (u16 i = 0; i < task_num; ++i, ++tasks) {
		if (target_layer != tasks->mLayerIdx)
			continue;
		int iVar5 = tasks->mY - param_7;
		int iVar7 = tasks->mY + param_7;
		if (iVar5 < 0)
			iVar5 = 0;
		if (iVar7 > 0xff)
			iVar7 = 0xff;

		GXSetTevColor(GX_TEVREG0, (GXColor) { iVar5, iVar5, iVar5, iVar5 });
		GXSetTevColor(GX_TEVREG1, (GXColor) { iVar7, iVar7, iVar7, iVar7 });
		s16 x1 = tasks->mX - (tasks->mSize / 2);
		s16 y1 = tasks->mZ - (tasks->mSize / 2);
		s16 x2 = tasks->mX + (tasks->mSize / 2);
		s16 y2 = tasks->mZ + (tasks->mSize / 2);
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
void TPollutionCounterLayer::drawTexStamp(int target_layer) const
{
	const TPollutionLayer* layer2 = mLayers[target_layer];

	int frac = layer2->mPos.mLog2Width >= layer2->mPos.mLog2Height
	               ? frac = layer2->mPos.mLog2Width
	               : layer2->mPos.mLog2Height;

	initGXforStamp(layer2->mPos.mHeightMap, layer2->mPollutionImage->width,
	               layer2->mPollutionImage->height, frac);

	for (int i = 0; i < mTexStampNum; ++i) {
		TPollutionTexStamp& stamp = mTexStamps[i];
		setTevColorInByStampType(mTexStamps[i].mStampType);
		doTask(target_layer, stamp.mTaskNum, stamp.mTaskQueue,
		       stamp.mStampShapeTex,
		       mLayers[target_layer]->getPollutionImage()->width,
		       mLayers[target_layer]->getPollutionImage()->height,
		       mLayers[target_layer]->getUnk48());
	}
}

void TPollutionCounterLayer::drawRevivalTexStamp(int layer_index) const
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

	for (int i = 0; i < mRevivalTexStampNum; ++i) {
		TPollutionRevivalTexStamp& stamp = mRevivalTexStamps[i];

		if (stamp.mStampInterval > 0) {
			++stamp.mStampTimer;
			if (stamp.mStampTimer >= stamp.mStampInterval) {
				stamp.mStampTimer = 0;
			} else {
				continue;
			}
		}

		if (stamp.mTargetLayer != layer_index)
			continue;

		if (!stamp.isThing())
			continue;

		JUTTexture texture(stamp.mStampShapeTex);
		texture.load(GX_TEXMAP0);
		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition2s16(stamp.mLeft, stamp.mTop);
		GXTexCoord2u16(0, 0);
		GXPosition2s16(stamp.mLeft + stamp.mWidth, stamp.mTop);
		GXTexCoord2u16(1, 0);
		GXPosition2s16(stamp.mLeft + stamp.mWidth, stamp.mTop + stamp.mHeight);
		GXTexCoord2u16(1, 1);
		GXPosition2s16(stamp.mLeft, stamp.mTop + stamp.mHeight);
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

	u32 w = mLayers[param_1]->getPollutionImage()->width;
	u32 h = mLayers[param_1]->getPollutionImage()->height;

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

void TPollutionCounterLayer::countTexDegree(int layer_index)
{
	if (!mIsLayerEnabled[layer_index])
		return;

	ReInitializeGX();
	drawPollutionLayer(layer_index);
	if (mModelStampTaskNum != 0) {
		j3dSys.setUnk4C(7);
		mModelStampDrawBuffers[layer_index]->draw();
		mModelStampDrawBuffers[layer_index]->frameInit();
	}

	ReInitializeGX();
	drawJointObjStamp(layer_index);
	drawTexStamp(layer_index);
	drawRevivalTexStamp(layer_index);
	if (mLayerModelStampTaskNum[layer_index] != 0)
		cleanProhibitArea(layer_index);
}

void TPollutionCounterLayer::resetTask()
{
	for (int i = 0; i < mTexStampNum; ++i)
		mTexStamps[i].mTaskNum = 0;

	for (int i = 0; i < mCounterNum; ++i)
		mLayerModelStampTaskNum[i] = 0;

	mModelStampTaskNum    = 0;
	mJointObjStampTaskNum = 0;
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
                                                   TPollutionObj* joint_obj)
{
	if (mJointObjStampTaskNum >= 20)
		return;

	mJointObjStampTaskQueue[mJointObjStampTaskNum].unk0      = param_1;
	mJointObjStampTaskQueue[mJointObjStampTaskNum].mLayerIdx = param_2;
	mJointObjStampTaskQueue[mJointObjStampTaskNum].mJointObj = joint_obj;
	++mJointObjStampTaskNum;
}

void TPollutionCounterLayer::calcViewMtx()
{
	TMtx34f afStack_68;
	afStack_68.set(j3dSys.getViewMtx());

	J3DDrawBuffer* oldDbOpa = j3dSys.getDrawBuffer(0);
	J3DDrawBuffer* oldDbXlu = j3dSys.getDrawBuffer(1);

	for (int i = 0; i < mCounterNum; ++i) {
		TPollutionLayer* layer = gpPollution->getLayer(i);

		TPosition3f local_a4;
		makeWorldToPollutionMtx(layer->mPos.mInverseTexelScale, layer->mMinX,
		                        layer->mMinZ, &local_a4);

		j3dSys.setViewMtx(local_a4);
		j3dSys.setDrawBuffer(mModelStampDrawBuffers[i], 0);
		j3dSys.setDrawBuffer(mModelStampDrawBuffers[i], 1);

		for (int j = 0; j < mModelStampTaskNum; ++j) {
			if (mModelStampTaskQueue[j].mLayerIdx != i)
				continue;
			mModelStampTaskQueue[j].mModel->viewCalc();
			mModelStampTaskQueue[j].mModel->entry();
		}
	}
	j3dSys.setViewMtx(afStack_68);

	j3dSys.setDrawBuffer(oldDbOpa, 0);
	j3dSys.setDrawBuffer(oldDbXlu, 1);
}

void TPollutionCounterLayer::pushModelStampTask(u8 target_layer,
                                                J3DModel* model)
{
	if (mModelStampTaskNum >= 20)
		return;

	mModelStampTaskQueue[mModelStampTaskNum].mLayerIdx = target_layer;
	mModelStampTaskQueue[mModelStampTaskNum].mModel    = model;
	++mLayerModelStampTaskNum[target_layer];
	++mModelStampTaskNum;
}

int TPollutionCounterLayer::registerRevivalTexStamp(int param_1, s16 param_2,
                                                    s16 param_3, s16 param_4,
                                                    s16 param_5, int param_6,
                                                    ResTIMG* param_7)
{
	mRevivalTexStamps[mRevivalTexStampNum].registerTex(
	    param_1, param_2, param_3, param_4, param_5, param_6, param_7);
	return mRevivalTexStampNum++;
}

int TPollutionCounterLayer::registerTexStamp(u16 param_1, u16 param_2,
                                             ResTIMG* param_3)
{
	if (mTexStampCapacity == 0)
		return 0;

	mTexStamps[mTexStampNum].registerTexStamp(param_1, param_2, param_3);
	return mTexStampNum++;
}

void TPollutionCounterLayer::registerLayer(const TPollutionLayer* layer,
                                           u32* counter)
{
	mLayers[mCounterNum]       = layer;
	mPolygonCount[mCounterNum] = 2;
	mCounters[mCounterNum]     = counter;
	*counter                   = -1;
	++mCounterNum;
}

bool TPollutionCounterLayer::stampIsCleanType(u16 param_1) const
{
	return mTexStamps[param_1].mStampType == 0 ? true : false;
}

void TPollutionCounterLayer::onLayer(int layer_idx)
{
	mIsLayerEnabled[layer_idx] = true;
}

void TPollutionCounterLayer::offLayer(int layer_idx)
{
	mIsLayerEnabled[layer_idx] = false;
	*mCounters[layer_idx]      = 0;
}

void TPollutionCounterLayer::init(int max_counters, u16 max_tex_stamps,
                                  u16 max_revival_tex_stamps)
{
	TPollutionCounterBase::initCounters(max_counters);

	mLayers = new const TPollutionLayer*[mCounterCapacity];
	for (int i = 0; i < mCounterCapacity; ++i)
		mLayers[i] = nullptr;

	mTexStampCapacity        = max_tex_stamps;
	mTexStamps               = new TPollutionTexStamp[mTexStampCapacity];
	mRevivalTexStampCapacity = max_revival_tex_stamps;
	mRevivalTexStamps = new TPollutionRevivalTexStamp[mRevivalTexStampCapacity];
	mModelStampDrawBuffers  = new J3DDrawBuffer*[max_counters];
	mLayerModelStampTaskNum = new u16[max_counters];
	mIsLayerEnabled         = new bool[max_counters];
	for (int i = 0; i < max_counters; ++i) {
		mModelStampDrawBuffers[i]  = new J3DDrawBuffer(0x10);
		mLayerModelStampTaskNum[i] = 0;
		onLayer(i);
	}
}

TPollutionCounterLayer::TPollutionCounterLayer()
    : mLayers(nullptr)
    , mTexStampCapacity(0)
    , mTexStampNum(0)
    , mTexStamps(nullptr)
    , mRevivalTexStampCapacity(0)
    , mRevivalTexStampNum(0)
    , mRevivalTexStamps(nullptr)
    , mModelStampTaskNum(0)
    , mModelStampDrawBuffers(nullptr)
    , mLayerModelStampTaskNum(nullptr)
    , mJointObjStampTaskNum(0)
{
}

void TPollutionTexStamp::pushTask(u8 layer_idx, u16 size, u16 x, u16 z, s16 y)
{
	if (mTaskNum >= mTaskCapacity)
		return;

	mTaskQueue[mTaskNum].mX        = x;
	mTaskQueue[mTaskNum].mZ        = z;
	mTaskQueue[mTaskNum].mSize     = size;
	mTaskQueue[mTaskNum].mLayerIdx = layer_idx;
	mTaskQueue[mTaskNum].mY        = y;
	++mTaskNum;
}

void TPollutionTexStamp::registerTexStamp(u16 stamp_type, u16 param_2,
                                          ResTIMG* shape_tex)
{
	mStampType     = stamp_type;
	mStampShapeTex = shape_tex;
	mTaskCapacity  = param_2;
	mTaskQueue     = new TPollutionLayerTaskInfo[mTaskCapacity];
}

TPollutionTexStamp::TPollutionTexStamp()
    : mStampType(0)
    , unk2(0)
    , mStampShapeTex(0)
    , mTaskNum(0)
    , mTaskCapacity(0)
    , mTaskQueue(nullptr)
{
}

void TPollutionRevivalTexStamp::registerTex(int target_layer, s16 left, s16 top,
                                            s16 width, s16 height,
                                            int stamp_interval,
                                            ResTIMG* shape_tex)
{
	mTargetLayer   = target_layer;
	mLeft          = left;
	mTop           = top;
	mWidth         = width;
	mHeight        = height;
	mStampShapeTex = shape_tex;
	unk0           = 0;
	mStampInterval = stamp_interval;
}

TPollutionRevivalTexStamp::TPollutionRevivalTexStamp()
    : unk0(0)
    , mTargetLayer(0)
    , mLeft(0)
    , mTop(0)
    , mWidth(0)
    , mHeight(0)
    , mStampShapeTex(0)
    , mStampTimer(0)
    , mStampInterval(0)
{
}
