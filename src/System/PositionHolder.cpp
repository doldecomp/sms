#include <System/PositionHolder.hpp>

TNameRefAryT<TStagePositionInfo>* gpPositionHolder;

void TStagePositionInfo::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);
	stream.read(&unkC.x, sizeof(f32));
	stream.read(&unkC.y, sizeof(f32));
	stream.read(&unkC.z, sizeof(f32));

	// wtf is happening with inlines here...
	stream.readF32();
	stream.readF32();
	stream.readF32();
	stream.readF32();
	stream.readF32();
	stream.readF32();
}
