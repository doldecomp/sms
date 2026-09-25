#ifndef JDR_NAME_REF_PTR_LIST_TPP
#define JDR_NAME_REF_PTR_LIST_TPP

#include <JSystem/JDrama/JDRNameRefPtrList.hpp>

namespace JDrama {

template <class T, class U>
void TNameRefPtrListT<T, U>::load(JSUMemoryInputStream& stream)
{
	TNameRef::load(stream);
	s32 count = stream.readS32();

	for (s32 i = 0; i < count; ++i) {
		JSUMemoryInputStream remainder(nullptr, 0);
		TNameRef* nr = TNameRef::genObject(stream, remainder);
		if (nr) {
			getChildren().push_back(nr);
			nr->load(remainder);
		}
	}
}

template <class T, class U> void TNameRefPtrListT<T, U>::loadAfter()
{
	TNameRef::loadAfter();
	typedef typename JGadget::TList_pointer<T*>::iterator I;
	for (I it = getChildren().begin(); it != getChildren().end(); ++it)
		(*it)->loadAfter();
}

template <class T, class U>
TNameRef* TNameRefPtrListT<T, U>::searchF(u16 key, const char* name)
{
	TNameRef* res = TNameRef::searchF(key, name);
	if (res != nullptr)
		return res;

	typedef typename JGadget::TList_pointer<T*>::iterator I;
	for (I it = getChildren().begin(); it != getChildren().end(); ++it) {
		TNameRef* r = (*it)->searchF(key, name);
		if (r != nullptr)
			return r;
	}

	return nullptr;
}

} // namespace JDrama

#endif
