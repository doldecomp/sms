#include <Player/MarioPositionObj.hpp>

void TMarioPositionObj::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);

	char buffer[0x50];
	u32 i;

	JGeometry::TVec3<f32> tmp;
	for (i = 0; i < 8
	            && stream.getLength() > stream.getPosition()
	                                        + 3 * sizeof(JGeometry::TVec3<f32>);
	     i++) {
		stream.readString(buffer, sizeof(buffer));

		stream >> unk10[i].x >> unk10[i].y >> unk10[i].z;
		stream >> unk70[i].x >> unk70[i].y >> unk70[i].z;
		stream >> tmp.x >> tmp.y >> tmp.z;
	}

	unkD0 = i;
}
