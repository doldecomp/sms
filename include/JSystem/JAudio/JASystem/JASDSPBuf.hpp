#ifndef JASDSPBUF_HPP
#define JASDSPBUF_HPP

#include <dolphin/types.h>

namespace JASystem {

class DSPBuf {
public:
	enum DSPBUF_EVENTS {
	};

	void process(DSPBUF_EVENTS event);
	static DSPBuf* dsp_buf;
	static void* write_buffer;
	static void* read_buffer;
	static u16 dspstatus;
	static int dac_sync_counter;
	void mixDSP(s32 param);
	void finishDSPFrame();
	void updateDSP();
};

} // namespace JASystem

#endif // JASDSPBUF_HPP
