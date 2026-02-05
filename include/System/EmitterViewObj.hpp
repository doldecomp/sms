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
	enum {
		INFO_FLAG_UNK4            = 0x4,
		INFO_FLAG_BIND_TO_POS     = 0x8,
		INFO_FLAG_BIND_TO_RT_MTX  = 0x10,
		INFO_FLAG_BIND_TO_SRT_MTX = 0x20,
	};

	struct TInfo {
		TInfo();

		bool checkFlag(u16 flag) { return mFlags & flag ? true : false; }
		void onFlag(u16 flag) { mFlags |= flag; }
		void offFlag(u16 flag) { mFlags &= ~flag; }

		/* 0x0 */ const void* unk0;
		/* 0x4 */ const void* unk4;
		/* 0x8 */ u16 mPrevFrameFlags;
		/* 0xA */ u16 mFlags;
		/* 0xC */ JPABaseEmitter* mEmitter;
	};

	virtual void perform(u32, JDrama::TGraphics*);

	TMarioParticleManager(const char* = "MarioParticleManager");
	void createEffectInfoAry(int);

	// TODO: figure out what the u8 parameters her mean conceptually

	JPABaseEmitter* emit(s32, const JGeometry::TVec3<f32>*, u8, const void*);
	JPABaseEmitter* emitWithRotate(s32, const JGeometry::TVec3<f32>*, s16, s16,
	                               s16, u8, const void*);
	JPABaseEmitter* emitAndBindToPosPtr(s32, const JGeometry::TVec3<f32>*, u8,
	                                    const void*);
	JPABaseEmitter* emitAndBindToMtxPtr(s32, MtxPtr, u8, const void*);
	JPABaseEmitter* emitAndBindToSRTMtxPtr(s32, MtxPtr, u8, const void*);
	JPABaseEmitter* emitAndBindToMtx(s32, MtxPtr, u8, const void*);

	JPABaseEmitter*
	emitParticleCallBack(s32, const JGeometry::TVec3<f32>*, u8,
	                     JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>*,
	                     const void*);
	void emitTry(s32, TMarioParticleManager::TInfo*, u8);
	int getAvailableIdx(s32, u8, const void*);

public:
	/* 0x10 */ TInfo* unk10[214];
	/* 0x368 */ TInfo* unk368[19];
	/* 0x3B4 */ int unk3B4;
	/* 0x3B8 */ JPAEmitterManager* unk3B8;
};

class TEmitterViewObj : public JDrama::TViewObj {
public:
	TEmitterViewObj(JPAEmitterManager*, const char* = "<EmitterViewObj>");
	virtual void perform(u32, JDrama::TGraphics*);

public:
	/* 0x10 */ JPAEmitterManager* unk10;
};

class TEmitterIndirectViewObj : public TEmitterViewObj {
public:
	TEmitterIndirectViewObj();

	virtual void perform(u32, JDrama::TGraphics*);
};

class TMarioEmitterCallBackBindToPosPtr
    : public JPACallBackBase<JPABaseEmitter*> {
public:
	virtual void execute(JPABaseEmitter*);
	virtual void draw(JPABaseEmitter*);
};

class TMarioEmitterCallBackBindToMtxPtr
    : public JPACallBackBase<JPABaseEmitter*> {
public:
	virtual void execute(JPABaseEmitter*);
	virtual void draw(JPABaseEmitter*);
};

class TMarioEmitterCallBackBindToSRTMtxPtr
    : public JPACallBackBase<JPABaseEmitter*> {
public:
	virtual void execute(JPABaseEmitter*);
	virtual void draw(JPABaseEmitter*);
};

void SMSSetEmitterPolColor(JPABaseEmitter*, int);

#endif
