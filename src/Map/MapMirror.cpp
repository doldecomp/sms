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
#include <M3DUtil/InfectiousStrings.hpp>

// Parked TU-local (BATCH_BRIEF rule): the mirror plane makeMirrorViewMtx
// reflects through. The levels are measured, not decorative --
// TMirrorModelManager::perform inlines makeMirrorViewMtx twice, once at depth
// 2 (from the CUE_MOVE block) and once at depth 3 (through
// TMirrorModel::entry -> TMirrorModel::calcView). Retail reaches
// TVec3<f32>::set<f32> out of line from both and TVec3<f32>::dot out of line
// from both, while TVec3<f32>::scaleAdd expands in the shallow copy and is
// called in the deep one: that only works if the normal is filled one level
// below makeMirrorViewMtx's body (this constructor) and the dot product three
// levels below it (calcReflectScale -> calcDistance -> dot).
//
// The four floats are parameters, not a camera pointer: retail evaluates them
// right to left at the call site (-unk90 first, then z, y, x, into f28-f31
// across the member's TVec3() call in the deep expansion) and stores mD only
// after `bl set<f32>`, which no mem-initialiser spelling reproduces. That
// spelling also lands TMirrorModel::entry on the map's 0x200.
struct MapMirrorPlane {
	MapMirrorPlane(f32 x, f32 y, f32 z, f32 d)
	{
		mNormal.set(x, y, z);
		mD = d;
	}

	f32 calcReflectScale(const JGeometry::TVec3<f32>& point) const
	{
		return -2.0f * (mNormal.dot(point) - mD);
	}

	/* 0x0 */ JGeometry::TVec3<f32> mNormal;
	/* 0xC */ f32 mD;
};

// JGVec3.hpp's `scaleAdd(scale, b, c)` is `x = b.x * scale + c.x`: the scaled
// operand is the first reference parameter. Retail's only out-of-line copy in
// the whole game is the one this TU emits, `lfs f2, 0(r4); lfs f0, 0(r5);
// fmadds f0, f2, f1, f0`, and the three `bl scaleAdd` sites below pass r4/r5
// to match it.
struct MapMirrorVecs {
	/* 0x0 */ JGeometry::TVec3<f32> mTarget;
	/* 0xC */ JGeometry::TVec3<f32> mUp;
};

void TMirrorCamera::makeMirrorViewMtx()
{
	// TODO: one level is still wrong in the deep (TMirrorModel::entry)
	// expansion. Retail inlines MapMirrorPlane's constructor at depth 4 and
	// leaves only mNormal's `bl TVec3::TVec3()` plus `bl set<f>` behind;
	// MWCC here refuses it because the implicit member construction costs 1
	// on top of the two statements and the depth-4 allowance is 2 (measured:
	// dropping either statement makes the constructor inline and the
	// `bl TVec3::TVec3()` appear at the right place). An explicit
	// `: mNormal()` is not free, and `: mD(d)` is not free either and also
	// moves the mD store ahead of `bl set<f32>`; a default constructor plus
	// a four-float set() member is much worse (73.8%). Folding mTarget/mUp
	// into the plane so the constructor is the only declaration costs more
	// than it gains (80.9%, and makeMirrorViewMtx then compiles to 0x178
	// instead of the map's 0x120); MapMirrorVecs keeps the 0x120 but pays
	// two extra `bl TVec3::TVec3()` for its own members.
	MapMirrorPlane plane(unk84.x, unk84.y, unk84.z, -unk90);
	MapMirrorVecs vecs;

	unk98.scaleAdd(plane.calcReflectScale(gpCamera->unk124), plane.mNormal,
	               gpCamera->unk124);
	vecs.mTarget.scaleAdd(plane.calcReflectScale(gpCamera->unk148),
	                      plane.mNormal, gpCamera->unk148);
	vecs.mUp.scaleAdd(plane.calcReflectScale(gpCamera->mUp), plane.mNormal,
	                  gpCamera->mUp);

	C_MTXLookAt(unk30, unk98, vecs.mUp, vecs.mTarget);
}

void TMirrorCamera::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & (CUE_CALC_VIEW | CUE_SET_PROJECTION)) {
		Mtx44Ptr projMtx = graphics->mProjMtx.mMtx;
		C_MTXPerspective(projMtx, getUnk80() * gpCamera->mFovy, gpCamera->mAspect,
		                 gpCamera->mNear, gpCamera->mFar);
		MTXCopy(unk30, graphics->mViewMtx);
		graphics->mNearPlane = gpCamera->mNear;
		graphics->mFarPlane  = gpCamera->mFar;
		if (cue & CUE_SET_PROJECTION)
			GXSetProjection(projMtx, GX_PERSPECTIVE);
		GXSetAlphaUpdate(GX_TRUE);
	}
}

void TMirrorCamera::drawSetting(MtxPtr param_1)
{
	GXLoadTexObj(&unk60, GX_TEXMAP0);
	Mtx afStack_38;
	C_MTXLightPerspective(afStack_38, getUnk80() * gpCamera->mFovy,
	                      gpCamera->mAspect, 0.5f, -0.5f, 0.5f, 0.5f);

	Mtx afStack_68;
	MTXConcat(getUnk30(), param_1, afStack_68);
	Mtx afStack_98;
	MTXConcat(afStack_38, afStack_68, afStack_98);
	GXLoadTexMtxImm(afStack_98, 0x1E, GX_MTX3x4);
}

void TMirrorCamera::calcEffectMtx(MtxPtr param_1)
{
	Mtx afStack_38;
	C_MTXLightPerspective(afStack_38, getUnk80() * gpCamera->mFovy,
	                      gpCamera->mAspect, 0.5f, -0.5f, 0.5f, 0.5f);
	MTXConcat(afStack_38, getUnk30(), param_1);
}

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
	MtxPtr mtx = unk4->getModel()->getBaseTRMtx();
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

static inline void MapMirrorMakeViewMtx(TMirrorCamera* camera)
{
	camera->makeMirrorViewMtx();
}

void TMirrorModel::entry()
{
	MapMirrorMakeViewMtx(unk8);

	Mtx effectMtx;
	unk8->calcEffectMtx(effectMtx);

	J3DMaterial* material
	    = unk4->getModel()->getModelData()->getMaterialNodePointer(0);
	material->change();
	material->getTexGenBlock()->getTexMtx(0)->setEffectMtx(effectMtx);

	unk4->entry();
}

void TMirrorModel::calcView() { unk4->viewCalc(); }

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
	                                 J3DMLF_MaterialPEFull
	                                     | J3DMLF_UseUniqueMaterials
	                                     | (1 << J3DMLF_TevStageNumShift));

	TPosition3f local_44;
	local_44.identity();
	unk4->getModel()->setBaseTRMtx(local_44);
	unk4->calc();
	unk4->getModel()->getModelData()->getMaterialNodePointer(0)->change();

	if (!gpMirrorModelManager->unk24)
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
	unk4->getModel()->setAnmMtx(0, unk28->getAnmMtx(0));
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

// Parked TU-local (BATCH_BRIEF rule): the inline level that puts
// makeMirrorViewMtx at depth 2 in the CUE_MOVE block, one below the depth 3
// it reaches through TMirrorModel::entry -> calcView in the CUE_ENTRY block.
// The manager is taken by pointer so that both statements re-read unk24,
// which is what retail does (two `lwz rN, 0x24(r30)`).
static inline void MapMirrorSetPlaneFromGround(TMirrorModelManager* manager,
                                               const TBGCheckData* ground)
{
	manager->unk24->setUnk84AndUnk90(ground->mNormal.x, ground->mNormal.y,
	                                 ground->mNormal.z,
	                                 ground->mPlaneDistance);
	manager->unk24->makeMirrorViewMtx();
}

void TMirrorModelManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		JGeometry::TVec3<f32> local_44 = *gpMarioPos;
		unk18 = gpCubeMirror->getDataNo(gpCubeMirror->getInCubeNo(local_44));
		if (!isUnk18Present()
		    && !gpMarioGroundPlane[0]->checkFlag(BG_CHECK_FLAG_ILLEGAL))
			MapMirrorSetPlaneFromGround(this, gpMarioGroundPlane[0]);
	}

	if (isUnk18Present()) {
		if (cue & CUE_CALC_ANIM)
			unk1C[unk18]->calc();

		if (cue & CUE_CALC_VIEW)
			unk1C[unk18]->calcView();

		if (cue & CUE_ENTRY) {
			unk1C[unk18]->setPlane();
			unk1C[unk18]->entry();
		}
	}
}

void TMirrorModelManager::findMirrorCamera()
{
	unk24 = JDrama::TNameRefGen::search<TMirrorCamera>("鏡カメラ");
}

void TMirrorModelManager::loadAfter()
{
	if (!unk24)
		findMirrorCamera();

	for (int i = 0; i < unk10; ++i) {
		J3DTexture* texture
		    = unk1C[i]->getUnk4()->getModel()->getModelData()->getTexture();

		ResTIMG& target       = *texture->getResTIMG(0);
		const ResTIMG& source = *unk24->getUnk94();

		target = source;
		target.imageDataOffset
		    = (u32)&source + source.imageDataOffset - (u32)&target;
	}
}

void TMirrorModelManager::registerObjMirror(TMirrorModel* model)
{
	unk1C[unk10] = model;
	unk10++;
}

// Binding level worth +8 of low region, landing TMirrorModelManager::load's
// frame at 0x150 (batch 124).
static inline u8 MapMirrorGetCurrentMapL0(TMarDirector* p)
{
	u8 currentMap = p->getCurrentMap();
	return currentMap;
}

static inline u8 MapMirrorGetCurrentMap(TMarDirector* p)
{
	u8 currentMap = MapMirrorGetCurrentMapL0(p);
	return currentMap;
}

void TMirrorModelManager::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	s32 local_28;
	s32 local_2C;
	s32 local_30;
	stream >> local_28;
	stream >> local_2C;
	stream >> local_30;
	unk14 = local_28 + local_2C + local_30 * 2;

	if (unk14 != 0) {
		unk20 = new MActorAnmData;
		unk1C = new TMirrorModel*[unk14];
		for (int i = 0; i < local_28; ++i) {
			unk1C[i] = new TMirrorModel;
			char acStack_130[0x100];
			if (MapMirrorGetCurrentMap(gpMarDirector) == 7) {
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

void TMirrorMapDrawBuf::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!(cue & CUE_DRAW) || (gpMirrorModelManager->unk18 != -1 ? true : false))
		JDrama::TDrawBufObj::perform(cue, graphics);
}
