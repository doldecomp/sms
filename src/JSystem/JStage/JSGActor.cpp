#include "JSystem/JStage/JSGActor.hpp"
#include "dolphin/types.h"

/* 8026DBBC-8026DC1C       .text __dt__Q26JStage6TActorFv */
JStage::TActor::~TActor() 
{

}

s32 JStage::TActor::JSGFGetType() const 
{
    return 2;
}

void JStage::TActor::JSGGetTranslation(Vec*) const 
{

}

void JStage::TActor::JSGSetTranslation(const Vec&) 
{

}

void JStage::TActor::JSGGetScaling(Vec*) const 
{

}

void JStage::TActor::JSGSetScaling(const Vec&) 
{

}

void JStage::TActor::JSGGetRotation(Vec*) const 
{

}

void JStage::TActor::JSGSetRotation(const Vec&) 
{

}

s32 JStage::TActor::JSGGetShape() const 
{ 
    return -1;
}

void JStage::TActor::JSGSetShape(u32) 
{

}

s32 JStage::TActor::JSGGetAnimation() const 
{ 
    return -1;
}

void JStage::TActor::JSGSetAnimation(u32) 
{

}

f32 JStage::TActor::JSGGetAnimationFrame() const 
{ 
    return 0.0f;
}

void JStage::TActor::JSGSetAnimationFrame(f32) 
{

}

f32 JStage::TActor::JSGGetAnimationFrameMax() const 
{ 
    return 0.0f; 
}