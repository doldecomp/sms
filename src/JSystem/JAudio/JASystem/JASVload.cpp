#include "JSystem/JAudio/JASystem/JASVload.hpp"

namespace JASystem {

u32 Vload::vlMaxArcs     = 0x10;
char* Vload::vlDirName   = 0;
char* Vload::vlArcName   = 0;
void* Vload::vlArc       = 0;
u32 Vload::vlCurrentArcs = 0;

void Vload::initVloadBuffers() { }

void Vload::setMaxArcs(long param) { }

void Vload::initHeader(char* param) { }

void Vload::initHeaderM(char* param1, u8* param2, u8* param3) { }

u32 Vload::getArchiveHandle(char* param) { return 0; }

u32 Vload::getLogicalHandle(char* param) { return 0; }

u32 Vload::getLogicalHandleS(char* param1, char* param2) { return 0; }

u32 Vload::getHandle(u32 param) { return 0; }

u32 Vload::getRealHandle(u32 param) { return 0; }

u32 Vload::checkSize(u32 param) { return 0; }

u32 Vload::loadFile(u32 param1, u8* param2, u32 param3, u32 param4)
{
	return 0;
}

void Vload::loadFileAsync(u32 param1, u8* param2, u32 param3, u32 param4,
                          void (*callback)(u32), u32 param6)
{
}

void* Vload::getMemoryFile(u32 param) { return 0; }

} // namespace JASystem
