#include <Camera/CubeMapTool.hpp>
#include <Camera/CameraMapTool.hpp>

void TCubeGeneralInfo::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);
	stream.read(&unkC.x, sizeof(f32));
	stream.read(&unkC.y, sizeof(f32));
	stream.read(&unkC.z, sizeof(f32));
	stream.read(&unk18.x, sizeof(f32));
	stream.read(&unk18.y, sizeof(f32));
	stream.read(&unk18.z, sizeof(f32));
	stream.read(&unk24.x, sizeof(f32));
	stream.read(&unk24.y, sizeof(f32));
	stream.read(&unk24.z, sizeof(f32));
	unk24.x *= 100.0f;
	unk24.y *= 100.0f;
	unk24.z *= 100.0f;
	unk30 = stream.readU32();
	stream.readU32();
	unk34 = stream.readU32();
}

void TCubeCameraInfo::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);
	stream.read(&unkC.x, sizeof(f32));
	stream.read(&unkC.y, sizeof(f32));
	stream.read(&unkC.z, sizeof(f32));
	stream.read(&unk18.x, sizeof(f32));
	stream.read(&unk18.y, sizeof(f32));
	stream.read(&unk18.z, sizeof(f32));
	stream.read(&unk24.x, sizeof(f32));
	stream.read(&unk24.y, sizeof(f32));
	stream.read(&unk24.z, sizeof(f32));
	unk24.x *= 100.0f;
	unk24.y *= 100.0f;
	unk24.z *= 100.0f;
	unk30 = stream.readU32();
	stream.readU32();
	unk34 = stream.readU32();

	const char* str = stream.readString();

	unk38 = gpCamMapToolTable->searchF(JDrama::TNameRef::calcKeyCode(str), str);
}

void TCubeStreamInfo::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);
	stream.read(&unkC.x, sizeof(f32));
	stream.read(&unkC.y, sizeof(f32));
	stream.read(&unkC.z, sizeof(f32));
	stream.read(&unk18.x, sizeof(f32));
	stream.read(&unk18.y, sizeof(f32));
	stream.read(&unk18.z, sizeof(f32));
	stream.read(&unk24.x, sizeof(f32));
	stream.read(&unk24.y, sizeof(f32));
	stream.read(&unk24.z, sizeof(f32));
	unk24.x *= 100.0f;
	unk24.y *= 100.0f;
	unk24.z *= 100.0f;
	unk30 = stream.readS32();
	stream.readU32();
	unk34 = stream.readS32();

	unk3C = unk34;
	unk40 = stream.readS32();
	stream.readU32();
}
