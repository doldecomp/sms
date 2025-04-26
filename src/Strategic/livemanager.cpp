#include <Strategic/LiveManager.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/spcinterp.hpp>
#include <Enemy/Conductor.hpp>
#include <MarioUtil/LightUtil.hpp>
#include <Camera/CubeManagerBase.hpp>

TLiveManager::TLiveManager(const char* name)
    : TObjManager(name)
    , unk34(nullptr)
{
	gpConductor->registerManager(this);
	// TODO: inlines
	gpLightManager->unk14[1]->unk20 = 1;
}

void TLiveManager::createSpcBinary()
{
	void* res = unk1C->TObjChara::getRes("/default.sb");
	if (res) {
		unk34 = new TSpcTypedBinary<TLiveActor>(res);
		unk34->init();
	}
}

void TLiveManager::load(JSUMemoryInputStream& stream)
{
	TObjManager::load(stream);
	createSpcBinary();
}

void TLiveManager::manageActor(TLiveActor* actor)
{
	actor->unk7C = unk14;
	TObjManager::manageObj(actor);
}

void TLiveManager::clipActorsAux(JDrama::TGraphics* param_1, float param_2,
                                 float param_3)
{
}

void TLiveManager::clipActors(JDrama::TGraphics* param_1)
{
	clipActorsAux(param_1, 4000.0f, 200.0f);
}

void TLiveManager::setFlagOutOfCube()
{
	for (int i = 0; i < unk14; ++i) {
		TLiveActor* actor         = (TLiveActor*)unk18[i];
		JGeometry::TVec3<f32> pos = actor->mPosition;
		pos.y += 75.0f;
		if (gpCubeArea->isInAreaCube(pos)) {
			actor->mLiveFlag &= ~0x200;
		} else {
			actor->mLiveFlag |= 0x200;
		}
	}
}

void TLiveManager::perform(u32 param_1, JDrama::TGraphics* param_2) { }

void TLiveManager::getActorByFlag(u32) const { }

bool TLiveManager::hasMapCollision() const { }
