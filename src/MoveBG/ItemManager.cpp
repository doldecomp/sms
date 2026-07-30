#include <MoveBG/ItemManager.hpp>
#include <MoveBG/Item.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TItemManager* gpItemManager;

void TItemManager::resetNozzleBoxesModel(int nozzle_type)
{
	for (int i = 0; i < getObjNum(); ++i) {
		THitActor* maybeBox = getObj(i);
		if (!maybeBox->isActorType(0x20000068))
			continue;

		TNozzleBox* box = (TNozzleBox*)maybeBox;
		if (nozzle_type != box->mContainedNozzleType)
			continue;

		if (box->checkLiveFlag(LIVE_FLAG_DEAD)) {
			JPABaseEmitter* emitter = gpMarioParticleManager->emit(
			    PARTICLE_MS_ENM_DISAP_A, &box->mPosition, 0, nullptr);
			if (emitter)
				emitter->setScale(JGeometry::TVec3<f32>(2.0f, 2.0f, 2.0f));

			SMSGetMSound()->startSoundActor(MSD_SE_SMOKE_EFFECT,
			                                &box->mPosition, 0, nullptr, 0, 4);
		}
		box->makeModelValid();
	}
}

TShine* TItemManager::makeShineAppearWithTime(const char* shine_name,
                                              int param_2, f32 x, f32 y, f32 z,
                                              int param_6, int param_7,
                                              int param_8)
{
	TShine* shine = JDrama::TNameRefGen::search<TShine>(shine_name);
	shine->mPosition.set(x, y, z);
	shine->appearWithTime(param_2, param_6, param_7, param_8);
	return shine;
}

TShine* TItemManager::makeShineAppearWithTimeOffset(const char*, int, f32, f32,
                                                    f32, int, int, int)
{
}

TShine* TItemManager::makeShineAppearWithDemo(const char* shine_name,
                                              const char* demo_name, f32 x,
                                              f32 y, f32 z)
{
	TShine* shine = JDrama::TNameRefGen::search<TShine>(shine_name);
	shine->mPosition.set(x, y, z);
	shine->appearWithDemo(demo_name);
	return shine;
}

TShine* TItemManager::makeShineAppearWithDemoOffset(const char* shine_name,
                                                    const char* demo_name,
                                                    f32 offset_x, f32 offset_y,
                                                    f32 offset_z)
{
	TShine* shine = JDrama::TNameRefGen::search<TShine>(shine_name);
	shine->mPosition.x += offset_x;
	shine->mPosition.y += offset_y;
	shine->mPosition.z += offset_z;
	shine->appearWithDemo(demo_name);
	return shine;
}

TCoin* TItemManager::newAndRegisterCoin(u32 event_id)
{
	TCoin* result;
	if (event_id < 0x32) {
		result = (TCoin*)newAndRegisterObj(
		    "coin_blue", JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
		    JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
		    JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	} else if (event_id == 100) {
		result = gpItemManager->unk78;
	} else if (event_id == 200) {
		result = (TCoin*)newAndRegisterObj(
		    "coin_red", JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
		    JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
		    JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	} else {
		return nullptr;
	}
	result->mEventId = event_id;
	return result;
}

void TItemManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		unk74 += unk70;
		if (unk74 > 360.0f)
			unk74 -= 360.0f;

		f32 fVar1        = JMASSin(unk74 * (65536.0f / 360.0f));
		f32 fVar2        = JMASCos(unk74 * (65536.0f / 360.0f));
		unk40.mMtx[0][0] = fVar2;
		unk40.mMtx[0][1] = 0.0;
		unk40.mMtx[0][2] = fVar1;
		unk40.mMtx[0][3] = 0.0;
		unk40.mMtx[1][0] = 0.0;
		unk40.mMtx[1][1] = 1.0;
		unk40.mMtx[1][2] = 0.0;
		unk40.mMtx[1][3] = 0.0;
		unk40.mMtx[2][0] = -fVar1;
		unk40.mMtx[2][1] = 0.0;
		unk40.mMtx[2][2] = fVar2;
		unk40.mMtx[2][3] = 0.0;
	}

	TMapObjBaseManager::perform(cue, graphics);
}

TCoin* TItemManager::newAndRegisterCoinReal()
{
	TCoin* coin = new TCoin("コイン");
	coin->initAndRegister("coin");
	return coin;
}

void TItemManager::loadAfter()
{
	TMapObjBaseManager::loadAfter();
	for (int i = 0; i < 20; ++i)
		newAndRegisterCoinReal();

	for (int i = 0; i < 2; ++i) {
		newAndRegisterObj("bottle_large",
		                  JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
		                  JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
		                  JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	}
}

void TItemManager::load(JSUMemoryInputStream& stream)
{
	TMapObjBaseManager::load(stream);
	unk78 = new TCoinEmpty;
	unk78->initHitActor(0x2000000E, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f);
}

TItemManager::TItemManager(const char* name)
    : TMapObjBaseManager(name)
{
	unk70 = 2.0f;
	unk74 = 0.0f;
	unk78 = nullptr;

	unk40.identity();

	gpItemManager = this;
}
