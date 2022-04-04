#ifndef JDRPLACEMENT_HPP
#define JDRPLACEMENT_HPP

#include "dolphin.h"
#include "types.h"

#include "JSystem/JDRViewObj.hpp"
#include "JSystem/JSUInputStream.hpp"
#include "JSystem/JGeometry.hpp"


namespace JDrama {
    class TPlacement : public TViewObj {
        public:
            virtual ~TPlacement() {};
            virtual void load(JSUMemoryInputStream &);

            virtual void _vt01C() = 0;
            virtual void _vt020() = 0;

            JGeometry::TVec3<f32> position;
            u32 _01C;
    };
};

#endif
