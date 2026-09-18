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

	// Declaration order is the map's, reversed: MarNameRefGen.cpp's explicit
	// instantiation emits these in reverse *declaration* order (definition
	// order in the .tpp is inert, measured in header round 22), and retail
	// emits perform, searchF, loadAfterSuper, loadAfter, loadSuper, load.
	// `loadSuper`/`loadAfterSuper` are the only new virtuals here, so their
	// relative order is what the vtable tail sees; the other four are
	// overrides and their positions are free.
	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadSuper(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual void loadAfterSuper();
	virtual TNameRef* searchF(u16 key, const char* name);
	virtual void perform(u32 cue, TGraphics* graphics);

	// surprisingly, real.
	JGadget::TList_pointer<T*>& getChildren() { return *this; }

	// fabricated name. The pointer is taken by value, not as `T* const&`, so
	// that a derived-to-base conversion temporary at the call site binds
	// inside push_back's `const T&` instead of here: with the reference
	// parameter the temporary lands at the bottom of the caller's frame,
	// where retail parks it above the iterator temporaries
	// (TMarDirector::registerEventWatcher exact, frame 0x50 not 0x48;
	// TLightWithDBSetManager::addChildGroupObj exact; initECTGft +2.8,
	// initECDisp +2.3, setupObjects +0.25).
	// Known exception: TBossMantaAdditionalCollisionSet's ctor loses one
	// instruction (99.75 -> 98.62), because there the conversion is
	// zero-offset and retail reuses the `new` result's own spill slot as the
	// reference. Neither a named `THitActor*` local nor
	// `getChildren().push_back(this)` at that site recovers it (98.6 / 93.1),
	// and that frame is independently 8 low bytes short of retail.
	void insert(T* obj) { getChildren().push_back(obj); }
};

} // namespace JDrama

#endif
