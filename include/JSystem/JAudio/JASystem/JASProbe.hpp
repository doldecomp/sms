#include "types.h"

namespace JASystem {
namespace Kernel {
	void probeStart(s32, char*) { return; }
	void probeFinish(s32) { return; }
	void switchProbe(u32, s32);
	void resetProbe();
	void getProbeName(s32);
	void getProbeLast(s32);
	void getProbeAvg(s32);
	void getProbeTotalAvg(s32);
	void getProbeTotal(s32);
	void getProbeMax(s32);
	void Console_printf(char*, ...);
} // namespace Kernel
} // namespace JASystem
