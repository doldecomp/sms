#ifndef JASHARDSTREAM_HPP
#define JASHARDSTREAM_HPP

#include <dolphin/dvd.h>
#include <dolphin/types.h>
#include <types.h>

namespace JASystem {

namespace HardStream {
	class TPlayList;

	class TControl {
	public:
		void setMasterVol(f32);
		u16 getIntroNum();
		u16 getLoopNum();
		BOOL fileOpen(u16, DVDFileInfo*);
		BOOL clearListOne();
		void setLastAddr(DVDFileInfo*);
		u32 getLastAddr();
		BOOL startFirst(u16, DVDFileInfo*, u32*);
		BOOL startSecond(u16, DVDFileInfo*, u32*);
		void resetFader();
		u8 getCurVol();
		void calcCurVolume();
		void extendFilename(char*, char*);
		u32 msecToFrames(u32);
		u8 volFloatToU8(f32);
		TControl();
		~TControl() { }

		TPlayList* getList() { return mList; }
		void setList(TPlayList* list) { mList = list; }

	public:
		/* 0x00 */ int unk0;
		/* 0x04 */ TPlayList* mList;
		/* 0x08 */ u16 unk8;
		/* 0x0A */ u8 unkA;
		/* 0x0B */ u8 unkB;
		/* 0x0C */ f32 unkC;
		/* 0x10 */ f32 unk10;
		/* 0x14 */ u32 unk14;
		/* 0x18 */ u32 unk18;
		/* 0x1C */ u32 unk1C;
		/* 0x20 */ f32 unk20;
		/* 0x24 */ f32 unk24;
		/* 0x28 */ f32 unk28;
		/* 0x2C */ f32 unk2C;
		/* 0x30 */ f32 unk30;
		/* 0x34 */ f32 unk34;
		/* 0x38 */ u32 unk38;
		/* 0x3C */ u32 unk3C;
		/* 0x40 */ u32 unk40;
		/* 0x44 */ u32 unk44[2];
		/* 0x4C */ u8 unk4C;
	};

	struct THardStreamFile {
		THardStreamFile();
		~THardStreamFile();
		void setNameVol(const char*, f32);
	};

	struct TPlayPair {
		TPlayPair();
		~TPlayPair();
		bool getRegisted();

		u16 getIntro() { return mIntroNum; }
		u16 getLoop() { return mLoopNum; }

	public:
		/* 0x00 */ u16 unk0;
		/* 0x02 */ u16 unk2;
		/* 0x04 */ u16 mIntroNum;
		/* 0x06 */ u16 mLoopNum;
	};

	struct TPlayList {
		TPlayList();
		~TPlayList();

		void clear()
		{
			mPair     = nullptr;
			mNextList = nullptr;
			unk8      = 0;
		}

		TPlayPair* getPair() { return mPair; }
		TPlayList* getNext() { return mNextList; }

	public:
		/* 0x00 */ TPlayPair* mPair;
		/* 0x04 */ TPlayList* mNextList;
		/* 0x08 */ u32 unk8;
	};

	void init();
	void main();
	void setMaxBgmPairs(u32);
	void setMaxFilenames(u32);
	void setRootDir(const char*);
	void setMasterVol(f32);
	void setFile(u32, const char*, f32);
	void setBgmPair(u32, u16, u16);
	void registBgmPair(u32, u32);
	void unregistBgmPair(u32);
	BOOL unregistBgmAll();
	void playBgm(u32);
	void stopBgm(u32);
	bool checkPlaying();
	void moveVolume(f32, u32);
	TPlayList* getFreeList();
} // namespace HardStream

} // namespace JASystem

#endif // JASHARDSTREAM_HPP
