#ifndef JGADGET_MEMORY_HPP
#define JGADGET_MEMORY_HPP

#include <stddef.h>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JGadget/define.h>

namespace JGadget {

template <typename T> class TAllocator {
public:
	T* allocate(size_t count, const void* = 0)
	{
		return (T*)AllocateRaw(count * sizeof(T));
	}

	void* AllocateRaw(size_t rawSize) { return ::operator new(rawSize); }

	void deallocate(T* p, size_t) { DeallocateRaw(p); }

	void DeallocateRaw(void* p) { ::operator delete(p); }

	void construct(T* p, const T& value)
	{
		// clang-format off
		JGADGET_ASSERT(p!=0);
		// clang-format on
		(void)::new (p) T(value);
	}

	void destroy(T* p)
	{
		// clang-format off
		JGADGET_ASSERT(p!=0);
		// clang-format on
		p->~T();
	}
};

}; // namespace JGadget

#endif
