#ifndef SYSTEM_DUMMY_STRINGS_HPP
#define SYSTEM_DUMMY_STRINGS_HPP

// Every retail TU carrying this message also carries the zero object in
// System/DummyMactorString.hpp, and always ahead of it, so that header comes
// first here; see it for the evidence that the two are separate headers.
//
// TODO: still no idea what header this actually was.
#include <System/DummyMactorString.hpp>

static const char* SMS_NO_MEMORY_MESSAGE = "メモリが足りません\n";

// The `cDirtyFileName`/`cDirtyTexName` pollution-texture pair is a third
// member of this family, `(object,local)` in twenty retail TUs (MarNameRefGen,
// MarNameRefGen_BossEnemy, MarDirectorPreEntry, emario, enemyMario, cameragc,
// MapObjFence and thirteen Player TUs, four of them dead-stripped). Its header
// is also unidentified, and intersecting the include closures of all twenty
// yields no game header at all, so it is a rogue-include carrier like these
// two rather than a natural dependency. What the .rodata blobs do pin down:
// the pair always follows this pair, and its position relative to the four
// M3DUtil/InfectiousStrings.hpp mtx-calc names varies per TU (after them in
// cameragc, MapObjFence and MarNameRefGen; before them in
// MarNameRefGen_BossEnemy; MarioCap has no mtx-calc names at all), and
// MapObjBall carries the mtx-calc names without the pair. So the pair's header
// includes this one and is independent of InfectiousStrings.hpp.

#endif
