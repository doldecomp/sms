#ifndef JKRDISPOSER_H
#define JKRDISPOSER_H

#include <JSystem/JSupport/JSUList.hpp>

class JKRHeap;

class JKRDisposer {
public:
	JKRDisposer();
	virtual ~JKRDisposer();

public:
	JKRHeap* mRootHeap;                 // _4
	JSULink<JKRDisposer> mPointerLinks; // _8
};

#endif
