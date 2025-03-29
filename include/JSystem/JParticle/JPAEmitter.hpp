#ifndef JPA_EMITTER_HPP
#define JPA_EMITTER_HPP

#include <JSystem/JParticle/JPADraw.hpp>
#include <JSystem/JParticle/JPACallback.hpp>
#include <JSystem/JParticle/JPAField.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JSupport/JSUList.hpp>

class JPAEmitterManager;
class JPADataBlockLinkInfo;

// name fabricated
class JPAEmitterInfo {
public:
	JPAEmitterInfo()
	    : unk8(0)
	{
	}

public:
	/* 0x0 */ JPABaseEmitter* unk0;
	/* 0x4 */ JPAFieldManager* unk4;
	/* 0x8 */ JMath::TRandom_fast_ unk8;
	/* 0xC */ Vec unkC;
	/* 0x18 */ Vec unk18;
	/* 0x24 */ Vec unk24;
	/* 0x30 */ Vec unk30;
	/* 0x3C */ Vec unk3C;
	/* 0x48 */ Mtx33 unk48;
	/* 0x6C */ JGeometry::SMatrix34C<f32> unk6C;
	/* 0x9C */ JGeometry::SMatrix34C<f32> unk9C;
	/* 0xCC */ JGeometry::SMatrix34C<f32> unkCC;
	/* 0xFC */ JGeometry::SMatrix34C<f32> unkFC;
	/* 0x12C */ JGeometry::SMatrix34C<f32> unk12C;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ f32 unk160;
	/* 0x164 */ u32 unk164;
	/* 0x168 */ u32 unk168;
	/* 0x16C */ u32 unk16C;
	/* 0x170 */ u32 unk170;
	/* 0x174 */ s16 unk174;
	/* 0x176 */ s16 unk176;
	/* 0x178 */ s16 unk178;
	/* 0x17A */ s16 unk17A;
	/* 0x17C */ u8 unk17C;
	/* 0x17D */ char unk17D[2];
	/* 0x17C */ u8 unk17F;
};

JPAEmitterInfo* JPAGetEmitterInfoPtr();

class JPADataBlock;
class JPABaseParticle;

class JPABaseEmitter {
public:
	JPABaseEmitter();

	f32 getFovy();
	f32 getAspect();
	void newParticle();
	void deleteBaseParticle(JPABaseParticle*, JSUList<JPABaseParticle>*);
	void deleteParticle(JPABaseParticle*);
	void deleteAllParticle();
	void createChildParticle(JPABaseParticle*);
	void getEmitterGlobalTranslation(JGeometry::TVec3<f32>&);
	void calcEmitterGlobalParams();
	void loadBaseEmitterBlock(JPADataBlock*);
	void executeBeforeCallBack();
	void executeAfterCallBack();
	void drawEmitterCallBack();
	void createParticle();
	void calcCurrentRateTimerStep();
	void calcCreateParticle();
	bool checkStartFrame();
	bool checkMaxFrame();
	void doParticle();
	void doChildParticle();
	void getKeyValue(f32, u16, f32*);
	void calcKeyFrameAnime();
	void calc();
	void setGlobalRMatrix(MtxPtr);
	void setGlobalRTMatrix(MtxPtr);
	void setGlobalSRTMatrix(MtxPtr);
	void getPivotX();
	void getPivotY();

	// from tww
	JSULink<JPABaseEmitter>* getLinkBufferPtr() { return &unk0; }
	u8 getGroupID() { return unk173; }

	// fabricated
	JPAFieldManager* getFieldManager() { return &unk20; }

public:
	/* 0x0 */ JSULink<JPABaseEmitter> unk0;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ JPAFieldManager unk20;
	/* 0x30 */ JPADraw unk30;
	/* 0xF4 */ JSUPtrList unkF4;
	/* 0x100 */ JSUPtrList unk100;
	/* 0x10C */ JPAEmitterManager* unk10C;
	/* 0x110 */ JPACallBackBase<JPABaseEmitter*>* unk110;
	/* 0x114 */ JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>* unk114;
	/* 0x118 */ JPADataBlockLinkInfo* unk118;
	/* 0x11C */ u32 unk11C;
	/* 0x120 */ char unk120[4];
	/* 0x124 */ Mtx unk124;

	/* 0x154 */ f32 unk154;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ Vec unk160;
	/* 0x16C */ s16 unk16C;
	/* 0x16E */ s16 unk16E;
	/* 0x170 */ s16 unk170;
	/* 0x172 */ u8 unk172;
	/* 0x173 */ u8 unk173;
	/* 0x174 */ f32 unk174;
	/* 0x178 */ f32 unk178;
	/* 0x17C */ f32 unk17C;
	/* 0x180 */ u8 unk180;
	/* 0x181 */ u8 unk181;
	/* 0x182 */ u8 unk182;
	/* 0x183 */ u8 unk183;
	/* 0x184 */ u8 unk184;
	/* 0x185 */ u8 unk185;
	/* 0x186 */ u8 unk186;
	/* 0x187 */ char unk187[0x190 - 0x187];

	/* 0x190 */ f32 unk190;
	/* 0x194 */ f32 unk194;
	/* 0x198 */ f32 unk198;
	/* 0x19C */ f32 unk19C;
	/* 0x1A0 */ f32 unk1A0;
	/* 0x1A4 */ f32 unk1A4;
	/* 0x1A8 */ s16 unk1A8;
	/* 0x1AA */ s16 unk1AA;
	/* 0x1AC */ s16 unk1AC;
	/* 0x1B0 */ f32 unk1B0;
	/* 0x1B4 */ char unk1B4[4];
	/* 0x1B8 */ f32 unk1B8;
	/* 0x1BC */ f32 unk1BC;
	/* 0x1C0 */ f32 unk1C0;
	/* 0x1C4 */ char unk1C4[0x1D8 - 0x1C4];
	/* 0x1D8 */ f32 unk1D8;
	/* 0x1DC */ char unk1DC[0x1E4 - 0x1DC];
	/* 0x1E4 */ f32 unk1E4;
	/* 0x1E8 */ char unk1E8[0x1EE - 0x1E8];
	/* 0x1EE */ s16 unk1EE;
	/* 0x1F0 */ s16 unk1F0;
	/* 0x1F4 */ f32 unk1F4;
	/* 0x1F8 */ char unk1F8[4];
	/* 0x1FC */ f32 unk1FC;
	/* 0x200 */ char unk200[8];
	/* 0x208 */ f32 unk208;
	/* 0x20C */ f32 unk20C;
	/* 0x210 */ f32 unk210;
	/* 0x214 */ f32 unk214;
	/* 0x218 */ f32 unk218;
	/* 0x21C */ JMath::TRandom_fast_ unk21C;
};

#endif
