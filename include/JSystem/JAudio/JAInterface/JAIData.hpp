#ifndef JAIDATA_HPP
#define JAIDATA_HPP

#include <JSystem/JAudio/JAInterface/JAIEntrySequence.hpp>
#include <JSystem/JAudio/JAInterface/JAIEntrySe.hpp>
#include <JSystem/JAudio/JAInterface/JAIEntryStream.hpp>
#include <types.h>

class JAIBasic;
class JAISound;
class JAISoundTable {
public:
	/* 0x0 */ u32 unk0;
	/* 0x4 */ u8 unk4;
	/* 0x5 */ u8 unk5;
	/* 0x6 */ u8 unk6;
	/* 0x7 */ u8 unk7;
	/* 0x8 */ u8 unk8;
	/* 0x9 */ u8 unk9;
	/* 0xA */ char unkA[0x10 - 0xA];
	/* 0x10 */ u8 unk10;
	/* 0x14 */ u32 unk14;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ f32* unk1C;
	/* 0x20 */ u8* unk20;
	/* 0x24 */ u32 unk24;
	/* 0x24 */ u32 unk28;
	/* 0x2C */ JAISound* unk2C;
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
	/* 0x5C */ u32 unk5C;
	/* 0x60 */ u32 unk60;
	/* 0x64 */ u32 unk64;
	/* 0x68 */ char unk68[4];
	/* 0x6C */ u32 unk6C;
	/* 0x70 */ char unk70[8];
	/* 0x78 */ void* unk78;
};

class JAIHeapBlock;
struct JAIMoveParaSet;
class JAILinkBuffer;
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
	void moveParameter(JAIMoveParaSet* moveParaSet);

	bool checkOnMemory(u32 param1, u8* param2);
	u8 checkUsefulAutoHeapPosition();
	u8* getFreeAutoHeapPointer(u8 param1, u32 param2);
	void releaseAutoHeapPointer(u8 param);
	u8* getFreeStayHeapPointer(u32 param1, u32 param2);

	void clearAutoHeap(JAIHeapBlock* heapBlock);
	void clearAutoHeap(u32 param);
	void clearStayHeap(u32 param);

	void setAutoHeapLoadedFlag(u8 param1, u8 param2);
	void changeAutoHeapPointerToPosition(u8* ptr);
	void stopPlayingSeq(u32 param);
	u8* getAutoHeapPointer(u32 param);
	bool getInfoPointer(u32 param, void** ptr);

	void reloadInfoDataWork(JAISoundTable* soundTable);
	void setInfoDataPointer(JAISoundTable* soundTable, u8* ptr);

public:
	/* 0x0 */ char unk0[0xC - 0x0];
	/* 0xC */ JAISoundTable unkC;
	/* 0x88 */ JAISoundTable unk88;
	/* 0x104 */ JAISoundTable unk104;
	/* 0x180 */ void* unk180;
	/* 0x184 */ void* unk184;
	/* 0x188 */ char unk188[0x1B4 - 0x188];
	/* 0x1B4 */ u32 unk1B4;
	/* 0x1B8 */ u32 unk1B8;
	/* 0x1BC */ char unk1BC[8];
	/* 0x1C4 */ JAISeqParameter* unk1C4;
	/* 0x1C8 */ char unk1C8[8];
	/* 0x1D0 */ JAISeParameter* unk1D0;
	/* 0x1D4 */ char unk1D4[0x1F4 - 0x1D4];
	/* 0x1F4 */ JAIBasic* unk1F4;
	/* 0x1F8 */ u32 unk1F8;
	/* 0x1FC */ JAISeqEntry unk1FC;
	/* 0x200 */ JAISeEntry unk200;
	/* 0x204 */ JAIStreamEntry unk204;
	
};

#endif // JAIDATA_HPP
