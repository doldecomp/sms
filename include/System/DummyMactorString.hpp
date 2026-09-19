#ifndef SYSTEM_DUMMY_MACTOR_STRING_HPP
#define SYSTEM_DUMMY_MACTOR_STRING_HPP

// A 12-byte zero object that opens the .rodata blob of 186 retail TUs. It used
// to sit in System/DummyStrings.hpp next to the "メモリが足りません" message,
// but they are two headers: MarioUtil/MtxUtil, MarioUtil/LightUtil and
// System/MarDirector carry this object (@1490) with no memory message anywhere
// in their .rodata, and in every TU carrying both the two ids are 620 apart
// (@1490 against @2110 in MarDirectorSetupObjects) rather than adjacent.
// Scanning retail's .rodata for a leading 12-zero object finds those 186 TUs
// and only those three lack the message, so the message's header includes this
// one and never the reverse.
//
// This object is also separate from the MActor mtx-calc type names in
// M3DUtil/InfectiousStrings.hpp. Scanning the retail objects for the literals:
// 142 carry both sets, 40 carry only this family, and none carry only the mtx
// calc names -- a strict superset, so this family lives in much more widely
// included headers and the mtx calc one pulls them in.
//
// TODO: still no idea what header this actually was.

static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";

#endif
