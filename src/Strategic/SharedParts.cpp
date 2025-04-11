#include <Strategic/SharedParts.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/ObjModel.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>

TSharedParts::TSharedParts(const TLiveActor* param_1, int param_2,
                           const char* param_3, u32 param_4, u32 param_5,
                           const char* name)
    : JDrama::TViewObj(name)
    , unk10(param_1)
    , mConnectedAnmMtxIndex(param_2)
    , unk18(nullptr)
{
	void* res             = JKRGetResource(param_3);
	J3DModelData* data    = J3DModelLoaderDataBase::load(res, param_4);
	SDLModelData* sdlData = new SDLModelData(data);
	SDLModel* model       = new SDLModel(sdlData, param_5, 1);
	unk18 = new MActor(unk10->getActorKeeper()->getMActorAnmData());
	unk18->setModel(model, 0);
}

TSharedParts::TSharedParts(const TLiveActor* param_1, int param_2,
                           SDLModelData* param_3, u32 param_4, const char* name)
    : JDrama::TViewObj(name)
    , unk10(param_1)
    , mConnectedAnmMtxIndex(param_2)
    , unk18(nullptr)
{
	SDLModel* model = new SDLModel(param_3, param_4, 1);
	unk18           = new MActor(unk10->getActorKeeper()->getMActorAnmData());
	unk18->setModel(model, 0);
}

TSharedParts::TSharedParts(const TLiveActor* param_1, int param_2,
                           MActor* param_3, const char* name)
    : JDrama::TViewObj(name)
    , unk10(param_1)
    , mConnectedAnmMtxIndex(param_2)
    , unk18(param_3)
{
}

MtxPtr TSharedParts::getConnectedMtx() const
{
	int idx = mConnectedAnmMtxIndex;
	return idx == -1 ? unk10->getModel()->getBaseTRMtx()
	                 : unk10->getModel()->getAnmMtx(idx);
}

void TSharedParts::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (unk10->checkLiveFlag(7))
		return;

	if (param_1 & 2)
		unk18->unk4->setBaseTRMtx(getConnectedMtx());

	if (param_1 & 0x200)
		unk18->setLightData(unk10->getUnkC4(), unk10->getPosition());

	unk18->perform(param_1, param_2);
}
