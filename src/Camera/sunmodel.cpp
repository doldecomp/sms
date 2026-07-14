#include <Camera/SunModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DAnmLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <Map/MapStaticObject.hpp>
#include <Camera/cameralib.hpp>
#include <Camera/CameraMarioData.hpp>
#include <Camera/Camera.hpp>
#include <System/Resolution.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <stdio.h>

static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

TSunModel* gpSunModel;

TSunModel::TSunModel(bool param_1, const char* name)
    : JDrama::TActor(name)
    , unk44(nullptr)
    , unk48(nullptr)
    , unk4C(nullptr)
    , unk64(nullptr)
    , unk68(-1)
    , unk6C(0.1f)
    , unk70(0.01f)
    , unk74(50)
    , unk78(0.1f)
    , unk7C(0.01f)
    , unk80(100)
    , unk84(3.0f)
    , unk88(2.0f)
    , unk9C(0.0f)
    , unkA0(0.0f)
    , unkA4(0.0f)
    , unkA8(0.0f)
    , unkAC(0.0f)
    , unkB0(0.0f)
    , unk191(0)
    , unk194(0.0f)
    , unk1A4(0.014f)
    , unk1A8(3.0f)
    , unk1AC(0)
{
	gpSunModel = this;
	if (param_1) {
		unk1AC |= 0x4;
		unk80 = 0;
	}

	JGeometry::TVec2<f32>* it2 = unkF8;
	JGeometry::TVec2<s16>* it1 = unkB4;
	bool* it3                  = unk180;
	for (int i = 0; i < 17; ++i) {
		it1->x = it1->y = -1;
		++it1;
		it2->x = it2->y = 10000.0f;
		++it2;
		*it3 = false;
		++it3;
	}
}

void TSunModel::load(JSUMemoryInputStream& param_1)
{
	JDrama::TActor::load(param_1);

	mScaling *= 0.4f;

	const char* volumeName = cSunVolumeName;
	u32 modelFlags         = 0x10020000;
	if (unk1AC & 4) {
		modelFlags |= 0x1000000;
		volumeName = cSunsetVolumeName;
	}

	char path[0x100];
	snprintf(path, sizeof(path), "%s/%s", volumeName, "model.bmd");
	unk44 = J3DModelLoaderDataBase::load(JKRGetResource(path), modelFlags);
	unk48 = new J3DModel(unk44, 0, 1);

	snprintf(path, sizeof(path), "%s/%s", volumeName, "model.btk");
	unk4C = (J3DAnmTextureSRTKey*)J3DAnmLoaderDataBase::load(
	    JKRGetResource(path));
	unk4C->searchUpdateMaterialID(unk44);

	u16 i;
	int e = unk44->getMaterialNum();
	for (i = 0; i < e; ++i) {
		J3DMaterialAnm* materialAnm = new J3DMaterialAnm;

		unk44->getMaterialNodePointer(i)->change();
		unk44->getMaterialNodePointer(i)->setMaterialAnm(materialAnm);
	}

	unk44->entryTexMtxAnimator(unk4C);
	unk8C = *unk44->getMaterialNodePointer(0)->getTevColor(0);
	unk94 = *unk44->getMaterialNodePointer(1)->getTevColor(0);

	unk9C = unkA4 = (f32)unk68;
	unkA0 = unkA8 = (f32)unk74;
	unk50.init(unk4C->getFrameMax());
	unk50.setRate(SMSGetAnmFrameRate());
	unk50.setAttribute(J3DFrameCtrl::ATTR_LOOP);

	unk198 = mPosition;
	unk64  = new TMapStaticObj("太陽in鏡");
	unk64->init("sun_mirror");
	unk64->mPosition = mPosition;
	unk64->mRotation = mRotation;
	unk64->mScaling  = mScaling;

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* mirrorScene
	    = JDrama::TNameRefGen::search<
	        JDrama::TViewObjPtrListT<JDrama::TViewObj> >("鏡シーン");
	mirrorScene->getChildren().push_back(unk64);
}

void TSunModel::calcOtherFPosFromCenterAndRadius_(
    JGeometry::TVec2<f32>* param_1, const JGeometry::TVec2<f32>& param_2,
    f32 param_3)
{
	f32 fVar1    = param_3 * 0.707f;
	param_1[0].x = param_2.x;
	param_1[0].y = param_2.y + param_3;
	param_1[1].x = param_2.x - fVar1;
	param_1[1].y = param_2.y + fVar1;
	param_1[2].x = param_2.x - param_3;
	param_1[2].y = param_2.y;
	param_1[3].x = param_2.x - fVar1;
	param_1[3].y = param_2.y - fVar1;
	param_1[4].x = param_2.x;
	param_1[4].y = param_2.y - param_3;
	param_1[5].x = param_2.x + fVar1;
	param_1[5].y = param_2.y - fVar1;
	param_1[6].x = param_2.x + param_3;
	param_1[6].y = param_2.y;
	param_1[7].x = param_2.x + fVar1;
	param_1[7].y = param_2.y + fVar1;
}

// TODO: mark as inline or even move to the header maybe?
void TSunModel::calcDispRatioAndScreenPos_()
{
	unk191   = 0;
	bool* it = unk180;
	for (int i = 0; i < 17; ++i, ++it)
		if (*it)
			unk191 += 1;

	unk194 = (f32)unk191 * (1.0f / 17.0f);

	CLBCalc2DFPos(unkF8, gpCamera->getUnk16C(), gpCamera->getUnk1EC(), unk198,
	              nullptr, false);

	f32 radius = unk1A4 * mScaling.y;
	calcOtherFPosFromCenterAndRadius_(&unkF8[1], unkF8[0], radius);
	radius /= 2.0f;
	calcOtherFPosFromCenterAndRadius_(&unkF8[9], unkF8[0], radius);

	int i;
	JGeometry::TVec2<f32>* it2;
	JGeometry::TVec2<s16>* it1;

	it1 = unkB4;
	it2 = unkF8;
	for (i = 0; i < 17; ++i) {
		CLBScreenFPosToSPos(it1, *it2);
		++it1;
		++it2;
	}
}

void TSunModel::perform(u32 cue, JDrama::TGraphics*)
{
	bool sunInBounds;
	if (gpCameraMario->isMarioIndoor()) {
		sunInBounds = false;
	} else {
		sunInBounds = isInBounds(unk1A8);
	}

	if (cue & CUE_MOVE) {
		unkA4 = CLBLinearInbetween<f32>((f32)unk68, 255.0f, unk194);
		unkA8 = CLBEaseOutInbetween<f32>((f32)unk74, 255.0f, unk194);

		f32 chase1;
		if (unk9C < unkA4)
			chase1 = unk6C;
		else
			chase1 = unk70;
		CLBChaseDecrease(&unk9C, unkA4, chase1, 0.0f);
		unk8C.color.a = (s16)unk9C;

		f32 chase2;
		if (unkA0 < unkA8)
			chase2 = unk78;
		else
			chase2 = unk7C;
		CLBChaseDecrease(&unkA0, unkA8, chase2, 0.0f);
		unk94.color.a = (s16)unkA0;

		if (gpCameraMario->isMarioIndoor()) {
			unkB0 = 0.0f;
		} else {
			f32 distSq = unkF8[0].squared();
			if (unkF8[0].squared() > 2.0f) {
				unkB0 = 0.0f;
			} else {
				unkB0 = CLBLinearInbetween<f32>(
				    0.0f, (f32)unk80, 0.5f * (2.0f - distSq) * unk194);
			}
		}

		f32 chase3;
		if (unkAC < unkB0)
			chase3 = unk84;
		else
			chase3 = unk88;
		CLBChaseGeneralConstantSpecifySpeed<f32>(&unkAC, unkB0, chase3);

		JGeometry::TVec3<f32> dir;
		dir.sub(mPosition, gpCamera->getUnk124());
		MsVECNormalize(&dir, &dir);

		JGeometry::TVec3<f32> camPos;
		camPos.set(gpCamera->getUnk124());
		unk198.scaleAdd(250000.0f, camPos, dir);

		if (unk64)
			unk64->mPosition = unk198;

		calcDispRatioAndScreenPos_();
	}

	if (cue & CUE_CALC_ANIM) {
		unk50.update();
		if (sunInBounds) {
			Mtx mtx;
			MsMtxSetTRS(mtx, unk198.x, unk198.y, unk198.z, mRotation.x,
			            mRotation.y, mRotation.z, mScaling.x, mScaling.y,
			            mScaling.z);
			MTXCopy(mtx, unk48->unk20);
			unk48->calc();
		}
	}

	if (cue & CUE_ENTRY && sunInBounds) {
		unk4C->setFrame(unk50.getFrame());
		unk44->mMaterials[0]->getTevBlock()->setTevColor(0, &unk8C);
		unk44->mMaterials[1]->getTevBlock()->setTevColor(0, &unk94);
		unk48->entry();
	}

	if (cue & CUE_CALC_VIEW && sunInBounds)
		unk48->viewCalc();
}

void TSunModel::getZBufValue()
{
	bool cVar1 = gpCameraMario->isMarioIndoor();

	JGeometry::TVec2<s16>* it = unkB4;
	bool* it2                 = unk180;
	for (int i = 0; i < 17; ++i, ++it, ++it2) {
		*it2 = 0;
		if (!cVar1 && it->x != -1 && it->y != -1) {
			u32 depth;
			GXPeekZ(it->x, it->y, &depth);
			if (depth == 0xffffff)
				*it2 = 1;
		}
	}
}
