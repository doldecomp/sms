#include <System/StageEventInfo.hpp>

void TStageEventInfo::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);

	unkC  = stream.readU32();
	unk10 = stream.readString();
	unk14 = stream.readString();
	unk18 = stream.readString();
	unk1C = stream.readString();
	unk20 = stream.readString();
	unk24 = stream.readString();
}
