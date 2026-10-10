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
#include <M3DUtil/InfectiousStrings.hpp>

void TMirrorActor::isInMirror() const { }

// TODO: active mirror read may need a missing reference-return inline in
// TMirrorModelManager.
void TMirrorActor::checkIsInMirror()
{
	if (unk1A & 1) {
		unk18 = 0;
		return;
	}

	if (unk1A & 2) {
		if (!gpMirrorModelManager->isUnk18Present() && !(unk1A & 4)) {
			if (unk14->getShapePacket(0)->isVisible())
				SMS_HideAllShapePacket(unk14);
			unk18 = 0;
		} else {
			if (!unk14->getShapePacket(0)->isVisible())
				SMS_ShowAllShapePacket(unk14);
			unk18 = 1;
		}

		return;
	}

	MtxPtr mtx = unk10->getAnmMtx(0);
	JGeometry::TVec3<f32> local_18;
	if (!(unk1A & 4)) {
		local_18.set(((TMtx34f*)mtx)->ref(0, 3), ((TMtx34f*)mtx)->ref(1, 3),
		             ((TMtx34f*)mtx)->ref(2, 3));
	} else {
		local_18.set(SMS_GetMarioPos());
	}

	int uVar4 = gpCubeMirror->getDataNo(gpCubeMirror->getInCubeNo(local_18));
	if (uVar4 != gpMirrorModelManager->unk18) {
		unk18 = 0;
	} else if (!gpMirrorModelManager->isUnk18Present() && !(unk1A & 4)) {
		unk18 = 0;
	} else if (gpMirrorModelManager->isUnk18Present()
	           && !gpMirrorModelManager->isUpperThanMirrorPlane(local_18)) {
		unk18 = 0;
	} else {
		unk18 = 1;
	}
}

// TODO: matrix-copy inline boundaries and stack frame remain nonmatching.
void TMirrorActor::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM) {
		checkIsInMirror();
		if (unk18 == 0)
			return;

		for (u16 i = 0; i < unk10->getModelData()->getJointNum(); ++i) {
			MtxPtr src = unk10->getAnmMtx(i);
			MTXCopy(src, unk14->getAnmMtx(i));
		}

		for (u16 i = 0; i < unk10->getModelData()->getWEvlpMtxNum(); ++i) {
			MtxPtr src = unk10->getWeightAnmMtx(i);
			MTXCopy(src, unk14->getWeightAnmMtx(i));
		}
	}

	if ((cue & CUE_CALC_VIEW) && unk18 != 0)
		unk14->viewCalc();

	if ((cue & CUE_ENTRY) && unk18 && !(unk1A & 2))
		unk14->entry();
}

void TMirrorActor::entryMirrorDrawBufferAlways(J3DModel* model)
{
	JDrama::TDrawBufObj* dbOpa = static_cast<JDrama::TDrawBufObj*>(
	    JDrama::TNameRefGen::search("DrawBuf MirrorAlways Opa"));
	j3dSys.setDrawBuffer(dbOpa->getDrawBuffer(), 0);
	dbOpa = static_cast<JDrama::TDrawBufObj*>(
	    JDrama::TNameRefGen::search("DrawBuf MirrorAlways Xlu"));
	J3DDrawBuffer* drawBuffer = dbOpa->getDrawBuffer();
	j3dSys.setDrawBuffer(drawBuffer, 1);
	model->calc();
	model->viewCalc();
	model->entry();
}

// TODO: frame-only; list-insertion iterator slots remain 4-8 bytes low.
void TMirrorActor::init(J3DModel* param_1, u16 param_2)
{
	unk1A = param_2;
	unk10 = param_1;

	if (unk1A & 8) {
		SDLModel* model
		    = new SDLModel(((SDLModel*)param_1)->getSDLModelData(), 3, 1);
		unk14 = model;
	} else {
		J3DModelData* modelData = unk10->getModelData();
		unk14                   = new J3DModel(modelData, 0, 1);
	}

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* mirrorScene
	    = static_cast<JDrama::TViewObjPtrListT<JDrama::TViewObj>*>(
	        JDrama::TNameRefGen::search("鏡シーン"));
	mirrorScene->getChildren().push_back(this);

	if (unk1A & 2)
		entryMirrorDrawBufferAlways(unk14);
}

TMirrorActor::TMirrorActor(const char* name)
    : JDrama::TViewObj(name)
    , unk10(nullptr)
    , unk14(0)
    , unk18(0)
    , unk1A(0)
{
}
