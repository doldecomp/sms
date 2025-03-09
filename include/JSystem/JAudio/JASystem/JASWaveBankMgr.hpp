#ifndef JASWAVEBANKMGR_HPP
#define JASWAVEBANKMGR_HPP

#include <dolphin/types.h>

namespace JASystem {

class TWaveBank;

class WaveBankMgr {
public:
	static void init(int tableSize);
	static TWaveBank* getWaveBank(int bankIndex);
	static bool registWaveBank(int bankIndex, TWaveBank* waveBank);
	static bool registWaveBankWS(int bankIndex, void* waveBankData);
	static bool loadWave(int bankIndex, int waveIndex);
	static bool eraseWave(int bankIndex, int waveIndex);
	u32 getUsedHeapSize();

	static int sTableSize;
	static TWaveBank** sWaveBank;
};

} // namespace JASystem

#endif // JASWAVEBANKMGR_HPP
