#ifndef JASBANKMGR_HPP
#define JASBANKMGR_HPP

#include <JSystem/JAudio/JASystem/JASBank.hpp>

namespace JASystem {

class TChannel;
class TChannelMgr;

namespace BankMgr {
	// Functions
	void init(int);
	void registBank(int, TBank*);
	void registBankBNK(int, void*);
	TBank* getBank(int);
	unsigned short getPhysicalNumber(unsigned short);
	void setVir2PhyTable(unsigned long, int);
	void assignWaveBank(int, int);
	float clamp01(float);
	void noteOn(TChannelMgr*, int, int, unsigned char, unsigned char,
	            unsigned long);
	void noteOnOsc(TChannelMgr*, int, unsigned char, unsigned char,
	               unsigned long);
	void gateOn(TChannel*, unsigned char, unsigned char, unsigned long);
	unsigned long getUsedHeapSize();

	// Variables
	extern float OSC_ENV[];
	extern float OSC_RELEASE_TABLE[];
	extern int sTableSize;
	extern TBank* sBankArray[];
	extern unsigned short sVir2PhyTable[];
} // namespace BankMgr

} // namespace JASystem

#endif // JASBANKMGR_HPP
