#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTevs.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/JRenderer.hpp>
#include <dolphin/gx.h>
#include <dolphin/gd.h>



J3DShapeMtx::LoadPipeline J3DShapeMtx::mtxLoadPipeline[4] = {
	&J3DShapeMtx::loadMtxIndx_PNGP,
	&J3DShapeMtx::loadMtxIndx_PCPU,
	&J3DShapeMtx::loadMtxIndx_NCPU,
	&J3DShapeMtx::loadMtxIndx_PNCPU,
};
u32 J3DShapeMtx::currentPipeline;

void J3DShapeMtx::loadMtxIndx_PNGP(int id, u16 mtx_index) const
{
	j3dSys.loadPosMtxIndx(id, mtx_index);
	j3dSys.loadNrmMtxIndx(id, mtx_index);
}

void J3DShapeMtx::loadMtxIndx_PCPU(int id, u16 mtx_index) const
{
	GXLoadPosMtxImm(j3dSys.mViewMtx, id * 3);
	j3dSys.loadNrmMtxIndx(id, mtx_index);
}

void J3DShapeMtx::loadMtxIndx_NCPU(int id, u16 mtx_index) const
{
	j3dSys.loadPosMtxIndx(id, mtx_index);
	GXLoadNrmMtxImm(j3dSys.mViewMtx, id * 3);
}

void J3DShapeMtx::loadMtxIndx_PNCPU(int id, u16) const
{
	GXLoadPosMtxImm(j3dSys.mViewMtx, id * 3);
	GXLoadNrmMtxImm(j3dSys.mViewMtx, id * 3);
}

void J3DShapeMtx::load() const
{
	LoadPipeline ld = mtxLoadPipeline[currentPipeline];
	(this->*ld)(0, unk4);
}

void J3DShapeMtx::calcNBTScale(const Vec& param_1, float (*param_2)[3][3],
                               float (*param_3)[3][3])
{
	J3DPSMtx33Copy(param_2[unk4], param_3[unk4]);
	J3DScaleNrmMtx33(param_3[unk4], param_1);
}

// ~J3DShapeMtx is included in the binary and must go *here*
static void dummy(J3DShapeMtx* mtx) { mtx->~J3DShapeMtx(); }

void J3DShapeMtxDL::load() const { GXCallDisplayList(mDisplayList, 0x20); }

void J3DShapeMtxMulti::load() const
{
	LoadPipeline ld = mtxLoadPipeline[currentPipeline];
	for (int i = 0; i < unk8; ++i) {
		if (unkC[i] == 0xffff)
			continue;
		(this->*ld)(i, unkC[i]);
	}
}

void J3DShapeMtxMulti::calcNBTScale(const Vec& param_1, float (*param_2)[3][3],
                                    float (*param_3)[3][3])
{
	for (int i = 0; i < unk8; ++i) {
		if (unkC[i] == 0xffff)
			continue;
		J3DPSMtx33Copy(param_2[unkC[i]], param_3[unkC[i]]);
		J3DScaleNrmMtx33(param_3[unkC[i]], param_1);
	}
}

J3DShapeDraw::J3DShapeDraw(const u8* list, u32 size)
{
	mDisplayList     = list;
	mDisplayListSize = size;
}

void J3DShapeDraw::draw() const
{
	GXCallDisplayList((void*)mDisplayList, mDisplayListSize);
}

void J3DShape::initialize()
{
	unk0           = 0;
	unk4           = 0xffff;
	mElementCount  = 0;
	unk8           = 0;
	unkC           = 0;
	unk10          = 0;
	unk14          = 0;
	unk18          = 0;
	unk1C          = 0;
	unk20          = 0;
	unk24          = 0;
	unk2C          = nullptr;
	mMatrices      = nullptr;
	mDraws         = nullptr;
	unk44          = 0;
	unk48          = 0;
	unk4C          = 0;
	mDrawMatrices  = nullptr;
	mNormMatrices  = nullptr;
	mCurrentViewNo = &j3dDefaultViewNo;
	unk30          = 0;
}

void J3DShape::calcNBTScale(const Vec& param_1, float (*param_2)[3][3],
                            float (*param_3)[3][3])
{
	for (u16 i = 0; i < mElementCount; ++i)
		mMatrices[i]->calcNBTScale(param_1, param_2, param_3);
}

int J3DShape::countBumpMtxNum() const
{
	int result = 0;
	for (u16 i = 0; i < mElementCount; ++i)
		result += mMatrices[i]->getUseMtxNum();
	return result;
}

void J3DShape::makeVtxArrayCmd()
{
	const GXVtxAttrFmtList* vtxAttr = unk44->getVtxAttrFmtList();
	u8 stride[12];
	void* array[12];

	for (int i = 0; i < 12; i++) {
		stride[i] = 0;
		array[i]  = 0;
	}

	for (; vtxAttr->attr != GX_VA_NULL; vtxAttr++) {
		switch (vtxAttr->attr) {
		case GX_VA_POS: {
			if (vtxAttr->type == GX_F32)
				stride[vtxAttr->attr - GX_VA_POS] = 12;
			else
				stride[vtxAttr->attr - GX_VA_POS] = 6;
			array[vtxAttr->attr - GX_VA_POS] = unk44->getVtxPosArray();
		} break;
		case GX_VA_NRM: {
			if (vtxAttr->type == GX_F32)
				stride[vtxAttr->attr - GX_VA_POS] = 12;
			else
				stride[vtxAttr->attr - GX_VA_POS] = 6;
			array[vtxAttr->attr - GX_VA_POS] = unk44->getVtxNormArray();
		} break;
		case GX_VA_CLR0:
		case GX_VA_CLR1: {
			stride[vtxAttr->attr - GX_VA_POS] = 4;
			array[vtxAttr->attr - GX_VA_POS]
			    = unk44->getVtxColorArray(vtxAttr->attr - GX_VA_CLR0);
		} break;
		case GX_VA_TEX0:
		case GX_VA_TEX1:
		case GX_VA_TEX2:
		case GX_VA_TEX3:
		case GX_VA_TEX4:
		case GX_VA_TEX5:
		case GX_VA_TEX6:
		case GX_VA_TEX7: {
			if (vtxAttr->type == GX_F32)
				stride[vtxAttr->attr - GX_VA_POS] = 8;
			else
				stride[vtxAttr->attr - GX_VA_POS] = 4;
			array[vtxAttr->attr - GX_VA_POS]
			    = unk44->getVtxTexCoordArray(vtxAttr->attr - GX_VA_TEX0);
		} break;
		default:
			break;
		}
	}
	for (GXVtxDescList* piVar5 = unk2C; piVar5->attr != GX_VA_NULL; ++piVar5) {
		if ((piVar5->attr == GX_VA_NBT) && (piVar5->type != GX_NONE)) {
			unk30 = 1;
			stride[1] *= 3;
			array[1] = unk44->getVtxNBTArray();
		}
	}

	for (s32 i = 0; i < 0x0C; i++) {
		if (array[i] != 0)
			GDSetArray((GXAttr)(i + GX_VA_POS), array[i], stride[i]);
		else
			GDSetArrayRaw((GXAttr)(i + GX_VA_POS), nullptr, stride[i]);
	}
}

void J3DShape::makeVcdVatCmd()
{
	GDLObj list;

	GDInitGDLObj(&list, mGDCommands, 0xC0);
	__GDCurrentDL = &list;
	GDSetVtxDescv(unk2C);
	makeVtxArrayCmd();
	J3DSetVtxAttrFmtv(GX_VTXFMT0, unk44->getVtxAttrFmtList(), unk30);
	GDPadCurr32();
	GDFlushCurrToMem();
	__GDCurrentDL = nullptr;
}

void J3DShape::loadVtxArray() const
{
	J3DLoadArrayBasePtr(GX_VA_POS, j3dSys.unk10C);
	if (unk30 == 0) {
		void* l = j3dSys.unk110;
		J3DLoadArrayBasePtr(GX_VA_NRM, l);
	}
	J3DLoadArrayBasePtr(GX_VA_CLR0, j3dSys.unk114);
}

void J3DShape::draw() const
{
	GXCallDisplayList(mGDCommands, 0xC0);

	J3DShapeMtx::currentPipeline = unk8 >> 2 & 3;
	loadVtxArray();

	j3dSys.setModelDrawMtx(mDrawMatrices[*mCurrentViewNo]);
	j3dSys.setModelNrmMtx(mNormMatrices[*mCurrentViewNo]);

	JRNLoadCurrentMtx(0, unk3C[0], unk3C[1], unk3C[2], unk3C[3], unk3C[4],
	                  unk3C[5], unk3C[6], unk3C[7]);

	for (u16 i = 0; i < mElementCount; ++i) {
		if (mMatrices[i])
			mMatrices[i]->load();
		if (mDraws[i])
			mDraws[i]->draw();
	}
}
