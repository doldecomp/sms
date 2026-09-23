#include <MoveBG/MapObjMonte.hpp>
#include <System/DummyStrings.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <Map/MapCollisionManager.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <M3DUtil/MActor.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/WaterGun.hpp>
#include <Player/Yoshi.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <dolphin/gx.h>
#include <math.h>
#include <stdlib.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TMapObjMonteRoot::initMapObj()
{
	TMapObjBase::initMapObj();
	mDamageHeight = 1400.0f * getScaling().y;
	calcEntryRadius();
	mPosition.y = getInitialPosition().y + getObjCollisionHeightOffset();
}

BOOL TJumpMushroom::receiveMessage(THitActor* sender, u32 message)
{
	startAnim(1);
	return TRUE;
}

// Binding level over a raw member read, worth +8 of low region in
// TJumpMushroom::load (batch 127).
static inline TMapCollisionManager* MapObjMonteMapCollisionManager(const TJumpMushroom* p)
{
	TMapCollisionManager* mapCollisionManager = p->mMapCollisionManager;
	return mapCollisionManager;
}

void TJumpMushroom::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);

	s32 data;
	stream >> data;
	if (mMapCollisionManager)
		MapObjMonteMapCollisionManager(this)->unk8->setAllData((s16)data);
}

f32 THangingBridgeBoard::mMarioAccelY        = 0.15f;
f32 THangingBridgeBoard::mMarioHipDropAccelY = 2.0f;
f32 THangingBridgeBoard::mReturnAccelRate    = 0.005f;
f32 THangingBridgeBoard::mSpeedDownRate      = 0.98f;
f32 THangingBridgeBoard::mRopeWidthX         = 10.0f;
f32 THangingBridgeBoard::mRopeWidthZ         = 7.0f;
f32 THangingBridgeBoard::mTexPosRate         = 0.01f;

static inline TMarDirector* MapObjMonteMarDirector()
{
	TMarDirector* director = SMSGetMarDirector();
	return director;
}

// TODO: 99.3%. Frame is retail's 0x78 via MapObjMonteMarDirector (was
// 0x10 short). Leftover is volatile colouring of top.y (f4 vs f2).
// A by-value top.y fork is +0x10 and swaps the hookY fadds operands.
void THangingBridgeBoard::drawOneRope(const JGeometry::TVec3<f32>& top) const
{
	f32 hookY  = top.y + THangingBridge::mRopeHeight;
	f32 bottom = top.y;

	f32 xPlus  = top.x + mRopeWidthX;
	f32 xMinus = top.x - mRopeWidthX;
	f32 zPlus  = top.z + mRopeWidthZ;
	f32 zMinus = top.z - mRopeWidthZ;

	if (MapObjMonteMarDirector()->mMap == 0xD)
		bottom -= 60.0f;

	f32 texTop    = mTexPosRate * (hookY - top.y);
	f32 texBottom = mTexPosRate * (bottom - top.y);

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 8);
	GXPosition3f32(top.x, hookY, zPlus);
	GXTexCoord2f32(0.0f, texTop);
	GXPosition3f32(top.x, bottom, zPlus);
	GXTexCoord2f32(0.0f, texBottom);

	GXPosition3f32(xMinus, hookY, zMinus);
	GXTexCoord2f32(1.0f, texTop);
	GXPosition3f32(xMinus, bottom, zMinus);
	GXTexCoord2f32(1.0f, texBottom);

	GXPosition3f32(xPlus, hookY, zMinus);
	GXTexCoord2f32(2.0f, texTop);
	GXPosition3f32(xPlus, bottom, zMinus);
	GXTexCoord2f32(2.0f, texBottom);

	GXPosition3f32(top.x, hookY, zPlus);
	GXTexCoord2f32(3.0f, texTop);
	GXPosition3f32(top.x, bottom, zPlus);
	GXTexCoord2f32(3.0f, texBottom);
	GXEnd();
}

// UNUSED (0x6c): the pair of rope draws THangingBridge::perform expands.
void THangingBridgeBoard::drawRopes() const
{
	drawOneRope(mRopeTop[0]);
	drawOneRope(mRopeTop[1]);
}

// UNUSED (0x10).
// The bridge pointer is reloaded at every site in retail, so this is a pure
// frame rung: the binder is +8 per expansion inside pushNeighbor and +12 at a
// control-tail site. Which four of the six tail sites carry it is unobservable.
static inline THangingBridge* HangingBridgeBoardBridge(const THangingBridgeBoard* board)
{
	THangingBridge* bridge = board->mBridge;
	return bridge;
}

// fp_contract folds `y -= accel * rate` into one fnmsubs; retail keeps the
// fmuls and the fsubs apart, so the product is named out of the statement.
static inline f32 HangingBridgeBoardPush(f32 accel, f32 rate)
{
	return accel * rate;
}

void THangingBridgeBoard::push(f32 accel) { mVelocity.y -= accel; }

// UNUSED (0x94): the neighbour chain control expands twice.
void THangingBridgeBoard::pushNeighbor(f32 accel)
{
	if (mPrev) {
		mPrev->mVelocity.y -= HangingBridgeBoardPush(accel, HangingBridgeBoardBridge(this)->mNeighborRate);
		if (mPrev2)
			mPrev2->mVelocity.y -= HangingBridgeBoardPush(accel, HangingBridgeBoardBridge(this)->mNeighbor2Rate);
	}

	if (mNext) {
		mNext->mVelocity.y -= HangingBridgeBoardPush(accel, HangingBridgeBoardBridge(this)->mNeighborRate);
		if (mNext2)
			mNext2->mVelocity.y -= HangingBridgeBoardPush(accel, HangingBridgeBoardBridge(this)->mNeighbor2Rate);
	}
}

void THangingBridgeBoard::control()
{
	TLeanBlock::control();

	if (marioIsOn()) {
		push(mMarioAccelY);
		pushNeighbor(mMarioAccelY);
	}

	if (marioHipAttack()) {
		push(mMarioHipDropAccelY);
		pushNeighbor(mMarioHipDropAccelY);
	}

	mPosition.y += mVelocity.y;
	mVelocity.y += mReturnAccelRate * (mInitialPosition.y - mPosition.y);
	mVelocity.y *= mSpeedDownRate;

	MtxPtr mtx      = getModel()->getAnmMtx(0);
	mRopeTop[0].x   = mPosition.x - mtx[0][0] * mBridge->mRopeOffset;
	mRopeTop[0].y   = 70.0f + (mPosition.y - mtx[1][0] * mBridge->mRopeOffset);
	mRopeTop[0].z   = mPosition.z - mtx[2][0] * HangingBridgeBoardBridge(this)->mRopeOffset;
	mRopeTop[1].x   = mtx[0][0] * HangingBridgeBoardBridge(this)->mRopeOffset + mPosition.x;
	mRopeTop[1].y   = 70.0f + (mtx[1][0] * HangingBridgeBoardBridge(this)->mRopeOffset + mPosition.y);
	mRopeTop[1].z   = mtx[2][0] * HangingBridgeBoardBridge(this)->mRopeOffset + mPosition.z;
}

void THangingBridgeBoard::calcDefaultMtx()
{
	Mtx rotX;
	makeRootMtxRotX(rotX);
	Mtx rotY;
	makeRootMtxRotY(rotY);
	MTXConcat(rotY, rotX, rotY);
	mDefaultMtx.set(rotY);

	mVelocity.y = 0.0f;
	mPosition.y = mInitialPosition.y;
}

void THangingBridgeBoard::setGroundCollision()
{
	if (SMS_GetYoshi()->isHatched()
	    && mPosition.x - mBodyRadius < SMS_GetYoshi()->getTranslation().x
	    && mPosition.x + mBodyRadius > SMS_GetYoshi()->getTranslation().x
	    && mPosition.z - mBodyRadius < SMS_GetYoshi()->getTranslation().z
	    && mPosition.z + mBodyRadius > SMS_GetYoshi()->getTranslation().z) {
		MtxPtr mtx = getModel()->getAnmMtx(0);
		if (mMapCollisionManager->unk8)
			mMapCollisionManager->unk8->moveMtx(mtx);
	} else {
		TMapObjBase::setGroundCollision();
	}
}

void THangingBridgeBoard::initMapObj()
{
	TLeanBlock::initMapObj();
	unk140 = 0.01f;
	unk144 = 0.02f;
	unk148 = 0.08f;
}

THangingBridgeBoard::THangingBridgeBoard(const char* name)
    : TLeanBlock(name)
{
	mBridge = nullptr;
	mPrev   = nullptr;
	mNext   = nullptr;
	mPrev2  = nullptr;
	mNext2  = nullptr;
	mRopeTop[0].zero();
	mRopeTop[1].zero();
}

f32 THangingBridge::mRopeWidthBetweenBoards  = 10.0f;
f32 THangingBridge::mRopeWidthBetweenBoardsY = 10.0f;
int THangingBridge::mPointNumBetweenBoards   = 10;
f32 THangingBridge::mBetweenBoardsTexPosRate = 0.01f;
f32 THangingBridge::mRopeHeight;

void THangingBridge::drawLowerMinus(const JGeometry::TVec3<f32>& from,
                                    const JGeometry::TVec3<f32>& to,
                                    const JGeometry::TVec2<f32>& width,
                                    int divide) const
{
	f32 x        = from.x;
	f32 y        = from.y;
	f32 z        = from.z;
	f32 step     = 1.0f / divide;
	f32 dx       = step * (to.x - from.x);
	f32 dy       = step * (to.y - from.y);
	f32 dz       = step * (to.z - from.z);

	for (int i = 0; i < divide; i++) {
		f32 sag = y - mRopeSag[i];
		f32 tex = mBetweenBoardsTexPosRate * (x + z);
		GXPosition3f32(x - width.x, sag, z - width.y);
		GXTexCoord2f32(0.0f, tex);
		GXPosition3f32(x, sag - mRopeWidthBetweenBoardsY, z);
		GXTexCoord2f32(1.0f, tex);
		x += dx;
		y += dy;
		z += dz;
	}
}

void THangingBridge::drawLowerPlus(const JGeometry::TVec3<f32>& from,
                                   const JGeometry::TVec3<f32>& to,
                                   const JGeometry::TVec2<f32>& width,
                                   int divide) const
{
	f32 x        = from.x;
	f32 y        = from.y;
	f32 z        = from.z;
	f32 step     = 1.0f / divide;
	f32 dx       = step * (to.x - from.x);
	f32 dy       = step * (to.y - from.y);
	f32 dz       = step * (to.z - from.z);

	for (int i = 0; i < divide; i++) {
		f32 sag = y - mRopeSag[i];
		f32 tex = mBetweenBoardsTexPosRate * (x + z);
		GXPosition3f32(x, sag - mRopeWidthBetweenBoardsY, z);
		GXTexCoord2f32(0.0f, tex);
		GXPosition3f32(x + width.x, sag, z + width.y);
		GXTexCoord2f32(1.0f, tex);
		x += dx;
		y += dy;
		z += dz;
	}
}

void THangingBridge::drawUpper(const JGeometry::TVec3<f32>& from,
                               const JGeometry::TVec3<f32>& to,
                               const JGeometry::TVec2<f32>& width,
                               int divide) const
{
	f32 x       = from.x;
	f32 y       = from.y;
	f32 z       = from.z;
	f32 step    = 1.0f / divide;
	f32 dx      = step * (to.x - from.x);
	f32 dy      = step * (to.y - from.y);
	f32 dz      = step * (to.z - from.z);

	for (int i = 0; i < divide; i++) {
		f32 sag = y - mRopeSag[i];
		f32 tex = mBetweenBoardsTexPosRate * (x + z);
		GXPosition3f32(x + width.x, sag, z + width.y);
		GXTexCoord2f32(0.0f, tex);
		GXPosition3f32(x - width.x, sag, z - width.y);
		GXTexCoord2f32(1.0f, tex);
		x += dx;
		y += dy;
		z += dz;
	}
}

static inline int HangingBridgeBoardNum(const THangingBridge* bridge)
{
	return bridge->mBoardNum;
}

static inline THangingBridgeBoard* HangingBridgeBoardAt(const THangingBridge* bridge,
                                                       int i)
{
	THangingBridgeBoard* board = bridge->mBoards[i];
	return board;
}

// Nested fork inside the binder is the +4 pool rung perform needs;
// drawRopeBetweenBoards keeps the flat binder (sites are not additive).
static inline THangingBridgeBoard** HangingBridgeBoards(const THangingBridge* bridge)
{
	return bridge->mBoards;
}

static inline THangingBridgeBoard* HangingBridgeBoardAtPerform(const THangingBridge* bridge,
                                                              int i)
{
	THangingBridgeBoard* board = HangingBridgeBoards(bridge)[i];
	return board;
}

// UNUSED (0x10): the board-position fetch drawRopeBetweenBoards expands.
void THangingBridge::setDrawPos(int i, f32 yOffset,
                                JGeometry::TVec3<f32>* out) const
{
	out->y += yOffset;
}

// TODO: every local slot matches; retail's frame is 0x108 against our 0x100,
// 8 bytes of dead region above the class-object block that no pool or binder
// rung reaches (two-local binder subsets all land 0x108 but 8 bytes too low).
// The board-number fork is also what swaps `mullw r0, r0, r30`'s operands.
// Priced: raw mBoardNum is 0xf8 (slots shift), the fork 0x100, a name-and-
// return binder 0x108 with shifted slots; fork in any loop condition, `<< 1`,
// `divide *` first, `* 2 * divide`, and inlining setDrawPos are inert/worse.
void THangingBridge::drawRopeBetweenBoards(f32 yOffset, int divide) const
{
	f32 offsetX = mSideDir.x * mRopeOffset;
	f32 offsetZ = mSideDir.y;
	offsetZ *= mRopeOffset;

	JGeometry::TVec2<f32> width(mSideDir);
	width.scale(mRopeWidthBetweenBoards);

	u16 vertexNum = (HangingBridgeBoardNum(this) + 2) * divide * 2;

	JGeometry::TVec3<f32> from;
	JGeometry::TVec3<f32> to;

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, vertexNum);
	from.set(mStart.x + offsetX, mStart.y + yOffset, mStart.z + offsetZ);
	for (int i = 0; i < mBoardNum; i++) {
		to = HangingBridgeBoardAt(this, i)->mRopeTop[0];
		setDrawPos(i, yOffset, &to);
		drawLowerMinus(from, to, width, divide);
		from = to;
	}
	to.set(mEnd.x + offsetX, mEnd.y + yOffset, mEnd.z + offsetZ);
	drawLowerMinus(from, to, width, divide);
	from = to;
	drawLowerMinus(from, to, width, divide);
	GXEnd();

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, vertexNum);
	from.set(mStart.x + offsetX, mStart.y + yOffset, mStart.z + offsetZ);
	for (int i = 0; i < mBoardNum; i++) {
		to = HangingBridgeBoardAt(this, i)->mRopeTop[0];
		setDrawPos(i, yOffset, &to);
		drawLowerPlus(from, to, width, divide);
		from = to;
	}
	to.set(mEnd.x + offsetX, mEnd.y + yOffset, mEnd.z + offsetZ);
	drawLowerPlus(from, to, width, divide);
	from = to;
	drawLowerPlus(from, to, width, divide);
	GXEnd();

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, vertexNum);
	from.set(mStart.x + offsetX, mStart.y + yOffset, mStart.z + offsetZ);
	for (int i = 0; i < mBoardNum; i++) {
		to = HangingBridgeBoardAt(this, i)->mRopeTop[0];
		setDrawPos(i, yOffset, &to);
		drawUpper(from, to, width, divide);
		from = to;
	}
	to.set(mEnd.x + offsetX, mEnd.y + yOffset, mEnd.z + offsetZ);
	drawUpper(from, to, width, divide);
	from = to;
	drawUpper(from, to, width, divide);
	GXEnd();

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, vertexNum);
	from.set(mStart.x - offsetX, mStart.y + yOffset, mStart.z - offsetZ);
	for (int i = 0; i < mBoardNum; i++) {
		to = HangingBridgeBoardAt(this, i)->mRopeTop[1];
		setDrawPos(i, yOffset, &to);
		drawLowerMinus(from, to, width, divide);
		from = to;
	}
	to.set(mEnd.x - offsetX, mEnd.y + yOffset, mEnd.z - offsetZ);
	drawLowerMinus(from, to, width, divide);
	from = to;
	drawLowerMinus(from, to, width, divide);
	GXEnd();

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, vertexNum);
	from.set(mStart.x - offsetX, mStart.y + yOffset, mStart.z - offsetZ);
	for (int i = 0; i < mBoardNum; i++) {
		to = HangingBridgeBoardAt(this, i)->mRopeTop[1];
		setDrawPos(i, yOffset, &to);
		drawLowerPlus(from, to, width, divide);
		from = to;
	}
	to.set(mEnd.x - offsetX, mEnd.y + yOffset, mEnd.z - offsetZ);
	drawLowerPlus(from, to, width, divide);
	from = to;
	drawLowerPlus(from, to, width, divide);
	GXEnd();

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, vertexNum);
	from.set(mStart.x - offsetX, mStart.y + yOffset, mStart.z - offsetZ);
	for (int i = 0; i < mBoardNum; i++) {
		to = HangingBridgeBoardAt(this, i)->mRopeTop[1];
		setDrawPos(i, yOffset, &to);
		drawUpper(from, to, width, divide);
		from = to;
	}
	to.set(mEnd.x - offsetX, mEnd.y + yOffset, mEnd.z - offsetZ);
	drawUpper(from, to, width, divide);
	from = to;
	drawUpper(from, to, width, divide);
	GXEnd();
}

void THangingBridge::initDraw() const
{
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXLoadPosMtxImm(j3dSys.mViewMtx, GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
	              GX_DF_NONE, GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
	              GX_DF_NONE, GX_AF_NONE);
	GXColor color = { 0, 0, 100, 255 };
	GXSetChanMatColor(GX_COLOR0A0, JUtility::TColor(color));
	GXSetNumTexGens(1);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
	                  GX_FALSE, GX_PTIDENTITY);

	if (gpMarDirector->mMap == 0xD) {
		JUTTexture texture(gpMapObjManager->unkCC);
		texture.load(GX_TEXMAP0);
	} else {
		JUTTexture texture(gpMapObjManager->unkCC);
		texture.load(GX_TEXMAP0);
	}

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
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetCullMode(GX_CULL_BACK);
}

void THangingBridge::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & 8) {
		initDraw();

		for (int i = 0; i < HangingBridgeBoardNum(this); i++) {
			THangingBridgeBoard* board = HangingBridgeBoardAtPerform(this, i);
			JGeometry::TVec3<f32> top;
			top = board->mRopeTop[0];
			board->drawOneRope(top);
			top = board->mRopeTop[1];
			board->drawOneRope(top);
		}

		if (MapObjMonteMarDirector()->mMap == 0xD)
			drawRopeBetweenBoards(-60.0f, mPointNumBetweenBoards);
		else
			drawRopeBetweenBoards(0.0f, mPointNumBetweenBoards);

		drawRopeBetweenBoards(mRopeHeight, 1);
	}
}

// UNUSED (0x120): the per-map parameter set-up at the head of loadAfter.
void THangingBridge::initMonte()
{
	mStart.set(0.0f, 0.0f, 11356.0f);
	mEnd.set(0.0f, -750.0f, 17743.0f);
	mRopeHeight     = 1000.0f;
	mNeighborRate   = 1.0f;
	mNeighbor2Rate  = 0.5f;
	mRopeOffset     = 315.0f;
}

// TODO: every instruction matches but the frame is 0x2a8 against our 0x208,
// and the board-fixup loop counter lands in r25 where retail uses r27.
// Retail puts the two unit-scale temporaries at 0x208/0x214 just under
// `rot`, leaves 0x18 above the board table (0xa0) and 0x94 below it; ours
// put the scale temporaries low at 0x1c/0x28. A named scale, rot declared
// first, and inlining initMonte() all fail. Likely a structural frame gap.
void THangingBridge::loadAfter()
{
	JDrama::TNameRef::loadAfter();

	f32 pitch = 0.0f;
	if (gpMarDirector->mMap == 0xD) {
		mBoardNum = 14;
		mStart.set(1550.0f, 2980.0f, -9410.0f);
		mEnd.set(3570.0f, 2455.0f, -9410.0f);
		pitch          = 90.0f;
		mRopeHeight    = 150.0f;
		mNeighborRate  = 0.8f;
		mNeighbor2Rate = 0.5f;
		mRopeOffset    = 160.0f;
	} else if (gpMarDirector->mMap == 8) {
		mBoardNum = 19;
		mStart.set(0.0f, 0.0f, 11356.0f);
		mEnd.set(0.0f, -750.0f, 17743.0f);
		mRopeHeight    = 1000.0f;
		mNeighborRate  = 1.0f;
		mNeighbor2Rate = 0.5f;
		mRopeOffset    = 315.0f;
	}

	mSideDir.set(mEnd.x - mStart.x, mEnd.z - mStart.z);
	mSideDir.setLength(1.0f);

	f32 cosQ = cosf(1.5707964f);
	f32 sinQ = sinf(1.5707964f);
	mSideDir.set(mSideDir.x * cosQ - mSideDir.y * sinQ,
	             mSideDir.x * sinQ + mSideDir.y * cosQ);

	mBoards = new THangingBridgeBoard*[mBoardNum];
	for (int i = 0; i < mBoardNum; i++) {
		f32 t = (f32)i / (f32)(mBoardNum - 1);
		JGeometry::TVec3<f32> pos;
		pos.x = t * (mEnd.x - mStart.x) + mStart.x;
		pos.y = (t * (mEnd.y - mStart.y) + mStart.y)
		    - 0.0f * sinf(3.14f * t);
		pos.z = t * (mEnd.z - mStart.z) + mStart.z;

		JGeometry::TVec3<f32> rot(15.0f, pitch, 0.0f);
		if (gpMarDirector->mMap == 8) {
			mBoards[i] = (THangingBridgeBoard*)
			    TMapObjManager::newAndRegisterObj(
			        "HangingBridgeBoard", pos, rot,
			        JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
		} else {
			mBoards[i] = (THangingBridgeBoard*)
			    TMapObjManager::newAndRegisterObj(
			        "PinnaHangingBridgeBoard", pos, rot,
			        JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
		}
		mBoards[i]->mBridge = this;
		mBoards[i]->appear();
	}

	if (gpMarDirector->mMap == 8) {
		f32 board[21][4] = {
			{ 0.0f, -130.0f, 11965.0f, 30.0f },
			{ 0.0f, -275.0f, 12225.0f, 29.0f },
			{ 0.0f, -415.0f, 12490.0f, 28.0f },
			{ 0.0f, -540.0f, 12760.0f, 26.0f },
			{ 0.0f, -660.0f, 13035.0f, 24.0f },
			{ 0.0f, -770.0f, 13315.0f, 22.0f },
			{ 0.0f, -875.0f, 13595.0f, 20.0f },
			{ 0.0f, -960.0f, 13895.0f, 12.0f },
			{ 0.0f, -1020.0f, 14190.0f, 8.0f },
			{ 0.0f, -1060.0f, 14490.0f, 4.0f },
			{ 0.0f, -1090.0f, 14790.0f, 2.0f },
			{ 0.0f, -1090.0f, 15090.0f, 0.0f },
			{ 0.0f, -1080.0f, 15395.0f, -4.0f },
			{ 0.0f, -1040.0f, 15695.0f, -6.0f },
			{ 0.0f, -995.0f, 15990.0f, -8.0f },
			{ 0.0f, -945.0f, 16285.0f, -8.0f },
			{ 0.0f, -900.0f, 16580.0f, -8.0f },
			{ 0.0f, -855.0f, 16880.0f, -8.0f },
			{ 0.0f, -800.0f, 17175.0f, -10.0f },
			{ -1.0f, 0.0f, 0.0f, 0.0f },
			{ -99999.0f, 0.0f, 0.0f, 0.0f },
		};

		for (int i = 0; i < mBoardNum; i++) {
			if (board[i][0] == -1.0f)
				break;
			if (board[i][0] == -1.0f)
				continue;

			mBoards[i]->mInitialPosition.set(board[i][0], board[i][1],
			                                 board[i][2]);
			mBoards[i]->mPosition.set(mBoards[i]->mInitialPosition);
			mBoards[i]->mRotation.x = board[i][3];
			mBoards[i]->calcDefaultMtx();
		}
	}

	if (gpMarDirector->mMap == 0xD) {
		mStart.set(1436.32f, 3201.477f - mRopeHeight, -9417.205f);
		mEnd.set(3656.32f, 2631.477f - mRopeHeight, -9417.205f);
	} else if (gpMarDirector->mMap == 8) {
		mEnd.z -= 120.0f;
	}

	for (int i = 0; i < mBoardNum; i++) {
		if (i > 0)
			mBoards[i]->mPrev = mBoards[i - 1];
		if (i > 1)
			mBoards[i]->mPrev2 = mBoards[i - 2];
		if (i < mBoardNum - 1)
			mBoards[i]->mNext = mBoards[i + 1];
		if (i < mBoardNum - 2)
			mBoards[i]->mNext2 = mBoards[i + 2];
	}

	mRopeSag  = new f32[mPointNumBetweenBoards];
	f32 tStep = 1.0f / mPointNumBetweenBoards;
	f32 t     = 0.0f;
	for (int i = 0; i < mPointNumBetweenBoards; i++) {
		mRopeSag[i] = 50.0f * sinf(3.14f * t);
		t += tStep;
	}
}

THangingBridge::THangingBridge(const char* name)
    : JDrama::TViewObj(name)
{
	mBoardNum      = 0;
	mBoards        = nullptr;
	mRopeSag       = nullptr;
	mRopeOffset    = 0.0f;
	mNeighborRate  = 0.0f;
	mNeighbor2Rate = 0.0f;
}

f32 TSwingBoard::mBoardWidth      = 315.0f;
f32 TSwingBoard::mRopeWidthX      = 10.0f;
f32 TSwingBoard::mRopeWidthZ      = 7.0f;
f32 TSwingBoard::mTexPosRate      = 0.01f;
f32 TSwingBoard::mReturnAccelRate = 0.0001f;
f32 TSwingBoard::mSpeedDownRate   = 0.998f;

void TSwingBoard::drawOneRope(const JGeometry::TVec3<f32>& bottom,
                              const JGeometry::TVec3<f32>& top) const
{
	f32 topXPlus     = top.x + mRopeWidthX;
	f32 topZPlus     = top.z + mRopeWidthZ;
	f32 topZMinus    = top.z - mRopeWidthZ;
	f32 bottomXPlus  = bottom.x + mRopeWidthX;
	f32 bottomZPlus  = bottom.z + mRopeWidthZ;
	f32 bottomZMinus = bottom.z - mRopeWidthZ;
	f32 texPos       = mRopeLength * mTexPosRate;
	f32 widthX       = mRopeWidthX;
	f32 topX         = top.x;
	f32 bottomX      = bottom.x;

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 8);
	GXPosition3f32(bottom.x, bottom.y, bottomZPlus);
	GXTexCoord2f32(0.0f, texPos);
	GXPosition3f32(top.x, top.y, topZPlus);
	GXTexCoord2f32(0.0f, 0.0f);

	GXPosition3f32(bottomXPlus, bottom.y, bottomZMinus);
	GXTexCoord2f32(1.0f, texPos);
	GXPosition3f32(topXPlus, top.y, topZMinus);
	GXTexCoord2f32(1.0f, 0.0f);

	GXPosition3f32(bottomX - widthX, bottom.y, bottomZMinus);
	GXTexCoord2f32(2.0f, texPos);
	GXPosition3f32(topX - widthX, top.y, topZMinus);
	GXTexCoord2f32(2.0f, 0.0f);

	GXPosition3f32(bottom.x, bottom.y, bottomZPlus);
	GXTexCoord2f32(3.0f, texPos);
	GXPosition3f32(top.x, top.y, topZPlus);
	GXTexCoord2f32(3.0f, 0.0f);
	GXEnd();
}

void TSwingBoard::initDraw() const
{
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXLoadPosMtxImm(j3dSys.mViewMtx, GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
	              GX_DF_NONE, GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
	              GX_DF_NONE, GX_AF_NONE);
	// TODO: 100% instructions, frame 0x88 vs retail 0x80. Same +8 TColor
	// residue as TCogwheel/TWireBell::initDraw; a named TColor lands the
	// frame but parks the color copy in the named block. The 4-arg
	// TColor(0,0,100,255) ctor is byte stores and drops to 92.9%.
	GXColor color = { 0, 0, 100, 255 };
	GXSetChanMatColor(GX_COLOR0A0, JUtility::TColor(color));
	GXSetNumTexGens(1);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
	                  GX_FALSE, GX_PTIDENTITY);

	JUTTexture texture(gpMapObjManager->unkCC);
	texture.load(GX_TEXMAP0);

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
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetCullMode(GX_CULL_BACK);
}

// TODO: 99.8%. Every instruction matches and the frame is retail's 0x58, but
// the `bottom`/`top` block sits 8 bytes low: we carry 8 bytes too few of pool
// and 8 too many above the block. The getInitialPosition()/getPosition()
// component rungs saturate at +0x10 and all land above the block.
void TSwingBoard::draw() const
{
	initDraw();

	JGeometry::TVec3<f32> bottom;

	JGeometry::TVec3<f32> top;
	MtxPtr mtx = getModel()->getAnmMtx(0);

	f32 width = mBoardWidth;
	top.x     = width * mtx[0][0] + getInitialPosition().x;
	top.y     = mRopeLength + getInitialPosition().y;
	top.z     = width * mtx[2][0] + getInitialPosition().z;
	bottom.x  = width * mtx[0][0] + getPosition().x;
	bottom.y  = 60.0f + mPosition.y;
	bottom.z  = width * mtx[2][0] + mPosition.z;
	drawOneRope(bottom, top);

	width    = mBoardWidth;
	top.x    = getInitialPosition().x - width * mtx[0][0];
	top.z    = getInitialPosition().z - width * mtx[2][0];
	bottom.x = mPosition.x - width * mtx[0][0];
	bottom.z = mPosition.z - width * mtx[2][0];
	drawOneRope(bottom, top);
}

// UNUSED (0xa8): the water-jet push at the head of control.
void TSwingBoard::swing()
{
	if (marioIsOn() && marioIsOn()
	    && SMS_GetMarioWaterGun()->isEmitWater()) {
		MtxPtr emit = SMS_GetMarioWaterGun()->getEmitMtx(0);
		f32 dirX    = -emit[0][0];
		f32 dirY    = 0.0f;
		f32 dirZ    = -emit[2][0];
		MtxPtr mtx  = getModel()->getAnmMtx(0);
		mAngleSpeed += mAccelRate
		    * (mtx[0][2] * dirX + mtx[1][2] * dirY + mtx[2][2] * dirZ);
	}
}

// TODO: 99.9%, every instruction exact; retail's frame is 0x118 against our
// 0x70, i.e. 0xa8 more dead low region with no carrier identified.
void TSwingBoard::control()
{
	TMapObjBase::control();

	if (marioIsOn() && marioIsOn()
	    && SMS_GetMarioWaterGun()->isEmitWater()) {
		MtxPtr emit = SMS_GetMarioWaterGun()->getEmitMtx(0);
		f32 dirX    = -emit[0][0];
		f32 dirY    = 0.0f;
		f32 dirZ    = -emit[2][0];
		MtxPtr mtx  = getModel()->getAnmMtx(0);
		mAngleSpeed += mAccelRate
		    * (mtx[0][2] * dirX + mtx[1][2] * dirY + mtx[2][2] * dirZ);
	}

	mAngle += mAngleSpeed;
	f32 previousSpeed = mAngleSpeed;
	mAngleSpeed       = previousSpeed - mAngle * mReturnAccelRate;
	if (fabsf(mAngleSpeed) > mAngleSpeedMax)
		mAngleSpeed *= mSpeedDownRate;

	if (previousSpeed * mAngleSpeed <= 0.0f) {
		if (mSound)
			mSound->stop(1);

		if (mAngleSpeed > 0.0f) {
			gpMSound->startSoundActorWithInfo(MSD_SE_OBJ_SWING1,
			                                  &mPosition, nullptr,
			                                  fabsf(mAngle), 0, 0, &mSound, 0,
			                                  4);
		} else {
			gpMSound->startSoundActorWithInfo(MSD_SE_OBJ_SWING2,
			                                  &mPosition, nullptr,
			                                  fabsf(mAngle), 0, 0, &mSound, 0,
			                                  4);
		}
	}

	mRotation.x = -mAngle;
	f32 sinA    = sinf(3.14f * (mRotation.x / 180.0f));
	f32 cosA    = cosf(3.14f * (mRotation.x / 180.0f));

	Mtx rot;
	rot[0][0] = 1.0f;
	rot[0][1] = 0.0f;
	rot[0][2] = 0.0f;
	rot[0][3] = 0.0f;
	rot[1][0] = 0.0f;
	rot[1][1] = cosA;
	rot[1][2] = -sinA;
	rot[1][3] = 0.0f;
	rot[2][0] = 0.0f;
	rot[2][1] = sinA;
	rot[2][2] = cosA;
	rot[2][3] = 0.0f;

	MtxPtr mtx = getModel()->getAnmMtx(0);
	MTXConcat(mBaseMtx, rot, mtx);

	cosf(3.14f * (mAngle / 180.0f));
	sinf(3.14f * (mAngle / 180.0f));

	mPosition.x = mInitialPosition.x - mtx[0][1] * mRopeLength;
	mPosition.y = (mRopeLength + mInitialPosition.y) - mtx[1][1] * mRopeLength;
	mPosition.z = mInitialPosition.z - mtx[2][1] * mRopeLength;
	mtx[0][3]   = mPosition.x;
	mtx[1][3]   = mPosition.y;
	mtx[2][3]   = mPosition.z;
}

void TSwingBoard::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);

	stream >> mRopeLength;
	if (mRopeLength == -1.0f)
		mRopeLength = 5000.0f;

	stream >> mAccelRate;
	if (mAccelRate > 10.0f || mAccelRate == 0.0f)
		mAccelRate = 0.003f;

	mAnchor.set(mPosition.x, mPosition.y + mRopeLength, mPosition.z);

	mAngleSpeedMax = 0.05f * ((1.0f + MsRandF()) / 2.0f);
	mAngle         = 20.0f * (MsRandF() - 0.5f);
	if (mAngle > 0.0f)
		mAngleSpeed = mAngleSpeedMax * -MsRandF();
	else
		mAngleSpeed = mAngleSpeedMax * MsRandF();

	// 12x ref() is +0x30 over all-raw (0x68 vs 0x38). One ref() (any of the
	// twelve) lands retail's 0x40 with every other instruction right but
	// swaps the epilogue addi/mtlr (98.5%). Left as all ref().
	s16 yaw  = (s16)(182.04445f * mRotation.y);
	f32 sinY = JMASSin(yaw);
	f32 cosY = JMASCos(yaw);
	mBaseMtx.ref(0, 0) = cosY;
	mBaseMtx.ref(0, 1) = 0.0f;
	mBaseMtx.ref(0, 2) = sinY;
	mBaseMtx.ref(0, 3) = 0.0f;
	mBaseMtx.ref(1, 0) = 0.0f;
	mBaseMtx.ref(1, 1) = 1.0f;
	mBaseMtx.ref(1, 2) = 0.0f;
	mBaseMtx.ref(1, 3) = 0.0f;
	mBaseMtx.ref(2, 0) = -sinY;
	mBaseMtx.ref(2, 1) = 0.0f;
	mBaseMtx.ref(2, 2) = cosY;
	mBaseMtx.ref(2, 3) = 0.0f;
}

TSwingBoard::TSwingBoard(const char* name)
    : TMapObjBase(name)
{
	mRopeLength    = 5000.0f;
	mAngle         = 0.0f;
	mAccelRate     = 0.0f;
	mAngleSpeed    = 0.0f;
	mAngleSpeedMax = 0.0f;
	mSound         = nullptr;

	mBaseMtx.ref(0, 3) = mBaseMtx.ref(1, 3) = mBaseMtx.ref(2, 3) = 0.0f;
	mBaseMtx.ref(0, 2) = mBaseMtx.ref(1, 2) = 0.0f;
	mBaseMtx.ref(0, 1) = mBaseMtx.ref(2, 1) = 0.0f;
	mBaseMtx.ref(1, 0) = mBaseMtx.ref(2, 0) = 0.0f;
	mBaseMtx.ref(0, 0) = mBaseMtx.ref(1, 1) = mBaseMtx.ref(2, 2) = 1.0f;

	mAnchor.zero();
}

// The guard reads the manager through a level the setter does not: the
// binder is +8 at one site and +0x10 (and a register reshuffle) at both,
// and retail's 0x28 frame wants exactly one.
static inline TFlagManager* GoalFlagGetFlagManager()
{
	TFlagManager* flagManager = TFlagManager::smInstance;
	return flagManager;
}

void TGoalFlag::touchActor(THitActor* actor)
{
	if (actor->isActorType(0x80000001)) {
		if (!GoalFlagGetFlagManager()->getBool(0x50005))
			TFlagManager::getInstance()->setBool(true, 0x50005);

		actor->receiveMessage(this, 0xE);
		return;
	}

	if (actor->isActorType(0x08000002))
		actor->receiveMessage(this, 0xE);
}

void TGoalFlag::initMapObj() { TMapObjBase::initMapObj(); }

f32 TFluff::mScaleUpSpeed   = 0.05f;
f32 TFluff::mScaleDownSpeed = 0.01f;

u32 TFluff::touchWater(THitActor* actor)
{
	const JGeometry::TVec3<f32>& water = getWaterPos(actor);

	JGeometry::TVec3<f32> push;
	getNormalVecFromTarget(water.x, water.y, water.z, &push);

	mVelocity.x = mVelocity.x - push.x * mWaterPushRate;
	mVelocity.y = mVelocity.y - push.y * mWaterPushRate;
	mVelocity.z = mVelocity.z - push.z * mWaterPushRate;

	return 1;
}

// TODO: 99.9%, every instruction matches; retail's frame is 0x78 against
// our 0x28. isZero is called on unkD0 itself (lfsu); the missing 0x50 is
// an unidentified carrier, not another wind copy.
void TFluff::move()
{
	mPosition.y -= mFallSpeed;
	if (mPosition.y < 0.0f) {
		mPosition.y = 5000.0f;
		if (mHeldObject) {
			mHeldObject->receiveMessage(this, 8);
			mHeldObject->mHolder = nullptr;
			mHeldObject          = nullptr;
		}
	}

	mDrift.x += mWindRate * gpMapObjManager->unkD0.x;
	mDrift.z += mWindRate * gpMapObjManager->unkD0.z;

	JGeometry::TVec3<f32> velocity = mVelocity;
	mDrift.x += velocity.x;
	mDrift.y += velocity.y;
	mDrift.z += velocity.z;

	f32 down = mSpeedDownRate;
	mVelocity.x *= down;
	mVelocity.y *= down;
	mVelocity.z *= down;

	f32 swing   = mSwingRadius * sinf(3.14f * mSwingAngle / 180.0f);
	mPosition.x = mDrift.x + (swing * (mSwingCos + mSwingSin)
	                          + mInitialPosition.x);
	mPosition.y += mWindRate * gpMapObjManager->unkD0.y;
	mPosition.z = mDrift.z + (swing * (mSwingSin - mSwingCos)
	                          + mInitialPosition.z);

	if (gpMapObjManager->unkD0.isZero()) {
		mSwingAngle += mSwingAngleSpeed;
		if (mSwingAngle > 360.0f)
			mSwingAngle -= 360.0f;
	}

	if (mHeldObject && mHeldObject->isActorType(0x80000001))
		gpMarioPos->y -= mFallSpeed;
}

// Binding level over a raw member read, worth +8 of low region in
// TFluff::kill (batch 127).
static inline TTakeActor* MapObjMonteHeldObject(const TFluff* p)
{
	TTakeActor* heldObject = p->mHeldObject;
	return heldObject;
}

void TFluff::kill()
{
	if (mHeldObject) {
		MapObjMonteHeldObject(this)->receiveMessage(this, 8);
		mHeldObject->mHolder = nullptr;
		mHeldObject          = nullptr;
	}

	mState = STATE_VANISHING;
}

static inline TMap* MapObjMonteMapRaw() { return gpMap; }

static inline TMap* MapObjMonteMap()
{
	TMap* map = gpMap;
	return map;
}

static inline TMap* MapObjMonteMapNested()
{
	TMap* map = MapObjMonteMapRaw();
	return map;
}

void TFluff::control()
{
	TMapObjBase::control();
	move();

	switch (mState) {
	case STATE_APPEARING:
		mScaling.x += mScaleUpSpeed;
		mScaling.y += mScaleUpSpeed;
		mScaling.z += mScaleUpSpeed;
		if (mScaling.x > 1.0f) {
			mScaling.set(1.0f, 1.0f, 1.0f);
			setObjHitData(0);
			mState = STATE_FLYING;
		}
		break;

	case STATE_FLYING: {
		mGroundHeight
		    = MapObjMonteMapNested()->checkGround(mPosition, &mGroundPlane);
		JGeometry::TVec3<f32> velocity = mVelocity;
		if (velocity.y < 0.0f
		    && (mGroundHeight > mPosition.y - mFallSpeed
		        || mPosition.y < -1000.0f))
			kill();

		if (MapObjMonteMap()->isTouchedOneWall(mPosition.x, mPosition.y,
		                                      mPosition.z, 100.0f))
			kill();

		if (mPosition.x < -14848.0f || 14848.0f < mPosition.x
		    || mPosition.z < -19968.0f || 19968.0f < mPosition.z)
			kill();
		break;
	}

	case STATE_VANISHING:
		mScaling.x -= mScaleDownSpeed;
		mScaling.y -= mScaleDownSpeed;
		mScaling.z -= mScaleDownSpeed;
		if (mScaling.x < 0.1f) {
			gpMarioParticleManager->emitAndBindToPosPtr(0xE5, &mPosition, 0,
			                                            nullptr);
			gpMSound->startSoundActor(MSD_SE_SMOKE_EFFECT, &mPosition,
			                          0, nullptr, 0, 4);
			mScaling.set(0.0001f, 0.0001f, 0.0001f);
			mStateTimer = 240;
			mState      = STATE_WAIT_APPEAR;
		}
		break;

	case STATE_WAIT_APPEAR:
		if (!isStateTimerEngaged()) {
			appear();
			mRotation.set(0.0f, 360.0f * MsRandF(), 0.0f);
			mInitialRotation = mRotation;
			mIsRideable = false;
			TFluffManager* man = mManager;
			if (!man->mNextFluff) {
				man->mNextFluff = this;
				man->mNextFluff->makeObjDead();
			}
		}
		break;
	}
}

void TFluff::appear()
{
	makeObjAppeared();

	mPosition.set(mManager->getRandomX(),
	              mManager->mPosition.y * MsRandF(),
	              mManager->getRandomZ());
	mInitialPosition = mPosition;

	mScaling.set(0.0001f, 0.0001f, 0.0001f);
	mDrift.zero();
	mSwingAngle = 0.0f;

	mWindRate   = 0.8f * MsRandF() + 0.2f;
	mSwingSin   = sinf(3.14f * mRotation.y / 180.0f);
	mSwingCos   = cosf(3.14f * mRotation.y / 180.0f);
	mSwingAngle = 360.0f * MsRandF();
	mSwingAngleSpeed = 0.3f;
	mState      = STATE_APPEARING;
}

void TFluff::initMapObj()
{
	TMapObjBase::initMapObj();
	mSwingRadius = 300.0f;
	mFallSpeed   = 0.5f;
}

TFluff::TFluff(const char* name)
    : TMapObjBase(name)
{
	mSwingRadius     = 0.0f;
	mFallSpeed       = 0.0f;
	mSwingSin        = 0.0f;
	mSwingCos        = 0.0f;
	mSwingAngle      = 0.0f;
	mSwingAngleSpeed = 0.0f;
	mWindRate        = 0.0f;
	mWaterPushRate   = 1.0f;
	mSpeedDownRate   = 0.95f;
	mManager         = nullptr;
	mIsRideable      = false;
	mDrift.zero();
}

f32 TFluffManager::mWindMin = 1.0f;

// UNUSED (0x118, ours 0xf4): the search control's STATE_WAIT branch expands.
//
// The distance test is `mPosition.distance(*gpMarioPos)`, not the class's own
// `getDistance(*gpMarioPos)`: retail's expansion in TFluffManager::control
// subtracts `mPosition - marioPos` off an advancing base (`lfsu f3, 0x10(r3)`)
// with no `mYOffset` term and ends in `bl TUtil<f32>::sqrt`, while
// getDistance's own out-of-line body (MapObjLib.cpp, map 0x7c, 99.9% here)
// subtracts the other way, reads 0x108 and expands the `frsqrte` refinement.
// Worth +6.4 on control (90.0 -> 96.4); no change to MapObjBase.hpp is needed.
void TFluffManager::findNextFluff()
{
	for (int i = 3; i < mFluffMax; i++) {
		if (!mFluffs[i]->mIsRideable && !mFluffs[i]->mHeldObject
		    && mFluffs[i]->mPosition.distance(*gpMarioPos) > 3000.0f) {
			mNextFluff = mFluffs[i];
			mFluffs[i]->kill();
			return;
		}
	}
}

// TODO: 99.6%. STATE_CALM is instruction-exact (reference-bind `lfsu` on
// unkD0, scale-in-place, raw stores after one global reload, `mWindMin`
// read at the compare). STATE_BLOW still colours the first four loads
// differently and swaps the z pair; the frame is 0x48 short. A
// reference-bind on BLOW's unkD0 is wrong (`lfsu` first, 99.3%).
void TFluffManager::control()
{
	switch (mState) {
	case STATE_WAIT:
		if (!mNextFluff
		    && mRideFluff->mPosition.y - 100.0f < mPosition.y - mBlowHeight)
			findNextFluff();

		if (mRideFluff->mPosition.y < mPosition.y - mBlowHeight) {
			gpMSound->startSoundActor(MSD_SE_OBJ_WATAGE_WIND,
			                          &mRideFluff->mPosition, 0, nullptr, 0,
			                          4);
			mStateTimer = mBlowTime;
			mState      = STATE_BLOW;
		}
		break;

	// `add(mWind, unkD0)` loads mWind.x/unkD0.x/mWind.y/unkD0.y in retail
	// order. The z pair is still swapped and the first `fadds` lives in
	// unkD0's register in retail (`fadds f2, f2, f0`).
	case STATE_BLOW: {
		TMapObjManager* man = gpMapObjManager;
		JGeometry::TVec3<f32> wind;
		wind.add(mWind, man->unkD0);
		man->unkD0.set(wind);
		if (!isStateTimerEngaged())
			mState = STATE_CALM;
		break;
	}

	case STATE_CALM: {
		JGeometry::TVec3<f32>& d0 = gpMapObjManager->unkD0;
		f32 windX                 = d0.x;
		f32 rate                  = mWindDownRate;
		f32 windY                 = d0.y;
		f32 windZ                 = d0.z;
		windX *= rate;
		windY *= rate;
		windZ *= rate;

		if (fabsf(windX) < mWindMin && fabsf(windY) < mWindMin
		    && fabsf(windZ) < mWindMin) {
			windX      = 0.0f;
			windY      = 0.0f;
			mRideFluff = mNextFluff;
			windZ      = 0.0f;

			mRideFluff->mRotation.set(mRotation);
			mRideFluff->mInitialRotation = mRotation;
			mRideFluff->appear();
			mRideFluff->mPosition.set(mPosition);
			mRideFluff->mInitialPosition = mPosition;
			mRideFluff->mRotation.set(0.0f, 0.0f, 0.0f);
			mRideFluff->mInitialRotation = mRideFluff->mRotation;
			mRideFluff->mSwingAngle = 0.0f;
			mRideFluff->mWindRate   = 1.0f;
			mRideFluff->mIsRideable = true;
			mNextFluff              = nullptr;
			mState                  = STATE_WAIT;
		}

		TMapObjManager* man = gpMapObjManager;
		man->unkD0.x        = windX;
		man->unkD0.y        = windY;
		man->unkD0.z        = windZ;
		break;
	}
	}
}

// UNUSED (0x40).
void TFluffManager::registerNextFluff(TFluff* fluff)
{
	mFluffs[mFluffNum] = fluff;
	mFluffNum++;
}

// UNUSED (0x10c): the placement loadAfter expands for both of the named
// seeds. TODO: the map size is 0x10c and this body compiles to 0x128, so the
// exact statement set is still a guess.
void TFluffManager::setUpNextFluff()
{
	mNextFluff->mPosition.set(mPosition);
	mNextFluff->mRotation.set(mRotation);
	mNextFluff->mInitialPosition.set(getRandomX(), mPosition.y * MsRandF(),
	                                 getRandomZ());
}

// UNUSED (0x6c).
TFluff* TFluffManager::newFluff(const char* name)
{
	TFluff* fluff = new TFluff(name);
	fluff->initAndRegister("Fluff");
	fluff->mManager = this;
	return fluff;
}

// UNUSED (0x64 each).
f32 TFluffManager::getRandomX() const
{
	return mRangeX * (2.0f * MsRandF() - 1.0f);
}

f32 TFluffManager::getRandomZ() const
{
	return mRangeZ * (2.0f * MsRandF() - 1.0f);
}

// TODO: 99.8%. Both named seeds now word-copy a stack TVec3 into
// mInitialPosition (`stfs` then `lwz`/`stw`); retail's frame is 0x78
// against our 0x88. The extra 0x10 is not absorbed by a ctor temporary
// (two slots, 0x98) or an inlined assign helper (97.8%, extra fluff
// pointer load). Ruled out: 4-arg TColor() on SwingBoard::initDraw
// (byte stores, 92.9%).
void TFluffManager::loadAfter()
{
	mFluffNum = 0;
	mFluffMax = 32;
	mFluffs   = new TFluff*[mFluffMax];

	mRideFluff = newFluff("１つ目のわた毛");
	mRideFluff->mIsRideable = true;
	mRideFluff->appear();
	mRideFluff->mPosition.set(mPosition);
	mRideFluff->mRotation.set(mRotation);
	JGeometry::TVec3<f32> initPos;
	initPos.set(getRandomX(), mPosition.y * MsRandF(), getRandomZ());
	mRideFluff->mInitialPosition = initPos;
	registerNextFluff(mRideFluff);

	mNextFluff = newFluff("２つ目のわた毛");
	mNextFluff->mPosition.set(mPosition);
	mNextFluff->mRotation.set(mRotation);
	initPos.set(getRandomX(), mPosition.y * MsRandF(), getRandomZ());
	mNextFluff->mInitialPosition = initPos;
	mNextFluff->makeObjDead();
	registerNextFluff(mNextFluff);

	for (int i = 2; i < mFluffMax; i++) {
		TFluff* fluff      = newFluff("わた毛");
		mFluffs[mFluffNum] = fluff;
		mFluffs[mFluffNum]->appear();
		mFluffNum++;
	}
}

void TFluffManager::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);

	stream >> mBlowHeight;

	f32 power;
	stream >> power;
	power *= 0.01f;

	stream >> mBlowTime;

	mRangeX       = 5000.0f;
	mRangeZ       = 5000.0f;
	mWindDownRate = 0.998f;

	JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > mtx;
	MsMtxSetXYZRPH(mtx, 0.0f, 0.0f, 0.0f, mRotation.x, mRotation.y,
	               mRotation.z);

	mWind.set(0.0f, 0.0f, 1.0f);
	mtx.mult(mWind, mWind);
	mWind.scale(power);
}

TFluffManager::TFluffManager(const char* name)
    : TMapObjBase(name)
{
	mRangeX       = 0.0f;
	mRangeZ       = 0.0f;
	mBlowHeight   = 0.0f;
	mBlowTime     = 0;
	mWindDownRate = 0.0f;
	mRideFluff    = nullptr;
	mNextFluff    = nullptr;
	mFluffNum     = 0;
	mFluffMax     = 0;
	mWind.set(0.0f, 0.0f, 0.0f);
}
