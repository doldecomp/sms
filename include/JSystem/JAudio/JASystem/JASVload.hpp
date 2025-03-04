#ifndef JASVLOAD_HPP
#define JASVLOAD_HPP

#include <dolphin/types.h>

namespace JASystem {

class Vload {
public:
	void initVloadBuffers();
	void setMaxArcs(long);
	void initHeader(char*);
	void initHeaderM(char*, u8*, u8*);
	u32 getArchiveHandle(char*);
	u32 getLogicalHandle(char*);
	u32 getLogicalHandleS(char*, char*);
	u32 getHandle(u32);
	u32 getRealHandle(u32);
	u32 checkSize(u32);
	u32 loadFile(u32, u8*, u32, u32);
	void loadFileAsync(u32, u8*, u32, u32, void (*)(u32), u32);
	void* getMemoryFile(u32);

	static u32 vlMaxArcs;
	static char* vlDirName;
	static char* vlArcName;
	static void* vlArc;
	static u32 vlCurrentArcs;
};

} // namespace JASystem

#endif // JASVLOAD_HPP
