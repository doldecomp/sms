#include <Map/MapWireManager.hpp>
#include <Player/MarioMain.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <Map/MapWire.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <dolphin/gx.h>

void TMapWireActor::checkTakingActor() { }

void TMapWireActor::getPosInWire() const { }

void TMapWireActor::getTipPoints(JGeometry::TVec3<f32>*,
                                 JGeometry::TVec3<f32>*) const
{
}

u32 TMapWireActor::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 == 8 && param_1 == unk6C) {
		unk6C = nullptr;
		unk70 = 1;
		return true;
	}

	return false;
}

void TMapWireActor::init(TMapWireActorManager*) { }

#pragma dont_inline on
TMapWireActor::TMapWireActor(const char* name)
    : TTakeActor(name)
    , unk70(0)
    , unk74(nullptr)
{
}
#pragma dont_inline off

static void initDraw()
{
	GXSetColorUpdate(GX_TRUE);
	GXLoadPosMtxImm(j3dSys.getViewMtx(), 0);
	GXSetCurrentMtx(0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetNumChans(1);
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0, 0, 0, 0xff });
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetLineWidth(24, GX_TO_ZERO);
	GXSetCullMode(GX_CULL_BACK);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0xff);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
}

void TMapWireActorManager::doActorToWire() { }

void TMapWireActorManager::doWireToActor() { }

TMapWireActorManager::TMapWireActorManager(TTakeActor* param_1)
    : unk0(param_1)
    , unk4("アクター補助")
    , unk7C(0)
{
	unk4.initHitActor(0x40000098, 1, -0x80000000,
	                  TMapWireActor::mCommonAttackRadius,
	                  TMapWireActor::mCommonAttackHeight, 0.0f, 0.0f);

	// TODO: inlines are messed up =(
	JDrama::TViewObjPtrListT<THitActor>* group
	    = JDrama::TNameRefGen::search<JDrama::TViewObjPtrListT<THitActor> >(
	        "アイテムグループ");
	group->getChildren().push_back(&unk4);
}

JUtility::TColor TMapWireManager::mUpperSurface;
JUtility::TColor TMapWireManager::mLowerSurface;

u32 TMapWireManager::getWireNo(const JGeometry::TVec3<f32>& param_1) const
{
	return gpCubeWire->getInCubeNo(param_1);
}

void TMapWireManager::getPointPosInNthWire(int param_1,
                                           const JGeometry::TVec3<f32>& param_2,
                                           JGeometry::TVec3<f32>* param_3) const
{
	unk18[param_1]->getPointPosOnWire(unk18[param_1]->getPosInWire(param_2),
	                                  param_3);
}

void TMapWireManager::getPointPosInWire(const JGeometry::TVec3<f32>&,
                                        JGeometry::TVec3<f32>*) const
{
}

void TMapWireManager::perform(u32, JDrama::TGraphics*) { }

void TMapWireManager::entry(TTakeActor*) { }

void TMapWireManager::loadAfter()
{
	JDrama::TViewObj::loadAfter();

	TTakeActor* mario = gpMarioOriginal;
	unk24[unk1C]      = new TMapWireActorManager(mario);
	++unk1C;
}

void TMapWireManager::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	stream.readString();
	stream.read(&unk14, 4);
	stream.read(&unk20, 4);
	stream.read(&TMapWire::mDrawWidth, 4);
	stream.read(&TMapWire::mDrawHeight, 4);
	int val;
	stream.read(&val, 4);
	mUpperSurface.r = val;
	stream.read(&val, 4);
	mUpperSurface.g = val;
	stream.read(&val, 4);
	mUpperSurface.b = val;

	stream.read(&val, 4);
	mLowerSurface.r = val;
	stream.read(&val, 4);
	mLowerSurface.g = val;
	stream.read(&val, 4);
	mLowerSurface.b = val;

	unk18 = new TMapWire*[unk14];
	unk24 = new TMapWireActorManager*[unk20];

	unk10 = gpCubeWire->unk10;

	for (int i = 0; i < unk10; ++i) {
		unk18[i] = new TMapWire;
		unk18[i]->init(&(*gpCubeWire->unk14)[i]);
	}
}

TMapWireManager::TMapWireManager(const char* name)
    : JDrama::TViewObj(name)
{
	gpMapWireManager = this;
	mUpperSurface.r  = 0x78;
	mUpperSurface.g  = 0x78;
	mUpperSurface.b  = 0x78;
	mUpperSurface.a  = 0xff;
	mLowerSurface.r  = 0x32;
	mLowerSurface.g  = 0x32;
	mLowerSurface.b  = 0x32;
	mLowerSurface.a  = 0xff;
}
