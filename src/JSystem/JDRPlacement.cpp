#include "dolphin.h"

#include "types.h"

#include "JSystem/JDRPlacement.hpp"
using namespace JDrama;

void TPlacement::load(JSUMemoryInputStream& stream)
{
	char pad[0x10];

	TNameRef::load(stream);
	stream.read(&this->position.x, sizeof(f32));
	stream.read(&this->position.y, sizeof(f32));
	stream.read(&this->position.z, sizeof(f32));
}
