#ifndef JASVLOAD_HPP
#define JASVLOAD_HPP

#include "types.h"

namespace JASystem {

class Vload {
public:
	static void initVloadBuffers();
	static void setMaxArcs(long);
	static void initHeader(char*);
	static void initHeaderM(char*, unsigned char*, unsigned char*);
	static unsigned long getArchiveHandle(char*);
	static unsigned long getLogicalHandle(char*);
	static unsigned long getLogicalHandleS(char*, char*);
	static unsigned long getHandle(unsigned long);
	static unsigned long getRealHandle(unsigned long);
	static unsigned long checkSize(unsigned long);
	static unsigned long loadFile(unsigned long, unsigned char*, unsigned long,
	                              unsigned long);
	static void loadFileAsync(unsigned long, unsigned char*, unsigned long,
	                          unsigned long, void (*)(unsigned long),
	                          unsigned long);
	static void* getMemoryFile(unsigned long);

	static long vlMaxArcs;
	static char* vlDirName;
	static char* vlArcName;
	static void* vlArc;
	static long vlCurrentArcs;
};

} // namespace JASystem

#endif // JASVLOAD_HPP
