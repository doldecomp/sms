#ifndef JASBANKMGR_HPP
#define JASBANKMGR_HPP

#include <JSystem/JAudio/JASystem/JASBank.hpp>
#include <dolphin/types.h>

namespace JASystem {

class TChannel;
class TChannelMgr;

namespace BankMgr {
	void init(int bankCount);
	bool registBank(int bankIndex, TBank* bank);
	bool registBankBNK(int bankIndex, void* bankData);
	TBank* getBank(int bankIndex);
	u16 getPhysicalNumber(u16 virtualNumber);
	void setVir2PhyTable(u32 vir_id, int banknum);
	bool assignWaveBank(int bankIndex, int waveBankIndex);
	f32 clamp01(f32 value);
	TChannel* noteOn(TChannelMgr* chan_mgr, int bank_no, int prog_no, u8 key,
	                 u8 velocity, u32 gate_time);
	void gateOn(TChannel* channel, u8 key, u8 velocity, u32 gate_time);
	u32 getUsedHeapSize();

} // namespace BankMgr

} // namespace JASystem

#endif // JASBANKMGR_HPP
