#include <MarioUtil/ShadowUtil.hpp>
#include <MarioUtil/GDUtil.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/ReinitGX.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/LightUtil.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <Camera/Camera.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/HitActor.hpp>
#include <System/Application.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/JGeometry/JGUtil.hpp>
#include <dolphin/gd.h>
#include <dolphin/gd/GDTransform.h>
#include <dolphin/gx.h>
#include <math.h>

TMBindShadowParts::TMBindShadowParts(J3DModel* param_1, u8 param_2,
                                     TMBindShadowBody* param_3, f32 param_4)
    : mMinRadius(0.01f)
    , mBody(param_3)
    , mJointName(nullptr)
    , mJointMtx(nullptr)
    , mChildMtx(nullptr)
    , unk14(true)
    , mIsCircle(false)
    , mIsBody(false)
{
	// TODO: fake
	(void)0;
	(void)0;
	(void)0;
	mJointName = param_1->getModelData()->getJointName()->getName(param_2);
	mJointMtx  = param_1->getAnmMtx(param_2);
	mChildMtx  = param_1->getAnmMtx(((J3DJoint*)param_1->getModelData()
                                        ->getJointNodePointer(param_2)
                                        ->getChild())
	                                    ->getJntNo());
	mMinRadius = param_4;
}

void TMBindShadowParts::calc(f32 param_1)
{
	if (!unk14)
		return;

	f32 y1 = param_1;
	f32 y2 = param_1;

	f32 dist = fabsf(mBody->mActor->mPosition.y - param_1);

	const JGeometry::TVec3<f32>& light = gpBindShadowManager->mLightDir;

	f32 lightX = light.x;
	f32 lightZ = light.z;
	f32 h1     = mJointMtx[1][3] - dist - param_1;
	f32 h2     = mChildMtx[1][3] - dist - param_1;
	f32 x1, z1, x2, z2;
	x1 = mJointMtx[0][3] - lightX * h1;
	z1 = mJointMtx[2][3] - lightZ * h1;
	x2 = mChildMtx[0][3] - lightX * h2;
	z2 = mChildMtx[2][3] - lightZ * h2;
	JGeometry::TVec3<f32> center;
	center.set(0.5f * (x1 + x2), 0.5f * (y1 + y2), 0.5f * (z1 + z2));
	f32 cx = center.x;
	f32 cy = center.y;
	f32 cz = center.z;

	f32 radiusX;
	f32 radiusZ;
	radiusZ = fabsf(cz - z2);
	radiusX = fabsf(cx - x2);

	if (mIsBody)
		mMinRadius = mBody->mBodyRadius;
	else if (mIsCircle)
		mMinRadius = mBody->mCircleRadius;
	else
		mMinRadius = mBody->mPartsRadius;

	if (radiusX < mMinRadius)
		radiusX = mMinRadius;
	if (radiusZ < mMinRadius)
		radiusZ = mMinRadius;

	if (!mIsCircle) {
		f32 ratio = gpBindShadowManager->unk68;
		f32 scale = gpBindShadowManager->unk6C;
		if (radiusZ > radiusX) {
			if (radiusZ > ratio * radiusX)
				radiusZ *= scale;
			if (radiusX > mMinRadius)
				radiusX = mMinRadius;
		} else if (radiusX > radiusZ) {
			if (radiusX > ratio * radiusZ)
				radiusX *= scale;
			if (radiusZ > mMinRadius)
				radiusZ = mMinRadius;
		}
	}

	TCircleShadowRequest request;
	request.mPosition.set(cx, cy, cz);
	request.mRadiusX = radiusX;
	request.mRadiusZ = radiusZ;

	if (!mIsCircle && mBody->mActor->getActorType() != 0x80000001
	    && mBody->mActor->getActorType() != 0x8000002) {
		f32 rotY = matan(z2 - z1, x2 - x1) * (360.0f / 65536.0f);
		if (radiusX > radiusZ)
			rotY -= 90.0f;
		request.mRotationY = rotY;
	} else {
		request.mRotationY = 0.0f;
	}

	gpBindShadowManager->request(request, mBody->mActor->getActorType());
}

TMBindShadowBody::TMBindShadowBody(THitActor* param_1, J3DModel* param_2,
                                   f32 param_3)
    : mParts(nullptr)
    , mActor(param_1)
    , mPartsNum(0)
    , mActorName(param_1->getName())
    , mCircleRadius(0.01f)
    , mPartsRadius(0.01f)
    , mBodyRadius(50.0f)
{
	switch (param_1->getActorType()) {
	case 0x80000001:
	case 0x8000002:
		mCircleRadius = 38.0f;
		mPartsRadius  = 18.0f;
		mBodyRadius   = 25.0f;
		break;
	case 0x8000001:
		mCircleRadius = 280.0f;
		mPartsRadius  = 50.0f;
		break;
	default:
		mCircleRadius = 50.0f;
		mPartsRadius  = 50.0f;
		break;
	}

	mCircleRadius *= param_3;
	mPartsRadius *= param_3;

	J3DModelData* modelData = param_2->getModelData();

	for (int i = 0; i < modelData->getJointNum(); i++) {
		if (modelData->getJointNodePointer((u8)i)->getKind() == 1
		    && isUseThisJoint(i))
			mPartsNum++;
	}

	mParts = new TMBindShadowParts*[mPartsNum];

	int count = 0;
	for (int i = 0; i < modelData->getJointNum(); i++) {
		if (modelData->getJointNodePointer((u8)i)->getKind() != 1
		    || !isUseThisJoint(i))
			continue;

		if (isCircleJoint(i)) {
			mParts[count]
			    = new TMBindShadowParts(param_2, i, this, mCircleRadius);
			mParts[count]->mIsCircle = true;
		} else if (isBodyJoint(i)) {
			mParts[count]
			    = new TMBindShadowParts(param_2, i, this, mBodyRadius);
			mParts[count]->mIsBody = true;
		} else {
			mParts[count]
			    = new TMBindShadowParts(param_2, i, this, mPartsRadius);
		}

		count++;
	}

	gpBindShadowManager->initEntry(this);
}

bool TMBindShadowBody::isUseThisJoint(int param_1)
{
	const THitActor* actor = mActor;
	switch (actor->getActorType()) {
	case 0x80000001:
	case 0x8000002:
		return true;
	case 0x8000001:
		if (param_1 == 0x17)
			return false;
		return true;
	}

	return true;
}

bool TMBindShadowBody::isCircleJoint(int param_1)
{
	switch (mActor->getActorType()) {
	case 0x80000001:
	case 0x8000002:
		if (param_1 == 0x1a)
			return true;
		return false;
	case 0x8000001:
		if (param_1 == 0x13 || param_1 == 0x17)
			return true;
		return false;
	}

	return false;
}

bool TMBindShadowBody::isBodyJoint(int param_1)
{
	switch (mActor->getActorType()) {
	case 0x80000001:
	case 0x8000002:
		if (param_1 == 2 || param_1 == 0xe)
			return true;
		return false;
	}

	return false;
}

void TMBindShadowBody::entryDrawShadow()
{
	f32 eps = JGeometry::TUtil<f32>::epsilon();

	if (gpMarioPos->epsilonEquals(mActor->mPosition, eps)) {
		if (!gpBindShadowManager->unk65) {
			gpBindShadowManager->unk65 = true;
			calc();
		}
	} else {
		calc();
	}
}

void TMBindShadowBody::calc()
{
	JGeometry::TVec3<f32> pos = mActor->mPosition;

	f32 y = pos.y;
	f32 z = pos.z;

	const TBGCheckData* ground;
	f32 groundY
	    = gpMap->checkGround(pos.x, y + gpBindShadowManager->unk60, z, &ground);

	if (ground->isWaterSurface())
		groundY = gpMap->checkGround(pos.x, y - 50.0f, z, &ground);

	if (ground->isIllegalData())
		return;

	for (int i = 0; i < mPartsNum; i++)
		mParts[i]->calc(groundY);
}

TSquareShadowInfo::TSquareShadowInfo()
{
	for (Vec* p = &mPoints[0]; p != &mPoints[5]; p++) {
		p->x = 0.0f;
		p->y = 0.0f;
		p->y = 0.0f;
	}
}

TModelShadowInfo::TModelShadowInfo()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mIsFar(false)
    , unkD(true)
    , unk10(0.01f)
{
}

void TAlphaShadowQuad::reset()
{
	mRadius        = 0.01f;
	mSquareOutline = nullptr;
	mRequest       = nullptr;
	mNext          = nullptr;
}

TModelShadow::TModelShadow(SDLModelData* param_1, void* param_2, int param_3) {
}

void TModelShadow::update() { }

void TModelShadow::calc(int param_1, JDrama::TGraphics* param_2) { }

void TModelShadow::draw(int param_1, JDrama::TGraphics* param_2) { }

TMBindShadowManager* gpBindShadowManager;

f32 TMBindShadowManager::mJoinDist;
f32 TMBindShadowManager::mSquareShadowHeight = 200.0f;
f32 TMBindShadowManager::mTreeScale          = 0.02f;
f32 TMBindShadowManager::mYScalePlus         = 20.0f;
u8 TMBindShadowManager::mTestSw;
u8 TMBindShadowManager::mDLSw;

TMBindShadowManager::TMBindShadowManager(const char* name)
    : JDrama::TViewObj(name)
    , mRequestNum(0)
    , mQuadAryNum(0)
    , mSquareShadowNum(0)
    , mModelShadowNum(0)
    , unk44(0)
    , unk48(0)
    , unk49(0)
    , unk60(30.0f)
    , unk64(false)
    , unk65(false)
    , unk68(0.5f)
    , unk6C(1.55f)
    , mModelShadows(nullptr)
{
	mShadowColor.r = 30;
	mShadowColor.g = 50;
	mShadowColor.b = 115;
	mShadowColor.a = 180;

	switch (gpApplication.mCurrArea.getStage()) {
	case 6:
		mShadowColor.r = 9;
		mShadowColor.g = 9;
		mShadowColor.b = 28;
		mShadowColor.a = 116;
		break;
	case 7:
		mShadowColor.r = 45;
		mShadowColor.g = 40;
		mShadowColor.b = 60;
		mShadowColor.a = 90;
		break;
	}

	gpBindShadowManager = this;

	mRequests     = new TCircleShadowRequest[0x200];
	mQuads        = new TAlphaShadowQuad[0x200];
	mQuadArys     = new TAlphaShadowQuadAry[0x100];
	mBlendQuads   = new TAlphaShadowBlendQuad[0x200];
	mSquareShadow = new TSquareShadowInfo[0x1E];
	mModelShadows = new TModelShadowInfo[1];
	mModelDatas   = new SDLModelData*[5];
}

void TMBindShadowManager::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);

	void* resource = JKRFileLoader::getGlbResource("/common/shadowCircle.bmd");
	mModelDatas[0]
	    = new SDLModelData(J3DModelLoaderDataBase::load(resource, 0x10210000));

	resource = JKRFileLoader::getGlbResource("/common/shadowCircleLow.bmd");
	mModelDatas[1]
	    = new SDLModelData(J3DModelLoaderDataBase::load(resource, 0x10210000));

	resource = JKRFileLoader::getGlbResource("/common/shadowCube.bmd");
	mModelDatas[2]
	    = new SDLModelData(J3DModelLoaderDataBase::load(resource, 0x10210000));

	resource = JKRFileLoader::getGlbResource("/common/ShipShadow.bmd");
	mModelDatas[3]
	    = new SDLModelData(J3DModelLoaderDataBase::load(resource, 0x10210000));

	reset();
}

void TMBindShadowManager::reset()
{
	unk49            = 1;
	mRequestNum      = 0;
	mQuadAryNum      = 0;
	unk65            = false;
	mSquareShadowNum = 0;
	mModelShadowNum  = 0;
}

void TMBindShadowManager::initEntry(TMBindShadowBody* param_1)
{
	mBodyList.push_back(param_1);
}

void TMBindShadowManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_VIEW) {
		unk49 = 0;
		VECNormalize(gpLightManager->getLightPos(), &mLightDir);
		calcVtx();
	}

	if (cue & CUE_DRAW) {
		if (mDLSw)
			drawShadowGD(cue, graphics);
		else
			drawShadow(cue, graphics);

		if (cue & 0x20000000)
			reset();
	}
}

static bool conectCubeDiffer(TAlphaShadowBlendQuad* param_1,
                             TAlphaShadowBlendQuad* param_2)
{
	if (param_1 == nullptr || param_2 == nullptr)
		return false;

	if (param_1->unk18 != param_2->unk18 || param_1->unk18 == 0
	    || param_2->unk18 == 0 || (param_1->unk18 & 0x40000000)
	    || (param_2->unk18 & 0x40000000))
		return false;

	if (fabsf(param_1->mMin.y - param_2->mMin.y) > 50.0f)
		return false;

	if (param_1->mMin.x <= param_2->mMax.x && param_1->mMax.x >= param_2->mMin.x
	    && param_1->mMin.z <= param_2->mMax.z
	    && param_1->mMax.z >= param_2->mMin.z) {
		if (param_1->mMax.x <= param_2->mMax.x)
			param_1->mMax.x = param_2->mMax.x;
		if (param_1->mMin.x >= param_2->mMin.x)
			param_1->mMin.x = param_2->mMin.x;
		if (param_1->mMax.z <= param_2->mMax.z)
			param_1->mMax.z = param_2->mMax.z;
		if (param_1->mMin.z >= param_2->mMin.z)
			param_1->mMin.z = param_2->mMin.z;
		if (param_1->mMin.y >= param_2->mMin.y)
			param_1->mMin.y = param_2->mMin.y;
		if (param_1->mMax.y <= param_2->mMax.y)
			param_1->mMax.y = param_2->mMax.y;
		return true;
	}

	return false;
}

static bool conectCubeSame(TAlphaShadowBlendQuad* param_1,
                           TAlphaShadowBlendQuad* param_2)
{
	if (param_1 == nullptr || param_2 == nullptr)
		return false;

	f32 dist = TMBindShadowManager::mJoinDist;

	if (fabsf(param_1->mMin.y - param_2->mMin.y) > 50.0f)
		return false;

	if (param_1->mMin.x <= param_2->mMax.x - dist
	    && param_1->mMax.x >= param_2->mMin.x + dist
	    && param_1->mMin.z <= param_2->mMax.z - dist
	    && param_1->mMax.z >= param_2->mMin.z + dist) {
		if (param_1->mMax.x <= param_2->mMax.x - dist)
			param_1->mMax.x = param_2->mMax.x;
		if (param_1->mMin.x >= param_2->mMin.x + dist)
			param_1->mMin.x = param_2->mMin.x;
		if (param_1->mMax.z <= param_2->mMax.z - dist)
			param_1->mMax.z = param_2->mMax.z;
		if (param_1->mMin.z >= param_2->mMin.z + dist)
			param_1->mMin.z = param_2->mMin.z;
		if (param_1->mMin.y >= param_2->mMin.y)
			param_1->mMin.y = param_2->mMin.y;
		if (param_1->mMax.y <= param_2->mMax.y)
			param_1->mMax.y = param_2->mMax.y;
		return true;
	}

	return false;
}

// fabricated name; the body is proven, the name is not
static inline void loadPosMtxImm(MtxPtr mtx)
{
	GXCmd1u8(GX_CMD_LOAD_XF_REG);
	GXCmd1u16(12 - 1);
	GXCmd1u16(0);
	GXParam1f32(mtx[0][0]);
	GXParam1f32(mtx[0][1]);
	GXParam1f32(mtx[0][2]);
	GXParam1f32(mtx[0][3]);
	GXParam1f32(mtx[1][0]);
	GXParam1f32(mtx[1][1]);
	GXParam1f32(mtx[1][2]);
	GXParam1f32(mtx[1][3]);
	GXParam1f32(mtx[2][0]);
	GXParam1f32(mtx[2][1]);
	GXParam1f32(mtx[2][2]);
	GXParam1f32(mtx[2][3]);
}

void TMBindShadowManager::drawShadowVolume(bool param_1,
                                           TAlphaShadowQuad* param_2)
{
	f32 height = 50.0f;
	if (param_2->mRequest->mShadowType == SHADOW_TYPE_SQUARE) {
		if (param_2->mSquareOutline == nullptr) {
			SMS_SettingDrawShape(mModelDatas[2]->getModelData(), 0);
			SMS_DrawShape(mModelDatas[2]->getModelData(), 0);
		} else {
			int topIndices[9]    = { 2, 1, 0, 3, 2, 0, 4, 3, 0 };
			int bottomIndices[9] = { 0, 1, 2, 0, 2, 3, 0, 3, 4 };

			GXClearVtxDesc();
			GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
			GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);

			GXBegin(GX_TRIANGLES, GX_VTXFMT0, 18);

			for (int i = 0; i < 9; i++) {
				Vec* v = &param_2->mSquareOutline[topIndices[i]];
				GXPosition3f32(v->x, 50.0f + v->y, v->z);
			}

			for (int i = 0; i < 9; i++) {
				Vec* v = &param_2->mSquareOutline[bottomIndices[i]];
				GXPosition3f32(v->x, v->y - 50.0f, v->z);
			}

			GXEnd();

			GXBegin(GX_TRIANGLES, GX_VTXFMT0, 60);

			GXPosition3f32(param_2->mSquareOutline[0].x,
			               param_2->mSquareOutline[0].y + height,
			               param_2->mSquareOutline[0].z);
			GXPosition3f32(param_2->mSquareOutline[1].x,
			               param_2->mSquareOutline[1].y + height,
			               param_2->mSquareOutline[1].z);
			GXPosition3f32(param_2->mSquareOutline[1].x,
			               param_2->mSquareOutline[1].y - 50.0f,
			               param_2->mSquareOutline[1].z);
			GXPosition3f32(param_2->mSquareOutline[1].x,
			               param_2->mSquareOutline[1].y - 50.0f,
			               param_2->mSquareOutline[1].z);
			GXPosition3f32(param_2->mSquareOutline[0].x,
			               param_2->mSquareOutline[0].y - 50.0f,
			               param_2->mSquareOutline[0].z);
			GXPosition3f32(param_2->mSquareOutline[0].x,
			               param_2->mSquareOutline[0].y + height,
			               param_2->mSquareOutline[0].z);
			GXPosition3f32(param_2->mSquareOutline[1].x,
			               param_2->mSquareOutline[1].y + height,
			               param_2->mSquareOutline[1].z);
			GXPosition3f32(param_2->mSquareOutline[0].x,
			               param_2->mSquareOutline[0].y + height,
			               param_2->mSquareOutline[0].z);
			GXPosition3f32(param_2->mSquareOutline[1].x,
			               param_2->mSquareOutline[1].y - 50.0f,
			               param_2->mSquareOutline[1].z);
			GXPosition3f32(param_2->mSquareOutline[1].x,
			               param_2->mSquareOutline[1].y - 50.0f,
			               param_2->mSquareOutline[1].z);
			GXPosition3f32(param_2->mSquareOutline[0].x,
			               param_2->mSquareOutline[0].y + height,
			               param_2->mSquareOutline[0].z);
			GXPosition3f32(param_2->mSquareOutline[0].x,
			               param_2->mSquareOutline[0].y - 50.0f,
			               param_2->mSquareOutline[0].z);

			GXPosition3f32(param_2->mSquareOutline[1].x,
			               param_2->mSquareOutline[1].y + height,
			               param_2->mSquareOutline[1].z);
			GXPosition3f32(param_2->mSquareOutline[2].x,
			               param_2->mSquareOutline[2].y + height,
			               param_2->mSquareOutline[2].z);
			GXPosition3f32(param_2->mSquareOutline[2].x,
			               param_2->mSquareOutline[2].y - 50.0f,
			               param_2->mSquareOutline[2].z);
			GXPosition3f32(param_2->mSquareOutline[2].x,
			               param_2->mSquareOutline[2].y - 50.0f,
			               param_2->mSquareOutline[2].z);
			GXPosition3f32(param_2->mSquareOutline[1].x,
			               param_2->mSquareOutline[1].y - 50.0f,
			               param_2->mSquareOutline[1].z);
			GXPosition3f32(param_2->mSquareOutline[1].x,
			               param_2->mSquareOutline[1].y + height,
			               param_2->mSquareOutline[1].z);
			GXPosition3f32(param_2->mSquareOutline[2].x,
			               param_2->mSquareOutline[2].y + height,
			               param_2->mSquareOutline[2].z);
			GXPosition3f32(param_2->mSquareOutline[1].x,
			               param_2->mSquareOutline[1].y + height,
			               param_2->mSquareOutline[1].z);
			GXPosition3f32(param_2->mSquareOutline[2].x,
			               param_2->mSquareOutline[2].y - 50.0f,
			               param_2->mSquareOutline[2].z);
			GXPosition3f32(param_2->mSquareOutline[2].x,
			               param_2->mSquareOutline[2].y - 50.0f,
			               param_2->mSquareOutline[2].z);
			GXPosition3f32(param_2->mSquareOutline[1].x,
			               param_2->mSquareOutline[1].y + height,
			               param_2->mSquareOutline[1].z);
			GXPosition3f32(param_2->mSquareOutline[1].x,
			               param_2->mSquareOutline[1].y - 50.0f,
			               param_2->mSquareOutline[1].z);

			GXPosition3f32(param_2->mSquareOutline[2].x,
			               param_2->mSquareOutline[2].y + height,
			               param_2->mSquareOutline[2].z);
			GXPosition3f32(param_2->mSquareOutline[3].x,
			               param_2->mSquareOutline[3].y + height,
			               param_2->mSquareOutline[3].z);
			GXPosition3f32(param_2->mSquareOutline[3].x,
			               param_2->mSquareOutline[3].y - 50.0f,
			               param_2->mSquareOutline[3].z);
			GXPosition3f32(param_2->mSquareOutline[3].x,
			               param_2->mSquareOutline[3].y - 50.0f,
			               param_2->mSquareOutline[3].z);
			GXPosition3f32(param_2->mSquareOutline[2].x,
			               param_2->mSquareOutline[2].y - 50.0f,
			               param_2->mSquareOutline[2].z);
			GXPosition3f32(param_2->mSquareOutline[2].x,
			               param_2->mSquareOutline[2].y + height,
			               param_2->mSquareOutline[2].z);
			GXPosition3f32(param_2->mSquareOutline[3].x,
			               param_2->mSquareOutline[3].y + height,
			               param_2->mSquareOutline[3].z);
			GXPosition3f32(param_2->mSquareOutline[2].x,
			               param_2->mSquareOutline[2].y + height,
			               param_2->mSquareOutline[2].z);
			GXPosition3f32(param_2->mSquareOutline[3].x,
			               param_2->mSquareOutline[3].y - 50.0f,
			               param_2->mSquareOutline[3].z);
			GXPosition3f32(param_2->mSquareOutline[3].x,
			               param_2->mSquareOutline[3].y - 50.0f,
			               param_2->mSquareOutline[3].z);
			GXPosition3f32(param_2->mSquareOutline[2].x,
			               param_2->mSquareOutline[2].y + height,
			               param_2->mSquareOutline[2].z);
			GXPosition3f32(param_2->mSquareOutline[2].x,
			               param_2->mSquareOutline[2].y - 50.0f,
			               param_2->mSquareOutline[2].z);

			GXPosition3f32(param_2->mSquareOutline[3].x,
			               param_2->mSquareOutline[3].y + height,
			               param_2->mSquareOutline[3].z);
			GXPosition3f32(param_2->mSquareOutline[4].x,
			               param_2->mSquareOutline[4].y + height,
			               param_2->mSquareOutline[4].z);
			GXPosition3f32(param_2->mSquareOutline[4].x,
			               param_2->mSquareOutline[4].y - 50.0f,
			               param_2->mSquareOutline[4].z);
			GXPosition3f32(param_2->mSquareOutline[4].x,
			               param_2->mSquareOutline[4].y - 50.0f,
			               param_2->mSquareOutline[4].z);
			GXPosition3f32(param_2->mSquareOutline[3].x,
			               param_2->mSquareOutline[3].y - 50.0f,
			               param_2->mSquareOutline[3].z);
			GXPosition3f32(param_2->mSquareOutline[3].x,
			               param_2->mSquareOutline[3].y + height,
			               param_2->mSquareOutline[3].z);
			GXPosition3f32(param_2->mSquareOutline[4].x,
			               param_2->mSquareOutline[4].y + height,
			               param_2->mSquareOutline[4].z);
			GXPosition3f32(param_2->mSquareOutline[3].x,
			               param_2->mSquareOutline[3].y + height,
			               param_2->mSquareOutline[3].z);
			GXPosition3f32(param_2->mSquareOutline[4].x,
			               param_2->mSquareOutline[4].y - 50.0f,
			               param_2->mSquareOutline[4].z);
			GXPosition3f32(param_2->mSquareOutline[4].x,
			               param_2->mSquareOutline[4].y - 50.0f,
			               param_2->mSquareOutline[4].z);
			GXPosition3f32(param_2->mSquareOutline[3].x,
			               param_2->mSquareOutline[3].y + height,
			               param_2->mSquareOutline[3].z);
			GXPosition3f32(param_2->mSquareOutline[3].x,
			               param_2->mSquareOutline[3].y - 50.0f,
			               param_2->mSquareOutline[3].z);

			GXPosition3f32(param_2->mSquareOutline[4].x,
			               param_2->mSquareOutline[4].y + height,
			               param_2->mSquareOutline[4].z);
			GXPosition3f32(param_2->mSquareOutline[0].x,
			               param_2->mSquareOutline[0].y + height,
			               param_2->mSquareOutline[0].z);
			GXPosition3f32(param_2->mSquareOutline[0].x,
			               param_2->mSquareOutline[0].y - 50.0f,
			               param_2->mSquareOutline[0].z);
			GXPosition3f32(param_2->mSquareOutline[0].x,
			               param_2->mSquareOutline[0].y - 50.0f,
			               param_2->mSquareOutline[0].z);
			GXPosition3f32(param_2->mSquareOutline[4].x,
			               param_2->mSquareOutline[4].y - 50.0f,
			               param_2->mSquareOutline[4].z);
			GXPosition3f32(param_2->mSquareOutline[4].x,
			               param_2->mSquareOutline[4].y + height,
			               param_2->mSquareOutline[4].z);
			GXPosition3f32(param_2->mSquareOutline[0].x,
			               param_2->mSquareOutline[0].y + height,
			               param_2->mSquareOutline[0].z);
			GXPosition3f32(param_2->mSquareOutline[4].x,
			               param_2->mSquareOutline[4].y + height,
			               param_2->mSquareOutline[4].z);
			GXPosition3f32(param_2->mSquareOutline[0].x,
			               param_2->mSquareOutline[0].y - 50.0f,
			               param_2->mSquareOutline[0].z);
			GXPosition3f32(param_2->mSquareOutline[0].x,
			               param_2->mSquareOutline[0].y - 50.0f,
			               param_2->mSquareOutline[0].z);
			GXPosition3f32(param_2->mSquareOutline[4].x,
			               param_2->mSquareOutline[4].y + height,
			               param_2->mSquareOutline[4].z);
			GXPosition3f32(param_2->mSquareOutline[4].x,
			               param_2->mSquareOutline[4].y - 50.0f,
			               param_2->mSquareOutline[4].z);

			GXEnd();
		}
	} else if (param_2->mRequest->mShadowType == SHADOW_TYPE_SHIP) {
		SMS_SettingDrawShape(mModelDatas[3]->getModelData(), 0);
		SMS_DrawShape(mModelDatas[3]->getModelData(), 0);
	} else {
		if (param_1)
			SMS_DrawShape(mModelDatas[0]->getModelData(), 0);
		else
			SMS_DrawShape(mModelDatas[1]->getModelData(), 0);
		return;
	}

	if (param_1)
		SMS_SettingDrawShape(mModelDatas[0]->getModelData(), 0);
	else
		SMS_SettingDrawShape(mModelDatas[1]->getModelData(), 0);
}

void TMBindShadowManager::drawShadowGD(u32 param_1, JDrama::TGraphics* param_2)
{
	class TCylinder : public TGDLStatic {
	public:
		TCylinder(u32 size)
		    : TGDLStatic(size)
		{
		}

		virtual void makeDL()
		{
			int div = 10;
			f32 z   = 1.0f;
			f32 nz  = -z;
			int i;

			f32 cosTable[100];
			f32 sinTable[100];

			for (i = 0; i <= div; i++) {
				f32 angle   = M_PI * (2.0f * i) / div;
				cosTable[i] = cosf(angle);
				sinTable[i] = sinf(angle);
			}

			GDBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, div * 2 + 2);
			for (i = 0; i <= div; i++) {
				GDPosition3f32(cosTable[i], sinTable[i], nz);
				GDPosition3f32(cosTable[i], sinTable[i], z);
			}
			GDEnd();

			GDBegin(GX_TRIANGLEFAN, GX_VTXFMT0, div + 2);
			GDPosition3f32(0.0f, 0.0f, z);
			for (i = 0; i <= div; i++)
				GDPosition3f32(cosTable[i], -sinTable[i], z);
			GDEnd();

			GDBegin(GX_TRIANGLEFAN, GX_VTXFMT0, div + 2);
			GDPosition3f32(0.0f, 0.0f, nz);
			for (i = 0; i <= div; i++)
				GDPosition3f32(cosTable[i], sinTable[i], nz);
			GDEnd();
		}
	};

	class TSetup1 : public TGDLStatic {
	public:
		TSetup1(u32 size)
		    : TGDLStatic(size)
		{
		}

		virtual void makeDL()
		{
			GDSetGenMode2(0, 1, 1, 0, GX_CULL_BACK);
			GDSetChanCtrl(GX_COLOR0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
			              GX_DF_NONE, GX_AF_NONE);
			GDSetChanCtrl(GX_ALPHA0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
			              GX_DF_NONE, GX_AF_NONE);
			GDSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
			GDSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
			              GX_COLOR0A0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
			              GX_COLOR0A0);

			GDSetChanMatColor(GX_COLOR0A0, (GXColor) { 30, 50, 115, 180 });
			GDSetCurrentMtx(0, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c);

			static GXVtxDescList vl[]
			    = { { GX_VA_POS, GX_DIRECT }, { GX_VA_NULL, GX_NONE } };
			GDSetVtxDescv(vl);

			static GXVtxAttrFmtList fl[]
			    = { { GX_VA_POS, GX_POS_XYZ, GX_F32, 0 },
				    { GX_VA_NULL, GX_POS_XYZ, GX_U8, 0 } };
			GDSetVtxAttrFmtv(GX_VTXFMT0, fl);
		}
	};

	GXSetZCompLoc(GX_TRUE);

	static TSetup1 setup1(0x100);
	setup1.callDL();

	static TCylinder cylinder(0x400);

	MtxPtr viewMtx = param_2->getViewMtx();

	for (int i = 0; i < mQuadAryNum; i++) {
		TAlphaShadowQuadAry* ary = &mQuadArys[i];

		if (ary->mQuadHead == nullptr || ary->mBlendHead == nullptr)
			continue;

		if (!(param_1 & ary->unk0))
			continue;

		class TSetup2 : public TGDLStatic {
		public:
			TSetup2(u32 size)
			    : TGDLStatic(size)
			{
			}

			virtual void makeDL()
			{
				GDSetCullMode(GX_CULL_NONE);
				GDSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
				GDSetBlendModeEtc(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP,
				                  GX_FALSE, GX_TRUE, GX_FALSE);
				GDSetDstAlpha(GX_TRUE, 0);
			}
		};

		static TSetup2 setup2(0x80);
		setup2.callDL();

		TAlphaShadowBlendQuad* blend = mQuadArys[i].mBlendHead;

		f32 minX;
		f32 y1;
		f32 minZ;
		f32 maxX;
		f32 maxZ;
		f32 y2;

		y2 = blend->mMin.y;
		y1 = y2 - blend->mMax.y;
		y2 += blend->mMax.y;
		minX = blend->mMin.x;
		minZ = blend->mMin.z;
		maxX = blend->mMax.x;
		maxZ = blend->mMax.z;

		loadPosMtxImm(param_2->getViewMtx());

		GXCmd1u8(GX_QUADS | GX_VTXFMT0);
		GXCmd1u16(24);
		GXPosition3f32(minX, y1, minZ);
		GXPosition3f32(minX, y1, maxZ);
		GXPosition3f32(maxX, y1, maxZ);
		GXPosition3f32(maxX, y1, minZ);
		GXPosition3f32(minX, y1, minZ);
		GXPosition3f32(maxX, y1, minZ);
		GXPosition3f32(maxX, y2, minZ);
		GXPosition3f32(minX, y2, minZ);
		GXPosition3f32(minX, y1, minZ);
		GXPosition3f32(minX, y2, minZ);
		GXPosition3f32(minX, y2, maxZ);
		GXPosition3f32(minX, y1, maxZ);
		GXPosition3f32(maxX, y2, maxZ);
		GXPosition3f32(minX, y2, maxZ);
		GXPosition3f32(minX, y2, minZ);
		GXPosition3f32(maxX, y2, minZ);
		GXPosition3f32(maxX, y2, maxZ);
		GXPosition3f32(maxX, y1, maxZ);
		GXPosition3f32(minX, y1, maxZ);
		GXPosition3f32(minX, y2, maxZ);
		GXPosition3f32(maxX, y2, maxZ);
		GXPosition3f32(maxX, y2, minZ);
		GXPosition3f32(maxX, y1, minZ);
		GXPosition3f32(maxX, y1, maxZ);
		GXEnd();

		TAlphaShadowQuad* quad = mQuadArys[i].mQuadHead;
		u8 lowPoly             = 0;

		class TSetup3 : public TGDLStatic {
		public:
			TSetup3(u32 size)
			    : TGDLStatic(size)
			{
			}

			virtual void makeDL()
			{
				GDSetDstAlpha(GX_FALSE, 0);
				GDSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
				GDSetCullMode(GX_CULL_BACK);
				GDSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
			}
		};

		static TSetup3 setup3(0x80);
		setup3.callDL();
		if (quad->mRequest->mCameraDistSq < 20000000.0f) {
			SMS_SettingDrawShape(mModelDatas[0]->getModelData(), 0);
			lowPoly = 1;
		} else {
			SMS_SettingDrawShape(mModelDatas[1]->getModelData(), 0);
		}

		while (quad != nullptr) {
			loadPosMtxImm(quad->mMtx);
			drawShadowVolume(lowPoly, quad);
			quad = quad->mNext;
		}

		class TSetup4 : public TGDLStatic {
		public:
			TSetup4(u32 size)
			    : TGDLStatic(size)
			{
			}

			virtual void makeDL()
			{
				GDSetDstAlpha(GX_TRUE, 0);
				GDSetZMode(GX_TRUE, GX_GEQUAL, GX_FALSE);
				GDSetCullMode(GX_CULL_FRONT);
				GDSetBlendModeEtc(GX_BM_BLEND, GX_BL_DSTALPHA,
				                  GX_BL_INVDSTALPHA, GX_LO_NOOP, GX_TRUE,
				                  GX_TRUE, GX_FALSE);
			}
		};

		static TSetup4 setup4(0x80);
		setup4.callDL();

		quad = mQuadArys[i].mQuadHead;
		while (quad != nullptr) {
			loadPosMtxImm(quad->mMtx);
			drawShadowVolume(lowPoly, quad);
			quad = quad->mNext;
		}

		class TSetup5 : public TGDLStatic {
		public:
			TSetup5(u32 size)
			    : TGDLStatic(size)
			{
			}

			virtual void makeDL()
			{
				GDSetDstAlpha(GX_TRUE, 0);
				GDSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
				GDSetCullMode(GX_CULL_BACK);
				GDSetBlendModeEtc(GX_BM_BLEND, GX_BL_DSTALPHA,
				                  GX_BL_INVDSTALPHA, GX_LO_NOOP, GX_FALSE,
				                  GX_TRUE, GX_FALSE);
			}
		};

		static TSetup5 setup5(0x100);
		setup5.callDL();

		loadPosMtxImm(viewMtx);

		GXCmd1u8(GX_QUADS | GX_VTXFMT0);
		GXCmd1u16(24);
		GXPosition3f32(minX, y1, minZ);
		GXPosition3f32(minX, y1, maxZ);
		GXPosition3f32(maxX, y1, maxZ);
		GXPosition3f32(maxX, y1, minZ);
		GXPosition3f32(minX, y1, minZ);
		GXPosition3f32(maxX, y1, minZ);
		GXPosition3f32(maxX, y2, minZ);
		GXPosition3f32(minX, y2, minZ);
		GXPosition3f32(minX, y1, minZ);
		GXPosition3f32(minX, y2, minZ);
		GXPosition3f32(minX, y2, maxZ);
		GXPosition3f32(minX, y1, maxZ);
		GXPosition3f32(maxX, y2, maxZ);
		GXPosition3f32(minX, y2, maxZ);
		GXPosition3f32(minX, y2, minZ);
		GXPosition3f32(maxX, y2, minZ);
		GXPosition3f32(maxX, y2, maxZ);
		GXPosition3f32(maxX, y1, maxZ);
		GXPosition3f32(minX, y1, maxZ);
		GXPosition3f32(minX, y2, maxZ);
		GXPosition3f32(maxX, y2, maxZ);
		GXPosition3f32(maxX, y2, minZ);
		GXPosition3f32(maxX, y1, minZ);
		GXPosition3f32(maxX, y1, maxZ);
		GXEnd();
	}
}

static inline void initShadowGX(const GXColor& color,
                                JDrama::TGraphics* graphics)
{
	ReInitializeGX();
	GXSetZCompLoc(GX_TRUE);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_ALPHA0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetAlphaUpdate(GX_TRUE);
	GXSetChanMatColor(GX_COLOR0A0, color);
	GXSetCurrentMtx(GX_PNMTX0);
	GXLoadNrmMtxImm(graphics->getViewMtx(), GX_PNMTX0);
}

void TMBindShadowManager::drawShadow(u32 param_1, JDrama::TGraphics* param_2)
{
	if (!mTestSw) {
		initShadowGX(mShadowColor, param_2);

		MtxPtr viewMtx = param_2->getViewMtx();

		for (int i = 0; i < mQuadAryNum; i++) {
			TAlphaShadowQuadAry* ary = &mQuadArys[i];

			if (ary->mQuadHead == nullptr || ary->mBlendHead == nullptr)
				continue;

			if (!(param_1 & ary->unk0))
				continue;

			GXSetCullMode(GX_CULL_NONE);
			GXLoadPosMtxImm(param_2->getViewMtx(), GX_PNMTX0);
			GXSetColorUpdate(GX_FALSE);
			GXSetDstAlpha(GX_TRUE, 0);
			GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
			GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);

			TAlphaShadowBlendQuad* blend = mQuadArys[i].mBlendHead;

			JGeometry::TVec3<f32> max;
			JGeometry::TVec3<f32> min;

			min.set(blend->mMin.x, blend->mMin.y - blend->mMax.y,
			        blend->mMin.z);
			max.set(blend->mMax.x, blend->mMin.y + blend->mMax.y,
			        blend->mMax.z);
			SMS_DrawCube(min, max);

			u8 lowPoly             = 0;
			TAlphaShadowQuad* quad = mQuadArys[i].mQuadHead;

			if (quad->mRequest->mCameraDistSq < 20000000.0f) {
				SMS_SettingDrawShape(mModelDatas[0]->getModelData(), 0);
				lowPoly = 1;
			} else {
				SMS_SettingDrawShape(mModelDatas[1]->getModelData(), 0);
			}

			GXSetDstAlpha(GX_FALSE, 0);
			GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
			GXSetCullMode(GX_CULL_BACK);
			GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);

			while (quad != nullptr) {
				GXLoadPosMtxImm(quad->mMtx, GX_PNMTX0);
				drawShadowVolume(lowPoly, quad);
				quad = quad->mNext;
			}

			GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_INVDSTALPHA,
			               GX_LO_NOOP);
			GXSetZMode(GX_TRUE, GX_GEQUAL, GX_FALSE);
			GXSetCullMode(GX_CULL_FRONT);
			GXSetDstAlpha(GX_TRUE, 0);
			GXSetColorUpdate(GX_TRUE);

			quad = mQuadArys[i].mQuadHead;
			while (quad != nullptr) {
				GXLoadPosMtxImm(quad->mMtx, GX_PNMTX0);
				drawShadowVolume(lowPoly, quad);
				quad = quad->mNext;
			}

			GXSetCullMode(GX_CULL_BACK);
			GXSetColorUpdate(GX_FALSE);
			GXSetDstAlpha(GX_TRUE, 0);
			GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);

			quad = mQuadArys[i].mQuadHead;
			while (quad != nullptr) {
				if (quad->mRequest->mShadowType == SHADOW_TYPE_SHIP) {
					GXLoadPosMtxImm(quad->mMtx, GX_PNMTX0);
					SMS_SettingDrawShape(mModelDatas[3]->getModelData(), 0);
					SMS_DrawShape(mModelDatas[3]->getModelData(), 0);
				}
				quad = quad->mNext;
			}

			GXClearVtxDesc();
			GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
			GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
			GXLoadPosMtxImm(viewMtx, GX_PNMTX0);
			SMS_DrawCube(min, max);

			if (unk64) {
				GXSetColorUpdate(GX_TRUE);
				GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
				               GX_LO_NOOP);
				GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
				SMS_DrawCube(min, max);
			}
		}
	} else {
		initShadowGX(mShadowColor, param_2);

		Mtx mtx;
		MTXIdentity(mtx);
		GXSetCurrentMtx(GX_PNMTX0);
		GXLoadPosMtxImm(mtx, GX_PNMTX0);
		GXLoadNrmMtxImm(mtx, GX_PNMTX0);
		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
		GXSetNumChans(1);
		GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);

		GXColor color1 = { 255, 255, 255, 128 };
		GXSetChanMatColor(GX_COLOR0A0, JUtility::TColor(color1));
		GXSetNumTexGens(0);
		GXSetNumTevStages(1);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		              GX_COLOR0A0);
		GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
		GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
		GXSetColorUpdate(GX_FALSE);
		GXSetAlphaUpdate(GX_TRUE);
		GXSetDstAlpha(GX_FALSE, 0);
		SMS_SettingDrawShape(mModelDatas[0]->getModelData(), 0);

		for (int i = 0; i < mRequestNum; i++) {
			if (param_1 & 0x40000000) {
				if (!(mQuads[i].mRequest->mActorType & 0x40000000))
					continue;
			} else if (mQuads[i].mRequest->mActorType & 0x40000000) {
				continue;
			}

			GXLoadPosMtxImm(mQuads[i].mMtx, GX_PNMTX0);
			GXSetCullMode(GX_CULL_BACK);
			GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
			SMS_DrawShape(mModelDatas[0]->getModelData(), 0);
			GXSetCullMode(GX_CULL_FRONT);
			GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
			SMS_DrawShape(mModelDatas[0]->getModelData(), 0);
		}

		GXSetCurrentMtx(GX_PNMTX0);
		GXLoadPosMtxImm(mtx, GX_PNMTX0);
		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
		GXSetCullMode(GX_CULL_FRONT);
		GXSetNumChans(1);
		GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);

		GXColor color2 = { 255, 255, 255, 255 };
		GXSetChanMatColor(GX_COLOR0A0, JUtility::TColor(color2));
		GXSetNumTevStages(1);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		              GX_COLOR0A0);
		GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_ONE, GX_LO_NOOP);
		GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition3f32(-1000.0f, 1000.0f, -200.0f);
		GXPosition3f32(1000.0f, 1000.0f, -200.0f);
		GXPosition3f32(1000.0f, -1000.0f, -200.0f);
		GXPosition3f32(-1000.0f, -1000.0f, -200.0f);
		GXEnd();

		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
		GXSetCullMode(GX_CULL_FRONT);

		GXColor color3 = { 255, 255, 255, 90 };
		GXSetChanMatColor(GX_COLOR0A0, JUtility::TColor(color3));
		GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_ZERO, GX_LO_NOOP);
		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition3f32(-1000.0f, 1000.0f, -200.0f);
		GXPosition3f32(1000.0f, 1000.0f, -200.0f);
		GXPosition3f32(1000.0f, -1000.0f, -200.0f);
		GXPosition3f32(-1000.0f, -1000.0f, -200.0f);
		GXEnd();

		GXSetColorUpdate(GX_TRUE);
		GXSetAlphaUpdate(GX_TRUE);
		GXSetDstAlpha(GX_TRUE, 0);
		GXSetChanMatColor(GX_COLOR0A0, mShadowColor);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_INVDSTALPHA,
		               GX_LO_NOOP);
		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition3f32(-1000.0f, 1000.0f, -200.0f);
		GXPosition3f32(1000.0f, 1000.0f, -200.0f);
		GXPosition3f32(1000.0f, -1000.0f, -200.0f);
		GXPosition3f32(-1000.0f, -1000.0f, -200.0f);
		GXEnd();
	}

	GXSetZCompLoc(GX_FALSE);
	GXSetColorUpdate(GX_FALSE);
	GXSetAlphaUpdate(GX_TRUE);
	GXSetDstAlpha(GX_TRUE, 0);
}

void TMBindShadowManager::request(const TCircleShadowRequest& param_1,
                                  u32 param_2)
{
	JGeometry::TVec3<f32> delta = param_1.mPosition;
	delta -= gpCamera->unk124;
	f32 dist = delta.squared();

	f32 range = 6.0f;
	if (param_1.mShadowType == SHADOW_TYPE_TREE)
		range = 10.0f;
	if (param_1.mShadowType == SHADOW_TYPE_SQUARE)
		range = 1.0f;

	if (dist > 20000000.0f * range)
		return;

	if (param_1.mRadiusX < 0.01f || param_1.mRadiusZ < 0.01f)
		return;

	if (!gpMap->isInArea(param_1.mPosition.x, param_1.mPosition.z))
		return;

	if (isnan(param_1.mPosition.x) || isnan(param_1.mPosition.z))
		return;

	if (mRequestNum < 0x200) {
		mRequests[mRequestNum]               = param_1;
		mRequests[mRequestNum].mActorType    = param_2;
		mRequests[mRequestNum].mCameraDistSq = dist;

		if (param_1.mShadowType == SHADOW_TYPE_TREE) {
			if (mModelShadowNum < 1) {
				mModelShadows[mModelShadowNum].mPosition = param_1.mPosition;
				mModelShadows[mModelShadowNum].mIsFar    = false;
				mModelShadows[mModelShadowNum].unkD      = true;

				if (dist > 200000000.0f)
					mModelShadows[mModelShadowNum].mIsFar = true;

				mModelShadowNum++;
			}
		} else {
			mRequestNum++;
		}
	}
}

void TMBindShadowManager::forceRequest(const TCircleShadowRequest& param_1,
                                       u32 param_2)
{
	JGeometry::TVec3<f32> pos   = param_1.mPosition;
	JGeometry::TVec3<f32> delta = pos;
	delta -= gpCamera->getUnk124();
	f32 dist = delta.squared();

	if (mRequestNum < 0x200) {
		mRequests[mRequestNum]               = param_1;
		mRequests[mRequestNum].mActorType    = param_2;
		mRequests[mRequestNum].mCameraDistSq = dist;
		mRequestNum++;
	}
}

void TMBindShadowManager::calcVtx()
{
	static const f32 calctablex[4] = { -1.0f, 1.0f, 1.0f, -1.0f };
	static const f32 calctablez[4] = { -1.0f, -1.0f, 1.0f, 1.0f };

	TCircleShadowRequest* request;
	int i;
	TAlphaShadowQuad* quad = mQuads;
	MtxPtr viewMtx         = j3dSys.getViewMtx();

	mSquareShadowNum = 0;

	for (i = 0; i < mRequestNum; i++) {
		request = &mRequests[i];

		JGeometry::TVec3<f32> oldPos = request->mPosition;

		if (request->mShadowType == SHADOW_TYPE_SQUARE) {
			JGeometry::TVec3<f32> foot = request->mPosition;
			JGeometry::TVec3<f32> head = foot;
			head.y += mSquareShadowHeight;

			const JGeometry::TVec3<f32>& light = gpBindShadowManager->mLightDir;

			f32 h1 = foot.y - foot.y;
			f32 h2 = head.y - foot.y;
			JGeometry::TVec3<f32> projectedFoot;
			projectedFoot.set(foot.x - light.x * h1, foot.y,
			                  foot.z - light.z * h1);
			JGeometry::TVec3<f32> projectedHead;
			projectedHead.set(head.x - light.x * h2, foot.y,
			                  head.z - light.z * h2);
			request->mPosition.set(0.5f * (projectedHead.x + projectedFoot.x),
			                       0.5f * (projectedFoot.y + projectedHead.y),
			                       0.5f * (projectedHead.z + projectedFoot.z));
		}

		JGeometry::TVec3<f32> pos = request->mPosition;

		f32 y;
		f32 groundY = y = pos.y;
		if (request->mNeedsGroundCheck) {
			f32 z = pos.z;

			const TBGCheckData* ground;
			groundY = gpMap->checkGround(pos.x, y + gpBindShadowManager->unk60,
			                             z, &ground);
			if (ground != nullptr && ground->isWaterSurface())
				groundY = gpMap->checkGroundIgnoreWaterSurface(pos.x, y, z,
				                                               &ground);
		}

		if (request->mShadowType != SHADOW_TYPE_SQUARE)
			request->mPosition.y = groundY;

		f32 shrink = 1.0f;
		if (request->mCameraDistSq > 20000000.0f || request->mRotationY != 0.0f)
			shrink = 0.2f;

		f32 radius = request->mRadiusX;
		if (request->mRadiusX < request->mRadiusZ)
			radius = request->mRadiusZ;

		f32 treeScale = 1.0f;
		f32 sx        = 0.08f * request->mRadiusX;
		f32 sy        = 0.08f * request->mRadiusZ;
		f32 sz        = 0.08f * (radius * shrink);

		JGeometry::TVec3<f32> rotation(90.0f, request->mRotationY, 0.0f);
		JGeometry::TVec3<f32> scale(sx * treeScale, sy * treeScale,
		                            treeScale * sz);

		if (request->mShadowType == SHADOW_TYPE_SHIP) {
			f32 treeScale     = mTreeScale;
			rotation.x        = 0.0f;
			scale.y           = 0.2f;
			scale.x           = sy * treeScale;
			request->mRadiusZ = 1.0f;
			scale.z           = sx * treeScale;
			request->mRadiusX = 1.0f;
		}

		JGeometry::TVec3<f32> trans = request->mPosition;

		request->mRadiusX *= 0.8f;
		request->mRadiusZ *= 0.8f;

		quad->reset();
		quad->mRequest = request;
		quad->mRadius  = request->mRadiusX;
		if (request->mRadiusX < request->mRadiusZ)
			quad->mRadius = request->mRadiusZ;
		if (quad->mRadius > 200.0f)
			quad->mRadius = 200.0f;
		quad->mRadius *= 1.1f;

		if (request->mShadowType == SHADOW_TYPE_SQUARE
		    && mSquareShadowNum < 0x1D
		    && fabsf(groundY - request->mPosition.y) < 1.0f) {
			bool done = false;

			trans      = oldPos;
			rotation.x = 0.0f;
			scale.x    = 1.0f;
			scale.y    = 1.0f;
			scale.z    = 1.0f;

			if (oldPos.x >= pos.x && !(oldPos.z < pos.z)) {
				JGeometry::TVec3<f32> base = oldPos;
				done                       = true;

				f32 dx = pos.x;
				f32 dz = pos.z;
				dx -= base.x;
				dz -= base.z;

				mSquareShadow[mSquareShadowNum].mPoints[0].x
				    = request->mRadiusX;
				mSquareShadow[mSquareShadowNum].mPoints[0].z
				    = -request->mRadiusZ;
				mSquareShadow[mSquareShadowNum].mPoints[1].x
				    = dx + request->mRadiusX;
				mSquareShadow[mSquareShadowNum].mPoints[1].z
				    = dz - request->mRadiusZ;
				mSquareShadow[mSquareShadowNum].mPoints[2].x
				    = dx - request->mRadiusX;
				mSquareShadow[mSquareShadowNum].mPoints[2].z
				    = dz - request->mRadiusZ;
				mSquareShadow[mSquareShadowNum].mPoints[3].x
				    = dx - request->mRadiusX;
				mSquareShadow[mSquareShadowNum].mPoints[3].z
				    = dz + request->mRadiusZ;
				mSquareShadow[mSquareShadowNum].mPoints[4].x
				    = -request->mRadiusX;
				mSquareShadow[mSquareShadowNum].mPoints[4].z
				    = request->mRadiusZ;
				mSquareShadow[mSquareShadowNum].mPoints[0].y = 0.0f;
				mSquareShadow[mSquareShadowNum].mPoints[1].y = 0.0f;
				mSquareShadow[mSquareShadowNum].mPoints[2].y = 0.0f;
				mSquareShadow[mSquareShadowNum].mPoints[3].y = 0.0f;
				mSquareShadow[mSquareShadowNum].mPoints[4].y = 0.0f;
			}

			if (!done) {
				JGeometry::TVec3<f32> base = oldPos;

				f32 dx = pos.x - base.x;
				f32 dz = pos.z - base.z;

				mSquareShadow[mSquareShadowNum].mPoints[0].x = 1.0f;
				mSquareShadow[mSquareShadowNum].mPoints[0].z = 1.0f;
				mSquareShadow[mSquareShadowNum].mPoints[1].x = 1.0f + dx;
				mSquareShadow[mSquareShadowNum].mPoints[1].z = dz - 1.0f;
				mSquareShadow[mSquareShadowNum].mPoints[2].x = dx - 1.0f;
				mSquareShadow[mSquareShadowNum].mPoints[2].z = dz - 1.0f;
				mSquareShadow[mSquareShadowNum].mPoints[3].x = dx - 1.0f;
				mSquareShadow[mSquareShadowNum].mPoints[3].z = 1.0f + dz;
				mSquareShadow[mSquareShadowNum].mPoints[4].x = -1.0f;
				mSquareShadow[mSquareShadowNum].mPoints[4].z = 1.0f;
				mSquareShadow[mSquareShadowNum].mPoints[0].y = 0.0f;
				mSquareShadow[mSquareShadowNum].mPoints[1].y = 0.0f;
				mSquareShadow[mSquareShadowNum].mPoints[2].y = 0.0f;
				mSquareShadow[mSquareShadowNum].mPoints[3].y = 0.0f;
				mSquareShadow[mSquareShadowNum].mPoints[4].y = 0.0f;
			}

			quad->mSquareOutline = mSquareShadow[0].mPoints;
			mSquareShadowNum++;
		}

		f32 stretch = 1.0f;
		if (request->mActorType == 0x80000001)
			stretch = 1.5f;

		MsMtxSetTRS(quad->mMtx, trans.x, trans.y, trans.z, rotation.x,
		            rotation.y, rotation.z, scale.x, scale.y,
		            scale.z * stretch);
		MTXConcat(viewMtx, quad->mMtx, quad->mMtx);

		for (int j = 0; j < 4; j++) {
			quad->mCorners[j].x
			    = request->mPosition.x + request->mRadiusX * calctablex[j];
			quad->mCorners[j].z
			    = request->mPosition.z + request->mRadiusZ * calctablez[j];
			quad->mCorners[j].y = groundY;
		}

		if (mRequestNum >= 0x200)
			return;
		quad++;
	}

	if (mRequestNum == 0)
		return;
	if (mTestSw)
		return;

	TAlphaShadowQuad* quads       = mQuads;
	TAlphaShadowBlendQuad* blends = mBlendQuads;
	TAlphaShadowQuadAry* arrays   = mQuadArys;

	for (int i = 0; i < mQuadAryNum; i++) {
		arrays[i].mQuadHead  = nullptr;
		arrays[i].mBlendHead = nullptr;
		arrays[i].mQuadTail  = nullptr;
		arrays[i].mBlendTail = nullptr;
		arrays[i].unk0       = 0x20000000;
	}

	mQuadAryNum = 0;

	for (int i = 0; i < mRequestNum; i++) {
		TAlphaShadowQuad* q      = &quads[i];
		TAlphaShadowBlendQuad* b = &blends[i];

		q->mNext = nullptr;
		b->mNext = nullptr;

		bool merged = false;

		b->mMin.x = q->mCorners[0].x;
		b->mMin.z = q->mCorners[0].z;
		b->mMax.x = q->mCorners[2].x;
		b->mMax.z = q->mCorners[2].z;
		b->mMin.y = q->mCorners[0].y;
		b->mMax.y = mYScalePlus + q->mRadius;
		b->unk18  = 0;

		for (int j = 0; j < mQuadAryNum; j++) {
			if (conectCubeDiffer(arrays[j].mBlendHead, b)) {
				merged                      = true;
				arrays[j].mQuadTail->mNext  = q;
				arrays[j].mQuadTail         = q;
				arrays[j].mBlendTail->mNext = b;
				arrays[j].mBlendTail        = b;
				break;
			}
		}

		if (!merged) {
			if (mQuadAryNum >= 0x100) {
				mQuadAryNum = 0x100;
			} else {
				arrays[mQuadAryNum].mQuadHead  = q;
				arrays[mQuadAryNum].mQuadTail  = q;
				arrays[mQuadAryNum].mBlendHead = b;
				arrays[mQuadAryNum].mBlendTail = b;
				arrays[mQuadAryNum].unk0       = 0x20000000;
				if (q->mRequest->mActorType & 0x40000000)
					arrays[mQuadAryNum].unk0 = 0x40000000;
				mQuadAryNum++;
			}
		}
	}

	for (int i = 0; i < mQuadAryNum; i++) {
		for (int j = 0; j < mQuadAryNum; j++) {
			if (i == j || arrays[i].mQuadHead == nullptr)
				continue;
			if (arrays[j].mQuadHead == nullptr)
				continue;
			if (!conectCubeSame(arrays[i].mBlendHead, arrays[j].mBlendHead))
				continue;

			arrays[i].mQuadTail->mNext  = arrays[j].mQuadHead;
			arrays[i].mQuadTail         = arrays[j].mQuadTail;
			arrays[i].mBlendTail->mNext = arrays[j].mBlendHead;
			arrays[i].mBlendTail        = arrays[j].mBlendTail;

			if ((arrays[i].mQuadHead->mRequest->mActorType & 0x40000000)
			    || (arrays[j].mQuadHead->mRequest->mActorType & 0x40000000))
				arrays[i].unk0 = 0x40000000;

			arrays[j].mQuadHead  = nullptr;
			arrays[j].mBlendHead = nullptr;
		}
	}
}
