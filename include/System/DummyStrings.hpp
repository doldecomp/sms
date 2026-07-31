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

static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

#endif
