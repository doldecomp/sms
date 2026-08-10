#ifndef SYSTEM_PERFORM_LIST_HPP
#define SYSTEM_PERFORM_LIST_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGadget/singlelinklist.hpp>

class TPerformLink {
public:
	TPerformLink(JDrama::TViewObj* param_1, u32 param_2)
	    : unk4(param_1)
	    , unk8(param_2)
	{
	}

public:
	/* 0x0 */ JGadget::TSingleLinkListNode unk0;
	/* 0x4 */ JDrama::TViewObj* unk4;
	/* 0x8 */ u32 unk8;
};

// NOTE: fabricated name, but a class of this shape must exist. Two independent
// results need one more class between TPerformList and TSingleNodeLinkList than
// a direct derivation gives:
//   * TPerformList::~TPerformList holds two nested `if (subobject != 0)` guards
//     before it calls ~TSingleNodeLinkList, and MWCC emits exactly one guard
//     per inlined destructor level;
//   * TMarDirector::TMarDirector calls Initialize_ out of line five times, and
//     Initialize_ is only pushed past the last inline pass with the extra
//     level.
// Both go to 100% with it.
class TPerformLinkList : public JGadget::TSingleLinkList<TPerformLink, 0> { };

class TPerformList : public JDrama::TViewObj, public TPerformLinkList {
public:
	TPerformList() { }
	TPerformList(const char* name)
	    : JDrama::TViewObj(name)
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	// Filters are &ed with the first param in perform
	void push_back(JDrama::TViewObj* object, u32 filter);
	void push_back(const char* name, u32 filter);

	void forEachPerform(JGadget::TSingleLinkList<TPerformLink, 0>::iterator,
	                    JGadget::TSingleLinkList<TPerformLink, 0>::iterator,
	                    JDrama::TGraphics*, u32);

	JGadget::TSingleLinkList<TPerformLink, 0>& getChildren() { return *this; }
};

#endif
