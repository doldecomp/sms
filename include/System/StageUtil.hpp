#ifndef STAGEUTIL_HPP
#define STAGEUTIL_HPP

#include "dolphin/types.h"

u8 SMS_getShineIDofExStage(u8);
u8 SMS_getShineStage(u8);
bool SMS_isOptionMap();
bool SMS_isDivingMap();
bool SMS_isMultiPlayerMap();
bool SMS_isExMap();
bool SMS_isExMap(u8); // UNUSED

#endif
