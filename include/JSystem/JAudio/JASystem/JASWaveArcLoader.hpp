#ifndef JASWAVEARCLOADER_HPP
#define JASWAVEARCLOADER_HPP

#include "types.h"

namespace JASystem {

class WaveArcLoader {
public:
	struct TObject;

	static void init();
	static void init(unsigned long);
	static void setCurrentDir(const char*);
	static const char* getCurrentDir();
	static void loadWave(TObject*);
	static void eraseWave(TObject*);
	static void* getRootHeap();

	static const char* sCurrentDir;
	static void* sAramHeap;
};

} // namespace JASystem

#endif // JASWAVEARCLOADER_HPP
