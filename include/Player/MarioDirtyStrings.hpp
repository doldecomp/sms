#ifndef PLAYER_MARIO_DIRTY_STRINGS_HPP
#define PLAYER_MARIO_DIRTY_STRINGS_HPP

// The graffiti texture Mario's body, cap, tongue and the enemy Marios swap in
// through SMS_ChangeTextureAll. Retail carries both names as `(object,local)`
// in twenty TUs -- thirteen Player ones, MarNameRefGen, MarNameRefGen_BossEnemy,
// MarDirectorPreEntry, emario, enemyMario, cameragc and MapObjFence -- plus
// four more where the linker dead-stripped them (MapWarp, MarioCollision,
// MarioMove, MarioReceiveMsg), so they came from a header rather than from
// each .cpp.
//
// TODO: the header's real name is still unknown. Intersecting the include
// closures of the twenty carriers yields no game header at all, so like
// System/DummyStrings.hpp and M3DUtil/InfectiousStrings.hpp this is a
// rogue-include carrier rather than a natural dependency, and it is included
// at the position each TU's .rodata wants it: the pair always follows the
// DummyStrings pair, while its position relative to InfectiousStrings.hpp's
// four mtx-calc names varies per TU (after them in cameragc, emario,
// MapObjFence and MarNameRefGen, before them in MarNameRefGen_BossEnemy and
// enemyMario, and MarioCap has no mtx-calc names at all). The four
// dead-stripped TUs deliberately do not include it: their copies are absent
// from the extracted target objects, so emitting them would only add data the
// comparison cannot match.
//
// `const` at namespace scope already gives internal linkage in C++, which is
// what the map's `local` scope wants; the `static` is the ROM's own spelling
// in the TUs that kept the pair alive.

static const char cDirtyFileName[] = "/scene/map/pollution/H_ma_rak.bti";
static const char cDirtyTexName[]  = "H_ma_rak_dummy";

#endif
