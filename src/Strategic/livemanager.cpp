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
	gpLightManager->getLightSet(LIGHT_TYPE_OBJECT)->enable();
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
	actor->mInstanceIndex = mObjNum;
	TObjManager::manageObj(actor);
}

void TLiveManager::clipActorsAux(JDrama::TGraphics* graphics, f32 clip_far,
                                 f32 actor_radius)
{
	SetViewFrustumClipCheckPerspective(gpCamera->getFovy(),
	                                   gpCamera->getAspect(),
	                                   graphics->getNearPlane(), clip_far);

	for (int i = 0; i < mObjNum; ++i) {
		TLiveActor* actor = getObj(i);
		if (!actor->checkLiveFlag(LIVE_FLAG_ENABLE_CLIPPING)) {
			actor->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		} else {
			JGeometry::TVec3<f32> pos = actor->getPosition();
			pos.y += 75.0f;
			if (actor->checkLiveFlag(LIVE_FLAG_UNK2000)
			    && SMS_IsInOtherFastCube(pos)) {
				actor->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);
			} else {
				if (ViewFrustumClipCheck(graphics, &actor->mPosition,
				                         actor_radius))
					actor->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
				else
					actor->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);
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
	for (int i = 0; i < getObjNum(); ++i) {
		TLiveActor* actor         = getObj(i);
		JGeometry::TVec3<f32> pos = actor->mPosition;
		pos.y += 75.0f;
		if (gpCubeArea->isInAreaCube(pos))
			actor->offLiveFlag(LIVE_FLAG_UNK200);
		else
			actor->onLiveFlag(LIVE_FLAG_UNK200);
	}
}

void TLiveManager::perform(u32 cue, JDrama::TGraphics* graphics)
{

	if (cue & CUE_CALC_ANIM) {
		if (unk30 & 1)
			TTimeRec::snapCPUTime(JUtility::TColor(0xff, 0xff, 0xff, 0xff));
		clipActors(graphics);
		setFlagOutOfCube();
		if (unk30 & 1)
			TTimeRec::snapCPUTime(0);
	}

	TObjManager::perform(cue, graphics);
}

const TLiveActor* TLiveManager::getActorByFlag(u32 flag) const
{
	for (int i = 0; i < mObjNum; ++i) {
		const TLiveActor* actor = getObj(i);
		if (actor->checkLiveFlag(flag))
			return actor;
	}
	return nullptr;
}

BOOL TLiveManager::hasMapCollision() const
{
	if (mObjNum == 0)
		return false;

	return getObj(0)->hasMapCollision();
}
