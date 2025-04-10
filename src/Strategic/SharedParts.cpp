#include <Strategic/SharedParts.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/ObjModel.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>

TSharedParts::TSharedParts(const TLiveActor* param_1, int param_2,
                           const char* param_3, u32 param_4, u32 param_5,
                           const char* name)
    : JDrama::TViewObj(name)
{
}

TSharedParts::TSharedParts(const TLiveActor* param_1, int param_2,
                           SDLModelData* param_3, u32 param_4, const char* name)
    : JDrama::TViewObj(name)
    , unk10(param_1)
    , unk14(param_2)
{
	SDLModel* model = new SDLModel(param_3, param_4, 1);
	unk18           = new MActor(nullptr);
}

TSharedParts::TSharedParts(const TLiveActor* param_1, int param_2,
                           MActor* param_3, const char* name)
    : JDrama::TViewObj(name)
    , unk10(param_1)
    , unk14(param_2)
    , unk18(param_3)
{
}

void TSharedParts::getConnectedMtx() const { }

void TSharedParts::perform(u32, JDrama::TGraphics*) { }
