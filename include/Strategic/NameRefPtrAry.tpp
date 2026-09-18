#ifndef STRATEGIC_NAME_REF_PTR_ARY_TPP
#define STRATEGIC_NAME_REF_PTR_ARY_TPP

#include <Strategic/NameRefPtrAry.hpp>

// Out of line, and in a .tpp so that a TU which only *instantiates* the class
// (Camera/CubeManagerBase.cpp) emits the weak vtable and the in-class dtor but
// leaves these three as external references, exactly as the map has it. Only
// MarNameRefGen.cpp (explicit instantiation -> global) and enemytable.cpp
// (implicit -> weak) include this file, which is what the map's bindings say.

template <class T, class U>
void TNameRefPtrAryT<T, U>::load(JSUMemoryInputStream& stream)
{
	U::load(stream);
	u32 local_44 = stream.readU32();
	getChildren().reserve(local_44);
	for (int i = 0; i < local_44; ++i) {
		JSUMemoryInputStream stream2;
		JDrama::TNameRef* ref = JDrama::TNameRef::genObject(stream, stream2);
		if (ref) {
			getChildren().push_back((T*)ref);
			ref->load(stream2);
		}
	}
}

template <class T, class U> void TNameRefPtrAryT<T, U>::loadAfter()
{
	U::loadAfter();

	for (T** it = getChildren().begin(); it != getChildren().end(); ++it)
		(*it)->loadAfter();
}

template <class T, class U>
JDrama::TNameRef* TNameRefPtrAryT<T, U>::searchF(u16 key, char const* name)
{
	if (JDrama::TNameRef* ref = JDrama::TNameRef::searchF(key, name))
		return ref;

	for (T** it = getChildren().begin(); it != getChildren().end(); ++it)
		if (JDrama::TNameRef* ref = (*it)->searchF(key, name))
			return ref;

	return nullptr;
}

#endif
