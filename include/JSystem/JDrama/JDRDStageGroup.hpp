#ifndef JDR_DSTAGE_GROUP_HPP
#define JDR_DSTAGE_GROUP_HPP

#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JDrama/JDRFrmGXSet.hpp>

namespace JDrama {

// TODO: the `setup`s of MenuDir, MovieDirector and GCLogoDir (and MenuDir /
// MovieDirector `rsetup`) are instruction-exact but short of frame: retail
// gives the inlined `new TDStageGroup` chain one dead 4-byte slot above
// each of the TDStageGroup, TViewObjPtrListT and (list-chain) TViewObj
// `this` slots, and none above the TViewObj inside the unk20 TFrmGXSet
// (MenuDir setup: this at 0x38/0x30/0x28, allocator 0x24, FrmGXSet's
// TViewObj 0x20; ours 0x24/0x20/0x1c/0x18/0x14, frame 0x40 vs 0x58).
// Header round c-jdrctor, all inert (frame and slots unchanged) on all five:
// `U(name)` for the list's base init, `TViewObjPtrListT<TViewObj,
// TViewObj>(name)` here, no default on TViewObj's name, TNameRef's ctor as
// body assignments, an explicit `TList_pointer<T*>()` base init, the
// TDStageGroup/TViewObj ctors defined out of class `inline`, user-declared
// `virtual ~T() { }` on any or all of the three levels, and at the caller
// an explicit name argument. Out-of-class template definition of the
// list's ctor puts it out of line (68.9%); `unkC(0)` inlines TFlagT (80%).
// A dead `const char*` local in each body (probe only, refused) reproduces
// the three upper slots exactly, so the residue is one 4-byte temporary
// per inline level that our spelling substitutes away.
class TDStageGroup : public TViewObjPtrListT<TViewObj> {
public:
	TDStageGroup(TDisplay* display, const char* name = "<TDStageGroup>")
	    : TViewObjPtrListT<TViewObj>(name)
	    , unk20(display)
	{
	}

	virtual void perform(u32 cue, TGraphics* graphics);

public:
	/* 0x20 */ TFrmGXSet unk20;
};

} // namespace JDrama

#endif
