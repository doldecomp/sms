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
	TNameRefPtrListT()
	    : TNameRef("<TNameRefPtrListT>")
	{
	}

	virtual ~TNameRefPtrListT() { }

	virtual void load(JSUMemoryInputStream& stream)
	{
		TNameRef::load(stream);
		int count = stream.readS32();

		JGadget::TList_pointer<T*>& lst = *this;
		for (int i = 0; i < count; ++i) {
			JSUMemoryInputStream stream2(nullptr, 0);
			TNameRef* nr = TNameRef::genObject(stream, stream2);
			if (nr) {
				lst.push_back(nr);
				nr->load(stream2);
			}
		}
	}

	virtual void loadAfter()
	{
		TNameRef::loadAfter();
		typename JGadget::TList_pointer<T*>::iterator it = this->begin();
		for (; it != this->end(); ++it)
			(*it)->loadAfter();
	}

	virtual TNameRef* searchF(u16 key, const char* name)
	{
		TNameRef* res = TNameRef::searchF(key, name);
		if (res != nullptr)
			return res;

		typename JGadget::TList_pointer<T*>::iterator it = this->begin();
		for (; it != this->end(); ++it) {
			TNameRef* r = (*it)->searchF(key, name);
			if (r != nullptr)
				return r;
		}

		return nullptr;
	}

	void insert(T* obj) { this->push_back(obj); }
};

} // namespace JDrama

#endif
