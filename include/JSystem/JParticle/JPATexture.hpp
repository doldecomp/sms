#ifndef JPA_TEXTURE_HPP
#define JPA_TEXTURE_HPP

#include <JSystem/JParticle/JPADataBlock.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>

class JPATexture : public JPADataBlock {
public:
	JPATexture(const u8*, JKRHeap*);
	~JPATexture() { }

public:
	/* 0x8 */ JUTTexture unk8;
};

class JPADefaultTexture {
public:
	JPADefaultTexture()
	    : unk0(nullptr)
	{
	}
	~JPADefaultTexture();
	void initialize(JKRHeap*);

public:
	/* 0x0 */ u8* unk0;
	/* 0x4 */ GXTexObj obj;
};

#endif
