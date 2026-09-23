#include <GC2D/MessageUtil.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>

void SMSMakeTextBuffer(J2DTextBox* param_1, int param_2)
{
	char* buffer = new char[param_2];
	for (int i = 0; i < param_2 - 1; ++i)
		buffer[i] = ' ';
	buffer[param_2 - 1] = '\0';
	param_1->setString(buffer);
}

const char* SMSGetMessageData(void* param_1, u32 param_2)
{
	if (!param_1)
		return nullptr;

	s32 local_88 = 0;
	s32 local_84 = 0;

	{
		JSUMemoryInputStream local_40(param_1, 0x20);
		local_40.skip(8);
		local_40 >> local_88;
		local_40 >> local_84;
	}

	u16 entrySize;
	int r30      = 0;
	u32 local_68 = 0;

	JSUMemoryInputStream local_74((u8*)param_1 + 0x20, local_88 * 0x20 - 0x20);

	const char* r31 = nullptr;

	while ((r30 == 0 || local_68 == 0) && local_74.getAvailable() != 0) {
		s32 iVar3 = local_74.readS32();
		s32 r27   = local_74.readS32();
		switch (iVar3) {
		case 'INF1': {
			if (param_2 >= local_74.readU16())
				return nullptr;

			entrySize = local_74.readU16();
			local_74.skip(4);
			u32 r24 = param_2 * entrySize;
			local_74.skip(r24);
			local_74 >> local_68;
			if (!local_68)
				return nullptr;
			local_74.skip(r27 - r24 - 0x14);
			break;
		}

		case 'DAT1':
			r30 = local_74.getPosition();
			local_74.skip(r27 - 8);
			break;

		default:
			local_74.skip(r27 - 8);
			break;
		}
	}

	if (r30 != 0 && local_68 != 0)
		r31 = (const char*)param_1 + r30 + local_68 + 0x20;

	return r31;
}
