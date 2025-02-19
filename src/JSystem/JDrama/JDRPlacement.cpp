#include <JSystem/JDrama/JDRPlacement.hpp>

using namespace JDrama;

void TPlacement::load(JSUMemoryInputStream& stream)
{
	char pad[0x10];
	TNameRef::load(stream);
	stream.read(&mPosition.x, sizeof(f32));
	stream.read(&mPosition.y, sizeof(f32));
	stream.read(&mPosition.z, sizeof(f32));
}
