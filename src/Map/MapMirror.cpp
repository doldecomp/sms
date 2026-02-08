#include <Map/MapMirror.hpp>
#include <Map/MapData.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorData.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <System/MarDirector.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/Camera.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TMirrorCamera::makeMirrorViewMtx() { }

void TMirrorCamera::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 0x14) {
		C_MTXPerspective(param_2->mProjMtx.mMtx, unk80 * gpCamera->mFovy,
		                 gpCamera->mAspect, gpCamera->mNear, gpCamera->mFar);
		MTXCopy(unk30, param_2->mViewMtx);
		param_2->mNearPlane = gpCamera->mNear;
		param_2->mFarPlane  = gpCamera->mFar;
		if (param_1 & 0x10)
			GXSetProjection(param_2->mProjMtx.mMtx, GX_PERSPECTIVE);
		GXSetAlphaUpdate(GX_TRUE);
	}
}

void TMirrorCamera::drawSetting(MtxPtr param_1)
{
	GXLoadTexObj(&unk60, GX_TEXMAP0);
	Mtx afStack_38;
	C_MTXLightPerspective(afStack_38, unk80 * gpCamera->mFovy,
	                      gpCamera->mAspect, 1.0f, -1.0f, 1.0f, 1.0f);

	Mtx afStack_68;
	MTXConcat(getUnk30(), param_1, afStack_68);
	Mtx afStack_98;
	MTXConcat(afStack_38, afStack_68, afStack_98);
	GXLoadTexMtxImm(afStack_98, 0x1E, GX_MTX3x4);
}

void TMirrorCamera::calcEffectMtx(MtxPtr) { }

TMirrorCamera::TMirrorCamera(const char* name)
    : JDrama::TCamera(10.0f, 300000.0f, name)
    , unk80(1.3f)
    , unk84(0.0f, 0.0f, 0.0f)
    , unk90(0.0f)
    , unk94(nullptr)
{
	unk94 = (ResTIMG*)new (0x20)
	    u8[GXGetTexBufferSize(0x100, 0x100, 5, 0, 0) + sizeof(ResTIMG)];
	memset(unk94, 0, sizeof(ResTIMG));
	unk94->format          = GX_TF_RGB5A3;
	unk94->alphaEnabled    = true;
	unk94->width           = 0x100;
	unk94->height          = 0x100;
	unk94->minFilter       = 1;
	unk94->magFilter       = 1;
	unk94->mipmapCount     = 1;
	unk94->imageDataOffset = 0x20;

	GXInitTexObj(&unk60, (u8*)unk94 + unk94->imageDataOffset, unk94->width,
	             unk94->height, (GXTexFmt)unk94->format, GX_REPEAT, GX_REPEAT,
	             0);

	GXInitTexObjLOD(&unk60, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE,
	                GX_FALSE, GX_ANISO_1);
	Vec local_20 = (Vec) { 10000.0f, 10000.0f, 10000.0f };
	Vec local_2C = (Vec) { 0.0f, 1.0f, 0.0f };
	Vec local_38 = (Vec) { 20000.0f, 20000.0f, 20000.0f };
	C_MTXLookAt(unk30, &local_20, &local_2C, &local_38);
	unk98.zero();
}

static u8 getVertexFormat(const J3DModelData* model_data, GXAttr attr)
{
	const GXVtxAttrFmtList* list
	    = model_data->getVertexData().getVtxAttrFmtList();
	for (; list->attr != GX_VA_NULL; ++list)
		if (list->attr == attr)
			return list->type;
	return 0xff;
}

void TMirrorModel::setPlane()
{
	MtxPtr mtx = unk4->unk4->unk20;
	MTXMultVec(mtx, &unkC, &unkC);
	MTXMultVecSR(mtx, &unk18, &unk18);
	VECNormalize(&unk18, &unk18);
	unk24 = -VECDotProduct(&unk18, &unkC);
	unk8->setUnk84AndUnk90(unk18.x, unk18.y, unk18.z, unk24);
}

void TMirrorModel::initPlaneInfo()
{
	u8 posComp = getVertexFormat(unk4->getModel()->getModelData(), GX_VA_POS);

	if (posComp == GX_S16) {
		S16Vec* v = (S16Vec*)unk4->getModel()
		                ->getModelData()
		                ->getVertexData()
		                .getVtxPosArray();
		unkC.x = v->x;
		unkC.y = v->y;
		unkC.z = v->z;
	} else {
		Vec* v = (Vec*)unk4->getModel()
		             ->getModelData()
		             ->getVertexData()
		             .getVtxPosArray();
		unkC.x = v->x;
		unkC.y = v->y;
		unkC.z = v->z;
	}

	u8 normComp = getVertexFormat(unk4->getModel()->getModelData(), GX_VA_NRM);

	if (normComp == GX_S16) {
		S16Vec* v = (S16Vec*)unk4->getModel()
		                ->getModelData()
		                ->getVertexData()
		                .getVtxNormArray();
		// BUG: probably meant to do a float division here?
		unk18.x = v->x / 16384;
		unk18.y = v->y / 16384;
		unk18.z = v->z / 16384;
	} else if (normComp == GX_F32) {
		Vec* v = (Vec*)unk4->getModel()
		             ->getModelData()
		             ->getVertexData()
		             .getVtxNormArray();
		unk18.x = v->x;
		unk18.y = v->y;
		unk18.z = v->z;
	} else {
		unk18.x = 0.0f;
		unk18.y = 1.0f;
		unk18.z = 0.0f;
	}
}

void TMirrorModel::entry() { }

void TMirrorModel::calcView() { }

void TMirrorModel::getMirrorTexInfo() { }

inline static void identity34(MtxPtr mtx)
{
	mtx[2][3] = 0.0f;
	mtx[1][3] = 0.0f;
	mtx[0][3] = 0.0f;
	mtx[1][2] = 0.0f;
	mtx[0][2] = 0.0f;
	mtx[2][1] = 0.0f;
	mtx[0][1] = 0.0f;
	mtx[2][0] = 0.0f;
	mtx[1][0] = 0.0f;
	mtx[2][2] = 1.0f;
	mtx[1][1] = 1.0f;
	mtx[0][0] = 1.0f;
}

void TMirrorModel::init(const char* name)
{
	unk4 = SMS_MakeMActorWithAnmData(name, gpMirrorModelManager->getUnk20(), 2,
	                                 0x10210000);

	TPosition3f local_44;
	// TODO: WTF is happening with inlines here?
	identity34(local_44.mMtx);
	MTXCopy(local_44, unk4->getModel()->getBaseTRMtx());
	unk4->calc();
	unk4->getModel()->getModelData()->getMaterialNodePointer(0)->change();

	gpMirrorModelManager->findMirrorCamera();
	unk8 = gpMirrorModelManager->unk24;

	initPlaneInfo();
}

TMirrorModel::TMirrorModel()
    : unk4(nullptr)
    , unk8(0)
    , unk24(0.0f)
{
	unkC.zero();
	unk18.zero();
}

void TMirrorModelObj::setPlane()
{
	MtxPtr mtx = unk4->getModel()->getAnmMtx(0);
	Vec* v     = (Vec*)unk4->getModel()
	             ->getModelData()
	             ->getVertexData()
	             .getVtxPosArray();

	JGeometry::TVec3<f32> local_18;
	local_18.x = v->x;
	local_18.y = v->y;
	local_18.z = v->z;

	unk18.x = mtx[0][1];
	unk18.y = mtx[1][1];
	unk18.z = mtx[2][1];

	MTXMultVec(mtx, &local_18, &local_18);
	unk24 = -VECDotProduct(unk18, local_18);
	unk8->setUnk84AndUnk90(unk18.x, unk18.y, unk18.z, unk24);
}

void TMirrorModelObj::calc()
{
	// TODO: what is unk28?
}

void TMirrorModelObj::init(const char* name)
{
	TMirrorModel::init(name);
	gpMirrorModelManager->registerObjMirror(this);
}

TMirrorModelManager* gpMirrorModelManager;

bool TMirrorModelManager::isUpperThanMirrorPlane(
    const JGeometry::TVec3<f32>& param_1) const
{
	const JGeometry::TVec3<f32>* normal
	    = unk18 != -1 ? &unk1C[unk18]->getNormalVec() : nullptr;

	f32 d   = unk18 != -1 ? unk1C[unk18]->getD() : 0.0f;
	f32 dot = normal->dot(param_1);

	return dot + d < -50.0f ? false : true;
}

bool TMirrorModelManager::isInMirror(JGeometry::TVec3<f32>& param_1) const
{
	return gpCubeMirror->getDataNo(gpCubeMirror->getInCubeNo(param_1)) != -1
	           ? true
	           : false;
}

void TMirrorModelManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	JGeometry::TVec3<f32> local_44 = *gpMarioPos;
	unk18 = gpCubeMirror->getDataNo(gpCubeMirror->getInCubeNo(local_44));
	if (!(unk18 != -1 ? true : false)
	    && !gpMarioGroundPlane[0]->checkFlag(BG_CHECK_FLAG_ILLEGAL)) {
		unk24->unk84 = gpMarioGroundPlane[1]->mNormal;
		unk24->unk90 = gpMarioGroundPlane[1]->mPlaneDistance;

		JGeometry::TVec3<f32> local_7C;
		local_7C.set(unk24->unk84);
		f32 fVar4 = (local_7C.dot(gpCamera->unk124) - -unk24->unk90) * -2.0f;
		unk24->unk98.scaleAdd(fVar4, gpCamera->unk124, local_7C);
		// TODO: awful vector math, one of unused functions inlined
	}

	if (unk18 != -1) {
		if (param_1 & 2)
			unk1C[unk18]->calc();

		if (param_1 & 4)
			unk1C[unk18]->unk4->viewCalc();

		if (param_1 & 0x200) {
			unk1C[unk18]->setPlane();

			// TODO: awful vector math, one of unused functions inlined
		}
	}
}

// TODO: size is incorrect
void TMirrorModelManager::findMirrorCamera()
{
	if (!unk24)
		unk24 = JDrama::TNameRefGen::search<TMirrorCamera>("鏡カメラ");
}

void TMirrorModelManager::loadAfter()
{
	findMirrorCamera();

	for (int i = 0; i < unk10; ++i) {
		// TODO: ghidra decompiler died here
	}
}

void TMirrorModelManager::registerObjMirror(TMirrorModel* model)
{
	unk1C[unk10] = model;
	unk10++;
}

void TMirrorModelManager::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	int local_28;
	int local_2C;
	int local_30;
	stream.read(&local_28, 4);
	stream.read(&local_2C, 4);
	stream.read(&local_30, 4);
	unk14 = local_28 + local_2C + local_30 * 2;

	if (unk14 != 0) {
		unk20 = new MActorAnmData;
		unk1C = new TMirrorModel*[unk14];
		for (int i = 0; i < local_28; ++i) {
			unk1C[i] = new TMirrorModel;
			char acStack_130[0x100];
			if (gpMarDirector->getCurrentMap() == 7) {
				static const char* table[] = { "205", nullptr };
				snprintf(acStack_130, 0x100, "/scene/map/mirror/mirror%s.bmd",
				         table[i]);
			} else {
				snprintf(acStack_130, 0x100, "/scene/map/mirror/mirror%02d.bmd",
				         i);
			}
			unk1C[i]->init(acStack_130);
			++unk10;
		}
	}
}

TMirrorModelManager::TMirrorModelManager(const char* name)
    : JDrama::TViewObj(name)
    , unk10(0)
    , unk14(0)
    , unk18(-1)
    , unk1C(nullptr)
    , unk24(0)
    , unk28(0)
{
	gpMirrorModelManager = this;
}

void TMirrorMapDrawBuf::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (!(param_1 & 8) || (gpMirrorModelManager->unk18 != -1 ? true : false))
		JDrama::TDrawBufObj::perform(param_1, param_2);
}
