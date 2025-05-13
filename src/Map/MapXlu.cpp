#include <Map/MapXlu.hpp>
#include <Map/MapModel.hpp>
#include <Map/Map.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TMapXlu::TMapXlu()
    : unk0(0)
    , unk4(nullptr)
{
}

void TMapXlu::init(JSUMemoryInputStream& stream)
{
	int tmp;
	stream.read(&tmp, 4);
	unk0 = tmp;
	if (unk0 != 0) {
		unk4 = new Entry[unk0];
		for (int i = 0; i < unk0; ++i) {
			Entry& entry = unk4[i];
			stream.read(&tmp, 4);
			entry.unk0 = tmp;
			entry.unk4 = new u32[entry.unk0];
			entry.unk8 = new u32[entry.unk0];
			for (int j = 0; j < entry.unk0; ++j) {
				stream.read(&tmp, 4);
				entry.unk4[j] = tmp;
				stream.read(&tmp, 4);
				entry.unk8[j] = tmp;
			}
		}
	}
}

bool TMapXlu::changeXluJoint(int n)
{
	if (n >= unk0)
		return false;

	for (int i = 0; i < gpMap->getRootJointModel()->getChildrenNum(); ++i)
		gpMap->getRootJointModel()->getChild(i)->sit();

	for (int i = 0; i < unk4[n].unk0; ++i)
		gpMap->getRootJointModel()
		    ->getChild(unk4[n].unk4[i])
		    ->getChild(unk4[n].unk8[i])
		    ->stand();

	return true;
}

void TMapXlu::changeNormalJoint()
{
	for (int i = 0; i < gpMap->getRootJointModel()->getChildrenNum(); ++i)
		gpMap->getRootJointModel()->getChild(i)->stand();

	for (int i = 0; i < unk0; ++i)
		for (int j = 0; j < unk4[i].unk0; ++j)
			gpMap->getRootJointModel()
			    ->getChild(unk4[i].unk4[j])
			    ->getChild(unk4[i].unk8[j])
			    ->sit();
}
