#include <System/ScenarioArchiveName.hpp>

void TScenarioArchiveName::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);

	unkC = stream.readString();
}
