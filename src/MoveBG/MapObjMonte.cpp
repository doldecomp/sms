#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JALibrary/JALSystem.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGeometry/JGUtil.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JStage/JSGActor.hpp>
#include <JSystem/JStage/JSGObject.hpp>
#include <JSystem/JSupport/JSUInputStream.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapCollisionManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjBlock.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjMonte.hpp>
#include <Player/WaterGun.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Yoshi.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/TakeActor.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <dolphin/gx.h>
#include <stdlib.h>
#include <math.h>

void TMapObjMonteRoot::initMapObj()
{
	TMapObjBase::initMapObj();
	setDamageHeight(1400.0f * mScaling.y);
	mPosition.y = mInitialPosition.y + mYOffset;
}

BOOL TJumpMushroom::receiveMessage(THitActor* param_1, u32 param_2)
{
	startAnim(1);
	return TRUE;
}

void TJumpMushroom::load(JSUMemoryInputStream& param_1)
{
	TMapObjBase::load(param_1);
	s32 data;
	param_1 >> data;
	if (mMapCollisionManager != nullptr)
		mMapCollisionManager->getActiveCollision()->setAllData(data);
}

f32 THangingBridgeBoard::mMarioAccelY        = 0.15f;
f32 THangingBridgeBoard::mMarioHipDropAccelY = 2.0f;
f32 THangingBridgeBoard::mReturnAccelRate    = 0.005f;
f32 THangingBridgeBoard::mSpeedDownRate      = 0.98f;
f32 THangingBridgeBoard::mRopeWidthX         = 10.0f;
f32 THangingBridgeBoard::mRopeWidthZ         = 7.0f;
f32 THangingBridgeBoard::mTexPosRate         = 0.01f;

void THangingBridgeBoard::drawOneRope(
    const JGeometry::TVec3<f32>& param_1) const
{
	f32 xPlus  = param_1.x + mRopeWidthX;
	f32 xMinus = param_1.x - mRopeWidthX;
	f32 zPlus  = param_1.z + mRopeWidthZ;
	f32 zMinus = param_1.z - mRopeWidthZ;

	f32 bottomY = param_1.y;
	f32 topY    = param_1.y + THangingBridge::mRopeHeight;

	f32 topTexCoordY    = mTexPosRate * (topY - param_1.y);
	f32 bottomTexCoordY = mTexPosRate * (bottomY - param_1.y);

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 8);
	GXPosition3f32(param_1.x, topY, zPlus);
	GXTexCoord2f32(0.0f, topTexCoordY);
	GXPosition3f32(param_1.x, bottomY, zPlus);
	GXTexCoord2f32(0.0f, bottomTexCoordY);
	GXPosition3f32(xMinus, topY, zMinus);
	GXTexCoord2f32(1.0f, topTexCoordY);
	GXPosition3f32(xMinus, bottomY, zMinus);
	GXTexCoord2f32(1.0f, bottomTexCoordY);
	GXPosition3f32(xPlus, topY, zMinus);
	GXTexCoord2f32(2.0f, topTexCoordY);
	GXPosition3f32(xPlus, bottomY, zMinus);
	GXTexCoord2f32(2.0f, bottomTexCoordY);
	GXPosition3f32(param_1.x, topY, zPlus);
	GXTexCoord2f32(3.0f, topTexCoordY);
	GXPosition3f32(param_1.x, bottomY, zPlus);
	GXTexCoord2f32(3.0f, bottomTexCoordY);
	GXEnd();
}

void THangingBridgeBoard::drawRopes() const
{
	JGeometry::TVec3<f32> pos = unk1A4[0];
	drawOneRope(pos);
	pos = unk1A4[1];
	drawOneRope(pos);
}

void THangingBridgeBoard::push(f32 param_1) { mVelocity.y -= param_1; }

void THangingBridgeBoard::pushNeighbor(f32 param_1)
{
	if (unk194 != nullptr) {
		unk194->push(param_1 * unk1BC->unk40);
		if (unk19C != nullptr)
			unk19C->push(param_1 * unk1BC->unk44);
	}
	if (unk198 != nullptr) {
		unk198->push(param_1 * unk1BC->unk40);
		if (unk1A0 != nullptr)
			unk1A0->push(param_1 * unk1BC->unk44);
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
	MtxPtr mtx  = getModel()->getAnmMtx(0);
	unk1A4[0].x = mPosition.x - mtx[0][0] * unk1BC->unk3C;
	unk1A4[0].y = 70.0f + (mPosition.y - mtx[1][0] * unk1BC->unk3C);
	unk1A4[0].z = mPosition.z - mtx[2][0] * unk1BC->unk3C;
	unk1A4[1].x = mPosition.x + mtx[0][0] * unk1BC->unk3C;
	unk1A4[1].y = 70.0f + (mPosition.y + mtx[1][0] * unk1BC->unk3C);
	unk1A4[1].z = mPosition.z + mtx[2][0] * unk1BC->unk3C;
}

void THangingBridgeBoard::calcDefaultMtx()
{
	Mtx rotX;
	Mtx rotY;
	makeRootMtxRotX(rotX);
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
		mMapCollisionManager->moveActiveCollisionMtx(mtx);
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

THangingBridgeBoard::THangingBridgeBoard(const char* param_1)
    : TLeanBlock(param_1)
{
	unk1BC = nullptr;
	unk194 = nullptr;
	unk198 = nullptr;
	unk19C = nullptr;
	unk1A0 = nullptr;
	unk1A4[0].zero();
	unk1A4[1].zero();
}

f32 THangingBridge::mRopeWidthBetweenBoards  = 10.0f;
f32 THangingBridge::mRopeWidthBetweenBoardsY = 10.0f;
int THangingBridge::mPointNumBetweenBoards   = 10;
f32 THangingBridge::mBetweenBoardsTexPosRate = 0.01f;
f32 THangingBridge::mRopeHeight;

void THangingBridge::drawLowerMinus(const JGeometry::TVec3<f32>& param_1,
                                    const JGeometry::TVec3<f32>& param_2,
                                    const JGeometry::TVec2<f32>& param_3,
                                    int param_4) const
{
	f32 rate  = 1.0f / param_4;
	f32 x     = param_1.x;
	f32 y     = param_1.y;
	f32 z     = param_1.z;
	f32 stepX = rate * (param_2.x - x);
	f32 stepY = rate * (param_2.y - y);
	f32 stepZ = rate * (param_2.z - z);
	for (int i = 0; i < param_4; i++) {
		f32 height = y - unk38[i];
		f32 texT   = mBetweenBoardsTexPosRate * (x + z);
		GXPosition3f32(x - param_3.x, height, z - param_3.y);
		GXTexCoord2f32(0.0f, texT);
		GXPosition3f32(x, height - mRopeWidthBetweenBoardsY, z);
		GXTexCoord2f32(1.0f, texT);
		x += stepX;
		y += stepY;
		z += stepZ;
	}
}

void THangingBridge::drawLowerPlus(const JGeometry::TVec3<f32>& param_1,
                                   const JGeometry::TVec3<f32>& param_2,
                                   const JGeometry::TVec2<f32>& param_3,
                                   int param_4) const
{
	f32 rate  = 1.0f / param_4;
	f32 x     = param_1.x;
	f32 y     = param_1.y;
	f32 z     = param_1.z;
	f32 stepX = rate * (param_2.x - x);
	f32 stepY = rate * (param_2.y - y);
	f32 stepZ = rate * (param_2.z - z);
	for (int i = 0; i < param_4; i++) {
		f32 height = y - unk38[i];
		f32 texT   = mBetweenBoardsTexPosRate * (x + z);
		GXPosition3f32(x, height - mRopeWidthBetweenBoardsY, z);
		GXTexCoord2f32(0.0f, texT);
		GXPosition3f32(x + param_3.x, height, z + param_3.y);
		GXTexCoord2f32(1.0f, texT);
		x += stepX;
		y += stepY;
		z += stepZ;
	}
}

void THangingBridge::drawUpper(const JGeometry::TVec3<f32>& param_1,
                               const JGeometry::TVec3<f32>& param_2,
                               const JGeometry::TVec2<f32>& param_3,
                               int param_4) const
{
	f32 rate = 1.0f / param_4;

	f32 x = param_1.x;
	f32 y = param_1.y;
	f32 z = param_1.z;

	f32 stepX = rate * (param_2.x - x);
	f32 stepY = rate * (param_2.y - y);
	f32 stepZ = rate * (param_2.z - z);

	for (int i = 0; i < param_4; i++) {
		f32 height = y - unk38[i];
		f32 texT   = mBetweenBoardsTexPosRate * (x + z);
		GXPosition3f32(x + param_3.x, height, z + param_3.y);
		GXTexCoord2f32(0.0f, texT);
		GXPosition3f32(x - param_3.x, height, z - param_3.y);
		GXTexCoord2f32(1.0f, texT);
		x += stepX;
		y += stepY;
		z += stepZ;
	}
}

void THangingBridge::setDrawPos(int board_index, f32 height,
                                JGeometry::TVec3<f32>* position) const
{
	position->y += height;
}

void THangingBridge::drawRopeBetweenBoards(f32 param_1, int param_2) const
{
	f32 xw = unk30.x * unk3C;
	f32 zw = unk30.y * unk3C;
	JGeometry::TVec2<f32> ropeWidth;
	ropeWidth.set(unk30);
	ropeWidth.scale(mRopeWidthBetweenBoards);
	JGeometry::TVec3<f32> prev;
	JGeometry::TVec3<f32> cur;
	u16 nverts = (unk10 + 2) * param_2 * 2;
	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, nverts);
	prev.set(unk18.x + xw, unk18.y + param_1, unk18.z + zw);
	for (int i = 0; i < unk10; i++) {
		cur = unk14[i]->unk1A4[0];
		setDrawPos(i, param_1, &cur);
		drawLowerMinus(prev, cur, ropeWidth, param_2);
		prev = cur;
	}
	cur.set(unk24.x + xw, unk24.y + param_1, unk24.z + zw);
	drawLowerMinus(prev, cur, ropeWidth, param_2);
	prev = cur;
	drawLowerMinus(prev, cur, ropeWidth, param_2);
	GXEnd();
	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, nverts);
	prev.set(unk18.x + xw, unk18.y + param_1, unk18.z + zw);
	for (int i = 0; i < unk10; i++) {
		cur = unk14[i]->unk1A4[0];
		setDrawPos(i, param_1, &cur);
		drawLowerPlus(prev, cur, ropeWidth, param_2);
		prev = cur;
	}
	cur.set(unk24.x + xw, unk24.y + param_1, unk24.z + zw);
	drawLowerPlus(prev, cur, ropeWidth, param_2);
	prev = cur;
	drawLowerPlus(prev, cur, ropeWidth, param_2);
	GXEnd();
	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, nverts);
	prev.set(unk18.x + xw, unk18.y + param_1, unk18.z + zw);
	for (int i = 0; i < unk10; i++) {
		cur = unk14[i]->unk1A4[0];
		setDrawPos(i, param_1, &cur);
		drawUpper(prev, cur, ropeWidth, param_2);
		prev = cur;
	}
	cur.set(unk24.x + xw, unk24.y + param_1, unk24.z + zw);
	drawUpper(prev, cur, ropeWidth, param_2);
	prev = cur;
	drawUpper(prev, cur, ropeWidth, param_2);
	GXEnd();
	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, nverts);
	prev.set(unk18.x - xw, unk18.y + param_1, unk18.z - zw);
	for (int i = 0; i < unk10; i++) {
		cur = unk14[i]->unk1A4[1];
		setDrawPos(i, param_1, &cur);
		drawLowerMinus(prev, cur, ropeWidth, param_2);
		prev = cur;
	}
	cur.set(unk24.x - xw, unk24.y + param_1, unk24.z - zw);
	drawLowerMinus(prev, cur, ropeWidth, param_2);
	prev = cur;
	drawLowerMinus(prev, cur, ropeWidth, param_2);
	GXEnd();
	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, nverts);
	prev.set(unk18.x - xw, unk18.y + param_1, unk18.z - zw);
	for (int i = 0; i < unk10; i++) {
		cur = unk14[i]->unk1A4[1];
		setDrawPos(i, param_1, &cur);
		drawLowerPlus(prev, cur, ropeWidth, param_2);
		prev = cur;
	}
	cur.set(unk24.x - xw, unk24.y + param_1, unk24.z - zw);
	drawLowerPlus(prev, cur, ropeWidth, param_2);
	prev = cur;
	drawLowerPlus(prev, cur, ropeWidth, param_2);
	GXEnd();
	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, nverts);
	prev.set(unk18.x - xw, unk18.y + param_1, unk18.z - zw);
	for (int i = 0; i < unk10; i++) {
		cur = unk14[i]->unk1A4[1];
		setDrawPos(i, param_1, &cur);
		drawUpper(prev, cur, ropeWidth, param_2);
		prev = cur;
	}
	cur.set(unk24.x - xw, unk24.y + param_1, unk24.z - zw);
	drawUpper(prev, cur, ropeWidth, param_2);
	prev = cur;
	drawUpper(prev, cur, ropeWidth, param_2);
	GXEnd();
}

void THangingBridge::initDraw() const
{
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXLoadPosMtxImm(j3dSys.getViewMtx(), GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0x00, 0x00, 0x64, 0xff });
	GXSetNumTexGens(1);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
	                  GX_FALSE, GX_PTIDENTITY);
	if (SMSGetMarDirector()->getCurrentMap() == 13) {
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

void THangingBridge::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & CUE_DRAW) {
		initDraw();
		for (int i = 0; i < unk10; i++)
			unk14[i]->drawRopes();
		drawRopeBetweenBoards(0.0f, mPointNumBetweenBoards);
		drawRopeBetweenBoards(mRopeHeight, 1);
	}
}

void THangingBridge::initMonte()
{
	struct BoardPosition {
		f32 x, y, z, rotation;
	};
	const BoardPosition positions[21] = {
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
	for (int i = 0; i < unk10; ++i) {
		if (positions[i].x == -1.0f)
			break;
		if (positions[i].x != -1.0f) {
			unk14[i]->mInitialPosition.set(positions[i].x, positions[i].y,
			                               positions[i].z);
			unk14[i]->mPosition.set(unk14[i]->mInitialPosition);
			unk14[i]->mRotation.x = positions[i].rotation;
			unk14[i]->calcDefaultMtx();
		}
	}
}

void THangingBridge::loadAfter()
{
	JDrama::TViewObj::loadAfter();
	f32 rotY = 0.0f;
	if (SMSGetMarDirector()->getCurrentMap() == 13) {
		unk10 = 14;
		unk18.set(1550.0f, 2980.0f, -9410.0f);
		unk24.set(3570.0f, 2455.0f, -9410.0f);
		mRopeHeight = 200.0f;
		rotY        = 90.0f;
		unk40       = 0.8f;
		unk44       = 0.5f;
		unk3C       = 150.0f;
	} else if (SMSGetMarDirector()->getCurrentMap() == 8) {
		unk10 = 19;
		unk18.set(0.0f, 0.0f, 11356.0f);
		unk24.set(0.0f, -750.0f, 17743.0f);
		mRopeHeight = 1000.0f;
		unk40       = 1.0f;
		unk44       = 0.5f;
		unk3C       = 315.0f;
	}
	unk30.set(unk24.x - unk18.x, unk24.z - unk18.z);
	unk30.normalize();
	unk30.rotate(1.5707964f);
	unk14 = new THangingBridgeBoard*[unk10];
	for (int i = 0; i < unk10; ++i) {
		f32 t = (f32)i / (f32)(unk10 - 1);
		JGeometry::TVec3<f32> pos;
		pos.x    = t * (unk24.x - unk18.x) + unk18.x;
		f32 wave = 0.0f * sinf(3.14f * t);
		pos.y    = t * (unk24.y - unk18.y) + unk18.y - wave;
		pos.z    = t * (unk24.z - unk18.z) + unk18.z;
		JGeometry::TVec3<f32> rot(15.0f, rotY, 0.0f);
		if (SMSGetMarDirector()->getCurrentMap() == 8)
			unk14[i]
			    = (THangingBridgeBoard*)TMapObjBaseManager::newAndRegisterObj(
			        "HangingBridgeBoard", pos, rot,
			        JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
		else
			unk14[i]
			    = (THangingBridgeBoard*)TMapObjBaseManager::newAndRegisterObj(
			        "PinnaHangingBridgeBoard", pos, rot,
			        JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
		unk14[i]->unk1BC = this;
		unk14[i]->appear();
	}
	if (SMSGetMarDirector()->getCurrentMap() == 8)
		initMonte();
	if (SMSGetMarDirector()->getCurrentMap() == 13) {
		unk18.set(1350.0f, 2980.0f, -9410.0f);
		unk24.set(3650.0f, 2455.0f, -9410.0f);
	}
	for (int i = 0; i < unk10; ++i) {
		if (i > 0)
			unk14[i]->unk194 = unk14[i - 1];
		if (i > 1)
			unk14[i]->unk19C = unk14[i - 2];
		if (i < unk10 - 1)
			unk14[i]->unk198 = unk14[i + 1];
		if (i < unk10 - 2)
			unk14[i]->unk1A0 = unk14[i + 2];
	}
	unk38     = new f32[mPointNumBetweenBoards];
	f32 step  = 1.0f / mPointNumBetweenBoards;
	f32 angle = 0.0f;
	for (int i = 0; i < mPointNumBetweenBoards; ++i) {
		unk38[i] = 50.0f * sinf(3.14f * angle);
		angle += step;
	}
}

THangingBridge::THangingBridge(const char* param_1)
    : JDrama::TViewObj(param_1)
    , unk10(0)
    , unk14(nullptr)
    , unk38(nullptr)
    , unk3C(0.0f)
    , unk40(0.0f)
    , unk44(0.0f)
{
}

f32 TSwingBoard::mBoardWidth      = 315.0f;
f32 TSwingBoard::mRopeWidthX      = 10.0f;
f32 TSwingBoard::mRopeWidthZ      = 7.0f;
f32 TSwingBoard::mTexPosRate      = 0.01f;
f32 TSwingBoard::mReturnAccelRate = 0.0001f;
f32 TSwingBoard::mSpeedDownRate   = 0.998f;

void TSwingBoard::drawOneRope(const JGeometry::TVec3<f32>& param_1,
                              const JGeometry::TVec3<f32>& param_2) const
{
	f32 xPlusB  = param_2.x + mRopeWidthX;
	f32 xMinusB = param_2.x - mRopeWidthX;
	f32 zPlusB  = param_2.z + mRopeWidthZ;
	f32 zMinusB = param_2.z - mRopeWidthZ;

	f32 xPlusA  = param_1.x + mRopeWidthX;
	f32 xMinusA = param_1.x - mRopeWidthX;
	f32 zPlusA  = param_1.z + mRopeWidthZ;
	f32 zMinusA = param_1.z - mRopeWidthZ;

	f32 texCoordY = unk138 * mTexPosRate;

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 8);
	GXPosition3f32(param_1.x, param_1.y, zPlusA);
	GXTexCoord2f32(0.0f, texCoordY);
	GXPosition3f32(param_2.x, param_2.y, zPlusB);
	GXTexCoord2f32(0.0f, 0.0f);
	GXPosition3f32(xPlusA, param_1.y, zMinusA);
	GXTexCoord2f32(1.0f, texCoordY);
	GXPosition3f32(xPlusB, param_2.y, zMinusB);
	GXTexCoord2f32(1.0f, 0.0f);
	GXPosition3f32(xMinusA, param_1.y, zMinusA);
	GXTexCoord2f32(2.0f, texCoordY);
	GXPosition3f32(xMinusB, param_2.y, zMinusB);
	GXTexCoord2f32(2.0f, 0.0f);
	GXPosition3f32(param_1.x, param_1.y, zPlusA);
	GXTexCoord2f32(3.0f, texCoordY);
	GXPosition3f32(param_2.x, param_2.y, zPlusB);
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
	GXLoadPosMtxImm(j3dSys.getViewMtx(), GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0x00, 0x00, 0x64, 0xff });
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

void TSwingBoard::draw() const
{
	initDraw();
	MtxPtr mtx = getModel()->getAnmMtx(0);
	JGeometry::TVec3<f32> bottom;
	JGeometry::TVec3<f32> top;
	bottom.x = mBoardWidth * mtx[0][0] + mInitialPosition.x;
	bottom.y = unk138 + mInitialPosition.y;
	bottom.z = mBoardWidth * mtx[2][0] + mInitialPosition.z;
	top.x    = mBoardWidth * mtx[0][0] + mPosition.x;
	top.y    = 60.0f + mPosition.y;
	top.z    = mBoardWidth * mtx[2][0] + mPosition.z;
	drawOneRope(top, bottom);
	bottom.x = mInitialPosition.x - mBoardWidth * mtx[0][0];
	bottom.z = mInitialPosition.z - mBoardWidth * mtx[2][0];
	top.x    = mPosition.x - mBoardWidth * mtx[0][0];
	top.z    = mPosition.z - mBoardWidth * mtx[2][0];
	drawOneRope(top, bottom);
}

void TSwingBoard::swing()
{
	if (marioIsOn() && SMS_GetMarioWaterGun()->mIsEmitWater != 0) {
		MtxPtr emitMtx = SMS_GetMarioWaterGun()->getEmitMtx(0);
		JGeometry::TVec3<f32> direction(-emitMtx[0][0], 0.0f, -emitMtx[2][0]);
		MtxPtr mtx = getModel()->getAnmMtx(0);
		unk144 += unk140
		          * (mtx[0][2] * direction.x + mtx[1][2] * direction.y
		             + mtx[2][2] * direction.z);
	}
}

void TSwingBoard::control()
{
	TMapObjBase::control();
	if (marioIsOn())
		swing();
	unk13C += unk144;
	f32 oldSpeed = unk144;
	unk144 -= unk13C * mReturnAccelRate;
	if (fabsf(unk144) > unk148)
		unk144 *= mSpeedDownRate;
	if (oldSpeed * unk144 <= 0.0f) {
		if (unk188 != nullptr)
			unk188->stop(1);
		if (unk144 > 0.0f)
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_OBJ_SWING1, &mPosition, nullptr, fabsf(unk13C), 0, 0,
			    &unk188, 0, 4);
		else
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_OBJ_SWING2, &mPosition, nullptr, fabsf(unk13C), 0, 0,
			    &unk188, 0, 4);
	}
	mRotation.x = -unk13C;
	Mtx rot;
	f32 sine   = sinf(3.14f * (mRotation.x / 180.0f));
	f32 cosine = cosf(3.14f * (mRotation.x / 180.0f));
	rot[0][0]  = 1.0f;
	rot[0][1]  = 0.0f;
	rot[0][2]  = 0.0f;
	rot[0][3]  = 0.0f;
	rot[1][0]  = 0.0f;
	rot[1][1]  = cosine;
	rot[1][2]  = -sine;
	rot[1][3]  = 0.0f;
	rot[2][0]  = 0.0f;
	rot[2][1]  = sine;
	rot[2][2]  = cosine;
	rot[2][3]  = 0.0f;
	MtxPtr mtx = getModel()->getAnmMtx(0);
	MTXConcat(unk14C, rot, mtx);
	mPosition.x = mInitialPosition.x - mtx[0][1] * unk138;
	mPosition.y = unk138 + mInitialPosition.y - mtx[1][1] * unk138;
	mPosition.z = mInitialPosition.z - mtx[2][1] * unk138;
	mtx[0][3]   = mPosition.x;
	mtx[1][3]   = mPosition.y;
	mtx[2][3]   = mPosition.z;
}

void TSwingBoard::load(JSUMemoryInputStream& param_1)
{
	TMapObjBase::load(param_1);

	param_1 >> unk138;
	if (unk138 == -1.0f)
		unk138 = 5000.0f;

	param_1 >> unk140;
	if (unk140 > 10.0f || unk140 == 0.0f)
		unk140 = 0.003f;

	unk17C.set(mPosition.x, mPosition.y + unk138, mPosition.z);

	unk148 = 0.05f * ((1.0f + MsRandF()) / 2.0f);
	unk13C = 20.0f * (MsRandF() - 0.5f);

	if (unk13C > 0.0f) {
		unk144 = unk148 * -MsRandF();
	} else {
		unk144 = unk148 * MsRandF();
	}

	MsMtxSetRotY(unk14C, mRotation.y);
}

TSwingBoard::TSwingBoard(const char* param_1)
    : TMapObjBase(param_1)
{
	unk138 = 5000.0f;
	unk13C = 0.0f;
	unk140 = 0.0f;
	unk144 = 0.0f;
	unk148 = 0.0f;
	unk188 = nullptr;
	unk14C.identity();
	unk17C.zero();
}

void TGoalFlag::touchActor(THitActor* param_1)
{
	if (param_1->isActorType(ACTOR_TYPE_MARIO)) {
		if (!TFlagManager::smInstance->getBool(MSF_RACE_GOAL_REACHED))
			TFlagManager::smInstance->setBool(true, MSF_RACE_GOAL_REACHED);
		param_1->receiveMessage(this, HIT_MESSAGE_ATTACK);
	} else if (param_1->isActorType(ACTOR_TYPE_E_MARIO)) {
		param_1->receiveMessage(this, HIT_MESSAGE_ATTACK);
	}
}

void TGoalFlag::initMapObj() { TMapObjBase::initMapObj(); }

f32 TFluff::mScaleUpSpeed   = 0.05f;
f32 TFluff::mScaleDownSpeed = 0.01f;

u32 TFluff::touchWater(THitActor* param_1)
{
	const JGeometry::TVec3<f32>& waterPos = getWaterPos(param_1);
	JGeometry::TVec3<f32> normal;
	getNormalVecFromTarget(waterPos.x, waterPos.y, waterPos.z, &normal);
	mVelocity.x -= normal.x * unk160;
	mVelocity.y -= normal.y * unk160;
	mVelocity.z -= normal.z * unk160;
	return 1;
}

void TFluff::move()
{
	mPosition.y -= unk13C;
	if (mPosition.y < 0.0f)
		mPosition.y = 5000.0f;
	unk154 += unk150 * gpMapObjManager->unkD0.x;
	unk15C += unk150 * gpMapObjManager->unkD0.z;
	unk154 += mVelocity.x;
	unk158 += mVelocity.y;
	unk15C += mVelocity.z;
	mVelocity *= unk164;
	f32 wave    = unk138 * sinf(3.14f * unk148 / 180.0f);
	mPosition.x = mInitialPosition.x + wave * (unk144 + unk140) + unk154;
	mPosition.y += unk150 * gpMapObjManager->unkD0.y;
	mPosition.z = mInitialPosition.z + wave * (unk140 - unk144) + unk15C;
	if (JGeometry::TVec3<f32>(gpMapObjManager->unkD0).isZero()) {
		unk148 += unk14C;
		if (unk148 > 360.0f)
			unk148 -= 360.0f;
	}
	if (mHeldObject != nullptr && mHeldObject->isActorType(ACTOR_TYPE_MARIO))
		SMS_GetMarioPos().y -= unk13C;
}

void TFluff::kill()
{
	if (mHeldObject != nullptr) {
		mHeldObject->receiveMessage(this, HIT_MESSAGE_DETACH);
		mHeldObject = nullptr;
	}
	mState = STATE_UNK3;
}

void TFluff::control()
{
	TMapObjBase::control();
	move();
	switch (mState) {
	case STATE_UNK2:
		mScaling.x += mScaleUpSpeed;
		mScaling.y += mScaleUpSpeed;
		mScaling.z += mScaleUpSpeed;
		if (mScaling.x > 1.0f) {
			mScaling.setAll(1.0f);
			setObjHitData(0);
			mState = STATE_NORMAL;
		}
		break;
	case STATE_NORMAL: {
		mGroundHeight = gpMap->checkGround(mPosition, &mGroundPlane);
		if (mVelocity.y < 0.0f
		    && (mGroundHeight > mPosition.y - unk13C || mPosition.y < -1000.0f))
			kill();
		if (gpMap->isTouchedOneWall(mPosition.x, mPosition.y, mPosition.z,
		                            100.0f))
			kill();
		if (mPosition.x < -14848.0f || 14848.0f < mPosition.x
		    || mPosition.z < -19968.0f || 19968.0f < mPosition.z)
			kill();
		break;
	}
	case STATE_UNK3:
		mScaling.x -= mScaleDownSpeed;
		mScaling.y -= mScaleDownSpeed;
		mScaling.z -= mScaleDownSpeed;
		if (mScaling.x < 0.1f) {
			gpMarioParticleManager->emitAndBindToPosPtr(
			    PARTICLE_MS_ENM_DISAP_A_W, &mPosition, 0, nullptr);
			SMSGetMSound()->startSoundActor(MSD_SE_SMOKE_EFFECT, &mPosition, 0,
			                                nullptr, 0, 4);
			mScaling.setAll(0.0001f);
			mStateTimer = 240;
			mState      = STATE_UNK4;
		}
		break;
	case STATE_UNK4:
		if (!isStateTimerEngaged()) {
			appear();
			mRotation.x      = 0.0f;
			mRotation.y      = 360.0f * MsRandF();
			mRotation.z      = 0.0f;
			mInitialRotation = mRotation;
			unk16C           = 0;
			unk168->registerNextFluff(this);
		}
		break;
	}
}

void TFluff::appear()
{
	makeObjAppeared();
	mPosition.set(unk168->getRandomX(), unk168->mPosition.y * MsRandF(),
	              unk168->getRandomZ());
	mInitialPosition = mPosition;
	mScaling.setAll(0.0001f);
	unk15C = 0.0f;
	unk158 = 0.0f;
	unk154 = 0.0f;
	unk148 = 0.0f;
	unk150 = 0.8f * MsRandF() + 0.2f;
	unk140 = sinf(3.14f * mRotation.y / 180.0f);
	unk144 = cosf(3.14f * mRotation.y / 180.0f);
	unk148 = 360.0f * MsRandF();
	unk14C = 0.3f;
	mState = STATE_UNK2;
}

void TFluff::initMapObj()
{
	TMapObjBase::initMapObj();
	unk138 = 300.0f;
	unk13C = 0.5f;
}

TFluff::TFluff(const char* param_1)
    : TMapObjBase(param_1)
{
	unk138 = 0.0f;
	unk13C = 0.0f;
	unk140 = 0.0f;
	unk144 = 0.0f;
	unk148 = 0.0f;
	unk14C = 0.0f;
	unk150 = 0.0f;
	unk160 = 1.0f;
	unk164 = 0.95f;
	unk168 = nullptr;
	unk16C = 0;
	unk15C = 0.0f;
	unk158 = 0.0f;
	unk154 = 0.0f;
}

f32 TFluffManager::mWindMin = 1.0f;

void TFluffManager::findNextFluff()
{
	for (int i = 3; i < unk164; ++i) {
		if (unk168[i]->unk16C == 0 && unk168[i]->getHeldObject() == nullptr
		    && unk168[i]->mPosition.distance(SMS_GetMarioPos()) > 3000.0f) {
			unk15C = unk168[i];
			unk168[i]->kill();
			break;
		}
	}
}

void TFluffManager::control()
{
	switch (mState) {
	case STATE_NORMAL:
		if (unk15C == nullptr
		    && unk158->mPosition.y - 100.0f < mPosition.y - unk140)
			findNextFluff();
		if (unk158->mPosition.y < mPosition.y - unk140) {
			SMSGetMSound()->startSoundActor(
			    MSD_SE_OBJ_WATAGE_WIND, &unk158->mPosition, 0, nullptr, 0, 4);
			mStateTimer = unk144;
			mState      = STATE_UNK2;
		}
		break;
	case STATE_UNK2: {
		JGeometry::TVec3<f32> wind(gpMapObjManager->unkD0);
		wind += unk148;
		gpMapObjManager->unkD0.x = wind.x;
		gpMapObjManager->unkD0.y = wind.y;
		gpMapObjManager->unkD0.z = wind.z;
		if (!isStateTimerEngaged())
			mState = STATE_UNK3;
		break;
	}
	case STATE_UNK3: {
		JGeometry::TVec3<f32> wind(gpMapObjManager->unkD0);
		wind.scale(unk154);
		if (fabsf(wind.x) < mWindMin && fabsf(wind.y) < mWindMin
		    && fabsf(wind.z) < mWindMin) {
			wind.setAll(0.0f);
			setUpNextFluff();
			mState = STATE_NORMAL;
		}
		gpMapObjManager->unkD0.x = wind.x;
		gpMapObjManager->unkD0.y = wind.y;
		gpMapObjManager->unkD0.z = wind.z;
		break;
	}
	}
}

void TFluffManager::registerNextFluff(TFluff* param_1)
{
	if (unk15C == nullptr) {
		unk15C = param_1;
		unk15C->makeObjDead();
	}
}

void TFluffManager::setUpNextFluff()
{
	unk158 = unk15C;
	unk158->mRotation.set(mRotation);
	unk158->mInitialRotation = mRotation;
	unk158->appear();
	unk158->mPosition.set(mPosition);
	unk158->mInitialPosition = mPosition;
	unk158->mRotation.setAll(0.0f);
	unk158->mInitialRotation = unk158->mRotation;
	unk158->unk148           = 0.0f;
	unk158->unk150           = 1.0f;
	unk158->unk16C           = 1;
	unk15C                   = nullptr;
}

TFluff* TFluffManager::newFluff(const char* param_1)
{
	TFluff* fluff = new TFluff(param_1);
	fluff->initAndRegister("Fluff");
	fluff->unk168 = this;
	return fluff;
}

f32 TFluffManager::getRandomX() const
{
	return unk138 * (2.0f * MsRandF() - 1.0f);
}

f32 TFluffManager::getRandomZ() const
{
	return unk13C * (2.0f * MsRandF() - 1.0f);
}

void TFluffManager::loadAfter()
{
	unk160         = 0;
	unk164         = 32;
	unk168         = new TFluff*[unk164];
	unk158         = newFluff("１つ目のわた毛");
	unk158->unk16C = 1;
	unk158->appear();
	unk158->mPosition.set(mPosition);
	unk158->mRotation.set(mRotation);
	JGeometry::TVec3<f32> position(getRandomX(), mPosition.y * MsRandF(),
	                               getRandomZ());
	unk158->mInitialPosition = position;
	unk168[unk160]           = unk158;
	++unk160;
	unk15C = newFluff("２つ目のわた毛");
	unk15C->mPosition.set(mPosition);
	unk15C->mRotation.set(mRotation);
	position.set(getRandomX(), mPosition.y * MsRandF(), getRandomZ());
	unk15C->mInitialPosition = position;
	unk15C->makeObjDead();
	unk168[unk160] = unk15C;
	++unk160;
	for (int i = 2; i < unk164; ++i) {
		unk168[unk160] = newFluff("わた毛");
		unk168[unk160]->appear();
		++unk160;
	}
}

void TFluffManager::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);
	stream >> unk140;
	f32 scale;
	stream >> scale;
	scale *= 0.01f;
	stream >> unk144;
	unk138 = 5000.0f;
	unk13C = 5000.0f;
	unk154 = 0.998f;
	TMtx34f mtx;
	MsMtxSetXYZRPH(mtx, 0.0f, 0.0f, 0.0f, mRotation.x, mRotation.y,
	               mRotation.z);
	unk148.set(0.0f, 0.0f, 1.0f);
	mtx.mult(unk148, unk148);
	unk148.scale(scale);
}

TFluffManager::TFluffManager(const char* param_1)
    : TMapObjBase(param_1)
{
	unk138   = 0.0f;
	unk13C   = 0.0f;
	unk140   = 0.0f;
	unk144   = 0;
	unk154   = 0.0f;
	unk158   = nullptr;
	unk15C   = nullptr;
	unk160   = 0;
	unk164   = 0;
	unk148.x = 0.0f;
	unk148.y = 0.0f;
	unk148.z = 0.0f;
}
