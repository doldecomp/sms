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

	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual TNameRef* searchF(u16 key, const char* name);

	void insert(T* obj) { this->push_back(obj); }

	// fabricated
	JGadget::TList_pointer<T*>& getChildren() { return *this; }
};

} // namespace JDrama

#endif
