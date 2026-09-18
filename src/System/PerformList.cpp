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

// TODO: frame 0xc0 vs retail 0xe8; all 54 instructions match and the thirteen
// iterator copy slots pair up one-to-one, but retail's block sits 0x24-0x30
// higher (0x90-0xdf against our 0x6c-0xb7) with one more hole in it, i.e. 40
// bytes of dead inline-temporary space below the copies that we do not
// reserve. forEachPerform's own out-of-line size is exact (0xa4), so the body
// is right and the residue is entirely the call-site expansion.
// Measured: getChildren() over begin()/end() directly is +24 (0xa8 -> 0xc0) and
// nothing goes past 0xc0. Zero: getChildren() returning TPerformLinkList&, an
// extra getChildren() forwarder level, begin()/end() forwarders on
// TPerformLinkList, a while loop instead of the for. Rejected: (*it).perform()
// (-10%), named iterator locals for b/e (-26%), pre-increment (frame 0xb0).
// A dead 36-40 byte non-trivial local (user ctor or dtor) in forEachPerform
// lands 0xe8 exactly with perform still 54 instructions and forEachPerform
// still 41, so its UNUSED size 0xa4 survives -- forEachPerform is UNUSED, so it
// is a legal carrier, unlike the emitted callees in PollutionPos and TalkCursor.
// Not applied: nothing in a list-walk helper motivates a 36-40 byte object.
// See docs/catalog/frame-gaps.md, "The dead low region", for the size ladder.
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

		obj = (JDrama::TViewObj*)JDrama::TNameRefGen::getInstance()
		          ->getRootNameRef()
		          ->search(elementName);

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
