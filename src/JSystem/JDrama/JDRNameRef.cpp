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

const char* TNameRef::getType(JSUMemoryInputStream& stream,
                              JSUMemoryInputStream& remainder)
{
	// The layout of scene graph nodes is:
	// - 4 bytes: size of the node data (including this 4-byte size field)
	// - 2 bytes: ???
	// - string: the name of the node type
	// - The remainder of the node data, class-dependent

	u32 size = stream.readU32();

	remainder.setBuffer((u8*)stream.mBuffer + stream.mPosition, size - 4);
	stream.skip(size - 4);

	// TODO: probably the keycode, someone confirm this in dolphin
	int idk = remainder.readU16();
	return remainder.readString();
}

TNameRef* TNameRef::genObject(JSUMemoryInputStream& stream,
                              JSUMemoryInputStream& remainder)
{
	return TNameRefGen::getInstance()->getNameRef(getType(stream, remainder));
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
