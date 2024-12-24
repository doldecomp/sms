#include "NPC/NpcBase.hpp"

// clang-format off

// TODO: convert to bitflags
static const u32 sBaseActionFlagTable[] = {
	0, 1, 2, 16, 32, 33, 0x80, 0x88, 0x40, 0, 0, 4, 0x1080, 0, 2, 16, 32, 8, 10, 0x18, 0x28, 0x80, 0, 0x0400, 0x4088,
};
static const u32 sBaseActionFlagTable2[] = {
	0, 1, 0x80, 0x88, 0x20, 0x21, 0, 4, 0x40, 0x2080, 0, 0, 8, 0, 0x20, 0, 2
};
static const u32 sBaseActionFlagTable3[] = {
	0, 1, 0x88, 0x20, 0x21, 0x80, 0, 8, 0x100, 0x101, 0x188, 0x120, 0x121, 0x180, 0x100, 0x108,
};

// clang-format on

void TBaseNPC::setKinoActionFlag_()
{
	if (_16C > 0xF) {
		_16C = 0;
	}
	mActionFlag = sBaseActionFlagTable3[_16C];
}

void TBaseNPC::setMareActionFlag_()
{
	if (_16C > 0x10) {
		_16C = 0;
	}
	mActionFlag = sBaseActionFlagTable2[_16C];
}

void TBaseNPC::setMonteActionFlag_()
{
	if (_16C > 0x18) {
		_16C = 0;
	}
	mActionFlag = sBaseActionFlagTable[_16C];
}
