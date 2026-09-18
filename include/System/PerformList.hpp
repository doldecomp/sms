#ifndef SYSTEM_PERFORM_LIST_HPP
#define SYSTEM_PERFORM_LIST_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGadget/singlelinklist.hpp>

class TPerformLink {
public:
	TPerformLink(JDrama::TViewObj* param_1, u32 param_2)
	    : mPerformer(param_1)
	    , mCueFilter(param_2)
	{
	}

	void perform(u32 cue, JDrama::TGraphics* graphics)
	{
		mPerformer->testPerform(mCueFilter & cue, graphics);
	}

public:
	/* 0x0 */ JGadget::TSingleLinkListNode unk0;
	/* 0x4 */ JDrama::TViewObj* mPerformer;
	/* 0x8 */ u32 mCueFilter;
};

class TPerformList : public JDrama::TViewObj,
                     public JGadget::TSingleLink<TPerformLink, 0> {
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
