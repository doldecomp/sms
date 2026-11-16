#ifndef JASVLOAD_HPP
#define JASVLOAD_HPP

#include <dolphin/types.h>

namespace JASystem {

namespace Vload {
	struct VLArcEntry;

	void initVloadBuffers();
	void setMaxArcs(s32 max_arcs);
	BOOL initHeader(char* path);
	BOOL initHeaderM(char* path, u8* header, u8*);
	u32 getArchiveHandle(char* archive_name);
	u32 getLogicalHandle(char*);
	u32 getLogicalHandleS(char*, char*);
	u32 getHandle(u32);
	VLArcEntry* getRealHandle(u32 handle);
	u32 checkSize(u32 handle);
	u32 loadFile(u32 handle, u8* buffer, u32 offset, u32 size);
	u32 loadFileAsync(u32 handle, u8* buffer, u32 offset, u32 size,
	                  void (*callback)(u32), u32 user_data);
	void* getMemoryFile(u32);
}; // namespace Vload

} // namespace JASystem

#endif // JASVLOAD_HPP
