#ifndef JDR_VIEW_OBJ_PTR_LIST_TPP
#define JDR_VIEW_OBJ_PTR_LIST_TPP

#include <JSystem/JDrama/JDRViewObjPtrList.hpp>

namespace JDrama {

// Definition order here is inert (measured in header round 22): an explicit
// instantiation emits the members in reverse *declaration* order, so the
// emission order lives in JDRViewObjPtrList.hpp. These are kept in the same
// order as the declarations only for readability.
template <class T, class U>
void TViewObjPtrListT<T, U>::load(JSUMemoryInputStream& stream)
{
	this->loadSuper(stream);
	s32 count = stream.readS32();

	for (s32 i = 0; i < count; ++i) {
		JSUMemoryInputStream stream2(nullptr, 0);
		TNameRef* nr = TNameRef::genObject(stream, stream2);
		if (nr) {
			getChildren().push_back((T*)nr);
			nr->load(stream2);
		}
	}
}

template <class T, class U>
void TViewObjPtrListT<T, U>::loadSuper(JSUMemoryInputStream& stream)
{
	TNameRef::load(stream);
}

template <class T, class U> void TViewObjPtrListT<T, U>::loadAfter()
{
	loadAfterSuper();

	typedef typename JGadget::TList_pointer<T*>::iterator I;

	for (I it = getChildren().begin(); it != getChildren().end(); ++it)
		(*it)->loadAfter();
}

template <class T, class U> void TViewObjPtrListT<T, U>::loadAfterSuper()
{
	TNameRef::loadAfter();
}

template <class T, class U>
TNameRef* TViewObjPtrListT<T, U>::searchF(u16 key, const char* name)
{
	TNameRef* ref = TNameRef::searchF(key, name);
	if (ref != nullptr)
		return ref;

	typedef typename JGadget::TList_pointer<T*>::iterator I;

	for (I it = getChildren().begin(); it != getChildren().end(); ++it) {
		TNameRef* r = (*it)->searchF(key, name);
		if (r != nullptr)
			return r;
	}
	return nullptr;
}

template <class T, class U>
void TViewObjPtrListT<T, U>::perform(u32 cue, TGraphics* graphics)
{
	typedef typename JGadget::TList_pointer<T*>::iterator I;

	for (I it = getChildren().begin(); it != getChildren().end(); ++it)
		(*it)->testPerform(cue, graphics);
}

} // namespace JDrama

#endif
