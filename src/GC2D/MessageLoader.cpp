#include <GC2D/MessageLoader.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>

TMessageLoader::TMessageLoader()
    : unk0(0)
    , unk4(0)
{
}

TMessageLoader::TMessageLoader(const char* param_1)
    : unk0(0)
    , unk4(0)
{
	u8* res = (u8*)JKRGetResource(param_1);
	if (res) {
		// The pair has to be one aggregate: readHeader() is inlined here and
		// MWCC folds the two stores away, and with two separate `u32` locals
		// (in this scope or the function's) it drops their slots too, leaving
		// the frame 8 bytes short of retail's 0x20 with every r1 displacement
		// 8 low. An array is homed, so the slots survive the folding.
		u32 header[2];
		readHeader(&header[0], &header[1], res);
		unk4 = parseBlock(header[0], header[1], res + 0x20);
		// NOTE: assert but in an if?
		if (unk4)
			(void)unk4;
	}
}

u32 TMessageLoader::loadMessageData(const char* param_1)
{
	u8* res = (u8*)JKRGetResource(param_1);
	if (!res)
		return -1;

	unk4 = parseBlock(*(int*)(res + 0x8) * 32, *(int*)(res + 0xC), res + 0x20);
	if (!unk4)
		return -1;

	return unk2;
}

void TMessageLoader::readHeader(u32* a, u32* b, void* header)
{
	u32* casted = (u32*)header;

	*a = *(casted + 2) * 32;
	*b = *(casted + 3);
}

void* TMessageLoader::parseBlock(u32 param_1, u32 param_2, void* param_3)
{
	s32 local_74;
	s32 local_70;
	JSUMemoryInputStream local_5c(param_3, param_1);

	void* result;

	for (int i = 0; i < param_2; ++i) {
		local_5c >> local_74;

		switch (local_74) {
		// readInfoBlock's argument must be the getCurrent() call itself: the
		// inlined body's parameter temporary is the 4-byte slot that puts
		// entrySize at 0x34 and its stream at 0x38. A named `void* info`
		// local is register-allocated and leaves both 4 low.
		case 'INF1':
			local_70 = readInfoBlock(local_5c.getCurrent());
			local_5c.skip(4);
			break;

		case 'DAT1':
			local_5c >> local_70;
			result = local_5c.getCurrent();
			break;

		case 'STR1':
			local_70 = 0;
			break;

		default:
			local_70 = 0;
			break;
		}

		local_5c.skip(local_70 - 8);
	}

	return result;
}

JMSMesgEntry* TMessageLoader::getMessageEntry(u32 param_1)
{
	JMSMesgEntry* result;
	if (u16(param_1) >= unk0)
		result = nullptr;
	else
		result = &unk8[param_1];

	return result;
}

int TMessageLoader::readInfoBlock(void* data)
{
	s32* cursor = (s32*)data;
	int length = *cursor++;
	u16 entrySize;
	JSUMemoryInputStream local_38(cursor, length - 8);
	local_38 >> unk0;
	local_38 >> entrySize;
	unk2 = local_38.readU16();
	local_38.skip(2);

	for (int i = 0; i < unk0; ++i)
		local_38.read(&unk8[i], sizeof(JMSMesgEntry));

	return length;
}
