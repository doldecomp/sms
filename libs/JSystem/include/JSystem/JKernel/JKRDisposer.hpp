#ifndef JKRDISPOSER_H
#define JKRDISPOSER_H

#include <JSystem/JSupport/JSUList.hpp>

class JKRHeap;

class JKRDisposer {
public:
	JKRDisposer();
	virtual ~JKRDisposer();

public:
	/* 0x4 */ JKRHeap* mRootHeap;
	/* 0x8 */ JSULink<JKRDisposer> mPointerLinks;
};

#endif
