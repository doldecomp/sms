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

#endif
