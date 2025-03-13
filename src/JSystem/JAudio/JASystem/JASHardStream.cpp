#include <JSystem/JAudio/JASystem/JASHardStream.hpp>
#include <dolphin/ai.h>
#include <dolphin/dvd.h>

namespace JASystem {

namespace HardStream {

	static bool useHardStreaming = false;
	static char* streamFiles     = 0;
	static TPlayList* playList   = 0;
	static TControl strCtrl;

	static char rootDir[32] = "";

	static s32 playListMax = 0x20;

	static void firstBgmCallback(s32, DVDFileInfo*);
	static void secondBgmCallback(s32, DVDFileInfo*);
	static void getAddrCallback(s32, DVDCommandBlock*);

	static void dummy(f32* f) { *f = 0.0f; }

	void init() { }

	void setMaxBgmPairs(u32) { }

	void setMaxFilenames(u32) { }

	void setRootDir(const char*) { }

	void setMasterVol(f32) { }

	void setFile(u32, const char*, f32) { }

	void setBgmPair(u32, u16, u16) { }

	void registBgmPair(u32, u32) { }

	void unregistBgmPair(u32) { }

	BOOL unregistBgmAll()
	{
		if (strCtrl.getList() == nullptr)
			return 0;

		strCtrl.setList(nullptr);
		for (int i = 0; i < playListMax; i++)
			playList[i].clear();

		return 1;
	}

	void playBgm(u32) { }

	void stopBgm(u32) { }

	bool checkPlaying() { return false; }

	void moveVolume(f32, u32) { }

	void main()
	{
		static DVDFileInfo finfo[3];
		static u32 cur_finfo   = 0;
		static u8 cur_addr_cmd = 0;
		static DVDCommandBlock get_addr_cmd[16];
		static DVDCommandBlock stop_cmd;
		static DVDCommandBlock finish_cmd;
		if (!useHardStreaming) {
			return;
		}
		while (true) {
			u16 introNum;
			switch (strCtrl.unkB) {
			case 1:
				if (!strCtrl.getList()) {
					return;
				}
				introNum = strCtrl.getIntroNum();
				if (introNum != 0xffff) {
					if (!strCtrl.startFirst(introNum, finfo, &cur_finfo)) {
						strCtrl.unkB = 0;
						return;
					}
					strCtrl.unk8 = 0;
				} else {
					if (!strCtrl.startFirst(strCtrl.getLoopNum(), finfo,
					                        &cur_finfo)) {
						strCtrl.unkB = 0;
						return;
					}
					strCtrl.clearListOne();
					strCtrl.unk8 = 1;
				}
				strCtrl.unkB = 3;
				return;
			case 2:
				if (strCtrl.unk8 == 0) {
					u16 loopNum = strCtrl.getLoopNum();
					if (loopNum != 0xffff) {
						if (!strCtrl.startSecond(loopNum, finfo, &cur_finfo)) {
							strCtrl.unkB = 0;
							return;
						}
						strCtrl.clearListOne();
						strCtrl.unk8 = 1;
						strCtrl.unkB = 4;
						return;
					}
				}
				strCtrl.clearListOne();
				if (!strCtrl.getList()) {
					if (strCtrl.unk8 == 0) {
						DVDStopStreamAtEndAsync(&stop_cmd, 0);
						strCtrl.unkB = 4;
						strCtrl.unk8 = 2;
						return;
					}
					strCtrl.unk8 = 3;
					strCtrl.unkB = 4;
					return;
				}
				introNum = strCtrl.getIntroNum();
				if (introNum != 0xffff) {
					if (!strCtrl.startSecond(introNum, finfo, &cur_finfo)) {
						strCtrl.unkB = 0;
						return;
					}
					strCtrl.unk8 = 0;
				} else {
					if (!strCtrl.startSecond(strCtrl.getLoopNum(), finfo,
					                         &cur_finfo)) {
						strCtrl.unkB = 0;
						return;
					}
					strCtrl.clearListOne();
					strCtrl.unk8 = 1;
				}
				strCtrl.unkB = 4;
				return;
			case 4: {
				u8 currentVol = strCtrl.getCurVol();
				AISetStreamVolLeft(currentVol);
				AISetStreamVolRight(currentVol);
				DVDGetStreamPlayAddrAsync(&get_addr_cmd[cur_addr_cmd],
				                          getAddrCallback);
				cur_addr_cmd++;
				cur_addr_cmd %= 16;
				return;
			}
			case 5: {
				TPlayList* list = strCtrl.getList();
				if (!list) {
					if (strCtrl.unk8 != 2) {
						strCtrl.unk8 = 3;
					}
					strCtrl.unkB = 4;
					return;
				}
				if (strCtrl.unk8 == 0) {
					if (list->getPair()->getLoop() == 0xffff) {
						if (list->getNext() == nullptr) {
							DVDStopStreamAtEndAsync(&stop_cmd, 0);
							strCtrl.clearListOne();
							strCtrl.unk8 = 2;
						} else {
							strCtrl.clearListOne();
							strCtrl.unk8 = 1;
							break;
						}
					} else {
						if (!strCtrl.startSecond(strCtrl.getLoopNum(), finfo,
						                         &cur_finfo)) {
							strCtrl.unkB = 0;
							return;
						}
						strCtrl.clearListOne();
						strCtrl.unk8 = 1;
					}
				} else if (!list) {
					strCtrl.unk8 = 3;
					strCtrl.unkB = 4;
				} else {
					introNum = strCtrl.getIntroNum();
					if (introNum == 0xffff) {
						strCtrl.unk8 = 0;
						break;
					}
					if (!strCtrl.startSecond(introNum, finfo, &cur_finfo)) {
						strCtrl.unkB = 0;
						return;
					}
					strCtrl.unk8 = 0;
				}
				strCtrl.unkB = 4;
				return;
			}
			case 6:
				AISetStreamVolLeft(0);
				AISetStreamVolRight(0);
				AISetStreamPlayState(0);
				DVDCancelStreamAsync(&finish_cmd, nullptr);
				strCtrl.unkB = 0;
				strCtrl.unk8 = 0;
				strCtrl.resetFader();
				strCtrl.unkA = 0;
				return;
			case 0:
			case 3:
			default:
				return;
			}
		}
	}

	TPlayList* getFreeList() { return 0; }

	static void firstBgmCallback(s32, DVDFileInfo*)
	{
		if (strCtrl.unkA == 0) {
			u8 vol = strCtrl.getCurVol();
			AISetStreamVolLeft(vol);
			AISetStreamVolRight(vol);
			AISetStreamSampleRate(1);
			AIResetStreamSampleCount();
			AISetStreamPlayState(1);
			strCtrl.unkB = 2;
		} else {
			strCtrl.unkB = 4;
			strCtrl.unkA = 0;
		}
	}

	static void secondBgmCallback(s32, DVDFileInfo*) { strCtrl.unkB = 4; }

	static void getAddrCallback(s32 param_1, DVDCommandBlock*)
	{
		static BOOL last_frame = 0;
		if (strCtrl.unk8 == 3) {
			if (param_1 == strCtrl.unk44[strCtrl.unk4C ^ 1]) {
				last_frame = true;
			} else if (last_frame) {
				last_frame   = false;
				strCtrl.unkB = 5;
			}
		} else if (strCtrl.unk8 == 2) {
			if (param_1 == strCtrl.unk44[strCtrl.unk4C ^ 1] - 0x8000) {
				last_frame = true;
			} else if (last_frame) {
				strCtrl.unkB = 6;
				unregistBgmAll();
				last_frame = false;
			}
		} else {
			if (param_1 == strCtrl.getLastAddr()) {
				last_frame = true;
			} else if (last_frame) {
				last_frame   = false;
				strCtrl.unkB = 5;
			}
		}
	}

	TControl::TControl()
	{
		unk0  = 0;
		mList = nullptr;
		unkA  = 0;
		unk8  = 0;
		unkB  = 0;
		unkC  = 1.0f;
		unk10 = 0.0f;
		unk14 = 0;
		unk18 = 0;
		unk1C = 0;
		unk20 = 0.0f;
		unk24 = 0.0f;
		unk2C = 1.0f;
		unk30 = 1.0f;
		unk28 = 1.0f;
		unk34 = 0.0f;
		unk38 = 0;
		unk3C = 0;
		unk40 = 0;
		unk4C = 0;
	}

	void TControl::setMasterVol(f32) { }

	u16 TControl::getIntroNum()
	{
		if (!mList)
			return 0xffff;

		if (mList->getPair() == NULL)
			return 0xffff;

		return mList->getPair()->getIntro();
	}

	u16 TControl::getLoopNum()
	{
		if (!mList)
			return 0xffff;

		if (mList->getPair() == NULL)
			return 0xffff;

		return mList->getPair()->getLoop();
	}

	BOOL TControl::fileOpen(u16 param_1, DVDFileInfo* param_2)
	{
		char buffer[64];
		char* ptr = streamFiles + param_1 * 0x24;
		strcpy(buffer, rootDir);
		strcat(buffer, ptr);
		if (!DVDOpen(buffer, param_2))
			return false;

		return true;
	}

	BOOL TControl::clearListOne()
	{
		if (mList == NULL)
			return FALSE;

		TPlayList* oldList = mList;
		mList              = mList->getNext();
		oldList->clear();
		return TRUE;
	}

	void TControl::setLastAddr(DVDFileInfo* param_1)
	{
		unk44[unk4C] = param_1->startAddr + param_1->length - 0x8000;
		unk4C ^= 1;
	}

	u32 TControl::getLastAddr() { return unk44[unk4C]; }

	BOOL TControl::startFirst(u16 param_1, DVDFileInfo* param_2, u32* param_3)
	{
		if (!fileOpen(param_1, param_2 + *param_3))
			return false;

		setLastAddr(param_2 + *param_3);
		DVDPrepareStreamAsync(param_2 + *param_3, 0, 0, firstBgmCallback);
		param_3[0]++;
		param_3[0] %= 3;
		return true;
	}

	BOOL TControl::startSecond(u16 param_1, DVDFileInfo* param_2, u32* param_3)
	{
		if (!fileOpen(param_1, param_2 + *param_3))
			return false;

		setLastAddr(param_2 + *param_3);
		DVDPrepareStreamAsync(param_2 + *param_3, 0, 0, secondBgmCallback);
		param_3[0]++;
		param_3[0] %= 3;
		return true;
	}

	void TControl::resetFader()
	{
		unk14 = 0;
		unk20 = 0.0f;
		unk38 = 0;
		unk18 = 0;
		unk24 = 0.0f;
		unk3C = 0;
		unk30 = 1.0f;
		unk2C = 1.0f;
		unk28 = 1.0f;
		unk1C = 0;
		unk40 = 0;
	}

	u8 TControl::getCurVol()
	{
		calcCurVolume();
		return volFloatToU8(unk10);
	}

	void TControl::calcCurVolume()
	{
		f32 f0 = 1.0f;
		f32 f6 = 1.0f;
		f32 f5 = 1.0f;

		if (unk14 != 0) {
			if (unk38 == 0)
				unk20 = 1.0f / (float)unk14;

			f32 thing = unk20 * (float)unk38;

			f6 = thing;
			if (thing >= 1.0f) {
				unk14 = 0;
				unk20 = 0.0f;
				unk38 = 0;
				f6    = 1.0f;
			} else {
				++unk38;
			}
		}

		if (unk18 != 0) {
			if (unk3C == 0)
				unk24 = 1.0f / (float)unk18;

			f32 thing = -(unk24 * (float)unk3C - 1.0f);

			f5 = thing;
			if (thing <= 0.0f) {
				f5 = 0.0f;

				unk14 = 0;
				unk20 = 0.0f;
				unk38 = 0;
				unk18 = 0;
				unk24 = 0.0f;
				unk3C = 0;
				unk30 = 1.0f;
				unk2C = 1.0f;
				unk28 = 1.0f;
				unk1C = 0;
				unk40 = 0;
				unkB  = 6;

				unregistBgmAll();
			} else {
				++unk3C;
			}
		}

		if (unk1C != 0) {
			if (unk40 == 0)
				unk34 = (unk28 - unk30) / (float)unk1C;

			f32 fVar2 = unk34 * (float)unk40 + unk30;
			if (unk40 >= unk1C) {
				fVar2 = unk28;
				unk30 = fVar2;
				unk28 = unk30;
				unk1C = 0;
				unk34 = 0.0f;
				unk40 = 0;
			} else {
				++unk40;
			}
			unk2C = fVar2;
		}

		f0 *= unk2C * (f6 * f5);
		f0 *= unkC;
		unk10 = f0;
	}

	void TControl::extendFilename(char*, char*) { }

	u32 TControl::msecToFrames(u32) { return 0; }

	u8 TControl::volFloatToU8(f32 param_1)
	{
		if (param_1 > 1.0f)
			param_1 = 1.0f;

		if (param_1 < 0.0f)
			param_1 = 0.0f;

		return param_1 * 255.0f;
	}

	THardStreamFile::THardStreamFile() { }

	void THardStreamFile::setNameVol(const char*, f32) { }

	TPlayPair::TPlayPair() { }

	bool TPlayPair::getRegisted() { return false; }

} // namespace HardStream

} // namespace JASystem
