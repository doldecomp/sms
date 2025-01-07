#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/serpoll.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/nubevent.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msgbuf.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msghndlr.h"
#include "TRK_MINNOW_DOLPHIN/Os/dolphin/dolphin_trk_glue.h"
#include "TRK_MINNOW_DOLPHIN/utils/common/MWTrace.h"
#include "PowerPC_EABI_Support/MetroTRK/trk.h"

static TRKFramingState gTRKFramingState;

void* gTRKInputPendingPtr;

MessageBufferID TRKTestForPacket()
{
	int bytes;
	int batch;
	int err;
	TRKBuffer* b;
	int id;

	bytes = TRKPollUART();
	if (bytes > 0) {
		TRKGetFreeBuffer(&id, &b);
		if (bytes > TRKMSGBUF_SIZE) {
			for (; bytes > 0; bytes -= batch) {
				batch = bytes > TRKMSGBUF_SIZE ? TRKMSGBUF_SIZE : bytes;
				TRKReadUARTN(b->data, batch);
			}
			TRKStandardACK(b, 0xff, 6);
		} else {
			err = TRKReadUARTN(b->data, bytes);
			if (err == 0) {
				b->length = bytes;
				return id;
			}
		}
	}
	if (id != -1) {
		TRKReleaseBuffer(id);
	}
	return -1;
}

void TRKGetInput(void)
{
	MessageBufferID id = TRKTestForPacket();
	if (id != -1) {
		TRKEvent event;
		TRKGetBuffer(id);
		TRKConstructEvent(&event, NUBEVENT_Request);
		event.msgBufID            = id;
		gTRKFramingState.msgBufID = -1;
		TRKPostEvent(&event);
	}
}

void TRKProcessInput(int bufferIdx)
{
	TRKEvent event;

	TRKConstructEvent(&event, NUBEVENT_Request);
	event.msgBufID            = bufferIdx;
	gTRKFramingState.msgBufID = -1;
	TRKPostEvent(&event);
}

DSError TRKInitializeSerialHandler(void)
{
	gTRKFramingState.msgBufID     = -1;
	gTRKFramingState.receiveState = DSRECV_Wait;
	gTRKFramingState.isEscape     = FALSE;

	MWTRACE(1, "TRK_Packet_Header \t    %ld bytes\n", 0x40);
	MWTRACE(1, "TRK_CMD_ReadMemory     %ld bytes\n", 0x40);
	MWTRACE(1, "TRK_CMD_WriteMemory    %ld bytes\n", 0x40);
	MWTRACE(1, "TRK_CMD_Connect \t    %ld bytes\n", 0x40);
	MWTRACE(1, "TRK_CMD_ReplyAck\t    %ld bytes\n", 0x40);
	MWTRACE(1, "TRK_CMD_ReadRegisters\t%ld bytes\n", 0x40);

	return DS_NoError;
}

DSError TRKTerminateSerialHandler(void) { return DS_NoError; }
