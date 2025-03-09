#include <JSystem/JAudio/JASystem/JASWaveBankMgr.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JAudio/JASystem/JASWSParser.hpp>
#include <JSystem/JAudio/JASystem/JASWaveBank.hpp>
#include <JSystem/JAudio/JASystem/JASBasicWaveBank.hpp>
#include <JSystem/JAudio/JASystem/JASSimpleWaveBank.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>

namespace JASystem {

int WaveBankMgr::sTableSize        = 0;
TWaveBank** WaveBankMgr::sWaveBank = 0;

void WaveBankMgr::init(int tableSize)
{
	sWaveBank = new (JASDram, 0) TWaveBank*[tableSize];
	Calc::bzero(sWaveBank, tableSize * sizeof(TWaveBank*));
	sTableSize = tableSize;
}

TWaveBank* WaveBankMgr::getWaveBank(int bankIndex)
{
	return sWaveBank[bankIndex];
}

bool WaveBankMgr::registWaveBank(int bankIndex, TWaveBank* waveBank)
{
	sWaveBank[bankIndex] = waveBank;
	return true;
}

bool WaveBankMgr::registWaveBankWS(int bankIndex, void* waveBankData)
{
	TWaveBank* bank;
	if (WSParser::getGroupCount(waveBankData) == 1)
		bank = WSParser::createSimpleWaveBank(waveBankData);
	else
		bank = WSParser::createBasicWaveBank(waveBankData);

	if (!bank)
		return false;

	return registWaveBank(bankIndex, bank);
}

bool WaveBankMgr::loadWave(int bankIndex, int waveIndex)
{
	TWaveBank* bank = getWaveBank(bankIndex);
	if (!bank)
		return false;

	if (bank->getType() == 'BSIC') {
		TBasicWaveBank::TWaveGroup* group
		    = ((TBasicWaveBank*)bank)->getWaveGroup(waveIndex);
		if (!group)
			return false;

		if (!WaveArcLoader::loadWave(group))
			return false;

		((TBasicWaveBank*)bank)->incWaveTable(group);

		return true;
	} else if (bank->getType() == 'SMPL') {
		TSimpleWaveBank* casted = (TSimpleWaveBank*)bank;
		return WaveArcLoader::loadWave(casted);
	}

	return false;
}

bool WaveBankMgr::eraseWave(int bankIndex, int waveIndex)
{

	TWaveBank* bank = getWaveBank(bankIndex);
	if (!bank)
		return false;

	if (bank->getType() == 'BSIC') {
		TBasicWaveBank::TWaveGroup* group
		    = ((TBasicWaveBank*)bank)->getWaveGroup(waveIndex);
		if (!group)
			return false;

		if (!WaveArcLoader::eraseWave(group))
			return false;

		((TBasicWaveBank*)bank)->decWaveTable(group);
		return true;
	} else if (bank->getType() == 'SMPL') {
		TSimpleWaveBank* casted = (TSimpleWaveBank*)bank;
		return WaveArcLoader::eraseWave(casted);
	}

	return false;
}

u32 WaveBankMgr::getUsedHeapSize() { return 0; }

} // namespace JASystem
