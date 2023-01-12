#ifndef MARIOMAIN_HPP
#define MARIOMAIN_HPP

#include "dolphin.h"
#include "JSystem/JGeometry.hpp"
#include "JSystem/JDRNameRef.hpp"
#include "JSystem/JDRPlacement.hpp"
#include "Player/MarioInit.hpp"

class TActor : public JDrama::TPlacement {
    public:
        // TActor
        char _020[0x24];
};

class THitActor : public TActor {
    public:
        THitActor **collisions;
        u16 colCount;
        u16 colCapacity;
        void *actorType; // ActorType

        f32 attackRadius; // 0x50
        f32 attackHeight;

        f32 damageRadius;
        f32 damageHeight;
        
        f32 entryRadius;

        u32 _064;
};

class TTakeActor : public THitActor {
    public:
        TTakeActor *holder; // _068
        TTakeActor *held;   // _06C
};

// TODO: Add fields
class TMario : public TTakeActor {
    public:
        void windMove(const JGeometry::TVec3<f32>&);
        void flowMove(const JGeometry::TVec3<f32>&);
        void warpRequest(const JGeometry::TVec3<f32>&, f32);
        u32 onYoshi() const;
        void throwMario(const JGeometry::TVec3<f32>&, f32);
        u32 askJumpIntoWaterEffectExist() const;


        u32 _070;
        u32 input;
        u32 _078;
        u32 action;
        u32 prevAction;
        s16 actionState;
        s16 actionTimer;
        u32 actionArg;
        f32 intendedMag;
        s16 intendedYaw;
        u16 _092;

        JGeometry::TVec3<s16> faceAngle; // 0x94
        u16 modelFaceAngle;
        u32 _09C;
        u32 _0A0;
        JGeometry::TVec3<f32> vel; // 0xA4

        f32 forwardVel;
        f32 slideVelX;
        f32 slideVelZ;

        char _0BC[0x1C];

        // TODO: TBGCheckData
        void *wall; // TBGCheckData 0xD8
        void *ceil; // TBGCheckData 0xDC
        void *floor; // TBGCheckData 0xE0
        void *waterFloor; // TBGCheckData 0xE4

        JGeometry::TVec3<f32> floorPosition; // 0xE8

        s16 slopeAngle;
        u16 _0F6;

        u16 lightID; // 0xF8
        // u16 _0FA;

        u32 _0FC[2];

        void *controller; // TMarioControllerWork

        u32 _108[4];

        u32 _118; // gpMarioFlag points here;

        u32 _11C;

        s16 health; // 0x0120

        u16 _122;
        char _124[0x264];

        u16 blooperColor; //TODO: Make enum (0 = red, 1 = yellow, 2 = green)

        char _38A[0x5A];

        void *waterGun; // TWaterGun 0x3E4

        u32 _3E8;
        u32 _3EC;

        void *yoshi; // TYoshi 0x3F0

        char _3F4[0x0FC];

        JGeometry::TVec3<f32> _4F0;

        void *gamePad; // TMarioGamePad

        char _500[0x74];

        // start of TDeParams at 0x574
        TDeParams deParams;

        // TODO: Should these be an array indexed by an enum?
        TBodyAngleParams bodyAngleParamsFree;
        TBodyAngleParams bodyAngleParamsWaterGun;

        TAttackParams attackParamsFencePunch;
        TAttackParams attackParamsKickRoof;
        TJumpParams jumpParams;
        TRunParams runParams;
        TSwimParams swimParams;
        THangingParams hangingParams;
        THangRoofParams hangRoofParams;
        TWireParams wireParams;

        // TODO: Should these be an array indexed by an enum?
        TPullParams pullParamsBGBeak;
        TPullParams pullParamsBGTentacle;
        TPullParams pullParamsBGFireWanWanBossTail;
        TPullParams pullParamsFireWanWanTail;

        TBarParams barParams;

        TSurfingParams surfingParamsWaterRed;
        TSurfingParams surfingParamsGroundRed;
        TSurfingParams surfingParamsWaterYellow;
        TSurfingParams surfingParamsGroundYellow;
        TSurfingParams surfingParamsWaterGreen;
        TSurfingParams surfingParamsGroundGreen;

        THoverParams hoverParams;
        TDivingParams divingParams;
        TYoshiParams yoshiParams;
        TWaterEffectParams waterEffectParams;
        TControllerParams controllerParams;
        TGraffitoParams graffitoParams;
        TDirtyParams dirtyParams;
        TMarioMotorParams marioMotorParams;
        TMarioParticleParams marioParticleParams;
        TMarioEffectParams marioEffectParams;

        // TODO: Should these be an array indexed by an enum?
        TSlipParams slipParamsNormal;
        TSlipParams slipParamsOil;
        TSlipParams slipParamsAll;
        TSlipParams slipParamsAllSlider;
        TSlipParams slipParams45;
        TSlipParams slipParamsWaterSlope;
        TSlipParams slipParamsWaterGround;
        TSlipParams slipParamsYoshi;

        TUpperBodyParams upperBodyParams;

        // TODO: Should these be an array indexed by an enum?
        TDmgParams dmgParamsEnemyCommon;
        TDmgParams dmgParamsHamakuri;
        TDmgParams dmgParamsNamekuri;
        TDmgParams dmgParamsHinokuri;
        TDmgParams dmgParamsFire;
        TDmgParams dmgParamsBGTentacle;
        TDmgParams dmgParamsBossEel;
        TDmgParams dmgParamsHanachanBoss;
        TDmgParams dmgParamsPoihana;
        TDmgParams dmgParamsKiller;
        TDmgParams dmgParamsLampTrapIron;
        TDmgParams dmgParamsLampTrapSpike;
        TDmgParams dmgParamsEnemyMario;
        TDmgParams dmgParamsCannotBreath;
        TDmgParams dmgParamsGraffitoFire;
        TDmgParams dmgParamsGraffitoPoison;
        TDmgParams dmgParamsGraffitoElec;
        TDmgParams dmgParamsGraffitoLava;
        TDmgParams dmgParamsWaterSurface;

        TDmgParams dmgMapParams0;
        TDmgParams dmgMapParams1;
        TDmgParams dmgMapParams2;
        TDmgParams dmgMapParams3;
        TDmgParams dmgMapParams4;
        TDmgParams dmgMapParams5;
        TDmgParams dmgMapParams6;
        TDmgParams dmgMapParams7;
        TDmgParams dmgMapParams8;
        TDmgParams dmgMapParams9;

        TAutoDemoParams autoDemoParams;
        TSoundParams soundParams;
        TOptionParams optionParams;

        char _4290[0x80];
};

extern TMario *gpMarioOriginal;

#endif
