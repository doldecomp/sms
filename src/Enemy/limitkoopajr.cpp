#include <Enemy/LimitKoopaJr.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 308 bytes.
DEFINE_NERVE(TNerveLimitKoopaJrLaunch, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 424 bytes.
DEFINE_NERVE(TNerveLimitKoopaJrRun, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 884 bytes.
DEFINE_NERVE(TNerveLimitKoopaJrWait, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 280 bytes.
DEFINE_NERVE(TNerveLimitKoopaJrYahoo, TLiveActor) { return FALSE; }
