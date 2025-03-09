#include <JSystem/JAudio/JASystem/JASDSPBuf.hpp>

namespace JASystem {

namespace DSPBuf {
	static u8 dsp_buf[0xC];

	static u8 write_buffer      = 0;
	static u8 read_buffer       = 0;
	static u8 dspstatus         = 0;
	static int dac_sync_counter = 0;
} // namespace DSPBuf

void DSPBuf::process(DSPBUF_EVENTS event) { }

void DSPBuf::mixDSP(s32 param) { }

void DSPBuf::finishDSPFrame() { }

void DSPBuf::updateDSP() { }

} // namespace JASystem
