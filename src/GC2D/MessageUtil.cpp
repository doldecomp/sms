#include <GC2d/MessageUtil.hpp>
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

u8* SMSGetMessageData(void* param_1, u32 param_2)
{
	if (!param_1)
		return nullptr;

	int local_88 = 0;
	int local_84 = 0;

	{
		JSUMemoryInputStream local_40(param_1, 0x20);
		local_40.skip(8);
		local_40.read(&local_88, 4);
		local_40.read(&local_84, 4);
	}

	char trash2[0x4];

	int r30      = 0;
	u32 local_68 = 0;

	JSUMemoryInputStream local_74((u8*)param_1 + 0x20, local_88 * 0x20 - 0x20);

	u8* r31 = nullptr;

	while ((r30 == 0 || local_68 == 0) && local_74.isNotDrained()) {
		int iVar3 = local_74.readS32();
		s32 r27   = local_74.readS32();
		switch (iVar3) {
		case 'INF1': {
			if (param_2 >= local_74.readU16())
				return nullptr;

			u32 r24 = local_74.readU16();
			local_74.skip(4);
			r24 = param_2 * r24;
			local_74.skip(r24);
			local_74.read(&local_68, 4);
			if (!local_68)
				return nullptr;
			local_74.skip(r27 - r24 - 0x14);
			break;
		}

		case 'DAT1':
			// TODO: using the virtual method and letting it devirtualize
			// results in too much stack... one more getter? Or s32/int memes?
			r30 = local_74.mPosition;
			local_74.skip(r27 - 8);
			break;

		default:
			local_74.skip(r27 - 8);
			break;
		}
	}

	char trash[0x4];

	if (r30 != 0 && local_68 != 0)
		r31 = (u8*)param_1 + r30 + local_68 + 0x20;

	return r31;
}
