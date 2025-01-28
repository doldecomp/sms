#include <JSystem/J3D/J3DGraphBase/J3DDrawBuffer.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>

#pragma opt_strength_reduction off

J3DDrawBuffer::sortFunc J3DDrawBuffer::sortFuncTable[6] = {
	&J3DDrawBuffer::entryMatSort,     &J3DDrawBuffer::entryMatAnmSort,
	&J3DDrawBuffer::entryZSort,       &J3DDrawBuffer::entryModelSort,
	&J3DDrawBuffer::entryInvalidSort, &J3DDrawBuffer::entryNonSort,
};
J3DDrawBuffer::drawFunc J3DDrawBuffer::drawFuncTable[2] = {
	&J3DDrawBuffer::drawHead,
	&J3DDrawBuffer::drawTail,
};
int J3DDrawBuffer::entryNum;

J3DDrawBuffer::J3DDrawBuffer(u32 size)
{
	mBuffer = new (0x20) J3DPacket*[size];
	mSize   = size;

	mDrawType = DRAW_HEAD;
	mSortType = SORT_MAT;

	mZNear = 1.0f;
	mZFar  = 10000.0f;

	mZMtx           = nullptr;
	mCallBackPacket = nullptr;

	mZRatio = (mZFar - mZNear) / mSize;

	frameInit();
}

void J3DDrawBuffer::frameInit()
{
	for (int i = 0; i < mSize; ++i)
		mBuffer[i] = nullptr;

	mCallBackPacket = nullptr;
}

int J3DDrawBuffer::entryMatSort(J3DMatPacket* packet) { }

int J3DDrawBuffer::entryMatAnmSort(J3DMatPacket* packet) { }

int J3DDrawBuffer::entryZSort(J3DMatPacket* packet) { }

int J3DDrawBuffer::entryModelSort(J3DMatPacket* packet) { }

int J3DDrawBuffer::entryInvalidSort(J3DMatPacket* packet) { }

int J3DDrawBuffer::entryNonSort(J3DMatPacket* packet) { }

int J3DDrawBuffer::entryImm(J3DPacket* packet, u16 index) { }

void J3DDrawBuffer::draw() const
{
	drawFunc func = drawFuncTable[mDrawType];
	(this->*func)();
}

void J3DDrawBuffer::drawHead() const { }

void J3DDrawBuffer::drawTail() const { }
