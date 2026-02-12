#include <System/MenuDir.hpp>
#include <stdio.h>
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JDrama/JDRScreen.hpp>
#include <JSystem/JDrama/JDRDStageGroup.hpp>
#include <JSystem/JDrama/JDRDStage.hpp>
#include <JSystem/JDrama/JDREfbCtrl.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <System/Resolution.hpp>
#include <System/MarioGamePad.hpp>
#include <System/Application.hpp>
#include <System/FlagManager.hpp>
#include <System/MovieDirector.hpp>
#include <MSound/MSound.hpp>
#include <GC2D/ScrnFader.hpp>
#include <GC2D/Menu.hpp>
#include <GC2D/MessageUtil.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TMenuDirector::TMenuDirector()
    : unk18(0)
    , unk1C(nullptr)
    , unk28(0)
{
	f32 sync = SMSGetVSyncTimesPerSec();
	unk30    = 0;
	unk34    = sync;
	unk3C    = nullptr;
	unk40    = nullptr;
	unk44    = nullptr;
	unk48    = 0;
	unk4C    = 0;
	unk50    = false;
}

TMenuDirector::~TMenuDirector()
{
	unk2C->offFlag(0x1);
	JKRMemArchive* arc = (JKRMemArchive*)JKRFileLoader::getVolume("title");
	if (arc)
		arc->unmountFixed();
}

void* TMenuDirector::setupThreadFunc(void* param_1)
{
	// BUG: return missing
	((TMenuDirector*)param_1)->rsetup();
}

extern OSThread gSetupThread;
extern u8* gpSetupThreadStack;

void TMenuDirector::setup(JDrama::TDisplay* param_1, TMarioGamePad* param_2)
{
	unk14         = new JDrama::TDStageGroup(param_1);
	unk2C         = param_2;
	unk2C->mFlags = 1;
	OSCreateThread(&gSetupThread, &setupThreadFunc, this,
	               gpSetupThreadStack + 0x10000, 0x10000, 0x11, 0);
	OSResumeThread(&gSetupThread);
}

int TMenuDirector::rsetup()
{
	void* arcBlob      = SMSLoadArchive("/data/title.arc", nullptr, 0, nullptr);
	JKRMemArchive* arc = new JKRMemArchive;
	arc->mountFixed(arcBlob, MBF_0);

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* rootViewObjs
	    = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("root View Objs");
	unk10 = rootViewObjs;

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* group2d
	    = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("Group 2D");
	rootViewObjs->getChildren().push_back(group2d);

	unk3C = new J2DSetScreen("title.blo", arc);

	if (!unk3C)
		return 0;

	group2d->getChildren().push_back(new TMenuBase(unk3C));

	unk20 = unk3C->search('lisA');
	unk20->hide();
	J2DPane* statPane = unk3C->search('stat');
	if (!statPane)
		return 1;
	if (statPane->mInfoTag != 0x12)
		return 1;

	unk24 = new TFlashPane(statPane);
	group2d->getChildren().push_back(unk24);
	unk24->hide();

	unk40 = new TMenuPlane(unk2C, unk3C->search('lisB'), 33, 64);
	group2d->getChildren().push_back(unk40);

	unk40->hide();

	unk44 = new TMenuPlane(unk2C, unk3C->search('lisC'), 33, 64);
	group2d->getChildren().push_back(unk44);

	unk44->hide();

	unk38 = (J2DTextBox*)unk3C->search('stag');

	if (!unk38)
		return 1;

	unk38->hide();

	unk1C = JKRGetResource("/title/marisun_stage.bmg");
	if (unk1C) {
		for (int i = 0; i < 19; ++i) {
			const char* message = SMSGetMessageData(unk1C, i);
			char acStack_40[22];
			if (message)
				snprintf(acStack_40, 22, "%02d %s", i, message);
			else if (i == 17 || i == 18)
				snprintf(acStack_40, 22, "show movie %d", i == 17 ? 1 : 2);
			else
				snprintf(acStack_40, 22, "%02d No Data            ", i);

			if (i < 9) {
				((J2DTextBox*)unk3C->search('tx01' + i))->setString(acStack_40);
			} else {
				((J2DTextBox*)unk3C->search('tx10' + i - 9))
				    ->setString(acStack_40);
			}
		}
	}

	for (int i = 0; i < 20; ++i) {
		int code            = i < 9 ? 'st_1' + i : 'st_a' + i - 9;
		J2DTextBox* textBox = (J2DTextBox*)unk3C->search(code);
		SMSMakeTextBuffer(textBox, 22);
	}

	JDrama::TDStageDisp* stageDisp = new JDrama::TDStageDisp;
	unk14->getChildren().push_back(stageDisp);

	JDrama::TRect rect(0, 0, SMSGetTitleRenderWidth(),
	                   SMSGetTitleRenderHeight());
	stageDisp->getEfbCtrlDisp()->TEfbCtrl::setSrcRect(rect);

	JDrama::TOrthoProj* camera
	    = new JDrama::TOrthoProj(0.0f, 16.0f, 600.0f, 464.0f);
	group2d->getChildren().push_back(camera);

	JDrama::TScreen* screen = new JDrama::TScreen(rect, "Screen 2D");
	stageDisp->getUnk14()->getChildren().push_back(screen);
	screen->assignCamera(camera);
	screen->assignViewObj(group2d);

	unk40->show();

	return 0;
}

int TMenuDirector::direct()
{
	if (!unk50) {
		if (!OSIsThreadTerminated(&gSetupThread))
			return 0;
		void* res;
		OSJoinThread(&gSetupThread, &res);
		gpApplication.mFader->startFadeinT(0.25f);
		if (TFlagManager::getInstance()->getBool(0x30007)) {
			TFlagManager::getInstance()->setBool(true, 0x30007);
			gpMSound->loadWave(MS_WAVE_UNK128);
		}
		unk50 = true;
	}

	u32 uVar13 = TApplication::APP_STATE_DEFAULT;

	JDrama::TDirector::direct();

	switch (unk18) {
	case 0:
		if (unk40->checkFlag(0x1)) {
			if (!(unk2C->getButton() & JUTGamePad::X)) {
				TFlagManager::getInstance()->firstStart();
				for (u8 i = 0; i < 30; ++i)
					TFlagManager::getInstance()->setShineFlag(i);
				for (u32 i = 0x10366; i < 0x103B4; ++i)
					TFlagManager::getInstance()->setBool(true, i);
				TFlagManager::getInstance()->saveSuccess();
			}

			unk40->fade();

			unk38->setString(unk40->unk30[unk40->unk2C]->getStringPtr());

			if (unk40->unk2C == 6) {
				for (int i = 0; i < 10; ++i) {
					int code = i + 'st_1';
					if (i == 9)
						code = 'st_a';
					J2DTextBox* box = (J2DTextBox*)unk3C->search(code);
					if (i < 6)
						snprintf(box->getStringPtr(), 22, "ビーチ %d", i);
					else
						snprintf(box->getStringPtr(), 22, "ホテル %d", i - 6);
				}

				snprintf(((J2DTextBox*)unk3C->search('st_f'))->getStringPtr(),
				         22, "ビーチ 6");
				snprintf(((J2DTextBox*)unk3C->search('st_g'))->getStringPtr(),
				         22, "ビーチ 7");
				snprintf(((J2DTextBox*)unk3C->search('st_h'))->getStringPtr(),
				         22, "ホテル 4");
				snprintf(((J2DTextBox*)unk3C->search('st_i'))->getStringPtr(),
				         22, "カジノ 0");
				snprintf(((J2DTextBox*)unk3C->search('st_j'))->getStringPtr(),
				         22, "カジノ 1");
				snprintf(((J2DTextBox*)unk3C->search('st_k'))->getStringPtr(),
				         22, "ボス");
			} else if (unk40->unk2C == 5) {
				for (int i = 0; i < 10; ++i) {
					int code = i + 'st_1';
					if (i == 9)
						code = 'st_a';
					J2DTextBox* box = (J2DTextBox*)unk3C->search(code);
					if (i < 4)
						snprintf(box->getStringPtr(), 22, "ビーチサイド %d", i);
					else
						snprintf(box->getStringPtr(), 22, "ピンナパーコ %d",
						         i - 4);
				}

				snprintf(((J2DTextBox*)unk3C->search('st_b'))->getStringPtr(),
				         22, "ピンナパーコ 6");
				snprintf(((J2DTextBox*)unk3C->search('st_c'))->getStringPtr(),
				         22, "ピンナパーコ 7");
				snprintf(((J2DTextBox*)unk3C->search('st_d'))->getStringPtr(),
				         22, "ビーチサイド 4");
				snprintf(((J2DTextBox*)unk3C->search('st_h'))->getStringPtr(),
				         22, "ボス 0");
				snprintf(((J2DTextBox*)unk3C->search('st_i'))->getStringPtr(),
				         22, "ボス 1");
				snprintf(((J2DTextBox*)unk3C->search('st_j'))->getStringPtr(),
				         22, "デモ 0");
				snprintf(((J2DTextBox*)unk3C->search('st_k'))->getStringPtr(),
				         22, "デモ 1");
			} else if (unk40->unk2C == 8) {
				for (int i = 0; i < 10; ++i) {
					int code = i + 'st_1';
					if (i == 9)
						code = 'st_a';
					J2DTextBox* box = (J2DTextBox*)unk3C->search(code);
					if (i < 8)
						snprintf(box->getStringPtr(), 22, "マーレ %d", i);
					if (i == 8)
						snprintf(box->getStringPtr(), 22, "カイテイ");
					if (i == 9)
						snprintf(box->getStringPtr(), 22, "ボス");
				}
			} else if (unk40->unk2C == 0x11 || unk40->unk2C == 0x12) {
				for (int i = 0; i < 20; ++i) {
					int code;
					if (i < 9)
						code = i + 'st_1';
					else
						code = i - 9 + 'st_a';

					J2DTextBox* box = (J2DTextBox*)unk3C->search(code);

					int movie = i;

					if (unk40->unk2C == 0x12)
						movie += 20;

					const char* movieName
					    = TMovieDirector::getStreamMovieName(movie);
					if (movieName) {
						snprintf(box->getStringPtr(), 22, "%s", movieName);
						char* it = strrchr(box->getStringPtr(), '.');
						if (it)
							*it = '\0';
					} else {
						snprintf(box->getStringPtr(), 22, "%02d not found",
						         movie);
					}
				}
			} else {
				for (int i = 0; i < 20; ++i) {
					int code;
					if (i < 9)
						code = i + 'st_1';
					else
						code = i - 9 + 'st_a';

					J2DTextBox* box = (J2DTextBox*)unk3C->search(code);

					if (i < 10)
						snprintf(box->getStringPtr(), 22, "%02d シーン %d", i,
						         i);
					else
						snprintf(box->getStringPtr(), 22, "%02d EX %d", i,
						         i - 10);
				}
			}

			unk38->show();
			unk44->show();
			unk18 = 1;
		}
		break;

	case 1:
		if (unk44->checkFlag(0x1)) {
			setFixedStageValue();
			unk18 = 2;
			gpApplication.mFader->startFadeoutT(0.25f);
			TGameSequence nextArea;
			nextArea.set(unk48, unk4C, 0);
			gpApplication.setNextArea(nextArea);
		} else if (unk44->checkFlag(0x2)) {
			unk18 = 0;
			unk40->unfade();
			unk38->hide();
			unk44->hide();
		}
		break;

	case 2:
		if (gpApplication.mFader->isFullyFadedOut()
		    && gpMSound->checkWaveOnAram(MS_WAVE_UNK128)) {
			if (unk40->unk2C == 0x11 || unk40->unk2C == 0x12)
				uVar13 = TApplication::APP_STATE_MOVIE;
			else
				uVar13 = TApplication::APP_STATE_GAMEPLAY;
		}
		break;

	case 3:
		if (gpApplication.mFader->isFullyFadedOut())
			uVar13 = TApplication::APP_STATE_QUIT;
		break;
	}

	return uVar13;
}

void TMenuDirector::setFixedStageValue()
{
	unk48 = unk40->unk2C;
	unk4C = unk44->unk2C;
	int local_30[]
	    = { 0, 0x14, 0x1c, 0x1e, 0x20, 0x22, 0x28, 0, 0x2a, 0x2c, 0x2e };

	if ((unk48 == 0x11) || (unk48 == 0x12)) {
		int movie = unk4C;
		if (unk48 == 0x12)
			movie += 0x14;
		gpApplication.mMovie = movie;

		unk48 = 0xf;
		unk4C = 0;
	} else {
		if (unk48 >= 7)
			unk48 += 1;

		if (unk48 == 3) {
			if (unk4C == 0x13) {
				unk48 = 0x3b;
				unk4C = 0;
			} else if (unk4C >= 10) {
				unk48 = unk4C - 10 + local_30[unk48];
				unk4C = 0;
			}
		} else if (unk48 == 9) {
			if (unk4C >= 10) {
				unk48 = unk4C - 10 + local_30[unk48];
				unk4C = 0;
			} else if (unk4C == 8) {
				unk48 = 0x10;
				unk4C = 0;
			} else if (unk4C == 9) {
				unk48 = 0x39;
				unk4C = 0;
			}
		} else if (unk48 == 6) {
			if (unk4C >= 0x13) {
				unk48 = 0x38;
				unk4C = 0;
			} else if (unk4C >= 0x11) {
				unk48 = 0xe;
				unk4C = unk4C - 0x11;
			} else if (unk4C >= 0x10) {
				unk48 = 7;
				unk4C = 4;
			} else if (unk4C >= 0xe) {
				unk48 = 6;
				unk4C = unk4C - 8;
			} else if (unk4C >= 10) {
				unk48 = unk4C - 10 + local_30[unk48];
				unk4C = 0;
			} else if (unk4C >= 6) {
				unk48 = 7;
				unk4C = unk4C - 6;
			}
		} else if (unk48 == 5) {
			if (unk4C == 0x12) {
				unk48 = 0xd;
				unk4C = 9;
			} else if (unk4C == 0x13) {
				unk48 = 0xd;
				unk4C = 10;
			} else if (unk4C >= 4 && unk4C <= 0xb) {
				unk48 = 0xd;
				unk4C = unk4C - 4;
			} else if (unk4C == 0xc) {
				unk48 = 5;
				unk4C = 4;
			} else if (unk4C == 0x11) {
				unk48 = 0x3a;
				unk4C = 1;
			} else if (unk4C == 0x10) {
				unk48 = 0x3a;
				unk4C = 0;
			} else if (unk4C >= 10) {
				unk48 = unk4C - 10 + local_30[unk48];
				unk4C = 0;
			}
		} else if (unk48 == 10) {
			if (unk4C >= 10) {
				unk48 = unk4C - 10 + local_30[unk48];
				unk4C = 0;
			} else if (unk4C == 7) {
				unk48 = 0x3c;
				unk4C = 0;
			}
		} else if (unk48 == 2) {
			if (unk4C >= 10) {
				unk48 = unk4C - 10 + local_30[unk48];
				unk4C = 0;
			} else if (unk4C == 8) {
				unk48 = 0x37;
				unk4C = 0;
			}
		} else if (unk48 != 0) {
			if (unk4C >= 10) {
				unk48 = unk4C - 10 + local_30[unk48];
				unk4C = 0;
			}
		}
	}
}
