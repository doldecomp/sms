#include <MoveBG/MapObjGrass.hpp>
#include <System/MarDirector.hpp>
#include <Camera/Camera.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <dolphin/gx.h>
#include <stdlib.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TMapObjGrassManager* gpMapObjGrassManager;

f32 TMapObjGrassManager::mWidth      = 12.0f;
f32 TMapObjGrassManager::mSwingWidth = 50.0f;
f32 TMapObjGrassManager::mSwingSpeed = 0.04f;
f32 TMapObjGrassManager::mDistNear   = 800.0f;
f32 TMapObjGrassManager::mDistFar    = 25000.0f;
JUtility::TColor TMapObjGrassManager::mColorUpper;
JUtility::TColor TMapObjGrassManager::mColorLower;
JUtility::TColor TMapObjGrassManager::mColorFar;
S16Vec TMapObjGrassManager::mDrawVecS16;
Vec TMapObjGrassManager::mDrawVec;

static f32 sGrassAddTime = 0.2f;
static u16 color_table[] = { 0x3CC8, 0x3CFF, 0x2864, 0x28FF };

void TMapObjGrassGroup::drawNear() const
{
	if (!(unk78 == 0 ? true : false))
		return;

	GXBegin(GX_TRIANGLES, GX_VTXFMT0, unk68 * 3);
	int iVar7 = 0;
	for (int i = 0; i < unk68; ++i) {
		GXPosition3f32(unk6C[i].x - TMapObjGrassManager::mDrawVec.x,
		               mPosition.y,
		               unk6C[i].z - TMapObjGrassManager::mDrawVec.z);
		GXColor1x8(1);
		GXPosition3f32(unk6C[i].x + gpMapObjGrassManager->unk20[iVar7],
		               unk6C[i].y, unk6C[i].z);
		GXColor1x8(0);
		GXPosition3f32(unk6C[i].x + TMapObjGrassManager::mDrawVec.x,
		               mPosition.y,
		               unk6C[i].z + TMapObjGrassManager::mDrawVec.z);
		GXColor1x8(1);

		++iVar7;
		if (iVar7 == 10)
			iVar7 = 0;
	}
	GXEnd();
}

void TMapObjGrassGroup::drawFar() const
{
	if (!(unk78 == 1 ? true : false))
		return;

	GXBegin(GX_TRIANGLES, GX_VTXFMT0, unk68 * 3);
	int iVar7 = 0;
	for (int i = 0; i < unk68; ++i) {
		GXPosition3s16(unk70[i].x - TMapObjGrassManager::mDrawVecS16.x,
		               unk70[i].y,
		               unk70[i].z - TMapObjGrassManager::mDrawVecS16.z);
		GXColor1x8(1);
		GXPosition3s16(unk70[i].x + gpMapObjGrassManager->unk24[iVar7],
		               unk74[i], unk70[i].z);
		GXColor1x8(0);
		GXPosition3s16(unk70[i].x + TMapObjGrassManager::mDrawVecS16.x,
		               unk70[i].y,
		               unk70[i].z + TMapObjGrassManager::mDrawVecS16.z);
		GXColor1x8(1);

		++iVar7;
		if (iVar7 == 10)
			iVar7 = 0;
	}
	GXEnd();
}

void TMapObjGrassGroup::calc() { }

void TMapObjGrassGroup::load(JSUMemoryInputStream& stream)
{
	THitActor::load(stream);
	stream.read(&unk68, 4);
	unk6C = new JGeometry::TVec3<f32>[unk68];
	unk70 = new JGeometry::TVec3<s16>[unk68];
	unk74 = new s16[unk68];
	JGeometry::TVec3<f32> scale(mScaling.x * 100.0f, mScaling.y,
	                            mScaling.z * 100.0f);

	for (int i = 0; i < unk68; ++i) {
		unk6C[i].x
		    = scale.x * rand() / (f32)RAND_MAX * 2.0f + mPosition.x - scale.x;
		unk6C[i].y
		    = scale.y * rand() / (f32)RAND_MAX * 2.0f + mPosition.y - scale.y;
		unk6C[i].z
		    = scale.z * rand() / (f32)RAND_MAX * 2.0f + mPosition.z - scale.z;

		unk70[i].x = unk6C[i].x;
		unk70[i].y = mPosition.y;
		unk70[i].z = unk6C[i].z;

		unk74[i] = mPosition.y + unk6C[i].y;
	}

	gpMapObjGrassManager->entryGrassGroup(this, unk68);
}

TMapObjGrassGroup::TMapObjGrassGroup()
    : unk68(0)
    , unk6C(nullptr)
    , unk70(nullptr)
    , unk74(nullptr)
    , unk78(0)
{
}

void TMapObjGrassManager::initDrawNear() const
{
	Mtx viewItm;
	MTXInverse(j3dSys.getViewMtx(), viewItm);
	mDrawVec.x    = viewItm[0][0] * mWidth;
	mDrawVec.y    = viewItm[1][0] * mWidth;
	mDrawVec.z    = viewItm[2][0] * mWidth;
	mDrawVecS16.x = mDrawVec.x;
	mDrawVecS16.y = mDrawVec.y;
	mDrawVecS16.z = mDrawVec.z;
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
	GXSetCurrentMtx(0);
	GXSetArray(GX_VA_CLR0, color_table, 4);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0x28, 0xc8, 0x28, 0xff });
	GXSetNumTexGens(0);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_RASC, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_RASA, GX_CA_ZERO, GX_CA_ZERO,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetCullMode(GX_CULL_NONE);
}

void TMapObjGrassManager::initDrawFar() const
{
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_RGBA4, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
}

void TMapObjGrassManager::draw() const
{
	initDrawNear();
	for (int i = 0; i < unk10; ++i)
		unk14[i]->drawNear();

	initDrawFar();
	for (int i = 0; i < unk10; ++i)
		unk14[i]->drawFar();
}

void TMapObjGrassManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 2) {
		f32 fVar1 = 0.0f;
		for (int i = 0; i < 10; ++i) {
			unk20[i] = mSwingWidth * sinf(unk18 + fVar1);
			unk24[i] = unk20[i];
			fVar1 += sGrassAddTime;
		}
		unk18 += mSwingSpeed;
		if (unk18 > M_PI * 2)
			unk18 = 0.0f;

		for (int i = 0; i < unk10; ++i) {
			TMapObjGrassGroup* grass = unk14[i];
			if (gpMarDirector->getCurrentMap() == 2) {
				grass->unk78 = 0;
			} else {
				// TODO: some kind of a vec_distance inline?
				JGeometry::TVec3<f32> v(grass->mPosition.x - gpCamera->unk124.x,
				                        grass->mPosition.y - gpCamera->unk124.y,
				                        grass->mPosition.z
				                            - gpCamera->unk124.z);

				f32 len = v.length();

				if (len < mDistNear) {
					grass->unk78 = 0;
				} else if (len < mDistFar) {
					grass->unk78 = 1;
				} else {
					grass->unk78 = 1;
				}
			}
		}
	}

	if (param_1 & 8) {
		draw();
	}
}

void TMapObjGrassManager::entryGrassGroup(TMapObjGrassGroup* param_1,
                                          s32 param_2)
{
	unk14[unk10] = param_1;
	unk1C += param_2;
	unk10 += 1;
}

void TMapObjGrassManager::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	unk14 = new TMapObjGrassGroup*[500];
	unk20 = new f32[10];
	unk24 = new s16[10];
}

TMapObjGrassManager::TMapObjGrassManager(const char* name)
    : JDrama::TViewObj(name)
    , unk10(0)
    , unk14(0)
    , unk18(0.0)
    , unk1C(0)
    , unk20(0)
    , unk24(0)
{
	gpMapObjGrassManager = this;

	mDrawVec.x = mDrawVec.y = mDrawVec.z = 0.0f;

	mColorFar.r = 40;
	mColorFar.g = 150;
	mColorFar.b = 40;
	mColorFar.a = 255;
}
