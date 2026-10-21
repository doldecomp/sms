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
		EFFECT_ONETIME_NORMAL   = 0,
		EFFECT_LOOP_NORMAL      = 1,
		EFFECT_ONETIME_INDIRECT = 2,
		EFFECT_LOOP_INDIRECT    = 3,
	};

	enum {
		ONETIME_NORMAL_FIRST   = 0,
		ONETIME_NORMAL_COUNT   = 253,
		LOOP_NORMAL_FIRST      = 253,
		LOOP_NORMAL_COUNT      = 214,
		ONETIME_INDIRECT_FIRST = 467,
		ONETIME_INDIRECT_COUNT = 19,
		LOOP_INDIRECT_FIRST    = 486,
		LOOP_INDIRECT_COUNT    = 19,
	};

	enum {
		INFO_FLAG_KEEP_ALIVE      = 0x4,
		INFO_FLAG_BIND_TO_POS     = 0x8,
		INFO_FLAG_BIND_TO_RT_MTX  = 0x10,
		INFO_FLAG_BIND_TO_SRT_MTX = 0x20,
	};

	struct TInfo {
		TInfo();

		bool checkFlag(u16 flag) { return mFlags & flag ? true : false; }
		void onFlag(u16 flag) { mFlags |= flag; }
		void offFlag(u16 flag) { mFlags &= ~flag; }

		/* 0x0 */ const void* mIdentity;
		/* 0x4 */ union {
			const JGeometry::TVec3<f32>* mBoundPos;
			MtxPtr mBoundMtx;
		};
		/* 0x8 */ u16 mPrevFrameFlags;
		/* 0xA */ u16 mFlags;
		/* 0xC */ JPABaseEmitter* mEmitter;
	};

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	TMarioParticleManager(const char* = "MarioParticleManager");
	void createEffectInfoAry(int max_simultaneous_loop_emitters);

	JPABaseEmitter* emit(s32 id, const JGeometry::TVec3<f32>* pos,
	                     u8 effect_type, const void* identity);
	JPABaseEmitter* emitWithRotate(s32 id, const JGeometry::TVec3<f32>* pos,
	                               s16 rot_x, s16 rot_y, s16 rot_z,
	                               u8 effect_type, const void* identity);
	JPABaseEmitter* emitAndBindToPosPtr(s32 id,
	                                    const JGeometry::TVec3<f32>* pos,
	                                    u8 effect_type, const void* identity);
	JPABaseEmitter* emitAndBindToMtxPtr(s32 id, MtxPtr mtx, u8 effect_type,
	                                    const void* identity);
	JPABaseEmitter* emitAndBindToSRTMtxPtr(s32 id, MtxPtr mtx, u8 effect_type,
	                                       const void* identity);
	JPABaseEmitter* emitAndBindToMtx(s32 id, MtxPtr mtx, u8 effect_type,
	                                 const void* identity);

	JPABaseEmitter* emitParticleCallBack(
	    s32 id, const JGeometry::TVec3<f32>* pos, u8 effect_type,
	    JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>* callback,
	    const void* identity);
	void emitTry(s32 id, TMarioParticleManager::TInfo* info, u8 effect_type);
	int getAvailableIdx(s32 table_idx, u8 effect_type, const void* identity);

public:
	/* 0x10 */ TInfo* mLoopNormalInfo[LOOP_NORMAL_COUNT];
	/* 0x368 */ TInfo* mLoopIndirectInfo[LOOP_INDIRECT_COUNT];
	/* 0x3B4 */ int mMaxSimultaneousLoopEmitters;
	/* 0x3B8 */ JPAEmitterManager* mEmitterManager;
};

class TEmitterViewObj : public JDrama::TViewObj {
public:
	TEmitterViewObj(JPAEmitterManager* emitter_manager,
	                const char* name = "<EmitterViewObj>");
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

protected:
	/* 0x10 */ JPAEmitterManager* unk10;
};

class TEmitterIndirectViewObj : public TEmitterViewObj {
public:
	TEmitterIndirectViewObj();

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
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
