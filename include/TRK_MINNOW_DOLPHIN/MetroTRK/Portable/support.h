#ifndef METROTRK_PORTABLE_SUPPORT_H
#define METROTRK_PORTABLE_SUPPORT_H

#include "PowerPC_EABI_Support/MetroTRK/trk.h"

#ifdef __cplusplus
extern "C" {
#endif

DSError TRKRequestSend(TRKBuffer* msgBuf, int* bufferId, u32 p1, u32 p2,
                       int p3);

#ifdef __cplusplus
}
#endif

#endif /* METROTRK_PORTABLE_SUPPORT_H */
