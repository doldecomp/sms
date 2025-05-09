#include <Map/Shimmer.hpp>
#include <Map/MapData.hpp>
#include <System/MarDirector.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <Player/MarioMain.hpp>
#include <Camera/Camera.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DAnmLoader.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <stdio.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TShimmer::TShimmer(const char* name)
    : JDrama::TActor(name)
    , unk44(nullptr)
    , unk48(nullptr)
    , unk54(nullptr)
    , unk58(new J3DFrameCtrl)
    , unk7C(0)
{
}

void TShimmer::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);
	char modelName[32];
	stream.readString(modelName, 32);
	char buffer[64];
	snprintf(buffer, 64, "/scene/mapObj/%s.bmd", modelName);
	unk44 = J3DModelLoaderDataBase::load(JKRGetResource(buffer), 0x11010000);
	unk48 = new J3DModel(unk44, 0, 1);
	snprintf(buffer, 64, "/scene/mapObj/%s.btk", modelName);
	unk54 = (J3DAnmTextureSRTKey*)J3DAnmLoaderDataBase::load(
	    JKRGetResource(buffer));

	unk54->searchUpdateMaterialID(unk44);

	for (u16 i = 0; i < unk44->getMaterialNum(); ++i) {
		J3DMaterialAnm* anm = new J3DMaterialAnm;
		unk44->getMaterialNodePointer(i)->change();
		unk44->getMaterialNodePointer(i)->setMaterialAnm(anm);
	}
	unk44->entryTexMtxAnimator(unk54);
	for (u16 i = 0; i < unk44->getMaterialNum(); ++i) {
		unk44->getMaterialNodePointer(i)->setSomeFlag();
	}
	unk58->init(unk54->getFrameMax());
	unk58->setAttribute(2);
}

void TShimmer::loadAfter()
{
	JDrama::TActor::loadAfter();
	TScreenTexture* ref
	    = JDrama::TNameRefGen::search<TScreenTexture>("スクリーンテクスチャ");
	unk44->getTexture()->setResTIMG(1, *ref->getTexture()->getTexInfo());
}

void TShimmer::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (gpMarioOriginal->_118 & 0x4000 ? true : false)
		return;

	if (param_1 & 1) {
		unk54->setFrame(unk58->getCurrentFrame());
		unk58->update();
	}

	if (param_1 & 4) {

		// TODO: more funky flag checks
		if (!gpMarioOriginal->isWearingCap()
		    && !gpMarioOriginal->mGroundPlane->checkFlag(0x4000)) {
			mPosition.set(0.0f, 0.0f, 9600.0f);
		} else {
			mPosition.set(0.0f, 0.0f, 0.0f);
		}

		Mtx effectMtx;
		SMS_GetLightPerspectiveForEffectMtx(effectMtx);
		// TODO: how do we make setEffectMtx not inline
		unk48->getModelData()
		    ->getMaterialNodePointer(0)
		    ->getTexGenBlock()
		    ->getTexMtx(1)
		    ->setEffectMtx(effectMtx);

		J3DTransformInfo info;
		info.mScale.x    = 1.0f;
		info.mScale.y    = 1.0f;
		info.mScale.z    = 1.0f;
		info.mRotation.x = 0.0f;
		info.mRotation.y = 0.0f;
		info.mRotation.z = 0.0f;
		info.mTranslate  = mPosition;
		Mtx afStack_b0;
		J3DGetTranslateRotateMtx(info, afStack_b0);
		Mtx afStack_e0;
		MTXScale(afStack_e0, mScaling.x, mScaling.y, mScaling.z);
		Mtx afStack_80;
		MTXInverse(param_2->unkB4, afStack_80);
		MTXConcat(afStack_80, afStack_b0, afStack_80);
		MTXConcat(afStack_80, afStack_e0, afStack_80);
		unk48->setBaseTRMtx(afStack_80);
		unk48->entry();
		unk48->calc();
	}

	if (param_1 & 0x200) {
		if (gpMarDirector->map == 2 || !(gpCamera->unk124.y < 0.0f))
			unk48->update();
	}
}

void TShimmer::far() { }

void TShimmer::near() { }
