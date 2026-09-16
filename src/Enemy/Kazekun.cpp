#include <Enemy/KazekunNerve.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 280 bytes.
DEFINE_NERVE(TNerveKazekunAppear, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 1408 bytes.
DEFINE_NERVE(TNerveKazekunAttack, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 316 bytes.
DEFINE_NERVE(TNerveKazekunDisappear, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 356 bytes.
DEFINE_NERVE(TNerveKazekunHitWater, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 568 bytes.
DEFINE_NERVE(TNerveKazekunPreAttack, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 268 bytes.
DEFINE_NERVE(TNerveKazekunSearch, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 568 bytes.
DEFINE_NERVE(TNerveKazekunTurn, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 236 bytes.
DEFINE_NERVE(TNerveKazekunWait, TLiveActor) { return FALSE; }
