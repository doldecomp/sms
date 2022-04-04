#ifndef NPCBALLOON_HPP
#define NPCBALLOON_HPP

#include "dolphin.h"

class TNpcBalloon {
    bool updateBalloon();
    void setNextMessage(u32, s32);
    int _000;
    int mBalloonAppearTimer;
};

#endif
