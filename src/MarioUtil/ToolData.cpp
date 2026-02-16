#include <MarioUtil/ToolData.hpp>
#include <types.h>

using namespace Koga;

ToolData::ToolData()
    : unk4(nullptr)
{
}

ToolData::~ToolData() { }

bool ToolData::Attach(const void* data)
{
	if (!data)
		return false;
	unk4 = (Header*)data;
	return true;
}

void ToolData::Detach() { }

u32 ToolData::Hash(const char* str)
{
	u32 hash = 0;
	for (; *str; ++str)
		hash = (hash + (*str << 8)) % 33554393;
	return hash;
}

bool ToolData::GetValue(int, const char*, u32&) const { }

bool ToolData::GetValue(int param_1, const char* param_2, s32& param_3) const
{
	int idx = SearchItemInfo(param_2);
	if (idx < 0)
		return false;
	u8* base = (u8*)unk4 + unk4->unk8 + param_1 * unk4->unkC;
	u8* ptr  = base + unk4->unk10[idx].getUnk8();
	param_3  = *(s32*)ptr;
	return true;
}

bool ToolData::GetValue(int, const char*, bool&) const { }

bool ToolData::GetValue(int param_1, const char* param_2,
                        const char*& param_3) const
{
	int idx = SearchItemInfo(param_2);
	if (idx < 0)
		return false;
	u8* base = (u8*)unk4 + unk4->unk8 + param_1 * unk4->unkC;
	u8* ptr  = base + unk4->unk10[idx].getUnk8();
	param_3  = (const char*)ptr;
	return true;
}

bool ToolData::GetValue(int, const char*, f32&) const { }

int ToolData::SearchItemInfo(const char* name) const
{
	int count = unk4->unk4;
	u32 hash  = Hash(name);

	for (int i = 0; i < count; i++)
		if (hash == unk4->unk10[i].unk0)
			return i;

	return -1;
}

void ToolData::FindElement(int, u32, int) const { }

void ToolData::FindElement(int, s32, int) const { }

void ToolData::FindElement(int, bool, int) const { }

void ToolData::FindElement(int, const char*, int) const { }

void ToolData::FindElement(int, f32, int) const { }
