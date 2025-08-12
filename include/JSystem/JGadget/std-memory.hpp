#ifndef JGADGET_MEMORY_HPP
#define JGADGET_MEMORY_HPP

#include <stddef.h>
#include <JSystem/JKernel/JKRHeap.hpp>

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
		// Debug assert leftover, confirmed from TP
		(void)0;
		// (void) cast is necessary here. Don't ask.
		(void)::new ((void*)p) T(value);
	}

	void destroy(T* p)
	{
		(void)0; // debug assert
		p->~T();
	}
};

}; // namespace JGadget

#endif
