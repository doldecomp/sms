#ifndef JSG_LIGHT_H
#define JSG_LIGHT_H

#include "JSystem/JStage/JSGObject.hpp"
#include "dolphin/mtx.h"
#include "dolphin/gx/GXEnum.h"
#include "dolphin/gx/GXStruct.h"

namespace JStage { 
    enum TELight { 
        TELIGHT_Unk0 = 0, 
        TELIGHT_Unk1 = 1, 
        TELIGHT_Unk2 = 2, 
        TELIGHT_Unk3 = 3, 
        
        // kLightMax = 0xFFFFFFFF, maybe not used? 
    }; 

    struct TLight : public TObject { 
        virtual ~TLight() = 0; 
        virtual s32 JSGFGetType() const; 
        virtual bool JSGGetLightType() const; 
        virtual void JSGSetLightType(JStage::TELight); 
        virtual void JSGGetPosition(Vec*) const; 
        virtual void JSGSetPosition(Vec const&); 
        virtual GXColor JSGGetColor() const; 
        virtual void JSGSetColor(GXColor); 
        virtual void JSGGetDistanceAttenuation(f32*, f32*, GXDistAttnFn*) const; 
        virtual void JSGSetDistanceAttenuation(f32, f32, GXDistAttnFn); 
        virtual void JSGGetAngleAttenuation(f32*, GXSpotFn*) const; 
        virtual void JSGSetAngleAttenuation(f32, GXSpotFn); 
        virtual void JSGGetDirection(Vec*) const; 
        virtual void JSGSetDirection(Vec const&); 
    };
}; // namespace JStage

#endif