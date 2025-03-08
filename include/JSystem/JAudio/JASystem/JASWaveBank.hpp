#ifndef JASWAVEBANK_HPP
#define JASWAVEBANK_HPP

#include <dolphin/types.h>
#include <JSystem/JAudio/JASystem/JASHeapCtrl.hpp>

class JKRHeap;

namespace JASystem {

// TODO: Yoinked from TWW cuz yolo. May be incorrect.
struct TWaveInfo {
	/* 0x00 */ u8 unk0;
	/* 0x01 */ u8 unk1;
	/* 0x02 */ u8 unk2;
	/* 0x04 */ f32 unk4;
	/* 0x08 */ int unk8;
	/* 0x0C */ int unkc;
	/* 0x10 */ int unk10;
	/* 0x14 */ int unk14;
	/* 0x18 */ int unk18;
	/* 0x1C */ int unk1c;
	/* 0x20 */ s16 unk20;
	/* 0x22 */ s16 unk22;
	/* 0x24 */ void* unk24;
	/* 0x28 */ int unk28;
};

class TWaveHandle {
public:
	virtual ~TWaveHandle() { }
	virtual const TWaveInfo* getWaveInfo() const = 0;
	virtual const void* getWavePtr() const       = 0;
};

class TWaveBank {
public:
	virtual ~TWaveBank() { }
	virtual TWaveHandle* getWaveHandle(u32) const = 0;
	virtual int getType() const                   = 0;

	static JKRHeap* getCurrentHeap();

	static JKRHeap* sCurrentHeap;
};

} // namespace JASystem

#endif // JASWAVEBANK_HPP
