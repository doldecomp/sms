#ifndef JDR_NAME_REF_PTR_LIST_HPP
#define JDR_NAME_REF_PTR_LIST_HPP

#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JGadget/std-list.hpp>
#include <dolphin/types.h>

class JSUMemoryInputStream;

namespace JDrama {

template <class T, class U = TNameRef>
class TNameRefPtrListT : public U, public JGadget::TList_pointer<T> {
public:
	TNameRefPtrListT()
	    : TNameRef("<TNameRefPtrListT>")
	{
	}

	virtual ~TNameRefPtrListT() { }
	virtual void load(JSUMemoryInputStream&) { }
	virtual void loadAfter() { }
	virtual TNameRef* searchF(u16, const char*) { }
};

} // namespace JDrama

#endif
