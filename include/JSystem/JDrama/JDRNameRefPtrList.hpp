#ifndef JDR_NAME_REF_PTR_LIST_HPP
#define JDR_NAME_REF_PTR_LIST_HPP

#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JGadget/std-list.hpp>
#include <dolphin/types.h>

class JSUMemoryInputStream;

namespace JDrama {

template <class T, class U = TNameRef>
class TNameRefPtrListT : public U, public JGadget::TList_pointer<T*> {
public:
	TNameRefPtrListT(const char* name = "<TNameRefPtrListT>")
	    : TNameRef(name)
	{
	}

	virtual void load(JSUMemoryInputStream& stream)
	{
		TNameRef::load(stream);
		s32 count = stream.readS32();

		JGadget::TList_pointer<T*>& lst = getChildren();
		for (s32 i = 0; i < count; ++i) {
			JSUMemoryInputStream remainder(nullptr, 0);
			TNameRef* nr = TNameRef::genObject(stream, remainder);
			if (nr) {
				lst.push_back(nr);
				nr->load(remainder);
			}
		}
	}

	virtual void loadAfter()
	{
		TNameRef::loadAfter();
		typedef typename JGadget::TList_pointer<T*>::iterator I;
		for (I it = getChildren().begin(); it != getChildren().end(); ++it)
			it->loadAfter();
	}

	virtual TNameRef* searchF(u16 key, const char* name)
	{
		TNameRef* res = TNameRef::searchF(key, name);
		if (res != nullptr)
			return res;

		typedef typename JGadget::TList_pointer<T*>::iterator I;
		for (I it = getChildren().begin(); it != getChildren().end(); ++it) {
			TNameRef* r = it->searchF(key, name);
			if (r != nullptr)
				return r;
		}

		return nullptr;
	}

	void insert(T* obj) { this->push_back(obj); }

	// fabricated
	JGadget::TList_pointer<T*>& getChildren() { return *this; }
};

} // namespace JDrama

#endif
