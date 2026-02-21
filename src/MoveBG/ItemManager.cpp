#include <MoveBG/ItemManager.hpp>
#include <MoveBG/Item.hpp>
#include <System/EmitterViewObj.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TItemManager* gpItemManager;

void TItemManager::resetNozzleBoxesModel(int param_1)
{
	for (int i = 0; i < getObjNum(); ++i) {
		THitActor* maybeBox = getObj(i);
		if (!maybeBox->isActorType(0x20000068))
			continue;

		TNozzleBox* box = (TNozzleBox*)maybeBox;
		if (param_1 != box->unk148)
			continue;

		if (box->checkLiveFlag(1)) {
			JPABaseEmitter* emitter = gpMarioParticleManager->emit(
			    0xE4, &box->mPosition, 0, nullptr);
			if (emitter) {
				emitter->unk154.set(2.0f, 2.0f, 2.0f);
				emitter->unk174.set(2.0f, 2.0f, 2.0f);
			}

			if (gpMSound->gateCheck(0x387D)) {
				MSoundSESystem::MSoundSE::startSoundActor(
				    0x387D, box->mPosition, 0, nullptr, 0, 4);
			}
		}
		box->makeModelValid();
	}
}

TShine* TItemManager::makeShineAppearWithTime(const char* param_1, int param_2,
                                              f32 param_3, f32 param_4,
                                              f32 param_5, int param_6,
                                              int param_7, int param_8)
{
	TShine* shine = JDrama::TNameRefGen::search<TShine>(param_1);
	shine->mPosition.set(param_3, param_4, param_5);
	shine->appearWithTime(param_2, param_6, param_7, param_8);
	return shine;
}

TShine* TItemManager::makeShineAppearWithTimeOffset(const char*, int, f32, f32,
                                                    f32, int, int, int)
{
}

TShine* TItemManager::makeShineAppearWithDemo(const char* param_1,
                                              const char* param_2, f32 param_3,
                                              f32 param_4, f32 param_5)
{
	TShine* shine = JDrama::TNameRefGen::search<TShine>(param_1);
	shine->mPosition.set(param_3, param_4, param_5);
	shine->appearWithDemo(param_2);
	return shine;
}

TShine* TItemManager::makeShineAppearWithDemoOffset(const char* param_1,
                                                    const char* param_2,
                                                    f32 param_3, f32 param_4,
                                                    f32 param_5)
{
	TShine* shine = JDrama::TNameRefGen::search<TShine>(param_1);
	shine->mPosition.x += param_3;
	shine->mPosition.y += param_4;
	shine->mPosition.z += param_5;
	shine->appearWithDemo(param_2);
	return shine;
}

TCoin* TItemManager::newAndRegisterCoin(u32 param_1)
{
	TCoin* result;
	if (param_1 < 0x32) {
		result = (TCoin*)newAndRegisterObj(
		    "coin_blue", JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
		    JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
		    JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	} else if (param_1 == 100) {
		result = gpItemManager->unk78;
	} else if (param_1 == 200) {
		result = (TCoin*)newAndRegisterObj(
		    "coin_red", JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
		    JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
		    JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	} else {
		return nullptr;
	}
	result->unk134 = param_1;
	return result;
}

void TItemManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 1) {
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

	TMapObjBaseManager::perform(param_1, param_2);
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
	unk78 = new TCoinEmpty("空コイン");
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
