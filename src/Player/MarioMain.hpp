#ifndef MARIOMAIN_HPP
#define MARIOMAIN_HPP

#include <dolphin.h>
#include "JSystem/JGeometry.hpp"
#include "JSystem/JDRNameRef.hpp"

class TViewObj { // based on TNameRef
    public:
        char _000[0x8];
        // TViewObj start
        char _008[0x8];
};

class TPlacement : public TViewObj {
    public:
        // TPlacement
        JGeometry::TVec3<float> position;
        u16 _01C;
};

class TActor : public TPlacement {
    public:
        // TActor
        char _020[0x24];
};

class THitActor : public TActor {
    public:
        u32 _044;
        u32 _048;
        u32 _04C;
        char _050[0x18];
};

class TTakeActor : public THitActor {
    public:
        TTakeActor *holder;
        TTakeActor *held;
};

// TODO: split inheritance
class TMario : public TTakeActor {
    public:
        void windMove(const JGeometry::TVec3<float>&);
        void flowMove(const JGeometry::TVec3<float>&);
        void warpRequest(const JGeometry::TVec3<float>&, float);
        u32 onYoshi() const;
        void throwMario(const JGeometry::TVec3<float>&, float);


        u32 _070;
        u32 _074;
        u32 _078;
        u32 _07C; // status or action?
        char _080[0x2C];

        u32 _0AC;

        char _0B0[0x28];

        void *_0D8; // wall pointer
        void *_0DC; // ceiling pointer
        void *_0E0; // floor pointer

        u32 _0E4;
        u32 _0E8;

        f32 _0EC;

        u32 _0D0;

        char _0D4[0x24];

        u32 _118;
};


extern TMario *gpMarioOriginal;


#endif
