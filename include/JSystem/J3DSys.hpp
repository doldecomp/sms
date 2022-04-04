#ifndef J3DSYS_HPP
#define J3DSYS_HPP

#include "dolphin.h"

#include "types.h"

class J3DSys {
    public:
        void setTexCacheRegion(GXTexCacheSize);
        char pad_000[0x34];
        s32 _034;
};

extern J3DSys j3dSys;

#endif
