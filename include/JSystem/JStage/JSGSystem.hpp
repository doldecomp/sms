#ifndef JSG_SYSTEM_H
#define JSG_SYSTEM_H

#include "JSystem/JStage/JSGObject.hpp"

namespace JStage { 
    struct TSystem : public TObject { 
        virtual ~TSystem() = 0; 
        virtual s32 JSGFGetType() const; 
        virtual void* JSGFindObject(char const*, JStage::TEObject) const; 
        virtual bool JSGCreateObject(char const*, JStage::TEObject, u32); 
        virtual void JSGDestroyObject(JStage::TObject*); 
        virtual bool JSGGetSystemData(u32); 
        virtual void JSGSetSystemData(u32, u32); 
    };
}; // namespace JStage

#endif