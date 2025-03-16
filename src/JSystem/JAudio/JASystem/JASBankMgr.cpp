#include <JSystem/JAudio/JASystem/JASBankMgr.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JAudio/JASystem/JASBNKParser.hpp>
#include <JSystem/JAudio/JASystem/JASWaveBankMgr.hpp>

namespace JASystem {
namespace BankMgr {

	u32 OSC_RELEASE_TABLE[3] = { 0x1000a, 0xf, 0x0 };
	// OSC_ENV is 100% f32...
	// There should also be a 803aad88, which is a reference to
	// OSC_RELEASE_TABLE?
	f32 OSC_ENV[6]     = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	s32 sTableSize     = 0;
	TBank** sBankArray = nullptr;
	u16* sVir2PhyTable = nullptr;

	void init(int bankCount)
	{
		sBankArray    = new (JASDram, 0) TBank*[bankCount];
		sVir2PhyTable = new (JASDram, 0) u16[bankCount];
		Calc::bzero(sBankArray, bankCount * sizeof(TBank*));

		for (int i = 0; i < bankCount; ++i)
			sVir2PhyTable[i] = -1;

		sTableSize = bankCount;
	}

	bool registBank(int bankIndex, TBank* bank)
	{
		sBankArray[bankIndex] = bank;
		return true;
	}

	bool registBankBNK(int bankIndex, void* bankData)
	{
		setVir2PhyTable(*((u32*)(bankData) + 2), bankIndex);
		TBasicBank* bank = BNKParser::createBasicBank(bankData);
		if (bank == nullptr)
			return false;

		return registBank(bankIndex, bank);
	}

	TBank* getBank(int bankIndex)
	{
		if (bankIndex >= sTableSize)
			return nullptr;

		return sBankArray[bankIndex];
	}

	u16 getPhysicalNumber(u16 virtualNumber)
	{
		return sVir2PhyTable[virtualNumber];
	}

	void setVir2PhyTable(u32 tableAddr, int size)
	{
		if (tableAddr == 0xFFFF)
			return;

		sVir2PhyTable[tableAddr] = size;
	}

	bool assignWaveBank(int bankIndex, int waveBankIndex)
	{
		TBank* bank = getBank(bankIndex);
		if (!bank)
			return false;

		TWaveBank* waveBank = WaveBankMgr::getWaveBank(waveBankIndex);
		if (!waveBank)
			return false;

		bank->unk4 = waveBank;
		return true;
	}

	f32 clamp01(f32 value) { return 0.0f; }

	TChannel* noteOn(TChannelMgr* channelMgr, int bankIndex, int programIndex,
	                 u8 note, u8 velocity, u32 params)
	{
	}

	static void noteOnOsc(TChannelMgr* channelMgr, int bankIndex, u8 note,
	                      u8 velocity, u32 params)
	{
	}

	void gateOn(TChannel* channel, u8 note, u8 velocity, u32 params) { }

	u32 getUsedHeapSize() { return 0; }

} // namespace BankMgr
} // namespace JASystem
