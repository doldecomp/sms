#include <Strategic/SharedParts.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/ObjModel.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>

TSharedParts::TSharedParts(const TLiveActor* actor, int anmMtxIndex,
                           const char* resName, u32 loadFlags, u32 modelFlags,
                           const char* name)
    : JDrama::TViewObj(name)
    , mActor(actor)
    , mConnectedAnmMtxIndex(anmMtxIndex)
    , mMActor(nullptr)
{
	void* res             = JKRGetResource(resName);
	J3DModelData* data    = J3DModelLoaderDataBase::load(res, loadFlags);
	SDLModelData* sdlData = new SDLModelData(data);
	SDLModel* model       = new SDLModel(sdlData, modelFlags, 1);
	mMActor = new MActor(mActor->getActorKeeper()->getMActorAnmData());
	mMActor->setModel(model, 0);
}

TSharedParts::TSharedParts(const TLiveActor* actor, int anmMtxIndex,
                           SDLModelData* modelData, u32 modelFlags,
                           const char* name)
    : JDrama::TViewObj(name)
    , mActor(actor)
    , mConnectedAnmMtxIndex(anmMtxIndex)
    , mMActor(nullptr)
{
	SDLModel* model = new SDLModel(modelData, modelFlags, 1);
	mMActor         = new MActor(mActor->getActorKeeper()->getMActorAnmData());
	mMActor->setModel(model, 0);
}

TSharedParts::TSharedParts(const TLiveActor* actor, int anmMtxIndex,
                           MActor* mActorParam, const char* name)
    : JDrama::TViewObj(name)
    , mActor(actor)
    , mConnectedAnmMtxIndex(anmMtxIndex)
    , mMActor(mActorParam)
{
}

MtxPtr TSharedParts::getConnectedMtx() const
{
	int idx = mConnectedAnmMtxIndex;
	return idx == -1 ? mActor->getModel()->getBaseTRMtx()
	                 : mActor->getModel()->getAnmMtx(idx);
}

void TSharedParts::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mActor->checkLiveFlag(LIVE_FLAG_BIT_AIRBORNE))
		return;

	if (cue & CUE_CALC_ANIM)
		mMActor->getModel()->setBaseTRMtx(getConnectedMtx());

	if (cue & CUE_ENTRY)
		mMActor->setLightData(mActor->getGroundPlane(), mActor->mPosition);

	mMActor->perform(cue, graphics);
}
