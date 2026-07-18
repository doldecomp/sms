#ifndef JPA_DATA_BLOCK_HPP
#define JPA_DATA_BLOCK_HPP

#include <JSystem/JKernel/JKRHeap.hpp>

class JPADataBlock {
public:
	JPADataBlock() { }
	JPADataBlock(const u8*, JKRHeap*);
	virtual ~JPADataBlock() { }

	// fabricated
	u8* getRawData() { return (u8*)mRawData; }

public:
	/* 0x4 */ void* mRawData;
};

#endif
