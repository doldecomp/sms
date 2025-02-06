#include <JSystem/J3D/J3DGraphBase/J3DDrawBuffer.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
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

int J3DDrawBuffer::entryMatSort(J3DMatPacket* packet)
{
	packet->drawClear();
	packet->getShapePacket()->drawClear();

	J3DTexture* texture = j3dSys.getTexture();
	u32 hash;
	u16 texNo = packet->getMaterial()->getTexNo(0);
	if (texNo == 0xFFFF) {
		hash = 0;
	} else {
		hash = (u32)texture->getResTIMG(texNo);
	}

	if (packet->unk3C & 0x80000000) {
		packet->setNextPacket(mBuffer[0]);
		mBuffer[0] = packet;
		return 1;
	} else {
		u32 slot = hash & (mSize - 1);
		if (mBuffer[slot] == NULL) {
			mBuffer[slot] = packet;
			return 1;
		} else {
			for (J3DMatPacket* pkt = (J3DMatPacket*)mBuffer[slot]; pkt != NULL;
			     pkt               = (J3DMatPacket*)pkt->getNextPacket()) {
				if (pkt->isSame(packet)) {
					pkt->addShapePacket(packet->getShapePacket());
					return 0;
				}
			}

			packet->setNextPacket(mBuffer[slot]);
			mBuffer[slot] = packet;
			return 1;
		}
	}
}

int J3DDrawBuffer::entryMatAnmSort(J3DMatPacket* packet)
{
	J3DMaterialAnm* pMaterialAnm = packet->unk44;
	u32 slot                     = (u32)pMaterialAnm & (mSize - 1);

	if (pMaterialAnm == NULL) {
		return entryMatSort(packet);
	} else {
		packet->drawClear();
		packet->getShapePacket()->drawClear();
		if (mBuffer[slot] == NULL) {
			mBuffer[slot] = packet;
			return 1;
		} else {
			for (J3DMatPacket* pkt = (J3DMatPacket*)mBuffer[slot]; pkt != NULL;
			     pkt               = (J3DMatPacket*)pkt->getNextPacket()) {
				if (pkt->unk44 == pMaterialAnm) {
					pkt->addShapePacket(packet->getShapePacket());
					return 0;
				}
			}

			packet->setNextPacket(mBuffer[slot]);
			mBuffer[slot] = packet;
			return 1;
		}
	}
}

int J3DDrawBuffer::entryZSort(J3DMatPacket* packet)
{
	packet->drawClear();
	packet->getShapePacket()->drawClear();

	Vec tmp;
	tmp.x = mZMtx[0][3];
	tmp.y = mZMtx[1][3];
	tmp.z = mZMtx[2][3];

	f32 value = -J3DCalcZValue(j3dSys.getViewMtx(), tmp);

	u32 idx;
	if (mZNear + mZRatio < value) {
		if (mZFar - mZRatio > value) {
			idx = value / mZRatio;
		} else {
			idx = mSize - 1;
		}
	} else {
		idx = 0;
	}

	idx = (mSize - 1) - idx;
	packet->setNextPacket(mBuffer[idx]);
	mBuffer[idx] = packet;

	return 1;
}

int J3DDrawBuffer::entryModelSort(J3DMatPacket* packet)
{
	packet->drawClear();
	packet->getShapePacket()->drawClear();

	if (mCallBackPacket != nullptr) {
		mCallBackPacket->addChildPacket(packet);
		return 1;
	}

	return 0;
}

int J3DDrawBuffer::entryInvalidSort(J3DMatPacket* packet)
{
	packet->drawClear();
	packet->getShapePacket()->drawClear();

	if (mCallBackPacket != nullptr) {
		mCallBackPacket->addChildPacket(packet->getShapePacket());
		return 1;
	}

	return 0;
}

int J3DDrawBuffer::entryNonSort(J3DMatPacket* packet)
{
	packet->drawClear();
	packet->getShapePacket()->drawClear();

	packet->setNextPacket(mBuffer[0]);
	mBuffer[0] = packet;

	return 1;
}

int J3DDrawBuffer::entryImm(J3DPacket* packet, u16 index)
{
	packet->setNextPacket(mBuffer[index]);
	mBuffer[index] = packet;

	return 1;
}

void J3DDrawBuffer::draw() const
{
	drawFunc func = drawFuncTable[mDrawType];
	(this->*func)();
}

void J3DDrawBuffer::drawHead() const
{
	for (u32 i = 0; i < mSize; i++) {
		for (J3DPacket* packet = mBuffer[i]; packet != nullptr;
		     packet            = packet->getNextPacket()) {
			packet->draw();
		}
	}
}

void J3DDrawBuffer::drawTail() const
{
	for (int i = mSize - 1; i >= 0; i--) {
		for (J3DPacket* packet = mBuffer[i]; packet != nullptr;
		     packet            = packet->getNextPacket()) {
			packet->draw();
		}
	}
}
