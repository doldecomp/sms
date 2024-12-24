#include <JSystem/JDrama/JDRPlacement.hpp>


using namespace JDrama;

void TPlacement::load(JSUMemoryInputStream& stream)
{
	char pad[0x10];

	TNameRef::load(stream);
	stream.read(&position.x, sizeof(f32));
	stream.read(&position.y, sizeof(f32));
	stream.read(&position.z, sizeof(f32));
}
