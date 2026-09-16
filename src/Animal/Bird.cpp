#include <Animal/BirdNerve.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 700 bytes.
DEFINE_NERVE(TNerveAnimalBirdActionOnGround, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 312 bytes.
DEFINE_NERVE(TNerveAnimalBirdChangeToCoin, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 700 bytes.
DEFINE_NERVE(TNerveAnimalBirdComeback, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 800 bytes.
DEFINE_NERVE(TNerveAnimalBirdGraphWander, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 692 bytes.
DEFINE_NERVE(TNerveAnimalBirdLanding, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 624 bytes.
DEFINE_NERVE(TNerveAnimalBirdPreLanding, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 368 bytes.
DEFINE_NERVE(TNerveAnimalBirdTakeoff, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 680 bytes.
DEFINE_NERVE(TNerveAnimalBirdWaitOnGround, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 896 bytes.
DEFINE_NERVE(TNerveAnimalBirdWalkOnGround, TLiveActor) { return FALSE; }
