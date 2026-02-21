#ifndef SYSTEM_STAGE_UTIL_HPP
#define SYSTEM_STAGE_UTIL_HPP

#include <types.h>
#include <System/FlagManager.hpp>

u8 SMS_getShineIDofExStage(u8);
u8 SMS_getShineStage(u8);
bool SMS_isOptionMap();
bool SMS_isDivingMap();
bool SMS_isMultiPlayerMap();
bool SMS_isExMap();
bool SMS_isExMap(u8); // UNUSED

// Yep, static data in a header. Cool. Free match%

static u8 scShineTableAirport[]   = { 0x56 };
static u8 scShineTableDolpicEtc[] = { 0 };
static u8 scShineTableBianco[]    = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7 };
static u8 scShineTableBiancoEtc[] = { 0 };
static u8 scShineTableRicco[]    = { 0xA, 0xB, 0xC, 0xD, 0xE, 0xF, 0x10, 0x11 };
static u8 scShineTableRiccoEtc[] = { 0 };
static u8 scShineTableMamma[]
    = { 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B };
static u8 scShineTableMammaEtc[] = { 0 };
static u8 scShineTablePinna[]
    = { 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25 };
static u8 scShineTablePinnaEtc[] = { 0 };
static u8 scShineTableSirena[]
    = { 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F };
static u8 scShineTableSirenaEtc[] = { 0 };
static u8 scShineTableMare[]
    = { 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };
static u8 scShineTableMareEtc[] = { 0 };
static u8 scShineTableMonte[]
    = { 0x3C, 0x41, 0x3E, 0x3D, 0x40, 0x3F, 0x42, 0x43 };
static u8 scShineTableMonteEtc[] = { 0 };

static u8* scEtcShineConvTable[] = {
	nullptr,
	scShineTableDolpicEtc,
	scShineTableBiancoEtc,
	scShineTableRiccoEtc,
	scShineTableMammaEtc,
	scShineTablePinnaEtc,
	scShineTableSirenaEtc,
	scShineTableMonteEtc,
	scShineTableMareEtc,
};
static u8* scShineConvTable[] = {
	scShineTableAirport, nullptr,           scShineTableBianco,
	scShineTableRicco,   scShineTableMamma, scShineTablePinna,
	scShineTableSirena,  scShineTableMonte, scShineTableMare,
};

// Yes, these were defined in a header and marked as static instead of inline
// by mistake. This is probably the right header, I think.

static u32 scScenarioNameTable[] = {
	0x0,  0x1,  0x2,  0x3,  0x4,  0x5,  0x6,  0x7,  0x8,  0x9,  0x32, 0x33,
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0xA,  0xB,  0xC,  0xD,
	0xE,  0xF,  0x10, 0x11, 0x12, 0x13, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,
	0x2E, 0x2F, 0x30, 0x31, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43,
	0x44, 0x45, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
	0x1E, 0x21, 0x20, 0x23, 0x22, 0x1F, 0x24, 0x25, 0x26, 0x27,
};

// size matches
static u16 SMS_getNormalStage(u32 param_1)
{
	return scScenarioNameTable[param_1];
}

static s16 SMS_getShineID(u32 stage, u32 scenario, bool is_etc_shine)
{
	u8* puVar1;

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
	s32 shineId = SMS_getShineID(stage, scenario, is_etc_shine);
	if (shineId == -1)
		return false;
	return TFlagManager::getInstance()->getShineFlag(shineId);
}

#endif
