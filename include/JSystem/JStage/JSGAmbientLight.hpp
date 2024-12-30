#ifndef JSG_AMBIENT_LIGHT_H
#define JSG_AMBIENT_LIGHT_H

#include "JSystem/JStage/JSGObject.hpp"
#include "dolphin/gx/GXStruct.h"

namespace JStage { 
    struct TAmbientLight : public TObject { 
        virtual ~TAmbientLight() = 0; 
        virtual s32 JSGFGetType() const; 
        virtual GXColor JSGGetColor() const; 
        virtual void JSGSetColor(GXColor); 
    };
}; // namespace JStage

#endif