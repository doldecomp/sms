#ifndef MARIOMAIN_HPP
#define MARIOMAIN_HPP

#include <dolphin.h>
#include "JSystem/JGeometry.hpp"
#include "JSystem/JDRNameRef.hpp"
#include "Player/MarioInit.hpp"

class TViewObj { // based on TNameRef
    public:
        char _000[0x8];
        // TViewObj start
        char _008[0x8];
};

class TPlacement : public TViewObj {
    public:
        // TPlacement
        JGeometry::TVec3<f32> position;
        u16 _01C;
};

class TActor : public TPlacement {
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
        u32 _074;
        u32 _078;
        u32 status; // rename to action?
        char _080[0x14];

        JGeometry::TVec3<s16> angle; // 0x94
        u32 _09C;
        u32 _0A0;
        JGeometry::TVec3<f32> speed; // 0xA4

        char _0B0[0x28];

        // TODO: TBGCheckData
        void *wall; // 0xD8
        void *ceil; // 0xDC
        void *floor; // 0xE0
        void *waterFloor; // 0xE4

        // floor pos? (used by SMS_IsMarioTouchGround4cm to compare with Mario->position.y)
        JGeometry::TVec3<f32> floorPosition; // 0xE8

        u32 _0F4;

        u16 lightID; // 0xF8
        // u16 _0FA;

        u32 _0FC[7];

        u32 _118; // gpMarioFlag points here;

        u32 _11C;

        s16 health; // 0x0120

        u16 _122;
        char _124[0x2C0];

        void *waterGun; // TWaterGun 0x3E4

        u32 _3E8;
        u32 _3EC;

        void *yoshi; // TYoshi 0x3F0

        char _3F4[0x180];

        // start of TDeParams at 0x574
        TDeParams deParams;

        char bodyAngleParams1[128]; // TBodyAngleParams 0x9A0
        char bodyAngleParams2[128]; // TBodyAngleParams 0xA20

        char attackParams1[48]; // TAttackParams 0xAA0
        char attackParams2[48]; // TAttackParams 0xAD0

        TJumpParams jumpParams; // 0xB00
};


extern TMario *gpMarioOriginal;


#endif
