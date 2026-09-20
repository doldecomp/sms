#ifndef JDR_VIEW_OBJ_PTR_LIST_HPP
#define JDR_VIEW_OBJ_PTR_LIST_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JGadget/std-list.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>

namespace JDrama {

template <class T, class U = TViewObj>
class TViewObjPtrListT : public U, public JGadget::TList_pointer<T*> {
public:
	TViewObjPtrListT(const char* name = "<TViewObjPtrListT>")
	    : TViewObj(name)
	{
	}

	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual TNameRef* searchF(u16 key, const char* name);
	virtual void perform(u32 cue, TGraphics* graphics);
	virtual void loadSuper(JSUMemoryInputStream& stream);
	virtual void loadAfterSuper();

	// surprisingly, real.
	JGadget::TList_pointer<T*>& getChildren() { return *this; }

	// fabricated
	void insert(T* const& obj) { getChildren().push_back(obj); }
};

} // namespace JDrama

#endif
