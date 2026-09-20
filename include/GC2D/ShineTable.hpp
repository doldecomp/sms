#ifndef GC2D_SHINE_TABLE_HPP
#define GC2D_SHINE_TABLE_HPP

#include <types.h>
#include <System/FlagManager.hpp>
#include <System/StageUtil.hpp>

// TODO: the header's retail name is a guess. What the map attests is its
// scope: these file-local statics are contributed by exactly five objects,
// all of them GC2D.a (ConsoleStr, PauseMenu2, CardLoad, SelectMenu, Guide --
// marioUS.MAP:35378-35517, 35673-35692, 41576-41608, 84633-84712,
// 98005-98260), and by no object outside that library. They were previously
// in System/StageUtil.hpp, which thirteen non-GC2D TUs also include; each of
// those got 0x5c bytes of .sdata2 and 0x50 of .data that retail's objects do
// not have, 4-misaligning their double pool and remapping their @NNNN ids.
// StageUtil.cpp (System.a) contributes only the six SMS_is*/SMS_get*Stage
// functions, so the tables were never in that header.

// Yep, static data in a header. Cool. Free match%
//
// This interleaved order (main table, then its *Etc sibling) is retail's, and
// Guide.o pins it: it is the only includer where both groups survive
// dead-stripping, and its .sdata2 block is byte-identical to ours over all
// sixteen tables. ConsoleStr and PauseMenu2 look like retail grouped the eight
// main tables instead, but that is only because the *Etc tables are
// dead-stripped there, which also happens to ours -- objdiff scores them as
// harmless extras. Do not regroup them.

static const u8 scShineTableAirport[]   = { 0x56 };
static const u8 scShineTableDolpicEtc[] = { 0x6B };
static const u8 scShineTableBianco[]
    = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7 };
static const u8 scShineTableBiancoEtc[] = { 0x64, 0x8, 0x9 };
static const u8 scShineTableRicco[]
    = { 0xA, 0xB, 0xC, 0xD, 0xE, 0xF, 0x10, 0x11 };
static const u8 scShineTableRiccoEtc[] = { 0x65, 0x12, 0x13 };
static const u8 scShineTableMamma[]
    = { 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B };
static const u8 scShineTableMammaEtc[] = { 0x66, 0x1C, 0x1D };
static const u8 scShineTablePinna[]
    = { 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25 };
static const u8 scShineTablePinnaEtc[] = { 0x67, 0x26, 0x27 };
static const u8 scShineTableSirena[]
    = { 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F };
static const u8 scShineTableSirenaEtc[] = { 0x68, 0x30, 0x31 };
static const u8 scShineTableMare[]
    = { 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };
static const u8 scShineTableMareEtc[] = { 0x69, 0x3A, 0x3B };
static const u8 scShineTableMonte[]
    = { 0x3C, 0x41, 0x3E, 0x3D, 0x40, 0x3F, 0x42, 0x43 };
static const u8 scShineTableMonteEtc[] = { 0x6A, 0x44, 0x45 };

static const u8* scShineConvTable[] = {
	scShineTableAirport, nullptr,
	scShineTableBianco,  scShineTableRicco,
	scShineTableMamma,   scShineTablePinna,
	scShineTableSirena,  scShineTableMonte,
	scShineTableMare,    nullptr,
};
static const u8* scEtcShineConvTable[] = {
	nullptr,
	scShineTableDolpicEtc,
	scShineTableBiancoEtc,
	scShineTableRiccoEtc,
	scShineTableMammaEtc,
	scShineTablePinnaEtc,
	scShineTableSirenaEtc,
	scShineTableMonteEtc,
	scShineTableMareEtc,
	nullptr,
};

// Yes, these were defined in a header and marked as static instead of inline
// by mistake. This is probably the right header, I think.

#if defined(VERSION_GMSE01)
// The US message bank has no ids 8 and 9 (the Japanese build's two extra
// Delfino Plaza lines), so every group after the first shifts down by two.
static u32 scScenarioNameTable[] = {
	0x0,  0x1,  0x2,  0x3,  0x4,  0x5,  0x6,  0x7,  0xA,  0xB,  0x34, 0x35,
	0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0xC,  0xD,  0xE,  0xF,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
	0x30, 0x31, 0x32, 0x33, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45,
	0x46, 0x47, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	0x20, 0x23, 0x22, 0x25, 0x24, 0x21, 0x26, 0x27, 0x28, 0x29,
};
#else
static u32 scScenarioNameTable[] = {
	0x0,  0x1,  0x2,  0x3,  0x4,  0x5,  0x6,  0x7,  0x8,  0x9,  0x32, 0x33,
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0xA,  0xB,  0xC,  0xD,
	0xE,  0xF,  0x10, 0x11, 0x12, 0x13, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,
	0x2E, 0x2F, 0x30, 0x31, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43,
	0x44, 0x45, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
	0x1E, 0x21, 0x20, 0x23, 0x22, 0x1F, 0x24, 0x25, 0x26, 0x27,
};
#endif

// The ten normal stage ids in menu order. Linked in SelectMenu.cpp and
// Guide.cpp and UNUSED (0x28) in PauseMenu2.cpp, CardLoad.cpp and
// ConsoleStr.cpp, and its .data slot sits right after scScenarioNameTable in
// both objects that keep it, so it is declared here and next to it.
//
// Ruled out: making this and scScenarioNameTable function-local statics of
// SMS_getNormalStage so that the objects that never call it would not carry
// them. The map's .data layout lists both under `GC2D.a CardLoad.cpp` as
// UNUSED at their full 0x118/0x28 sizes, i.e. retail's CardLoad.o does emit
// them and the linker strips them; a function-local static would also have
// been named `scScenarioNameTable$NNNN`, and it is not.
static u32 scNormalStageTable[] = {
	0x0, 0x1, 0x2, 0x3, 0x4, 0xD, 0x6, 0x8, 0x9, 0xA,
};

// Despite the name this maps a shine id to a scenario message index, which is
// what all four call sites want (SMSGetMessageData(mScenarioBmg2, ...)).
//
// The return is `u32` -- 0x18, the map's UNUSED size -- and the narrowing the
// call sites need is written as an explicit `(u16)` on each of the seven
// SMSGetMessageData arguments. That is codegen-identical to a `u16` return at
// every site (empty changes_all) and 4 bytes shorter here, so it replaces the
// old compromise of a `u16` return at 0x1c. SMSGetMessageData itself takes a
// `u32` (SMSGetMessageData__FPvUl), so the mask is not a parameter
// conversion. A `u16` table would drop the mask altogether, but the .data
// object is 280 bytes for 70 entries, i.e. u32.
static u32 SMS_getNormalStage(u32 param_1)
{
	return scScenarioNameTable[param_1];
}

static s16 SMS_getShineID(u32 stage, u32 scenario, bool is_etc_shine)
{
	const u8* puVar1;

	if (stage > 9)
		return -1;

	if (is_etc_shine)
		puVar1 = scEtcShineConvTable[stage];
	else
		puVar1 = scShineConvTable[stage];

	if (puVar1 == nullptr)
		return -1;

	return puVar1[scenario];
}

// size matches
static bool SMS_isGetShine(u32 stage, u32 scenario, bool is_etc_shine)
{
	s16 shineId = SMS_getShineID(stage, scenario, is_etc_shine);
	if (shineId == -1)
		return false;
	return TFlagManager::getInstance()->getShineFlag(shineId);
}

#endif
