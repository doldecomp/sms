#include <Strategic/MirrorActor.hpp>
#include <Map/MapMirror.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <Player/MarioAccess.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JDrama/JDRDrawBufObj.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/MActorAnm.hpp>

bool TMirrorActor::isInMirror() const { return mIsInMirror; }

void TMirrorActor::checkIsInMirror()
{
	if (mFlags & MIRROR_ACTOR_FLAG_HIDDEN) {
		mIsInMirror = false;
		return;
	}

	if (mFlags & MIRROR_ACTOR_FLAG_ALWAYS) {
		if (!gpMirrorModelManager->isMirrorActive()
		    && !(mFlags & MIRROR_ACTOR_FLAG_MARIO)) {
			if (mMirrorModel->getShapePacket(0)->isVisible())
				SMS_HideAllShapePacket(mMirrorModel);
			mIsInMirror = false;
		} else {
			if (!mMirrorModel->getShapePacket(0)->isVisible())
				SMS_ShowAllShapePacket(mMirrorModel);
			mIsInMirror = true;
		}

		return;
	}

	MtxPtr mtx = mNormalModel->getAnmMtx(0);
	JGeometry::TVec3<f32> position;
	if (!(mFlags & MIRROR_ACTOR_FLAG_MARIO)) {
		position.set(mtx[0][3], mtx[1][3], mtx[2][3]);
	} else {
		position.set(SMS_GetMarioPos());
	}

	int mirrorIdx
	    = gpCubeMirror->getDataNo(gpCubeMirror->getInCubeNo(position));
	if (mirrorIdx != gpMirrorModelManager->getActiveMirrorIdx()) {
		mIsInMirror = false;
	} else if (!gpMirrorModelManager->isMirrorActive()
	           && !(mFlags & MIRROR_ACTOR_FLAG_MARIO)) {
		mIsInMirror = false;
	} else if (gpMirrorModelManager->isMirrorActive()
	           && !gpMirrorModelManager->isUpperThanMirrorPlane(position)) {
		mIsInMirror = false;
	} else {
		mIsInMirror = true;
	}
}

void TMirrorActor::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM) {
		checkIsInMirror();
		if (!isInMirror())
			return;

		for (u16 i = 0; i < mNormalModel->getModelData()->getJointNum(); ++i)
			MTXCopy(mNormalModel->getAnmMtx(i), mMirrorModel->getAnmMtx(i));

		for (u16 i = 0; i < mNormalModel->getModelData()->getWEvlpMtxNum(); ++i)
			MTXCopy(mNormalModel->getWeightAnmMtx(i),
			        mMirrorModel->getWeightAnmMtx(i));
	}

	if ((cue & CUE_CALC_VIEW) && isInMirror())
		mMirrorModel->viewCalc();

	if ((cue & CUE_ENTRY) && isInMirror()
	    && !(mFlags & MIRROR_ACTOR_FLAG_ALWAYS))
		mMirrorModel->entry();
}

void TMirrorActor::entryMirrorDrawBufferAlways(J3DModel* model)
{
	JDrama::TDrawBufObj* dbOpa = static_cast<JDrama::TDrawBufObj*>(
	    JDrama::TNameRefGen::search("DrawBuf MirrorAlways Opa"));
	j3dSys.setDrawBuffer(dbOpa->getDrawBuffer(), 0);
	JDrama::TDrawBufObj* dbXlu = static_cast<JDrama::TDrawBufObj*>(
	    JDrama::TNameRefGen::search("DrawBuf MirrorAlways Xlu"));
	j3dSys.setDrawBuffer(dbXlu->getDrawBuffer(), 1);
	model->calc();
	model->viewCalc();
	model->entry();
}

void TMirrorActor::init(J3DModel* model, u16 flags)
{
	mFlags       = flags;
	mNormalModel = model;

	if (mFlags & MIRROR_ACTOR_FLAG_SDL_MODEL)
		mMirrorModel
		    = new SDLModel(((SDLModel*)model)->getSDLModelData(), 3, 1);
	else {
		J3DModelData* modelData = mNormalModel->getModelData();
		mMirrorModel            = new J3DModel(modelData, 0, 1);
	}

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* mirrorScene
	    = static_cast<JDrama::TViewObjPtrListT<JDrama::TViewObj>*>(
	        JDrama::TNameRefGen::search("鏡シーン"));
	mirrorScene->getChildren().push_back(this);

	if (mFlags & MIRROR_ACTOR_FLAG_ALWAYS)
		entryMirrorDrawBufferAlways(mMirrorModel);
}

TMirrorActor::TMirrorActor(const char* name)
    : JDrama::TViewObj(name)
    , mNormalModel(nullptr)
    , mMirrorModel(nullptr)
    , mIsInMirror(false)
    , mFlags(0)
{
}
