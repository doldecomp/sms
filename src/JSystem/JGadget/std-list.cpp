
#include <JSystem/JGadget/std-list.hpp>

using namespace JGadget;

TList_pointer_void::TList_pointer_void(
    const JGadget::TAllocator<void*>& allocator)
    : TList(allocator)
{
}

TList_pointer_void::~TList_pointer_void() {
}

TList_pointer_void::iterator TList_pointer_void::insert(iterator where, const T& value)
{
  return insert1234(where, value);
}
