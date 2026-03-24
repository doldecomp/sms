#ifndef JPA_RESOURCE_MANAGER_HPP
#define JPA_RESOURCE_MANAGER_HPP

#include <JSystem/JParticle/JPATexture.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <dolphin/types.h>

class JKRHeap;

class JPATextureResource {
public:
	JPATextureResource(u32, JKRHeap*);

	int registration(const u8*, JKRHeap*);

	// from TWW
	void load(u16 id, GXTexMapID texMap) { unk2C[id]->load(texMap); }
	void loadDefaultTexture(GXTexMapID texMap) { unk0.load(texMap); }

	// fabricated
	const ResTIMG* swapImage(const ResTIMG* timg, u32 idx)
	{
		const ResTIMG* old = unk2C[idx]->unk8.getTexInfo();
		unk2C[idx]->unk8.storeTIMG(timg);
		return old;
	}

public:
	/* 0x0 */ JPADefaultTexture unk0;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ u32 unk28;
	/* 0x2C */ JPATexture** unk2C;
};

class JPABaseShape;
class JPAExtraShape;
class JPASweepShape;
class JPAExTexShape;
class JPAKeyFrameAnime;

// TODO: fabricated. Name taken from TWW
struct JPADataBlockLinkInfo {
	JPADataBlockLinkInfo()
	    : mBaseEmitterBlock(nullptr)
	    , mBaseShape(nullptr)
	    , mExtraShape(nullptr)
	    , mSweepShape(nullptr)
	    , mExTexShape(nullptr)
	    , mKeyframeAnimations(nullptr)
	    , mFields(nullptr)
	    , mTextureDataBase(nullptr)
	    , mFieldNum(0)
	    , mTextureNum(0)
	    , mKeyframeAnimationNum(0)
	{
	}

	// fabricated, got renamed to getDynamics in TWW due to refactoring of JPA
	JPADataBlock* getBaseEmitterBlock() { return mBaseEmitterBlock; }

	// All real, from TWW.
	JPABaseShape* getBaseShape() { return mBaseShape; }
	JPAExTexShape* getExTexShape() { return mExTexShape; }
	JPAExtraShape* getExtraShape() { return mExtraShape; }
	JPASweepShape* getSweepShape() { return mSweepShape; }

	JPADataBlock** getField() { return mFields; }
	u8 getFieldNum() { return mFieldNum; }

	u16* getTextureDataBase() { return mTextureDataBase; }
	u8 getTextureNum() { return mTextureNum; }

	JPAKeyFrameAnime** getKey() { return mKeyframeAnimations; }
	u8 getKeyNum() { return mKeyframeAnimationNum; }

public:
	/* 0x0 */ JPADataBlock* mBaseEmitterBlock;
	/* 0x4 */ JPABaseShape* mBaseShape;
	/* 0x8 */ JPAExtraShape* mExtraShape;
	/* 0xC */ JPASweepShape* mSweepShape;
	/* 0x10 */ JPAExTexShape* mExTexShape;
	/* 0x14 */ JPAKeyFrameAnime** mKeyframeAnimations;
	/* 0x18 */ JPADataBlock** mFields;
	/* 0x1C */ u16* mTextureDataBase;
	/* 0x20 */ u8 mFieldNum;
	/* 0x21 */ u8 mTextureNum;
	/* 0x22 */ u8 mKeyframeAnimationNum;
};

// TODO: name & usage might be wrong
struct JPAEmitterData {
	JPAEmitterData()
	    : unk0(0)
	{
	}

	// inlines from TWW
	void setUserIndex(u16 v) { unk6 = v; }
	u16 getUserIndex() { return unk6; }

	// You'd THINK that they support multiple emitters here --
	// but no, in SMS, only a single emitter is allowed.

	/* 0x0 */ JPADataBlockLinkInfo** unk0;
	/* 0x4 */ u16 unk4;
	/* 0x6 */ u16 unk6;
};

class JPAEmitterResource {
public:
	JPAEmitterResource(u32, JKRHeap*);

	int registration(JPAEmitterData*, u16);
	JPAEmitterData* getByUserIndex(u16);

public:
	/* 0x0 */ u32 unk0;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ JPAEmitterData** unk8;
};

class JPAResourceManager {
public:
	JPAResourceManager(u32, u32, JKRHeap*);

	int load(const char*, u16);
	int load(const void*, u16);

	JPATextureResource* getTextureResource() { return unk8; }
	JPAEmitterResource* getEmitterResource() { return unk4; }

public:
	/* 0x0 */ JKRHeap* unk0;
	/* 0x4 */ JPAEmitterResource* unk4;
	/* 0x8 */ JPATextureResource* unk8;
};

#endif
