#ifndef YOSHI_HPP
#define YOSHI_HPP

#include <JSystem/JGeometry.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <M3DUtil/MActor.hpp>

class TMario;

class TYoshi {
public:
    enum Color { GREEN, ORANGE, PURPLE, PINK };
    enum State { EGG = 0, DROWNING = 3, DYING = 4, UNMOUNTED = 6, MOUNTED = 8 };

    bool appearFromEgg(const JGeometry::TVec3<f32> &);
    void calcAnim();
    void changeAnimation(int id);
    bool disappear();
    void doEat(u32 fruitID);
    void doSearch();
    void entry();
    void getEmitPosDir(JGeometry::TVec3<f32> *, JGeometry::TVec3<f32> *) const;
    J3DFrameCtrl *getFrameCtrl() const;
    MtxPtr getMtxPtrFootL() const;
    MtxPtr getMtxPtrFootR() const;
    void getOff(bool knockedOff);
    void init(TMario *);
    void initInLoadAfter();
    void kill();
    void movement();
    bool onYoshi();
    void ride();
    void setEggYoshiPtr(void *); // TEggYoshi*
    void thinkAnimation();
    void thinkBtp(int);
    void thinkHoldOut();
    void thinkUpper();
    void viewCalc();

    s8 mState;                 // 0x0000
    u16 mSubState;             // 0x0002 ??
    u32 _01;                   // 0x0004
    s32 mMaxJuice;             // 0x0008
    s32 mCurJuice;             // 0x000C
    TMario *mMario;            // 0x0010
    u32 _02[0xC / 4];          // 0x0014
    JGeometry::TVec3<f32> mTranslation;       // 0x0020
    u32 _03[0x8 / 4];          // 0x002C
    MActor *mActor;            // 0x0034
    u32 _38;                   // 0x0038
    u16 mJoint;                // 0x003c
    u16 _3e;                   // 0x003e
    u32 _04[0x44 / 4];         // 0x0040
    f32 mRedComponent;         // 0x0084
    f32 mGreenComponent;       // 0x0088
    f32 mBlueComponent;        // 0x008C
    u32 _05[0x28 / 4];         // 0x0090
    u8 mFlutterState;          // 0x00B8
    u8 _06;                    // 0x00B9
    u16 mFlutterTimer;         // 0x00BA
    u16 mMaxFlutterTimer;      // 0x00BC
    u16 _07;                   // 0x00BE
    f32 mMaxVSpdStartFlutter;  // 0x00C0
    f32 mFlutterAcceleration;  // 0x00C4
    u32 _08[0x8 / 4];          // 0x00C8
    s8 mType;                  // 0x00D0
    u8 _09;                    // 0x00D1
    u16 _10;                   // 0x00D2
    u32 _11[0x1C / 4];         // 0x00D4
    void *mEgg;           // 0x00F0
    u32 _F4[0x30 / 4];
};

extern JUtility::TColor bodyColor[4];

#endif