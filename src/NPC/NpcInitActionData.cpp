#include <NPC/NpcBase.hpp>
#include <macros.h>

// TODO: convert to bitflags

void TBaseNPC::setMonteActionFlag_()
{
	static const u32 sBaseActionFlagTable[] = {
		0, 1, 2,  16, 32, 33, 0x80, 0x88, 0x40, 0, 0,      4,      0x1080,
		0, 2, 16, 32, 8,  10, 0x18, 0x28, 0x80, 0, 0x0400, 0x4088,
	};

	if (_16C > ARRAY_COUNT(sBaseActionFlagTable) - 1)
		_16C = 0;

	mActionFlag = sBaseActionFlagTable[_16C];
}

void TBaseNPC::setMareActionFlag_()
{
	static const u32 sBaseActionFlagTable[] = {
		0,      1, 0x80, 0x88, 0x20, 0x21, 0, 4, 0x40,
		0x2080, 0, 0,    8,    0,    0x20, 0, 2,
	};

	if (_16C > ARRAY_COUNT(sBaseActionFlagTable) - 1)
		_16C = 0;

	mActionFlag = sBaseActionFlagTable[_16C];
}

void TBaseNPC::setKinoActionFlag_()
{
	static const u32 sBaseActionFlagTable[] = {
		0,     1,     0x88,  0x20,  0x21,  0x80,  0,     8,
		0x100, 0x101, 0x188, 0x120, 0x121, 0x180, 0x100, 0x108,
	};

	if (_16C > ARRAY_COUNT(sBaseActionFlagTable) - 1)
		_16C = 0;

	mActionFlag = sBaseActionFlagTable[_16C];
}
