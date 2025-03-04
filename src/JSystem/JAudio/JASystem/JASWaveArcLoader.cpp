#include "JSystem/JAudio/JASystem/JASWaveArcLoader.hpp"

namespace JASystem {

const char* WaveArcLoader::sCurrentDir = "/Banks/";
void* WaveArcLoader::sAramHeap         = 0;

void WaveArcLoader::init() { }

void WaveArcLoader::init(u32 param) { }

void WaveArcLoader::setCurrentDir(const char* dir) { }

const char* WaveArcLoader::getCurrentDir() { return 0; }

void WaveArcLoader::loadWave(TObject* obj) { }

void WaveArcLoader::eraseWave(TObject* obj) { }

void* WaveArcLoader::getRootHeap() { return 0; }

} // namespace JASystem
