#include <JSystem/JDrama/JDRPlacement.hpp>

using namespace JDrama;

void TPlacement::load(JSUMemoryInputStream& stream)
{
	TNameRef::load(stream);
	stream >> mPosition.x >> mPosition.y >> mPosition.z;
}
