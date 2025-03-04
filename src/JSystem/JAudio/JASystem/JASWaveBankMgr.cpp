#include "JSystem/JAudio/JASystem/JASWaveBankMgr.hpp"

namespace JASystem {

int WaveBankMgr::sTableSize        = 0;
TWaveBank** WaveBankMgr::sWaveBank = 0;

void WaveBankMgr::init(int tableSize) { }

TWaveBank* WaveBankMgr::getWaveBank(int bankIndex) { return 0; }

void WaveBankMgr::registWaveBank(int bankIndex, TWaveBank* waveBank) { }

void WaveBankMgr::registWaveBankWS(int bankIndex, void* waveBankData) { }

void WaveBankMgr::loadWave(int bankIndex, int waveIndex) { }

void WaveBankMgr::eraseWave(int bankIndex, int waveIndex) { }

u32 WaveBankMgr::getUsedHeapSize() { return 0; }

} // namespace JASystem
