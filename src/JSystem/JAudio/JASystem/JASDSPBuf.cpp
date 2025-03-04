#include <JSystem/JAudio/JASystem/JASDSPBuf.hpp>

namespace JASystem {

// I think DSPBuf is a namespace and not a class
DSPBuf* DSPBuf::dsp_buf      = 0;
void* DSPBuf::write_buffer   = 0;
void* DSPBuf::read_buffer    = 0;
u16 DSPBuf::dspstatus        = 0;
int DSPBuf::dac_sync_counter = 0;

void DSPBuf::process(DSPBUF_EVENTS event) { }

void DSPBuf::mixDSP(s32 param) { }

void DSPBuf::finishDSPFrame() { }

void DSPBuf::updateDSP() { }

} // namespace JASystem
