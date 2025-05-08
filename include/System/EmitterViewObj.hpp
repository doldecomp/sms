#ifndef EMITTER_VIE_OBJ_HPP
#define EMITTER_VIE_OBJ_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JParticle/JPACallback.hpp>
#include <dolphin/mtx.h>

class JPABaseEmitter;
class JPABaseParticle;
class JPAEmitterManager;

class TMarioParticleManager;

extern TMarioParticleManager* gpMarioParticleManager;

class TMarioParticleManager : public JDrama::TViewObj {
public:
	struct TInfo { };

	virtual void perform(u32, JDrama::TGraphics*);

	TMarioParticleManager(const char*);
	void createEffectInfoAry(int);

	JPABaseEmitter* emit(s32, const JGeometry::TVec3<f32>*, u8, const void*);
	JPABaseEmitter* emitWithRotate(s32, const JGeometry::TVec3<f32>*, s16, s16,
	                               s16, u8, const void*);
	JPABaseEmitter* emitAndBindToPosPtr(s32, const JGeometry::TVec3<f32>*, u8,
	                                    const void*);
	JPABaseEmitter* emitAndBindToMtxPtr(s32, MtxPtr, u8, const void*);
	JPABaseEmitter* emitAndBindToSRTMtxPtr(s32, MtxPtr, u8, const void*);
	JPABaseEmitter* emitAndBindToMtx(s32, MtxPtr, u8, const void*);

	void
	emitParticleCallBack(s32, const JGeometry::TVec3<f32>*, u8,
	                     JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>*,
	                     const void*);
	void emitTry(s32, TMarioParticleManager::TInfo*, u8);
	void getAvailableIdx(s32, u8, const void*);
};

class TEmitterViewObj : public JDrama::TViewObj {
public:
	TEmitterViewObj(JPAEmitterManager*, const char* = "<EmitterViewObj>");
	virtual void perform(u32, JDrama::TGraphics*);

public:
	/* 0x10 */ JPAEmitterManager* unk10;
};

#endif
