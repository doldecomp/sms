#include <System/MarDirector.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/PerformList.hpp>
#include <System/RenderModeObj.hpp>
#include <System/Resolution.hpp>
#include <Map/MapMirror.hpp>
#include <Map/PollutionManager.hpp>
#include <Map/BathWaterManager.hpp>
#include <Camera/LensGlow.hpp>
#include <Camera/LensFlare.hpp>
#include <JSystem/JDrama/JDRViewport.hpp>
#include <JSystem/JDrama/JDRCamera.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDREfbCtrl.hpp>

// rogue includes
#include <M3DUtil/InfectiousStrings.hpp>

void TMarDirector::initECTGft(
    TPerformList* param_1, TPerformList* param_2,
    JDrama::TViewObjPtrListT<JDrama::TViewObj>* param_3,
    JDrama::TViewObjPtrListT<JDrama::TViewObj>* param_4)
{
	if (gpPollution->getJointModelNum() == 0) {
		JDrama::TViewObj* bathtubWater
		    = JDrama::TNameRefGen::search<JDrama::TViewObj>("バスタブの水");
		if (bathtubWater)
			param_2->push_back(bathtubWater, 8);
	} else {
		JDrama::TViewObjPtrListT<JDrama::TViewObj>* graffitiGroup
		    = JDrama::TNameRefGen::search<
		        JDrama::TViewObjPtrListT<JDrama::TViewObj> >("落書きグループ");
		JDrama::TViewObj* drawInit
		    = JDrama::TNameRefGen::search<JDrama::TViewObj>("SMS Draw Init");

		JDrama::TEfbCtrlTex* graffitiEfbTex
		    = new JDrama::TEfbCtrlTex("graffito check");
		param_4->insert(graffitiEfbTex);

		JDrama::TRect rect;
		rect.set(0, 0, 0x200, 0x200);
		graffitiEfbTex->setSrcRect(rect);
		param_1->push_back(graffitiEfbTex, 0x80);

		param_1->push_back(new JDrama::TViewport(rect, "graffito"), 0x8);
		param_1->push_back(new JDrama::TOrthoProj(0.0f, 0.0f, 512.0f, 512.0f),
		                   0x10);
		param_1->push_back(drawInit, 0x8);
		param_1->push_back(graffitiGroup, 0x1000000);
		param_1->push_back(graffitiEfbTex, 0x8);

		for (int i = 0; i < gpPollution->getJointModelNum(); ++i) {
			JDrama::TEfbCtrlTex* efbTex = new JDrama::TEfbCtrlTex("graffito");
			param_4->insert(efbTex);
			const ResTIMG* img = gpPollution->getLayer(i)->getUnk58();
			efbTex->mImagePtr  = (u8*)&img + img->imageDataOffset;
			efbTex->mWidth     = img->width;
			efbTex->mHeight    = img->height;
			efbTex->mTexFmt    = GX_CTF_R8;
			JDrama::TRect rect;
			rect.set(0, 0, img->width, img->height);
			efbTex->setSrcRect(rect);
			param_2->push_back(efbTex, 0x80);
			param_2->push_back(new JDrama::TViewport(rect, "graffito"), 0x8);
			param_2->push_back(
			    new JDrama::TOrthoProj(0.0f, 0.0f, img->width, img->height),
			    0x10);
			param_1->push_back(drawInit, 0x8);
			param_1->push_back(graffitiGroup, (i << 16) | 0x2000008);
			param_1->push_back(efbTex, 0x8);
		}
	}
}

JDrama::TViewObj* TMarDirector::initECTMir(
    TPerformList* param_1,
    JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>* param_2)
{
	JDrama::TEfbCtrlTex* mirrorTex
	    = JDrama::TNameRefGen::search<JDrama::TEfbCtrlTex>("鏡描画ステージ");

	mirrorTex->unk20.set(0x228);
	mirrorTex->unk44 = SMSVFilter_flicker;

	TMirrorCamera* mirrorCam
	    = JDrama::TNameRefGen::search<TMirrorCamera>("鏡カメラ");

	GXTexObj& obj = mirrorCam->unk60;
	mirrorTex->setTexAttb(obj);
	mirrorTex->setSrcRect(
	    JDrama::TRect(0, 0, GXGetTexObjWidth(&obj), GXGetTexObjHeight(&obj)));

	return mirrorTex;
}

extern void marker();

void TMarDirector::initECDisp(
    TPerformList* param_1,
    JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>* param_2,
    JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>* param_3)
{
	JDrama::TEfbCtrlDisp* stageDisp = new JDrama::TEfbCtrlDisp("stageDisp");
	stageDisp->JDrama::TEfbCtrl::setSrcRect(JDrama::TRect(
	    0, 0, (u16)SMSGetGameRenderWidth(), (u16)SMSGetGameRenderHeight()));
	param_2->insert(stageDisp);

	JDrama::TViewObj* composite3
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>("合成3");
	JDrama::TViewObj* specularSheen
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>("スペキュラシーン");

	TLensGlow* lensGlow       = nullptr;
	TLensFlare* lensFlare     = nullptr;
	JDrama::TOrthoProj* ortho = nullptr;

	JDrama::TViewObj* sunModel
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>("太陽モデル");

	if (sunModel) {
		lensGlow = new TLensGlow(true, "太陽遮蔽物グロー");
		param_2->insert(lensGlow);
		lensFlare = new TLensFlare("レンズフレア");
		param_2->insert(lensFlare);
	} else {
		sunModel = JDrama::TNameRefGen::search<JDrama::TViewObj>("夕日モデル");
		if (sunModel) {
			lensGlow = new TLensGlow(true, "太陽遮蔽物グロー");
			param_2->insert(lensGlow);
			lensFlare = new TLensFlare("レンズフレア");
			param_2->insert(lensFlare);
		}
	}

	if (specularSheen || lensFlare || lensGlow) {
		f32 w = (u16)SMSGetGameRenderWidth() / 2;
		f32 h = (u16)SMSGetGameRenderHeight() / 2;
		ortho = new JDrama::TOrthoProj(-w, h, w, -h);
	}

	JDrama::TOrthoProj* ortho2
	    = new JDrama::TOrthoProj(0.0f, 0.0f, (u16)SMSGetGameRenderHeight(),
	                             (u16)SMSGetGameRenderWidth());

	param_1->push_back(stageDisp, 0x80);
	param_1->push_back(
	    new JDrama::TViewport(SMSGetRederRect_Game(), "Screen 2D"), 0x8);
	param_1->push_back(ortho2, 0x10);
	param_1->push_back(composite3, 0x8);

	JDrama::TViewObj* setViewMtx
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>(
	        "J3D System Set View Mtx");
	JDrama::TViewObj* drawInit
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>("SMS Draw Init");
	JDrama::TDrawBufObj* drawBufLensFlare
	    = JDrama::TNameRefGen::search<JDrama::TDrawBufObj>("DrawBuf LensFlare");
	JDrama::TCamera* camera1
	    = JDrama::TNameRefGen::search<JDrama::TCamera>("camera 1");

	if (specularSheen || lensFlare || lensGlow) {
		param_1->push_back(ortho, 0x10);
		param_1->push_back(setViewMtx, 0x4);
		param_1->push_back(drawBufLensFlare, 0x480);
		if (specularSheen)
			param_1->push_back(specularSheen, 0x204);
		if (lensGlow)
			param_1->push_back(lensGlow, 0x204);
		param_1->push_back(drawInit, 0x8);
		param_1->push_back(drawBufLensFlare, 0x8);
		if (lensFlare) {
			param_1->push_back(camera1, 0x10);
			param_1->push_back(setViewMtx, 0x4);
			param_1->push_back(drawBufLensFlare, 0x80);
			param_1->push_back(lensFlare, 0x204);
			param_1->push_back(drawInit, 0x8);
			param_1->push_back(drawBufLensFlare, 0x8);
		}
	}

	JDrama::TCamera* drawBufChrOpa
	    = JDrama::TNameRefGen::search<JDrama::TCamera>("DrawBuf ChrOpa");
	JDrama::TCamera* drawBufChrXlu
	    = JDrama::TNameRefGen::search<JDrama::TCamera>("DrawBuf ChrXlu");

	param_1->push_back(camera1, 0x10);
	param_1->push_back(setViewMtx, 0x4);
	param_1->push_back(drawBufChrOpa, 0x480);
	param_1->push_back(drawBufChrXlu, 0x480);
	param_1->push_back("会話カーソル", 0x204);
	param_1->push_back("ターゲット矢印", 0x204);
	param_1->push_back(drawInit, 0x8);
	param_1->push_back(drawBufChrOpa, 0x8);
	param_1->push_back(drawBufChrXlu, 0x8);

	JDrama::TOrthoProj* ortho3
	    = new JDrama::TOrthoProj(0.0f, 0.0f, (u16)SMSGetGameRenderHeight(),
	                             (u16)SMSGetGameRenderWidth());
	param_1->push_back(ortho3, 0x10);

	JDrama::TViewObj* group2D2
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>("Group 2D 2");
	param_1->push_back(group2D2, 0x8);

	JDrama::TOrthoProj* ortho4
	    = new JDrama::TOrthoProj(0.0f, 16.0f, 600.0f, 464.0f);
	param_1->push_back(ortho4, 0x10);

	JDrama::TViewObj* group2D
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>("Group 2D");
	param_1->push_back(group2D, 0x8);

	param_1->push_back(ortho4, 0x10);
	JDrama::TViewObj* guide
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>("Guide");
	param_1->push_back(guide, 0x8);
	param_1->push_back(stageDisp, 0x8);
}

extern JPAEmitterManager* gpEmitterManager4D2;

void TMarDirector::setupPerformList_console()
{
	JDrama::TViewObjPtrListT<JDrama::TViewObj>* list
	    = JDrama::TNameRefGen::search<
	        JDrama::TViewObjPtrListT<JDrama::TViewObj> >("Group 2D");

	list->insert(new TEmitterViewObj(gpEmitterManager4D2));

	unk30->push_back(list, 3);
	unk30->push_back("Group 2D 2", 3);
	unk30->push_back("Guide", 3);
}
