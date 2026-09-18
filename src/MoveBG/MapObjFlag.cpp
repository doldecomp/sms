#include <MoveBG/MapObjFlag.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <System/MarDirector.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JMath.hpp>
#include <dolphin/gx.h>
#include <stdio.h>
#include <string.h>

// rogue includes needed for matching sinit & bss
#include <System/DummyStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TMapObjFlagManager* gpMapObjFlagManager;

// UNUSED (0x170). TODO: a copy of the base body; nothing in the binary says
// how a "sail" differs, and the map's size is 0x38 larger than this shape.
void TMapObjFlagSail::updateVertex()
{
	for (int y = 0; y < mNumY; y += mSkip) {
		f32 rowPhase = (f32)y * mWavePhaseY;
		for (int z = 0; z < mNumZ; z += mSkip) {
			f32 rate  = (f32)z / (f32)mNumZ;
			f32 angle = mWaveAngle + (-z * mWavePhaseZ + rowPhase);
			angle     = MsWrap(angle, -180.0f, 180.0f);
			mVertices[y][z].x = mWaveAmplitude * rate * JMASin(angle);
		}
	}
}

// UNUSED (0x140). TODO: same as above; 8 bytes short of the map's size.
void TMapObjFlagLower::updateVertex()
{
	for (int y = 0; y < mNumY; y += mSkip) {
		f32 rowPhase = (f32)y * mWavePhaseY;
		for (int z = 0; z < mNumZ; z += mSkip) {
			f32 rate  = (f32)z / (f32)mNumZ;
			f32 angle = mWaveAngle + (-z * mWavePhaseZ + rowPhase);
			angle     = MsWrap(angle, -180.0f, 180.0f);
			mVertices[y][z].x = mWaveAmplitude * rate * JMASin(angle);
		}
	}
}

f32 TMapObjFlag::mFlutterSpeed = 4.0f;

void TMapObjFlag::draw()
{
	JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > mtx;
	mtx.set(j3dSys.getViewMtx());
	MTXConcat(mtx, mMtx, mtx);
	GXLoadPosMtxImm(mtx, GX_PNMTX0);

	// Two rows at a time, so every strip is (columns + 2) * 2 vertices wide.
	u16 vtxNum = ((mNumZ - mSkip * 2) / mSkip + 2) * 2;
	f32 invZ   = 1.0f / (f32)(mNumZ - 1);
	f32 invY   = 1.0f / (f32)(mNumY - 1);

	for (int y = 0; y < mNumY - mSkip; y += mSkip) {
		f32 t0 = invY * (f32)(mNumY - 1 - y);
		f32 t1 = invY * (f32)(mNumY - 1 - (y + 1));

		GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, vtxNum);

		GXPosition3f32(mVertices[y][0].x, mVertices[y][0].y,
		               mVertices[y][0].z);
		GXTexCoord2f32(0.0f, t0);
		GXPosition3f32(mVertices[y + 1][0].x, mVertices[y + 1][0].y,
		               mVertices[y + 1][0].z);
		GXTexCoord2f32(0.0f, t1);

		for (int z = 1; z < mNumZ - mSkip; z += mSkip) {
			f32 s = invZ * (f32)z;
			GXPosition3f32(mVertices[y][z].x, mVertices[y][z].y,
			               mVertices[y][z].z);
			GXTexCoord2f32(s, t0);
			GXPosition3f32(mVertices[y + 1][z].x, mVertices[y + 1][z].y,
			               mVertices[y + 1][z].z);
			GXTexCoord2f32(s, t1);
		}

		GXPosition3f32(mVertices[y][mNumZ - 1].x, mVertices[y][mNumZ - 1].y,
		               mVertices[y][mNumZ - 1].z);
		GXTexCoord2f32(1.0f, t0);
		GXPosition3f32(mVertices[y + 1][mNumZ - 1].x,
		               mVertices[y + 1][mNumZ - 1].y,
		               mVertices[y + 1][mNumZ - 1].z);
		GXTexCoord2f32(1.0f, t1);

		GXEnd();
	}
}

void TMapObjFlag::updateVertex()
{
	for (int y = 0; y < mNumY; y += mSkip) {
		f32 rowPhase = (f32)y * mWavePhaseY;
		for (int z = 0; z < mNumZ; z += mSkip) {
			f32 rate  = (f32)z / (f32)mNumZ;
			f32 angle = mWaveAngle + (-z * mWavePhaseZ + rowPhase);
			angle     = MsWrap(angle, -180.0f, 180.0f);
			mVertices[y][z].x = mWaveAmplitude * rate * JMASin(angle);
		}
	}
}

// UNUSED (0x114): the per-flag body of TMapObjFlagManager::perform's
// CUE_CALC_ANIM loop, inlined there.
void TMapObjFlag::update()
{
	MsMtxSetXYZRPH(mMtx, mPosition.x, mPosition.y, mPosition.z, mRotation.x,
	               mRotation.y, mRotation.z);
	updateVertex();

	mWaveAngle += mFlutterSpeed;
	if (mWaveAngle > 360.0f)
		mWaveAngle -= 360.0f;

	// Only the big flags are loud enough to be heard, and Delfino Plaza has
	// its own ambience.
	if (mScaling.y > 3.0f && mScaling.z > 3.0f && gpMarDirector->mMap != 3
	    && gpMSound->gateCheck(MSD_SE_OBJ_FLAG))
		MSoundSESystem::MSoundSE::startSoundActor(MSD_SE_OBJ_FLAG, &mPosition,
		                                          0, nullptr, 0, 4);
}

void TMapObjFlag::init(const char* name)
{
	mLength = 100.0f * getScaling().z;
	mHeight = 100.0f * getScaling().y;
	mWavePhaseZ /= mScaling.z;
	mWavePhaseY /= mScaling.y;
	mWaveAmplitude *= mScaling.z;

	mNumZ = (int)(mLength / 50.0f);
	mNumY = (int)(mHeight / 100.0f);
	if (mNumZ < 2)
		mNumZ = 3;
	if (mNumY < 2)
		mNumY = 3;

	MsMtxSetXYZRPH(mMtx, mPosition.x, mPosition.y, mPosition.z, mRotation.x,
	               mRotation.y, mRotation.z);

	f32 stepZ = mLength / (f32)mNumZ;
	f32 stepY = mHeight / (f32)mNumY;
	JKRHeap::sCurrentHeap->getTotalFreeSize();

	mVertices = new JGeometry::TVec3<f32>*[mNumY];
	for (int y = 0; y < mNumY; y++) {
		mVertices[y] = new JGeometry::TVec3<f32>[mNumZ];
		for (int z = 0; z < mNumZ; z++) {
			JGeometry::TVec3<f32>& vertex = mVertices[y][z];
			vertex.x                      = 0.0f;
			vertex.y                      = (f32)y * stepY;
			vertex.z                      = (f32)z * stepZ;
		}
	}

	static int total_use_size = 0;
	JKRHeap::sCurrentHeap->getTotalFreeSize();

	gpMapObjFlagManager->registerObj(this, name);
	initHitActor(0x4000000D, 1, 0, 0.0f, 0.0f, 0.0f, 0.0f);
}

void TMapObjFlag::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);

	char textureName[64];
	stream.readString(textureName, 64);
	init(textureName);
}

TMapObjFlag::TMapObjFlag(const char* name)
    : THitActor(name)
    , mLength(0.0f)
    , mHeight(0.0f)
    , mNumZ(0)
    , mNumY(0)
    , mVertices(nullptr)
    , mWavePhaseZ(125.0f)
    , mWavePhaseY(130.0f)
    , mWaveAmplitude(20.0f)
    , mWaveAngle(360.0f * MsRandF())
    , mSkip(1)
{
	mMtx.identity();
}

f32 TMapObjFlagManager::mDistNearMiddle = 5000.0f;
f32 TMapObjFlagManager::mDistMiddleFar  = 10000.0f;

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
	GXColor color = { 0xff, 0xff, 0xff, 0xff };
	GXSetChanMatColor(GX_COLOR0A0, JUtility::TColor(color));

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

void TMapObjFlagManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM) {
		for (int i = 0; i < FLAG_KIND_NUM; i++)
			for (int j = 0; j < mInfos[i].mNum; j++)
				mInfos[i].mFlags[j]->update();
	}

	if (cue & CUE_DRAW) {
		initDraw();
		for (int i = 0; i < FLAG_KIND_NUM; i++) {
			if (mInfos[i].mNum != 0) {
				JUTTexture texture(mInfos[i].mTimg);
				texture.load(GX_TEXMAP0);
				for (int j = 0; j < mInfos[i].mNum; j++)
					mInfos[i].mFlags[j]->draw();
			}
		}
	}
}

// UNUSED (0x80): the body every registerObj branch repeats, inlined there.
void TMapObjFlagManager::loadFlag(TMapObjFlagInfo* info, TMapObjFlag* flag,
                                  const char* name)
{
	if (!info->mTimg) {
		char path[64];
		snprintf(path, 64, "/scene/mapObj/%s.bti", name);
		info->mTimg = (const ResTIMG*)JKRFileLoader::getGlbResource(path);
	}

	info->mFlags[info->mNum] = flag;
	info->mNum++;
}

void TMapObjFlagManager::registerObj(TMapObjFlag* flag, const char* name)
{
	if (strcmp(name, "flagSun") == 0) {
		loadFlag(&mInfos[0], flag, name);
		return;
	}
	if (strcmp(name, "flagWhite") == 0) {
		loadFlag(&mInfos[1], flag, name);
		return;
	}
	if (strcmp(name, "flagRedsun") == 0) {
		loadFlag(&mInfos[2], flag, name);
		return;
	}
	if (strcmp(name, "flagMonte") == 0) {
		loadFlag(&mInfos[3], flag, name);
		return;
	}
	if (strcmp(name, "flagBird") == 0) {
		loadFlag(&mInfos[4], flag, name);
		return;
	}
	if (strcmp(name, "flagHigekuri") == 0) {
		loadFlag(&mInfos[5], flag, name);
		return;
	}
	if (strcmp(name, "flagBenvenuto") == 0) {
		loadFlag(&mInfos[6], flag, name);
		return;
	}
	if (strcmp(name, "flagDolpicDolphin") == 0) {
		loadFlag(&mInfos[7], flag, name);
		return;
	}
	if (strcmp(name, "flagDolSun") == 0) {
		loadFlag(&mInfos[8], flag, name);
		return;
	}
	if (strcmp(name, "flagDolSunWelcome") == 0) {
		loadFlag(&mInfos[9], flag, name);
		return;
	}
	if (strcmp(name, "flagBianco") == 0) {
		loadFlag(&mInfos[10], flag, name);
		return;
	}
	if (strcmp(name, "flagRiccoBuoy") == 0) {
		loadFlag(&mInfos[11], flag, name);
		return;
	}
	if (strcmp(name, "flagSailMonte") == 0) {
		loadFlag(&mInfos[12], flag, name);
		return;
	}
	if (strcmp(name, "MammaYacht00") == 0) {
		loadFlag(&mInfos[13], flag, name);
		return;
	}
	if (strcmp(name, "flagMare") == 0) {
		loadFlag(&mInfos[14], flag, name);
		return;
	}
}

void TMapObjFlagManager::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);

	char buffer[8];
	stream.readString(buffer, 8);

	switch (gpMarDirector->mMap) {
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

TMapObjFlagManager::TMapObjFlagManager(const char* name)
    : JDrama::TViewObj(name)
{
	gpMapObjFlagManager = this;
}
