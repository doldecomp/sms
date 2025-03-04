#ifndef JASWAVEARCLOADER_HPP
#define JASWAVEARCLOADER_HPP

#include <dolphin/types.h>

namespace JASystem {

class WaveArcLoader {
public:
	struct TObject;

	void init();
	void init(u32);
	void setCurrentDir(const char*);
	const char* getCurrentDir();
	void loadWave(TObject*);
	void eraseWave(TObject*);
	void* getRootHeap();

	static const char* sCurrentDir;
	static void* sAramHeap;
};

} // namespace JASystem

#endif // JASWAVEARCLOADER_HPP
