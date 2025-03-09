#ifndef JASWAVEARCLOADER_HPP
#define JASWAVEARCLOADER_HPP

#include <dolphin/types.h>

class JKRHeap;

namespace JASystem {

namespace Kernel {
	class THeap;
}

namespace WaveArcLoader {
	class TObject {
	public:
		virtual u32* getLoadFlagPtr()                  = 0;
		virtual const char* getWaveArcFileName() const = 0;
		virtual Kernel::THeap* getHeap()               = 0;
	};

	bool init();
	void init(u32);
	void setCurrentDir(const char*);
	const char* getCurrentDir();
	bool loadWave(TObject*);
	bool eraseWave(TObject*);
	Kernel::THeap* getRootHeap();
}; // namespace WaveArcLoader

} // namespace JASystem

#endif // JASWAVEARCLOADER_HPP
