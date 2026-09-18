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

// Parked helper for a JDRNameRefGen.hpp item: the ROM's
// `JDrama::TNameRefGen::search2` binds its result before returning it, which is
// worth 8-16 bytes of frame per expansion (the "named-and-returned result"
// carrier of closure batch 74/82). Measured here: +16 per site in initECTMir
// (0x60 -> 0x80 over two sites) and +8 in setupPerformList_console; it is
// parked rather than put in the header because 40 other units expand search2
// and the header batch owns that file. It is deliberately NOT used in
// initECDisp: 13 sites there overshoot the ROM's 0x370 by 48.
static inline JDrama::TNameRef* ECTSearch(const char* name)
{
	JDrama::TNameRef* ref = JDrama::TNameRefGen::search2(name);
	return ref;
}

// TODO: 93.9%, 16 bytes of frame short (0x208 vs 0x218) after the ECTSearch
// level (+32 over three sites).
void TMarDirector::initECTGft(
    TPerformList* param_1, TPerformList* param_2,
    JDrama::TViewObjPtrListT<JDrama::TViewObj>* perf_event_group,
    JDrama::TViewObjPtrListT<JDrama::TViewObj>* scene)
{
	if (gpPollution->getJointModelNum() == 0) {
		TBathWaterManager* bathtubWater
		    = (TBathWaterManager*)ECTSearch("バスタブの水");
		if (bathtubWater)
			param_2->push_back(bathtubWater->getPreprocessor(), CUE_DRAW);

		return;
	}

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* graffitiGroup
	    = (JDrama::TViewObjPtrListT<JDrama::TViewObj>*)ECTSearch(
	        "落書きグループ");
	JDrama::TViewObj* drawInit = (JDrama::TViewObj*)ECTSearch("SMS Draw Init");

	JDrama::TEfbCtrlTex* graffitiEfbTex
	    = new JDrama::TEfbCtrlTex("graffito check");
	scene->insert(graffitiEfbTex);

	JDrama::TRect rect;
	rect.set(0, 0, 0x200, 0x200);
	graffitiEfbTex->setSrcRect(rect);
	param_1->push_back(graffitiEfbTex, CUE_DRAW_INIT);

	param_1->push_back(new JDrama::TViewport(rect, "graffito"), CUE_DRAW);
	param_1->push_back(
	    new JDrama::TOrthoProj(-1.0f, 1.0f, 0.0f, 0.0f, 512.0f, 512.0f),
	    CUE_SET_PROJECTION);
	param_1->push_back(drawInit, CUE_DRAW);
	param_1->push_back(graffitiGroup, CUE_UNK1000000);
	param_1->push_back(graffitiEfbTex, CUE_DRAW);

	for (int i = 0; i < gpPollution->getJointModelNum(); ++i) {
		JDrama::TEfbCtrlTex* efbTex = new JDrama::TEfbCtrlTex("graffito");
		scene->insert(efbTex);

		const ResTIMG* img = gpPollution->getLayer(i)->getPollutionImage();

		efbTex->setImgPtr((u8*)&img + img->imageDataOffset);
		JDrama::TSize size(img->width, img->height);
		efbTex->setDstSize(size);
		efbTex->setTexFmt(GX_CTF_R8);
		JDrama::TRect rect;
		rect.set(0, 0, size.mWidth, size.mHeight);
		efbTex->setSrcRect(rect);

		param_2->push_back(efbTex, CUE_DRAW_INIT);
		param_2->push_back(new JDrama::TViewport(rect, "graffito"), CUE_DRAW);
		param_2->push_back(new JDrama::TOrthoProj(-1.0f, 1.0f, 0.0f, 0.0f,
		                                          img->width, img->height),
		                   CUE_SET_PROJECTION);
		param_1->push_back(drawInit, CUE_DRAW);
		param_1->push_back(graffitiGroup, (i << CUE_OFFSET_POLLUTION_LAYER)
		                                      | CUE_SEMITRANSPARENT_PRIO_2
		                                      | CUE_DRAW);
		param_1->push_back(efbTex, CUE_DRAW);
	}
}

// Binding level over the address of a struct member, worth +8 of low region
// in TMarDirector::initECTMir (batch 130).
static inline const GXTexObj* MarDirectorInitECTUnk60(const TMirrorCamera* p)
{
	const GXTexObj* v60 = &p->unk60;
	return v60;
}

JDrama::TViewObj* TMarDirector::initECTMir(
    TPerformList* param_1,
    JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>* param_2)
{
	JDrama::TEfbCtrlTex* mirrorTex
	    = (JDrama::TEfbCtrlTex*)ECTSearch("鏡描画ステージ");

	mirrorTex->unk20.set(0x228);
	mirrorTex->mVFilter = SMSVFilter_flicker;

	TMirrorCamera* mirrorCam = (TMirrorCamera*)ECTSearch("鏡カメラ");

	GXTexObj& obj = *MarDirectorInitECTUnk60(mirrorCam);
	mirrorTex->setTexAttb(obj);
	// TODO: 99.8%, frame 0x80 against the ROM's 0x88. Passing the rectangle
	// as an unnamed temporary instead
	// (`setSrcRect(JDrama::TRect(0, 0, w, h))`, same instructions, the
	// `bl JUTRect::set` and the `&temp` hand-off are identical) makes the
	// frame byte-exact and cuts the diff from 12 operand mismatches to 3,
	// but leaves the rectangle's slot at 0x60 where the ROM has 0x64 (+4
	// low) and swaps the last two argument loads -- retail materialises the
	// receiver (`addi r3, r29, 0`) before the rectangle's address, we do it
	// the other way round. `fuzzy_match` scores that spelling 99.6 because
	// it weighs the register operands above the `stwu`, so it is not
	// committed; whichever lever supplies the last +4 should land both at
	// once. Measured and inert on top of it: qualifying the call as
	// `JDrama::TEfbCtrl::setSrcRect`, a `JDrama::TEfbCtrl*` receiver local,
	// one more binding level above the first ECTSearch, `unk20 = 0x228`
	// instead of `unk20.set(0x228)`, and swapping the width/height
	// arguments.
	JDrama::TRect rect(0, 0, GXGetTexObjWidth(&obj), GXGetTexObjHeight(&obj));
	mirrorTex->setSrcRect(rect);

	return mirrorTex;
}

// TODO: 97.55%. `initECDisp` is a *static* member in the ROM, like its
// neighbours `initECTMir`, `initECTGft` and `preEntry`: retail's prologue
// clobbers r4 with the `.rodata` base immediately and keeps r3 and r5, so the
// `TPerformList*` the whole body uses as `push_back`'s receiver arrives in
// **r3**, which can only happen with no `this`. The single call site sets only
// r3/r4/r5 (`lwz r3, 0x24(r30)` / `lwz r4, 0xa60(r1)` / `mr r5, r28`), which
// confirms the three-parameter static shape. With `static` the parameter
// registers line up and `insert`'s receiver is the *third* parameter (r5), so
// the five `insert` calls go through `param_3` and `param_2` is genuinely
// unused in this revision. Applied in header round 20 (batch 117): zero
// regressions whole-tree, initECDisp 97.50 -> 97.55 and the single caller
// `TMarDirector::setupObjects` 98.00 -> 98.08 (the `this` argument it no
// longer has to set up).
//
// What is left after that is 112 bytes of frame (0x300 vs 0x370) and one
// extra callee-saved register: retail saves r20-r31 (`stmw r20`) where we
// save r21-r31, with the whole set shifted by one from the very first
// expansion. The ECTSearch level above overshoots here (+152 over 13 sites),
// so this function's low region is not the search chain; the rotation starts
// at the TEfbCtrlDisp construction, before any search.
void TMarDirector::initECDisp(
    TPerformList* param_1,
    JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>* param_2,
    JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>* param_3)
{
	JDrama::TEfbCtrlDisp* stageDisp = new JDrama::TEfbCtrlDisp("stageDisp");
	stageDisp->JDrama::TEfbCtrl::setSrcRect(JDrama::TRect(
	    0, 0, (u16)SMSGetGameRenderWidth(), (u16)SMSGetGameRenderHeight()));
	param_3->insert(stageDisp);

	JDrama::TViewObj* composite3
	    = (JDrama::TViewObj*)JDrama::TNameRefGen::search2("合成3");
	JDrama::TViewObj* specularSheen
	    = (JDrama::TViewObj*)JDrama::TNameRefGen::search2("スペキュラシーン");

	TLensGlow* lensGlow       = nullptr;
	TLensFlare* lensFlare     = nullptr;
	JDrama::TOrthoProj* ortho = nullptr;

	JDrama::TViewObj* sunModel
	    = (JDrama::TViewObj*)JDrama::TNameRefGen::search2("太陽モデル");

	if (sunModel) {
		lensGlow = new TLensGlow(true, "太陽遮蔽物グロー");
		param_3->insert(lensGlow);
		lensFlare = new TLensFlare("レンズフレア");
		param_3->insert(lensFlare);
	} else {
		sunModel = (JDrama::TViewObj*)JDrama::TNameRefGen::search2("夕日モデル");
		if (sunModel) {
			lensGlow = new TLensGlow(true, "太陽遮蔽物グロー");
			param_3->insert(lensGlow);
			lensFlare = new TLensFlare("レンズフレア");
			param_3->insert(lensFlare);
		}
	}

	if (specularSheen || lensFlare || lensGlow) {
		f32 w = (u16)SMSGetGameRenderWidth() / 2;
		f32 h = (u16)SMSGetGameRenderHeight() / 2;
		ortho = new JDrama::TOrthoProj(-1.0f, 1.0f, -w, h, w, -h);
	}

	JDrama::TOrthoProj* ortho2 = new JDrama::TOrthoProj(
	    10.0f, 300000.0f, 0.0f, 0.0f, (u16)SMSGetGameRenderHeight(),
	    (u16)SMSGetGameRenderWidth());

	param_1->push_back(stageDisp, CUE_DRAW_INIT);
	param_1->push_back(
	    new JDrama::TViewport(SMSGetRederRect_Game(), "Screen 2D"), CUE_DRAW);
	param_1->push_back(ortho2, CUE_SET_PROJECTION);
	param_1->push_back(composite3, CUE_DRAW);

	JDrama::TViewObj* setViewMtx
	    = (JDrama::TViewObj*)JDrama::TNameRefGen::search2(
	        "J3D System Set View Mtx");
	JDrama::TViewObj* drawInit
	    = (JDrama::TViewObj*)JDrama::TNameRefGen::search2("SMS Draw Init");
	JDrama::TDrawBufObj* drawBufLensFlare
	    = (JDrama::TDrawBufObj*)JDrama::TNameRefGen::search2(
	        "DrawBuf LensFlare");
	JDrama::TCamera* camera1
	    = (JDrama::TCamera*)JDrama::TNameRefGen::search2("camera 1");

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
	    = (JDrama::TCamera*)JDrama::TNameRefGen::search2("DrawBuf ChrOpa");
	JDrama::TCamera* drawBufChrXlu
	    = (JDrama::TCamera*)JDrama::TNameRefGen::search2("DrawBuf ChrXlu");

	param_1->push_back(camera1, CUE_SET_PROJECTION);
	param_1->push_back(setViewMtx, CUE_CALC_VIEW);
	param_1->push_back(drawBufChrOpa, CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	param_1->push_back(drawBufChrXlu, CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	param_1->push_back("会話カーソル", CUE_CALC_VIEW | CUE_ENTRY);
	param_1->push_back("ターゲット矢印", CUE_CALC_VIEW | CUE_ENTRY);
	param_1->push_back(drawInit, CUE_DRAW);
	param_1->push_back(drawBufChrOpa, CUE_DRAW);
	param_1->push_back(drawBufChrXlu, CUE_DRAW);

	JDrama::TOrthoProj* ortho3 = new JDrama::TOrthoProj(
	    -1.0f, 1.0f, 0.0f, 0.0f, (u16)SMSGetGameRenderHeight(),
	    (u16)SMSGetGameRenderWidth());
	param_1->push_back(ortho3, CUE_SET_PROJECTION);

	JDrama::TViewObj* group2D2
	    = (JDrama::TViewObj*)JDrama::TNameRefGen::search2("Group 2D 2");
	param_1->push_back(group2D2, CUE_DRAW);

	JDrama::TOrthoProj* ortho4
	    = new JDrama::TOrthoProj(-500.0f, 500.0f, 0.0f, 16.0f, 600.0f, 464.0f);
	param_1->push_back(ortho4, CUE_SET_PROJECTION);

	JDrama::TViewObj* group2D
	    = (JDrama::TViewObj*)JDrama::TNameRefGen::search2("Group 2D");
	param_1->push_back(group2D, CUE_DRAW);

	param_1->push_back(ortho4, CUE_SET_PROJECTION);
	JDrama::TViewObj* guide
	    = (JDrama::TViewObj*)JDrama::TNameRefGen::search2("Guide");
	param_1->push_back(guide, CUE_DRAW);
	param_1->push_back(stageDisp, CUE_DRAW);
}

extern JPAEmitterManager* gpEmitterManager4D2;

// TODO: 99.8%, frame exact; every referenced slot is 4 bytes low, i.e. one
// +4 of low region below the ECTSearch expansion, which is the earliest one.
// Rejected: a second pointer-returning level over ECTSearch (a typed
// `ECTSearchViewObjList` binding the cast result) -- a pointer return is worth
// +4 elsewhere but is +8 here, overshooting to 0x88 and adding an
// instruction (99.8 -> 97.2). No +4 lever is known for this pool.
void TMarDirector::setupPerformList_console()
{
	JDrama::TViewObjPtrListT<JDrama::TViewObj>* list
	    = (JDrama::TViewObjPtrListT<JDrama::TViewObj>*)ECTSearch("Group 2D");

	TEmitterViewObj* emitter = new TEmitterViewObj(gpEmitterManager4D2);
	list->insert(emitter);

	unk30->push_back(list, CUE_MOVE | CUE_CALC_ANIM);
	unk30->push_back("Group 2D 2", CUE_MOVE | CUE_CALC_ANIM);
	unk30->push_back("Guide", CUE_MOVE | CUE_CALC_ANIM);
}
