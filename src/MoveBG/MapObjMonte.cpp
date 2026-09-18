#include <MoveBG/MapObjMonte.hpp>
#include <System/DummyStrings.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <Map/MapCollisionManager.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JGeometry.hpp>
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
	mDamageRadius = 1400.0f * getScaling().y;
	calcEntryRadius();
	mPosition.y = getInitialPosition().y + getObjCollisionHeightOffset();
}

BOOL TJumpMushroom::receiveMessage(THitActor* sender, u32 message)
{
	startAnim(1);
	return TRUE;
}

void TJumpMushroom::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);

	s32 data;
	stream >> data;
	if (mMapCollisionManager)
		mMapCollisionManager->unk8->setAllData((s16)data);
}

f32 THangingBridgeBoard::mMarioAccelY        = 0.15f;
f32 THangingBridgeBoard::mMarioHipDropAccelY = 2.0f;
f32 THangingBridgeBoard::mReturnAccelRate    = 0.005f;
f32 THangingBridgeBoard::mSpeedDownRate      = 0.98f;
f32 THangingBridgeBoard::mRopeWidthX         = 10.0f;
f32 THangingBridgeBoard::mRopeWidthZ         = 7.0f;
f32 THangingBridgeBoard::mTexPosRate         = 0.01f;

void THangingBridgeBoard::drawOneRope(const JGeometry::TVec3<f32>& top) const
{
	f32 bottom = top.y;
	f32 hookY  = top.y + THangingBridge::mRopeHeight;

	f32 xPlus  = top.x + mRopeWidthX;
	f32 xMinus = top.x - mRopeWidthX;
	f32 zPlus  = top.z + mRopeWidthZ;
	f32 zMinus = top.z - mRopeWidthZ;

	if (gpMarDirector->mMap == 0xD)
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
void THangingBridgeBoard::push(f32 accel) { mVelocity.y -= accel; }

// UNUSED (0x94): the neighbour chain control expands twice.
void THangingBridgeBoard::pushNeighbor(f32 accel)
{
	if (mPrev) {
		mPrev->mVelocity.y -= accel * mBridge->mNeighborRate;
		if (mPrev2)
			mPrev2->mVelocity.y -= accel * mBridge->mNeighbor2Rate;
	}

	if (mNext) {
		mNext->mVelocity.y -= accel * mBridge->mNeighborRate;
		if (mNext2)
			mNext2->mVelocity.y -= accel * mBridge->mNeighbor2Rate;
	}
}

// TODO: 89.3%. pushNeighbor is expanded twice here as in retail; the residual
// is register numbering around mBridge's two rates.
void THangingBridgeBoard::control()
{
	TLeanBlock::control();

	if (marioIsOn()) {
		mVelocity.y -= mMarioAccelY;
		if (mPrev) {
			mPrev->mVelocity.y -= mMarioAccelY * mBridge->mNeighborRate;
			if (mPrev2)
				mPrev2->mVelocity.y
				    -= mMarioAccelY * mBridge->mNeighbor2Rate;
		}
		if (mNext) {
			mNext->mVelocity.y -= mMarioAccelY * mBridge->mNeighborRate;
			if (mNext2)
				mNext2->mVelocity.y
				    -= mMarioAccelY * mBridge->mNeighbor2Rate;
		}
	}

	if (marioHipAttack()) {
		mVelocity.y -= mMarioHipDropAccelY;
		if (mPrev) {
			mPrev->mVelocity.y
			    -= mMarioHipDropAccelY * mBridge->mNeighborRate;
			if (mPrev2)
				mPrev2->mVelocity.y
				    -= mMarioHipDropAccelY * mBridge->mNeighbor2Rate;
		}
		if (mNext) {
			mNext->mVelocity.y
			    -= mMarioHipDropAccelY * mBridge->mNeighborRate;
			if (mNext2)
				mNext2->mVelocity.y
				    -= mMarioHipDropAccelY * mBridge->mNeighbor2Rate;
		}
	}

	mPosition.y += mVelocity.y;
	mVelocity.y += mReturnAccelRate * (mInitialPosition.y - mPosition.y);
	mVelocity.y *= mSpeedDownRate;

	MtxPtr mtx      = getModel()->getAnmMtx(0);
	mRopeTop[0].x   = mPosition.x - mtx[0][0] * mBridge->mRopeOffset;
	mRopeTop[0].y   = 70.0f + (mPosition.y - mtx[1][0] * mBridge->mRopeOffset);
	mRopeTop[0].z   = mPosition.z - mtx[2][0] * mBridge->mRopeOffset;
	mRopeTop[1].x   = mtx[0][0] * mBridge->mRopeOffset + mPosition.x;
	mRopeTop[1].y   = 70.0f + (mtx[1][0] * mBridge->mRopeOffset + mPosition.y);
	mRopeTop[1].z   = mtx[2][0] * mBridge->mRopeOffset + mPosition.z;
}

void THangingBridgeBoard::calcDefaultMtx()
{
	Mtx rotX;
	makeRootMtxRotX(rotX);
	Mtx rotY;
	makeRootMtxRotY(rotY);
	MTXConcat(rotY, rotX, rotY);
	getDefaultMtx().set(rotY);

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
	f32 texRate  = mBetweenBoardsTexPosRate;
	f32 ropeY    = mRopeWidthBetweenBoardsY;
	f32 dx       = step * (to.x - from.x);
	f32 dy       = step * (to.y - from.y);
	f32 dz       = step * (to.z - from.z);

	for (int i = 0; i < divide; i++) {
		f32 sag = y - mRopeSag[i];
		f32 tex = texRate * (x + z);
		GXPosition3f32(x - width.x, sag, z - width.y);
		GXTexCoord2f32(0.0f, tex);
		GXPosition3f32(x, sag - ropeY, z);
		GXTexCoord2f32(1.0f, tex);
		x += dx;
		y += dy;
		z += dz;
	}
}

// TODO: 88.4%. Same instructions as retail; only float-register numbering and
// the scheduling of the 1.0f/divide load differ (declaring step first changes
// nothing).
void THangingBridge::drawLowerPlus(const JGeometry::TVec3<f32>& from,
                                   const JGeometry::TVec3<f32>& to,
                                   const JGeometry::TVec2<f32>& width,
                                   int divide) const
{
	f32 x        = from.x;
	f32 y        = from.y;
	f32 z        = from.z;
	f32 step     = 1.0f / divide;
	f32 texRate  = mBetweenBoardsTexPosRate;
	f32 ropeY    = mRopeWidthBetweenBoardsY;
	f32 dx       = step * (to.x - from.x);
	f32 dy       = step * (to.y - from.y);
	f32 dz       = step * (to.z - from.z);

	for (int i = 0; i < divide; i++) {
		f32 sag = y - mRopeSag[i];
		f32 tex = texRate * (x + z);
		GXPosition3f32(x, sag - ropeY, z);
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
	f32 texRate = mBetweenBoardsTexPosRate;
	f32 dx      = step * (to.x - from.x);
	f32 dy      = step * (to.y - from.y);
	f32 dz      = step * (to.z - from.z);

	for (int i = 0; i < divide; i++) {
		f32 sag = y - mRopeSag[i];
		f32 tex = texRate * (x + z);
		GXPosition3f32(x + width.x, sag, z + width.y);
		GXTexCoord2f32(0.0f, tex);
		GXPosition3f32(x - width.x, sag, z - width.y);
		GXTexCoord2f32(1.0f, tex);
		x += dx;
		y += dy;
		z += dz;
	}
}

// UNUSED (0x10): the board-position fetch drawRopeBetweenBoards expands.
void THangingBridge::setDrawPos(int i, f32 yOffset,
                                JGeometry::TVec3<f32>* out) const
{
	out->y += yOffset;
}

void THangingBridge::drawRopeBetweenBoards(f32 yOffset, int divide) const
{
	f32 offsetX = mSideDir.x * mRopeOffset;
	f32 offsetZ = mSideDir.y * mRopeOffset;

	JGeometry::TVec2<f32> width(mSideDir);
	width.scale(mRopeWidthBetweenBoards);

	int vertexNum = ((mBoardNum + 2) * divide * 2) & ~1;

	JGeometry::TVec3<f32> from;
	JGeometry::TVec3<f32> to;

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, vertexNum);
	from.set(mStart.x + offsetX, mStart.y + yOffset, mStart.z + offsetZ);
	for (int i = 0; i < mBoardNum; i++) {
		to = mBoards[i]->mRopeTop[0];
		to.y += yOffset;
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
		to = mBoards[i]->mRopeTop[0];
		to.y += yOffset;
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
		to = mBoards[i]->mRopeTop[0];
		to.y += yOffset;
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
		to = mBoards[i]->mRopeTop[1];
		to.y += yOffset;
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
		to = mBoards[i]->mRopeTop[1];
		to.y += yOffset;
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
		to = mBoards[i]->mRopeTop[1];
		to.y += yOffset;
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
	GXSetChanCtrl(GX_COLOR0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
	              GX_DF_NONE, GX_AF_NONE);
	GXSetChanCtrl(GX_ALPHA0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
	              GX_DF_NONE, GX_AF_NONE);
	GXColor color = { 0, 0, 100, 255 };
	GXSetChanMatColor(GX_COLOR0, color);
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
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
	               GX_LO_NOOP);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetCullMode(GX_CULL_BACK);
}

void THangingBridge::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & 8) {
		initDraw();

		for (int i = 0; i < mBoardNum; i++) {
			THangingBridgeBoard* board = mBoards[i];
			JGeometry::TVec3<f32> top;
			top = board->mRopeTop[0];
			board->drawOneRope(top);
			top = board->mRopeTop[1];
			board->drawOneRope(top);
		}

		if (gpMarDirector->mMap == 0xD)
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
	f32 dirZ = mSideDir.y;
	f32 dirX = mSideDir.x;
	mSideDir.x = dirX * cosQ - dirZ * sinQ;
	mSideDir.y = dirX * sinQ + dirZ * cosQ;

	mBoards = new THangingBridgeBoard*[mBoardNum];
	for (int i = 0; i < mBoardNum; i++) {
		f32 t = (f32)i / (f32)(mBoardNum - 1);
		JGeometry::TVec3<f32> pos;
		pos.x = t * (mEnd.x - mStart.x) + mStart.x;
		sinf(3.14f * t);
		pos.y = (t * (mEnd.y - mStart.y) + mStart.y) - 0.0f;
		pos.z = t * (mEnd.z - mStart.z) + mStart.z;

		JGeometry::TVec3<f32> rot(15.0f, pitch, 0.0f);
		if (gpMarDirector->mMap == 8) {
			mBoards[i] = (THangingBridgeBoard*)
			    TMapObjManager::newAndRegisterObj(
			        "PinnaHangingBridgeBoard", pos, rot,
			        JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
		} else {
			mBoards[i] = (THangingBridgeBoard*)
			    TMapObjManager::newAndRegisterObj(
			        "HangingBridgeBoard", pos, rot,
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
	f32 t     = 0.0f;
	f32 tStep = 1.0f / mPointNumBetweenBoards;
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
	f32 texPos = mRopeLength * mTexPosRate;

	f32 topXPlus    = top.x + mRopeWidthX;
	f32 bottomXPlus = bottom.x + mRopeWidthX;
	f32 topZPlus    = top.z + mRopeWidthZ;
	f32 topZMinus   = top.z - mRopeWidthZ;
	f32 bottomZPlus = bottom.z + mRopeWidthZ;
	f32 bottomZMinus = bottom.z - mRopeWidthZ;

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 8);
	GXPosition3f32(bottom.x, bottom.y, bottomZPlus);
	GXTexCoord2f32(0.0f, texPos);
	GXPosition3f32(top.x, top.y, topZPlus);
	GXTexCoord2f32(0.0f, 0.0f);

	GXPosition3f32(bottomXPlus, bottom.y, bottomZMinus);
	GXTexCoord2f32(1.0f, texPos);
	GXPosition3f32(topXPlus, top.y, topZMinus);
	GXTexCoord2f32(1.0f, 0.0f);

	GXPosition3f32(bottom.x - mRopeWidthX, bottom.y, bottomZMinus);
	GXTexCoord2f32(2.0f, texPos);
	GXPosition3f32(top.x - mRopeWidthX, top.y, topZMinus);
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
	GXSetChanCtrl(GX_COLOR0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
	              GX_DF_NONE, GX_AF_NONE);
	GXSetChanCtrl(GX_ALPHA0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
	              GX_DF_NONE, GX_AF_NONE);
	GXColor color = { 0, 0, 100, 255 };
	GXSetChanMatColor(GX_COLOR0, color);
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
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
	               GX_LO_NOOP);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetCullMode(GX_CULL_BACK);
}

void TSwingBoard::draw() const
{
	initDraw();

	MtxPtr mtx = getModel()->getAnmMtx(0);

	JGeometry::TVec3<f32> bottom;
	JGeometry::TVec3<f32> top;

	f32 width = mBoardWidth;
	top.x     = width * mtx[0][0] + mInitialPosition.x;
	top.y     = mRopeLength + mInitialPosition.y;
	top.z     = width * mtx[2][0] + mInitialPosition.z;
	bottom.x  = width * mtx[0][0] + mPosition.x;
	bottom.y  = 60.0f + mPosition.y;
	bottom.z  = width * mtx[2][0] + mPosition.z;
	drawOneRope(bottom, top);

	width    = mBoardWidth;
	top.x    = mInitialPosition.x - width * mtx[0][0];
	top.z    = mInitialPosition.z - width * mtx[2][0];
	bottom.x = mPosition.x - width * mtx[0][0];
	bottom.z = mPosition.z - width * mtx[2][0];
	drawOneRope(bottom, top);
}

// UNUSED (0xa8): the water-jet push at the head of control.
void TSwingBoard::swing()
{
	if (marioIsOn() && marioIsOn()
	    && SMS_GetMarioWaterGun()->mIsEmitWater) {
		MtxPtr emit = SMS_GetMarioWaterGun()->getEmitMtx(0);
		f32 dirX    = -emit[0][0];
		f32 dirZ    = -emit[2][0];
		MtxPtr mtx  = getModel()->getAnmMtx(0);
		mAngleSpeed += mAccelRate
		    * (mtx[0][2] * dirX + mtx[1][2] * 0.0f + mtx[2][2] * dirZ);
	}
}

void TSwingBoard::control()
{
	TMapObjBase::control();

	if (marioIsOn() && marioIsOn()
	    && SMS_GetMarioWaterGun()->mIsEmitWater) {
		MtxPtr emit = SMS_GetMarioWaterGun()->getEmitMtx(0);
		f32 dirX    = -emit[0][0];
		f32 dirZ    = -emit[2][0];
		MtxPtr mtx  = getModel()->getAnmMtx(0);
		mAngleSpeed += mAccelRate
		    * (mtx[0][2] * dirX + mtx[1][2] * 0.0f + mtx[2][2] * dirZ);
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
			f32 volume = fabsf(mAngle);
			gpMSound->startSoundActorWithInfo(MSD_SE_OBJ_SWING1,
			                                  &mPosition, nullptr, volume, 0,
			                                  0, &mSound, 0, 4);
		} else {
			f32 volume = fabsf(mAngle);
			gpMSound->startSoundActorWithInfo(MSD_SE_OBJ_SWING2,
			                                  &mPosition, nullptr, volume, 0,
			                                  0, &mSound, 0, 4);
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

	mAngleSpeedMax = 0.05f * ((1.0f + MsRandF()) * 0.5f);
	mAngle         = 20.0f * (MsRandF() - 0.5f);
	if (mAngle > 0.0f)
		mAngleSpeed = mAngleSpeedMax * -MsRandF();
	else
		mAngleSpeed = mAngleSpeedMax * MsRandF();

	s16 yaw   = (s16)(182.04445f * mRotation.y);
	f32 sinY  = JMASSin(yaw);
	f32 cosY  = JMASCos(yaw);
	mBaseMtx[0][0] = cosY;
	mBaseMtx[0][1] = 0.0f;
	mBaseMtx[0][2] = sinY;
	mBaseMtx[0][3] = 0.0f;
	mBaseMtx[1][0] = 0.0f;
	mBaseMtx[1][1] = 1.0f;
	mBaseMtx[1][2] = 0.0f;
	mBaseMtx[1][3] = 0.0f;
	mBaseMtx[2][0] = -sinY;
	mBaseMtx[2][1] = 0.0f;
	mBaseMtx[2][2] = cosY;
	mBaseMtx[2][3] = 0.0f;
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

	mBaseMtx[0][0] = 1.0f;
	mBaseMtx[0][1] = 0.0f;
	mBaseMtx[0][2] = 0.0f;
	mBaseMtx[0][3] = 0.0f;
	mBaseMtx[1][0] = 0.0f;
	mBaseMtx[1][1] = 1.0f;
	mBaseMtx[1][2] = 0.0f;
	mBaseMtx[1][3] = 0.0f;
	mBaseMtx[2][0] = 0.0f;
	mBaseMtx[2][1] = 0.0f;
	mBaseMtx[2][2] = 1.0f;
	mBaseMtx[2][3] = 0.0f;

	mAnchor.zero();
}

void TGoalFlag::touchActor(THitActor* actor)
{
	if (actor->isActorType(0x80000001)) {
		if (!TFlagManager::getInstance()->getBool(0x50005))
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

// TODO: 87.4%. Load order around the wind vector and the swing term still
// differs; the wind is probably read through a named local in the original.
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

	mDrift.x += mVelocity.x;
	mDrift.y += mVelocity.y;
	mDrift.z += mVelocity.z;

	f32 down = mSpeedDownRate;
	mVelocity.x *= down;
	mVelocity.y *= down;
	mVelocity.z *= down;

	f32 swing   = mSwingRadius * sinf(3.14f * (mSwingAngle / 180.0f));
	mPosition.x = mDrift.x + (swing * (mSwingCos + mSwingSin)
	                          + mInitialPosition.x);
	mPosition.y += mWindRate * gpMapObjManager->unkD0.y;
	mPosition.z = mDrift.z + (swing * (mSwingSin - mSwingCos)
	                          + mInitialPosition.z);

	JGeometry::TVec3<f32> wind(gpMapObjManager->unkD0);
	if (wind.isZero()) {
		mSwingAngle += mSwingAngleSpeed;
		if (mSwingAngle > 360.0f)
			mSwingAngle -= 360.0f;
	}

	if (mHeldObject && mHeldObject->isActorType(0x80000001))
		gpMarioPos->y -= mFallSpeed;
}

void TFluff::kill()
{
	if (mHeldObject) {
		mHeldObject->receiveMessage(this, 8);
		mHeldObject->mHolder = nullptr;
		mHeldObject          = nullptr;
	}

	mState = STATE_VANISHING;
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

	case STATE_FLYING:
		mGroundHeight = gpMap->checkGround(mPosition, &mGroundPlane);
		if (mVelocity.y < 0.0f
		    && (mGroundHeight > mPosition.y - mFallSpeed
		        || mPosition.y < -1000.0f))
			kill();

		if (gpMap->isTouchedOneWall(mPosition.x, mPosition.y, mPosition.z,
		                           100.0f))
			kill();

		if (mPosition.x < -14848.0f || mPosition.x > 14848.0f
		    || mPosition.z < -19968.0f || mPosition.z > 19968.0f)
			kill();
		break;

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
			mInitialRotation.set(mRotation);
			mIsRideable = false;
			if (!mManager->mNextFluff) {
				mManager->mNextFluff = this;
				mManager->mNextFluff->makeObjDead();
			}
		}
		break;
	}
}

// TODO: 89.9%. The three MsRandF draws are evaluated in the right order but
// the manager pointer is re-read where retail keeps it.
void TFluff::appear()
{
	makeObjAppeared();

	mPosition.set(mManager->getRandomX(),
	              mManager->mPosition.y * MsRandF(),
	              mManager->getRandomZ());
	mInitialPosition.set(mPosition);

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

// UNUSED (0x118): the search control's STATE_WAIT branch expands.
void TFluffManager::findNextFluff()
{
	for (int i = 3; i < mFluffMax; i++) {
		if (!mFluffs[i]->mIsRideable && !mFluffs[i]->mHeldObject
		    && mFluffs[i]->getDistance(*gpMarioPos) > 3000.0f) {
			mNextFluff = mFluffs[i];
			mFluffs[i]->kill();
			return;
		}
	}
}

// TODO: 70.3%. The instruction stream is close but the frame is 0x68 short
// and the search loop's float registers are renumbered; the missing locals
// are somewhere in the STATE_CALM hand-off.
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

	case STATE_BLOW:
		gpMapObjManager->unkD0.x += mWind.x;
		gpMapObjManager->unkD0.y += mWind.y;
		gpMapObjManager->unkD0.z += mWind.z;
		if (!isStateTimerEngaged())
			mState = STATE_CALM;
		break;

	case STATE_CALM: {
		f32 rate = mWindDownRate;
		f32 windX = gpMapObjManager->unkD0.x * rate;
		f32 windY = gpMapObjManager->unkD0.y * rate;
		f32 minWind = mWindMin;
		f32 windZ = gpMapObjManager->unkD0.z * rate;

		if (fabsf(windX) < minWind && fabsf(windY) < minWind
		    && fabsf(windZ) < minWind) {
			windX      = 0.0f;
			windY      = 0.0f;
			mRideFluff = mNextFluff;
			windZ      = 0.0f;

			mRideFluff->mRotation.set(mRotation);
			mRideFluff->mInitialRotation.set(mRotation);
			mRideFluff->appear();
			mRideFluff->mPosition.set(mPosition);
			mRideFluff->mInitialPosition.set(mPosition);
			mRideFluff->mRotation.set(0.0f, 0.0f, 0.0f);
			mRideFluff->mInitialRotation.set(mRideFluff->mRotation);
			mRideFluff->mSwingAngle = 0.0f;
			mRideFluff->mWindRate   = 1.0f;
			mRideFluff->mIsRideable = true;
			mNextFluff              = nullptr;
			mState                  = STATE_WAIT;
		}

		gpMapObjManager->unkD0.x = windX;
		gpMapObjManager->unkD0.y = windY;
		gpMapObjManager->unkD0.z = windZ;
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
	mRideFluff->mInitialPosition.set(getRandomX(), mPosition.y * MsRandF(),
	                                 getRandomZ());
	registerNextFluff(mRideFluff);

	mNextFluff = newFluff("２つ目のわた毛");
	setUpNextFluff();
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
	mWind.zero();
}
