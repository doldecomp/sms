#ifndef JPA_DATA_BLOCK_HPP
#define JPA_DATA_BLOCK_HPP

#include <JSystem/JKernel/JKRHeap.hpp>

class JPADataBlock {
public:
	JPADataBlock() { }
	JPADataBlock(const u8*, JKRHeap*);
	virtual ~JPADataBlock() { }

	template <class T> T* getAtOffset(u32 offset) const
	{
		return (T*)((u8*)mRawData + offset);
	}

public:
	/* 0x4 */ void* mRawData;
};

#endif
