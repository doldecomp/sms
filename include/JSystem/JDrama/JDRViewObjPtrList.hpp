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
	TViewObjPtrListT(const char* name = "<TViewObjPtrListT>")
	    : TViewObj(name)
	{
	}

	virtual void load(JSUMemoryInputStream& stream)
	{
		this->loadSuper(stream);
		int count = stream.readS32();

		JGadget::TList_pointer<T*>& lst = *this;

		for (int i = 0; i < count; ++i) {
			JSUMemoryInputStream stream2(nullptr, 0);
			TNameRef* nr = TNameRef::genObject(stream, stream2);
			if (nr) {
				lst.push_back((T*)nr);
				nr->load(stream2);
			}
		}
	}

	virtual void loadAfter()
	{
		loadAfterSuper();

		typedef typename JGadget::TList_pointer<T*>::iterator I;

		for (I it = getChildren().begin(); it != getChildren().end(); ++it)
			it->loadAfter();
	}

	virtual TNameRef* searchF(u16 key, const char* name)
	{
		TNameRef* ref = TNameRef::searchF(key, name);
		if (ref != nullptr)
			return ref;

		typedef typename JGadget::TList_pointer<T*>::iterator I;

		for (I it = getChildren().begin(); it != getChildren().end(); ++it) {
			TNameRef* r = it->searchF(key, name);
			if (r != nullptr)
				return r;
		}
		return nullptr;
	}

	virtual void perform(u32 param_1, TGraphics* param_2);

	virtual void loadSuper(JSUMemoryInputStream& stream);

	virtual void loadAfterSuper() { TNameRef::loadAfter(); }

	// surprisingly, real.
	JGadget::TList_pointer<T*>& getChildren() { return *this; }

	// fabricated
	void insert(T* const& obj) { getChildren().push_back(obj); }
};

template <class T, class U>
void TViewObjPtrListT<T, U>::perform(u32 param_1, TGraphics* param_2)
{
	typedef typename JGadget::TList_pointer<T*>::iterator I;

	for (I it = getChildren().begin(); it != getChildren().end(); ++it)
		it->testPerform(param_1, param_2);
}

template <class T, class U>
void TViewObjPtrListT<T, U>::loadSuper(JSUMemoryInputStream& stream)
{
	TNameRef::load(stream);
}

} // namespace JDrama

#endif
