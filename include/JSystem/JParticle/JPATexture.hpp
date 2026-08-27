#ifndef JPA_TEXTURE_HPP
#define JPA_TEXTURE_HPP

#include <JSystem/JParticle/JPADataBlock.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <dolphin/gx/GXTexture.h>

// from TWW/TP
struct JPATextureData {
	/* 0x00 */ u8 field_0x00[0xC];
	/* 0x0C */ char mName[0x14];
	/* 0x20 */ ResTIMG mResTIMG;
};

class JPATexture : public JPADataBlock {
public:
	JPATexture(const u8*, JKRHeap*);
	~JPATexture() { }

	void load(GXTexMapID texMapID) { unk8.load(texMapID); }

	const JPATextureData* getTextureData()
	{
		return (const JPATextureData*)mRawData;
	}
	const char* getName() { return getTextureData()->mName; }

public:
	/* 0x8 */ JUTTexture unk8;
};

class JPADefaultTexture {
public:
	JPADefaultTexture()
	    : unk0(nullptr)
	{
	}
	~JPADefaultTexture() { }
	void initialize(JKRHeap*);

	// from tww
	void load(GXTexMapID texMap) { GXLoadTexObj(&unk4, texMap); }

public:
	/* 0x0 */ u8* unk0;
	/* 0x4 */ GXTexObj unk4;
};

#endif
