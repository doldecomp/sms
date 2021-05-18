#ifndef NPCBALLOON_HPP
#define NPCBALLOON_HPP

class TNpcBalloon {
    bool updateBalloon(void);
    void setNextMessage(u32, s32);
    int _000;
    int mBalloonAppearTimer;
};

#endif
