#include <JSystem/JDrama/JDRCharacter.hpp>
#include <JSystem/JKernel/JKRArchive.hpp>

using namespace JDrama;

void TSmplChara::load(JSUMemoryInputStream& stream)
{
	TNameRef::load(stream);
	char buf[0x50];
	stream.readString(buf, 0x50);
	mountArc(buf);
}

TSmplChara::~TSmplChara()
{
	if (mArchive)
		mArchive->unmount();
}

void* TSmplChara::getRes(const char* path) const
{
	return mArchive->getResource(path);
}

void TSmplChara::mountArc(const char* name)
{
	mArchive = JKRMountArchive(name, JKRArchive::MOUNT_MEM,
	                           JKRHeap::getCurrentHeap(),
	                           JKRArchive::MOUNT_DIRECTION_HEAD);
}
