#ifndef JPA_EMITTER_LOADER_HPP
#define JPA_EMITTER_LOADER_HPP

#include <JSystem/JParticle/JPADataBlock.hpp>
#include <dolphin/types.h>

class JKRHeap;
class JPATextureResource;

class JPAKeyFrameAnime : public JPADataBlock {
public:
	JPAKeyFrameAnime(const u8* param_1, JKRHeap* param_2)
	    : JPADataBlock(param_1, param_2)
	{
	}
	virtual ~JPAKeyFrameAnime() { }
};

struct JPAEmitterData;

class JPAEmitterLoaderDataBase {
public:
	static JPAEmitterData* load(const u8*, JKRHeap*, JPATextureResource*);
};

struct JPABinaryHeader {
	u32 unk0;
	u32 unk4;
	u32 unk8;
	u32 unkC;
};

class JPAEmitterLoader {
public:
	JPAEmitterLoader(JKRHeap* param_1, const u8* param_2,
	                 const JPABinaryHeader* param_3)
	{
		unk8  = param_2;
		unkC  = param_3;
		unk10 = 0;
		unk4  = param_1;
	}

	virtual ~JPAEmitterLoader() { }

public:
	/* 0x4 */ JKRHeap* unk4;
	/* 0x8 */ const u8* unk8;
	/* 0xC */ const JPABinaryHeader* unkC;
	/* 0x10 */ JPAEmitterData* unk10;
};

class JPAEmitterLoader_v10 : public JPAEmitterLoader {
public:
	JPAEmitterLoader_v10(JKRHeap*, const u8*, const JPABinaryHeader*);
	virtual ~JPAEmitterLoader_v10() { }

	JPAEmitterData* load(JPATextureResource*);
};

#endif
