#include "dolphin.h"
#include "types.h"

#include "JSystem/JUTNameTab.hpp"

template <typename T>
void* JSUConvertOffsetToPtr(const void* ptr, const void* offset)
{
	if (offset == NULL) {
		return NULL;
	}
	return (void*)((u32)ptr + (u32)offset);
};

template void* JSUConvertOffsetToPtr<u16>(const void* ptr, const void* offset);
template void* JSUConvertOffsetToPtr<f32>(const void* ptr, const void* offset);
template void* JSUConvertOffsetToPtr<ResNTAB>(const void* ptr,
                                              const void* offset);
