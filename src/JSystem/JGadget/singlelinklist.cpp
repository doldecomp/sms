#include <JSystem/JGadget/singlelinklist.hpp>

namespace JGadget {

// namespace {

// 	template <class T> struct TPRIsEqual_pointer_ { };

// } // namespace

TSingleNodeLinkList::~TSingleNodeLinkList() { }

void TSingleNodeLinkList::erase(JGadget::TSingleNodeLinkList::iterator) { }

void TSingleNodeLinkList::erase(JGadget::TSingleNodeLinkList::iterator,
                                JGadget::TSingleNodeLinkList::iterator)
{
}

void TSingleNodeLinkList::clear() { }

void TSingleNodeLinkList::splice(JGadget::TSingleNodeLinkList::iterator,
                                 JGadget::TSingleNodeLinkList&)
{
}

void TSingleNodeLinkList::splice(JGadget::TSingleNodeLinkList::iterator,
                                 JGadget::TSingleNodeLinkList&,
                                 JGadget::TSingleNodeLinkList::iterator)
{
}

void TSingleNodeLinkList::splice(JGadget::TSingleNodeLinkList::iterator,
                                 JGadget::TSingleNodeLinkList&,
                                 JGadget::TSingleNodeLinkList::iterator,
                                 JGadget::TSingleNodeLinkList::iterator)
{
}
// std::__distance<JGadget::TSingleNodeLinkList::iterator>(JGadget::TSingleNodeLinkList::iterator,
// JGadget::TSingleNodeLinkList::iterator, std::input_iterator_tag)

void TSingleNodeLinkList::swap(JGadget::TSingleNodeLinkList&) { }

void TSingleNodeLinkList::reverse() { }

void TSingleNodeLinkList::Find(const JGadget::TSingleLinkListNode*) { }

TSingleNodeLinkList::iterator
TSingleNodeLinkList::Insert(JGadget::TSingleNodeLinkList::iterator param_1,
                            JGadget::TSingleLinkListNode* param_2)
{
	TSingleLinkListNode** nodePtr = param_1.unk0;

	TSingleLinkListNode* node = *nodePtr;

	param_2->mNext = node;
	*nodePtr       = param_2;
	if (node == nullptr)
		unk8 = param_2;
	++unk0;

	return param_1;
}

void TSingleNodeLinkList::Remove(JGadget::TSingleLinkListNode*) { }

// template <>
// void TSingleNodeLinkList::Remove_if<
//     TPRIsEqual_pointer_<JGadget::TSingleLinkListNode> >(
//     TPRIsEqual_pointer_<JGadget::TSingleLinkListNode>,
//     JGadget::TSingleNodeLinkList&)
// {
// }

// std::swap<JGadget::TSingleLinkListNode**>(JGadget::TSingleLinkListNode**&,
// JGadget::TSingleLinkListNode**&)
// std::swap<JGadget::TSingleLinkListNode*>(JGadget::TSingleLinkListNode*&,
// JGadget::TSingleLinkListNode*&)

} // namespace JGadget
