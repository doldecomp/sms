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

// TODO: frame 0xc0 vs retail 0xe8, and the residue is **not** a plain frame
// gap: the thirteen iterator copy slots pair up one-to-one but their *grouping*
// differs, so no amount of low region lines them up. Ours are
// [0x6c 0x70 0x74 0x78] [0x84 0x88] [0x90 0x94 0x98] [0xa8 0xac 0xb0 0xb4];
// retail's are [0x90 0x94] [0xa4 0xa8] [0xb0 0xb4 0xb8 0xbc 0xc0]
// [0xd0 0xd4 0xd8 0xdc] -- a 12-byte hole between the two `operator!=`
// temporary pairs where we have none, and no hole inside the middle group
// where we have four bytes. That is the shared `JGadget` iterator expansion
// structure that batch 81 flagged in `std-list.hpp` (the same "off by 8 per
// group gap" seen in `SDLModel::entry` and `TMirrorActor::init`), not
// something this TU can spell.
// Frame levers measured anyway (all 54 instructions, all 36 diffs, i.e. none
// of them changes the grouping): one level that binds `getChildren()` is +0x10
// per call site and +0x18 for both, a second stacked level +0x18 more,
// `begin()`/`end()` without `getChildren()` -0x18. Worse: binding levels on
// `begin()`/`end()` themselves (67 instructions), named iterator locals for b
// and e (63), wrapping the whole call (81).
// `forEachPerform`'s own out-of-line copy is exact (UNUSED 0xa4), so the body
// is right; a dead 36-40 byte non-trivial local in it would land 0xe8 (it is
// UNUSED, so a legal carrier) but nothing in a list walk motivates one and it
// would not fix the grouping either.
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
