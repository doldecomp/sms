#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/notify.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msgbuf.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/support.h"
#include "TRK_MINNOW_DOLPHIN/ppc/Generic/targimpl.h"
#include "PowerPC_EABI_Support/MetroTRK/trk.h"

inline DSError TRKDoNotifyStopped_Inline(TRKBuffer* msg, MessageCommandID cmd)
{
	if (msg->position >= 0x880) {
		return DS_MessageBufferOverflow;
	} else {
		msg->data[msg->position++] = cmd;
		msg->length += 1;
		return DS_NoError;
	}
}

DSError TRKDoNotifyStopped(MessageCommandID cmd)
{
	DSError err;
	int reqIdx;
	TRKBuffer* msg;
	int bufIdx;

	err = TRKGetFreeBuffer(&bufIdx, &msg);
	if (err == DS_NoError) {
		err = TRKDoNotifyStopped_Inline(msg, cmd);

		if (err == DS_NoError) {
			if (cmd == DSMSG_NotifyStopped) {
				TRKTargetAddStopInfo(msg);
			} else {
				TRKTargetAddExceptionInfo(msg);
			}
		}

		err = TRKRequestSend(msg, &reqIdx, 2, 3, 1);
		if (err == DS_NoError) {
			TRKReleaseBuffer(reqIdx);
		}
		TRKReleaseBuffer(bufIdx);
	}

	return err;
}
