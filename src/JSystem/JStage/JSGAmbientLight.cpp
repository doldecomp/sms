#include "JSystem/JStage/JSGAmbientLight.hpp"
#include "dolphin/types.h"

JStage::TAmbientLight::~TAmbientLight() { 

}

s32 JStage::TAmbientLight::JSGFGetType() const { 
    return 4; 
}

GXColor JStage::TAmbientLight::JSGGetColor() const { 
    return (GXColor){0xFF, 0xFF, 0xFF, 0xFF}; 
}

void JStage::TAmbientLight::JSGSetColor(GXColor) { 

}