#ifndef JGADGET_MEMORY_HPP
#define JGADGET_MEMORY_HPP

#include <stddef.h>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JGadget/define.h>

namespace JGadget {

template <typename T> class TAllocator {
public:
	// NOTE: this calls ::operator new itself instead of going through
	// AllocateRaw. Routing it through AllocateRaw adds a second temporary --
	// the void* result, and then the cast of it -- and the compiler then
	// tests that temporary rather than the variable the result is stored in,
	// which costs the `mr.` in TVector::reserve and InsertRaw.
	T* allocate(size_t count, const void* = 0)
	{
		return (T*)::operator new(count * sizeof(T));
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
