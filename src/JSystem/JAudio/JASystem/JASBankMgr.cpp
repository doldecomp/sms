#include <JSystem/JAudio/JASystem/JASBankMgr.hpp>

namespace JASystem {
namespace BankMgr {

	u32 OSC_RELEASE_TABLE[3] = { 0x1000a, 0xf, 0x0 };
	// OSC_ENV is 100% f32...
	// There should also be a 803aad88, which is a reference to
	// OSC_RELEASE_TABLE?
	f32 OSC_ENV[6]     = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	u32 sTableSize     = 0;
	TBank* sBankArray  = nullptr;
	u16* sVir2PhyTable = nullptr;

	void init(int bankCount) { }

	void registBank(int bankIndex, TBank* bank) { }

	void registBankBNK(int bankIndex, void* bankData) { }

	TBank* getBank(int bankIndex) { return nullptr; }

	u16 getPhysicalNumber(u16 virtualNumber) { return 0; }

	void setVir2PhyTable(u32 tableAddr, int size) { }

	void assignWaveBank(int bankIndex, int waveBankIndex) { }

	f32 clamp01(f32 value) { return 0.0f; }

	void noteOn(TChannelMgr* channelMgr, int bankIndex, int programIndex,
	            u8 note, u8 velocity, u32 params)
	{
	}

	void noteOnOsc(TChannelMgr* channelMgr, int bankIndex, u8 note, u8 velocity,
	               u32 params)
	{
	}

	void gateOn(TChannel* channel, u8 note, u8 velocity, u32 params) { }

	u32 getUsedHeapSize() { return 0; }

} // namespace BankMgr
} // namespace JASystem
