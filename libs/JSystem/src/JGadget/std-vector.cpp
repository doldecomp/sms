#include <JSystem/JGadget/std-vector.hpp>

using namespace JGadget;

TVector_pointer_void::TVector_pointer_void(const TAllocator<value_type>& alloc)
    : Base(alloc)
{
}

TVector_pointer_void::TVector_pointer_void(size_t count,
                                           value_type const& default_value,
                                           const TAllocator<value_type>& alloc)
    : Base(count, default_value, alloc)
{
}

TVector_pointer_void::~TVector_pointer_void() { }

TVector_pointer_void::iterator
TVector_pointer_void::insert(iterator where, const value_type& what)
{
	return Base::insert(where, what);
}

void TVector_pointer_void::insert(value_type* p, size_t count,
                                  value_type const& value)
{
	Base::insert(p, count, value);
}

TVector_pointer_void::iterator TVector_pointer_void::erase(value_type* p)
{
	return Base::erase(p);
}

TVector_pointer_void::iterator TVector_pointer_void::erase(value_type* pFirst,
                                                           value_type* pLast)
{
	return Base::erase(pFirst, pLast);
}

void TVector_pointer_void::assign(size_t count, value_type const& value)
{
	Base::assign(count, value);
}

void TVector_pointer_void::resize(size_t new_size, value_type const& value)
{
	Base::resize(new_size, value);
}

void TVector_pointer_void::reserve(size_t new_capacity)
{
	Base::reserve(new_capacity);
}

TVector_pointer_void&
TVector_pointer_void::operator=(const TVector_pointer_void& other)
{
	Base::operator=(other);
	return *this;
}

TVector_pointer_void::value_type* TVector_pointer_void::InsertRaw(value_type* p,
                                                                  size_t count)
{
	return Base::InsertRaw(p, count);
}

TVector_pointer_void::value_type*
TVector_pointer_void::ResizeRaw(size_t new_size)
{
	return Base::ResizeRaw(new_size);
}
