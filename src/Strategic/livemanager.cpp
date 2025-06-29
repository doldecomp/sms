#include <Strategic/LiveManager.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/spcinterp.hpp>
#include <System/TimeRec.hpp>
#include <Camera/Camera.hpp>
#include <Enemy/Conductor.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/LightUtil.hpp>
#include <Camera/CubeManagerBase.hpp>

TLiveManager::TLiveManager(const char* name)
    : TObjManager(name)
    , unk34(nullptr)
{
	gpConductor->registerManager(this);
	// TODO: inlines
	gpLightManager->getUnk14(1)->unk20 = 1;
}

void TLiveManager::createSpcBinary()
{
	void* res = getChara()->TObjChara::getRes("/default.sb");
	if (!res)
		return;

	unk34 = new TSpcTypedBinary<TLiveActor>(res);
	unk34->init();
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
	SetViewFrustumClipCheckPerspective(gpCamera->getFovy(),
	                                   gpCamera->getAspect(),
	                                   param_1->getUnkE8(), param_2);

	for (int i = 0; i < unk14; ++i) {
		TLiveActor* actor = getObj(i);
		if (!actor->checkLiveFlag(0x100)) {
			actor->offLiveFlag(0x4);
		} else {
			JGeometry::TVec3<f32> pos = actor->getPosition();
			pos.y += 75.0f;
			if (actor->checkLiveFlag(0x2000) && SMS_IsInOtherFastCube(pos)) {
				actor->onLiveFlag(0x4);
			} else {
				if (ViewFrustumClipCheck(param_1, &actor->mPosition, param_3))
					actor->offLiveFlag(0x4);
				else
					actor->onLiveFlag(0x4);
			}
		}
	}
}

void TLiveManager::clipActors(JDrama::TGraphics* param_1)
{
	clipActorsAux(param_1, 4000.0f, 200.0f);
}

void TLiveManager::setFlagOutOfCube()
{
	for (int i = 0; i < objNum(); ++i) {
		TLiveActor* actor         = getObj(i);
		JGeometry::TVec3<f32> pos = actor->mPosition;
		pos.y += 75.0f;
		if (gpCubeArea->isInAreaCube(pos))
			actor->offLiveFlag(0x200);
		else
			actor->onLiveFlag(0x200);
	}
}

void TLiveManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{

	if (param_1 & 2) {
		if (unk30 & 1)
			TTimeRec::startTimer(0xffffffff);
		clipActors(param_2);
		setFlagOutOfCube();
		if (unk30 & 1)
			TTimeRec::endTimer();
	}

	TObjManager::perform(param_1, param_2);
}

const TLiveActor* TLiveManager::getActorByFlag(u32 flag) const
{
	for (int i = 0; i < unk14; ++i) {
		const TLiveActor* actor = getObj(i);
		if (actor->checkLiveFlag(flag))
			return actor;
	}
	return nullptr;
}

BOOL TLiveManager::hasMapCollision() const
{
	if (unk14 == 0)
		return false;

	return getObj(0)->hasMapCollision();
}
