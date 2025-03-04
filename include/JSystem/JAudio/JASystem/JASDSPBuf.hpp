#ifndef JASDSPBUF_HPP
#define JASDSPBUF_HPP

#include <dolphin/types.h>

namespace JASystem {

class DSPBuf {
public:
	enum DSPBUF_EVENTS {
		// Enum values would be defined here
	};

	static void process(DSPBUF_EVENTS event);
	static DSPBuf* dsp_buf;
	static void* write_buffer;
	static void* read_buffer;
	static u16 dspstatus;
	static int dac_sync_counter;
	static void mixDSP(s32 param);
	static void finishDSPFrame();
	static void updateDSP();
};

} // namespace JASystem

#endif // JASDSPBUF_HPP
