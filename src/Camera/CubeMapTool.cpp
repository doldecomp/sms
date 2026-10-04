#include <Camera/CubeMapTool.hpp>
#include <Camera/CameraMapTool.hpp>

void TCubeGeneralInfo::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);
	stream >> unkC.x >> unkC.y >> unkC.z;
	stream >> unk18.x >> unk18.y >> unk18.z;
	stream >> unk24.x >> unk24.y >> unk24.z;

	unk24 *= 100.0f;

	unk30 = stream.readU32();
	stream.readU32();
	unk34 = stream.readU32();
}

void TCubeCameraInfo::load(JSUMemoryInputStream& stream)
{
	TCubeGeneralInfo::load(stream);

	const char* str = stream.readString();

	unk38 = (TCameraMapTool*)gpCamMapToolTable->searchF(
	    JDrama::TNameRef::calcKeyCode(str), str);
}

void TCubeStreamInfo::load(JSUMemoryInputStream& stream)
{
	TCubeGeneralInfo::load(stream);

	unk3C = unk34;
	s32 tmp;
	stream >> tmp;
	unk40 = tmp;
	stream >> unk38;
}
