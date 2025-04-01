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
	    : unk0(nullptr)
	    , unk4(nullptr)
	    , unk8(nullptr)
	    , unkC(nullptr)
	    , unk10(nullptr)
	    , unk14(nullptr)
	    , unk18(nullptr)
	    , unk1C(nullptr)
	    , unk20(0)
	    , unk21(0)
	    , unk22(0)
	{
	}

	/* 0x0 */ JPADataBlock* unk0;
	/* 0x4 */ JPABaseShape* unk4;
	/* 0x8 */ JPAExtraShape* unk8;
	/* 0xC */ JPASweepShape* unkC;
	/* 0x10 */ JPAExTexShape* unk10;
	/* 0x14 */ JPAKeyFrameAnime** unk14;
	/* 0x18 */ JPADataBlock** unk18;
	/* 0x1C */ u16* unk1C;
	/* 0x20 */ u8 unk20;
	/* 0x21 */ u8 unk21;
	/* 0x22 */ u8 unk22;
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
	/* 0x4 */ JPATextureResource* unk8;
};

#endif
