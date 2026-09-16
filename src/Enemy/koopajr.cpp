#include <Enemy/KoopaJr.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 156 bytes.
DEFINE_NERVE(TNerveKoopaJrDamage, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 116 bytes.
DEFINE_NERVE(TNerveKoopaJrDemo, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 144 bytes.
DEFINE_NERVE(TNerveKoopaJrLaunch, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 104 bytes.
DEFINE_NERVE(TNerveKoopaJrSubmarineCannonOpenClose, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 100 bytes.
DEFINE_NERVE(TNerveKoopaJrSubmarineLaunchKiller, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 136 bytes.
DEFINE_NERVE(TNerveKoopaJrSubmarineWait, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 172 bytes.
DEFINE_NERVE(TNerveKoopaJrWait, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 144 bytes.
DEFINE_NERVE(TNerveKoopaJrYahoo, TLiveActor) { return FALSE; }
