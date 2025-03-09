#ifndef JASDSPBUF_HPP
#define JASDSPBUF_HPP

#include <dolphin/types.h>

namespace JASystem {

namespace DSPBuf {
	enum DSPBUF_EVENTS {};

	void process(DSPBUF_EVENTS event);
	void mixDSP(s32 param);
	void finishDSPFrame();
	void updateDSP();
}; // namespace DSPBuf

} // namespace JASystem

#endif // JASDSPBUF_HPP
