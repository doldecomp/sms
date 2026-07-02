#include <Map/PollutionLayer.hpp>
#include <Map/PollutionManager.hpp>
#include <MoveBG/MapObjWave.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/ReinitGX.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/ResTIMG.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <dolphin/os/OSCache.h>
#include <dolphin/gx.h>
#include <stdio.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

int TPollutionLayer::mEffectTime   = 15;
f32 TPollutionLayerWave::mInterval = 300.0f;
u8 TPollutionLayerWave::mAlpha     = 0xE6;

void TPollutionLayerWave::initGX() const
{
	ReInitializeGX();
	GXLoadPosMtxImm(j3dSys.mViewMtx, GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0, 0, 0, mAlpha });

	JUTTexture texture(mPollutionImage);
	texture.load(GX_TEXMAP0);

	GXSetNumTexGens(1);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_RASC, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetCullMode(GX_CULL_NONE);
	GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_GREATER, 0);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetZCompLoc(GX_FALSE);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
}

void TPollutionLayerWave::draw() const
{
	u16 xCount   = (u16)((mMaxX - mMinX) / mInterval);
	f32 invXSize = 1.0f / (mMaxX - mMinX);
	f32 invZSize = 1.0f / (mMaxZ - mMinZ);

	for (f32 z = mMinZ; z < mMaxZ - mInterval; z += mInterval) {
		GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, xCount * 2);
		for (f32 x = mMinX; x < mMaxX - mInterval; x += mInterval) {
			f32 zNext = z + mInterval;

			f32 h1 = gpMapObjWave->getWaveHeight(x, z);
			GXPosition3f32(x, h1 - 10.0f, z);
			GXTexCoord2f32(invXSize * (x - mMinX), invZSize * (z - mMinZ));

			f32 h2 = gpMapObjWave->getWaveHeight(x, zNext);
			GXPosition3f32(x, h2 - 10.0f, zNext);
			GXTexCoord2f32(invXSize * (x - mMinX), invZSize * (zNext - mMinZ));
		}
	}
}

void TPollutionLayerWave::perform(u32 flags, JDrama::TGraphics*)
{
	if (flags & 8) {
		initGX();
		draw();
	}
}

ResTIMG* TPollutionLayerWave::getTexResource(const char* name)
{
	char buf[256];
	snprintf(buf, 256, "/scene/map/pollution/%s.bti", name);
	return (ResTIMG*)JKRFileLoader::getGlbResource(buf);
}

void TPollutionLayerWave::initJointModel(TJointModelManager* mgr,
                                         const char* name, MActorAnmData*)
{
	mManager = mgr;

	initPollutionTex(name);
}

void TPollutionLayerWallPlusZ::stamp(u16 type, f32 x, f32 y, f32 z, f32 s)
{
	if (!isInAreaSize(x, y, z, s))
		return;
	TPollutionPos* pos = &mPos;
	gpPollution->getCounterLayer().pushStampTask(
	    type, mIndexInParent, pos->worldToTexSize(s), getTexPosS(x),
	    getTexPosT(y), pos->worldToDepth(z));
}

void TPollutionLayerWallPlusZ::initLayerInfo(const TPollutionLayerInfo* info)
{
	TPollutionLayer::initLayerInfo(info);
	mMinY = mMinZ;
	mMaxY = mMaxZ;
	unk48 = 20;
}

void TPollutionLayerWallPlusX::stamp(u16 type, f32 x, f32 y, f32 z, f32 s)
{
	if (!isInAreaSize(x, y, z, s))
		return;
	TPollutionPos* pos = &mPos;
	gpPollution->getCounterLayer().pushStampTask(
	    type, mIndexInParent, pos->worldToTexSize(s), getTexPosS(z),
	    getTexPosT(y), pos->worldToDepth(x));
}

void TPollutionLayerWallPlusX::initLayerInfo(const TPollutionLayerInfo* info)
{
	TPollutionLayer::initLayerInfo(info);
	mMinY = mMinZ;
	mMaxY = mMaxZ;
	mMinZ = mMinX;
	mMaxZ = mMaxX;
	unk48 = 20;
}

TPollutionLayerWallBase::TPollutionLayerWallBase()
    : mMinY(0.0f)
    , mMaxY(0.0f)
{
}

void TPollutionLayer::stampModel(J3DModel* model)
{
	MtxPtr mtx = model->getBaseTRMtx();
	f32 x      = mtx[0][3];
	f32 z      = mtx[2][3];
	if (x < mMinX || z < mMinZ || x >= mMaxX || z >= mMaxZ)
		return;
	gpPollution->unk70.pushModelStampTask(mIndexInParent & 0xff, model);
}

#pragma dont_inline on
void TPollutionLayer::appearItem(f32, f32, f32) { }
#pragma dont_inline off

void TPollutionLayer::cleaned(f32 x, f32 y, f32 z, f32 s)
{
	static int effect_counter = 1;
	effect_counter += 1;

	if (effect_counter > 5) {
		effect_counter = 0;

		static JGeometry::TVec3<f32> pos[10];
		static int now_pos_no = 0;

		pos[now_pos_no].set(x, y, z);

		static int x_offset_table[] = { -1, 0, 2, 4, 1, -1, -2, 0, 3, -3 };
		static int z_offset_table[] = { -1, -1, 0, 2, -2, -3, 0, 3, 0, 1 };

		static int counter_x = 0;
		static int counter_z = 0;

		pos[now_pos_no].x += 32.0f * (f32)x_offset_table[counter_x];
		pos[now_pos_no].z += 32.0f * (f32)z_offset_table[counter_z];

		counter_x += 1;
		counter_z += 3;
		if (counter_x >= 10)
			counter_x -= 10;
		if (counter_z >= 10)
			counter_z -= 10;

		if (isPolluted(pos[now_pos_no].x, pos[now_pos_no].y,
		               pos[now_pos_no].z)) {
			int texT = getTexPosT(pos[now_pos_no].z);
			int texS = getTexPosS(pos[now_pos_no].x);
			if (mPollutionMap[mPos.index(texS, texT)] > 100) {
				if (unkA8 <= 0)
					unkA8 = TPollutionManager::mFlushTime;

				SMSGetMSound()->startSoundSet(MSD_SE_ERASE_SCRAWL,
				                              &pos[now_pos_no], 0, s, 0, 0, 4);

				static int effect_timer = 0;
				if (effect_timer == 0) {
					if (gpMarDirector->mMap == 5)
						gpMarioParticleManager->emit(MAPOBJ_SANDSTEAM,
						                             &pos[now_pos_no], 0, this);
					else
						gpMarioParticleManager->emit(PARTICLE_MS_RAKU_KIE,
						                             &pos[now_pos_no], 2, this);
				}

				effect_timer += 1;
				if (effect_timer > mEffectTime)
					effect_timer = 0;

				now_pos_no += 1;
				if (now_pos_no > 10)
					now_pos_no = 0;
			}
		}
	}
	appearItem(x, y, z);
}

void TPollutionLayer::stamp(u16 stamp_type, f32 x, f32 y, f32 z, f32 size)
{
	if (!isInAreaSize(x, y, z, size))
		return;

	gpPollution->getCounterLayer().pushStampTask(
	    stamp_type, mIndexInParent, getPos().worldToTexSize(size),
	    getTexPosS(x), getTexPosT(z), mPos.worldToDepth(y));

	if (getPlaneType() != 6
	    && gpPollution->getCounterLayer().stampIsCleanType(stamp_type))
		cleaned(x, y, z, size);
}

void TPollutionLayer::isProhibit(f32, f32, f32) const { }

bool TPollutionLayer::isPolluted(f32 x, f32 y, f32 z) const
{
	if (!isInArea(x, y, z))
		return false;
	if (getPlaneType() == 6 && y > 1.0f)
		return false;
	int texS = getTexPosS(x);
	int texT = getTexPosT(z);
	return isPolluted(texS, texT, y);
}

bool TPollutionLayer::isPolluted(int s, int t, f32 y) const
{
	if (!mPos.isSame(s, t, y))
		return false;
	if (mPollutionMap[mPos.index(s, t)] > mPollutedThreshold)
		return true;
	return false;
}

void TPollutionLayer::subtractFromYMap(f32, f32, f32) const { }

void TPollutionLayer::perform(u32 flags, JDrama::TGraphics* gfx)
{
	if (flags & 1)
		action();
	TJointModel::perform(flags, gfx);
}

static inline u8 readBmpPixel(const u8* bmp, int x, int y, int w, int h)
{
	return bmp[0x436 + x + w * (h - 1 - y)];
}

void TPollutionLayer::initTexImage(const char* name)
{
	char fullPath[256];
	snprintf(fullPath, 256, "/scene/map/pollution/%s.bmp", name);
	mPollutionBmp = (u8*)JKRGetResource(fullPath);

	bool cVar1 = false;
	if (gpMarDirector->getCurrentMap() == 9)
		cVar1 = true;

	for (int y = 0; y < mPos.getHeight(); ++y) {
		for (int x = 0; x < mPos.getWidth(); ++x) {
			int w    = mPos.getWidth();
			int h    = mPos.getHeight();
			u8 depth = readBmpPixel(mPollutionBmp, x, y, w, h);

			if ((!cVar1 || (x > 0 && x < w - 1 && y > 0 && y < h - 1))
			    && depth != 0 && !mPos.isProhibit(x, y)) {
				int degree = mPos.getEdgeDegree(x, y);
				if (degree != 0) {
					mPollutionMap[mPos.index(x, y)]
					    = depth - degree * TPollutionManager::mEdgeAlpha;
				} else {
					mPollutionMap[mPos.index(x, y)] = depth;
				}
			} else {
				mPollutionMap[mPos.index(x, y)] = 0;
			}
		}
	}
	DCStoreRange(mPollutionMap, mPos.getWidth() * mPos.getHeight());
}

void TPollutionLayer::initTex(const char*) { }

void TPollutionLayer::initLayerInfo(const TPollutionLayerInfo* param_1)
{
	mPollutionType = param_1->mPollutionType;
	mFlags         = param_1->mFlags;
	mMinX          = param_1->mLeft;
	mMinZ          = param_1->mTop;
	mMaxX          = param_1->mRight;
	mMaxZ          = param_1->mBottom;
	unk48          = 2;

	if (getPollutionType() == POLLUTION_TYPE_UNK7) {
		mPollutedThreshold       = 200;
		mPerFrameChangeThreshold = 160;
	} else if (getPollutionType() == POLLUTION_TYPE_FIRE) {
		mPollutedThreshold       = 128;
		mPerFrameChangeThreshold = 128;
	} else {
		mPollutedThreshold       = 30;
		mPerFrameChangeThreshold = 50;
	}

	mEffectPositionsCapacity = 30;
	mEffectPositions = new JGeometry::TVec3<f32>[mEffectPositionsCapacity];
	memset(mEffectPositions, 0,
	       mEffectPositionsCapacity * sizeof(mEffectPositions[0]));
}

void TPollutionLayer::initPollutionTex(const char* depth_tex_name)
{
	const TPollutionLayerInfo* info
	    = ((TPollutionManager*)mManager)->getLayerInfo(mIndexInParent);
	initLayerInfo(info);
	mPos.init(this, info->mVerticalOffset, info->mTexelSize, info->mHeightMap,
	          info->mLog2Width, info->mLog2Height);
	unk88 = info->unk24;

	mPollutionImage               = getTexResource(depth_tex_name);
	mPollutionImage->alphaEnabled = 2;

	mPollutionMap = (u8*)mPollutionImage + mPollutionImage->imageDataOffset;
	initTexImage(depth_tex_name);

	if (getPollutionType() == POLLUTION_TYPE_ELECTRIC)
		SMS_LoadParticle("/scene/map/pollution/ms_thunder_s.jpa",
		                 MAP_POLLUTION_MS_THUNDER_S);
}

void TPollutionLayer::initJointModel(TJointModelManager* param_1,
                                     const char* param_2,
                                     MActorAnmData* param_3)
{
	TJointModel::initJointModel(param_1, param_2, param_3);

	initPollutionTex(param_2);

	if (mActor->checkAnmFileExist(param_2, 4))
		mActor->setBtk(param_2);

	if (mActor->checkAnmFileExist(param_2, 2))
		mActor->setBpk(param_2);

	if (mActor->checkAnmFileExist(param_2, 5))
		mActor->setBrk(param_2);

	for (int i = 0; i < mChildrenNum; ++i)
		((TPollutionObj*)mChildren[i])->initAreaInfo(this);
}

TPollutionLayer::TPollutionLayer()
    : mPollutionType(0)
    , mFlags(0)
    , mCounter(0)
    , mMinX(0.0f)
    , mMaxX(0.0f)
    , mMinZ(0.0f)
    , mMaxZ(0.0f)
    , unk48(0)
    , mSpreadTimer(0)
    , mPollutedThreshold(0)
    , mPollutionMap(nullptr)
    , mPollutionImage(nullptr)
    , mPollutionBmp(nullptr)
    , mPerFrameChangeDelta(8)
    , mPerFrameChangeThreshold(0)
    , mEffectTimer(0)
    , mCurEffectPosIndex(0)
    , mEffectPositionsCapacity(0)
    , mEffectPositions(nullptr)
    , unk9C(1)
    , unkA0(1000)
    , unkA4(100)
    , unkA8(0)
{
}
