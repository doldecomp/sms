#ifndef JPA_EMITTER_MANAGER_HPP
#define JPA_EMITTER_MANAGER_HPP

#include <JSystem/JSupport/JSUList.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JParticle/JPACallback.hpp>
#include <dolphin/mtx.h>

class JPADrawInfo {
public:
	MtxPtr getCameraMtxPtr() { return unk0; }
	f32 getFovy() { return unk4; }
	void setFovy(f32 fovy) { unk4 = fovy; }
	f32 getAspect() { return unk8; }
	void setAspect(f32 aspect) { unk8 = aspect; }

	MtxPtr unk0;
	f32 unk4;
	f32 unk8;
};

class JPAResourceManager;
class JPADrawInfo;
class JPADataBlock;
class JPABaseEmitter;
class JPABaseParticle;
class JPABaseField;
class JKRHeap;
class JKRSolidHeap;

class JPAEmitterManager {
public:
	JPAEmitterManager(JPAResourceManager*, s32, s32, s32, JKRHeap*);
	u32 getEmitterNumber();
	u32 getParticleNumber();
	u32 getFieldNumber();

	void calcBase(u8);
	void calc();
	void calc(u8);

	void drawBase(JPADrawInfo*, u8);
	void draw(JPADrawInfo*);
	void draw(MtxPtr);
	void draw(JPADrawInfo*, u8);
	void draw(MtxPtr, u8);

	JPABaseEmitter* createVolumeEmitter(JPADataBlock*, u8);
	JPABaseEmitter*
	createEmitterBase(s32, u8, u8, JPACallBackBase<JPABaseEmitter*>*,
	                  JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>*);
	JPABaseEmitter*
	createSimpleEmitterID(const JGeometry::TVec3<f32>&, s32, u8, u8,
	                      JPACallBackBase<JPABaseEmitter*>*,
	                      JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>*);
	JPABaseEmitter*
	createSimpleEmitter(const JGeometry::TVec3<f32>&, s32,
	                    JPACallBackBase<JPABaseEmitter*>*,
	                    JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>*);
	bool
	createEmitter(const JGeometry::TVec3<f32>&, s32,
	              JPACallBackBase<JPABaseEmitter*>*,
	              JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>*);

	void deleteEmitter(JPABaseEmitter*);
	void forceDeleteEmitter(JPABaseEmitter*);
	void forceDeleteAllEmitter();

public:
	/* 0x0 */ JSUList<JPABaseParticle> unk0;
	/* 0xC */ JKRSolidHeap* unkC;
	/* 0x10 */ u32 unk10;
	/* 0x14 */ JSUList<JPABaseEmitter> unk14;
	/* 0x20 */ JKRSolidHeap* unk20;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ JSUList<JPABaseField> unk28;
	/* 0x34 */ JKRSolidHeap* unk34;
	/* 0x38 */ u32 unk38;
	/* 0x3C */ f32 unk3C;
	/* 0x40 */ f32 unk40;
	/* 0x44 */ JSUList<JPABaseEmitter> unk44[8];
	/* 0xA4 */ JPAResourceManager* unkA4[8];
	/* 0xC4 */ u32 unkC4;
	/* 0xC8 */ JPABaseEmitter* unkC8[2][16];
};

#endif
