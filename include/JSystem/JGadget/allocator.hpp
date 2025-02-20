#ifndef JGADGET_ALLOCATOR_HPP
#define JGADGET_ALLOCATOR_HPP

#include <stddef.h>

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
		// (void) is necessary here.
		(void)::new ((void*)p) T(value);
	}

	void destroy(T* p) { p->~T(); }
};

}; // namespace JGadget

#endif
