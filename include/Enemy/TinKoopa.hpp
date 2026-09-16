#ifndef ENEMY_TINKOOPA_HPP
#define ENEMY_TINKOOPA_HPP

#include <Strategic/Nerve.hpp>

class TLiveActor;

DECLARE_NERVE(TNerveTinKoopaBreak, TLiveActor)
DECLARE_NERVE(TNerveTinKoopaDamage, TLiveActor)
DECLARE_NERVE(TNerveTinKoopaWait, TLiveActor)

#endif
