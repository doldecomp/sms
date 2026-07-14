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
    JDrama::TViewObjPtrListT<JDrama::TViewObj>* perf_event_group,
    JDrama::TViewObjPtrListT<JDrama::TViewObj>* scene)
{
	if (gpPollution->getJointModelNum() == 0) {
		TBathWaterManager* bathtubWater
		    = JDrama::TNameRefGen::search<TBathWaterManager>("バスタブの水");
		if (bathtubWater)
			param_2->push_back(bathtubWater->getPreprocessor(), CUE_DRAW);
	} else {
		JDrama::TViewObjPtrListT<JDrama::TViewObj>* graffitiGroup
		    = JDrama::TNameRefGen::search<
		        JDrama::TViewObjPtrListT<JDrama::TViewObj> >("落書きグループ");
		JDrama::TViewObj* drawInit
		    = JDrama::TNameRefGen::search<JDrama::TViewObj>("SMS Draw Init");

		JDrama::TEfbCtrlTex* graffitiEfbTex
		    = new JDrama::TEfbCtrlTex("graffito check");
		scene->insert(graffitiEfbTex);

		JDrama::TRect rect;
		rect.set(0, 0, 0x200, 0x200);
		graffitiEfbTex->setSrcRect(rect);
		param_1->push_back(graffitiEfbTex, CUE_DRAW_INIT);

		param_1->push_back(new JDrama::TViewport(rect, "graffito"), CUE_DRAW);
		param_1->push_back(new JDrama::TOrthoProj(0.0f, 0.0f, 512.0f, 512.0f),
		                   CUE_SET_PROJECTION);
		param_1->push_back(drawInit, CUE_DRAW);
		param_1->push_back(graffitiGroup, CUE_UNK1000000);
		param_1->push_back(graffitiEfbTex, CUE_DRAW);

		for (int i = 0; i < gpPollution->getJointModelNum(); ++i) {
			JDrama::TEfbCtrlTex* efbTex = new JDrama::TEfbCtrlTex("graffito");
			scene->insert(efbTex);
			const ResTIMG* img = gpPollution->getLayer(i)->getPollutionImage();
			efbTex->mImagePtr  = (u8*)&img + img->imageDataOffset;
			efbTex->mWidth     = img->width;
			efbTex->mHeight    = img->height;
			efbTex->mTexFmt    = GX_CTF_R8;
			JDrama::TRect rect;
			rect.set(0, 0, img->width, img->height);
			efbTex->setSrcRect(rect);
			param_2->push_back(efbTex, CUE_DRAW_INIT);
			param_2->push_back(new JDrama::TViewport(rect, "graffito"),
			                   CUE_DRAW);
			param_2->push_back(
			    new JDrama::TOrthoProj(0.0f, 0.0f, img->width, img->height),
			    CUE_SET_PROJECTION);
			param_1->push_back(drawInit, CUE_DRAW);
			// TODO: what's with the cue here? Layer ID encoded inside?
			param_1->push_back(graffitiGroup, (i << 16)
			                                      | CUE_SEMITRANSPARENT_PRIO_2
			                                      | CUE_DRAW);
			param_1->push_back(efbTex, CUE_DRAW);
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

	param_1->push_back(stageDisp, CUE_DRAW_INIT);
	param_1->push_back(
	    new JDrama::TViewport(SMSGetRederRect_Game(), "Screen 2D"), CUE_DRAW);
	param_1->push_back(ortho2, CUE_SET_PROJECTION);
	param_1->push_back(composite3, CUE_DRAW);

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
		param_1->push_back(ortho, CUE_SET_PROJECTION);
		param_1->push_back(setViewMtx, CUE_CALC_VIEW);
		param_1->push_back(drawBufLensFlare,
		                   CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
		if (specularSheen)
			param_1->push_back(specularSheen, CUE_CALC_VIEW | CUE_ENTRY);
		if (lensGlow)
			param_1->push_back(lensGlow, CUE_CALC_VIEW | CUE_ENTRY);
		param_1->push_back(drawInit, CUE_DRAW);
		param_1->push_back(drawBufLensFlare, CUE_DRAW);
		if (lensFlare) {
			param_1->push_back(camera1, CUE_SET_PROJECTION);
			param_1->push_back(setViewMtx, CUE_CALC_VIEW);
			param_1->push_back(drawBufLensFlare, CUE_DRAW_INIT);
			param_1->push_back(lensFlare, CUE_CALC_VIEW | CUE_ENTRY);
			param_1->push_back(drawInit, CUE_DRAW);
			param_1->push_back(drawBufLensFlare, CUE_DRAW);
		}
	}

	JDrama::TCamera* drawBufChrOpa
	    = JDrama::TNameRefGen::search<JDrama::TCamera>("DrawBuf ChrOpa");
	JDrama::TCamera* drawBufChrXlu
	    = JDrama::TNameRefGen::search<JDrama::TCamera>("DrawBuf ChrXlu");

	param_1->push_back(camera1, CUE_SET_PROJECTION);
	param_1->push_back(setViewMtx, CUE_CALC_VIEW);
	param_1->push_back(drawBufChrOpa, CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	param_1->push_back(drawBufChrXlu, CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	param_1->push_back("会話カーソル", CUE_CALC_VIEW | CUE_ENTRY);
	param_1->push_back("ターゲット矢印", CUE_CALC_VIEW | CUE_ENTRY);
	param_1->push_back(drawInit, CUE_DRAW);
	param_1->push_back(drawBufChrOpa, CUE_DRAW);
	param_1->push_back(drawBufChrXlu, CUE_DRAW);

	JDrama::TOrthoProj* ortho3
	    = new JDrama::TOrthoProj(0.0f, 0.0f, (u16)SMSGetGameRenderHeight(),
	                             (u16)SMSGetGameRenderWidth());
	param_1->push_back(ortho3, CUE_SET_PROJECTION);

	JDrama::TViewObj* group2D2
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>("Group 2D 2");
	param_1->push_back(group2D2, CUE_DRAW);

	JDrama::TOrthoProj* ortho4
	    = new JDrama::TOrthoProj(0.0f, 16.0f, 600.0f, 464.0f);
	param_1->push_back(ortho4, CUE_SET_PROJECTION);

	JDrama::TViewObj* group2D
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>("Group 2D");
	param_1->push_back(group2D, CUE_DRAW);

	param_1->push_back(ortho4, CUE_SET_PROJECTION);
	JDrama::TViewObj* guide
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>("Guide");
	param_1->push_back(guide, CUE_DRAW);
	param_1->push_back(stageDisp, CUE_DRAW);
}

extern JPAEmitterManager* gpEmitterManager4D2;

void TMarDirector::setupPerformList_console()
{
	JDrama::TViewObjPtrListT<JDrama::TViewObj>* list
	    = JDrama::TNameRefGen::search<
	        JDrama::TViewObjPtrListT<JDrama::TViewObj> >("Group 2D");

	list->insert(new TEmitterViewObj(gpEmitterManager4D2));

	unk30->push_back(list, CUE_MOVE | CUE_CALC_ANIM);
	unk30->push_back("Group 2D 2", CUE_MOVE | CUE_CALC_ANIM);
	unk30->push_back("Guide", CUE_MOVE | CUE_CALC_ANIM);
}
