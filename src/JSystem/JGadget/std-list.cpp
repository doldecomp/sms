
#include <JSystem/JGadget/std-list.hpp>

using namespace JGadget;

TList_pointer_void::TList_pointer_void(
    const JGadget::TAllocator<void*>& allocator)
    : TList(allocator)
{
}

TList_pointer_void::~TList_pointer_void() { }

TList_pointer_void::iterator TList_pointer_void::insert(iterator where,
                                                        const value_type& value)
{
	return Base::insert(where, value);
}
