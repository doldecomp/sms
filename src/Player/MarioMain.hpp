#ifndef MARIOMAIN_HPP
#define MARIOMAIN_HPP

#include <dolphin.h>
#include "JSystem/JGeometry.hpp"


class TMarioOriginal {
    public:
        char _000[0x7C];
        u32 _07C;
};


// TODO: move to src/Strategic/HitActor.hpp
class THitActor {
    public:
        char _000[0x7C];
        u32 _07C; // status or action?
        char _080[0x58];

        void *_0D8; // wall pointer
        void *_0DC; // ceiling pointer
        void *_0E0; // floor pointer
};

// TODO: find a spot for this
class TMario : public THitActor{
    public:
        void windMove(const JGeometry::TVec3<float>&);
        void flowMove(const JGeometry::TVec3<float>&);
        void warpRequest(const JGeometry::TVec3<float>&, float);
        void onYoshi() const;
        void throwMario(const JGeometry::TVec3<float>&);

};


extern class TMario *gpMarioOriginal;


#endif
