#include <System/SelectDir.hpp>

#include <dolphin/os.h>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <JSystem/JDrama/JDRDStageGroup.hpp>
#include <JSystem/JDrama/JDRDStage.hpp>
#include <JSystem/JDrama/JDRCamera.hpp>
#include <JSystem/JDrama/JDREfbCtrl.hpp>
#include <JSystem/JDrama/JDRScreen.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <GC2D/ScrnFader.hpp>
#include <GC2D/SelectMenu.hpp>
#include <MSound/MSound.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <System/Application.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/FlagManager.hpp>
#include <System/MarioGamePad.hpp>
#include <System/Resolution.hpp>

extern OSThread gSetupThread;
extern u8* gpSetupThreadStack;

TSelectDir::TSelectDir()
    : unk20(0)
    , unk24(0)
    , unk28(0)
    , unk2C(0)
    , unk30(0)
    , unk34(0)
    , unk38(false)
    , unk3C(0)
    , unk40(0)
    , unk44(0)
    , unk48(0)
    , unk4C(false)
{
}

TSelectDir::~TSelectDir()
{
	JKRMemArchive* arc = (JKRMemArchive*)JKRFileLoader::getVolume("select");
	if (arc)
		arc->unmountFixed();

	unk18->offFlag(1);
}

void* TSelectDir::setupThreadFunc(void* param_1)
{
	((TSelectDir*)param_1)->rsetup();
}

void TSelectDir::setup(JDrama::TDisplay* display, TMarioGamePad* gamePad,
                       unsigned char stage)
{
	unk1C = display;
	unk18 = gamePad;
	SMSRumbleMgr->reset();
	unk40 = stage;

	OSCreateThread(&gSetupThread, &setupThreadFunc, this,
	               gpSetupThreadStack + 0x10000, 0x10000, 0x11, 0);
	OSResumeThread(&gSetupThread);
}

void TSelectDir::changeOrder()
{
	unk44->unkC.on(0xb);
	unk48->unkC.off(0xb);
}

int TSelectDir::rsetup()
{
	void* arcData = SMSLoadArchive("/data/select.arc", 0, 0, 0);

	JKRMemArchive* archive = new JKRMemArchive;
	if (!archive->mountFixed(arcData, MBF_0))
		return 1;

	unk2C = archive;

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* root
	    = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("root View Objs");
	unk10 = root;

	unk14 = new JDrama::TDStageGroup(unk1C);
	unk20 = new TSelectMenu("<TSelectMenu>");
	unk28 = new TSelectShineManager("<SelectShineManger>");
	unk24 = new TSelectGrad("<TSelectGrad>");
	unk24->setStageColor(unk40);

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* group3D
	    = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("Group 3D");
	JDrama::TViewObjPtrListT<JDrama::TViewObj>* group2D
	    = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("Group 2D");
	JDrama::TViewObjPtrListT<JDrama::TViewObj>* groupGrad
	    = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("Group Grad");
	JDrama::TViewObjPtrListT<JDrama::TViewObj>* group2DParticle
	    = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("Group 2D Particle");

	root->getChildren().push_back(group2D);
	root->getChildren().push_back(group3D);
	root->getChildren().push_back(groupGrad);
	root->getChildren().push_back(group2DParticle);

	group2D->getChildren().push_back(unk20);
	group3D->getChildren().push_back(unk28);
	groupGrad->getChildren().push_back(unk24);

	unk18->mFlags = 1;
	unk20->unk100 = unk18;

	JPAResourceManager* resourceManager2D = new JPAResourceManager(9, 0x200, 0);
	JPAResourceManager* resourceManager3D = new JPAResourceManager(9, 0x200, 0);

	resourceManager2D->load("/select/particle/ms_2d_shine_promi.jpa", 0);
	resourceManager2D->load("/select/particle/ms_2d_shine_senko.jpa", 1);
	resourceManager2D->load("/select/particle/ms_2d_shine_kira.jpa", 2);
	resourceManager2D->load("/select/particle/ms_2d_shine_kira_em.jpa", 3);
	resourceManager3D->load("/select/particle/ms_2d_scsel_on_a.jpa", 4);
	resourceManager3D->load("/select/particle/ms_2d_scsel_on_a2.jpa", 5);
	resourceManager3D->load("/select/particle/ms_2d_scsel_on_b.jpa", 6);
	resourceManager3D->load("/select/particle/ms_2d_scsel_on_c.jpa", 7);
	resourceManager3D->load("/select/particle/ms_2d_scsel_on_d.jpa", 8);

	unk30 = new JPAEmitterManager(resourceManager2D, 0x400, 0x80, 0x100, 0);
	unk34 = new JPAEmitterManager(resourceManager3D, 0x400, 0x80, 0x100, 0);

	TEmitterViewObj* emitterView2D = new TEmitterViewObj(unk30);
	group3D->getChildren().push_back(emitterView2D);

	TEmitterViewObj* emitterView3D = new TEmitterViewObj(unk34);
	group2DParticle->getChildren().push_back(emitterView3D);

	JDrama::TDStageDisp* stageDisp = new JDrama::TDStageDisp;
	unk14->getChildren().push_back(stageDisp);

	JDrama::TRect rect(0, 0, SMSGetTitleRenderWidth(),
	                   SMSGetTitleRenderHeight());
	stageDisp->getEfbCtrlDisp()->TEfbCtrl::setSrcRect(rect);

	JDrama::TOrthoProj* gradCamera
	    = new JDrama::TOrthoProj(-100.0f, 100.0f, 0.0f, 16.0f, 600.0f, 464.0f);
	groupGrad->getChildren().push_back(gradCamera);

	JDrama::TScreen* gradScreen = new JDrama::TScreen(rect, "Screen Grad");
	stageDisp->getUnk14()->getChildren().push_back(gradScreen);
	gradScreen->assignCamera(gradCamera);
	gradScreen->assignViewObj(groupGrad);

	JDrama::TOrthoProj* screen2DCamera
	    = new JDrama::TOrthoProj(-100.0f, 100.0f, 0.0f, 16.0f, 600.0f, 464.0f);
	group2D->getChildren().push_back(screen2DCamera);

	JDrama::TScreen* screen2D = new JDrama::TScreen(rect, "Screen 2D");
	stageDisp->getUnk14()->getChildren().push_back(screen2D);
	screen2D->assignCamera(screen2DCamera);
	screen2D->assignViewObj(group2D);
	unk48 = screen2D;

	JDrama::TLookAtCamera* camera3D = new JDrama::TLookAtCamera(
	    JGeometry::TVec3<f32>(300.0f, 240.0f, 1300.0f),
	    JGeometry::TVec3<f32>(300.0f, 240.0f, 0.0f),
	    JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f), 30.0f, 1.3333334f,
	    "<TLookAtCamera>");
	group3D->getChildren().push_back(camera3D);

	JDrama::TScreen* screen3D = new JDrama::TScreen(rect, "Screen 3D");
	stageDisp->getUnk14()->getChildren().push_back(screen3D);
	screen3D->assignCamera(camera3D);
	screen3D->assignViewObj(group3D);

	JDrama::TOrthoProj* screen2DCamera2
	    = new JDrama::TOrthoProj(-100.0f, 100.0f, 0.0f, 16.0f, 600.0f, 464.0f);
	group2D->getChildren().push_back(screen2DCamera2);

	JDrama::TScreen* screen2D2 = new JDrama::TScreen(rect, "Screen 2D");
	stageDisp->getUnk14()->getChildren().push_back(screen2D2);
	screen2D2->assignCamera(screen2DCamera2);
	screen2D2->assignViewObj(group2D);
	unk44 = screen2D2;

	JDrama::TOrthoProj* particleCamera
	    = new JDrama::TOrthoProj(-500.0f, 500.0f, 0.0f, 16.0f, 600.0f, 464.0f);
	group2DParticle->getChildren().push_back(particleCamera);

	JDrama::TScreen* particleScreen = new JDrama::TScreen(rect, "Screen Grad");
	stageDisp->getUnk14()->getChildren().push_back(particleScreen);
	particleScreen->assignCamera(particleCamera);
	particleScreen->assignViewObj(group2DParticle);

	unk44->unkC.off(0xb);
	unk48->unkC.on(0xb);

	return 0;
}

// TODO: The target uses a 0xD0-byte frame while this uses 0x50. All 177
// instructions otherwise match in sequence, so this is the same unresolved
// stack-allocation issue seen in rsetup rather than missing direct behavior.
int TSelectDir::direct()
{
	if (!unk38) {
		if (!OSIsThreadTerminated(&gSetupThread))
			return TApplication::APP_STATE_WAIT;

		void* res;
		OSJoinThread(&gSetupThread, &res);
		if (res != 0)
			return TApplication::APP_STATE_GAMEPLAY;

		unk38 = true;
		unk20->initData(unk40, unk2C, unk28, this);
		unk20->startMove();
		unk20->startOpenWindow();

		gpApplication.mFader->startWipe(0xe, 0.4f, 0.0f);
		gpApplication.mFader->setColor(
		    unk40 == 9 ? JUtility::TColor(0xff, 0xff, 0xff, 0xff)
		               : JUtility::TColor(0, 0, 0, 0xff));
		gpMSound->initSound();
		return TApplication::APP_STATE_WAIT;
	}

	JDrama::TDirector::direct();

	switch (gpApplication.mFader->mFadeStatus) {
	case TSMSFader::FADE_STATUS_FULLY_FADED_IN:
	case TSMSFader::FADE_STATUS_FADING_IN:
		if (unk20->unk14B)
			return TApplication::APP_STATE_DONE;

		if (unk20->unk14A) {
			gpApplication.mNextArea.unk1 = unk20->unk13B;
			TFlagManager::smInstance->setFlag(0x40003, unk20->unk13B);
			gpApplication.mFader->startWipe(0xf, 1.0f, 0.0f);
			gpApplication.mFader->setColor(
			    JUtility::TColor(0xff, 0xff, 0xff, 0xff));
			MSound* sound = gpMSound;
			sound->fadeOutAllSound(SMSGetVSyncTimesPerSec());
		}
		break;
	}

	if (unk18->isSomethingPushed() && !unk4C) {
		unk4C = true;
		gpApplication.mFader->startWipe(4, 1.0f, 0.0f);
		MSound* sound = gpMSound;
		sound->fadeOutAllSound(SMSGetVSyncTimesPerSec() * 0.4f);
		unk10->unkC.on(3);
	}

	if (gpApplication.mFader->mFadeStatus
	    == TSMSFader::FADE_STATUS_FULLY_FADED_OUT) {
		gpMSound->stopAllSound();
		if (unk18->isSomethingPushed())
			return TApplication::APP_STATE_DONE;
		else
			return TApplication::APP_STATE_GAMEPLAY;
	}

	return TApplication::APP_STATE_DEFAULT;
}
