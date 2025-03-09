#ifndef JASWAVEARCLOADER_HPP
#define JASWAVEARCLOADER_HPP

#include <dolphin/types.h>

class JKRHeap;

namespace JASystem {

namespace Kernel {
	class THeap;
}

class WaveArcLoader {
public:
	class TObject {
	public:
		virtual u32* getLoadFlagPtr()                  = 0;
		virtual const char* getWaveArcFileName() const = 0;
		virtual Kernel::THeap* getHeap()               = 0;
	};

	static bool init();
	void init(u32);
	static void setCurrentDir(const char*);
	static const char* getCurrentDir();
	static bool loadWave(TObject*);
	static bool eraseWave(TObject*);
	static Kernel::THeap* getRootHeap();

	static char sCurrentDir[64];
	static Kernel::THeap sAramHeap;
};

} // namespace JASystem

#endif // JASWAVEARCLOADER_HPP
