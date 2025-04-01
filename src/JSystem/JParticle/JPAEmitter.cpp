#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>
#include <JSystem/JParticle/JPAParticle.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/JParticle/JPASweepShape.hpp>
#include <JSystem/JParticle/JPAMath.hpp>
#include <JSystem/JParticle/JPAEmitterLoader.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>

static JPAEmitterInfo JPAEmitterInfoObj;

JPAEmitterInfo* JPAGetEmitterInfoPtr() { return &JPAEmitterInfoObj; }

JPABaseEmitter::JPABaseEmitter()
    : unk0(this)
    , unk14(1.0f)
    , unk18(0.0f)
    , unk1C(1.0f)
    , unk21C(0)
{
	PSMTXIdentity(unk124);

	// TODO: vectors?

	unk154.x = 1.0f;
	unk154.y = 1.0f;
	unk154.z = 1.0f;

	unk174 = 1.0f;
	unk178 = 1.0f;
	unk17C = 1.0f;

	unk170 = 0;
	unk16E = 0;
	unk16C = 0;

	unk160.x = unk160.y = unk160.z = 0.0f;

	unk1B8 = 0.0f;
	unk1BC = 0.0f;
	unk1C0 = 1.0f;

	unk110 = nullptr;
	unk114 = nullptr;
	unk118 = nullptr;

	unk172 = 0;
	unk11C = 0;
	unk11C |= 0x30;
	unk21C.value = JPAEmitterInfoObj.unk8.next();
}

f32 JPABaseEmitter::getFovy() { return JPAEmitterInfoObj.unk15C; }

f32 JPABaseEmitter::getAspect() { return JPAEmitterInfoObj.unk160; }

void JPABaseEmitter::newParticle() { }

void JPABaseEmitter::deleteBaseParticle(JPABaseParticle* particle,
                                        JSUList<JPABaseParticle>* list)
{
	particle->init();
	list->remove(particle->getLinkBufferPtr());
	unk10C->unk0.prepend(particle->getLinkBufferPtr());
}

void JPABaseEmitter::deleteParticle(JPABaseParticle* particle) { }

void JPABaseEmitter::deleteAllParticle()
{
	JSUList<JPABaseParticle>* list;
	JSULink<JPABaseParticle>* it;

	list = &unkF4;
	it   = list->getFirst();
	while (it) {
		JSULink<JPABaseParticle>* next = it->getNext();
		deleteBaseParticle(it->getObject(), list);
		it = next;
	}

	list = &unk100;
	it   = list->getFirst();
	while (it) {
		JPABaseParticle* particle = it->getObject();
		it                        = it->getNext();
		deleteBaseParticle(particle, list);
	}
}

void JPABaseEmitter::createChildParticle(JPABaseParticle* parent)
{
	JGeometry::TVec3<f32> parentUnk20 = parent->unk14;
	JGeometry::TVec3<f32> parentUnk14 = parent->unk14;

	JGeometry::TVec3<f32> local_c0(0.0f, 0.0f, 0.0f);

	JPASweepShape* sweepShape = unk118->unkC;
	int count                 = sweepShape->unk42;

	bool bVar1 = false;
	if (!JPAEmitterInfoObj.unk17F)
		bVar1 = true;

	for (int i = 0; i < count; ++i) {

		if (unk10C->unk0.getNumLinks() == 0)
			break;

		JPABaseParticle* baseParticle = unk10C->unk0.getFirst()->getObject();
		unk10C->unk0.remove(baseParticle->getLinkBufferPtr());
		JPAParticle* particle = (JPAParticle*)baseParticle;

		baseParticle->unk10 |= 0x1;
		unk100.prepend(baseParticle->getLinkBufferPtr());

		baseParticle->unk10 |= 0x4;
		baseParticle->unk48 = 0.0f;
		baseParticle->unk4C = sweepShape->unk40;
		baseParticle->unk50 = unk114;

		if (bVar1) {
			baseParticle->unk10 |= 0x40;
		} else {
			particle->unk80 = parent->getDragForce();
			particle->unk84 = parent->getCurrentDragForce();
		}

		particle->unk88 = local_c0;

		f32 sweepShapeUnk1C = sweepShape->unk1C;
		if (sweepShapeUnk1C != 0.0f) {
			JGeometry::TVec3<f32> baseVel;
			baseVel.scale(sweepShapeUnk1C, parent->accessBaseVelVec());
			JGeometry::TVec3<f32> fVel;
			fVel.scale(sweepShapeUnk1C, parent->accessFVelVec());
			baseParticle->setBaseVelVec(baseVel);
			particle->unk94 = fVel;
		} else {
			baseParticle->setBaseVelVec(local_c0);
			particle->unk94 = local_c0;
		}

		if (sweepShape->unkC != 0.0f) {
			JGeometry::TVec3<f32> vec(unk21C.get_ufloat_1() - 0.5f,
			                          unk21C.get_ufloat_1() - 0.5f,
			                          unk21C.get_ufloat_1() - 0.5f);
			vec.setLength(vec, sweepShape->unk20);
			baseParticle->addBaseVelVec(vec);
		}

		particle->unk68.x = 0.0f;
		particle->unk68.y = -1.0f;
		particle->unk68.z = 0.0f;

		particle->unk78 = sweepShape->unk24;
		particle->unk7C = parent->getAirResistance();
		particle->unk74 = parent->getDynamicsWeight();
		baseParticle->setVelocity();

		if (unk188 & 0x10 ? true : false)
			baseParticle->unk10 |= 0x20;

		baseParticle->unk14 = parentUnk14;
		baseParticle->unk20 = parentUnk20;

		f32 sweepShapeUnk8 = sweepShape->unk8;
		if (sweepShapeUnk8 != 0.0f) {
			Mtx rotMtx;
			JPAGetXYRotateMtx(unk21C.get(), unk21C.get(), rotMtx);
			JGeometry::TVec3<f32> vec(0.0f, 0.0f,
			                          unk21C.get_ufloat_1() * sweepShapeUnk8);
			MTXMultVec(rotMtx, vec, vec);
			baseParticle->unk20.add(vec);
		}

		unk30.initChild(parent, baseParticle);
		baseParticle->unk54 = parent->unk54;
	}
}

void JPABaseEmitter::getEmitterGlobalTranslation(JGeometry::TVec3<f32>& vec)
{
	Mtx local_3c;
	PSMTXScale(local_3c, unk154.x, unk154.y, unk154.z);
	PSMTXConcat(unk124, local_3c, local_3c);

	vec.x = unk160.x //
	        + local_3c[0][0] * unk19C.x + local_3c[0][1] * unk19C.y
	        + local_3c[0][2] * unk19C.z;
	vec.y = unk160.y //
	        + local_3c[1][0] * unk19C.x + local_3c[1][1] * unk19C.y
	        + local_3c[1][2] * unk19C.z;
	vec.z = unk160.z //
	        + local_3c[2][0] * unk19C.x + local_3c[2][1] * unk19C.y
	        + local_3c[2][2] * unk19C.z;
}

void JPABaseEmitter::calcEmitterGlobalParams()
{
	JPAEmitterInfoObj.unk4 = &unk20;
	if (unk118->unkC == nullptr || unk118->unkC->unk48 == 0)
		JPAEmitterInfoObj.unk17F = 0;
	else
		JPAEmitterInfoObj.unk17F = 1;

	JPAEmitterInfoObj.unk0 = this;
	JPAGetXYZRotateMtx(unk1A8 * 0xB6, unk1AA * 0xB6, unk1AC * 0xB6,
	                   JPAEmitterInfoObj.unkCC.mMtx);
	Mtx afStack_48;
	PSMTXCopy(JPAEmitterInfoObj.unkCC.mMtx, afStack_48);
	JPAEmitterInfoObj.unk30 = unk190;
	Mtx afStack_78;
	PSMTXScale(afStack_78, unk190.x, unk190.y, unk190.z);
	PSMTXConcat(JPAEmitterInfoObj.unkCC.mMtx, afStack_78,
	            JPAEmitterInfoObj.unkFC.mMtx);
	JGeometry::TVec3<f32> local_3C = unk19C;
	JPAEmitterInfoObj.unk18        = unk154;
	JPAEmitterInfoObj.unk30.x *= unk154.x;
	JPAEmitterInfoObj.unk30.y *= unk154.y;
	JPAEmitterInfoObj.unk30.z *= unk154.z;

	JPAEmitterInfoObj.unkC.x = JPAEmitterInfoObj.unk18.x * 1.0f;
	JPAEmitterInfoObj.unkC.y = JPAEmitterInfoObj.unk18.y * 1.0f;
	JPAEmitterInfoObj.unkC.z = JPAEmitterInfoObj.unk18.z * 1.0f;

	PSMTXScale(JPAEmitterInfoObj.unk9C.mMtx, unk154.x, unk154.y, unk154.z);
	PSMTXCopy(unk124, JPAEmitterInfoObj.unk6C.mMtx);
	PSMTXConcat(JPAEmitterInfoObj.unk6C.mMtx, JPAEmitterInfoObj.unkCC.mMtx,
	            JPAEmitterInfoObj.unkCC.mMtx);
	PSMTXConcat(JPAEmitterInfoObj.unk6C.mMtx, JPAEmitterInfoObj.unkFC.mMtx,
	            JPAEmitterInfoObj.unkFC.mMtx);
	PSMTXConcat(JPAEmitterInfoObj.unk6C.mMtx, JPAEmitterInfoObj.unk9C.mMtx,
	            JPAEmitterInfoObj.unk9C.mMtx);

	JPAEmitterInfoObj.unk9C.mMtx[0][3] = unk160.x;
	JPAEmitterInfoObj.unk9C.mMtx[1][3] = unk160.y;
	JPAEmitterInfoObj.unk9C.mMtx[2][3] = unk160.z;

	PSMTXConcat(JPAEmitterInfoObj.unk9C.mMtx, afStack_48, afStack_48);
	JPAEmitterInfoObj.unk24       = local_3C;
	JPAEmitterInfoObj.unk48[0][0] = JPAEmitterInfoObj.unkCC.mMtx[0][0];
	JPAEmitterInfoObj.unk48[0][1] = JPAEmitterInfoObj.unkCC.mMtx[1][0];
	JPAEmitterInfoObj.unk48[0][2] = JPAEmitterInfoObj.unkCC.mMtx[2][0];
	JPAEmitterInfoObj.unk48[1][0] = JPAEmitterInfoObj.unkCC.mMtx[0][1];
	JPAEmitterInfoObj.unk48[1][1] = JPAEmitterInfoObj.unkCC.mMtx[1][1];
	JPAEmitterInfoObj.unk48[1][2] = JPAEmitterInfoObj.unkCC.mMtx[2][1];
	JPAEmitterInfoObj.unk48[2][0] = JPAEmitterInfoObj.unkCC.mMtx[0][2];
	JPAEmitterInfoObj.unk48[2][1] = JPAEmitterInfoObj.unkCC.mMtx[1][2];
	JPAEmitterInfoObj.unk48[2][2] = JPAEmitterInfoObj.unkCC.mMtx[2][2];

	Vec local_84;
	local_84.x = 0.0f;
	local_84.y = 0.0f;
	local_84.z = 1.0f;
	JGeometry::TVec3<f32> local_90;
	local_90.setLength(unk210, 1.0f);

	bool bVar5              = false;
	bool bVar4              = false;
	JPAEmitterInfoObj.unk3C = local_90;
	if (local_90.x == local_84.x && local_90.y == local_84.y)
		bVar4 = true;

	if (bVar4 == true && local_90.z == local_84.z)
		bVar5 = true;

	if (bVar5) {
		PSMTXIdentity(JPAEmitterInfoObj.unk12C.mMtx);
	} else {
		JPAVecToRotaMtx(
		    JPAEmitterInfoObj.unk12C.mMtx,
		    JGeometry::TVec3<f32>(local_84.x, local_84.y, local_84.z),
		    JGeometry::TVec3<f32>(local_90.x, local_90.y, local_90.z));
	}
}

void JPABaseEmitter::loadBaseEmitterBlock(JPADataBlock* block)
{
	JSUMemoryInputStream stream(block->unk4, *(u32*)((u8*)block->unk4 + 4));
	stream.skip(0xC);
	stream.read(&unk190, 0xC);
	stream.read(&unk19C, 0xC);
	stream.read(&unk1A8, 6);
	stream.read(&unk1AE, 1);
	stream.read(&unk1AF, 1);
	stream.skip(2);
	stream.read(&unk1F2, 2);
	stream.read(&unk1B0, 4);
	unk1B4 = JPAConvertFixToFloat(stream.readS16());
	unk1E8 = stream.readS16();
	stream.read(&unk1EC, 2);
	stream.read(&unk1F0, 2);
	unk1F4 = JPAConvertFixToFloat(stream.readS16());
	unk1F8 = JPAConvertFixToFloat(stream.readS16());
	stream.read(&unk1EE, 2);
	unk1E0 = JPAConvertFixToFloat(stream.readS16());
	unk1D8 = JPAConvertFixToFloat(stream.readS16());
	unk1DC = JPAConvertFixToFloat(stream.readS16());
	unk1C4 = JPAConvertFixToFloat(stream.readS16());
	unk1CC = JPAConvertFixToFloat(stream.readS16());
	unk1D0 = JPAConvertFixToFloat(stream.readS16());
	unk1D4 = JPAConvertFixToFloat(stream.readS16());
	stream.read(&unk1FC, 4);
	stream.read(&unk200, 4);
	stream.read(&unk204, 4);
	stream.read(&unk208, 4);
	stream.read(&unk1C8, 4);

	JGeometry::TVec3<s16> fixVec;
	stream.read(&fixVec, 6);
	JGeometry::TVec3<f32> floatVec;
	JPAConvertFixVecToFloatVec(floatVec, fixVec);
	unk210.setLength(floatVec, 1.0f);
	unk1E4 = JPAConvertFixToFloat(stream.readS16());
	stream.read(&unk188, 4);
	stream.read(&unk18C, 4);
}

void JPABaseEmitter::executeBeforeCallBack() { }

void JPABaseEmitter::executeAfterCallBack() { }

void JPABaseEmitter::drawEmitterCallBack()
{
	if (unk110)
		unk110->draw(this);
}

void JPABaseEmitter::createParticle() { }

void JPABaseEmitter::calcCurrentRateTimerStep()
{
	if (unk1AF == 0) {
		unk1C0 = 1.0f;
	} else {
		unk1C0 = 1.0f / (unk1AF + 1.0f) + 1e-07f;
	}
}

#pragma dont_inline on
void JPABaseEmitter::calcCreateParticle() { }
#pragma dont_inline off

bool JPABaseEmitter::checkStartFrame()
{
	if ((s16)unk18 >= unk1EC)
		return true;

	unk18 += unk1C;
	if (unk18 < 0.0f)
		unk18 = 0.0f;
	return false;
}

bool JPABaseEmitter::checkMaxFrame()
{
	// TODO: constantly true inlines?

	if (unk1E8 == 0)
		return true;

	if (unk1E8 < 0) {
		unk11C |= 8;
		return true;
	}

	if ((s32)unk10.getFrame() >= unk1E8) {
		unk11C |= 8;

		if (unk11C & 0x40)
			return true;
	}

	return false;
}

void JPABaseEmitter::doParticle()
{
	JSUList<JPABaseParticle>* list = getParticleList();

	JSULink<JPABaseParticle>* link = unkF4.getFirst();
	while (link) {
		JPABaseParticle* particle      = link->getObject();
		JSULink<JPABaseParticle>* next = link->getNext();

		particle->incTimer();
		if (!particle->checkFlag(0x80)) {
			particle->calcVelocity();
			particle->executeParticleCallBack(this);
			if (!particle->checkFlag(2)) {
				unk30.calcParticle(particle);
				if (particle->checkCreateChildParticle())
					createChildParticle(particle);
				particle->calcGlobalPosition();
			}
		} else {
			particle->unk10 |= 2;
		}

		if (particle->checkFlag(2)) {
			deleteBaseParticle(particle, list);
		}

		link = next;
	}
}

void JPABaseEmitter::doChildParticle()
{
	JSUList<JPABaseParticle>* list = getChildParticleList();

	JSULink<JPABaseParticle>* link = unk100.getFirst();
	while (link) {
		JPABaseParticle* particle      = link->getObject();
		JSULink<JPABaseParticle>* next = link->getNext();

		particle->incTimer();
		if (!particle->checkFlag(0x80)) {
			if ((int)particle->unk44)
				particle->calcVelocity();
			particle->executeParticleCallBack(this);
			if (!particle->checkFlag(2)) {
				unk30.calcChild(particle);
				particle->calcGlobalPosition();
			}
		} else {
			particle->unk10 |= 2;
		}

		if (particle->checkFlag(2)) {
			deleteBaseParticle(particle, list);
		}

		link = next;
	}
}

void JPABaseEmitter::getKeyValue(f32, u16, f32*) { }

void JPABaseEmitter::calcKeyFrameAnime()
{
	JPAKeyFrameAnime** animeFrames;
	u32 flags;
	u32 bit;
	u32 bitIdx;
	u32 uVar8;

	uVar8 = unk118->unk22;
	if (!uVar8)
		return;

	flags       = unk18C;
	animeFrames = unk118->unk14;
	bit         = 1;
	bitIdx      = 0;

	for (int i = 0; i < uVar8; ++i) {

		u32 stop = false;
		do {
			if ((flags & bit) != 0)
				stop = true;
			bit <<= 1;
			++bitIdx;
		} while (!stop);

		f32 dVar9     = unk10.getFrame();
		u8* animeData = (u8*)animeFrames[i]->unk4;
		f32* frames   = (f32*)(animeData + 0x20);
		u8 frameNum   = animeData[0x10];
		u8 thing      = animeData[0x12];
		if (thing ? true : false)
			dVar9 -= (int)unk10.getFrame()
			         / ((int)frames[(frameNum - 1) * 4] + 1)
			         * ((int)frames[(frameNum - 1) * 4] + 1);

		switch (bitIdx - 1) {
		case 0:
			unk1B0 = JPAGetKeyFrameValue(dVar9, frameNum, frames);
			break;
		case 1:
			unk1F0 = JPAGetKeyFrameValue(dVar9, frameNum, frames);
			break;
		case 2:
			unk1F4 = JPAGetKeyFrameValue(dVar9, frameNum, frames);
			break;
		case 3:
			unk20C = JPAGetKeyFrameValue(dVar9, frameNum, frames);
			break;
		case 4:
			unk1EE = JPAGetKeyFrameValue(dVar9, frameNum, frames);
			break;
		case 5:
			unk1D8 = JPAGetKeyFrameValue(dVar9, frameNum, frames);
			break;
		case 6:
			unk1FC = JPAGetKeyFrameValue(dVar9, frameNum, frames);
			break;
		case 7:
			unk1FC = JPAGetKeyFrameValue(dVar9, frameNum, frames);
			break;
		case 8:
			unk208 = JPAGetKeyFrameValue(dVar9, frameNum, frames);
			break;
		case 9:
			unk1E4 = JPAGetKeyFrameValue(dVar9, frameNum, frames);
			break;
		case 10:
			// TODO: wrong, need JPADraw
			unk1E4 = JPAGetKeyFrameValue(dVar9, frameNum, frames);
			break;
		}
	}
}

void JPABaseEmitter::calc()
{
	bool doAnime = false;

	JPAEmitterInfoObj.unk164 = 0;

	if (unk11C & 2)
		doAnime = true;

	if (!doAnime)
		calcKeyFrameAnime();

	if (unk110)
		unk110->execute(this);

	if (!(unk11C & 2 ? true : false)) {
		calcEmitterGlobalParams();
		unk30.calc();
		unk20.calcFieldParams();
		if (!(unk11C & 8 ? true : false))
			calcCreateParticle();
	}

	if (unk110)
		unk110->executeAfter(this);

	if (!(unk11C & 2 ? true : false)) {
		doParticle();
		doChildParticle();
		unk10.incFrame();
	}
}

void JPABaseEmitter::setGlobalRMatrix(MtxPtr) { }

void JPABaseEmitter::setGlobalRTMatrix(MtxPtr param_1)
{
	JPAGetRMtxTVecElement(param_1, unk124, unk160);
}

void JPABaseEmitter::setGlobalSRTMatrix(MtxPtr param_1)
{
	JPAGetRMtxSTVecElement(param_1, unk124, unk154, unk160);
}

void JPABaseEmitter::getPivotX() { }

void JPABaseEmitter::getPivotY() { }
