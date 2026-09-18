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

// UNUSED, 0x8 in the map: two instructions, so a plain getter for the
// in-mirror flag rather than a recomputation.
u8 TMirrorActor::isInMirror() const { return unk18; }

// Parked, not promoted to TMirrorModelManager::isUnk18Present(): retail's
// frame for checkIsInMirror needs the 8 bytes an inlined free function's
// pointer parameter binds. A member predicate written over getUnk18() is +16
// (0x70 vs 0x68) and the direct field test is +0 (0x60); see the trial table
// at the declaration in include/Map/MapMirror.hpp.
static inline bool MirrorActor_isMirrorNoPresent(const TMirrorModelManager* mgr)
{
	return mgr->getUnk18() != -1 ? true : false;
}

void TMirrorActor::checkIsInMirror()
{
	if (unk1A & 1) {
		unk18 = 0;
		return;
	}

	if (unk1A & 2) {
		if (!gpMirrorModelManager->isUnk18Present() && !(unk1A & 4)
		    && !(unk1A & 0x20)) {
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
		local_18.set(mtx[0][3], mtx[1][3], mtx[2][3]);
	} else {
		local_18.set(*gpMarioPos);
	}

	int uVar4 = gpCubeMirror->getDataNo(gpCubeMirror->getInCubeNo(local_18));
	if (uVar4 != gpMirrorModelManager->getUnk18()) {
		unk18 = 0;
	} else if (!MirrorActor_isMirrorNoPresent(gpMirrorModelManager)
	           && !(unk1A & 4) && !(unk1A & 0x20)) {
		unk18 = 0;
	} else if (MirrorActor_isMirrorNoPresent(gpMirrorModelManager)
	           && !gpMirrorModelManager->isUpperThanMirrorPlane(local_18)) {
		unk18 = 0;
	} else {
		unk18 = 1;
	}
}

void TMirrorActor::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM) {
		checkIsInMirror();
		if (unk18 == 0)
			return;

		for (u16 i = 0; i < getUnk10()->getModelData()->getJointNum(); ++i)
			MTXCopy(getUnk10()->getAnmMtx(i), unk14->getAnmMtx(i));

		for (u16 i = 0; i < getUnk10()->getModelData()->getWEvlpMtxNum(); ++i)
			MTXCopy(getUnk10()->getWeightAnmMtx(i),
			        unk14->getWeightAnmMtx(i));
	}

	if ((cue & CUE_CALC_VIEW) && unk18 != 0)
		unk14->viewCalc();

	if ((cue & CUE_ENTRY) && unk18 && !(unk1A & 2))
		unk14->entry();
}

void TMirrorActor::entryMirrorDrawBufferAlways(J3DModel* model)
{
	JDrama::TDrawBufObj* dbOpa
	    = (JDrama::TDrawBufObj*)JDrama::TNameRefGen::getInstance()
	          ->getRootNameRef()
	          ->search("DrawBuf MirrorAlways Opa");
	j3dSys.setDrawBuffer(dbOpa->getDrawBuffer(), 0);
	JDrama::TDrawBufObj* dbXlu
	    = (JDrama::TDrawBufObj*)JDrama::TNameRefGen::getInstance()
	          ->getRootNameRef()
	          ->search("DrawBuf MirrorAlways Xlu");
	j3dSys.setDrawBuffer(dbXlu->getDrawBuffer(), 1);
	model->calc();
	model->viewCalc();
	model->entry();
}

// TODO: frame exact (0xd8) and every instruction exact; the only residue is
// where the inlined push_back's temporaries sit.  Retail's three pairs are
// 0x90/0x94, 0xa4/0xa8, 0xb4/0xb8 -- gaps of 0x10 and 0xc between them -- while
// ours are 0x80/0x84, 0x9c/0xa0, 0xb4/0xb8, i.e. 8 extra bytes in each gap with
// the top pair anchored.  The same shape is open in SDLModel::entry(), so this
// is a JGadget list/iterator header property (std-list.hpp, JDRViewObjPtrList),
// not a call-site one.  Rejected here: named `new` results (frame 8 over when
// combined with getUnk14(), 11 diffs alone) and a named cast for the SDLModel
// branch (+0).
void TMirrorActor::init(J3DModel* param_1, u16 param_2)
{
	unk1A = param_2;
	unk10 = param_1;

	if (unk1A & 8)
		unk14 = new SDLModel(((SDLModel*)param_1)->getSDLModelData(), 3, 1);
	else {
		J3DModelData* modelData = unk10->getModelData();
		unk14                   = new J3DModel(modelData, 0, 1);
	}

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* mirrorScene
	    = JDrama::TNameRefGen::search<
	        JDrama::TViewObjPtrListT<JDrama::TViewObj> >("鏡シーン");
	mirrorScene->getChildren().push_back(this);

	if (unk1A & 2)
		entryMirrorDrawBufferAlways(getUnk14());
}

TMirrorActor::TMirrorActor(const char* name)
    : JDrama::TViewObj(name)
    , unk10(nullptr)
    , unk14(0)
    , unk18(0)
    , unk1A(0)
{
}
