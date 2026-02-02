#include "NPC/NpcCoin.hpp"
#include "MSound/MSound.hpp"
#include "MSound/MSoundSE.hpp"
#include "MoveBG/ItemManager.hpp"
#include "MoveBG/MapObjBase.hpp"
#include "MoveBG/MapObjManager.hpp"
#include "System/MarDirector.hpp"
#include "types.h"




TNpcCoin::TNpcCoin(int arg1) {
    unk0 = nullptr;
    unk4 = 0;
    unk8.x = 0.0f;
    unk8.y = 0.0f;
    unk8.z = 0.0f;
    unk14.x = 0.0f;
    unk14.y = 0.0f;
    unk14.z = 0.0f;
    unk0 = TMapObjBaseManager::newAndRegisterObjByEventID(arg1, nullptr);
    this->unk0->kill();
};

void TNpcCoin::updateCoin() {
    bool bVar2;
    bool bVar3;
    TMapObjBase* temp;
    if (0 < this->unk4) {
        bVar2 = true;
        bVar3 = true;
        if (gpMarDirector->unk124  != 1 && gpMarDirector->unk124  != 2 ) {
            bVar2 = false;
        }
        if (!bVar2) {
            bVar2 = true;
            if (gpMarDirector->unk124 != 3 && gpMarDirector->unk124  != 4) {
                bVar2 = false;
            }
        }
        if (!bVar2) {
            bVar3 = false;
        }
        if (!bVar3 && (this->unk4--, this->unk4 != 0)) {
            temp->appear();
        } else {
                temp = gpItemManager->makeObjAppear(unk8.x, unk8.y, unk8.z,0x2000000e,true);
                if (temp) {
                f32 float2;
                f32 float1;
                f32 float3;
                float1 = unk14.z;
                float2 = unk14.y;
                float3 = unk14.x;
                temp->unkAC.x = float3;
                temp->unkAC.y = float2;
                temp->unkAC.z = float1;
                temp->mLiveFlag &= ~0x10;
            } 
        }
        if (gpMSound->gateCheck(0x8807)) {
            MSoundSESystem::MSoundSE::startSoundNpcActor(0x8807, unk8, 0, nullptr, 0, 4);
        }
    }
}