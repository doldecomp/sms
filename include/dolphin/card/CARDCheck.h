#ifndef _DOLPHIN_CARDCHECK_H_
#define _DOLPHIN_CARDCHECK_H_

#include <types.h>

s32 CARDCheckAsync(s32 chan, CARDCallback callback);
long CARDCheck(long chan);

#endif // _DOLPHIN_CARDCHECK_H_
