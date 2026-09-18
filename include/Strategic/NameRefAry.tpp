#ifndef STRATEGIC_NAME_REF_ARY_TPP
#define STRATEGIC_NAME_REF_ARY_TPP

#include <Strategic/NameRefAry.hpp>

// Out of line, and in a .tpp so that the twenty-odd TUs which only *use* a
// TNameRefAryT do not each emit a weak copy: the map has these three only in
// MarNameRefGen.cpp, global for the TStagePositionInfo, TCameraMapTool and
// TScenarioArchiveName instantiations (explicitly instantiated there) and weak
// for TStageEventInfo (implicit), with no unreferenced duplicates anywhere.

template <class T, class U>
void TNameRefAryT<T, U>::load(JSUMemoryInputStream& stream)
{
	U::load(stream);
	u32 local_44 = stream.readU32();
	getChildren().resize(local_44);

	for (int i = 0; i < local_44; ++i) {
		JSUMemoryInputStream stream2;
		JDrama::TNameRef::getType(stream, stream2);
		getChildren()[i].load(stream2);
	}
}

template <class T, class U> void TNameRefAryT<T, U>::loadAfter()
{
	U::loadAfter();
	for (T* it = getChildren().begin(); it != getChildren().end(); ++it)
		it->loadAfter();
}

template <class T, class U>
JDrama::TNameRef* TNameRefAryT<T, U>::searchF(u16 key, const char* name)
{
	if (JDrama::TNameRef* candidate = JDrama::TNameRef::searchF(key, name))
		return candidate;

	for (T* it = getChildren().begin(); it != getChildren().end(); ++it)
		if (JDrama::TNameRef* candidate = it->searchF(key, name))
			return candidate;

	return nullptr;
}

#endif
