#ifndef JASVLOAD_HPP
#define JASVLOAD_HPP

#include <dolphin/types.h>

namespace JASystem {

namespace Vload {
	struct VLArcEntry;

	void initVloadBuffers();
	void setMaxArcs(long);
	BOOL initHeader(char*);
	BOOL initHeaderM(char*, u8*, u8*);
	u32 getArchiveHandle(char*);
	u32 getLogicalHandle(char*);
	u32 getLogicalHandleS(char*, char*);
	u32 getHandle(u32);
	VLArcEntry* getRealHandle(u32);
	u32 checkSize(u32);
	u32 loadFile(u32, u8*, u32, u32);
	u32 loadFileAsync(u32, u8*, u32, u32, void (*)(u32), u32);
	void* getMemoryFile(u32);
}; // namespace Vload

} // namespace JASystem

#endif // JASVLOAD_HPP
