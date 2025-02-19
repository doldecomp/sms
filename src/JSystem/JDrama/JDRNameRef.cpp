#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

using namespace JDrama;

u16 TNameRef::calcKeyCode(const char* name)
{
	u32 result = 0;
	while ((s32)(*(u8*)name) != 0) {
		u32 tmp = result * 3;
		result  = *(u8*)name++ + tmp;
	}
	return result;
}

const char* TNameRef::getType(JSUMemoryInputStream& param_1,
                              JSUMemoryInputStream& param_2)
{
	u32 x = param_1.readU32();

	// TODO: wtf is happening here
	param_2.setBuffer((u8*)param_1.mBuffer + param_1.mPosition, x - 4);
	param_1.skip(x - 4);

	u32 len = param_2.readU16();
	return param_2.readString();
}

TNameRef* TNameRef::genObject(JSUMemoryInputStream& param_1,
                              JSUMemoryInputStream& param_2)
{
	return TNameRefGen::getInstance()->getNameRef(getType(param_1, param_2));
}

TNameRef::~TNameRef() { }

int TNameRef::getType() const { return 0; }

void TNameRef::load(JSUMemoryInputStream& stream)
{
	mKeyCode = stream.readU16();
	mName    = stream.readString();
}

void TNameRef::save(JSUMemoryOutputStream&) { }

void TNameRef::loadAfter() { }

TNameRef* TNameRef::searchF(u16 key, const char* name)
{
	bool match = false;
	if (mKeyCode == key && strcmp(mName, name) == 0)
		match = true;

	if (match)
		return this;
	else
		return nullptr;
}
