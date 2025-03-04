#include "types.h"

namespace JASystem {
namespace Kernel {
	void probeStart(long, char*) { return; }
	void probeFinish(long) { return; }
	void switchProbe(u32, s32) { return; }
	void resetProbe() { return; }
	void getProbeName(s32) { return; }
	void getProbeLast(s32) { return; }
	void getProbeAvg(s32) { return; }
	void getProbeTotalAvg(s32) { return; }
	void getProbeTotal(s32) { return; }
	void getProbeMax(s32) { return; }
	void Console_printf(char*, ...) { return; }
} // namespace Kernel
} // namespace JASystem
