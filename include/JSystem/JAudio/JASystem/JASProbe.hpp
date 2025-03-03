#include "types.h"

namespace JASystem {
namespace Kernel {
	void probeStart(long, char*) { return; }
	void probeFinish(long) { return; }
	void switchProbe(unsigned long, long);
	void resetProbe();
	void getProbeName(long);
	void getProbeLast(long);
	void getProbeAvg(long);
	void getProbeTotalAvg(long);
	void getProbeTotal(long);
	void getProbeMax(long);
	void Console_printf(char*, ...);
} // namespace Kernel
} // namespace JASystem
