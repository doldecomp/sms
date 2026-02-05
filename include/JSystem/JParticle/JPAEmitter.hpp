#ifndef JPA_EMITTER_HPP
#define JPA_EMITTER_HPP

#include <JSystem/JParticle/JPADraw.hpp>
#include <JSystem/JParticle/JPACallback.hpp>
#include <JSystem/JParticle/JPAField.hpp>
#include <JSystem/JParticle/JPAMath.hpp>
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
	/* 0x18 */ JGeometry::TVec3<f32> unk18;
	/* 0x24 */ JGeometry::TVec3<f32> unk24;
	/* 0x30 */ Vec unk30;
	/* 0x3C */ JGeometry::TVec3<f32> unk3C;
	/* 0x48 */ Mtx33 unk48;
	/* 0x6C */ TPosition3f unk6C;
	/* 0x9C */ TPosition3f unk9C;
	/* 0xCC */ TPosition3f unkCC;
	/* 0xFC */ TPosition3f unkFC;
	/* 0x12C */ TPosition3f unk12C;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ f32 unk160;
	/* 0x164 */ u32 unk164;
	/* 0x168 */ f32 unk168;
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

struct JPAFrameManager {
public:
	JPAFrameManager()
	    : mFrame(0.0f)
	{
	}

	f32 getFrame() const { return mFrame; }
	void setFrame(f32 frame) { mFrame = frame; }
	void incFrame()
	{
		mFrame++;
		if (mFrame < 0.0f)
			mFrame = 0.0f;
	}

private:
	/* 0x00 */ f32 mFrame;
};

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
	JSUList<JPABaseParticle>* getParticleList() { return &mParticleList; }
	JSUList<JPABaseParticle>* getChildParticleList()
	{
		return &mChildParticleList;
	}
	f32 getRandomF() { return mRng.get_ufloat_1(); }
	f32 getRandomRF() { return mRng.get_ufloat_1() * 2.0f - 1.0f; }
	f32 getRandomSF() { return mRng.get_ufloat_1() - 0.5f; }
	JPADataBlockLinkInfo* getEmitterDataBlockInfoPtr() const
	{
		return mEmitterDataBlockInfo;
	}

	// fabricated
	JPAFieldManager* getFieldManager() { return &mFieldManager; }
	u32 getUnk172() { return unk172; }

	// fabricated
	void setScale(const JGeometry::TVec3<f32>& scale)
	{
		unk154.set(scale);
		unk174.set(scale);
	}

	void setRotation(s16 x, s16 y, s16 z)
	{
		unk16C.x = x;
		unk16C.y = y;
		unk16C.z = z;
		JPAGetXYZRotateMtx(unk16C.x, unk16C.y, unk16C.z, unk124);
	}

	void setParamColor(u8 r, u8 g, u8 b)
	{
		unk180.r = r;
		unk180.g = g;
		unk180.b = b;
	}

	void setEnviColor(u8 r, u8 g, u8 b)
	{
		unk180.r = r;
		unk180.g = g;
		unk180.b = b;
	}

	bool checkUnk11C(u32 flag) const { return unk11C & flag ? true : false; }
	bool isThing() const
	{
		if (checkUnk11C(0x8)
		    && mParticleList.getNumLinks() + mChildParticleList.getNumLinks()
		           == 0)
			return true;
		return false;
	}

public:
	/* 0x0 */ JSULink<JPABaseEmitter> unk0;
	/* 0x10 */ JPAFrameManager unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ JPAFieldManager mFieldManager;
	/* 0x30 */ JPADraw mDraw;
	/* 0xF4 */ JSUList<JPABaseParticle> mParticleList;
	/* 0x100 */ JSUList<JPABaseParticle> mChildParticleList;
	/* 0x10C */ JPAEmitterManager* unk10C;
	/* 0x110 */ JPACallBackBase<JPABaseEmitter*>* unk110;
	/* 0x114 */ JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>* unk114;
	/* 0x118 */ JPADataBlockLinkInfo* mEmitterDataBlockInfo;
	/* 0x11C */ u32 unk11C;
	/* 0x120 */ void* unk120;
	/* 0x124 */ Mtx unk124;

	/* 0x154 */ JGeometry::TVec3<f32> unk154;
	/* 0x160 */ JGeometry::TVec3<f32> unk160;
	/* 0x16C */ S16Vec unk16C;
	/* 0x172 */ u8 unk172;
	/* 0x173 */ u8 unk173;
	/* 0x174 */ JGeometry::TVec3<f32> unk174;
	/* 0x180 */ GXColor unk180;
	/* 0x184 */ u8 unk184;
	/* 0x185 */ u8 unk185;
	/* 0x186 */ u8 unk186;
	/* 0x188 */ u32 unk188;
	/* 0x18C */ u32 unk18C;

	/* 0x190 */ Vec unk190;
	/* 0x19C */ JGeometry::TVec3<f32> unk19C;
	/* 0x1A8 */ s16 unk1A8;
	/* 0x1AA */ s16 unk1AA;
	/* 0x1AC */ s16 unk1AC;
	/* 0x1AE */ u8 unk1AE;
	/* 0x1AF */ u8 unk1AF;
	/* 0x1B0 */ f32 unk1B0;
	/* 0x1B4 */ f32 unk1B4;
	/* 0x1B8 */ f32 unk1B8;
	/* 0x1BC */ f32 unk1BC;
	/* 0x1C0 */ f32 unk1C0;
	/* 0x1C4 */ f32 unk1C4;
	/* 0x1C4 */ char unk1C8[4];
	/* 0x1CC */ f32 unk1CC;
	/* 0x1D0 */ f32 unk1D0;
	/* 0x1D4 */ f32 unk1D4;
	/* 0x1D8 */ f32 unk1D8;
	/* 0x1DC */ f32 unk1DC;
	/* 0x1E0 */ f32 unk1E0;
	/* 0x1E4 */ f32 unk1E4;
	/* 0x1E8 */ s32 unk1E8;
	/* 0x1EC */ s16 unk1EC;
	/* 0x1EE */ s16 unk1EE;
	/* 0x1F0 */ s16 unk1F0;
	/* 0x1F2 */ s16 unk1F2;
	/* 0x1F4 */ f32 unk1F4;
	/* 0x1F8 */ f32 unk1F8;
	/* 0x1FC */ f32 unk1FC;
	/* 0x200 */ u32 unk200;
	/* 0x200 */ u32 unk204;
	/* 0x208 */ f32 unk208;
	/* 0x20C */ f32 unk20C;
	/* 0x210 */ JGeometry::TVec3<f32> unk210;
	/* 0x21C */ JMath::TRandom_fast_ mRng;
};

#endif
