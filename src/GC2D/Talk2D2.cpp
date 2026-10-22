#include <Camera/Camera.hpp>
#include <GC2D/BoundPane.hpp>
#include <GC2D/GCConsole2.hpp>
#include <GC2D/Helpers.hpp>
#include <GC2D/MessageLoader.hpp>
#include <GC2D/MessageUtil.hpp>
#include <GC2D/Talk2D2.hpp>
#include <JSystem/J2D/J2DGrafContext.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/J2D/J2DPane.hpp>
#include <JSystem/J2D/J2DPicture.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/JAudio/JALibrary/JALSystem.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGeometry/JGVec2.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JSupport/JSUInputStream.hpp>
#include <JSystem/JSupport/JSUIosBase.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <JSystem/JSupport/JSUMemoryOutputStream.hpp>
#include <JSystem/JSupport/JSUOutputStream.hpp>
#include <JSystem/JSupport/JSURandomInputStream.hpp>
#include <JSystem/JSupport/JSURandomOutputStream.hpp>
#include <JSystem/JUtility/JUTRect.hpp>
#include <JSystem/JUtility/JUTResFont.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/ReinitGX.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MoveBG/MapObjHide.hpp>
#include <NPC/NpcBase.hpp>
#include <System/Application.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/MarioGamePad.hpp>
#include <dolphin/gx.h>
#include <dolphin/os.h>
#include <math.h>
#include <stdio.h>
#include <macros.h>
#include <version.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TTalk2D2* gpTalk2D;

u32 TTalk2D2::cColorTable[6] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFB48CFF,
	                             0x6EE6FFFF, 0xFFFF00FF, 0xAAFF50FF };

TTalk2D2::TTalk2D2(const char* param_1)
    : JDrama::TViewObj(param_1)
    , unk28(0)
    , unk2C(nullptr)
    , unk90(nullptr)
    , unk94(0.0f)
    , unk214(-1)
    , unk220(0x1A)
    , unk222(0)
    , unk248(STATE_UNK0)
    , unk24C(nullptr)
    , unk250(1)
    , unk251(0)
    , unk252(0)
    , unk254(nullptr)
    , unk264(3)
    , unk26A(0)
    , unk26B(0)
    , unk26C(0)
    , unk26D(0)
    , unk270(1)
    , unk274(0)
    , unk278(0)
    , unk27C(-1)
    , unk280(0)
    , unk2DC(0)
    , unk330(0)
    , unk332(0)
    , unk334(0)
    , unk338(0.04f)
    , unk33C(1.0f)
    , unk340(0x64)
{
	gpTalk2D = this;
	for (int i = 0; i < ARRAY_COUNT(unk9C); ++i)
		unk9C[i] = nullptr;

	for (int i = 0; i < ARRAY_COUNT(unk224); ++i) {
		unk224[i] = 0;
		unk6C[i]  = nullptr;
		unk78[i]  = nullptr;
		unk228[i] = 0;
	}
}

void TTalk2D2::load(JSUMemoryInputStream& param_1)
{
	JDrama::TViewObj::load(param_1);

	JKRArchive* archive = (JKRArchive*)JKRFileLoader::getVolume("game_6");

	unk2C = new J2DSetScreen("message_2.blo", archive);

	for (int i = 0; i < ARRAY_COUNT(unk30); i++) {
		unk30[i] = unk2C->search('me_1' + i);
		unk3C[i] = unk2C->search('bac1' + i);
		unk48[i] = unk2C->search('f_1' + i * 3);
		unk54[i] = unk2C->search('f_2' + i * 3);
		unk60[i] = unk2C->search('f_3' + i * 3);
		unk6C[i] = unk2C->search('cu_1' + i);
		unk78[i] = unk2C->search('cc_1' + i);
		unk84[i] = unk2C->search('cs_1' + i);
	}

	unk244 = new JUTTexture(
	    (const ResTIMG*)JKRGetResource("/game_6/timg/message_back_1.bti"));

	unk90 = unk2C->search('me_0');

	unk258 = new TMessageLoader;
#if defined(VERSION_GMSP01)
	switch (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)) {
	case MSLANG_ENGLISH:
		unk258->loadMessageData("/scene/map/message_en.bmg");
		break;
	case MSLANG_GERMAN:
		unk258->loadMessageData("/scene/map/message_ge.bmg");
		break;
	case MSLANG_FRENCH:
		unk258->loadMessageData("/scene/map/message_fr.bmg");
		break;
	case MSLANG_SPANISH:
		unk258->loadMessageData("/scene/map/message_sp.bmg");
		break;
	case MSLANG_ITALIAN:
		unk258->loadMessageData("/scene/map/message_it.bmg");
		break;
	default:
		unk258->loadMessageData("/scene/map/message.bmg");
		break;
	}
#elif defined(VERSION_GMSP01_NX)
	static const char* messageDataName[] = {
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		"/scene/map/message_jp.bmg",
		"/scene/map/message_us.bmg",
		"/scene/map/message_en.bmg",
		"/scene/map/message_ge.bmg",
		"/scene/map/message_fr.bmg",
		"/scene/map/message_sp.bmg",
		"/scene/map/message_it.bmg",
	};
	int lang = TFlagManager::getInstance()->getFlag(MSF_LANGUAGE);
	if (lang < 5 || lang > 11) {
		OSReport("Language overflow %d %s %d\n", lang, "Talk2D2.cpp", 194);
		for (;;)
			;
	}
	unk258->loadMessageData(messageDataName[lang]);
#else
	unk258->loadMessageData("/scene/map/message.bmg");
#endif
	unk25C = new TMessageLoader;
	unk25C->loadMessageData(VERSION_SELECT(
	    "/common/2d/sys_message.bmg", GMSP01("/cmn2d/sys_message.bmg"),
	    GMSP01_NX("/cmn2d/sys_message.bmg")));

	unk204 = unk2C->search('me_4');
	unk208 = (J2DTextBox*)unk2C->search('slct');
	unk208->setFont(gpSystemFont);

	for (int i = 0; i < ARRAY_COUNT(unk20C); i++) {
		unk20C[i] = unk2C->search('sc_1' + i);
		unk218[i] = new char[0x11];
	}

	unk10 = new J2DSetScreen("message_board_1.blo", archive);
	unk14 = new TBoundPane(unk10, 'mb_0');
	unk18 = (J2DTextBox*)unk10->search('text');
	SMSMakeTextBuffer(unk18, 0x200);
	unk18->setFont(gpSystemFont);
	unk1C = unk10->search('cu_1');
	unk20 = unk10->search('cc_1');
	unk24 = unk10->search('cs_1');
}

static const u32 scTalkSoundList[]
    = { MSD_SE_NPC_VM_PEACH_NORMAL,  MSD_SE_NPC_VM_PEACH_SURPRS,
	    MSD_SE_NPC_VM_PEACH_WORRY,   MSD_SE_NPC_VM_PEACH_ANGER_L,
	    MSD_SE_NPC_VM_PEACH_APPEAL,  MSD_SE_NPC_VM_PEACH_DOUBT,
	    MSD_SE_NPC_VM_KINOJ_NORMAL,  MSD_SE_NPC_VM_KINOJ_EXCIT,
	    MSD_SE_NPC_VM_KINOJ_ADVICE,  MSD_SE_NPC_VM_KINOJ_CONFUSE,
	    MSD_SE_NPC_VM_KINOJ_ASK,     MSD_SE_NPC_VM_KINOP_NORMAL,
	    MSD_SE_NPC_VM_KINOP_CRY_S,   MSD_SE_NPC_VM_KINOP_CRY_L,
	    MSD_SE_NPC_VM_KINOP_PEACE,   MSD_SE_NPC_VM_KINOP_SAD_S,
	    MSD_SE_NPC_VM_KINOP_ASK,     MSD_SE_NPC_VM_KINOP_CONFUSE,
	    MSD_SE_NPC_VM_KINOP_RECOVER, MSD_SE_NPC_VM_KINOP_SAD_L,
	    MSD_SE_NPC_VM_MONTM_NORMAL,  MSD_SE_NPC_VM_MONTM_LAUGH_D,
	    MSD_SE_NPC_VM_MONTM_DSIGST,  MSD_SE_NPC_VM_MONTM_ANGER_S,
	    MSD_SE_NPC_VM_MONTM_SURPRS,  MSD_SE_NPC_VM_MONTM_DOUBT,
	    MSD_SE_NPC_VM_MONTM_DISPLES, MSD_SE_NPC_VM_MONTM_CONFUSE,
	    MSD_SE_NPC_VM_MONTM_REGRET,  MSD_SE_NPC_VM_MONTM_PROUD,
	    MSD_SE_NPC_VM_MONTM_RECOVER, MSD_SE_NPC_VM_MONTM_INVITN,
	    MSD_SE_NPC_VM_MONTM_QUESTN,  MSD_SE_NPC_VM_MONTM_LAUGH,
	    MSD_SE_NPC_VM_MONTM_THANKS,  MSD_SE_NPC_VM_MONTW_NORAML,
	    MSD_SE_NPC_VM_MONTW_REGRET,  MSD_SE_NPC_VM_MONTW_INVITN,
	    MSD_SE_NPC_VM_MONTW_LAUGH,   MSD_SE_NPC_VM_MAREM_NORMAL,
	    MSD_SE_NPC_VM_MAREM_REGRET,  MSD_SE_NPC_VM_MAREM_LAUGH,
	    MSD_SE_NPC_VM_MAREM_APPEAL,  MSD_SE_NPC_VM_MAREM_SURPRS,
	    MSD_SE_NPC_VM_MAREM_SAD,     MSD_SE_NPC_VM_MAREM_ASK,
	    MSD_SE_NPC_VM_MAREM_PROMPT,  MSD_SE_NPC_VM_MAREM_THANKS,
	    MSD_SE_NPC_VM_MAREW_NORMAL,  MSD_SE_NPC_VM_MAREW_REGRET,
	    MSD_SE_NPC_VM_MAREW_LAUGH,   MSD_SE_NPC_VM_MAREW_APPEAL,
	    MSD_SE_NPC_VM_MAREW_SURPRS,  MSD_SE_NPC_VM_MAREW_SAD,
	    MSD_SE_NPC_VM_MAREW_ASK,     MSD_SE_NPC_VM_MAREW_PROMPT,
	    MSD_SE_NPC_VM_MAREW_THANKS,  MSD_SE_NPC_VM_MAREJ_NORMAL,
	    MSD_SE_NPC_VM_MAREJ_REGRET,  MSD_SE_NPC_VM_MAREJ_LAUGH,
	    MSD_SE_NPC_VM_MAREJ_APPEAL,  MSD_SE_NPC_VM_MAREJ_SURPRS,
	    MSD_SE_NPC_VM_MAREJ_SAD,     MSD_SE_NPC_VM_MAREJ_ASK,
	    MSD_SE_NPC_VM_MAREJ_PROMPT,  MSD_SE_NPC_VM_MAREJ_THANKS,
	    MSD_SE_NPC_VM_TANUK_NORMAL,  MSD_SE_NPC_VM_SUNFP_JOY,
	    MSD_SE_NPC_VM_SUNFP_SAD,     0xffffffff,
	    MSD_SE_NPC_VM_MONTW_LAUGH_D, MSD_SE_NPC_VM_MONTW_DSIGST,
	    MSD_SE_NPC_VM_MONTW_ANGER_S, MSD_SE_NPC_VM_MONTW_SURPRS,
	    MSD_SE_NPC_VM_MONTW_DOUBT,   MSD_SE_NPC_VM_MONTW_DISPLES,
	    MSD_SE_NPC_VM_MONTW_CONFUSE, MSD_SE_NPC_VM_MONTW_PROUD,
	    MSD_SE_NPC_VM_MONTW_RECOVER, MSD_SE_NPC_VM_MONTW_QUESTN,
	    MSD_SE_NPC_VM_MONTW_THANKS,  MSD_SE_NPC_VM_CMNTM_NORMAL,
	    MSD_SE_NPC_VM_CMNTM_LAUGH_D, MSD_SE_NPC_VM_CMNTM_DSIGST,
	    MSD_SE_NPC_VM_CMNTM_ANGER_S, MSD_SE_NPC_VM_CMNTM_SURPRS,
	    MSD_SE_NPC_VM_CMNTM_DOUBT,   MSD_SE_NPC_VM_CMNTM_DISPLES,
	    MSD_SE_NPC_VM_CMNTM_CONFUSE, MSD_SE_NPC_VM_CMNTM_REGRET,
	    MSD_SE_NPC_VM_CMNTM_PROUD,   MSD_SE_NPC_VM_CMNTM_RECOVER,
	    MSD_SE_NPC_VM_CMNTM_INVITN,  MSD_SE_NPC_VM_CMNTM_QUESTN,
	    MSD_SE_NPC_VM_CMNTM_LAUGH,   MSD_SE_NPC_VM_CMNTM_THANKS,
	    MSD_SE_NPC_VM_CMNTW_NORAML,  MSD_SE_NPC_VM_CMNTW_LAUGH_D,
	    MSD_SE_NPC_VM_CMNTW_DSIGST,  MSD_SE_NPC_VM_CMNTW_ANGER_S,
	    MSD_SE_NPC_VM_CMNTW_SURPRS,  MSD_SE_NPC_VM_CMNTW_DOUBT,
	    MSD_SE_NPC_VM_CMNTW_DISPLES, MSD_SE_NPC_VM_CMNTW_CONFUSE,
	    MSD_SE_NPC_VM_CMNTW_REGRET,  MSD_SE_NPC_VM_CMNTW_PROUD,
	    MSD_SE_NPC_VM_CMNTW_RECOVER, MSD_SE_NPC_VM_CMNTW_INVITN,
	    MSD_SE_NPC_VM_CMNTW_QUESTN,  MSD_SE_NPC_VM_CMNTW_LAUGH,
	    MSD_SE_NPC_VM_CMNTW_THANKS,  MSD_SE_NPC_VM_CMAREM_NORMAL,
	    MSD_SE_NPC_VM_CMAREM_REGRET, MSD_SE_NPC_VM_CMAREM_LAUGH,
	    MSD_SE_NPC_VM_CMAREM_APPEAL, MSD_SE_NPC_VM_CMAREM_SURPRS,
	    MSD_SE_NPC_VM_CMAREM_SAD,    MSD_SE_NPC_VM_CMAREM_ASK,
	    MSD_SE_NPC_VM_CMAREM_PROMPT, MSD_SE_NPC_VM_CMAREM_THANKS,
	    MSD_SE_NPC_VM_CMAREW_NORMAL, MSD_SE_NPC_VM_CMAREW_REGRET,
	    MSD_SE_NPC_VM_CMAREW_LAUGH,  MSD_SE_NPC_VM_CMAREW_APPEAL,
	    MSD_SE_NPC_VM_CMAREW_SURPRS, MSD_SE_NPC_VM_CMAREW_SAD,
	    MSD_SE_NPC_VM_CMAREW_ASK,    MSD_SE_NPC_VM_CMAREW_PROMPT,
	    MSD_SE_NPC_VM_CMAREW_THANKS, MSD_SE_SY_COLLECT_DELIGHT,
	    MSD_BGM_FANFARE_RACE,        MSD_SE_SY_NOT_COLLECT,
	    MSD_SE_NPC_VM_FMARIO_PSHOT,  MSD_SE_NPC_VM_MTMAN_NORMAL,
	    MSD_SE_NPC_VM_MTMAN_LOST };

void TTalk2D2::loadAfter()
{
	JDrama::TNameRef::loadAfter();

	JUTPoint p0(unk48[1]->getBounds().x1, unk48[1]->getBounds().y1);
	JUTPoint p1(0, unk54[1]->getBounds().y1 - unk220);
	JUTPoint p2(unk60[1]->getBounds().x2 - 10, unk60[1]->getBounds().y1);

	f32 length = 0.0f;
	f32 prevX, prevY;
	makeLine(&prevX, &prevY, 0.0f, p0, p1, p2);
	for (f32 t = 0.01f; t <= 1.0f; t += 0.01f) {
		f32 x, y;
		makeLine(&x, &y, t, p0, p1, p2);
		f32 dx = x - prevX;
		f32 dy = y - prevY;
		length += JGeometry::TUtil<f32>::sqrt(dx * dx + dy * dy);
		prevX = x;
		prevY = y;
	}
	unk94 = 1.0f / length;

	for (int i = 0; i < ARRAY_COUNT(unk48); ++i) {
		unk48[i]->hide();
		unk54[i]->hide();
		unk60[i]->hide();
	}

	for (int i = 0; i < ARRAY_COUNT(unk9C); ++i) {
		unk9C[i] = new J2DTextBox(0, JUTRect(0, 0, 20, 20),
		                          gpSystemFont->getResFont(), "あ", HBIND_LEFT,
		                          VBIND_CENTER);
#if defined(VERSION_GMSP01_NX)
		if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE) != 5)
			unk9C[i]->setFontSize(20, 24);
#elif !defined(VERSION_GMSJ01) && !defined(VERSION_GMSJ01_01)
		unk9C[i]->setFontSize(20, 24);
#endif
		unk9C[i]->setBlackWhite(0xFFFFFF00, 0xFFFFFFFF);
		unk9C[i]->hide();
	}
	for (int i = 0; i < ARRAY_COUNT(unk234); ++i) {
		unk234[i] = i + 1.0f;
		unk6C[i]->hide();
	}
	unk244->mWrapT = 1;
	unk244->mWrapS = 0;
	unk90->move(393, 115);
	unk90->rotate(-18.0f);
	unk330 = unk90->getBounds().x1;
	unk332 = unk90->getBounds().y1;
	unk334 = unk90->getRotate();
	unk204->hide();
	char* spaces = new char[0x5e];
	for (int i = 0; i < 0x5d; i++)
		spaces[i] = ' ';
	spaces[0x5d] = 0;
	unk208->setString(spaces);
	setupTextBox(unk25C->getMessageData(), unk25C->getMessageEntry(3));
	unk260 = unk25C;
	const char* names[10]
	    = { "空港沈みモンテ", "モンテ1", "", "", "", "", "", "", "", "" };
	const int ids[10] = { VERSION_SELECT(0x2B, GMSJ01(0x26)),
		                  VERSION_SELECT(0x28, GMSJ01(0x23)) };
	for (int i = 0; i < ARRAY_COUNT(unk2E0); i++) {
		unk2E0[i].unk0 = (TBaseNPC*)JDrama::TNameRefGen::search(names[i]);
		unk2E0[i].unk4 = ids[i];
	}
}

void TTalk2D2::setMessageID(u32 param_1, u32 param_2)
{
	TBaseNPC* npc = SMSGetMarDirector()->getTalkingNPC();

	if (npc->checkActionFlag(TBaseNPC::NPC_ACTION_HAPPY)) {
		if (npc->isMonte()) {
			if (npc->isNormalMonteW() || npc->isSpecialMonteW()) {
				if (npc->isChild())
					unk264 = VERSION_SELECT(0x31, GMSJ01(0x2C));
				else
					unk264 = VERSION_SELECT(0x2C, GMSJ01(0x27));
			} else {
				if (npc->isChild())
					unk264 = VERSION_SELECT(0x2E, GMSJ01(0x29));
				else
					unk264 = VERSION_SELECT(0x28, GMSJ01(0x23));
			}
		} else {
			if (npc->isMare()) {
				if (npc->isNormalMareW() || npc->isSpecialMareW()) {
					if (npc->isChild())
						unk264 = VERSION_SELECT(0x32, GMSJ01(0x2D));
					else
						unk264 = VERSION_SELECT(0x2D, GMSJ01(0x28));
				} else {
					if (npc->isChild())
						unk264 = VERSION_SELECT(0x2F, GMSJ01(0x2A));
					else
						unk264 = VERSION_SELECT(0x29, GMSJ01(0x24));
				}
			} else if (npc->getActorType() == ACTOR_TYPE_NPC_KINOPIO) {
				unk264 = VERSION_SELECT(0x2A, GMSJ01(0x25));
			} else if (npc->getActorType() == ACTOR_TYPE_NPC_MARE_MB) {
				unk264 = VERSION_SELECT(0x30, GMSJ01(0x2B));
			}
		}

		for (int i = 0; i < ARRAY_COUNT(unk2E0); i++) {
			if (npc == unk2E0[i].unk0) {
				unk264 = unk2E0[i].unk4;
				break;
			}
		}
	} else {
		unk264 = param_1;
	}

	if (npc->getActorType() == ACTOR_TYPE_NPC_BOARD)
		unk28 = 1;
	else
		unk28 = 0;

	unk270 = param_2;
	unk278 = 0;
	unk214 = -1;
	unk26A = 0;
	unk27C = -1;
	unk280 = 0;
	unk26C = 0;
	unk26D = 0;
	unk254 = nullptr;
	unk29  = 0;

#if defined(VERSION_GMSJ01) || defined(VERSION_GMSP01_NX)
	if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)
	    == VERSION_SELECT(5, GMSJ01(0x100)))
		unk340 = 0x20;
	else
		unk340 = 0x40;
#else
	unk340 = VERSION_SELECT(0x40, GMSJ01_01(0x20));
#endif

	u32 msgID = unk264;
	TMessageLoader* loader;
	if ((msgID & 0xFFFF0000) == 0)
		loader = unk25C;
	else
		loader = unk258;

	if (loader->getMessageData() != nullptr) {
		JMSMesgEntry* entry = loader->getMessageEntry(msgID & 0xFFFF);
		if (entry == nullptr) {
			unk264 = 4;
			loader = unk25C;
			entry  = loader->getMessageEntry(unk264 & 0xFFFF);
		}
		setupTextBox(loader->getMessageData(), entry);
	} else {
		unk264 = 3;
		loader = unk25C;
		setupTextBox(loader->getMessageData(),
		             loader->getMessageEntry(unk264 & 0xFFFF));
	}

	unk260 = loader;
	unk2DC = 0;

	if (unk254 != nullptr) {
		s32 soundID = scTalkSoundList[(u8)unk254->unk8[VERSION_SELECT(
		    2, GMSJ01(0), GMSE01(0))]];
		if (soundID != -1 && SMSGetMSound()->gateCheck(soundID)) {
			if (soundID & 0x80000000)
				MSBgm::startBGM(soundID);
			else
				MSoundSESystem::MSoundSE::startSoundSystemSE(soundID, 0,
				                                             nullptr, 0);
		}
	}

	if (unk248 == STATE_UNK1) {
		unk248 = STATE_UNK3;
		unk90->setAlpha(0xFF);
	}

	unk252 = 1;
}

void TTalk2D2::forceCloseTalk()
{
	gpCamera->makeMtxForPrevTalk();

	if (unk28 != 0) {
		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_RACE_START, 0, nullptr, 0);
	} else {
		SMSGetMSound()->talkModeOut();
	}

	SMSGetMarDirector()->getConsole()->startAppearTelop(false);

	if (unk248 == STATE_UNK1)
		unk248 = STATE_UNK0;
	else
		unk248 = STATE_UNK6;
}

void TTalk2D2::closeTalkWindow()
{
	if (unk270 & 1) {
		if (unk264 == VERSION_SELECT(30, GMSJ01(25))) {
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_RACE_START, 0, nullptr,
			                                   0);
		} else if (unk28 != 0) {
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_2D_OUT, 0, nullptr, 0);
			SMSGetMSound()->talkModeOut();
		} else {
			SMSGetMSound()->talkModeOut();
		}
		gpCamera->makeMtxForPrevTalk();
		SMSGetMarDirector()->getConsole()->startAppearTelop(false);
		SMSRumbleMgr->finishPause();
		unk252 = 0;
	}
	unk248 = STATE_UNK6;
}

void TTalk2D2::openTalkWindow(TBaseNPC* param_1)
{
	if (param_1 != nullptr)
		gpCamera->makeMtxForTalk(param_1);

	if (unk28 != 0) {
		unk14->setPanePosition(60, JUTPoint(0, -800), JUTPoint(0, 80),
		                       JUTPoint(0, 80));
		unk14->update();
		unk1C->setAlpha(0);
		unk248 = STATE_UNK4;
	} else {
		unk248 = STATE_UNK2;
	}

	unk90->setAlpha(0xFF);

#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSE01)
	int selectX, selectY, selectRotation;
#endif
	switch (gpCamera->mMode) {
	case CAMERA_MODE_TALK_B:
		unk330 = 160;
		unk332 = 135;
		unk334 = 20;
#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSE01)
		selectX        = 190;
		selectY        = 210;
		selectRotation = 22;
#endif
		break;
	case CAMERA_MODE_TALK_A:
	default:
		unk330 = 388;
		unk332 = 115;
		unk334 = -18;
#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSE01)
		selectX        = 366;
		selectY        = 191;
		selectRotation = 342;
#endif
		break;
	}

	unk90->move(unk330, unk332);
	unk90->rotate(unk334);
#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSE01)
	unk204->move(selectX, selectY);
	unk204->rotate(selectRotation);
#endif
	unk250 = 1;

	SMSGetMarDirector()->getConsole()->startDisappearTelop();
	SMSGetMarDirector()->getConsole()->startDisappearBalloon(
	    SMSGetMarDirector()->getConsole()->unk3E0, true);
	SMSGetMarDirector()->getConsole()->startDisappearMario();

	if (unk264 == VERSION_SELECT(30, GMSJ01(25))) {
		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_PROBLEM_SIGN, 0, nullptr,
		                                   0);
	} else if (unk28 != 0) {
		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_2D_IN, 0, nullptr, 0);
		SMSGetMSound()->talkModeIn(false);
	} else {
		SMSGetMSound()->talkModeIn(true);
	}

	SMSRumbleMgr->startPause();
}

#if defined(VERSION_GMSP01_NX)
class Offsetter {
public:
	Offsetter()
	{
		mX[0]  = 0.068858f;
		mX[1]  = 0.137716f;
		mX[2]  = 0.162964f;
		mX[3]  = 0.206574f;
		mX[4]  = 0.231822f;
		mX[5]  = 0.275432f;
		mX[6]  = 0.30068f;
		mX[7]  = 0.34429f;
		mX[8]  = 0.369538f;
		mX[9]  = 0.413148f;
		mX[10] = 0.438396f;
		mX[11] = 0.482006f;
		mX[12] = 0.507254f;
		mX[13] = 0.550864f;
		mX[14] = 0.576112f;
		mX[15] = 0.587971f;
		mX[16] = 0.619722f;
		mX[17] = 0.64497f;
		mX[18] = 0.68858f;
		mX[19] = 0.713828f;
		mX[20] = 0.757438f;
		mX[21] = 0.761646f;
		mX[22] = 0.782686f;
		mX[23] = 0.826296f;
		mX[24] = 0.851544f;
		mX[25] = 0.895154f;
		mX[26] = 0.920402f;
		mX[27] = 0.964012f;
		mX[28] = 1.03287f;
		mX[29] = 1.101728f;
		mY[0]  = 1;
		mY[1]  = -4;
		mY[2]  = -6;
		mY[3]  = -8;
		mY[4]  = -10;
		mY[5]  = -12;
		mY[6]  = -13;
		mY[7]  = -15;
		mY[8]  = -16;
		mY[9]  = -17;
		mY[10] = -17;
		mY[11] = -18;
		mY[12] = -19;
		mY[13] = -19;
		mY[14] = -19;
		mY[15] = -18;
		mY[16] = -18;
		mY[17] = -18;
		mY[18] = -17;
		mY[19] = -16;
		mY[20] = -16;
		mY[21] = -15;
		mY[22] = -14;
		mY[23] = -13;
		mY[24] = -12;
		mY[25] = -9;
		mY[26] = -8;
		mY[27] = -4;
		mY[28] = 1;
		mY[29] = 7;
	}

	int GetY(f32 x) const
	{
		int i;
		for (i = 0; i < 30; i++) {
			if (x < mX[i])
				break;
		}

		if (i == 0 || i == 29)
			return mY[i];

		f32 y = (f32)(mY[i] - mY[i - 1]) / (mX[i] - mX[i - 1]) * (x - mX[i - 1])
		        + mY[i - 1];
		return y > 0.0f ? (int)(y + 0.5f) : (int)(y - 0.5f);
	}

	/* 0x00 */ f32 mX[30];
	/* 0x78 */ s16 mY[30];
};
#endif

void TTalk2D2::makeBoxLine(s8 param_1, char* param_2)
{
	f32 w       = 0.0f;
	int srcIdx  = 0;
	int charIdx = 0;

	JUTPoint p0(unk48[param_1]->getBounds().x1, unk48[param_1]->getBounds().y1);
	JUTPoint p1(0, unk54[param_1]->getBounds().y1 - unk220);
	JUTPoint p2(unk60[param_1]->getBounds().x2 - 10,
	            unk60[param_1]->getBounds().y1);

	f32 posX, posY;
	makeLine(&posX, &posY, w, p0, p1, p2);

	do {
		int idx = charIdx + param_1 * 30;
		if (param_2 != nullptr) {
			char* dst = unk9C[idx]->getStringPtr();
			dst[0]    = param_2[srcIdx];
			if ((u8)param_2[srcIdx] >= 0x80) {
				dst[1] = param_2[srcIdx + 1];
				srcIdx += 2;
			} else {
				dst[1] = 0;
				srcIdx += 1;
			}
		}

#if defined(VERSION_GMSP01)
		u16 code = *unk9C[idx]->getStringPtr();
#else
		char* str = unk9C[idx]->getStringPtr();
		u16 code  = str[0];
#if defined(VERSION_GMSP01_NX)
		if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE) == 5
		    && code >= 0x80)
#else
		if (code >= 0x80)
#endif
			code |= str[0];
#endif
		if (code == 0)
			break;

		JUTFont::TWidth width;
		gpSystemFont->getWidthEntry(
		    VERSION_SELECT((u8)code, GMSJ01(code), GMSE01(code)), &width);

#if defined(VERSION_GMSP01)
		int yOffset   = 0;
		bool noResize = false;
		switch ((u8)*unk9C[idx]->getStringPtr()) {
		case '#':
		case '%':
		case '@':
			yOffset = 2;
			break;
		case 0xA5:
			w += 0.03f;
		case '+':
			yOffset = 2;
			break;
		case '<':
		case '>':
			yOffset = 1;
			break;
		case '$':
			w += 0.04f;
			noResize = true;
			yOffset  = 1;
			break;
		case '*':
			w += 0.01f;
			yOffset = 0;
			break;
		}
#endif

#if defined(VERSION_GMSJ01) || defined(VERSION_GMSP01_NX)
		if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)
		    == VERSION_SELECT(5, GMSJ01(0x100)))
			w += unk94 * (f32)width.field_0x1;
		else
			w += unk94 * (0.7f * (f32)width.field_0x1 + 4.0f);
#elif defined(VERSION_GMSJ01_01)
		w += unk94 * (f32)width.field_0x1;
#else
		w += unk94 * (0.7f * (f32)width.field_0x1 + 4.0f);
#endif

		f32 curX, curY;
		makeLine(&curX, &curY, w, p0, p1, p2);
#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSE01)
		if (charIdx < 29
		    && *unk9C[charIdx + param_1 * 30 + 1]->getStringPtr() == 'm')
			w += 0.01f;
#endif
		f32 ang = fabsf(atan2f(curY - posY, curX - posX));
		if (curX > 0.0f)
			ang *= -1.0f;

		JGeometry::TVec2<f32> sum(posX + curX, posY + curY);
		JGeometry::TVec2<f32> pos = sum * -0.5f;
		f32 cosA                  = cosf(ang);
		f32 sinA                  = sinf(ang);
		JGeometry::TVec2<f32> rotated(posX * cosA + posY * -sinA,
		                              posX * sinA + posY * cosA);
		pos += rotated;
		pos += sum * 0.5f;
		int ix = (int)(pos.x + (pos.x > 0.0f ? 0.5f : -0.5f));
		int iy = (int)(pos.y + (pos.y > 0.0f ? 0.5f : -0.5f));
#if defined(VERSION_GMSP01)
		if (!noResize)
			unk9C[idx]->setFontSize(20, 22);
		unk9C[idx]->move((s16)ix, -0x26 - (s16)iy + yOffset);
#elif defined(VERSION_GMSP01_NX)
		if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE) == 5) {
			unk9C[idx]->move((s16)ix, -0x25 - (s16)iy);
		} else {
			static Offsetter offset;
			unk9C[idx]->move((s16)ix, (s16)(Helpers::IsSpecialChar(
			                                    *unk9C[idx]->getStringPtr())
			                                    ? offset.GetY(w)
			                                    : -0x26 - iy));
		}
#else
		unk9C[idx]->move((s16)ix,
		                 VERSION_SELECT(-0x25, GMSE01(-0x28)) - (s16)iy);
#endif
		unk9C[idx]->setBasePosition(J2DBasePosition_4);
		unk9C[idx]->rotate(180.0f * ang / 3.1415927f);
		unk30[param_1]->appendChild(unk9C[idx]);
		if (w > VERSION_SELECT(1.05f, GMSJ01(1.1f))) {
			if (unk228[param_1] > charIdx)
				unk228[param_1] = charIdx;
			break;
		}
		charIdx++;
		makeLine(&posX, &posY, w, p0, p1, p2);
	} while (charIdx < 30);
}

bool TTalk2D2::openBoardWindow()
{
	bool result = false;
	switch (unk29) {
	case 0:
		if (unk14->update()) {
			unk14->setPanePosition(0x19, JUTPoint(0, 0x50), JUTPoint(0, 0x50),
			                       JUTPoint(0, 0));
			unk29 += 1;
		}
		break;
	case 1:
		if (unk14->update()) {
			unk1C->setAlpha(0);
			result = true;
			unk1C->show();
			unk29 += 1;
		}
		break;
	}
	return result;
}

bool TTalk2D2::openNormalWindow()
{
	bool result = false;

	if (unk2DE > 2 && unk24C->checkMeaning(TMarioGamePad::MEANING_SELECT_A)) {
		unk26C = 1;
#if defined(VERSION_GMSJ01) || defined(VERSION_GMSP01_NX)
		if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)
		    == VERSION_SELECT(5, GMSJ01(0x100)))
			unk340 = 0x5A;
		else
			unk340 = 0x80;
#else
		unk340 = VERSION_SELECT(0x80, GMSJ01_01(0x5A));
#endif
	}

	bool finished = true;
	for (int i = 0; i <= unk274; i++) {
		if (unk234[i] > -0.109f) {
			finished = false;
			unk234[i] -= unk338;
		}
		if (unk234[i] < 1.0f && i != 0
		    && !unk9C[unk228[i - 1] + (i - 1) * 30]->isVisible())
			unk234[i] = 1.0f;

		if (unk224[i] == 0 && unk234[i] < unk33C) {
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_TALK_OBI, 0, nullptr,
			                                   0);
			unk224[i] = 1;
			unk9C[i * 30]->show();
		}
	}

	if (finished) {
		for (int i = 0; i <= unk274; i++)
			unk3C[i]->show();
		result = true;
	}

	for (int line = 0; line < ARRAY_COUNT(unk224); line++) {
		if (unk224[line] == 0)
			continue;

		int idx = unk224[line] + line * 30;
		while (unk224[line] <= unk228[line]) {
			if (unk9C[idx]->isVisible()) {
				s16 alpha = unk9C[idx]->getAlpha();
				alpha += unk340;
				unk9C[idx]->setAlpha(alpha > 255 ? 255 : alpha);
				unk2DE = idx;
				if (alpha < 255)
					break;
				unk224[line]++;
				idx++;
			} else {
				if (unk2DC <= 0) {
					if (unk26C != 0)
						unk2DC = 0;
					else
						unk2DC = unk281[idx];
					unk9C[idx]->show();
					unk9C[idx]->setAlpha(0);
				} else {
					unk2DC--;
				}
				break;
			}
		}
	}

	return result;
}

void TTalk2D2::moveBoardWindow()
{
	int alpha = unk1C->getAlpha();

	if (alpha < 0xFF) {
		alpha += 4;
		if (alpha > 0xFF) {
			unk24->setAlpha(0);
			unk20->setAlpha(0);
			unk26B = 1;
			alpha  = 0xFF;
		}
		unk1C->setAlpha(alpha);
		return;
	}

	J2DPane* pane;
	if (unk26A != 0)
		pane = unk24;
	else
		pane = unk20;
	alpha = pane->getAlpha();
	if (unk26B != 0) {
		alpha += 4;
		if (alpha > 0xFF) {
			unk26B = 0;
			alpha  = 0xFF;
		}
	} else {
		alpha -= 4;
		if (alpha < 0) {
			unk26B = 1;
			alpha  = 0;
		}
	}
	pane->setAlpha(alpha);
}

void TTalk2D2::checkBoardControler()
{
	if (unk26A != 0) {
		if (unk26D != 0
		    || unk24C->checkFrameMeaning(TMarioGamePad::MEANING_SELECT_A)
		    || unk24C->checkFrameMeaning(TMarioGamePad::MEANING_SELECT_B)) {
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0,
			                                   nullptr, 0);

			unk14->setPanePosition(60, JUTPoint(0, 0), JUTPoint(0, 0),
			                       JUTPoint(0, -600));
			if (unk270 & 1) {
				if (unk264 == VERSION_SELECT(30, GMSJ01(25))) {
					SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_RACE_START, 0,
					                                   nullptr, 0);
				} else if (unk28 != 0) {
					SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_2D_OUT, 0,
					                                   nullptr, 0);
					SMSGetMSound()->talkModeOut();
				} else {
					SMSGetMSound()->talkModeOut();
				}
				gpCamera->makeMtxForPrevTalk();
				SMSGetMarDirector()->getConsole()->startAppearTelop(false);
				SMSRumbleMgr->finishPause();
				unk252 = 0;
			}
			unk248 = STATE_UNK6;
		}
	} else if (unk24C->checkFrameMeaning(TMarioGamePad::MEANING_SELECT_A
	                                     | TMarioGamePad::MEANING_SELECT_B)) {
		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0, nullptr,
		                                   0);
		unk248 = STATE_UNK7;
	}
}

void TTalk2D2::moveTalkWindow()
{
	for (int line = 0; line < ARRAY_COUNT(unk224); line++) {
		int idx = unk224[line] + line * 30;
		if (unk224[line] != 0 && unk224[line] <= unk228[line]) {
			if (unk9C[idx]->isVisible()) {
				s16 alpha = unk9C[idx]->getAlpha();
				alpha += unk340;
				unk9C[idx]->setAlpha(alpha > 255 ? 255 : alpha);
				if (alpha >= 255)
					unk224[line]++;
			} else if (unk2DC <= 0) {
				unk2DC = unk281[idx];
				unk9C[idx]->show();
				unk9C[idx]->setAlpha(0);
			} else {
				unk2DC--;
			}
		}
	}

	if (unk224[unk274] < 30 && unk224[unk274] <= unk228[unk274])
		return;

	J2DPane* pane;
	J2DPane* fade;
	if (unk214 == -1) {
		pane = unk6C[unk274];
		if (unk26A != 0) {
			fade = unk84[unk274];
			unk78[unk274]->hide();
			unk84[unk274]->show();
		} else {
			fade = unk78[unk274];
			unk78[unk274]->show();
			unk84[unk274]->hide();
		}
	} else {
		pane = unk204;
		fade = unk20C[unk214];
	}

	if (pane->isVisible()) {
		s16 alpha = pane->getAlpha();
		if (alpha < 255) {
			alpha += 0x10;
			if (alpha >= 255)
				alpha = 255;
			pane->setAlpha(alpha);
		}

		s16 fadeAlpha = fade->getAlpha();
		if (unk26B != 0) {
			fadeAlpha += 2;
			if (fadeAlpha > 255) {
				unk26B    = 0;
				fadeAlpha = 255;
			}
		} else {
			fadeAlpha -= 4;
			if (fadeAlpha < 60) {
				unk26B    = 1;
				fadeAlpha = 60;
			}
		}
		fade->setAlpha(fadeAlpha);

		u8 color = fadeAlpha;
		if (unk214 == 1) {
			snprintf(unk208->getStringPtr(), 0x5E,
			         "\x1B"
			         "CC[ffffff60]\x1BGC[ffffff60]%s\x1B"
			         "CC[ffffff%02x]\x1BGC[ffffff%02x]\n%s",
			         unk218[0], color, color, unk218[1]);
		} else if (unk214 == 0) {
			snprintf(unk208->getStringPtr(), 0x5E,
			         "\x1B"
			         "CC[ffffff%02x]\x1BGC[ffffff%02x]%s\n\x1B"
			         "CC[ffffff60]\x1BGC[ffffff60]%s",
			         color, color, unk218[0], unk218[1]);
		}
	} else {
		pane->show();
		pane->setAlpha(0);
		fade->setAlpha(255);
	}
}

void TTalk2D2::checkControler()
{
	if (unk6C[unk274]->isVisible()) {
		if (unk26A != 0) {
			if (unk26D != 0
			    || unk24C->checkFrameMeaning(TMarioGamePad::MEANING_SELECT_A)
			    || unk24C->checkFrameMeaning(TMarioGamePad::MEANING_SELECT_B)) {
				SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0,
				                                   nullptr, 0);
				closeTalkWindow();
			}
		} else if (unk24C->checkFrameMeaning(
		               TMarioGamePad::MEANING_SELECT_A
		               | TMarioGamePad::MEANING_SELECT_B)) {
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0,
			                                   nullptr, 0);
			unk248 = STATE_UNK7;
		}
	} else if (unk204->isVisible()) {
		if (unk24C->checkFrameMeaning(TMarioGamePad::MEANING_SELECT_UP)
		    && unk214 == 1) {
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_E3_MENU_CURSOR, 0,
			                                   nullptr, 0);
			unk214 = 0;
			unk20C[1]->setAlpha(0xFE);
			unk20C[1]->hide();
			unk20C[0]->show();
		} else if (unk24C->checkFrameMeaning(TMarioGamePad::MEANING_SELECT_DOWN)
		           && unk214 == 0) {
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_E3_MENU_CURSOR, 0,
			                                   nullptr, 0);
			unk214 = 1;
			unk20C[0]->setAlpha(0xFE);
			unk20C[0]->hide();
			unk20C[1]->show();
		} else if (unk26A != 0) {
			if (unk24C->checkFrameMeaning(TMarioGamePad::MEANING_SELECT_A
			                              | TMarioGamePad::MEANING_SELECT_B)) {
				if (unk270 & 1)
					gpCamera->makeMtxForPrevTalk();
				SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0,
				                                   nullptr, 0);
				unk248 = STATE_UNK6;
			}
		} else if (unk24C->checkFrameMeaning(
		               TMarioGamePad::MEANING_SELECT_A
		               | TMarioGamePad::MEANING_SELECT_B)) {
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0,
			                                   nullptr, 0);
			unk248 = STATE_UNK7;
		}
	}
}

bool TTalk2D2::closeNormalWindow()
{
	s16 alpha = unk90->getAlpha();
	alpha -= 0x10;
	bool result = false;
	if (alpha < 0) {
		alpha = 0;
		for (int i = 0; i < ARRAY_COUNT(unk234); ++i) {
			unk234[i] = i + 1.0f;
			unk3C[i]->hide();
			unk224[i] = 0;
			unk6C[i]->hide();
		}
		for (int i = 0; i < ARRAY_COUNT(unk9C); ++i) {
			if (unk9C[i] != nullptr)
				unk9C[i]->hide();
		}
		if (unk204->isVisible())
			unk204->hide();
		result = true;
	}
	unk90->setAlpha(alpha);
	if (unk204->isVisible())
		unk204->setAlpha(alpha);
	return result;
}

bool TTalk2D2::closeBoardWindow()
{
	bool result = false;
	if (unk14->update())
		result = true;
	return result;
}

bool TTalk2D2::eraseNormalWindow()
{
	s16 alpha = unk90->getAlpha();
	alpha -= 0x10;
	bool result = false;
	if (alpha < 0) {
		alpha = 0xFF;
		for (int i = 0; i < ARRAY_COUNT(unk234); ++i) {
			unk234[i] = i + 1.0f;
			unk3C[i]->hide();
			unk224[i] = 0;
			unk6C[i]->hide();
		}
		for (int i = 0; i < ARRAY_COUNT(unk9C); ++i) {
			if (unk9C[i] != nullptr)
				unk9C[i]->hide();
		}
		setupTextBox(unk260->getMessageData(),
		             unk260->getMessageEntry(unk264 & 0xFFFF));
		unk26C = 0;
#if defined(VERSION_GMSJ01) || defined(VERSION_GMSP01_NX)
		if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)
		    == VERSION_SELECT(5, GMSJ01(0x100)))
			unk340 = 0x20;
		else
			unk340 = 0x40;
#else
		unk340 = VERSION_SELECT(0x40, GMSJ01_01(0x20));
#endif
		unk27C = -1;
		unk2DE = 0;
		unk2DC = 0;
		result = true;
	}
	unk90->setAlpha(alpha);
	return result;
}

bool TTalk2D2::eraseBoardWindow()
{
	s16 alpha = unk18->getAlpha();
	alpha -= 4;
	bool result = false;
	if (alpha < 0) {
		alpha = 0;
		setupTextBox(unk260->getMessageData(),
		             unk260->getMessageEntry(unk264 & 0xFFFF));
		unk27C = -1;
		unk2DE = 0;
		unk2DC = 0;
		result = true;
	}
	unk18->setAlpha(alpha);
	return result;
}

bool TTalk2D2::appearBoardBoxWindow()
{
	u16 alpha = unk18->getAlpha();
	alpha += 4;
	bool result = false;
	if (alpha > 255) {
		alpha  = 255;
		result = true;
	}
	unk18->setAlpha(alpha);
	return result;
}

void TTalk2D2::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & CUE_MOVE) {
		switch (SMSGetMarDirector()->unk124) {
		case 2:
			switch (unk248) {
			case STATE_UNK2:
				if (gpCamera->isThing()) {
					unk251 = 0x14;
					unk248 = STATE_UNK3;
				}
				break;
			case STATE_UNK4:
				if (unk28 != 0 ? openBoardWindow() : openNormalWindow())
					unk248 = STATE_UNK5;
				break;
			case STATE_UNK5:
				if (unk28 != 0) {
					moveBoardWindow();
					checkBoardControler();
				} else {
					moveTalkWindow();
					checkControler();
				}
				break;
			case STATE_UNK6: {
				bool closed;
				if (unk28 != 0)
					closed = closeBoardWindow();
				else
					closed = closeNormalWindow();
				if (closed) {
					if (unk270 & 1)
						unk248 = STATE_UNK0;
					else
						unk248 = STATE_UNK1;
				}
				break;
			}
			case STATE_UNK7:
				if (unk28 != 0 ? eraseBoardWindow() : eraseNormalWindow()) {
					if (unk28 != 0)
						unk248 = STATE_UNK8;
					else
						unk248 = STATE_UNK4;
				}
				break;
			case STATE_UNK8:
				if (appearBoardBoxWindow())
					unk248 = STATE_UNK5;
				break;
			}
			break;
		}
	}

	if (param_1 & CUE_CALC_ANIM) {
		switch (SMSGetMarDirector()->unk124) {
		case 2:
			switch (unk248) {
			case STATE_UNK2:
				break;
			case STATE_UNK3:
				unk251--;
				if ((s8)unk251 < 0) {
					unk2DE = 0;
					unk248 = STATE_UNK4;
				}
				break;
			case STATE_UNK7: {
				s16 alpha = unk90->getAlpha();
				alpha -= 0x10;
				if (alpha < 0) {
					alpha = 0xFF;
					for (int i = 0; i < ARRAY_COUNT(unk234); ++i) {
						unk234[i] = i + 1.0f;
						unk3C[i]->hide();
						unk224[i] = 0;
						unk6C[i]->hide();
					}
					for (int i = 0; i < ARRAY_COUNT(unk9C); ++i) {
						if (unk9C[i] != nullptr)
							unk9C[i]->hide();
					}
					setupTextBox(unk260->getMessageData(),
					             unk260->getMessageEntry(unk264 & 0xFFFF));
					unk26C = 0;
#if defined(VERSION_GMSJ01) || defined(VERSION_GMSP01_NX)
					if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)
					    == VERSION_SELECT(5, GMSJ01(0x100)))
						unk340 = 0x20;
					else
						unk340 = 0x40;
#else
					unk340 = VERSION_SELECT(0x40, GMSJ01_01(0x20));
#endif
					unk27C = -1;
					unk2DE = 0;
					unk2DC = 0;
					unk27C = -1;
					unk248 = STATE_UNK4;
				}
				unk90->setAlpha(alpha);
				break;
			}
			}
			break;
		}
	}

	if (param_1 & CUE_DRAW) {
		switch (SMSGetMarDirector()->unk124) {
		case 2: {
			ReInitializeGX();
			SMS_DrawInit();

			J2DOrthoGraph ortho(param_2->getViewport());
			ortho.setup2D();

			if (unk250 != 0) {
				unk3C[0]->show();
				unk3C[1]->show();
				unk3C[2]->show();
				J2DPane* pane = unk2C->search('ROOT');
				pane->setAlpha(0);
				unk2C->draw(0, 0, &ortho);
				pane->setAlpha(0xFF);
				ortho.setup2D();
				unk250 = 0;
				unk3C[0]->hide();
				unk3C[1]->hide();
				unk3C[2]->hide();
			}

			switch (unk248) {
			case STATE_UNK4:
				for (s8 i = 0; i <= unk274; i++)
					openWindow(i, unk234[i]);
			case STATE_UNK5:
			case STATE_UNK6:
			case STATE_UNK7:
				ortho.setup2D();
				if (unk28 != 0) {
					unk10->draw(0, 0, &ortho);
				} else {
					unk90->move(unk330, unk332);
					unk90->rotate(unk334);
					unk2C->draw(0, 0, &ortho);
				}
				break;
			}
			break;
		}
		}
	}
}

void TTalk2D2::makeLine(f32* param_1, f32* param_2, f32 param_3,
                        JUTPoint& param_4, JUTPoint& param_5, JUTPoint& param_6)
{
	f32 inv  = 1.0f - param_3;
	*param_1 = param_4.x * (inv * inv) + param_5.x * (2.0f * param_3 * inv)
	           + param_6.x * (param_3 * param_3);
	*param_2 = param_4.y * (inv * inv) + param_5.y * (2.0f * param_3 * inv)
	           + param_6.y * (param_3 * param_3);
}

void TTalk2D2::setupBoardTextBox(const void* param_1, JMSMesgEntry* param_2)
{
	JSUMemoryInputStream stream((const u8*)param_1 + param_2->unk0 + unk278,
	                            0x400);
	JSUMemoryOutputStream out(unk18->getStringPtr(), 0x200);

	unk254 = param_2;

	int lineIdx = 0;
	while (lineIdx < 6) {
		u8 code = stream.readU8();

		switch (code) {
		case 0x0A:
			out << u8(0x0A);
			lineIdx++;
			break;

		case 0x00:
			unk26A  = 1;
			lineIdx = 6;
			break;

		case 0x1A:
			break;

		default: {
			stream.skip(-1);

			u8 lead = stream.readU8();
			out << lead;

			if (lead >= 0x80)
				out << stream.readU8();
			break;
		}
		}
	}

	if (unk26A == 0) {
		s8 next;
		stream.peek(&next, 1);
		if (next == 0) {
			unk26A = 1;
			stream.skip(1);
			out << u8(0);
		}
	}
#if !defined(VERSION_GMSJ01)
	else {
		out << u8(0);
	}
#endif

	unk278 += stream.getPosition();
}

void TTalk2D2::setupTextBox(const void* param_1, JMSMesgEntry* param_2)
{
	if (unk28 != 0) {
		setupBoardTextBox(param_1, param_2);
		return;
	}

	JSUMemoryInputStream stream((const u8*)param_1 + param_2->unk0 + unk278,
	                            0x400);

	unk254 = param_2;

	int charIdx = 0;
	int lineIdx = 0;
	unk274      = 0;
	unk2DE      = 0;

	while (lineIdx < 3) {
		char* dest = unk9C[charIdx + lineIdx * 30]->getStringPtr();
#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSE01)
		dest[0] = 0;
#endif

		s8 code;
		stream >> code;

		switch (code) {
		case 0x0A:
			unk228[lineIdx] = charIdx == 0 ? 0 : charIdx - 1;
			charIdx         = 0;
			makeBoxLine(lineIdx, nullptr);
			lineIdx++;
			break;

		case 0x00:
			if (charIdx != 0) {
				unk228[lineIdx] = charIdx - 1;
				charIdx         = 0;
				makeBoxLine(lineIdx, nullptr);
			}
			lineIdx = 3;
			unk26A  = 1;
			break;

		case 0x1A:
			setTagParam(stream, *unk9C[charIdx + lineIdx * 30], &charIdx,
			            &lineIdx);
			break;

		default: {
			if (unk274 != lineIdx)
				unk274 = lineIdx;

			stream.skip(-1);
			dest[0] = stream.readU8();
#if defined(VERSION_GMSJ01) || defined(VERSION_GMSE01)
			if ((u8)dest[0] >= 0x80) {
				dest[1] = stream.readU8();
			} else {
				dest[1] = 0;
			}

			int idx = charIdx + lineIdx * 30;
			unk9C[idx]->setFontColor(unk27C, unk27C);
			unk9C[idx]->setBlackWhite(unk27C & 0xFFFFFF00, unk27C);
			unk281[unk2DE] = unk280;
			unk2DE         = idx;
#else
			JUtility::TColor color;
			int idx = charIdx + lineIdx * 30;
#if defined(VERSION_GMSP01)
			bool special = true;
			switch ((u8)dest[0]) {
			case '@':
				color.set(100, 255, 100, 255);
				break;
			case '#':
				color.set(255, 160, 100, 255);
				break;
			case '%':
				color.set(255, 255, 0, 255);
				break;
			case '+':
			case '<':
			case '>':
			case 0xA5:
				color.set(220, 220, 220, 255);
				break;
			case '$':
				unk9C[idx]->setFontSize(30, 30);
				color.set(200, 180, 255, 255);
				break;
			case '*':
				color.set(220, 220, 220, 255);
				break;
			default:
				special = false;
				break;
			}
			dest[1] = 0;

			if (special) {
				unk9C[idx]->setFontColor(color, color);
				unk9C[idx]->setBlackWhite(color & 0xFFFFFF00, color);
			} else {
				unk9C[idx]->setFontColor(unk27C, unk27C);
				unk9C[idx]->setBlackWhite(unk27C & 0xFFFFFF00, unk27C);
				unk9C[idx]->setFontSize(20, 24);
			}
#else
#if defined(VERSION_GMSP01_NX)
			if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE) == 5) {
#endif
				if ((u8)dest[0] >= 0x80) {
					dest[1] = stream.readU8();
				} else {
					dest[1] = 0;
				}
#if defined(VERSION_GMSP01_NX)
			} else {
				bool special = Helpers::IsSpecialChar(dest[0]);
				bool round   = Helpers::IsSpecialRound(dest[0]);
				dest[1]      = 0;

				unk9C[idx]->setFontColor(unk27C, unk27C);
				unk9C[idx]->setBlackWhite(unk27C & 0xFFFFFF00, unk27C);
				if (special) {
					if (round)
						unk9C[idx]->setFontSize(21, 22);
					else
						unk9C[idx]->setFontSize(20, 24);
				} else {
					unk9C[idx]->setFontSize(20, 24);
				}
			}
#endif
#endif
			unk281[unk2DE] = unk280;
			unk2DE         = idx;
#endif
			charIdx++;
			break;
		}
		}
	}

	if (unk26A == 0) {
		s8 next;
		stream.peek(&next, 1);
		if (next == 0)
			unk26A = 1;
	}

	if (unk214 != -1) {
		if (unk214 == 0)
			snprintf(unk208->getStringPtr(), 0x5E,
			         "%s\n\x1B"
			         "CC[7f7f7f]\x1B"
			         "GC[7f7f7f]%s",
			         unk218[0], unk218[1]);
		else
			snprintf(unk208->getStringPtr(), 0x5E,
			         "\x1B"
			         "CC[7f7f7f]\x1B"
			         "GC[7f7f7f]%s\x1B"
			         "CC[ffffff]\x1B"
			         "GC[ffffff]\n%s",
			         unk218[0], unk218[1]);
	}

	unk278 += stream.getPosition();
}

void TTalk2D2::setTagParam(JSUMemoryInputStream& param_1, J2DTextBox& param_2,
                           int* param_3, int* param_4)
{
	u8 size  = param_1.readU8();
	u8 group = param_1.readU8();
	u16 tag  = param_1.readU16();

	switch (group) {
	case 0:
		switch (tag) {
		case 0:
			unk280 = param_1.readU8();
			return;
		case 1:
			unk26D = 1;
			return;
		default:
			param_1.skip(size - 5);
			return;
		}

	case 1:
		switch (tag) {
		case 0: {
			if (unk214 == -1) {
				unk214 = 0;
				unk20C[1]->setAlpha(0xFE);
				unk20C[1]->hide();
				unk20C[0]->show();
			}
			int len = size - 4 < 17 ? size - 4 : 17;
			snprintf(unk218[0], len, "%s", (const char*)param_1.getCurrent());
			param_1.skip(size - 5);
			return;
		}
		case 1: {
			if (unk214 == -1) {
				unk214 = 1;
				unk20C[0]->setAlpha(0xFE);
				unk20C[0]->hide();
				unk20C[1]->show();
			}
			int len = size - 4 < 17 ? size - 4 : 17;
			snprintf(unk218[1], len, "%s", (const char*)param_1.getCurrent());
			param_1.skip(size - 5);
			return;
		}
		default:
			param_1.skip(size - 5);
			return;
		}

	case 2:
		switch (tag) {
		case 0:
		case 1:
		case 6: {
			int time;
			if (tag == 0)
				time = TFlagManager::getInstance()->getFlag(
				    MSF_RACE_RECORD_PIANTA);
			else if (tag == 1)
				time = TFlagManager::getInstance()->getFlag(
				    MSF_RACE_RECORD_GELATO);
			else if (tag == 6)
				time = TFlagManager::getInstance()->getFlag(
				    MSF_RACE_RECORD_NOKI);

			if (time > 599999)
				time = 599999;
			if (time < 0)
				time = 0;

			u16 minutes   = (time - time % 100) / 6000;
			int rest      = time - minutes * 6000;
			u16 seconds   = 0.01 * rest;
			u16 hundredth = rest - seconds * 100;

			snprintf(unk9C[*param_3 + *param_4 * 30]->getStringPtr(), 2, "%d",
			         minutes / 10);
			snprintf(unk9C[*param_3 + *param_4 * 30 + 1]->getStringPtr(), 2,
			         "%d", minutes % 10);
			snprintf(unk9C[*param_3 + *param_4 * 30 + 2]->getStringPtr(), 2,
			         ":");
			snprintf(unk9C[*param_3 + *param_4 * 30 + 3]->getStringPtr(), 2,
			         "%d", seconds / 10);
			snprintf(unk9C[*param_3 + *param_4 * 30 + 4]->getStringPtr(), 2,
			         "%d", seconds % 10);
			snprintf(unk9C[*param_3 + *param_4 * 30 + 5]->getStringPtr(), 2,
			         ":");
			snprintf(unk9C[*param_3 + *param_4 * 30 + 6]->getStringPtr(), 2,
			         "%d", hundredth / 10);
			snprintf(unk9C[*param_3 + *param_4 * 30 + 7]->getStringPtr(), 2,
			         "%d", hundredth % 10);

			for (int i = 0; i < 8; i++) {
				unk9C[*param_3 + *param_4 * 30 + i]->setFontColor(unk27C,
				                                                  unk27C);
				unk9C[*param_3 + *param_4 * 30 + i]->setBlackWhite(
				    unk27C & 0xFFFFFF00, unk27C);
				unk281[i + *param_3 + *param_4 * 30] = unk280;
			}
			*param_3 += 8;
			return;
		}

		case 2: {
			int count
			    = 0.01f
			      * (TFlagManager::getInstance()->getFlag(MSF_BOX_GAME_RECORD)
			         + 99);
			if (count < 10) {
				snprintf(unk9C[*param_3 + *param_4 * 30]->getStringPtr(), 2,
				         "%d", count);
				*param_3 += 1;
				return;
			}
			snprintf(unk9C[*param_3 + *param_4 * 30]->getStringPtr(), 2, "%d",
			         count / 10);
			snprintf(unk9C[*param_3 + *param_4 * 30 + 1]->getStringPtr(), 2,
			         "%d", count % 10);
			*param_3 += 2;
			return;
		}

		case 3: {
			int value
			    = TFlagManager::getInstance()->getFlag(MSF_BLUE_COIN_COUNT);
			int collected = 0;
			for (int i = 0x46; i < 0x56; i++) {
				if (TFlagManager::getInstance()->getFlag(MSF_SHINE_BASE + i)
				    != 0)
					collected++;
			}
			for (int i = 0x6C; i <= 0x73; i++) {
				if (TFlagManager::getInstance()->getFlag(MSF_SHINE_BASE + i)
				    != 0)
					collected++;
			}
			value -= collected * 10;

			if (value < 100) {
				snprintf(unk9C[*param_3 + *param_4 * 30]->getStringPtr(), 2,
				         "%d", value / 10);
				*param_3 += 1;
				return;
			}
			snprintf(unk9C[*param_3 + *param_4 * 30]->getStringPtr(), 2, "%d",
			         value / 100);
			value -= value / 100 * 100;
			snprintf(unk9C[*param_3 + *param_4 * 30 + 1]->getStringPtr(), 2,
			         "%d", value / 10);
			*param_3 += 2;
			return;
		}

		case 4: {
			int max;
			int kind;
			TFruitBasketEvent* basket;
			switch (param_1.readU8()) {
			case 0:
				basket = (TFruitBasketEvent*)JDrama::TNameRefGen::search(
				    "フルーツかごＡ");
				max  = 3;
				kind = 0;
				break;
			case 1:
				basket = (TFruitBasketEvent*)JDrama::TNameRefGen::search(
				    "フルーツかごＢ");
				max  = 3;
				kind = 4;
				break;
			case 2:
				basket = (TFruitBasketEvent*)JDrama::TNameRefGen::search(
				    "フルーツかごＣ");
				max  = 3;
				kind = 3;
				break;
			case 3:
				basket = (TFruitBasketEvent*)JDrama::TNameRefGen::search(
				    "フルーツかごＤ");
				kind = 1;
				max  = 3;
				break;
			}

			if (basket != nullptr) {
				max -= basket->getFruitNum(kind);
				if (max < 0 || max > 9)
					max = 0;
				snprintf(unk9C[*param_3 + *param_4 * 30]->getStringPtr(), 2,
				         "%d", max);
				snprintf(unk9C[*param_3 + *param_4 * 30 + 1]->getStringPtr(), 2,
				         " ");
				*param_3 += 2;
			}
			return;
		}
		}
		break;

	case 0xFF:
		switch (tag) {
		case 0:
			unk27C = cColorTable[param_1.readU8()];
			break;
		}
		break;

	default:
		param_1.skip(size - 5);
		break;
	}
}

void TTalk2D2::openWindow(s8 param_1, f32 param_2)
{
	Mtx mtxA;
	Mtx mtxB;
	f32 x = unk90->getGlbBounds().x1 + 5;
	f32 y = unk90->getGlbBounds().y1 + 5;

	MTXTrans(mtxA, -x, -y, 0.0f);
#if defined(VERSION_GMSP01_NX)
	if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE) == 5)
		MTXRotRad(mtxB, 'Z', DEG_TO_RAD(-unk90->getRotate()));
	else
		MTXRotRad(mtxB, 'Z', DEG_TO_RAD(-unk90->getRotate() - 1.0f));
#elif defined(VERSION_GMSJ01) || defined(VERSION_GMSJ01_01)
	MTXRotRad(mtxB, 'Z', DEG_TO_RAD(-unk90->getRotate()));
#else
	MTXRotRad(mtxB, 'Z', DEG_TO_RAD(-unk90->getRotate() - 1.0f));
#endif
	MTXConcat(mtxB, mtxA, mtxA);
	MTXTrans(mtxB, x, y, 0.0f);
	MTXConcat(mtxB, mtxA, mtxA);
	GXLoadPosMtxImm(mtxA, GX_PNMTX0);

	GXSetCullMode(GX_CULL_BACK);
	GXSetNumTexGens(2);
	GXSetNumTevStages(2);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S8, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_TRUE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanAmbColor(GX_COLOR0A0, (GXColor) { 0xFF, 0xFF, 0xFF, 0xFF });

	JUTTexture* tex = ((J2DPicture*)unk3C[param_1])->getTexture(0);
	tex->load(GX_TEXMAP1);
	unk244->load(GX_TEXMAP0);

	GXSetTevColor(GX_TEVREG0, JUtility::TColor(0x0000ff00));
	GXSetTevColor(GX_TEVREG1, JUtility::TColor(0x0000ffa0));
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_A1, GX_CA_TEXA, GX_CA_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);

	MTXTrans(mtxA, param_2, 0.0f, 0.0f);
	GXLoadTexMtxImm(mtxA, GX_TEXMTX0, GX_MTX2x4);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0,
	                  GX_FALSE, GX_PTIDENTITY);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA,
	                GX_CA_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);

	MTXIdentity(mtxA);
	GXLoadTexMtxImm(mtxA, GX_TEXMTX1, GX_MTX2x4);
	GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1,
	                  GX_FALSE, GX_PTIDENTITY);
	GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);

	JUTRect rect(unk3C[param_1]->getGlbBounds());

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition2f32(rect.x1, rect.y1);
	GXTexCoord2s8(0, 0);
	GXPosition2f32(rect.x2, rect.y1);
	GXTexCoord2s8(1, 0);
	GXPosition2f32(rect.x2, rect.y2);
	GXTexCoord2s8(1, 1);
	GXPosition2f32(rect.x1, rect.y2);
	GXTexCoord2s8(0, 1);
	GXEnd();
}
