#ifndef JASWAVEBANKMGR_HPP
#define JASWAVEBANKMGR_HPP

#include "types.h"

namespace JASystem {

class TWaveBank;

class WaveBankMgr {
public:
	static void init(int tableSize);
	static TWaveBank* getWaveBank(int bankIndex);
	static void registWaveBank(int bankIndex, TWaveBank* waveBank);
	static void registWaveBankWS(int bankIndex, void* waveBankData);
	static void loadWave(int bankIndex, int waveIndex);
	static void eraseWave(int bankIndex, int waveIndex);
	static unsigned long getUsedHeapSize();

	static int sTableSize;
	static TWaveBank** sWaveBank;
};

} // namespace JASystem

#endif // JASWAVEBANKMGR_HPP
