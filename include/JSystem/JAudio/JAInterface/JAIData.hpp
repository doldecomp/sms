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
	/* 0x0 */ char unk0[0x10];
};

// TODO: I accidentally put a bunch of stuff in here
// that doesn't actually belong, fix it!!!
class JAISoundTable {
public:
	/* 0x0 */ u8 unk0;
	/* 0x1 */ u8 unk1;
	/* 0x2 */ char unk2[2];
	/* 0x4 */ u8 unk4;
	/* 0x5 */ char unk5[1];
	/* 0x6 */ u8 unk6;
	/* 0x7 */ u8 unk7;
	/* 0x8 */ u8 unk8;
	/* 0x9 */ char unk9[7];
	/* 0x10 */ char unk10[4];
	/* 0x14 */ u32 unk14;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ f32* unk1C;
	/* 0x20 */ u8* unk20;
	/* 0x24 */ u32 unk24;
	/* 0x24 */ u32 unk28;
	/* 0x2C */ void* unk2C;
	/* 0x30 */ u32 unk30;
	/* 0x34 */ u32 unk34;
	/* 0x38 */ u32 unk38;
	/* 0x3C */ u32 unk3C;
	/* 0x40 */ void* unk40;
	/* 0x44 */ u32 unk44;
	/* 0x44 */ u32 unk48;
	/* 0x44 */ u32 unk4C;
	/* 0x44 */ u32 unk50;
	/* 0x54 */ s32* unk54;
	/* 0x58 */ s32* unk58;
	/* 0x5C */ u8** unk5C;
	/* 0x60 */ u32* unk60;
	/* 0x64 */ u32 unk64;
	/* 0x68 */ char unk68[4];
	/* 0x6C */ u32 unk6C;
	/* 0x70 */ char unk70[8];
	/* 0x78 */ void* unk78;
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
	/* 0x0 */ void* unk0;
	/* 0x4 */ u8** unk4;
	/* 0x8 */ void** unk8;
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
	/* 0x1F8 */ u32 unk1F8;
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
