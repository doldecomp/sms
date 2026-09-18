#include <dolphin/odemuexi/DebuggerDriver.h>

// `__declspec(weak)` (not plain `inline`, which MWCC drops when nothing in the
// TU calls it) is what makes this the only, weak symbol of odenotstub.a, so the
// real OdemuExi2 driver can override the stub at link time.
__declspec(weak) int Hu_IsStub() { return 0; }
