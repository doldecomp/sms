#include <JSystem/JAudio/JASystem/JASVload.hpp>
#include <JSystem/JAudio/JASystem/JASDvdThread.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <types.h>

namespace JASystem {

namespace Vload {

	// fabricated
	struct VLArc {
		/* 0x0 */ u32 unk0;
		/* 0x4 */ u32 unk4;
		/* 0x8 */ u8* unk8;
		/* 0xC */ u32 unkC;
		/* 0x10 */ char unk10[0x10];
	};

	// fabricated
	struct VLArcEntry {
		/* 0x0 */ u32 unk0;
		/* 0x4 */ u32 unk4;
		/* 0x8 */ u32 unk8;
		/* 0xC */ u16 unkC;
		/* 0xE */ u16 unkE;
		/* 0x10 */ u32 unk10;
		/* 0x14 */ u32 unk14;
		/* 0x18 */ u32 unk18;
		/* 0x1C */ u32 unk1C;
	};

	s32 vlMaxArcs     = 0x10;
	char** vlDirName  = 0;
	char** vlArcName  = 0;
	VLArc** vlArc     = 0;
	u32 vlCurrentArcs = 0;
} // namespace Vload

void Vload::setMaxArcs(long param) { }

BOOL Vload::initHeader(char* param) { return initHeaderM(param, 0, 0); }

BOOL Vload::initHeaderM(char* param_1, u8* param_2, u8* param_3)
{
	if (param_2 == nullptr) {
		u32 uVar1 = Dvd::checkFileExtend(param_1);
		if (!uVar1)
			return 0;

		param_2 = (u8*)Kernel::allocFromSysDram(uVar1);
		if (!param_2)
			return 0;

		if (!Dvd::loadFileDvdT(param_1, param_2))
			return 0;
	}

	u32 lastSlash = strlen(param_1) - 1;
	while (lastSlash != 0) {
		if (param_1[lastSlash] == '/')
			break;
		--lastSlash;
	}

	if (lastSlash == 0) {
		strcpy(vlDirName[vlCurrentArcs], "/");
	} else {
		strncpy(vlDirName[vlCurrentArcs], param_1, lastSlash);
		vlDirName[vlCurrentArcs][lastSlash] = 0;
	}

	strcpy(vlArcName[vlCurrentArcs], &param_1[lastSlash + 1]);

	vlArc[vlCurrentArcs]       = (VLArc*)param_2;
	vlArc[vlCurrentArcs]->unk8 = param_3;

	++vlCurrentArcs;

	return true;
}

u32 Vload::getArchiveHandle(char* param)
{
	u32 i = 0;
	for (; i < vlCurrentArcs; ++i)
		if (strcmp(param, vlArcName[i]) == 0)
			break;

	if (i != vlCurrentArcs)
		return i << 0x10;

	return -1;
}

u32 Vload::getLogicalHandleS(char* param1, char* param2) { return 0; }

u32 Vload::getLogicalHandle(char* param) { return 0; }

u32 Vload::getHandle(u32 param) { return 0; }

#pragma dont_inline on
Vload::VLArcEntry* Vload::getRealHandle(u32 param)
{
	// TODO: wtf is happening here
	u32 low = param & 0xffff;
	if ((param >> 0x10) >= vlCurrentArcs)
		return 0;
	VLArc* arc = vlArc[param >> 0x10];
	if (!arc)
		return 0;

	if (low >= arc->unkC)
		return 0;

	VLArcEntry* thing = (VLArcEntry*)(&arc[low] + 1);
	while (thing->unkE != 0xffff) {
		thing = (VLArcEntry*)(&arc[thing->unkE] + 1);
	}

	return thing;
}
#pragma dont_inline off

u32 Vload::checkSize(u32 param)
{
	VLArcEntry* hndl = getRealHandle(param);
	if (!hndl)
		return 0;
	return hndl->unk1C;
}

u32 Vload::loadFile(u32 param1, u8* param2, u32 param3, u32 param4)
{
	char buffer[0x80];
	volatile u32 result;
	u32 thing;
	u32 hi;

	hi     = param1 >> 0x10;
	result = 0;

	VLArcEntry* hndl = getRealHandle(param1);

	thing = hndl->unk18 + param3;

	strcpy(buffer, vlDirName[hi]);
	strcat(buffer, "/");
	strcat(buffer, vlArc[hi]->unk10);

	Dvd::loadToDramDvdT(0, buffer, param2, thing, param4, (u32*)&result,
	                    nullptr);

	while (result == 0)
		;
	return result;
}

u32 Vload::loadFileAsync(u32 param1, u8* param2, u32 param3, u32 param4,
                         void (*param5)(u32), u32 param6)
{
	char buffer[0x80];
	u32 thing;
	u32 hi;

	hi = param1 >> 0x10;

	VLArcEntry* hndl = getRealHandle(param1);

	thing = hndl->unk18 + param3;

	strcpy(buffer, vlDirName[hi]);
	strcat(buffer, "/");
	strcat(buffer, vlArc[hi]->unk10);

	Dvd::loadToDramDvdT(param6, buffer, param2, thing, param4, 0, param5);

	return param4;
}

void* Vload::getMemoryFile(u32 param) { return 0; }

void Vload::initVloadBuffers()
{
	vlDirName = new (JASDram, 0) char*[vlMaxArcs];
	vlArcName = new (JASDram, 0) char*[vlMaxArcs];
	vlArc     = new (JASDram, 0) VLArc*[vlMaxArcs];
	for (s32 i = 0; i < vlMaxArcs; ++i) {
		vlDirName[i] = new (JASDram, 0) char[0x40];
		vlArcName[i] = new (JASDram, 0) char[0x20];
	}
}

} // namespace JASystem
