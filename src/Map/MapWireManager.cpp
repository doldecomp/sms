#include <Map/MapWireManager.hpp>
#include <Player/Mario.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <Strategic/Strategy.hpp>
#include <Map/MapWire.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <dolphin/gx.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

f32 TMapWireActor::mCommonAttackRadius = 200.0f;
f32 TMapWireActor::mCommonAttackHeight = 200.0f;

void TMapWireActor::checkTakingActor() { }

f32 TMapWireActor::getPosInWire() const
{
	JGeometry::TVec3<f32> start;
	JGeometry::TVec3<f32> end;
	getTipPoints(&start, &end);
	start.y = 0.0f;
	end.y   = 0.0f;

	JGeometry::TVec3<f32> foot = MsPerpendicFootToLineR(start, end, mPosition);

	return JGeometry::TVec3<f32>(foot - start).length()
	       / JGeometry::TVec3<f32>(end - start).length();
}

void TMapWireActor::getTipPoints(JGeometry::TVec3<f32>* start,
                                 JGeometry::TVec3<f32>* end) const
{
	TMapWire* wire = unk74->unk7C;
	*start         = wire->mStartPoint;
	*end           = wire->mEndPoint;
}

BOOL TMapWireActor::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_UNK8 && sender == mHeldObject) {
		mHeldObject = nullptr;
		unk70       = 1;
		return true;
	}

	return false;
}

void TMapWireActor::init(TMapWireActorManager* manager)
{
	unk74 = manager;

	initHitActor(0x40000098, 1, -0x80000000, TMapWireActor::mCommonAttackRadius,
	             TMapWireActor::mCommonAttackHeight, 0.0f, 0.0f);

	TIdxGroupObj* group
	    = JDrama::TNameRefGen::search<TIdxGroupObj>("アイテムグループ");
	group->getChildren().push_back(this);
}

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
	GXLoadPosMtxImm(j3dSys.getViewMtx(), GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
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

void TMapWireActorManager::doActorToWire()
{
	TMapWire* previousWire = unk7C;
	int cubeNo             = gpCubeWire->getInCubeNo(unk0->mPosition);
	if (cubeNo != -1)
		unk7C = gpMapWireManager->getWire((u16)cubeNo);
	else
		unk7C = nullptr;

	if (unk4.mHeldObject != nullptr && unk4.mHeldObject->mHolder != &unk4) {
		unk4.mHeldObject = nullptr;
		unk4.unk70       = 1;
	}

	if (unk0->mHeldObject != nullptr) {
		for (int i = 0; i < unk4.mColCount; ++i) {
			THitActor* col = unk4.mCollisions[i];
			if (col->isActorType(0x80000001)
			    && col->receiveMessage(&unk4, HIT_MESSAGE_TAKE))
				unk4.mHeldObject = (TTakeActor*)unk4.mCollisions[i];
		}
	}

	if (previousWire != nullptr) {
		if (unk7C != nullptr && unk7C != previousWire)
			unk4.unk70 = 1;
	}

	if (unk4.unk70) {
		if (previousWire != nullptr)
			previousWire->release();
		unk4.unk70 = 0;
		return;
	}

	if (unk4.mHeldObject != nullptr) {
		if (unk7C != nullptr)
			unk7C->setFootPointsAtHanged(gpMarioOriginal->getTakenMtx());
		if (previousWire != nullptr) {
			previousWire->setFootPointsAtHanged(gpMarioOriginal->getTakenMtx());
			unk7C = previousWire;
		}
	}

	if (unk7C == nullptr && previousWire != nullptr)
		previousWire->release();
}

void TMapWireActorManager::doWireToActor() { }

TMapWireActorManager::TMapWireActorManager(TTakeActor* param_1)
    : unk0(param_1)
    , unk4("アクター補助")
    , unk7C(0)
{
	unk4.init(this);
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
	getWire(param_1)->getPointPosOnWire(getWire(param_1)->getPosInWire(param_2),
	                                    param_3);
}

void TMapWireManager::getPointPosInWire(const JGeometry::TVec3<f32>&,
                                        JGeometry::TVec3<f32>*) const
{
}

void TMapWireManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		for (int i = 0; i < unk1C; ++i)
			unk24[i]->doActorToWire();

		for (int i = 0; i < unk10; ++i)
			unk18[i]->move();

		TMapWireActorManager* mgr;
		for (int i = 0; i < unk1C; ++i) {
			mgr = unk24[i];
			mgr->unk4.onHitFlag(HIT_FLAG_NO_COLLISION);
			if (mgr->unk7C != nullptr) {
				MtxPtr mtx = gpMarioOriginal->getTakenMtx();
				mgr->unk4.offHitFlag(HIT_FLAG_NO_COLLISION);
				mgr->unk4.mPosition.set(mtx[0][3], mtx[1][3], mtx[2][3]);
			}
		}
	}
	if (cue & CUE_DRAW) {
		initDraw();
		GXSetChanMatColor(GX_COLOR0A0, mUpperSurface);
		for (int i = 0; i < unk10; ++i)
			unk18[i]->drawUpper();
		GXSetChanMatColor(GX_COLOR0A0, mLowerSurface);
		for (int i = 0; i < unk10; ++i)
			unk18[i]->drawLower();
	}
	if (cue & CUE_ENTRY) {
		for (int i = 0; i < unk10; ++i)
			unk18[i]->calcViewAndDBEntry();
	}
}

void TMapWireManager::entry(TTakeActor* actor)
{
	unk24[unk1C] = new TMapWireActorManager(actor);
	++unk1C;
}

void TMapWireManager::loadAfter()
{
	JDrama::TViewObj::loadAfter();

	entry(gpMarioOriginal);
}

void TMapWireManager::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	stream.readString();
	stream >> unk14;
	stream >> unk20;
	stream >> TMapWire::mDrawWidth;
	stream >> TMapWire::mDrawHeight;

	s32 val;
	stream >> val;
	mUpperSurface.r = val;
	stream >> val;
	mUpperSurface.g = val;
	stream >> val;
	mUpperSurface.b = val;

	stream >> val;
	mLowerSurface.r = val;
	stream >> val;
	mLowerSurface.g = val;
	stream >> val;
	mLowerSurface.b = val;

	unk18 = new TMapWire*[unk14];
	unk24 = new TMapWireActorManager*[unk20];

	unk10 = gpCubeWire->unk10;

	for (int i = 0; i < unk10; ++i) {
		unk18[i] = new TMapWire;
		unk18[i]->init(&gpCubeWire->unk14->getChildren()[i]);
	}
}

TMapWireManager::TMapWireManager(const char* name)
    : JDrama::TViewObj(name)
    , unk10(0)
    , unk18(nullptr)
    , unk1C(0)
    , unk24(nullptr)
    , unk28(0)
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
