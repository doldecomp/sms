#ifndef JASWAVEBANKMGR_HPP
#define JASWAVEBANKMGR_HPP

#include <dolphin/types.h>

namespace JASystem {

class TWaveBank;

class WaveBankMgr {
public:
	void init(int tableSize);
	TWaveBank* getWaveBank(int bankIndex);
	void registWaveBank(int bankIndex, TWaveBank* waveBank);
	void registWaveBankWS(int bankIndex, void* waveBankData);
	void loadWave(int bankIndex, int waveIndex);
	void eraseWave(int bankIndex, int waveIndex);
	u32 getUsedHeapSize();

	static int sTableSize;
	static TWaveBank** sWaveBank;
};

} // namespace JASystem

#endif // JASWAVEBANKMGR_HPP
