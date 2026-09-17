#ifndef SYSTEM_DUMMY_STRINGS_HPP
#define SYSTEM_DUMMY_STRINGS_HPP

// These two are separate from the MActor mtx-calc type names in
// M3DUtil/InfectiousStrings.hpp. Scanning the retail objects for the literals:
// 142 carry both sets, 40 carry only this pair, and none carry only the mtx
// calc names -- a strict superset, so this pair lives in its own, much more
// widely included header, and the mtx calc one pulls it in. TUs needing only
// this pair include GC2D/CardSave, GC2D/Option and MSound/MSound.
//
// TODO: still no idea what header this actually was.
//
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

static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

#endif
