#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/support.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msgbuf.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msg.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/serpoll.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msghndlr.h"
#include "stddef.h"
#include "string.h"

/* TODO: 179 of 182 instructions and the 0x48 frame match; the residue is a
   straight swap of two callee-saved registers over 13 operands. Retail keeps
   `done` in r28 and `replyBuffer` in r29, we use r29 and r28. Everything else
   -- the r21 error, r24 exit, r31 length, the stack homes of `buffer`/
   `bufferId` at 0x10/0x14 (they are address-taken by TRKGetFreeBuffer) -- is
   identical, so this is purely how the allocator ranks the loop-long `done`
   against the reply-only `replyBuffer`.
   Declaration order is a lever but no order fixes it, and batch 145 exhausted
   it: all 120 orders of the five register-held locals (`done`, `exit`,
   `error`, `replyBuffer`, `length`; the other five declarations are
   address-taken and stack-homed) were built, and the order below is the unique
   minimum at 12 differing operands -- every other order gives 19, 20, 23, 27
   or 28. Do not re-sweep it. Declaration order only ranks function-scope
   locals against each other, which is why it reorders this pair but never
   reaches retail's ranking (docs/catalog/frame-gaps.md, "batch 145").
   Codegen-neutral: dropping the `(TRKBuffer*)` cast on TRKGetBuffer's `void*`,
   `done = done + length`, a named `remain` for `*count - done`, and extra
   parentheses in the `while`. Worse: reordering the `while` conditions (19) and
   writing the length clamp as a ternary (26).
   Unit round 2026-09-23: `int done` is identical; `replyBuffer` (or all
   four reply locals) scoped to the `need_reply` block is worse (27/39). */
/* Lib pass 2026-09-23: a `u8 replyIOResult` read without the cast is
   identical; the error/replyBufferId/.../exit declaration order is worse (41). */
DSError TRKSuppAccessFile(u32 file_handle, u8* data, size_t* count,
                          DSIOResult* io_result, BOOL need_reply, BOOL read)
{
	u32 done;
	BOOL exit;
	DSError error;
	TRKBuffer* replyBuffer;
	int replyBufferId;
	TRKBuffer* buffer;
	u32 length;
	int bufferId;
	u16 replyLength;
	DSIOResult replyIOResult;

	if (data == NULL || *count == 0) {
		return DS_ParameterError;
	}

	exit       = FALSE;
	*io_result = DS_IONoError;
	done       = 0;
	error      = DS_NoError;
	while (!exit && done < *count && error == DS_NoError
	       && *io_result == DS_IONoError) {
		if (*count - done > 0x800) {
			length = 0x800;
		} else {
			length = *count - done;
		}

		error = TRKGetFreeBuffer(&bufferId, &buffer);

		if (error == DS_NoError)
			error = TRKAppendBuffer1_ui8(buffer, read ? DSMSG_ReadFile
			                                          : DSMSG_WriteFile);

		if (error == DS_NoError)
			error = TRKAppendBuffer1_ui32(buffer, file_handle);

		if (error == DS_NoError)
			error = TRKAppendBuffer1_ui16(buffer, length);

		if (!read && error == DS_NoError)
			error = TRKAppendBuffer_ui8(buffer, data + done, length);

		if (error == DS_NoError) {
			if (need_reply) {
				replyLength   = 0;
				replyIOResult = DS_IONoError;

				error = TRKRequestSend(buffer, &replyBufferId, read ? 5 : 5, 3,
				                       !(read && file_handle == 0));
				if (error == DS_NoError) {
					replyBuffer = (TRKBuffer*)TRKGetBuffer(replyBufferId);
					TRKSetBufferPosition(replyBuffer, 2);
				}

				if (error == DS_NoError)
					error
					    = TRKReadBuffer1_ui8(replyBuffer, (u8*)&replyIOResult);

				if (error == DS_NoError)
					error = TRKReadBuffer1_ui16(replyBuffer, &replyLength);

				if (read && error == DS_NoError) {
					if (replyBuffer->length != replyLength + 5) {
						replyLength = replyBuffer->length - 5;
						if (replyIOResult == DS_IONoError)
							replyIOResult = DS_IOError;
					}

					if (replyLength <= length)
						error = TRKReadBuffer_ui8(replyBuffer, data + done,
						                          replyLength);
				}

				if (replyLength != length) {
					if ((!read || replyLength >= length)
					    && replyIOResult == DS_IONoError)
						replyIOResult = DS_IOError;
					length = replyLength;
					exit   = TRUE;
				}

				*io_result = replyIOResult;
				TRKReleaseBuffer(replyBufferId);
			} else {
				error = TRKMessageSend((TRK_Msg*)buffer);
			}
		}

		TRKReleaseBuffer(bufferId);
		done += length;
	}

	*count = done;
	return error;
}

DSError TRKRequestSend(TRKBuffer* msgBuf, int* bufferId, u32 p1, u32 p2, int p3)
{
	int error = DS_NoError;
	TRKBuffer* buffer;
	u32 timer;
	int tries;
	u8 msg_error;
	u8 msg_command;
	BOOL badReply = TRUE;

	*bufferId = -1;

	for (tries = p2 + 1; tries != 0 && *bufferId == -1 && error == DS_NoError;
	     tries--) {
		error = TRKMessageSend((TRK_Msg*)msgBuf);
		if (error == DS_NoError) {
			if (p3) {
				timer = 0;
			}

			while (TRUE) {
				do {
					*bufferId = TRKTestForPacket();
					if (*bufferId != -1)
						break;
				} while (!p3 || ++timer < 79999980);

				if (*bufferId == -1)
					break;

				badReply = FALSE;

				buffer = TRKGetBuffer(*bufferId);
				TRKSetBufferPosition(buffer, 0);

				if ((error = TRKReadBuffer1_ui8(buffer, &msg_command))
				    != DS_NoError)
					break;

				if (msg_command >= DSMSG_ReplyACK)
					break;

				TRKProcessInput(*bufferId);
				*bufferId = -1;
			}

			if (*bufferId != -1) {
				if (buffer->length < p1) {
					badReply = TRUE;
				}
				if (error == DS_NoError && !badReply) {
					error = TRKReadBuffer1_ui8(buffer, &msg_error);
				}
				if (error == DS_NoError && !badReply) {
					if (msg_command != DSMSG_ReplyACK
					    || msg_error != DSREPLY_NoError) {
						badReply = TRUE;
					}
				}
				if (error != DS_NoError || badReply) {
					TRKReleaseBuffer(*bufferId);
					*bufferId = -1;
				}
			}
		}
	}

	if (*bufferId == -1) {
		error = DS_Error800;
	}

	return error;
}
