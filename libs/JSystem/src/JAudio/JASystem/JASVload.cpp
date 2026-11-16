#include <JSystem/JAudio/JASystem/JASVload.hpp>
#include <JSystem/JAudio/JASystem/JASDvdThread.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/types.h>
#include <string.h>

namespace JASystem {

namespace Vload {

	// fabricated
	struct VLArcEntry {
		/* 0x0 */ u32 unk0;
		/* 0x4 */ u32 unk4;
		/* 0x8 */ u32 unk8;
		/* 0xC */ u16 unkC;
		/* 0xE */ u16 redirectIndex;
		/* 0x10 */ u32 unk10;
		/* 0x14 */ u32 unk14;
		/* 0x18 */ u32 dataOffset;
		/* 0x1C */ u32 fileSize;
	};

	// fabricated
	struct VLArcHeader {
		/* 0x0 */ u32 unk0;
		/* 0x4 */ u32 unk4;
		/* 0x8 */ u8* unk8;
		/* 0xC */ u32 entryCount;
		/* 0x10 */ char dataFileName[0x10];
		/* 0x20 */ VLArcEntry entries[];
	};

	s32 vlMaxArcs       = 16;
	char** vlDirName    = nullptr;
	char** vlArcName    = nullptr;
	VLArcHeader** vlArc = nullptr;
	u32 vlCurrentArcs   = 0;
} // namespace Vload

void Vload::setMaxArcs(s32 max_arcs) { vlMaxArcs = max_arcs; }

BOOL Vload::initHeader(char* path)
{
	return initHeaderM(path, nullptr, nullptr);
}

BOOL Vload::initHeaderM(char* path, u8* header, u8* param_3)
{
	if (header == nullptr) {
		u32 fileSize = Dvd::checkFileExtend(path);
		if (!fileSize)
			return 0;

		header = (u8*)Kernel::allocFromSysDram(fileSize);
		if (!header)
			return 0;

		if (!Dvd::loadFileDvdT(path, header))
			return 0;
	}

	u32 lastSlash = strlen(path) - 1;
	while (lastSlash != 0) {
		if (path[lastSlash] == '/')
			break;
		--lastSlash;
	}

	if (lastSlash == 0) {
		strcpy(vlDirName[vlCurrentArcs], "/");
	} else {
		strncpy(vlDirName[vlCurrentArcs], path, lastSlash);
		vlDirName[vlCurrentArcs][lastSlash] = 0;
	}

	strcpy(vlArcName[vlCurrentArcs], &path[lastSlash + 1]);

	vlArc[vlCurrentArcs]       = (VLArcHeader*)header;
	vlArc[vlCurrentArcs]->unk8 = param_3;

	++vlCurrentArcs;

	return true;
}

u32 Vload::getArchiveHandle(char* archive_name)
{
	u32 i = 0;
	for (; i < vlCurrentArcs; ++i)
		if (strcmp(archive_name, vlArcName[i]) == 0)
			break;

	if (i != vlCurrentArcs)
		return i << 0x10;

	return -1;
}

u32 Vload::getLogicalHandleS(char*, char*) { return 0; }

u32 Vload::getLogicalHandle(char*) { return 0; }

u32 Vload::getHandle(u32) { return 0; }

#pragma dont_inline on
Vload::VLArcEntry* Vload::getRealHandle(u32 handle)
{
	u32 entryIndex = handle & 0xffff;
	if ((handle >> 16) >= vlCurrentArcs)
		return nullptr;

	VLArcHeader* arc = vlArc[handle >> 16];
	if (!arc)
		return nullptr;

	if (entryIndex >= arc->entryCount)
		return nullptr;

	VLArcEntry* entry = arc[entryIndex].entries;
	while (entry->redirectIndex != 0xffff)
		entry = arc[entry->redirectIndex].entries;

	return entry;
}
#pragma dont_inline off

u32 Vload::checkSize(u32 handle)
{
	VLArcEntry* entry = getRealHandle(handle);
	if (!entry)
		return 0;
	return entry->fileSize;
}

u32 Vload::loadFile(u32 handle, u8* buffer, u32 offset, u32 size)
{
	char path[0x80];
	volatile u32 result;
	u32 archiveIndex;
	u32 fileOffset;

	result = 0;

	archiveIndex = handle >> 0x10;

	fileOffset = getRealHandle(handle)->dataOffset;
	fileOffset += offset;

	strcpy(path, vlDirName[archiveIndex]);
	strcat(path, "/");
	strcat(path, vlArc[archiveIndex]->dataFileName);

	Dvd::loadToDramDvdT(0, path, buffer, fileOffset, size, (u32*)&result,
	                    nullptr);

	while (result == 0)
		;
	return result;
}

u32 Vload::loadFileAsync(u32 handle, u8* buffer, u32 offset, u32 size,
                         void (*callback)(u32), u32 user_data)
{
	char path[0x80];
	u32 archiveIndex;
	u32 fileOffset;

	archiveIndex = handle >> 0x10;

	fileOffset = getRealHandle(handle)->dataOffset;
	fileOffset += offset;

	strcpy(path, vlDirName[archiveIndex]);
	strcat(path, "/");
	strcat(path, vlArc[archiveIndex]->dataFileName);

	Dvd::loadToDramDvdT(user_data, path, buffer, fileOffset, size, 0, callback);

	return size;
}

void* Vload::getMemoryFile(u32) { return 0; }

void Vload::initVloadBuffers()
{
	vlDirName = new (JASDram, 0) char*[vlMaxArcs];
	vlArcName = new (JASDram, 0) char*[vlMaxArcs];
	vlArc     = new (JASDram, 0) VLArcHeader*[vlMaxArcs];
	for (s32 i = 0; i < vlMaxArcs; ++i) {
		vlDirName[i] = new (JASDram, 0) char[0x40];
		vlArcName[i] = new (JASDram, 0) char[0x20];
	}
}

} // namespace JASystem
