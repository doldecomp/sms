#include <JSystem/J3D/J3DGraphBase/J3DPacket.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <dolphin/os.h>
#include <dolphin/gd.h>
#include <dolphin/gx.h>
#include <macros.h>

int J3DDrawPacket::sInterruptFlag;

void J3DDisplayListObj::newDisplayList(u32 param_1)
{
	mCapacity = ALIGN_NEXT(param_1, 0x20);
	mpData[0] = new (0x20) char[mCapacity];
	mpData[1] = new (0x20) char[mCapacity];
	mSize     = 0;
}

void J3DDisplayListObj::swapBuffer()
{
	void* tmp = mpData[0];
	mpData[0] = mpData[1];
	mpData[1] = tmp;
}

void J3DDisplayListObj::callDL() { GXCallDisplayList(mpData[0], mSize); }

bool J3DPacket::isSame(J3DMatPacket*) const { return false; }

bool J3DPacket::entry(J3DDrawBuffer*) { return true; }

void J3DPacket::addChildPacket(J3DPacket* packet)
{
	if (mpFirstChild == nullptr) {
		mpFirstChild = packet;
		return;
	}
	packet->mpNext = mpFirstChild;
	mpFirstChild   = packet;
}

void J3DCallBackPacket::draw()
{
	if (mpCallBack != nullptr)
		mpCallBack(this, 0);

	for (J3DPacket* packet = mpFirstChild; packet != nullptr;
	     packet            = packet->getNextPacket())
        packet->draw();

	if (mpCallBack != nullptr)
		mpCallBack(this, 1);
}

J3DDrawPacket::J3DDrawPacket()
{
	mFlags           = 0;
	mpDisplayListObj = nullptr;
}

J3DDrawPacket::~J3DDrawPacket() { }

void J3DDrawPacket::draw()
{
	GXCallDisplayList(mpDisplayListObj->mpData[0], mpDisplayListObj->mSize);
}

void J3DDrawPacket::beginDL()
{
	mpDisplayListObj->swapBuffer();
	sInterruptFlag = OSDisableInterrupts();
	GDInitGDLObj(&mGDList, mpDisplayListObj->mpData[0],
	             mpDisplayListObj->mCapacity);
	GDSetCurrent(&mGDList);
}

u32 J3DDrawPacket::endDL()
{
	GDPadCurr32();
	OSRestoreInterrupts(sInterruptFlag);
	mpDisplayListObj->mSize = mGDList.ptr - mGDList.start;
	GDFlushCurrToMem();
	GDSetCurrent(nullptr);
	return mpDisplayListObj->mSize;
}

void J3DDrawPacket::beginPatch() { beginDL(); }

u32 J3DDrawPacket::endPatch()
{
	OSRestoreInterrupts(sInterruptFlag);
	GDSetCurrent(nullptr);
	return mpDisplayListObj->mSize;
}

J3DMatPacket::J3DMatPacket()
{
	mpMaterial = 0;
	unk3C      = 0xffffffff;
	mTexture   = 0;
	unk44      = 0;
}

J3DMatPacket::~J3DMatPacket() { }

void J3DMatPacket::addShapePacket(J3DShapePacket* packet)
{
	if (mpShapePacket == nullptr) {
		mpShapePacket = packet;
		return;
	}
	packet->setNextPacket(mpShapePacket);
	mpShapePacket = packet;
}

bool J3DMatPacket::isHideAllShapePacket_()
{
	bool ret = true;

	J3DShapePacket* packet = getShapePacket();

	while (packet != nullptr) {
		if (packet->isVisible()) {
			ret = false;
			break;
		}
		packet = (J3DShapePacket*)packet->getNextPacket();
	}

	return ret;
}

void J3DMatPacket::draw()
{
	if (isHideAllShapePacket_())
		return;

	j3dSys.setTexture(mTexture);
	j3dSys.setMatPacket(this);
	mpMaterial->load();

	J3DShapePacket* packet = getShapePacket();

	while (packet != nullptr) {
		packet->draw();
		packet = (J3DShapePacket*)packet->getNextPacket();
	}
}

J3DShapePacket::J3DShapePacket()
{
	mpShape           = nullptr;
	mDrawMatrices     = nullptr;
	mNormMatrices     = nullptr;
	mpCurrentViewNo   = &j3dDefaultViewNo;
	mpVertexPositions = nullptr;
	mpVertexNormals   = nullptr;
	mpVertexColors    = nullptr;
	mVisible          = true;
}

J3DShapePacket::~J3DShapePacket() { }

void J3DShapePacket::draw()
{
	if (mpShape != nullptr && mVisible) {
		if (mpCallBack != nullptr)
			mpCallBack(this, 0);

		j3dSys.setVtxPos(mpVertexPositions);
		j3dSys.setVtxNrm(mpVertexNormals);
		j3dSys.setVtxCol(mpVertexColors);
		mpShape->setDrawMtx(mDrawMatrices);
		mpShape->setNrmMtx(mNormMatrices);
		mpShape->setCurrentViewNoPtr(mpCurrentViewNo);

		mpShape->draw();

		if (mpCallBack != nullptr)
			mpCallBack(this, 1);
	}
}
