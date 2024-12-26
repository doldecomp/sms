#include "JSystem/JKernel/JKRDisposer.hpp"

#include "JSystem/JKernel/JKRHeap.hpp"

JKRDisposer::JKRDisposer()
    : mPointerLinks(this)
{
	mRootHeap = JKRHeap::findFromRoot(this);
	if (mRootHeap != nullptr) {
		mRootHeap->appendDisposer(this);
	}
}

JKRDisposer::~JKRDisposer()
{
	if (mRootHeap != nullptr) {
		mRootHeap->removeDisposer(this);
	}
}
