#ifndef JPA_EMITTER_MANAGER_HPP
#define JPA_EMITTER_MANAGER_HPP

#include <JSystem/JSupport/JSUList.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JParticle/JPACallback.hpp>
#include <dolphin/mtx.h>

class JPAResourceManager;
class JPADrawInfo;
class JPADataBlock;
class JPABaseEmitter;
class JPABaseParticle;
class JKRHeap;
class JKRSolidHeap;

class JPAEmitterManager {
public:
	JPAEmitterManager(JPAResourceManager*, s32, s32, s32, JKRHeap*);
	void getEmitterNumber();
	void getParticleNumber();
	void getFieldNumber();

	void calcBase(u8);
	void calc();
	void calc(u8);

	void drawBase(JPADrawInfo*, u8);
	void draw(JPADrawInfo*);
	void draw(MtxPtr);
	void draw(JPADrawInfo*, u8);
	void draw(MtxPtr, u8);

	void createVolumeEmitter(JPADataBlock*, u8);
	void
	createEmitterBase(s32, u8, u8, JPACallBackBase<JPABaseEmitter*>*,
	                  JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>*);
	void
	createSimpleEmitterID(const JGeometry::TVec3<f32>&, s32, u8, u8,
	                      JPACallBackBase<JPABaseEmitter*>*,
	                      JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>*);
	void
	createSimpleEmitter(const JGeometry::TVec3<f32>&, s32,
	                    JPACallBackBase<JPABaseEmitter*>*,
	                    JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>*);
	void createEmitter(const JGeometry::TVec3<f32>&, s32,
	                   JPACallBackBase<JPABaseEmitter*>*,
	                   JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>*);

	void deleteEmitter(JPABaseEmitter*);
	void forceDeleteEmitter(JPABaseEmitter*);
	void forceDeleteAllEmitter();

public:
	/* 0x0 */ JSUPtrList unk0;
	/* 0xC */ JKRSolidHeap* unkC;
	/* 0x10 */ u32 unk10;
	/* 0x14 */ JSUPtrList unk14;
	/* 0x20 */ JKRSolidHeap* unk20;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ JSUPtrList unk28;
	/* 0x34 */ JKRSolidHeap* unk34;
	/* 0x38 */ u32 unk38;
	/* 0x3C */ f32 unk3C;
	/* 0x40 */ f32 unk40;
	/* 0x44 */ JSUPtrList unk44[8];
	/* 0xA4 */ JPAResourceManager* unkA4;
	/* 0xA8 */ u32 unkA8;
	/* 0xAC */ u32 unkAC;
	/* 0xB0 */ u32 unkB0;
	/* 0xB4 */ u32 unkB4;
	/* 0xB8 */ u32 unkB8;
	/* 0xBC */ u32 unkBC;
	/* 0xC0 */ u32 unkC0;
	/* 0xC4 */ u32 unkC4;
	/* 0xC8 */ u32 unkC8[16][2];
};

#endif
