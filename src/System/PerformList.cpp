#include <System/PerformList.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

// JGadget::TSingleNodeLinkList::iterator::iterator(const
// JGadget::TSingleNodeLinkList::iterator&)
// JGadget::TSingleNodeLinkList::iterator::iterator(JGadget::TSingleLinkListNode**)
// JGadget::TSingleLinkList<TPerformLink, 0>::Element_getNode(TPerformLink*)
// JGadget::TSingleLinkList<TPerformLink,
// 0>::iterator::iterator(JGadget::TSingleNodeLinkList::iterator)
// JGadget::TSingleNodeLinkList::end()

void TPerformList::forEachPerform(
    JGadget::TSingleLinkList<TPerformLink, 0>::iterator b,
    JGadget::TSingleLinkList<TPerformLink, 0>::iterator e,
    JDrama::TGraphics* param_3, u32 param_4)
{
	for (JGadget::TSingleLinkList<TPerformLink, 0>::iterator it = b; it != e;
	     ++it) {
		it->unk4->testPerform(param_4, param_3);
	}
}

void TPerformList::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	forEachPerform(unk10.begin(), unk10.end(), param_2, param_1);
}

void TPerformList::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);

	while (stream.getLength() - stream.getPosition() > 0) {
		char acStack_6c[80];
		stream.readString(acStack_6c, 80);

		JDrama::TViewObj* obj
		    = JDrama::TNameRefGen::search<JDrama::TViewObj>(acStack_6c);
		u32 value;
		stream.read(&value, 4);
		u32 uVar5 = value;
		if (value & 1)
			uVar5 = value | 0x3000;
		if (obj)
			push_back(obj, uVar5);
	}
}

void TPerformList::push_back(const char* param_1, u32 param_2)
{
	JDrama::TViewObj* obj
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>(param_1);

	unk10.Push_back(new TPerformLink(obj, param_2));
}

void TPerformList::push_back(JDrama::TViewObj* param_1, u32 param_2)
{
	unk10.Push_back(new TPerformLink(param_1, param_2));
}
