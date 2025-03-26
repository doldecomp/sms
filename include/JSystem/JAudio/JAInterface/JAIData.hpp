#ifndef JAIDATA_HPP
#define JAIDATA_HPP

#include <JSystem/JAudio/JAInterface/JAIEntrySequence.hpp>
#include <JSystem/JAudio/JAInterface/JAIEntrySe.hpp>
#include <JSystem/JAudio/JAInterface/JAIEntryStream.hpp>
#include <JSystem/JAudio/JASystem/JASDSPInterface.hpp>
#include <types.h>

class JAIBasic;
class JAISound;
class JAISeqUpdateData;
class JAIDummyVec {
public:
	/* 0x0 */ JAIDummyVec* unk0;
	/* 0x4 */ JAIDummyVec* unk4;
	/* 0x8 */ JAISound* unk8;
	/* 0xC */ u32 unkC;
	/* 0x10 */ char unk10[0xC];
};
struct JAIStreamUpdateParameter;

// fabricated
struct JAISoundInfo {
	/* 0x0 */ u32 unk0;
	/* 0x4 */ char unk4[0xC];
};

struct FabricatedSoundTableMemoryHeader {
	/* 0x0 */ char unk0[3];
	/* 0x3 */ u8 unk3;
	/* 0x4 */ char unk4[2];
	struct Pair {
		s16 unk0;
		s16 unk2;
	};
	/* 0x6 */ Pair unk6[18];
	/* 0x50 */ JAISoundInfo unk50[];
};

// TODO: I accidentally put a bunch of stuff in here
// that doesn't actually belong, fix it!!!
class JAISoundTable {
public:
	/* 0x0 */ u8 unk0;
	/* 0x1 */ u8 unk1;
	/* 0x2 */ u16 unk2[18];
	/* 0x28 */ u32 unk28;
	/* 0x2C */ void* unk2C;
	/* 0x30 */ JAISoundInfo* unk30[18];
	/* 0x78 */ u8* unk78;
};

class JAIHeapBlock {
public:
	/* 0x0 */ u8 unk0;
	/* 0x4 */ void* unk4;
	/* 0x8 */ u32 unk8;
	/* 0xC */ u32 unkC;
	/* 0x10 */ u32 unk10;
};

struct JAIMoveParaSet;
class JAILinkBuffer {
public:
	/* 0x0 */ JAISound* unk0;
	/* 0x4 */ JAISound* unk4;
	/* 0x8 */ JAISound* unk8;
};
struct JAISeqParameter;
struct JAISeParameter;
struct JAIStreamParameter;

class JAIData {
public:
	JAIData();

	void init();
	void initData();
	void initInfoDataWork(JAISoundTable* soundTable, char* param);
	void initLinkBuffer(JAILinkBuffer* linkBuffer, u32 param);
	void initSeqParaLinkBuffer();
	void initSeParaLinkBuffer();
	void initStreamParaLinkBuffer();
	void initDummyVecLink();
	void initSeqParameter(JAISeqParameter* param);
	void initSePara(JAISeParameter* param);
	void initStreamParameter(JAIStreamParameter* param);
	void initSeqTrackInfoParameter(u32 param);
	void initStreamUpdateParameter();

	void setSeMovePara(JAIMoveParaSet* moveParaSet);
	bool moveParameter(JAIMoveParaSet* moveParaSet);

	void* checkOnMemory(u32 param1, u8* param2);
	u8 checkUsefulAutoHeapPosition();
	void* getFreeAutoHeapPointer(u8 param1, u32 param2);
	void releaseAutoHeapPointer(u8 param);
	u8* getFreeStayHeapPointer(u32 param1, u32 param2);

	void clearAutoHeap(JAIHeapBlock* heapBlock);
	void clearAutoHeap(u32 param);
	void clearStayHeap(u32 param);

	void setAutoHeapLoadedFlag(u8 param1, u8 param2);
	void changeAutoHeapPointerToPosition(u8* ptr);
	void stopPlayingSeq(u32 param);
	u8* getAutoHeapPointer(u32 param);
	void getInfoPointer(u32 param, void** ptr);

	void reloadInfoDataWork(JAISoundTable* soundTable);
	void setInfoDataPointer(JAISoundTable* soundTable, u8* ptr);

public:
	struct FabricatedUnk0Struct {
		u8 unk0;
		f32 unk4;
		f32 unk8;
		f32 unkC;
		f32 unk10;
		f32 unk14;
	};

	/* 0x0 */ FabricatedUnk0Struct* unk0;
	/* 0x4 */ u8** unk4;

	struct FabricatedUnk8Struct {
		u32 unk0;
		u32 unk4;
		JAISound* unk8;
	};

	/* 0x8 */ FabricatedUnk8Struct** unk8;
	/* 0xC */ JAISoundTable unkC;
	/* 0x88 */ JAISoundTable unk88;
	/* 0x104 */ JAISoundTable unk104;
	/* 0x180 */ JAISeqUpdateData* unk180;
	/* 0x184 */ JAIStreamUpdateParameter* unk184;
	// TODO: some kind of a struct?
	/* 0x188 */ u8 unk188;
	/* 0x18C */ u32 unk18C[4];
	/* 0x19C */ s16* unk19C[4];
	/* 0x1AC */ JASystem::DSPInterface::FxlineConfig_** unk1AC;
	/* 0x1B0 */ u8 unk1B0;
	/* 0x1B4 */ u32 unk1B4;
	/* 0x1B8 */ u32 unk1B8;
	/* 0x1BC */ JAISeqParameter* unk1BC;
	/* 0x1C0 */ JAISeqParameter* unk1C0;
	/* 0x1C4 */ JAISeqParameter* unk1C4;
	/* 0x1C8 */ JAISeParameter* unk1C8;
	/* 0x1CC */ JAISeParameter* unk1CC;
	/* 0x1D0 */ JAISeParameter* unk1D0;
	/* 0x1D4 */ JAIStreamParameter* unk1D4;
	/* 0x1D8 */ JAIStreamParameter* unk1D8;
	/* 0x1DC */ JAIStreamParameter* unk1DC;
	/* 0x1E0 */ u32* unk1E0;
	/* 0x1E4 */ JAISound** unk1E4;
	/* 0x1E8 */ JAILinkBuffer* unk1E8;
	/* 0x1EC */ JAIHeapBlock* unk1EC;
	/* 0x1F0 */ JAIHeapBlock* unk1F0;
	/* 0x1F4 */ JAIBasic* unk1F4;

	struct FabricatedUnk1F8Struct {
		/* 0x0 */ char unk0[0x10];
		/* 0x10 */ char unk10[16]; // file name?
		/* 0x20 */ void* unk20;
		/* 0x24 */ char unk24[0xC];
	};

	/* 0x1F8 */ FabricatedUnk1F8Struct* unk1F8;
	/* 0x1FC */ JAISeqEntry unk1FC;
	/* 0x200 */ JAISeEntry unk200;
	/* 0x204 */ JAIStreamEntry unk204;
	/* 0x208 */ JAISound* unk208;
	/* 0x20C */ JAISound* unk20C;
	/* 0x210 */ JAILinkBuffer unk210;
	/* 0x21C */ JAILinkBuffer unk21C;
	/* 0x228 */ JAIDummyVec* unk228;
	/* 0x22C */ JAIDummyVec* unk22C;
	/* 0x230 */ JAIDummyVec* unk230;
};

#endif // JAIDATA_HPP
