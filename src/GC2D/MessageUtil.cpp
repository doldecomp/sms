#include <GC2D/MessageUtil.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>

void SMSMakeTextBuffer(J2DTextBox* textbox, int size)
{
	char* buffer = new char[size];
	for (int i = 0; i < size - 1; ++i)
		buffer[i] = ' ';
	buffer[size - 1] = '\0';
	textbox->setString(buffer);
}

const char* SMSGetMessageData(void* messageArchive, u32 messageIndex)
{
	if (!messageArchive)
		return nullptr;

	s32 numSections = 0;
	s32 fileSize    = 0;

	{
		JSUMemoryInputStream headerStream(messageArchive, 0x20);
		headerStream.skip(8);
		headerStream >> numSections;
		headerStream >> fileSize;
	}

	char trash2[0x4];

	int dataSectionPos = 0;
	u32 messageOffset  = 0;

	JSUMemoryInputStream sectionStream((u8*)messageArchive + 0x20,
	                                   numSections * 0x20 - 0x20);

	const char* messageDataPtr = nullptr;

	while ((dataSectionPos == 0 || messageOffset == 0)
	       && sectionStream.isNotDrained()) {
		int sectionMagic = sectionStream.readS32();
		s32 sectionSize  = sectionStream.readS32();
		switch (sectionMagic) {
		case 'INF1': {
			if (messageIndex >= sectionStream.readU16())
				return nullptr;

			u32 recordSize = sectionStream.readU16();
			sectionStream.skip(4);
			recordSize = messageIndex * recordSize;
			sectionStream.skip(recordSize);
			sectionStream >> messageOffset;
			if (!messageOffset)
				return nullptr;
			sectionStream.skip(sectionSize - recordSize - 0x14);
			break;
		}

		case 'DAT1':
			// TODO: using the virtual method and letting it devirtualize
			// results in too much stack... one more getter? Or s32/int memes?
			dataSectionPos = sectionStream.mPosition;
			sectionStream.skip(sectionSize - 8);
			break;

		default:
			sectionStream.skip(sectionSize - 8);
			break;
		}
	}

	char trash[0x4];

	if (dataSectionPos != 0 && messageOffset != 0)
		messageDataPtr = (const char*)messageArchive + dataSectionPos
		                 + messageOffset + 0x20;

	return messageDataPtr;
}
