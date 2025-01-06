#ifndef METROTRK_PORTABLE_MSGHNDLR_H
#define METROTRK_PORTABLE_MSGHNDLR_H

#include "PowerPC_EABI_Support/MetroTRK/trk.h"

void SetTRKConnected(BOOL);
BOOL GetTRKConnected(void);
DSError TRKDoSetOption(TRKBuffer*);
DSError TRKDoStop(TRKBuffer*);
DSError TRKDoStep(TRKBuffer*);
DSError TRKDoContinue(TRKBuffer*);
DSError TRKDoWriteRegisters(TRKBuffer*);
DSError TRKDoReadRegisters(TRKBuffer*);
DSError TRKDoWriteMemory(TRKBuffer*);
DSError TRKDoReadMemory(TRKBuffer*);
DSError TRKDoSupportMask(TRKBuffer*);
DSError TRKDoVersions(TRKBuffer*);
DSError TRKDoOverride(TRKBuffer*);
DSError TRKDoReset(TRKBuffer*);
DSError TRKDoDisconnect(TRKBuffer*);
DSError TRKDoConnect(TRKBuffer*);

#endif /* METROTRK_PORTABLE_MSGHNDLR_H */
