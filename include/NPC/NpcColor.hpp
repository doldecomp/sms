#ifndef NPC_NPC_COLOR_HPP
#define NPC_NPC_COLOR_HPP

#include <NPC/NpcInitData.hpp>

class MActor;

void SMS_InitChangeNpcColor(const MActor*, const TColorChangeInfo*, s16 param3,
                            const GXColor*);

#endif
