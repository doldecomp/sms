#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/notify.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msgbuf.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/support.h"
#include "TRK_MINNOW_DOLPHIN/ppc/Generic/targimpl.h"
#include "PowerPC_EABI_Support/MetroTRK/trk.h"

DSError TRKDoNotifyStopped(MessageCommandID cmd)
{
	int reqIdx;
	int bufIdx;
	TRKBuffer* msg;
	DSError err;
	DSError bufError;

	bufError = TRKGetFreeBuffer(&bufIdx, &msg);
	if ((err = bufError) == FALSE) {
		if (err == DS_NoError) {
			if (cmd == DSMSG_NotifyStopped) {
				TRKTargetAddStopInfo(msg);
			} else {
				TRKTargetAddExceptionInfo(msg);
			}
		}
		bufError = TRKRequestSend(msg, &reqIdx, 2, 3, 1);
		err      = bufError;
		if (err == DS_NoError) {
			TRKReleaseBuffer(reqIdx);
		}
		TRKReleaseBuffer(bufIdx);
	}
	return err;
}
