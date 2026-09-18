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

// The `unk74->unk7C` guard is read off doActorToWire's inlined copy
// (`lwz r3, 0x78(r30)` is unk4.unk74, not the manager's own unk7C), which is
// what proves this block belongs to TMapWireActor rather than the manager.
// TODO: doActorToWire is frame-only (0x30 vs 0x58, all 121 instructions
// identical, target's 0xc..0x48 locals region wholly unreferenced), and it
// needs exactly 40 more dead bytes, which a dead local of this UNUSED callee
// would supply. Measured accounting for dead non-trivial locals, both here and
// in doActorToWire's own body: the region grows by floor(total_size / 8) * 8,
// so k dead `TVec3`s give +8/+24/+32/+48 for k = 1..4 -- 3 TVec3 (36 B) is 32,
// 4 (48 B) is 48, and no TVec3-only spelling reaches 40. A total size of 40-47
// does: `TQuat4<f32> + 2 TVec3` was verified exact (frame 0x58, 100%), as is 3
// TVec3 here plus 1 in doActorToWire. All are implausible content for a
// collision scan, so the lever is left unpulled rather than fabricated; the
// body above is what the matching expansion proves, and the map's UNUSED 0xe8
// stays 40 bytes above it.
void TMapWireActor::checkTakingActor()
{
	if (unk74->unk7C != nullptr) {
		for (int i = 0; i < mColCount; ++i) {
			THitActor* col = mCollisions[i];
			if (col->isActorType(0x80000001)
			    && col->receiveMessage(this, HIT_MESSAGE_TAKE))
				mHeldObject = (TTakeActor*)mCollisions[i];
		}
	}
}

// `foot` is declared first because retail's named block descends
// foot (0xa4), start (0x98), end (0x8c), MsPerpendicFootToLineR's return
// buffer (0x80), and named locals are allocated top-down in declaration order;
// `len` is named because f31 is saved across the second sqrt call, and only a
// named scalar local of the function's own body gets a callee-saved FPR.
// TODO: frame 0xb8 vs 0xa8, 99.3%, all 106 instructions present. Retail's low
// region is 0xc..0x4c dead (64 B) then temps 0x4c, [4-byte hole at 0x58],
// 0x5c, 0x68, 0x74; ours is 0xc..0x3c dead (48 B) then four contiguous 12-byte
// temps, so the deficit is 16 dead bytes below the temps plus one dead 4-byte
// temp after the first subtraction. Measured: `getTipPoints` works as a carrier
// for those 16 bytes even though it has an out-of-line copy, because its leaf
// copy allocates no frame for a dead local and keeps matching -- a 16-byte dead
// local there, or one TU-local binding accessor `const TVec3& getStartPoint()`
// (+16), makes the frame exactly 0xb8 and leaves only the 4-byte hole (99.6%,
// 40 operand-only marks). Both spellings are fabricated, so neither is kept.
// The 4 bytes are a temp allocated between the two `operator-` expansions:
// splitting `len` off into its own statement reserves one, but then the temps
// permute, because MWCC allocates inline temps statement by statement in
// reverse statement order and within one statement by level (both subtractions,
// then both TVec3 copies) -- which is why the quotient has to stay one
// expression. not: `getPosition()` over `mPosition` (99.3 -> 96.5), two named
// scalars for the two lengths (frame 0xc0), binding accessors on both tip
// points (frame 0xc8).
f32 TMapWireActor::getPosInWire() const
{
	JGeometry::TVec3<f32> foot;
	JGeometry::TVec3<f32> start;
	JGeometry::TVec3<f32> end;
	getTipPoints(&start, &end);
	start.y = 0.0f;
	end.y   = 0.0f;

	foot = MsPerpendicFootToLineR(start, end, mPosition);

	f32 len = JGeometry::TVec3<f32>(end - start).length();
	return JGeometry::TVec3<f32>(foot - start).length() / len;
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

// The pragma is load-bearing and costs one map symbol on purpose. Removing it
// lets MWCC expand this constructor at its two sites, which emits the map's
// UNUSED `__ct__10TTakeActorFPCc` (0x50) and brings
// `__ct__20TMapWireActorManagerFP10TTakeActor` to its map size of 0x154 -- but
// `TMapWireManager::loadAfter` then falls from exact to 81.3% and the UNUSED
// `entry` grows from 0x140 to 0x160 against the map's 0x144. One exact
// function is worth more than two UNUSED sizes, so the pragma stays and
// `__ct__10TTakeActorFPCc` stays MISSING in validate-symbol-order. A real fix
// needs the inline decision per call site, which `dont_inline` cannot express.
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

	unk4.checkTakingActor();

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

// TODO: UNUSED at 0x6c in the map and never inlined anywhere in the TU, so
// there is no call site to read a body off. Left empty rather than fabricated.
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
	f32 posInWire = getWire(param_1)->getPosInWire(param_2);
	getWire(param_1)->getPointPosOnWire(posInWire, param_3);
}

void TMapWireManager::getPointPosInWire(const JGeometry::TVec3<f32>& param_1,
                                        JGeometry::TVec3<f32>* param_2) const
{
	int wireNo = getWireNo(param_1);
	if (wireNo != -1)
		getPointPosInNthWire(wireNo, param_1, param_2);
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

// TODO: frame 0x50 vs 0x40. Uniform +0x10 on the saved registers and +8 on the
// single `stream >>` temporary at 0x34/0x2c, so both regions are short.
void TMapWireManager::load(JSUMemoryInputStream& stream)
{
	// A C-style declaration block with two dead 4-byte scalars, the shape
	// TAreaCylinder::load also has: they are the top 8 bytes of the 0x50
	// frame and nothing reads them, so the counts below are read straight
	// into the members. The chained first read is the pool's last 8 bytes
	// (one `>>` continuation = 8 low bytes).
	s32 wireNum;
	s32 actorNum;
	s32 val;
	JDrama::TViewObj::load(stream);
	stream.readString();
	stream >> unk14 >> unk20;
	stream >> TMapWire::mDrawWidth;
	stream >> TMapWire::mDrawHeight;

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
		unk18[i]->init(gpCubeWire->unk14->getChildren()[i]);
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
