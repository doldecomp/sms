#ifndef JASWAVEARCLOADER_HPP
#define JASWAVEARCLOADER_HPP

#include <dolphin/types.h>

namespace JASystem {

class WaveArcLoader {
public:
	class TObject {
	public:
		virtual bool* getLoadFlagPtr()                 = 0;
		virtual const char* getWaveArcFileName() const = 0;
		virtual void* getHeap()                        = 0;
	};

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
