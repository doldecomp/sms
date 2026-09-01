#include <System/PerformList.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

void TPerformList::forEachPerform(
    JGadget::TSingleLinkList<TPerformLink, 0>::iterator b,
    JGadget::TSingleLinkList<TPerformLink, 0>::iterator e,
    JDrama::TGraphics* graphics, u32 cue)
{
	for (JGadget::TSingleLinkList<TPerformLink, 0>::iterator it = b; it != e;
	     it++) {
		it->perform(cue, graphics);
	}
}

void TPerformList::perform(u32 cue, JDrama::TGraphics* graphics)
{
	forEachPerform(getChildren().begin(), getChildren().end(), graphics, cue);
}

void TPerformList::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);

	JDrama::TViewObj* obj;
	char elementName[80];

	while (stream.getLength() - stream.getPosition() > 0) {
		stream.readString(elementName, 80);

		obj = JDrama::TNameRefGen::search<JDrama::TViewObj>(elementName);

		u32 value = stream.readU32();

		if (value & CUE_MOVE)
			value |= (CUE_MOVEMENT_GATE_A | CUE_MOVEMENT_GATE_B);

		if (obj)
			push_back(obj, value);
	}
}

void TPerformList::push_back(const char* param_1, u32 param_2)
{
	JDrama::TViewObj* obj
	    = (JDrama::TViewObj*)JDrama::TNameRefGen::search2(param_1);

	Push_back(new TPerformLink(obj, param_2));
}

void TPerformList::push_back(JDrama::TViewObj* param_1, u32 param_2)
{
	Push_back(new TPerformLink(param_1, param_2));
}
