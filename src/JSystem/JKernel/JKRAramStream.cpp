#include <JSystem/JKernel/JKRAramStream.hpp>
#include <JSystem/JKernel/JKRAramPiece.hpp>
#include <JSystem/JSupport/JSUFileInputStream.hpp>
#include <dolphin/os.h>
#include <macros.h>

JKRAramStream* JKRAramStream::sAramStreamObject;

JKRAramStream* JKRAramStream::create(s32 priority)
{
	if (!sAramStreamObject) {
		sAramStreamObject = new (JKRGetSystemHeap(), 0) JKRAramStream(priority);
		setTransBuffer(nullptr, 0, nullptr);
	}

	return sAramStreamObject;
}

void* JKRAramStream::sMessageBuffer[4] = { nullptr, nullptr, nullptr, nullptr };
OSMessageQueue JKRAramStream::sMessageQueue = { 0 };

JKRAramStream::JKRAramStream(s32 priority)
    : JKRThread(0x4000, 0x10, priority)
{
	resume();
}

JKRAramStream::~JKRAramStream() { }

void* JKRAramStream::run()
{
	OSInitMessageQueue(&sMessageQueue, sMessageBuffer,
	                   ARRAY_COUNT(sMessageBuffer));

	for (;;) {
		OSMessage message;
		OSReceiveMessage(&sMessageQueue, &message, OS_MESSAGE_BLOCK);
		JKRAramStreamCommand* command = (JKRAramStreamCommand*)message;

		switch (command->mType) {
		case JKRAramStreamCommand::READ:
			readFromAram();
			break;
		case JKRAramStreamCommand::WRITE:
			writeToAram(command);
			break;
		default:
			break;
		}
	}
}

s32 JKRAramStream::readFromAram() { return 1; }

s32 JKRAramStream::writeToAram(JKRAramStreamCommand* command)
{
	u32 dstSize       = command->mSize;
	u32 offset        = command->mOffset;
	u32 writtenLength = 0;
	u32 destination   = command->mAddress;
	u8* buffer        = command->mTransferBuffer;
	u32 bufferSize    = command->mTransferBufferSize;
	JKRHeap* heap     = command->mHeap;

	if (buffer) {
		bufferSize = !bufferSize ? 0x8000 : bufferSize;

		command->mTransferBufferSize      = bufferSize;
		command->mAllocatedTransferBuffer = false;
	} else {
		bufferSize = !bufferSize ? 0x8000 : bufferSize;

		if (heap) {
			buffer = (u8*)JKRAllocFromHeap(heap, bufferSize, -0x20);
			command->mTransferBuffer = buffer;
		} else {
			buffer = (u8*)JKRAllocFromHeap(nullptr, bufferSize, -0x20);
			command->mTransferBuffer = buffer;
		}

		command->mTransferBufferSize      = bufferSize;
		command->mAllocatedTransferBuffer = true;
	}

	if (!buffer) {
		if (!heap) {
			JKRGetCurrentHeap()->dump();
		} else {
			heap->dump();
		}

		OSPanic(__FILE__, 169, "abort\n");
	}

	if (buffer) {
		command->mStream->seek(offset, JSUStreamSeekFrom_SET);

		while (dstSize != 0) {
			u32 length;
			if (dstSize > bufferSize)
				length = bufferSize;
			else
				length = dstSize;

			s32 readLength = command->mStream->read(buffer, length);

			JKRAramPcs(0, (u32)buffer, destination, length, nullptr);
			dstSize -= length;
			offset += length;
			writtenLength += length;
			destination += length;
		}

		if (command->mAllocatedTransferBuffer) {
			JKRFree(buffer);
			command->mAllocatedTransferBuffer = false;
		}
	}

	OSSendMessage(&command->mMessageQueue, (OSMessage)writtenLength,
	              OS_MESSAGE_NOBLOCK);
	return writtenLength;
}

u8* JKRAramStream::transBuffer;
u32 JKRAramStream::transSize;
JKRHeap* JKRAramStream::transHeap;

JKRAramStreamCommand*
JKRAramStream::write_StreamToAram_Async(JSUFileInputStream* stream, u32 addr,
                                        u32 size, u32 offset)
{
	JKRAramStreamCommand* command
	    = new (JKRGetSystemHeap(), -4) JKRAramStreamCommand();
	command->mType               = JKRAramStreamCommand::WRITE;
	command->mAddress            = addr;
	command->mSize               = size;
	command->mStream             = stream;
	command->field_0x2c          = 0;
	command->mOffset             = offset;
	command->mTransferBuffer     = transBuffer;
	command->mHeap               = transHeap;
	command->mTransferBufferSize = transSize;

	OSInitMessageQueue(&command->mMessageQueue, &command->mMessage, 1);
	OSSendMessage(&sMessageQueue, command, OS_MESSAGE_BLOCK);
	return command;
}

/* 802B6624-802B66B8       .text sync__13JKRAramStreamFP20JKRAramStreamCommandi
 */
JKRAramStreamCommand* JKRAramStream::sync(JKRAramStreamCommand* command,
                                          int isNonBlocking)
{
	OSMessage message;
	if (isNonBlocking == 0) {
		OSReceiveMessage(&command->mMessageQueue, &message, OS_MESSAGE_BLOCK);
		if (message == nullptr) {
			command = nullptr;
			return command;
		} else {
			return command;
		}
	} else {
		BOOL receiveResult = OSReceiveMessage(&command->mMessageQueue, &message,
		                                      OS_MESSAGE_NOBLOCK);
		if (receiveResult == FALSE) {
			command = nullptr;
			return command;
		} else if (message == nullptr) {
			command = nullptr;
			return command;
		} else {
			return command;
		}
	}
}

void JKRAramStream::setTransBuffer(u8* buffer, u32 bufferSize, JKRHeap* heap)
{
	transBuffer = nullptr;
	transSize   = 0x8000;
	transHeap   = nullptr;

	if (buffer) {
		transBuffer = (u8*)ALIGN_NEXT((u32)buffer, 0x20);
	}

	if (bufferSize) {
		transSize = ALIGN_PREV(bufferSize, 0x20);
	}

	if (heap && !buffer) {
		transHeap = heap;
	}
}

JKRAramStreamCommand::JKRAramStreamCommand()
{
	mAllocatedTransferBuffer = false;
}

static void dummy(JSURandomInputStream* stream) { stream->getAvailable(); }
