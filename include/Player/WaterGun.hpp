#ifndef WATERGUN_HPP
#define WATERGUN_HPP

#include <JSystem/JGeometry.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <Player/NozzleBase.hpp>
#include <Player/NozzleTrigger.hpp>

class TMario;

class TWaterGun {
public:
    typedef u8 TNozzleType;
    enum {
        Spray = 0,
        Rocket,
        Underwater,
        Yoshi,
        Hover,
        Turbo
    };

    TWaterGun(TMario *mario);

    virtual void perform(u32, JDrama::TGraphics *);

    void calcAnimation(JDrama::TGraphics *);
    void changeBackup();
    void changeNozzle(TNozzleType, bool);
    bool damage();
    void emit();
    // TNozzleBase *getCurrentNozzle() const;
    Mtx *getEmitMtx(int);
    void getEmitPosDirSpeed(int, JGeometry::TVec3<f32> *pos, JGeometry::TVec3<f32> *dir, JGeometry::TVec3<f32> *speed);
    Mtx *getNozzleMtx();
    f32 getPressure();
    f32 getPressureMax();
    void init();
    void initInLoadAfter();
    bool isEmitting();
    bool isPressureOn();
    void movement();
    void rotateProp(f32);
    void setAmountToRate(f32);
    void setBaseTRMtx(Mtx);
    bool suck(void);
    // void triggerPressureMovement(const TMarioControllerWork &);

    u16 mFlags;                             // 0x0000
    TMario *mMario;                         // 0x0008
    TNozzleBase mNozzleDeform;              // 0x000C
    TNozzleTrigger mNozzleDeformBomb;       // 0x0390
    TNozzleTrigger mNozzleRocket;           // 0x0720
    TNozzleBase mNozzleUnderWater;          // 0x0AB0
    TNozzleBase mNozzleYoshiDeform;         // 0x0E34
    TNozzleTrigger mNozzleYoshiDeformBomb;  // 0x11B8
    TNozzleTrigger mNozzleHover;            // 0x1548
    TNozzleTrigger mNozzleTurbo;            // 0x18D8
    TNozzleBase *mNozzleList[6];            // 0x1C68
    s32 mCurrentWater;                      // 0x1C80
    u8 mCurrentNozzle;                      // 0x1C84
    u8 mSecondNozzle;                       // 0x1C85
    bool mIsEmitWater;                      // 0x1C86
    u8 _13;                                 // 0x1C87
    u32 _14;                                // 0x1C88
    u8  mCurrentPressure;                   // 0x1c8c
    u8  mPreviousPressure;                  // 0x1c8d
    u8  _16;                                // 0x1c8e
    u8  _17;                                // 0x1c8f
    JGeometry::TVec3<f32> mEmitPos[4];                     // 0x1C90
    s16 _24;                                // 0x1CC0
    s16 _1cc2;                              // 0x1CC2
    s16 _1cc4;                              // 0x1CC4
    s16 _1cc6;                              // 0x1CC6
    f32 mNozzleSpeedY;                      // 0x1CC8
    f32 mNozzleSpeedZ;                      // 0x1CCC
    s16 _1cd0;                              // 0x1CD0
    s16 _1cd2;                              // 0x1CD2
    u32 mFluddModel;                    // 0x1CD4 // MActor*
    u8 mCurFluddTransformIdx;               // 0x1CD8
    u8 _27;                                 // 0x1CD9
    u16 _28;                                // 0x1CDA
    f32 _25;                                // 0x1CDC
    f32 _26;                                // 0x1CE0
    JGeometry::TVec3<f32> mGeometry3;       // 0x1CE4
    JGeometry::TVec3<f32> mGeometry4;       // 0x1CF0
    f32 _1cfc;                              // 0x1CFC
    f32 _1d00;                              // 0x1D00
    f32 _1d04;                              // 0x1D04
    u32 _18;                                // 0x1D08
    u32 mEmitInfo;                          // 0x1D0C // TWaterEmitInfo
    u32 _19;                                // 0x1D10
    
    struct TWatergunParams : public TParams {
        TParamT<f32> mRocketHeight;
        TParamT<f32> mHoverHeight;
        TParamT<f32> mLAngleNormal;
        TParamT<f32> mNozzleAngleYSpeed;
        TParamT<f32> mNozzleAngleYBrake;
        TParamT<f32> mNozzleAngleYSpeedMax;
        TParamT<s16> mHoverRotMax;
        TParamT<f32> mChangeSpeed;
    } mWatergunParams;  // 0x1d14
};

#endif