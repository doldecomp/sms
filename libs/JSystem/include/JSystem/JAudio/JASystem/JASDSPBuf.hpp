#ifndef JASDSPBUF_HPP
#define JASDSPBUF_HPP

#include <dolphin/types.h>

namespace JASystem {

namespace DSPBuf {
	enum DSPBUF_EVENTS {
		DSPBUF_EVENTS_UNK0,
		DSPBUF_EVENTS_UNK1,
		DSPBUF_EVENTS_UNK2,
	};

	s16* process(DSPBUF_EVENTS event);
	s16* mixDSP(s32 param);
	void finishDSPFrame();
	void updateDSP();
}; // namespace DSPBuf

} // namespace JASystem

#endif // JASDSPBUF_HPP
