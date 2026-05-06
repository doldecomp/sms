#include <Player/MarioPositionObj.hpp>

void TMarioPositionObj::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);

	char buffer[0x50];

	u32 i = 0;
	while (i < 8 && stream.getLength() > (u32)stream.getPosition() + 36) {
		stream.readString(buffer, 0x50);

		stream.read(&unk10[i].x, 4);
		stream.read(&unk10[i].y, 4);
		stream.read(&unk10[i].z, 4);

		stream.read(&unk70[i].x, 4);
		stream.read(&unk70[i].y, 4);
		stream.read(&unk70[i].z, 4);

		JGeometry::TVec3<f32> tmp;
		stream.read(&tmp.x, 4);
		stream.read(&tmp.y, 4);
		stream.read(&tmp.z, 4);

		i++;
	}

	unkD0 = i;
}
