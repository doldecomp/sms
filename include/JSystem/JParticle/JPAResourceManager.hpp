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

public:
	/* 0x0 */ JPADefaultTexture unk0;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ u32 unk28;
	/* 0x2C */ JPATexture** unk2C;
};

// TODO: name & usage might be wrong
struct JPAEmitterData {
	/* 0x0 */ char unk0[6];
	/* 0x6 */ u16 unk6;
};

class JPAEmitterResource {
public:
	JPAEmitterResource(u32, JKRHeap*);

	void registration(JPAEmitterData*, u16);
	JPAEmitterData* getByUserIndex(u16);

public:
	/* 0x0 */ u32 unk0;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ JPAEmitterData** unk8;
};

class JPAResourceManager {
public:
	JPAResourceManager(u32, u32, JKRHeap*);

	void load(const char*, u16);
	void load(const void*, u16);

	JPATextureResource* getTextureResource() { return unk8; }
	JPAEmitterResource* getEmitterResource() { return unk4; }

public:
	/* 0x0 */ JKRHeap* unk0;
	/* 0x4 */ JPAEmitterResource* unk4;
	/* 0x4 */ JPATextureResource* unk8;
};

#endif
