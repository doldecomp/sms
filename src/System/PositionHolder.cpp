#include <System/PositionHolder.hpp>

TNameRefAryT<TStagePositionInfo>* gpPositionHolder;

void TStagePositionInfo::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);
	stream >> unkC.x >> unkC.y >> unkC.z;

	stream.readF32();
	stream.readF32();
	stream.readF32();
	stream.readF32();
	stream.readF32();
	stream.readF32();
}
