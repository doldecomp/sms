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
    : unk0(0.01f)
    , unk4(param_3)
    , unk8(nullptr)
    , unkC(nullptr)
    , unk10(nullptr)
    , unk14(true)
    , unk15(false)
    , unk16(false)
{
	// TODO: fake
	(void)0;
	(void)0;
	(void)0;
	unk8 = param_1->getModelData()->getJointName()->getName(param_2);
	unkC = param_1->getAnmMtx(param_2);

	unk10 = param_1->getAnmMtx(((J3DJoint*)param_1->getModelData()
	                                ->getJointNodePointer(param_2)
	                                ->getChild())
	                               ->getJntNo());
	unk0  = param_4;
}

void TMBindShadowParts::calc(f32 param_1)
{
	if (!unk14)
		return;

	f32 y1 = param_1;
	f32 y2 = param_1;

	f32 dist = fabsf(unk4->unk4->mPosition.y - param_1);

	const JGeometry::TVec3<f32>& light = gpBindShadowManager->unk30;

	f32 lightX = light.x;
	f32 lightZ = light.z;
	f32 h1     = unkC[1][3] - dist - param_1;
	f32 h2     = unk10[1][3] - dist - param_1;
	f32 x1, z1, x2, z2;
	x1 = unkC[0][3] - lightX * h1;
	z1 = unkC[2][3] - lightZ * h1;
	x2 = unk10[0][3] - lightX * h2;
	z2 = unk10[2][3] - lightZ * h2;
	JGeometry::TVec3<f32> center;
	center.set(0.5f * (x1 + x2), 0.5f * (y1 + y2), 0.5f * (z1 + z2));
	f32 cx = center.x;
	f32 cy = center.y;
	f32 cz = center.z;

	f32 radiusX;
	f32 radiusZ;
	radiusZ = fabsf(cz - z2);
	radiusX = fabsf(cx - x2);

	if (unk16)
		unk0 = unk4->unk18;
	else if (unk15)
		unk0 = unk4->unk10;
	else
		unk0 = unk4->unk14;

	if (radiusX < unk0)
		radiusX = unk0;
	if (radiusZ < unk0)
		radiusZ = unk0;

	if (!unk15) {
		f32 ratio = gpBindShadowManager->unk68;
		f32 scale = gpBindShadowManager->unk6C;
		if (radiusZ > radiusX) {
			if (radiusZ > ratio * radiusX)
				radiusZ *= scale;
			if (radiusX > unk0)
				radiusX = unk0;
		} else if (radiusX > radiusZ) {
			if (radiusX > ratio * radiusZ)
				radiusX *= scale;
			if (radiusZ > unk0)
				radiusZ = unk0;
		}
	}

	TCircleShadowRequest request;
	request.unk0.set(cx, cy, cz);
	request.unkC  = radiusX;
	request.unk10 = radiusZ;

	if (!unk15 && unk4->unk4->mActorType != 0x80000001
	    && unk4->unk4->mActorType != 0x8000002) {
		f32 rotY = matan(z2 - z1, x2 - x1) * (360.0f / 65536.0f);
		if (radiusX > radiusZ)
			rotY -= 90.0f;
		request.unk14 = rotY;
	} else {
		request.unk14 = 0.0f;
	}

	gpBindShadowManager->request(request, unk4->unk4->mActorType);
}

TMBindShadowBody::TMBindShadowBody(THitActor* param_1, J3DModel* param_2,
                                   f32 param_3)
    : unk0(nullptr)
    , unk4(param_1)
    , unk8(0)
    , unkC(param_1->getName())
    , unk10(0.01f)
    , unk14(0.01f)
    , unk18(50.0f)
{
	switch (param_1->mActorType) {
	case 0x80000001:
	case 0x8000002:
		unk10 = 38.0f;
		unk14 = 18.0f;
		unk18 = 25.0f;
		break;
	case 0x8000001:
		unk10 = 280.0f;
		unk14 = 50.0f;
		break;
	default:
		unk10 = 50.0f;
		unk14 = 50.0f;
		break;
	}

	unk10 *= param_3;
	unk14 *= param_3;

	J3DModelData* modelData = param_2->getModelData();

	for (int i = 0; i < modelData->getJointNum(); i++) {
		if (modelData->getJointNodePointer((u8)i)->getKind() == 1
		    && isUseThisJoint(i))
			unk8++;
	}

	unk0 = new TMBindShadowParts*[unk8];

	int count = 0;
	for (int i = 0; i < modelData->getJointNum(); i++) {
		if (modelData->getJointNodePointer((u8)i)->getKind() != 1
		    || !isUseThisJoint(i))
			continue;

		if (isCircleJoint(i)) {
			unk0[count]        = new TMBindShadowParts(param_2, i, this, unk10);
			unk0[count]->unk15 = true;
		} else if (isBodyJoint(i)) {
			unk0[count]        = new TMBindShadowParts(param_2, i, this, unk18);
			unk0[count]->unk16 = true;
		} else {
			unk0[count] = new TMBindShadowParts(param_2, i, this, unk14);
		}

		count++;
	}

	gpBindShadowManager->initEntry(this);
}

bool TMBindShadowBody::isUseThisJoint(int param_1)
{
	const THitActor* actor = unk4;
	switch (actor->mActorType) {
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
	switch (unk4->mActorType) {
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
	switch (unk4->mActorType) {
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

	if (gpMarioPos->epsilonEquals(unk4->mPosition, eps)) {
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
	JGeometry::TVec3<f32> pos = unk4->mPosition;

	f32 y = pos.y;
	f32 z = pos.z;

	const TBGCheckData* ground;
	f32 groundY
	    = gpMap->checkGround(pos.x, y + gpBindShadowManager->unk60, z, &ground);

	if (ground->isWaterSurface())
		groundY = gpMap->checkGround(pos.x, y - 50.0f, z, &ground);

	if (ground->isIllegalData())
		return;

	for (int i = 0; i < unk8; i++)
		unk0[i]->calc(groundY);
}

TSquareShadowInfo::TSquareShadowInfo()
{
	for (Vec* p = &unk0[0]; p != &unk0[5]; p++) {
		p->x = 0.0f;
		p->y = 0.0f;
		p->y = 0.0f;
	}
}

TModelShadowInfo::TModelShadowInfo()
    : unk0(0.0f, 0.0f, 0.0f)
    , unkC(0)
    , unkD(1)
    , unk10(0.01f)
{
}

void TAlphaShadowQuad::reset()
{
	unk0  = 0.01f;
	unk64 = nullptr;
	unk68 = nullptr;
	unk6C = nullptr;
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
    , unk14(0)
    , unk20(0)
    , unk2C(0)
    , unk40(0)
    , unk44(0)
    , unk48(0)
    , unk49(0)
    , unk4C()
    , unk60(30.0f)
    , unk64(0)
    , unk65(false)
    , unk68(0.5f)
    , unk6C(1.55f)
    , unk70(nullptr)
{
	unk5C.r = 30;
	unk5C.g = 50;
	unk5C.b = 115;
	unk5C.a = 180;

	switch (gpApplication.mCurrArea.unk0) {
	case 6:
		unk5C.r = 9;
		unk5C.g = 9;
		unk5C.b = 28;
		unk5C.a = 116;
		break;
	case 7:
		unk5C.r = 45;
		unk5C.g = 40;
		unk5C.b = 60;
		unk5C.a = 90;
		break;
	}

	gpBindShadowManager = this;

	unk10 = new TCircleShadowRequest[0x200];
	unk18 = new TAlphaShadowQuad[0x200];
	unk1C = new TAlphaShadowQuadAry[0x100];
	unk24 = new TAlphaShadowBlendQuad[0x200];
	unk28 = new TSquareShadowInfo[0x1E];
	unk70 = new TModelShadowInfo[1];
	unk3C = new SDLModelData*[5];
}

void TMBindShadowManager::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);

	void* resource = JKRFileLoader::getGlbResource("/common/shadowCircle.bmd");
	unk3C[0]
	    = new SDLModelData(J3DModelLoaderDataBase::load(resource, 0x10210000));

	resource = JKRFileLoader::getGlbResource("/common/shadowCircleLow.bmd");
	unk3C[1]
	    = new SDLModelData(J3DModelLoaderDataBase::load(resource, 0x10210000));

	resource = JKRFileLoader::getGlbResource("/common/shadowCube.bmd");
	unk3C[2]
	    = new SDLModelData(J3DModelLoaderDataBase::load(resource, 0x10210000));

	resource = JKRFileLoader::getGlbResource("/common/ShipShadow.bmd");
	unk3C[3]
	    = new SDLModelData(J3DModelLoaderDataBase::load(resource, 0x10210000));

	reset();
}

void TMBindShadowManager::reset()
{
	unk49 = 1;
	unk14 = 0;
	unk20 = 0;
	unk65 = false;
	unk2C = 0;
	unk40 = 0;
}

void TMBindShadowManager::initEntry(TMBindShadowBody* param_1)
{
	unk4C.push_back(param_1);
}

void TMBindShadowManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_VIEW) {
		unk49 = 0;
		VECNormalize(gpLightManager->getLightPos(), &unk30);
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

	if (fabsf(param_1->unk0.y - param_2->unk0.y) > 50.0f)
		return false;

	if (param_1->unk0.x <= param_2->unkC.x && param_1->unkC.x >= param_2->unk0.x
	    && param_1->unk0.z <= param_2->unkC.z
	    && param_1->unkC.z >= param_2->unk0.z) {
		if (param_1->unkC.x <= param_2->unkC.x)
			param_1->unkC.x = param_2->unkC.x;
		if (param_1->unk0.x >= param_2->unk0.x)
			param_1->unk0.x = param_2->unk0.x;
		if (param_1->unkC.z <= param_2->unkC.z)
			param_1->unkC.z = param_2->unkC.z;
		if (param_1->unk0.z >= param_2->unk0.z)
			param_1->unk0.z = param_2->unk0.z;
		if (param_1->unk0.y >= param_2->unk0.y)
			param_1->unk0.y = param_2->unk0.y;
		if (param_1->unkC.y <= param_2->unkC.y)
			param_1->unkC.y = param_2->unkC.y;
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

	if (fabsf(param_1->unk0.y - param_2->unk0.y) > 50.0f)
		return false;

	if (param_1->unk0.x <= param_2->unkC.x - dist
	    && param_1->unkC.x >= param_2->unk0.x + dist
	    && param_1->unk0.z <= param_2->unkC.z - dist
	    && param_1->unkC.z >= param_2->unk0.z + dist) {
		if (param_1->unkC.x <= param_2->unkC.x - dist)
			param_1->unkC.x = param_2->unkC.x;
		if (param_1->unk0.x >= param_2->unk0.x + dist)
			param_1->unk0.x = param_2->unk0.x;
		if (param_1->unkC.z <= param_2->unkC.z - dist)
			param_1->unkC.z = param_2->unkC.z;
		if (param_1->unk0.z >= param_2->unk0.z + dist)
			param_1->unk0.z = param_2->unk0.z;
		if (param_1->unk0.y >= param_2->unk0.y)
			param_1->unk0.y = param_2->unk0.y;
		if (param_1->unkC.y <= param_2->unkC.y)
			param_1->unkC.y = param_2->unkC.y;
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
	// TODO: instruction-identical and the frame size is right, but the two
	// index arrays sit 4 bytes lower than in the map, so one 4-byte local is
	// still missing after them.
	f32 height = 50.0f;
	if (param_2->unk68->unk1C == 1) {
		if (param_2->unk64 == nullptr) {
			SMS_SettingDrawShape(unk3C[2]->getModelData(), 0);
			SMS_DrawShape(unk3C[2]->getModelData(), 0);
		} else {
			int topIndices[9]    = { 2, 1, 0, 3, 2, 0, 4, 3, 0 };
			int bottomIndices[9] = { 0, 1, 2, 0, 2, 3, 0, 3, 4 };

			GXClearVtxDesc();
			GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
			GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);

			GXBegin(GX_TRIANGLES, GX_VTXFMT0, 18);

			for (int i = 0; i < 9; i++) {
				Vec* v = &param_2->unk64[topIndices[i]];
				GXPosition3f32(v->x, 50.0f + v->y, v->z);
			}

			for (int i = 0; i < 9; i++) {
				Vec* v = &param_2->unk64[bottomIndices[i]];
				GXPosition3f32(v->x, v->y - 50.0f, v->z);
			}

			GXEnd();

			GXBegin(GX_TRIANGLES, GX_VTXFMT0, 60);

			GXPosition3f32(param_2->unk64[0].x, param_2->unk64[0].y + height,
			               param_2->unk64[0].z);
			GXPosition3f32(param_2->unk64[1].x, param_2->unk64[1].y + height,
			               param_2->unk64[1].z);
			GXPosition3f32(param_2->unk64[1].x, param_2->unk64[1].y - 50.0f,
			               param_2->unk64[1].z);
			GXPosition3f32(param_2->unk64[1].x, param_2->unk64[1].y - 50.0f,
			               param_2->unk64[1].z);
			GXPosition3f32(param_2->unk64[0].x, param_2->unk64[0].y - 50.0f,
			               param_2->unk64[0].z);
			GXPosition3f32(param_2->unk64[0].x, param_2->unk64[0].y + height,
			               param_2->unk64[0].z);
			GXPosition3f32(param_2->unk64[1].x, param_2->unk64[1].y + height,
			               param_2->unk64[1].z);
			GXPosition3f32(param_2->unk64[0].x, param_2->unk64[0].y + height,
			               param_2->unk64[0].z);
			GXPosition3f32(param_2->unk64[1].x, param_2->unk64[1].y - 50.0f,
			               param_2->unk64[1].z);
			GXPosition3f32(param_2->unk64[1].x, param_2->unk64[1].y - 50.0f,
			               param_2->unk64[1].z);
			GXPosition3f32(param_2->unk64[0].x, param_2->unk64[0].y + height,
			               param_2->unk64[0].z);
			GXPosition3f32(param_2->unk64[0].x, param_2->unk64[0].y - 50.0f,
			               param_2->unk64[0].z);

			GXPosition3f32(param_2->unk64[1].x, param_2->unk64[1].y + height,
			               param_2->unk64[1].z);
			GXPosition3f32(param_2->unk64[2].x, param_2->unk64[2].y + height,
			               param_2->unk64[2].z);
			GXPosition3f32(param_2->unk64[2].x, param_2->unk64[2].y - 50.0f,
			               param_2->unk64[2].z);
			GXPosition3f32(param_2->unk64[2].x, param_2->unk64[2].y - 50.0f,
			               param_2->unk64[2].z);
			GXPosition3f32(param_2->unk64[1].x, param_2->unk64[1].y - 50.0f,
			               param_2->unk64[1].z);
			GXPosition3f32(param_2->unk64[1].x, param_2->unk64[1].y + height,
			               param_2->unk64[1].z);
			GXPosition3f32(param_2->unk64[2].x, param_2->unk64[2].y + height,
			               param_2->unk64[2].z);
			GXPosition3f32(param_2->unk64[1].x, param_2->unk64[1].y + height,
			               param_2->unk64[1].z);
			GXPosition3f32(param_2->unk64[2].x, param_2->unk64[2].y - 50.0f,
			               param_2->unk64[2].z);
			GXPosition3f32(param_2->unk64[2].x, param_2->unk64[2].y - 50.0f,
			               param_2->unk64[2].z);
			GXPosition3f32(param_2->unk64[1].x, param_2->unk64[1].y + height,
			               param_2->unk64[1].z);
			GXPosition3f32(param_2->unk64[1].x, param_2->unk64[1].y - 50.0f,
			               param_2->unk64[1].z);

			GXPosition3f32(param_2->unk64[2].x, param_2->unk64[2].y + height,
			               param_2->unk64[2].z);
			GXPosition3f32(param_2->unk64[3].x, param_2->unk64[3].y + height,
			               param_2->unk64[3].z);
			GXPosition3f32(param_2->unk64[3].x, param_2->unk64[3].y - 50.0f,
			               param_2->unk64[3].z);
			GXPosition3f32(param_2->unk64[3].x, param_2->unk64[3].y - 50.0f,
			               param_2->unk64[3].z);
			GXPosition3f32(param_2->unk64[2].x, param_2->unk64[2].y - 50.0f,
			               param_2->unk64[2].z);
			GXPosition3f32(param_2->unk64[2].x, param_2->unk64[2].y + height,
			               param_2->unk64[2].z);
			GXPosition3f32(param_2->unk64[3].x, param_2->unk64[3].y + height,
			               param_2->unk64[3].z);
			GXPosition3f32(param_2->unk64[2].x, param_2->unk64[2].y + height,
			               param_2->unk64[2].z);
			GXPosition3f32(param_2->unk64[3].x, param_2->unk64[3].y - 50.0f,
			               param_2->unk64[3].z);
			GXPosition3f32(param_2->unk64[3].x, param_2->unk64[3].y - 50.0f,
			               param_2->unk64[3].z);
			GXPosition3f32(param_2->unk64[2].x, param_2->unk64[2].y + height,
			               param_2->unk64[2].z);
			GXPosition3f32(param_2->unk64[2].x, param_2->unk64[2].y - 50.0f,
			               param_2->unk64[2].z);

			GXPosition3f32(param_2->unk64[3].x, param_2->unk64[3].y + height,
			               param_2->unk64[3].z);
			GXPosition3f32(param_2->unk64[4].x, param_2->unk64[4].y + height,
			               param_2->unk64[4].z);
			GXPosition3f32(param_2->unk64[4].x, param_2->unk64[4].y - 50.0f,
			               param_2->unk64[4].z);
			GXPosition3f32(param_2->unk64[4].x, param_2->unk64[4].y - 50.0f,
			               param_2->unk64[4].z);
			GXPosition3f32(param_2->unk64[3].x, param_2->unk64[3].y - 50.0f,
			               param_2->unk64[3].z);
			GXPosition3f32(param_2->unk64[3].x, param_2->unk64[3].y + height,
			               param_2->unk64[3].z);
			GXPosition3f32(param_2->unk64[4].x, param_2->unk64[4].y + height,
			               param_2->unk64[4].z);
			GXPosition3f32(param_2->unk64[3].x, param_2->unk64[3].y + height,
			               param_2->unk64[3].z);
			GXPosition3f32(param_2->unk64[4].x, param_2->unk64[4].y - 50.0f,
			               param_2->unk64[4].z);
			GXPosition3f32(param_2->unk64[4].x, param_2->unk64[4].y - 50.0f,
			               param_2->unk64[4].z);
			GXPosition3f32(param_2->unk64[3].x, param_2->unk64[3].y + height,
			               param_2->unk64[3].z);
			GXPosition3f32(param_2->unk64[3].x, param_2->unk64[3].y - 50.0f,
			               param_2->unk64[3].z);

			GXPosition3f32(param_2->unk64[4].x, param_2->unk64[4].y + height,
			               param_2->unk64[4].z);
			GXPosition3f32(param_2->unk64[0].x, param_2->unk64[0].y + height,
			               param_2->unk64[0].z);
			GXPosition3f32(param_2->unk64[0].x, param_2->unk64[0].y - 50.0f,
			               param_2->unk64[0].z);
			GXPosition3f32(param_2->unk64[0].x, param_2->unk64[0].y - 50.0f,
			               param_2->unk64[0].z);
			GXPosition3f32(param_2->unk64[4].x, param_2->unk64[4].y - 50.0f,
			               param_2->unk64[4].z);
			GXPosition3f32(param_2->unk64[4].x, param_2->unk64[4].y + height,
			               param_2->unk64[4].z);
			GXPosition3f32(param_2->unk64[0].x, param_2->unk64[0].y + height,
			               param_2->unk64[0].z);
			GXPosition3f32(param_2->unk64[4].x, param_2->unk64[4].y + height,
			               param_2->unk64[4].z);
			GXPosition3f32(param_2->unk64[0].x, param_2->unk64[0].y - 50.0f,
			               param_2->unk64[0].z);
			GXPosition3f32(param_2->unk64[0].x, param_2->unk64[0].y - 50.0f,
			               param_2->unk64[0].z);
			GXPosition3f32(param_2->unk64[4].x, param_2->unk64[4].y + height,
			               param_2->unk64[4].z);
			GXPosition3f32(param_2->unk64[4].x, param_2->unk64[4].y - 50.0f,
			               param_2->unk64[4].z);

			GXEnd();
		}
	} else if (param_2->unk68->unk1C == 3) {
		SMS_SettingDrawShape(unk3C[3]->getModelData(), 0);
		SMS_DrawShape(unk3C[3]->getModelData(), 0);
	} else {
		if (param_1)
			SMS_DrawShape(unk3C[0]->getModelData(), 0);
		else
			SMS_DrawShape(unk3C[1]->getModelData(), 0);
		return;
	}

	if (param_1)
		SMS_SettingDrawShape(unk3C[0]->getModelData(), 0);
	else
		SMS_SettingDrawShape(unk3C[1]->getModelData(), 0);
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

	// TODO: instruction-identical, but the frame is 0x188 against the map's
	// 0x400. The 8 spill slots sit right below the register save area in
	// both, so the deficit is a block of locals below them that this
	// reconstruction does not name yet.
	GXSetZCompLoc(GX_TRUE);

	static TSetup1 setup1(0x100);
	setup1.callDL();

	static TCylinder cylinder(0x400);

	MtxPtr viewMtx = param_2->getViewMtx();

	for (int i = 0; i < unk20; i++) {
		TAlphaShadowQuadAry* ary = &unk1C[i];

		if (ary->unk4 == nullptr || ary->unkC == nullptr)
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

		TAlphaShadowBlendQuad* blend = unk1C[i].unkC;

		f32 minX;
		f32 y1;
		f32 minZ;
		f32 maxX;
		f32 maxZ;
		f32 y2;

		y2 = blend->unk0.y;
		y1 = y2 - blend->unkC.y;
		y2 += blend->unkC.y;
		minX = blend->unk0.x;
		minZ = blend->unk0.z;
		maxX = blend->unkC.x;
		maxZ = blend->unkC.z;

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

		TAlphaShadowQuad* quad = unk1C[i].unk4;
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
		if (quad->unk68->unk18 < 20000000.0f) {
			SMS_SettingDrawShape(unk3C[0]->getModelData(), 0);
			lowPoly = 1;
		} else {
			SMS_SettingDrawShape(unk3C[1]->getModelData(), 0);
		}

		while (quad != nullptr) {
			loadPosMtxImm(quad->unk4);
			drawShadowVolume(lowPoly, quad);
			quad = quad->unk6C;
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

		quad = unk1C[i].unk4;
		while (quad != nullptr) {
			loadPosMtxImm(quad->unk4);
			drawShadowVolume(lowPoly, quad);
			quad = quad->unk6C;
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
		initShadowGX(unk5C, param_2);

		MtxPtr viewMtx = param_2->getViewMtx();

		for (int i = 0; i < unk20; i++) {
			TAlphaShadowQuadAry* ary = &unk1C[i];

			if (ary->unk4 == nullptr || ary->unkC == nullptr)
				continue;

			if (!(param_1 & ary->unk0))
				continue;

			GXSetCullMode(GX_CULL_NONE);
			GXLoadPosMtxImm(param_2->getViewMtx(), GX_PNMTX0);
			GXSetColorUpdate(GX_FALSE);
			GXSetDstAlpha(GX_TRUE, 0);
			GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
			GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);

			TAlphaShadowBlendQuad* blend = unk1C[i].unkC;

			JGeometry::TVec3<f32> max;
			JGeometry::TVec3<f32> min;

			min.set(blend->unk0.x, blend->unk0.y - blend->unkC.y,
			        blend->unk0.z);
			max.set(blend->unkC.x, blend->unk0.y + blend->unkC.y,
			        blend->unkC.z);
			SMS_DrawCube(min, max);

			u8 lowPoly             = 0;
			TAlphaShadowQuad* quad = unk1C[i].unk4;

			if (quad->unk68->unk18 < 20000000.0f) {
				SMS_SettingDrawShape(unk3C[0]->getModelData(), 0);
				lowPoly = 1;
			} else {
				SMS_SettingDrawShape(unk3C[1]->getModelData(), 0);
			}

			GXSetDstAlpha(GX_FALSE, 0);
			GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
			GXSetCullMode(GX_CULL_BACK);
			GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);

			while (quad != nullptr) {
				GXLoadPosMtxImm(quad->unk4, GX_PNMTX0);
				drawShadowVolume(lowPoly, quad);
				quad = quad->unk6C;
			}

			GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_INVDSTALPHA,
			               GX_LO_NOOP);
			GXSetZMode(GX_TRUE, GX_GEQUAL, GX_FALSE);
			GXSetCullMode(GX_CULL_FRONT);
			GXSetDstAlpha(GX_TRUE, 0);
			GXSetColorUpdate(GX_TRUE);

			quad = unk1C[i].unk4;
			while (quad != nullptr) {
				GXLoadPosMtxImm(quad->unk4, GX_PNMTX0);
				drawShadowVolume(lowPoly, quad);
				quad = quad->unk6C;
			}

			GXSetCullMode(GX_CULL_BACK);
			GXSetColorUpdate(GX_FALSE);
			GXSetDstAlpha(GX_TRUE, 0);
			GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);

			quad = unk1C[i].unk4;
			while (quad != nullptr) {
				if (quad->unk68->unk1C == 3) {
					GXLoadPosMtxImm(quad->unk4, GX_PNMTX0);
					SMS_SettingDrawShape(unk3C[3]->getModelData(), 0);
					SMS_DrawShape(unk3C[3]->getModelData(), 0);
				}
				quad = quad->unk6C;
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
		initShadowGX(unk5C, param_2);

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
		SMS_SettingDrawShape(unk3C[0]->getModelData(), 0);

		for (int i = 0; i < unk14; i++) {
			if (param_1 & 0x40000000) {
				if (!(unk18[i].unk68->unk20 & 0x40000000))
					continue;
			} else if (unk18[i].unk68->unk20 & 0x40000000) {
				continue;
			}

			GXLoadPosMtxImm(unk18[i].unk4, GX_PNMTX0);
			GXSetCullMode(GX_CULL_BACK);
			GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
			SMS_DrawShape(unk3C[0]->getModelData(), 0);
			GXSetCullMode(GX_CULL_FRONT);
			GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
			SMS_DrawShape(unk3C[0]->getModelData(), 0);
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
		GXSetChanMatColor(GX_COLOR0A0, unk5C);
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
	// TODO: instruction-identical, but the frame is 0x28 short of the map.
	JGeometry::TVec3<f32> delta = param_1.unk0;
	delta -= gpCamera->unk124;
	f32 dist = delta.squared();

	f32 range = 6.0f;
	if (param_1.unk1C == 2)
		range = 10.0f;
	if (param_1.unk1C == 1)
		range = 1.0f;

	if (dist > 20000000.0f * range)
		return;

	if (param_1.unkC < 0.01f || param_1.unk10 < 0.01f)
		return;

	if (!gpMap->isInArea(param_1.unk0.x, param_1.unk0.z))
		return;

	if (isnan(param_1.unk0.x) || isnan(param_1.unk0.z))
		return;

	if (unk14 < 0x200) {
		unk10[unk14]       = param_1;
		unk10[unk14].unk20 = param_2;
		unk10[unk14].unk18 = dist;

		if (param_1.unk1C == 2) {
			if (unk40 < 1) {
				unk70[unk40].unk0 = param_1.unk0;
				unk70[unk40].unkC = 0;
				unk70[unk40].unkD = 1;

				if (dist > 200000000.0f)
					unk70[unk40].unkC = 1;

				unk40++;
			}
		} else {
			unk14++;
		}
	}
}

void TMBindShadowManager::forceRequest(const TCircleShadowRequest& param_1,
                                       u32 param_2)
{
	JGeometry::TVec3<f32> pos   = param_1.unk0;
	JGeometry::TVec3<f32> delta = pos;
	delta -= gpCamera->getUnk124();
	f32 dist = delta.squared();

	if (unk14 < 0x200) {
		unk10[unk14]       = param_1;
		unk10[unk14].unk20 = param_2;
		unk10[unk14].unk18 = dist;
		unk14++;
	}
}

void TMBindShadowManager::calcVtx()
{
	static const f32 calctablex[4] = { -1.0f, 1.0f, 1.0f, -1.0f };
	static const f32 calctablez[4] = { -1.0f, -1.0f, 1.0f, 1.0f };

	TCircleShadowRequest* request;
	int i;
	TAlphaShadowQuad* quad = unk18;
	MtxPtr viewMtx         = j3dSys.getViewMtx();

	unk2C = 0;

	for (i = 0; i < unk14; i++) {
		request = &unk10[i];

		JGeometry::TVec3<f32> oldPos = request->unk0;

		if (request->unk1C == 1) {
			JGeometry::TVec3<f32> foot = request->unk0;
			JGeometry::TVec3<f32> head = foot;
			head.y += mSquareShadowHeight;

			const JGeometry::TVec3<f32>& light = gpBindShadowManager->unk30;

			f32 h1 = foot.y - foot.y;
			f32 h2 = head.y - foot.y;
			JGeometry::TVec3<f32> projectedFoot;
			projectedFoot.set(foot.x - light.x * h1, foot.y,
			                  foot.z - light.z * h1);
			JGeometry::TVec3<f32> projectedHead;
			projectedHead.set(head.x - light.x * h2, foot.y,
			                  head.z - light.z * h2);
			request->unk0.set(0.5f * (projectedHead.x + projectedFoot.x),
			                  0.5f * (projectedFoot.y + projectedHead.y),
			                  0.5f * (projectedHead.z + projectedFoot.z));
		}

		JGeometry::TVec3<f32> pos = request->unk0;

		f32 y;
		f32 groundY = y = pos.y;
		if (request->unk1D) {
			f32 z = pos.z;

			const TBGCheckData* ground;
			groundY = gpMap->checkGround(pos.x, y + gpBindShadowManager->unk60,
			                             z, &ground);
			if (ground != nullptr && ground->isWaterSurface())
				groundY = gpMap->checkGroundIgnoreWaterSurface(pos.x, y, z,
				                                               &ground);
		}

		if (request->unk1C != 1)
			request->unk0.y = groundY;

		f32 shrink = 1.0f;
		if (request->unk18 > 20000000.0f || request->unk14 != 0.0f)
			shrink = 0.2f;

		f32 radius = request->unkC;
		if (request->unkC < request->unk10)
			radius = request->unk10;

		f32 treeScale = 1.0f;
		f32 sx        = 0.08f * request->unkC;
		f32 sy        = 0.08f * request->unk10;
		f32 sz        = 0.08f * (radius * shrink);

		JGeometry::TVec3<f32> rotation(90.0f, request->unk14, 0.0f);
		JGeometry::TVec3<f32> scale(sx * treeScale, sy * treeScale,
		                            treeScale * sz);

		if (request->unk1C == 3) {
			f32 treeScale  = mTreeScale;
			rotation.x     = 0.0f;
			scale.y        = 0.2f;
			scale.x        = sy * treeScale;
			request->unk10 = 1.0f;
			scale.z        = sx * treeScale;
			request->unkC  = 1.0f;
		}

		JGeometry::TVec3<f32> trans = request->unk0;

		request->unkC *= 0.8f;
		request->unk10 *= 0.8f;

		quad->reset();
		quad->unk68 = request;
		quad->unk0  = request->unkC;
		if (request->unkC < request->unk10)
			quad->unk0 = request->unk10;
		if (quad->unk0 > 200.0f)
			quad->unk0 = 200.0f;
		quad->unk0 *= 1.1f;

		if (request->unk1C == 1 && unk2C < 0x1D
		    && fabsf(groundY - request->unk0.y) < 1.0f) {
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

				unk28[unk2C].unk0[0].x = request->unkC;
				unk28[unk2C].unk0[0].z = -request->unk10;
				unk28[unk2C].unk0[1].x = dx + request->unkC;
				unk28[unk2C].unk0[1].z = dz - request->unk10;
				unk28[unk2C].unk0[2].x = dx - request->unkC;
				unk28[unk2C].unk0[2].z = dz - request->unk10;
				unk28[unk2C].unk0[3].x = dx - request->unkC;
				unk28[unk2C].unk0[3].z = dz + request->unk10;
				unk28[unk2C].unk0[4].x = -request->unkC;
				unk28[unk2C].unk0[4].z = request->unk10;
				unk28[unk2C].unk0[0].y = 0.0f;
				unk28[unk2C].unk0[1].y = 0.0f;
				unk28[unk2C].unk0[2].y = 0.0f;
				unk28[unk2C].unk0[3].y = 0.0f;
				unk28[unk2C].unk0[4].y = 0.0f;
			}

			if (!done) {
				JGeometry::TVec3<f32> base = oldPos;

				f32 dx = pos.x - base.x;
				f32 dz = pos.z - base.z;

				unk28[unk2C].unk0[0].x = 1.0f;
				unk28[unk2C].unk0[0].z = 1.0f;
				unk28[unk2C].unk0[1].x = 1.0f + dx;
				unk28[unk2C].unk0[1].z = dz - 1.0f;
				unk28[unk2C].unk0[2].x = dx - 1.0f;
				unk28[unk2C].unk0[2].z = dz - 1.0f;
				unk28[unk2C].unk0[3].x = dx - 1.0f;
				unk28[unk2C].unk0[3].z = 1.0f + dz;
				unk28[unk2C].unk0[4].x = -1.0f;
				unk28[unk2C].unk0[4].z = 1.0f;
				unk28[unk2C].unk0[0].y = 0.0f;
				unk28[unk2C].unk0[1].y = 0.0f;
				unk28[unk2C].unk0[2].y = 0.0f;
				unk28[unk2C].unk0[3].y = 0.0f;
				unk28[unk2C].unk0[4].y = 0.0f;
			}

			quad->unk64 = unk28[0].unk0;
			unk2C++;
		}

		f32 stretch = 1.0f;
		if (request->unk20 == 0x80000001)
			stretch = 1.5f;

		MsMtxSetTRS(quad->unk4, trans.x, trans.y, trans.z, rotation.x,
		            rotation.y, rotation.z, scale.x, scale.y,
		            scale.z * stretch);
		MTXConcat(viewMtx, quad->unk4, quad->unk4);

		for (int j = 0; j < 4; j++) {
			quad->unk34[j].x = request->unk0.x + request->unkC * calctablex[j];
			quad->unk34[j].z = request->unk0.z + request->unk10 * calctablez[j];
			quad->unk34[j].y = groundY;
		}

		if (unk14 >= 0x200)
			return;
		quad++;
	}

	if (unk14 == 0)
		return;
	if (mTestSw)
		return;

	TAlphaShadowQuad* quads       = unk18;
	TAlphaShadowBlendQuad* blends = unk24;
	TAlphaShadowQuadAry* arrays   = unk1C;

	for (int i = 0; i < unk20; i++) {
		arrays[i].unk4  = nullptr;
		arrays[i].unkC  = nullptr;
		arrays[i].unk8  = nullptr;
		arrays[i].unk10 = nullptr;
		arrays[i].unk0  = 0x20000000;
	}

	unk20 = 0;

	for (int i = 0; i < unk14; i++) {
		TAlphaShadowQuad* q      = &quads[i];
		TAlphaShadowBlendQuad* b = &blends[i];

		q->unk6C = nullptr;
		b->unk1C = nullptr;

		bool merged = false;

		b->unk0.x = q->unk34[0].x;
		b->unk0.z = q->unk34[0].z;
		b->unkC.x = q->unk34[2].x;
		b->unkC.z = q->unk34[2].z;
		b->unk0.y = q->unk34[0].y;
		b->unkC.y = mYScalePlus + q->unk0;
		b->unk18  = 0;

		for (int j = 0; j < unk20; j++) {
			if (conectCubeDiffer(arrays[j].unkC, b)) {
				merged                 = true;
				arrays[j].unk8->unk6C  = q;
				arrays[j].unk8         = q;
				arrays[j].unk10->unk1C = b;
				arrays[j].unk10        = b;
				break;
			}
		}

		if (!merged) {
			if (unk20 >= 0x100) {
				unk20 = 0x100;
			} else {
				arrays[unk20].unk4  = q;
				arrays[unk20].unk8  = q;
				arrays[unk20].unkC  = b;
				arrays[unk20].unk10 = b;
				arrays[unk20].unk0  = 0x20000000;
				if (q->unk68->unk20 & 0x40000000)
					arrays[unk20].unk0 = 0x40000000;
				unk20++;
			}
		}
	}

	for (int i = 0; i < unk20; i++) {
		for (int j = 0; j < unk20; j++) {
			if (i == j || arrays[i].unk4 == nullptr)
				continue;
			if (arrays[j].unk4 == nullptr)
				continue;
			if (!conectCubeSame(arrays[i].unkC, arrays[j].unkC))
				continue;

			arrays[i].unk8->unk6C  = arrays[j].unk4;
			arrays[i].unk8         = arrays[j].unk8;
			arrays[i].unk10->unk1C = arrays[j].unkC;
			arrays[i].unk10        = arrays[j].unk10;

			if ((arrays[i].unk4->unk68->unk20 & 0x40000000)
			    || (arrays[j].unk4->unk68->unk20 & 0x40000000))
				arrays[i].unk0 = 0x40000000;

			arrays[j].unk4 = nullptr;
			arrays[j].unkC = nullptr;
		}
	}
}
