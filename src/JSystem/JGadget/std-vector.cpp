#include <JSystem/JGadget/std-vector.hpp>

using namespace JGadget;

TVector_pointer_void::TVector_pointer_void(const TAllocator<void*>& alloc)
    : Base(alloc)
{
}

TVector_pointer_void::~TVector_pointer_void() { }

TVector_pointer_void::iterator
TVector_pointer_void::insert(iterator where, const value_type& what)
{
	return Base::insert(where, what);
}

void TVector_pointer_void::reserve(size_t new_capacity)
{
	Base::reserve(new_capacity);
}
