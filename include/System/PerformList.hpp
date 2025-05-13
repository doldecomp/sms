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

class TPerformList : public JDrama::TViewObj {
public:
	TPerformList() { }

	void push_back(JDrama::TViewObj*, u32);
	void push_back(const char*, u32);
	void load(JSUMemoryInputStream&);
	void perform(u32, JDrama::TGraphics*);
	void forEachPerform(JGadget::TSingleLinkList<TPerformLink, 0>::iterator,
	                    JGadget::TSingleLinkList<TPerformLink, 0>::iterator,
	                    JDrama::TGraphics*, u32);

public:
	JGadget::TSingleLinkList<TPerformLink, 0> unk10;
};

#endif
