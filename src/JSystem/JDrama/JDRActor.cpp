#include "JSystem/JDrama/JDRNameRefGen.hpp"
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRLighting.hpp>

void JDrama::TActor::load(JSUMemoryInputStream& stream)
{
	TPlacement::load(stream);
	stream.read(&mRotation.x, sizeof(f32));
	stream.read(&mRotation.y, sizeof(f32));
	stream.read(&mRotation.z, sizeof(f32));
	stream.read(&mScaling.x, sizeof(f32));
	stream.read(&mScaling.y, sizeof(f32));
	stream.read(&mScaling.z, sizeof(f32));

	char str[0x50];
	stream.readString(str, 0x50);

	unk3C = TNameRefGen::getInstance()->getRootNameRef()->search(str);

	unk40 = new TLightMap();
	unk40->load(stream);
}

void JDrama::TActor::perform(u32 param_1, TGraphics* param_2)
{
	if ((param_1 & 0x8) && unk40 != nullptr)
		unk40->perform(param_1 | 0x20, param_2);
}

JDrama::TActor::~TActor() { }

void JDrama::TActor::JSGGetTranslation(Vec* v) const { *v = mPosition; }

void JDrama::TActor::JSGSetTranslation(const Vec& v)
{
	mPosition.x = v.x;
	mPosition.y = v.y;
	mPosition.z = v.z;
}

void JDrama::TActor::JSGGetScaling(Vec* v) const { *v = mScaling; }

void JDrama::TActor::JSGSetScaling(const Vec& v)
{
	mScaling.x = v.x;
	mScaling.y = v.y;
	mScaling.z = v.z;
}

void JDrama::TActor::JSGGetRotation(Vec* v) const { *v = mRotation; }

void JDrama::TActor::JSGSetRotation(const Vec& v)
{
	mRotation.x = v.x;
	mRotation.y = v.y;
	mRotation.z = v.z;
}

int JDrama::TActor::getType() const { return 1; }
