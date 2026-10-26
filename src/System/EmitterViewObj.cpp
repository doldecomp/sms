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

TMarioEmitterCallBackBindToPosPtr emitterCallBackBindToPosPtr;
TMarioEmitterCallBackBindToMtxPtr emitterCallBackBindToMtxPtr;
TMarioEmitterCallBackBindToSRTMtxPtr emitterCallBackBindToSRTMtxPtr;
TMarioParticleManager* gpMarioParticleManager = nullptr;

TEmitterViewObj::TEmitterViewObj(JPAEmitterManager* emitter_manager,
                                 const char* name)
    : JDrama::TViewObj(name)
    , unk10(emitter_manager)
{
}

void TEmitterViewObj::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM) {
		for (int i = SMSGetAnmFrameRate(); i > 0; --i)
			unk10->calc();
	}

	if (cue & CUE_DRAW) {
		JPADrawInfo drawInfo(graphics->getViewMtx());
		unk10->draw(&drawInfo);
	}
}

void TEmitterIndirectViewObj::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM) {
		for (int i = SMSGetAnmFrameRate(); i > 0; --i)
			unk10->calc();
	}

	if (cue & CUE_DRAW) {
		SMS_DrawInit();
		JPADrawInfo drawInfo(graphics->getViewMtx());
		drawInfo.setFovy(gpCamera->getFovy());
		drawInfo.setAspect(gpCamera->getAspect());
		unk10->draw(&drawInfo);
	}
}

void TMarioEmitterCallBackBindToPosPtr::execute(JPABaseEmitter* emitter)
{
	JGeometry::TVec3<f32>* vec = (JGeometry::TVec3<f32>*)emitter->getUserWork();
	emitter->setGlobalTranslation(*vec);
}

void TMarioEmitterCallBackBindToPosPtr::draw(JPABaseEmitter*) { }

void TMarioEmitterCallBackBindToMtxPtr::execute(JPABaseEmitter* emitter)
{
	MtxPtr mtx = (MtxPtr)emitter->getUserWork();
	emitter->setGlobalRTMatrix(mtx);
}

void TMarioEmitterCallBackBindToMtxPtr::draw(JPABaseEmitter*) { }

void TMarioEmitterCallBackBindToSRTMtxPtr::execute(JPABaseEmitter* emitter)
{
	MtxPtr mtx = (MtxPtr)emitter->getUserWork();
	emitter->setGlobalSRTMatrix(mtx);
}

void TMarioEmitterCallBackBindToSRTMtxPtr::draw(JPABaseEmitter*) { }

TMarioParticleManager::TInfo::TInfo()
{
	mIdentity       = nullptr;
	mBoundPos       = nullptr;
	mBoundMtx       = nullptr;
	mPrevFrameFlags = 0;
	mFlags          = 0;
	mEmitter        = nullptr;
}

TMarioParticleManager::TMarioParticleManager(const char* name)
    : JDrama::TViewObj(name)
    , mEmitterManager(nullptr)
{
}

void TMarioParticleManager::createEffectInfoAry(
    int max_simultaneous_loop_emitters)
{
	mMaxSimultaneousLoopEmitters = max_simultaneous_loop_emitters;
	for (int i = 0; i < LOOP_NORMAL_COUNT; ++i)
		mLoopNormalInfo[i] = new TInfo[mMaxSimultaneousLoopEmitters];
	for (int i = 0; i < LOOP_INDIRECT_COUNT; ++i)
		mLoopIndirectInfo[i] = new TInfo[mMaxSimultaneousLoopEmitters];
}

void TMarioParticleManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM) {
		for (int i = SMSGetAnmFrameRate(); i > 0; --i)
			mEmitterManager->calc();

		for (int i = LOOP_INDIRECT_FIRST;
		     i < LOOP_INDIRECT_FIRST + LOOP_INDIRECT_COUNT; ++i) {
			for (int j = 0; j < mMaxSimultaneousLoopEmitters; ++j) {
				TInfo* info = &mLoopIndirectInfo[i - LOOP_INDIRECT_FIRST][j];
				if ((info->mPrevFrameFlags & INFO_FLAG_KEEP_ALIVE)
				    && (info->mFlags & INFO_FLAG_KEEP_ALIVE)) {
					JPABaseEmitter* emitter = info->mEmitter;
					if (emitter == nullptr) {
						emitTry(i, info, EFFECT_LOOP_INDIRECT);
					} else if (emitter->isEnableDeleteEmitter()) {
						emitter->becomeInvalidEmitter();
						info->mEmitter = nullptr;
						emitTry(i, info, EFFECT_LOOP_INDIRECT);
					}
				}
				if ((info->mPrevFrameFlags & INFO_FLAG_KEEP_ALIVE)
				    && !(info->mFlags & INFO_FLAG_KEEP_ALIVE)) {
					if (JPABaseEmitter* emitter = info->mEmitter) {
						emitter->becomeInvalidEmitter();
						info->mEmitter  = nullptr;
						info->mIdentity = nullptr;
						info->mBoundPos = nullptr;
						info->mBoundMtx = nullptr;
					}
				}
				info->mPrevFrameFlags = info->mFlags;
				info->offFlag(INFO_FLAG_KEEP_ALIVE);
			}
		}

		for (int i = LOOP_NORMAL_FIRST;
		     i < LOOP_NORMAL_FIRST + LOOP_NORMAL_COUNT; ++i) {
			for (int j = 0; j < mMaxSimultaneousLoopEmitters; ++j) {
				TInfo* info = &mLoopNormalInfo[i - LOOP_NORMAL_FIRST][j];
				if ((info->mPrevFrameFlags & INFO_FLAG_KEEP_ALIVE)
				    && (info->mFlags & INFO_FLAG_KEEP_ALIVE)) {
					JPABaseEmitter* emitter = info->mEmitter;
					if (emitter == nullptr) {
						emitTry(i, info, EFFECT_LOOP_NORMAL);
					} else if (emitter->isEnableDeleteEmitter()) {
						emitter->becomeInvalidEmitter();
						info->mEmitter = nullptr;
						emitTry(i, info, EFFECT_LOOP_NORMAL);
					}
				}
				if ((info->mPrevFrameFlags & INFO_FLAG_KEEP_ALIVE)
				    && !(info->mFlags & INFO_FLAG_KEEP_ALIVE)) {
					if (JPABaseEmitter* emitter = info->mEmitter) {
						emitter->becomeInvalidEmitter();
						info->mEmitter  = nullptr;
						info->mIdentity = nullptr;
						info->mBoundPos = nullptr;
						info->mBoundMtx = nullptr;
					}
				}
				info->mPrevFrameFlags = info->mFlags;
				info->offFlag(INFO_FLAG_KEEP_ALIVE);
			}
		}
	}

	if (cue & CUE_DRAW) {
		if (cue & CUE_UNK40000000) {
			SMS_DrawInit();
			JPADrawInfo drawInfo(graphics->getViewMtx());
			drawInfo.setFovy(gpCamera->getFovy());
			drawInfo.setAspect(gpCamera->getAspect());
			mEmitterManager->draw(&drawInfo, EFFECT_ONETIME_INDIRECT);
			mEmitterManager->draw(&drawInfo, EFFECT_LOOP_INDIRECT);
		}

		if (cue & CUE_UNK80000000) {
			JPADrawInfo drawInfo(graphics->getViewMtx());
			mEmitterManager->draw(&drawInfo, EFFECT_ONETIME_NORMAL);
			mEmitterManager->draw(&drawInfo, EFFECT_LOOP_NORMAL);
		}
	}
}

JPABaseEmitter* TMarioParticleManager::emit(s32 id,
                                            const JGeometry::TVec3<f32>* pos,
                                            u8 effect_type,
                                            const void* identity)
{
	if (effect_type == EFFECT_ONETIME_NORMAL)
		if (JPABaseEmitter* emitter = mEmitterManager->createSimpleEmitterID(
		        *pos, id, effect_type, 0, nullptr, nullptr))
			return emitter;

	if (effect_type == EFFECT_ONETIME_INDIRECT)
		if (JPABaseEmitter* emitter = mEmitterManager->createSimpleEmitterID(
		        *pos, id, effect_type, 0, nullptr, nullptr)) {
			emitter->mDraw.swapImage(
			    gpScreenTexture->getTexture()->getTexInfo(),
			    emitter->mDraw.getMainTextureID(0));
			return emitter;
		}

	if (effect_type == EFFECT_LOOP_NORMAL) {
		int table_idx = id - LOOP_NORMAL_FIRST;
		int idx       = getAvailableIdx(table_idx, effect_type, identity);
		TInfo* info   = &mLoopNormalInfo[table_idx][idx];
		info->onFlag(INFO_FLAG_KEEP_ALIVE);
		info->offFlag(INFO_FLAG_BIND_TO_POS);
		info->offFlag(INFO_FLAG_BIND_TO_RT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_SRT_MTX);
		info->mBoundPos = pos;
		info->mIdentity = identity;
		if (info->mEmitter == nullptr)
			emitTry(id, info, effect_type);
		if (info->mEmitter != nullptr)
			return info->mEmitter;
	}

	if (effect_type == EFFECT_LOOP_INDIRECT) {
		int table_idx = id - LOOP_INDIRECT_FIRST;
		int idx       = getAvailableIdx(table_idx, effect_type, identity);
		TInfo* info   = &mLoopIndirectInfo[table_idx][idx];
		info->onFlag(INFO_FLAG_KEEP_ALIVE);
		info->offFlag(INFO_FLAG_BIND_TO_POS);
		info->offFlag(INFO_FLAG_BIND_TO_RT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_SRT_MTX);
		info->mBoundPos = pos;
		info->mIdentity = identity;
		if (info->mEmitter == nullptr)
			emitTry(id, info, effect_type);
		if (info->mEmitter != nullptr)
			return info->mEmitter;
	}

	return nullptr;
}

JPABaseEmitter*
TMarioParticleManager::emitWithRotate(s32 id, const JGeometry::TVec3<f32>* pos,
                                      s16 rot_x, s16 rot_y, s16 rot_z,
                                      u8 effect_type, const void* identity)
{

	if (effect_type == EFFECT_ONETIME_NORMAL)
		if (JPABaseEmitter* emitter = mEmitterManager->createSimpleEmitterID(
		        *pos, id, effect_type, 0, nullptr, nullptr)) {
			emitter->setRotation(rot_x, rot_y, rot_z);
			return emitter;
		}

	if (effect_type == EFFECT_ONETIME_INDIRECT)
		if (JPABaseEmitter* emitter = mEmitterManager->createSimpleEmitterID(
		        *pos, id, effect_type, 0, nullptr, nullptr)) {
			emitter->setRotation(rot_x, rot_y, rot_z);
			emitter->mDraw.swapImage(
			    gpScreenTexture->getTexture()->getTexInfo(),
			    emitter->mDraw.getMainTextureID(0));
			return emitter;
		}

	return nullptr;
}

JPABaseEmitter*
TMarioParticleManager::emitAndBindToPosPtr(s32 id,
                                           const JGeometry::TVec3<f32>* pos,
                                           u8 effect_type, const void* identity)
{
	if (effect_type == EFFECT_ONETIME_NORMAL)
		if (JPABaseEmitter* emitter = mEmitterManager->createSimpleEmitterID(
		        *pos, id, effect_type, 0, nullptr, nullptr)) {
			emitter->setUserWork((uintptr_t)pos);
			emitter->setEmitterCallBackPtr(&emitterCallBackBindToPosPtr);
			return emitter;
		}

	if (effect_type == EFFECT_ONETIME_INDIRECT)
		if (JPABaseEmitter* emitter = mEmitterManager->createSimpleEmitterID(
		        *pos, id, effect_type, 0, nullptr, nullptr)) {
			emitter->setUserWork((uintptr_t)pos);
			emitter->setEmitterCallBackPtr(&emitterCallBackBindToPosPtr);
			emitter->mDraw.swapImage(
			    gpScreenTexture->getTexture()->getTexInfo(),
			    emitter->mDraw.getMainTextureID(0));
			return emitter;
		}

	if (effect_type == EFFECT_LOOP_NORMAL) {
		int table_idx = id - LOOP_NORMAL_FIRST;
		int idx       = getAvailableIdx(table_idx, effect_type, identity);
		TInfo* info   = &mLoopNormalInfo[table_idx][idx];
		info->onFlag(INFO_FLAG_KEEP_ALIVE);
		info->onFlag(INFO_FLAG_BIND_TO_POS);
		info->offFlag(INFO_FLAG_BIND_TO_RT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_SRT_MTX);
		info->mBoundPos = pos;
		info->mIdentity = identity;
		if (info->mEmitter == nullptr)
			emitTry(id, info, effect_type);
		if (info->mEmitter != nullptr)
			return info->mEmitter;
	}

	if (effect_type == EFFECT_LOOP_INDIRECT) {
		int table_idx = id - LOOP_INDIRECT_FIRST;
		int idx       = getAvailableIdx(table_idx, effect_type, identity);
		TInfo* info   = &mLoopIndirectInfo[table_idx][idx];
		info->onFlag(INFO_FLAG_KEEP_ALIVE);
		info->onFlag(INFO_FLAG_BIND_TO_POS);
		info->offFlag(INFO_FLAG_BIND_TO_RT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_SRT_MTX);
		info->mBoundPos = pos;
		info->mIdentity = identity;
		if (info->mEmitter == nullptr)
			emitTry(id, info, effect_type);
		if (info->mEmitter != nullptr)
			return info->mEmitter;
	}

	return nullptr;
}

JPABaseEmitter* TMarioParticleManager::emitAndBindToMtxPtr(s32 id, MtxPtr mtx,
                                                           u8 effect_type,
                                                           const void* identity)
{
	JGeometry::TVec3<f32> local_24;
	local_24.x = mtx[0][3];
	local_24.y = mtx[1][3];
	local_24.z = mtx[2][3];

	if (effect_type == EFFECT_ONETIME_NORMAL)
		if (JPABaseEmitter* emitter = mEmitterManager->createSimpleEmitterID(
		        local_24, id, effect_type, 0, nullptr, nullptr)) {
			emitter->setUserWork((uintptr_t)mtx);
			emitter->setEmitterCallBackPtr(&emitterCallBackBindToMtxPtr);
			return emitter;
		}

	if (effect_type == EFFECT_ONETIME_INDIRECT)
		if (JPABaseEmitter* emitter = mEmitterManager->createSimpleEmitterID(
		        local_24, id, effect_type, 0, nullptr, nullptr)) {
			emitter->setUserWork((uintptr_t)mtx);
			emitter->setEmitterCallBackPtr(&emitterCallBackBindToMtxPtr);
			emitter->mDraw.swapImage(
			    gpScreenTexture->getTexture()->getTexInfo(),
			    emitter->mDraw.getMainTextureID(0));
			return emitter;
		}

	if (effect_type == EFFECT_LOOP_NORMAL) {
		int table_idx = id - LOOP_NORMAL_FIRST;
		int idx       = getAvailableIdx(table_idx, effect_type, identity);
		TInfo* info   = &mLoopNormalInfo[table_idx][idx];
		info->onFlag(INFO_FLAG_KEEP_ALIVE);
		info->onFlag(INFO_FLAG_BIND_TO_RT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_SRT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_POS);
		info->mBoundMtx = mtx;
		info->mIdentity = identity;
		if (info->mEmitter == nullptr)
			emitTry(id, info, effect_type);
		if (info->mEmitter != nullptr)
			return info->mEmitter;
	}

	if (effect_type == EFFECT_LOOP_INDIRECT) {
		int table_idx = id - LOOP_INDIRECT_FIRST;
		int idx       = getAvailableIdx(table_idx, effect_type, identity);
		TInfo* info   = &mLoopIndirectInfo[table_idx][idx];
		info->onFlag(INFO_FLAG_KEEP_ALIVE);
		info->onFlag(INFO_FLAG_BIND_TO_RT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_SRT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_POS);
		info->mBoundMtx = mtx;
		info->mIdentity = identity;
		if (info->mEmitter == nullptr)
			emitTry(id, info, effect_type);
		if (info->mEmitter != nullptr)
			return info->mEmitter;
	}

	return nullptr;
}

JPABaseEmitter* TMarioParticleManager::emitAndBindToSRTMtxPtr(
    s32 id, MtxPtr mtx, u8 effect_type, const void* identity)
{
	JGeometry::TVec3<f32> local_24;
	local_24.x = mtx[0][3];
	local_24.y = mtx[1][3];
	local_24.z = mtx[2][3];

	if (effect_type == EFFECT_ONETIME_NORMAL)
		if (JPABaseEmitter* emitter = mEmitterManager->createSimpleEmitterID(
		        local_24, id, effect_type, 0, nullptr, nullptr)) {
			emitter->setUserWork((uintptr_t)mtx);
			emitter->setEmitterCallBackPtr(&emitterCallBackBindToSRTMtxPtr);
			return emitter;
		}

	if (effect_type == EFFECT_ONETIME_INDIRECT)
		if (JPABaseEmitter* emitter = mEmitterManager->createSimpleEmitterID(
		        local_24, id, effect_type, 0, nullptr, nullptr)) {
			emitter->setUserWork((uintptr_t)mtx);
			emitter->setEmitterCallBackPtr(&emitterCallBackBindToSRTMtxPtr);
			emitter->mDraw.swapImage(
			    gpScreenTexture->getTexture()->getTexInfo(),
			    emitter->mDraw.getMainTextureID(0));
			return emitter;
		}

	if (effect_type == EFFECT_LOOP_NORMAL) {
		int table_idx = id - LOOP_NORMAL_FIRST;
		int idx       = getAvailableIdx(table_idx, effect_type, identity);
		TInfo* info   = &mLoopNormalInfo[table_idx][idx];
		info->onFlag(INFO_FLAG_KEEP_ALIVE);
		info->onFlag(INFO_FLAG_BIND_TO_SRT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_RT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_POS);
		info->mBoundMtx = mtx;
		info->mIdentity = identity;
		if (info->mEmitter == nullptr)
			emitTry(id, info, effect_type);
		if (info->mEmitter != nullptr)
			return info->mEmitter;
	}

	if (effect_type == EFFECT_LOOP_INDIRECT) {
		int table_idx = id - LOOP_INDIRECT_FIRST;
		int idx       = getAvailableIdx(table_idx, effect_type, identity);
		TInfo* info   = &mLoopIndirectInfo[table_idx][idx];
		info->onFlag(INFO_FLAG_KEEP_ALIVE);
		info->onFlag(INFO_FLAG_BIND_TO_SRT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_RT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_POS);
		info->mBoundMtx = mtx;
		info->mIdentity = identity;
		if (info->mEmitter == nullptr)
			emitTry(id, info, effect_type);
		if (info->mEmitter != nullptr)
			return info->mEmitter;
	}

	return nullptr;
}

JPABaseEmitter* TMarioParticleManager::emitAndBindToMtx(s32 id, MtxPtr mtx,
                                                        u8 effect_type,
                                                        const void* identity)
{
	JGeometry::TVec3<f32> local_24;
	JPABaseEmitter* emitter;
	local_24.x = mtx[0][3];
	local_24.y = mtx[1][3];
	local_24.z = mtx[2][3];

	if (effect_type == EFFECT_ONETIME_NORMAL)
		if (emitter = mEmitterManager->createSimpleEmitterID(
		        local_24, id, effect_type, 0, nullptr, nullptr)) {
			emitter->setGlobalRTMatrix(mtx);
			return emitter;
		}

	if (effect_type == EFFECT_ONETIME_INDIRECT)
		if (emitter = mEmitterManager->createSimpleEmitterID(
		        local_24, id, effect_type, 0, nullptr, nullptr)) {
			emitter->setGlobalRTMatrix(mtx);
			emitter->mDraw.swapImage(
			    gpScreenTexture->getTexture()->getTexInfo(),
			    emitter->mDraw.getMainTextureID(0));
			return emitter;
		}

	return nullptr;
}

JPABaseEmitter* TMarioParticleManager::emitParticleCallBack(
    s32 id, const JGeometry::TVec3<f32>* pos, u8 effect_type,
    JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>* callback,
    const void* identity)
{
	if (effect_type == EFFECT_LOOP_NORMAL) {
		int table_idx = id - LOOP_NORMAL_FIRST;
		int idx       = getAvailableIdx(table_idx, effect_type, identity);
		TInfo* info   = &mLoopNormalInfo[table_idx][idx];
		info->onFlag(INFO_FLAG_KEEP_ALIVE);
		info->offFlag(INFO_FLAG_BIND_TO_POS);
		info->offFlag(INFO_FLAG_BIND_TO_RT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_SRT_MTX);
		info->mBoundPos = pos;
		info->mIdentity = identity;
		if (info->mEmitter == nullptr)
			emitTry(id, info, effect_type);
		if (info->mEmitter != nullptr) {
			info->mEmitter->setParticleCallBackPtr(callback);
			return info->mEmitter;
		}
	}

	if (effect_type == EFFECT_LOOP_INDIRECT) {
		int table_idx = id - LOOP_INDIRECT_FIRST;
		int idx       = getAvailableIdx(table_idx, effect_type, identity);
		TInfo* info   = &mLoopIndirectInfo[table_idx][idx];
		info->onFlag(INFO_FLAG_KEEP_ALIVE);
		info->offFlag(INFO_FLAG_BIND_TO_POS);
		info->offFlag(INFO_FLAG_BIND_TO_RT_MTX);
		info->offFlag(INFO_FLAG_BIND_TO_SRT_MTX);
		info->mBoundPos = pos;
		info->mIdentity = identity;
		if (info->mEmitter == nullptr)
			emitTry(id, info, effect_type);
		if (info->mEmitter != nullptr) {
			info->mEmitter->setParticleCallBackPtr(callback);
			return info->mEmitter;
		}
	}

	return nullptr;
}

void TMarioParticleManager::emitTry(s32 id, TMarioParticleManager::TInfo* info,
                                    u8 effect_type)
{
	if (info->checkFlag(INFO_FLAG_BIND_TO_POS)) {
		info->mEmitter = mEmitterManager->createSimpleEmitterID(
		    *info->mBoundPos, id, effect_type, 0, nullptr, nullptr);

		if (info->mEmitter != nullptr) {
			info->mEmitter->setUserWork((uintptr_t)info->mBoundPos);
			info->mEmitter->setEmitterCallBackPtr(&emitterCallBackBindToPosPtr);
		}
	} else {
		if (info->checkFlag(INFO_FLAG_BIND_TO_RT_MTX
		                    | INFO_FLAG_BIND_TO_SRT_MTX)) {
			JGeometry::TVec3<f32> local_14;
			local_14.x = info->mBoundMtx[0][3];
			local_14.y = info->mBoundMtx[1][3];
			local_14.z = info->mBoundMtx[2][3];

			info->mEmitter = mEmitterManager->createSimpleEmitterID(
			    local_14, id, effect_type, 0, nullptr, nullptr);

			if (info->mEmitter != nullptr) {
				info->mEmitter->setUserWork((uintptr_t)info->mBoundMtx);
				if (info->checkFlag(INFO_FLAG_BIND_TO_RT_MTX))
					info->mEmitter->setEmitterCallBackPtr(
					    &emitterCallBackBindToMtxPtr);
				else
					info->mEmitter->setEmitterCallBackPtr(
					    &emitterCallBackBindToSRTMtxPtr);
			}
		} else {
			info->mEmitter = mEmitterManager->createSimpleEmitterID(
			    *info->mBoundPos, id, effect_type, 0, nullptr, nullptr);
		}
	}

	if (effect_type == EFFECT_LOOP_INDIRECT && info->mEmitter != nullptr) {
		JPABaseEmitter* emitter = info->mEmitter;
		emitter->mDraw.swapImage(gpScreenTexture->getTexture()->getTexInfo(),
		                         emitter->mDraw.getMainTextureID(0));
	}
}

int TMarioParticleManager::getAvailableIdx(s32 table_idx, u8 effect_type,
                                           const void* identity)
{
	for (int i = 0; i < mMaxSimultaneousLoopEmitters; ++i) {
		if (effect_type == EFFECT_LOOP_INDIRECT
		    && mLoopIndirectInfo[table_idx][i].mEmitter != nullptr
		    && mLoopIndirectInfo[table_idx][i].mIdentity == identity)
			return i;
		if (effect_type == EFFECT_LOOP_NORMAL
		    && mLoopNormalInfo[table_idx][i].mEmitter != nullptr
		    && mLoopNormalInfo[table_idx][i].mIdentity == identity)
			return i;
	}

	for (int i = 0; i < mMaxSimultaneousLoopEmitters; ++i) {
		if (effect_type == EFFECT_LOOP_INDIRECT
		    && !mLoopIndirectInfo[table_idx][i].checkFlag(INFO_FLAG_KEEP_ALIVE)
		    && mLoopIndirectInfo[table_idx][i].mEmitter == nullptr)
			return i;
		if (effect_type == EFFECT_LOOP_NORMAL
		    && !mLoopNormalInfo[table_idx][i].checkFlag(INFO_FLAG_KEEP_ALIVE)
		    && mLoopNormalInfo[table_idx][i].mEmitter == nullptr)
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

	if (param_2 == 6) {
		switch (SMSGetMarDirector()->getCurrentStage()) {
		case 1:
			if (SMSGetMarDirector()->getCurrentScenario() == 5) {
				param_2 = 3;
				break;
			}
			// FALLTHROUGH

		case 2:
		case 55:
			param_2 = 2;
			break;

		case 9:
		case 0:
		case 57:
			param_2 = 1;
			break;

		case 3:
			param_2 = 3;
			break;

		case 8:
			param_2 = 4;
			break;

		case 6:
		case 7:
		case 14:
		case 56:
			param_2 = 5;
			break;

		default:
			param_2 = 0;
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

	param_1->setGlobalPrmColor(prmarray[param_2].r, prmarray[param_2].g,
	                           prmarray[param_2].b);

	param_1->setGlobalEnvColor(envarray[param_2].r, envarray[param_2].g,
	                           envarray[param_2].b);
}
