#include <Strategic/SmplCharacter.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <stdio.h>

void TSMSSmplChara::load(JSUMemoryInputStream& stream)
{
	JDrama::TCharacter::load(stream);
	stream.readString(mPath, 256);
}

void* TSMSSmplChara::getRes(const char* name) const
{
	char fullPath[256];

  // BUG: name is unused lol
	snprintf(fullPath, 256, "%s%s", mPath, fullPath);
	return JKRGetResource(fullPath);
}
