#ifndef JASWAVEBANKMGR_HPP
#define JASWAVEBANKMGR_HPP

#include <dolphin/types.h>

namespace JASystem {

class TWaveBank;

namespace WaveBankMgr {
	void init(int tableSize);
	TWaveBank* getWaveBank(int bankIndex);
	bool registWaveBank(int bankIndex, TWaveBank* waveBank);
	bool registWaveBankWS(int bankIndex, void* waveBankData);
	bool loadWave(int bankIndex, int waveIndex);
	bool eraseWave(int bankIndex, int waveIndex);
	u32 getUsedHeapSize();

	extern int sTableSize;
	extern TWaveBank** sWaveBank;
} // namespace WaveBankMgr

} // namespace JASystem

#endif // JASWAVEBANKMGR_HPP
