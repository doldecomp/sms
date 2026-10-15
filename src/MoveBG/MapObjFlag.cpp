#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JSupport/JSUInputStream.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MoveBG/MapObjFlag.hpp>
#include <System/MarDirector.hpp>
#include <dolphin/gx.h>
#include <macros.h>
#include <stdio.h>
#include <string.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <System/DummyStrings.hpp>

TMapObjFlagManager* gpMapObjFlagManager;

f32 TMapObjFlag::mFlutterSpeed = 4.0f;

void TMapObjFlagSail::updateVertex() { }

void TMapObjFlagLower::updateVertex() { }

void TMapObjFlag::draw()
{
	TMtx34f mtx;
	mtx.set(j3dSys.getViewMtx());
	MTXConcat(mtx, unk8C, mtx);
	GXLoadPosMtxImm(mtx, GX_PNMTX0);
	u16 count = 2 * ((unk70 - 2 * unkBC) / unkBC + 2);
	f32 sStep = 1.0f / (unk70 - 1);
	f32 tStep = 1.0f / (unk74 - 1);
	for (s32 i = 0; i < unk74 - unkBC; i += unkBC) {
		f32 t0 = tStep * (unk74 - 1 - i);
		f32 t1 = tStep * (unk74 - 1 - (i + 1));
		GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, count);
		GXPosition3f32(unk78[i][0].x, unk78[i][0].y, unk78[i][0].z);
		GXTexCoord2f32(0.0f, t0);
		GXPosition3f32(unk78[i + 1][0].x, unk78[i + 1][0].y, unk78[i + 1][0].z);
		GXTexCoord2f32(0.0f, t1);
		for (s32 j = 1; j < unk70 - unkBC; j += unkBC) {
			GXPosition3f32(unk78[i][j].x, unk78[i][j].y, unk78[i][j].z);
			f32 s = sStep * j;
			GXTexCoord2f32(s, t0);
			GXPosition3f32(unk78[i + 1][j].x, unk78[i + 1][j].y,
			               unk78[i + 1][j].z);
			GXTexCoord2f32(s, t1);
		}
		s32 j = unk70 - 1;
		GXPosition3f32(unk78[i][j].x, unk78[i][j].y, unk78[i][j].z);
		GXTexCoord2f32(1.0f, t0);
		GXPosition3f32(unk78[i + 1][j].x, unk78[i + 1][j].y, unk78[i + 1][j].z);
		GXTexCoord2f32(1.0f, t1);
		GXEnd();
	}
}

void TMapObjFlag::updateVertex()
{
	for (s32 i = 0; i < unk74; i += unkBC) {
		f32 rowAngle = i * unk80;
		for (s32 j = 0; j < unk70; j += unkBC) {
			f32 ratio     = (f32)j / unk70;
			f32 angle     = unk88 + (-j * unk7C + rowAngle);
			angle         = MsWrap(angle, -180.0f, 180.0f);
			unk78[i][j].x = unk84 * ratio * MsSin(angle);
		}
	}
}

void TMapObjFlag::update()
{
	MsMtxSetXYZRPH(unk8C, mPosition.x, mPosition.y, mPosition.z, mRotation.x,
	               mRotation.y, mRotation.z);
	updateVertex();
	unk88 += mFlutterSpeed;
	if (unk88 > 360.0f)
		unk88 -= 360.0f;
	if (mScaling.y > 3.0f && mScaling.z > 3.0f)
		SMSGetMSound()->startSoundActor(MSD_SE_OBJ_FLAG, &mPosition, 0, nullptr,
		                                0, 4);
}

void TMapObjFlag::init(const char* param_1)
{
	unk68 = 100.0f * mScaling.z;
	unk6C = 100.0f * mScaling.y;
	unk7C /= mScaling.z;
	unk80 /= mScaling.y;
	unk84 *= mScaling.z;
	unk70 = (s32)(unk68 / 50.0f);
	unk74 = (s32)(unk6C / 100.0f);
	if (unk70 < 2)
		unk70 = 3;
	if (unk74 < 2)
		unk74 = 3;

	MsMtxSetXYZRPH(unk8C, mPosition.x, mPosition.y, mPosition.z, mRotation.x,
	               mRotation.y, mRotation.z);

	f32 colWidth  = unk68 / unk70;
	f32 rowHeight = unk6C / unk74;
	JKRGetCurrentHeap()->getTotalFreeSize();
	unk78 = new JGeometry::TVec3<f32>*[unk74];
	for (s32 i = 0; i < unk74; ++i) {
		unk78[i] = new JGeometry::TVec3<f32>[unk70];
		for (s32 j = 0; j < unk70; ++j)
			unk78[i][j].set(0.0f, i * rowHeight, j * colWidth);
	}

	static int total_use_size = 0;
	JKRGetCurrentHeap()->getTotalFreeSize();
	gpMapObjFlagManager->registerObj(this, param_1);
	initHitActor(ACTOR_TYPE_MAP_OBJ_FLAG, 1, 0, 0.0f, 0.0f, 0.0f, 0.0f);
}

void TMapObjFlag::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);

	char buf[0x40];
	stream.readString(buf, 0x40);
	init(buf);
}

TMapObjFlag::TMapObjFlag(const char* param_1)
    : THitActor(param_1)
    , unk68(0.0f)
    , unk6C(0.0f)
    , unk70(0)
    , unk74(0)
    , unk78(nullptr)
    , unk7C(125.0f)
    , unk80(130.0f)
    , unk84(20.0f)
    , unk88(360.0f * MsRandF())
    , unkBC(1)
{
	unk8C.identity();
}

void TMapObjFlagManager::initDraw()
{
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetNumChans(0);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, 0xff });
	GXSetNumTexGens(1);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
	                  GX_FALSE, GX_PTIDENTITY);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
	GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_GREATER, 0);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetZCompLoc(GX_FALSE);
	GXSetCullMode(GX_CULL_NONE);
}

void TMapObjFlagManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & CUE_CALC_ANIM) {
		for (s32 i = 0; i < ARRAY_COUNT(unk10); ++i) {
			for (s32 j = 0; j < unk10[i].unk0; ++j)
				unk10[i].unk4[j]->update();
		}
	}
	if (param_1 & CUE_DRAW) {
		initDraw();
		for (s32 i = 0; i < ARRAY_COUNT(unk10); ++i) {
			if (unk10[i].unk0 != 0) {
				JUTTexture texture(unk10[i].unk54);
				texture.load(GX_TEXMAP0);
				for (s32 j = 0; j < unk10[i].unk0; ++j)
					unk10[i].unk4[j]->draw();
			}
		}
	}
}

void TMapObjFlagManager::loadFlag(TMapObjFlagManager::TMapObjFlagInfo* param_1,
                                  TMapObjFlag* param_2, const char* param_3)
{
	char buffer[0x40];
	if (param_1->unk54 == nullptr) {
		snprintf(buffer, 0x40, "/scene/mapObj/%s.bti", param_3);
		param_1->unk54 = (ResTIMG*)JKRGetResource(buffer);
	}
	param_1->unk4[param_1->unk0] = param_2;
	param_1->unk0 += 1;
}

void TMapObjFlagManager::registerObj(TMapObjFlag* param_1, const char* param_2)
{
	if (strcmp(param_2, "flagSun") == 0) {
		loadFlag(&unk10[0], param_1, param_2);
		return;
	}
	if (strcmp(param_2, "flagWhite") == 0) {
		loadFlag(&unk10[1], param_1, param_2);
		return;
	}
	if (strcmp(param_2, "flagRedsun") == 0) {
		loadFlag(&unk10[2], param_1, param_2);
		return;
	}
	if (strcmp(param_2, "flagMonte") == 0) {
		loadFlag(&unk10[3], param_1, param_2);
		return;
	}
	if (strcmp(param_2, "flagBird") == 0) {
		loadFlag(&unk10[4], param_1, param_2);
		return;
	}
	if (strcmp(param_2, "flagHigekuri") == 0) {
		loadFlag(&unk10[5], param_1, param_2);
		return;
	}
	if (strcmp(param_2, "flagBenvenuto") == 0) {
		loadFlag(&unk10[6], param_1, param_2);
		return;
	}
	if (strcmp(param_2, "flagDolpicDolphin") == 0) {
		loadFlag(&unk10[7], param_1, param_2);
		return;
	}
	if (strcmp(param_2, "flagDolSun") == 0) {
		loadFlag(&unk10[8], param_1, param_2);
		return;
	}
	if (strcmp(param_2, "flagDolSunWelcome") == 0) {
		loadFlag(&unk10[9], param_1, param_2);
		return;
	}
	if (strcmp(param_2, "flagBianco") == 0) {
		loadFlag(&unk10[10], param_1, param_2);
		return;
	}
	if (strcmp(param_2, "flagRiccoBuoy") == 0) {
		loadFlag(&unk10[11], param_1, param_2);
		return;
	}
	if (strcmp(param_2, "flagSailMonte") == 0) {
		loadFlag(&unk10[12], param_1, param_2);
		return;
	}
	if (strcmp(param_2, "MammaYacht00") == 0) {
		loadFlag(&unk10[13], param_1, param_2);
		return;
	}
	if (strcmp(param_2, "flagMare") == 0)
		loadFlag(&unk10[14], param_1, param_2);
}

void TMapObjFlagManager::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);

	char buf[8];
	stream.readString(buf, 8);

	switch (SMSGetMarDirector()->getCurrentMap()) {
	case 0:
		TMapObjFlag::mFlutterSpeed = 16.0f;
		break;
	case 2:
		TMapObjFlag::mFlutterSpeed = 16.0f;
		break;
	case 4:
		TMapObjFlag::mFlutterSpeed = 12.0f;
		break;
	default:
		TMapObjFlag::mFlutterSpeed = 8.0f;
		break;
	}
}

TMapObjFlagManager::TMapObjFlagManager(const char* param_1)
    : JDrama::TViewObj(param_1)
{
	gpMapObjFlagManager = this;
}
