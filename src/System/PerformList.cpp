#include <System/PerformList.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

void TPerformList::forEachPerform(
    JGadget::TSingleLinkList<TPerformLink, 0>::iterator b,
    JGadget::TSingleLinkList<TPerformLink, 0>::iterator e,
    JDrama::TGraphics* param_3, u32 param_4)
{
	for (JGadget::TSingleLinkList<TPerformLink, 0>::iterator it = b; it != e;
	     ++it) {
		TPerformLink& link = *it;
		link.unk4->testPerform(link.unk8 & param_4, param_3);
	}
}

void TPerformList::perform(u32 cue, JDrama::TGraphics* graphics)
{
	forEachPerform(getChildren().begin(), getChildren().end(), graphics, cue);
}

void TPerformList::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);

	while (stream.getLength() - stream.getPosition() > 0) {
		char acStack_6c[80];
		stream.readString(acStack_6c, 80);

		JDrama::TViewObj* obj
		    = JDrama::TNameRefGen::search<JDrama::TViewObj>(acStack_6c);

		// TODO: feels fake and stack is missing, needs more tinkering
		u32 value;
		stream >> value;
		u32 uVar5 = value;
		if (value & CUE_MOVE)
			uVar5 |= (CUE_MOVEMENT_GATE_A | CUE_MOVEMENT_GATE_B);
		if (obj)
			push_back(obj, uVar5);
	}
}

void TPerformList::push_back(const char* param_1, u32 param_2)
{
	// TODO: every instruction matches, but the target holds one more compiler
	// temporary and no `obj` local, so its stack slots all sit 4 bytes higher.
	// Folding the search into the `new TPerformLink(...)` argument removes the
	// local, but it also moves the allocation ahead of the search, which the
	// target does not do. Delegating to the TViewObj* overload costs an inline
	// pass and turns end() into a call. Both are worse.
	JDrama::TViewObj* obj
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>(param_1);

	Push_back(new TPerformLink(obj, param_2));
}

void TPerformList::push_back(JDrama::TViewObj* param_1, u32 param_2)
{
	Push_back(new TPerformLink(param_1, param_2));
}
