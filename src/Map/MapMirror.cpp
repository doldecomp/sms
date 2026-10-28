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
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TMirrorCamera::makeMirrorViewMtx()
{
	// TODO: this bastard need to call TVec3() out of line >:(
	JGeometry::TPartition3<f32> plane(mPlane.mNormal.x, mPlane.mNormal.y,
	                                  mPlane.mNormal.z, -mPlane.mDist);
	plane.reflect(mPosition, gpCamera->unk124);
	JGeometry::TVec3<f32> at;
	plane.reflect(at, gpCamera->unk148);
	JGeometry::TVec3<f32> up;
	plane.reflect(up, gpCamera->mUp);
	C_MTXLookAt(mViewMtx, &mPosition, &up, &at);
}

void TMirrorCamera::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & (CUE_CALC_VIEW | CUE_SET_PROJECTION)) {
		MtxPtr projMtx = graphics->mProjMtx.mMtx;
		C_MTXPerspective(projMtx, mFovyMultiplier * gpCamera->getFovy(),
		                 gpCamera->getAspect(), gpCamera->getNear(),
		                 gpCamera->getFar());
		MTXCopy(mViewMtx, graphics->mViewMtx);
		graphics->setNearPlane(gpCamera->getNear());
		graphics->setFarPlane(gpCamera->getFar());
		if (cue & CUE_SET_PROJECTION)
			GXSetProjection(projMtx, GX_PERSPECTIVE);
		GXSetAlphaUpdate(GX_TRUE);
	}
}

void TMirrorCamera::drawSetting(MtxPtr param_1)
{
	GXLoadTexObj(&mFrameBufferTexObj, GX_TEXMAP0);
	Mtx afStack_38;
	C_MTXLightPerspective(afStack_38, mFovyMultiplier * gpCamera->getFovy(),
	                      gpCamera->getAspect(), 0.5f, -0.5f, 0.5f, 0.5f);

	Mtx afStack_68;
	MTXConcat(getViewMtx(), param_1, afStack_68);
	Mtx afStack_98;
	MTXConcat(afStack_38, afStack_68, afStack_98);
	GXLoadTexMtxImm(afStack_98, 0x1E, GX_MTX3x4);
}

void TMirrorCamera::calcEffectMtx(MtxPtr mtx)
{
	Mtx lightPerspective;
	C_MTXLightPerspective(lightPerspective,
	                      mFovyMultiplier * gpCamera->getFovy(),
	                      gpCamera->getAspect(), 0.5f, -0.5f, 0.5f, 0.5f);
	MTXConcat(lightPerspective, getViewMtx(), mtx);
}

TMirrorCamera::TMirrorCamera(const char* name)
    : JDrama::TCamera(10.0f, 300000.0f, name)
    , mFovyMultiplier(1.3f)
    , mPlane(0.0f, 0.0f, 0.0f, 0.0f)
    , mFrameBuffer(nullptr)
{
	mFrameBuffer = (ResTIMG*)new (0x20)
	    u8[GXGetTexBufferSize(0x100, 0x100, 5, 0, 0) + sizeof(ResTIMG)];
	memset(mFrameBuffer, 0, sizeof(ResTIMG));
	mFrameBuffer->format          = GX_TF_RGB5A3;
	mFrameBuffer->alphaEnabled    = true;
	mFrameBuffer->width           = 0x100;
	mFrameBuffer->height          = 0x100;
	mFrameBuffer->minFilter       = 1;
	mFrameBuffer->magFilter       = 1;
	mFrameBuffer->mipmapCount     = 1;
	mFrameBuffer->imageDataOffset = sizeof(ResTIMG);

	GXInitTexObj(&mFrameBufferTexObj,
	             (u8*)mFrameBuffer + mFrameBuffer->imageDataOffset,
	             mFrameBuffer->width, mFrameBuffer->height,
	             (GXTexFmt)mFrameBuffer->format, GX_REPEAT, GX_REPEAT, 0);

	GXInitTexObjLOD(&mFrameBufferTexObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f,
	                GX_FALSE, GX_FALSE, GX_ANISO_1);
	Vec local_20 = (Vec) { 10000.0f, 10000.0f, 10000.0f };
	Vec local_2C = (Vec) { 0.0f, 1.0f, 0.0f };
	Vec local_38 = (Vec) { 20000.0f, 20000.0f, 20000.0f };
	C_MTXLookAt(getViewMtx(), &local_20, &local_2C, &local_38);
	mPosition.zero();
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
	MtxPtr mtx = mMActor->getModel()->getBaseTRMtx();
	MTXMultVec(mtx, &mModelVertexSample, &mModelVertexSample);
	MTXMultVecSR(mtx, &mPlane.mNormal, &mPlane.mNormal);
	VECNormalize(&mPlane.mNormal, &mPlane.mNormal);
	mPlane.mDist = -VECDotProduct(&mPlane.mNormal, &mModelVertexSample);
	mMirrorCamera->setPlane(mPlane.mNormal.x, mPlane.mNormal.y,
	                        mPlane.mNormal.z, mPlane.mDist);
}

void TMirrorModel::initPlaneInfo()
{
	u8 posComp
	    = getVertexFormat(mMActor->getModel()->getModelData(), GX_VA_POS);

	if (posComp == GX_S16) {
		S16Vec* v
		    = (S16Vec*)mMActor->getModel()->getModelData()->getVtxPosArray();
		mModelVertexSample.x = v->x;
		mModelVertexSample.y = v->y;
		mModelVertexSample.z = v->z;
	} else {
		Vec* v = (Vec*)mMActor->getModel()->getModelData()->getVtxPosArray();
		mModelVertexSample.x = v->x;
		mModelVertexSample.y = v->y;
		mModelVertexSample.z = v->z;
	}

	u8 normComp
	    = getVertexFormat(mMActor->getModel()->getModelData(), GX_VA_NRM);

	if (normComp == GX_S16) {
		S16Vec* v = (S16Vec*)mMActor->getModel()
		                ->getModelData()
		                ->getVertexData()
		                .getVtxNormArray();
		// BUG: probably meant to do a float division here?
		mPlane.mNormal.x = v->x / 16384;
		mPlane.mNormal.y = v->y / 16384;
		mPlane.mNormal.z = v->z / 16384;
	} else if (normComp == GX_F32) {
		Vec* v = (Vec*)mMActor->getModel()
		             ->getModelData()
		             ->getVertexData()
		             .getVtxNormArray();
		mPlane.mNormal.x = v->x;
		mPlane.mNormal.y = v->y;
		mPlane.mNormal.z = v->z;
	} else {
		mPlane.mNormal.x = 0.0f;
		mPlane.mNormal.y = 1.0f;
		mPlane.mNormal.z = 0.0f;
	}
}

void TMirrorModel::entry()
{
	mMirrorCamera->makeMirrorViewMtx();
	Mtx effectMtx;
	mMirrorCamera->calcEffectMtx(effectMtx);
	J3DMaterial* material
	    = mMActor->getModel()->getModelData()->getMaterialNodePointer(0);
	material->change();
	material->getTexGenBlock()->getTexMtx(0)->setEffectMtx(effectMtx);
	mMActor->entry();
}

void TMirrorModel::calcView() { mMActor->viewCalc(); }

ResTIMG* TMirrorModel::getMirrorTexInfo()
{
	return mMActor->getModel()->getModelData()->getTexture()->getResTIMG(0);
}

void TMirrorModel::init(const char* name)
{
	mMActor = SMS_MakeMActorWithAnmData(
	    name, gpMirrorModelManager->getSharedAnmData(), 2,
	    J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	        | (1 << J3DMLF_TevStageNumShift));

	TPosition3f mtx;
	mtx.identity();
	mMActor->getModel()->setBaseTRMtx(mtx);
	mMActor->calc();
	mMActor->getModel()->getModelData()->getMaterialNodePointer(0)->change();

	if (!gpMirrorModelManager->getMirrorCamera())
		gpMirrorModelManager->findMirrorCamera();
	mMirrorCamera = gpMirrorModelManager->getMirrorCamera();

	initPlaneInfo();
}

TMirrorModel::TMirrorModel()
    : mMActor(nullptr)
    , mMirrorCamera(nullptr)
{
	mPlane.mDist = 0.0f;
	mModelVertexSample.zero();
	mPlane.mNormal.zero();
}

void TMirrorModelObj::setPlane()
{
	MtxPtr mtx = mMActor->getModel()->getAnmMtx(0);
	Vec* v     = (Vec*)mMActor->getModel()->getModelData()->getVtxPosArray();

	JGeometry::TVec3<f32> sampleVtx;
	sampleVtx.x = v->x;
	sampleVtx.y = v->y;
	sampleVtx.z = v->z;

	mPlane.mNormal.x = mtx[0][1];
	mPlane.mNormal.y = mtx[1][1];
	mPlane.mNormal.z = mtx[2][1];

	MTXMultVec(mtx, &sampleVtx, &sampleVtx);
	mPlane.mDist = -VECDotProduct(mPlane.mNormal, sampleVtx);
	mMirrorCamera->setPlane(mPlane.mNormal.x, mPlane.mNormal.y,
	                        mPlane.mNormal.z, mPlane.mDist);
}

void TMirrorModelObj::calc()
{
	mMActor->getModel()->setAnmMtx(0, mModel->getAnmMtx(0));
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
	    = mActiveMirrorModelIdx != -1
	          ? &mMirrorModels[mActiveMirrorModelIdx]->getNormalVec()
	          : nullptr;

	f32 d   = mActiveMirrorModelIdx != -1
	              ? mMirrorModels[mActiveMirrorModelIdx]->getD()
	              : 0.0f;
	f32 dot = normal->dot(param_1);

	return dot + d < -50.0f ? false : true;
}

bool TMirrorModelManager::isInMirror(JGeometry::TVec3<f32>& param_1) const
{
	return gpCubeMirror->getDataNo(gpCubeMirror->getInCubeNo(param_1)) != -1
	           ? true
	           : false;
}

void TMirrorModelManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		JGeometry::TVec3<f32> marioPos = SMS_GetMarioPos();
		mActiveMirrorModelIdx
		    = gpCubeMirror->getDataNo(gpCubeMirror->getInCubeNo(marioPos));
		if (!isMirrorActive() && !SMS_GetMarioGroundPlane()->isIllegalData()) {
			const TBGCheckData* plane = SMS_GetMarioGroundPlane();
			mMirrorCamera->setPlane(plane->getNormal().x, plane->getNormal().y,
			                        plane->getNormal().z,
			                        plane->getPlaneDistance());
			mMirrorCamera->makeMirrorViewMtx();
		}
	}

	if (isMirrorActive()) {
		if (cue & CUE_CALC_ANIM)
			mMirrorModels[mActiveMirrorModelIdx]->calc();

		if (cue & CUE_CALC_VIEW)
			mMirrorModels[mActiveMirrorModelIdx]->calcView();

		if (cue & CUE_ENTRY) {
			mMirrorModels[mActiveMirrorModelIdx]->setPlane();
			mMirrorModels[mActiveMirrorModelIdx]->entry();
		}
	}
}

void TMirrorModelManager::findMirrorCamera()
{
	mMirrorCamera
	    = static_cast<TMirrorCamera*>(JDrama::TNameRefGen::search("鏡カメラ"));
}

void TMirrorModelManager::loadAfter()
{
	if (!mMirrorCamera)
		findMirrorCamera();

	for (int i = 0; i < mMirrorModelNum; ++i) {
		ResTIMG& target       = *mMirrorModels[i]->getMirrorTexInfo();
		const ResTIMG& source = *mMirrorCamera->getFrameBuffer();

		target = source;
		target.imageDataOffset
		    = (uintptr_t)&source + source.imageDataOffset - (uintptr_t)&target;
	}
}

void TMirrorModelManager::registerObjMirror(TMirrorModel* model)
{
	mMirrorModels[mMirrorModelNum] = model;
	mMirrorModelNum++;
}

void TMirrorModelManager::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	s32 mirrorNum;
	s32 local_2C;
	s32 local_30;
	stream >> mirrorNum;
	stream >> local_2C;
	stream >> local_30;
	mMirrorModelMax = mirrorNum + local_2C + local_30 * 2;

	if (mMirrorModelMax != 0) {
		mSharedEmptyAnmData = new MActorAnmData;
		mMirrorModels       = new TMirrorModel*[mMirrorModelMax];
		for (int i = 0; i < mirrorNum; ++i) {
			mMirrorModels[i] = new TMirrorModel;
			char modelBmdPath[0x100];
			if (SMSGetMarDirector()->getCurrentStage() == 7) {
				static const char* table[] = { "205", nullptr };
				snprintf(modelBmdPath, 0x100, "/scene/map/mirror/mirror%s.bmd",
				         table[i]);
			} else {
				snprintf(modelBmdPath, 0x100,
				         "/scene/map/mirror/mirror%02d.bmd", i);
			}
			mMirrorModels[i]->init(modelBmdPath);
			++mMirrorModelNum;
		}
	}
}

TMirrorModelManager::TMirrorModelManager(const char* name)
    : JDrama::TViewObj(name)
    , mMirrorModelNum(0)
    , mMirrorModelMax(0)
    , mActiveMirrorModelIdx(-1)
    , mMirrorModels(nullptr)
    , mMirrorCamera(nullptr)
    , unk28(0)
{
	gpMirrorModelManager = this;
}

void TMirrorMapDrawBuf::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!(cue & CUE_DRAW) || gpMirrorModelManager->isMirrorActive())
		JDrama::TDrawBufObj::perform(cue, graphics);
}
