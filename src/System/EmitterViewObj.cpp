#include <System/EmitterViewObj.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <System/MarDirector.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <System/Application.hpp>
#include <Camera/Camera.hpp>

// TODO: place in correct header
static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

TMarioEmitterCallBackBindToPosPtr emitterCallBackBindToPosPtr;
TMarioEmitterCallBackBindToMtxPtr emitterCallBackBindToMtxPtr;
TMarioEmitterCallBackBindToSRTMtxPtr emitterCallBackBindToSRTMtxPtr;
TMarioParticleManager* gpMarioParticleManager = nullptr;

TEmitterViewObj::TEmitterViewObj(JPAEmitterManager* param_1, const char* name)
    : JDrama::TViewObj(name)
    , unk10(param_1)
{
}

void TEmitterViewObj::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 0x2) {
		for (int i = SMSGetAnmFrameRate(); i > 0; --i)
			unk10->calc();
	}

	if (param_1 & 0x8) {
		JPADrawInfo drawInfo;
		drawInfo.unk0 = param_2->getUnkB4();
		drawInfo.unk4 = 45.0f;
		drawInfo.unk8 = 1.218f;
		unk10->draw(&drawInfo);
	}
}

void TEmitterIndirectViewObj::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 0x2) {
		for (int i = SMSGetAnmFrameRate(); i > 0; --i)
			unk10->calc();
	}

	if (param_1 & 0x8) {
		SMS_DrawInit();
		JPADrawInfo drawInfo;
		drawInfo.unk0 = param_2->getUnkB4();
		drawInfo.unk4 = 45.0f;
		drawInfo.unk8 = 1.218f;

		drawInfo.unk4 = gpCamera->getFovy();
		drawInfo.unk8 = gpCamera->getAspect();
		unk10->draw(&drawInfo);
	}
}

void TMarioEmitterCallBackBindToPosPtr::execute(JPABaseEmitter* emitter)
{
	Vec* vec = (Vec*)emitter->unk120;

	emitter->unk160.x = vec->x;
	emitter->unk160.y = vec->y;
	emitter->unk160.z = vec->z;
}

void TMarioEmitterCallBackBindToPosPtr::draw(JPABaseEmitter*) { }

void TMarioEmitterCallBackBindToMtxPtr::execute(JPABaseEmitter* emitter)
{
	MtxPtr mtx = (MtxPtr)emitter->unk120;
	emitter->setGlobalRTMatrix(mtx);
}

void TMarioEmitterCallBackBindToMtxPtr::draw(JPABaseEmitter*) { }

void TMarioEmitterCallBackBindToSRTMtxPtr::execute(JPABaseEmitter* emitter)
{
	MtxPtr mtx = (MtxPtr)emitter->unk120;
	emitter->setGlobalSRTMatrix(mtx);
}

void TMarioEmitterCallBackBindToSRTMtxPtr::draw(JPABaseEmitter*) { }

TMarioParticleManager::TInfo::TInfo()
    : unk0(nullptr)
    , unk4(nullptr)
{
	unk4            = nullptr;
	mPrevFrameFlags = 0;
	mFlags          = 0;
	mEmitter        = nullptr;
}

TMarioParticleManager::TMarioParticleManager(const char* name)
    : JDrama::TViewObj(name)
    , unk3B8(nullptr)
{
}

void TMarioParticleManager::createEffectInfoAry(int param_1)
{
	unk3B4 = param_1;
	for (int i = 0; i < 214; ++i)
		unk10[i] = new TInfo[unk3B4];
	for (int i = 0; i < 19; ++i)
		unk368[i] = new TInfo[unk3B4];
}

void TMarioParticleManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 0x2) {
		for (int i = SMSGetAnmFrameRate(); i > 0; --i)
			unk3B8->calc();

		for (int i = 486; i < 505; ++i) {
			for (int j = 0; j < unk3B4; ++j) {
				TInfo* info = &unk368[i - 486][j];
				if ((info->mPrevFrameFlags & INFO_FLAG_UNK4)
				    && (info->mFlags & INFO_FLAG_UNK4)) {
					JPABaseEmitter* emitter = info->mEmitter;
					if (emitter == nullptr) {
						emitTry(i, info, 3);
					} else if (emitter->isThing()) {
						emitter->unk1E8 = -1;
						emitter->unk11C |= 0x1;
						info->mEmitter = nullptr;
						emitTry(i, info, 3);
					}
				}
				if ((info->mPrevFrameFlags & INFO_FLAG_UNK4)
				    && !(info->mFlags & INFO_FLAG_UNK4)) {
					if (JPABaseEmitter* emitter = info->mEmitter) {
						emitter->unk1E8 = -1;
						emitter->unk11C |= 0x1;
						info->mEmitter = nullptr;
						info->unk0     = nullptr;
						info->unk4     = nullptr;
						info->unk4     = nullptr;
					}
				}
				info->mPrevFrameFlags = info->mFlags;
				info->offFlag(INFO_FLAG_UNK4);
			}
		}

		for (int i = 253; i < 467; ++i) {
			for (int j = 0; j < unk3B4; ++j) {
				TInfo* info = &unk10[i - 253][j];
				if ((info->mPrevFrameFlags & INFO_FLAG_UNK4)
				    && (info->mFlags & INFO_FLAG_UNK4)) {
					JPABaseEmitter* emitter = info->mEmitter;
					if (emitter == nullptr) {
						emitTry(i, info, 3);
					} else if (emitter->isThing()) {
						emitter->unk1E8 = -1;
						emitter->unk11C |= 0x1;
						info->mEmitter = nullptr;
						emitTry(i, info, 3);
					}
				}
				if ((info->mPrevFrameFlags & INFO_FLAG_UNK4)
				    && !(info->mFlags & INFO_FLAG_UNK4)) {
					if (JPABaseEmitter* emitter = info->mEmitter) {
						emitter->unk1E8 = -1;
						emitter->unk11C |= 0x1;
						info->mEmitter = nullptr;
						info->unk0     = nullptr;
						info->unk4     = nullptr;
						info->unk4     = nullptr;
					}
				}
				info->mPrevFrameFlags = info->mFlags;
				info->offFlag(INFO_FLAG_UNK4);
			}
		}
	}

	if (param_1 & 0x8) {
		if (param_1 & 0x40000000) {
			SMS_DrawInit();
			JPADrawInfo drawInfo;
			drawInfo.unk0 = param_2->getUnkB4();
			drawInfo.unk4 = 45.0f;
			drawInfo.unk8 = 1.218f;
			drawInfo.unk4 = gpCamera->getFovy();
			drawInfo.unk8 = gpCamera->getAspect();
			unk3B8->draw(&drawInfo, 2);
			unk3B8->draw(&drawInfo, 3);
		}

		if (param_1 & 0x80000000) {
			JPADrawInfo drawInfo;
			drawInfo.unk0 = param_2->getUnkB4();
			drawInfo.unk4 = 45.0f;
			drawInfo.unk8 = 1.218f;
			unk3B8->draw(&drawInfo, 0);
			unk3B8->draw(&drawInfo, 1);
		}
	}
}

JPABaseEmitter*
TMarioParticleManager::emit(s32 param_1, const JGeometry::TVec3<f32>* param_2,
                            u8 param_3, const void* param_4)
{
	if (param_3 == 0)
		if (JPABaseEmitter* emitter = unk3B8->createSimpleEmitterID(
		        *param_2, param_1, param_3, 0, nullptr, nullptr))
			return emitter;

	if (param_3 == 2)
		if (JPABaseEmitter* emitter = unk3B8->createSimpleEmitterID(
		        *param_2, param_1, param_3, 0, nullptr, nullptr)) {
			emitter->mDraw.swapImage(
			    gpScreenTexture->getTexture()->getTexInfo(),
			    emitter->mDraw.getMainTextureID(0));
			return emitter;
		}

	if (param_3 == 1) {
		int tmp     = param_1 - 253;
		int idx     = getAvailableIdx(tmp, param_3, param_4);
		TInfo* info = &unk10[tmp][idx];
		info->onFlag(INFO_FLAG_UNK4);
		info->offFlag(INFO_FLAG_BIND_TO_POS);
		info->offFlag(INFO_FLAG_BIND_TO_RT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_SRT_MTX);
		info->unk4 = param_2;
		info->unk0 = param_4;
		if (info->mEmitter == nullptr)
			emitTry(param_1, info, param_3);
		if (info->mEmitter != nullptr)
			return info->mEmitter;
	}

	if (param_3 == 3) {
		int tmp     = param_1 - 486;
		int idx     = getAvailableIdx(tmp, param_3, param_4);
		TInfo* info = &unk368[tmp][idx];
		info->onFlag(INFO_FLAG_UNK4);
		info->offFlag(INFO_FLAG_BIND_TO_POS);
		info->offFlag(INFO_FLAG_BIND_TO_RT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_SRT_MTX);
		info->unk4 = param_2;
		info->unk0 = param_4;
		if (info->mEmitter == nullptr)
			emitTry(param_1, info, param_3);
		if (info->mEmitter != nullptr)
			return info->mEmitter;
	}

	return nullptr;
}

JPABaseEmitter* TMarioParticleManager::emitWithRotate(
    s32 param_1, const JGeometry::TVec3<f32>* param_2, s16 param_3, s16 param_4,
    s16 param_5, u8 param_6, const void* param_7)
{

	if (param_6 == 0)
		if (JPABaseEmitter* emitter = unk3B8->createSimpleEmitterID(
		        *param_2, param_1, param_6, 0, nullptr, nullptr)) {
			emitter->setRotation(param_3, param_4, param_5);
			return emitter;
		}

	if (param_6 == 2)
		if (JPABaseEmitter* emitter = unk3B8->createSimpleEmitterID(
		        *param_2, param_1, param_6, 0, nullptr, nullptr)) {
			emitter->setRotation(param_3, param_4, param_5);
			emitter->mDraw.swapImage(
			    gpScreenTexture->getTexture()->getTexInfo(),
			    emitter->mDraw.getMainTextureID(0));
			return emitter;
		}

	return nullptr;
}

JPABaseEmitter*
TMarioParticleManager::emitAndBindToPosPtr(s32 param_1,
                                           const JGeometry::TVec3<f32>* param_2,
                                           u8 param_3, const void* param_4)
{
	if (param_3 == 0)
		if (JPABaseEmitter* emitter = unk3B8->createSimpleEmitterID(
		        *param_2, param_1, param_3, 0, nullptr, nullptr)) {
			emitter->unk120 = (void*)param_2;
			emitter->unk110 = &emitterCallBackBindToPosPtr;
			return emitter;
		}

	if (param_3 == 2)
		if (JPABaseEmitter* emitter = unk3B8->createSimpleEmitterID(
		        *param_2, param_1, param_3, 0, nullptr, nullptr)) {
			emitter->unk120 = (void*)param_2;
			emitter->unk110 = &emitterCallBackBindToPosPtr;
			emitter->mDraw.swapImage(
			    gpScreenTexture->getTexture()->getTexInfo(),
			    emitter->mDraw.getMainTextureID(0));
			return emitter;
		}

	if (param_3 == 1) {
		int tmp     = param_1 - 253;
		int idx     = getAvailableIdx(tmp, param_3, param_4);
		TInfo* info = &unk10[tmp][idx];
		info->onFlag(INFO_FLAG_UNK4);
		info->onFlag(INFO_FLAG_BIND_TO_POS);
		info->offFlag(INFO_FLAG_BIND_TO_RT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_SRT_MTX);
		info->unk4 = param_2;
		info->unk0 = param_4;
		if (info->mEmitter == nullptr)
			emitTry(param_1, info, param_3);
		if (info->mEmitter != nullptr)
			return info->mEmitter;
	}

	if (param_3 == 3) {
		int tmp     = param_1 - 486;
		int idx     = getAvailableIdx(tmp, param_3, param_4);
		TInfo* info = &unk368[tmp][idx];
		info->onFlag(INFO_FLAG_UNK4);
		info->onFlag(INFO_FLAG_BIND_TO_POS);
		info->offFlag(INFO_FLAG_BIND_TO_RT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_SRT_MTX);
		info->unk4 = param_2;
		info->unk0 = param_4;
		if (info->mEmitter == nullptr)
			emitTry(param_1, info, param_3);
		if (info->mEmitter != nullptr)
			return info->mEmitter;
	}

	return nullptr;
}

JPABaseEmitter* TMarioParticleManager::emitAndBindToMtxPtr(s32 param_1,
                                                           MtxPtr param_2,
                                                           u8 param_3,
                                                           const void* param_4)
{
	JGeometry::TVec3<f32> local_24;
	local_24.x = param_2[0][3];
	local_24.y = param_2[1][3];
	local_24.z = param_2[2][3];

	if (param_3 == 0)
		if (JPABaseEmitter* emitter = unk3B8->createSimpleEmitterID(
		        local_24, param_1, param_3, 0, nullptr, nullptr)) {
			emitter->unk120 = (void*)param_2;
			emitter->unk110 = &emitterCallBackBindToMtxPtr;
			return emitter;
		}

	if (param_3 == 2)
		if (JPABaseEmitter* emitter = unk3B8->createSimpleEmitterID(
		        local_24, param_1, param_3, 0, nullptr, nullptr)) {
			emitter->unk120 = (void*)param_2;
			emitter->unk110 = &emitterCallBackBindToMtxPtr;
			emitter->mDraw.swapImage(
			    gpScreenTexture->getTexture()->getTexInfo(),
			    emitter->mDraw.getMainTextureID(0));
			return emitter;
		}

	if (param_3 == 1) {
		int tmp     = param_1 - 253;
		int idx     = getAvailableIdx(tmp, param_3, param_4);
		TInfo* info = &unk10[tmp][idx];
		info->onFlag(INFO_FLAG_UNK4);
		info->onFlag(INFO_FLAG_BIND_TO_RT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_SRT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_POS);
		info->unk4 = param_2;
		info->unk0 = param_4;
		if (info->mEmitter == nullptr)
			emitTry(param_1, info, param_3);
		if (info->mEmitter != nullptr)
			return info->mEmitter;
	}

	if (param_3 == 3) {
		int tmp     = param_1 - 486;
		int idx     = getAvailableIdx(tmp, param_3, param_4);
		TInfo* info = &unk368[tmp][idx];
		info->onFlag(INFO_FLAG_UNK4);
		info->onFlag(INFO_FLAG_BIND_TO_RT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_SRT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_POS);
		info->unk4 = param_2;
		info->unk0 = param_4;
		if (info->mEmitter == nullptr)
			emitTry(param_1, info, param_3);
		if (info->mEmitter != nullptr)
			return info->mEmitter;
	}

	return nullptr;
}

JPABaseEmitter*
TMarioParticleManager::emitAndBindToSRTMtxPtr(s32 param_1, MtxPtr param_2,
                                              u8 param_3, const void* param_4)
{
	JGeometry::TVec3<f32> local_24;
	local_24.x = param_2[0][3];
	local_24.y = param_2[1][3];
	local_24.z = param_2[2][3];

	if (param_3 == 0)
		if (JPABaseEmitter* emitter = unk3B8->createSimpleEmitterID(
		        local_24, param_1, param_3, 0, nullptr, nullptr)) {
			emitter->unk120 = (void*)param_2;
			emitter->unk110 = &emitterCallBackBindToSRTMtxPtr;
			return emitter;
		}

	if (param_3 == 2)
		if (JPABaseEmitter* emitter = unk3B8->createSimpleEmitterID(
		        local_24, param_1, param_3, 0, nullptr, nullptr)) {
			emitter->unk120 = (void*)param_2;
			emitter->unk110 = &emitterCallBackBindToSRTMtxPtr;
			emitter->mDraw.swapImage(
			    gpScreenTexture->getTexture()->getTexInfo(),
			    emitter->mDraw.getMainTextureID(0));
			return emitter;
		}

	if (param_3 == 1) {
		int tmp     = param_1 - 253;
		int idx     = getAvailableIdx(tmp, param_3, param_4);
		TInfo* info = &unk10[tmp][idx];
		info->onFlag(INFO_FLAG_UNK4);
		info->onFlag(INFO_FLAG_BIND_TO_SRT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_RT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_POS);
		info->unk4 = param_2;
		info->unk0 = param_4;
		if (info->mEmitter == nullptr)
			emitTry(param_1, info, param_3);
		if (info->mEmitter != nullptr)
			return info->mEmitter;
	}

	if (param_3 == 3) {
		int tmp     = param_1 - 486;
		int idx     = getAvailableIdx(tmp, param_3, param_4);
		TInfo* info = &unk368[tmp][idx];
		info->onFlag(INFO_FLAG_UNK4);
		info->onFlag(INFO_FLAG_BIND_TO_SRT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_RT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_POS);
		info->unk4 = param_2;
		info->unk0 = param_4;
		if (info->mEmitter == nullptr)
			emitTry(param_1, info, param_3);
		if (info->mEmitter != nullptr)
			return info->mEmitter;
	}

	return nullptr;
}

JPABaseEmitter* TMarioParticleManager::emitAndBindToMtx(s32 param_1,
                                                        MtxPtr param_2,
                                                        u8 param_3,
                                                        const void* param_4)
{
	JGeometry::TVec3<f32> local_24;
	local_24.x = param_2[0][3];
	local_24.y = param_2[1][3];
	local_24.z = param_2[2][3];

	if (param_3 == 0)
		if (JPABaseEmitter* emitter = unk3B8->createSimpleEmitterID(
		        local_24, param_1, param_3, 0, nullptr, nullptr)) {
			emitter->setGlobalRTMatrix(param_2);
			return emitter;
		}

	if (param_3 == 2)
		if (JPABaseEmitter* emitter = unk3B8->createSimpleEmitterID(
		        local_24, param_1, param_3, 0, nullptr, nullptr)) {
			emitter->setGlobalRTMatrix(param_2);
			emitter->mDraw.swapImage(
			    gpScreenTexture->getTexture()->getTexInfo(),
			    emitter->mDraw.getMainTextureID(0));
			return emitter;
		}

	return nullptr;
}

JPABaseEmitter* TMarioParticleManager::emitParticleCallBack(
    s32 param_1, const JGeometry::TVec3<f32>* param_2, u8 param_3,
    JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>* param_4,
    const void* param_5)
{
	if (param_3 == 1) {
		int type    = param_1 - 253;
		int idx     = getAvailableIdx(type, param_3, param_4);
		TInfo* info = &unk10[type][idx];
		info->onFlag(INFO_FLAG_UNK4);
		info->offFlag(INFO_FLAG_BIND_TO_POS);
		info->offFlag(INFO_FLAG_BIND_TO_RT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_SRT_MTX);
		info->unk4 = param_2;
		info->unk0 = param_4;
		if (info->mEmitter == nullptr)
			emitTry(param_1, info, param_3);
		if (info->mEmitter != nullptr) {
			info->mEmitter->unk114 = param_4;
			return info->mEmitter;
		}
	}

	if (param_3 == 3) {
		int type    = param_1 - 486;
		int idx     = getAvailableIdx(type, param_3, param_4);
		TInfo* info = &unk368[type][idx];
		info->onFlag(INFO_FLAG_UNK4);
		info->offFlag(INFO_FLAG_BIND_TO_POS);
		info->offFlag(INFO_FLAG_BIND_TO_RT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_SRT_MTX);
		info->unk4 = param_2;
		info->unk0 = param_4;
		if (info->mEmitter == nullptr)
			emitTry(param_1, info, param_3);
		if (info->mEmitter != nullptr) {
			info->mEmitter->unk114 = param_4;
			return info->mEmitter;
		}
	}

	return nullptr;
}

void TMarioParticleManager::emitTry(s32 param_1,
                                    TMarioParticleManager::TInfo* param_2,
                                    u8 param_3)
{
	if (param_2->checkFlag(INFO_FLAG_BIND_TO_POS)) {
		param_2->mEmitter = unk3B8->createSimpleEmitterID(
		    *(const JGeometry::TVec3<f32>*)param_2->unk4, param_1, param_3, 0,
		    nullptr, nullptr);

		if (param_2->mEmitter != nullptr) {
			param_2->mEmitter->unk120 = (void*)param_2->unk4;
			param_2->mEmitter->unk110 = &emitterCallBackBindToPosPtr;
		}
	} else {
		if (param_2->checkFlag(INFO_FLAG_BIND_TO_RT_MTX
		                       | INFO_FLAG_BIND_TO_SRT_MTX)) {
			JGeometry::TVec3<f32> local_14;
			local_14.x = ((MtxPtr)param_2->unk4)[0][3];
			local_14.y = ((MtxPtr)param_2->unk4)[1][3];
			local_14.z = ((MtxPtr)param_2->unk4)[2][3];

			param_2->mEmitter = unk3B8->createSimpleEmitterID(
			    local_14, param_1, param_3, 0, nullptr, nullptr);

			if (param_2->mEmitter != nullptr) {
				param_2->mEmitter->unk120 = (void*)param_2->unk4;
				if (param_2->checkFlag(0x10))
					param_2->mEmitter->unk110 = &emitterCallBackBindToSRTMtxPtr;
				else
					param_2->mEmitter->unk110 = &emitterCallBackBindToMtxPtr;
			}
		} else {
			param_2->mEmitter = unk3B8->createSimpleEmitterID(
			    *(const JGeometry::TVec3<f32>*)param_2->unk4, param_1, param_3,
			    0, nullptr, nullptr);
		}
	}

	if (param_3 == 3 && param_2->mEmitter != nullptr) {
		JPABaseEmitter* emitter = param_2->mEmitter;
		emitter->mDraw.swapImage(gpScreenTexture->getTexture()->getTexInfo(),
		                         emitter->mDraw.getMainTextureID(0));
	}
}

int TMarioParticleManager::getAvailableIdx(s32 param_1, u8 param_2,
                                           const void* param_3)
{
	for (int i = 0; i < unk3B4; ++i) {
		if (param_2 == 3 && unk368[param_1][i].mEmitter != nullptr
		    && unk368[param_1][i].unk0 == param_3)
			return i;
		if (param_2 == 1 && unk10[param_1][i].mEmitter != nullptr
		    && unk10[param_1][i].unk0 == param_3)
			return i;
	}

	for (int i = 0; i < unk3B4; ++i) {
		if (param_2 == 3 && !unk368[param_1][i].checkFlag(0x4)
		    && unk368[param_1][i].mEmitter == nullptr)
			return i;
		if (param_2 == 1 && !unk10[param_1][i].checkFlag(0x4)
		    && unk10[param_1][i].mEmitter == nullptr)
			return i;
	}

	return 0;
}

void SMSSetEmitterPolColor(JPABaseEmitter* param_1, int param_2)
{
	if (param_1 == nullptr)
		return;
	if (param_2 < 0 || param_2 > 7)
		return;

	int value;

	if (param_2 == 6) {
		switch (gpMarDirector->mMap) {
		case 1:
			if (gpMarDirector->unk7D == 5) {
				value = 3;
				break;
			}
			// FALLTHROUGH

		case 2:
		case 55:
			value = 2;
			break;

		case 9:
		case 0:
		case 57:
			value = 1;
			break;

		case 3:
			value = 3;
			break;

		case 8:
			value = 4;
			break;

		case 6:
		case 7:
		case 14:
		case 56:
			value = 5;
			break;

		default:
			value = 0;
			break;
		}
	}

	static const GXColor prmarray[6] = {
		{ 0xB5, 0x84, 0x66, 0xFF }, { 0xFF, 0xE3, 0xA8, 0xFF },
		{ 0xB5, 0x84, 0x66, 0xFF }, { 0x6E, 0x67, 0x57, 0xFF },
		{ 0xEF, 0xB0, 0x2E, 0xFF }, { 0xB0, 0xBF, 0x30, 0xFF },
	};

	static const GXColor envarray[6] = {
		{ 0x62, 0x0E, 0x00, 0xFF }, { 0xD8, 0x30, 0x60, 0xFF },
		{ 0x62, 0x0E, 0x00, 0xFF }, { 0x00, 0x00, 0x00, 0xFF },
		{ 0xB7, 0x24, 0x08, 0xFF }, { 0x00, 0x73, 0x6C, 0xFF },
	};

	param_1->setParamColor(prmarray[value].r, prmarray[value].g,
	                       prmarray[value].b);

	param_1->setEnviColor(envarray[value].r, envarray[value].g,
	                      envarray[value].b);
}
