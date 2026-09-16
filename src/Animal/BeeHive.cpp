#include <Animal/BeeHive.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: no nerve body below is reconstructed; each carries its map size.
// Defining them emits theNerve() and the destructor, both compiler-generated.

// TODO: incorrect size. Map records 912 bytes.
DEFINE_NERVE(TNerveBeeHiveAttack, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 660 bytes.
DEFINE_NERVE(TNerveBeeHiveBreak, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 644 bytes.
DEFINE_NERVE(TNerveBeeHiveFall, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 748 bytes.
DEFINE_NERVE(TNerveBeeHiveMarioWaterIn, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 336 bytes.
DEFINE_NERVE(TNerveBeeHiveReset, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 640 bytes.
DEFINE_NERVE(TNerveBeeHiveWait, TLiveActor) { return FALSE; }
