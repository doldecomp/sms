#ifndef JDR_VIEW_OBJ_PTR_LIST_HPP
#define JDR_VIEW_OBJ_PTR_LIST_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>

namespace JDrama {

template <class T, class U = TViewObj>
class TViewObjPtrListT : public U, public JGadget::TList_pointer<T*> {
public:
	TViewObjPtrListT()
	    : TViewObj("<TViewObjPtrListT>")
	{
	}

	TViewObjPtrListT(const char* name)
	    : TViewObj(name)
	{
	}

	virtual ~TViewObjPtrListT() { }
	virtual void load(JSUMemoryInputStream&) { }
	virtual void loadAfter() { }
	virtual TNameRef* searchF(u16, const char*) { }

	virtual void perform(u32 param_1, TGraphics* param_2)
	{
		JGadget::TList_pointer_void::iterator it;
		for (it = this->begin(); it != this->end(); ++it) {
			(*((TViewObj**)it.operator->()))->testPerform(param_1, param_2);
		}
	}
	virtual void loadSuper(JSUMemoryInputStream& stream)
	{
		TNameRef::load(stream);
	}
	virtual void loadAfterSuper() { TNameRef::loadAfter(); }
};

} // namespace JDrama

#endif
