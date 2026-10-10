#include <JSystem/JParticle/JPAField.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include <JSystem/JParticle/JPAParticle.hpp>
#include <JSystem/JParticle/JPADataBlock.hpp>
#include <JSystem/JParticle/JPAMath.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <JSystem/JMath.hpp>

static JMath::TRandomFast FieldRand(0);

// TODO: fabricated; centered-sample inline follows JPABaseEmitter::getRandomSF.
static inline f32 getRandomSF() { return FieldRand.get_ufloat_1() - 0.5f; }

JPABaseField::JPABaseField()
    : unk0(this)
{
	unk54 = 0;

	unk18.zero();
	unk7C.zero();

	unk24.x = 0.0f;
	unk24.y = -1.0f;
	unk24.z = 0.0f;

	unk10         = 1.0f;
	unk14         = 0.0f;
	mFadeInEnd    = 0.0f;
	mFadeOutStart = 1.0f;
	mFadeInStart  = 0.0f;
	mFadeOutEnd   = 1.0f;
	mMaxDistance  = 0.0f;
	unk52         = 0;
}
JPABaseField::~JPABaseField() { }
void JPABaseField::set() { }
void JPABaseField::calcMaxDistance()
{
	if (checkStatus(STATUS_LIMIT_DISTANCE))
		mMaxDistanceSq = mMaxDistance * mMaxDistance;
}
bool JPABaseField::checkMaxDistance(JGeometry::TVec3<f32>& param_1,
                                    JGeometry::TVec3<f32>& param_2)
{
	bool result = false;
	if ((param_1.x - param_2.x) * (param_1.x - param_2.x)
	        + (param_1.y - param_2.y) * (param_1.y - param_2.y)
	        + (param_1.z - param_2.z) * (param_1.z - param_2.z)
	    >= mMaxDistanceSq) {
		result = true;
	}
	return result;
}

// TODO: fabricated; original reciprocal-estimate helper identity is unknown.
static inline f32 getReciprocal(register f32 value)
{
	// TODO: bare fres needs a register operand; __fres enables status CSE.
	asm { fres value, value }
	return value;
}

f32 JPABaseField::calcFieldFadeScale(f32 progress)
{
	bool cutOff = false;
	f32 result  = 1.0f;
	if (checkStatus(STATUS_FADE_ENABLE_START_CUTOFF)
	    && progress < mFadeInStart) {
		result = 0.0f;
		cutOff = true;
	}

	if (checkStatus(STATUS_FADE_ENABLE_END_CUTOFF) && progress >= mFadeOutEnd) {
		result = 0.0f;
		cutOff = true;
	}

	if (!cutOff) {
		if (checkStatus(STATUS_FADE_ENABLE_FADE_OUT)
		    && progress >= mFadeOutStart) {
			f32 diff = mFadeOutEnd - mFadeOutStart;
			if (diff > 0.0f) {
				diff   = getReciprocal(diff);
				result = diff * (mFadeOutEnd - progress);
			}
		} else if (checkStatus(STATUS_FADE_ENABLE_FADE_IN)
		           && progress < mFadeInEnd) {
			f32 diff = mFadeInEnd - mFadeInStart;
			if (diff > 0.0f) {
				diff   = getReciprocal(diff);
				result = diff * (progress - mFadeInStart);
			}
		}
	}

	return result;
}

void JPABaseField::calcFieldVelocity(JPAParticle* particle)
{
	JGeometry::TVec3<f32> local_14;
	local_14.set(unk7C.x, unk7C.y, unk7C.z);

	if (!particle->checkStatus(JPABaseParticle::FLAG_UNK4)) {
		f32 progress = particle->mLifeProgress;
		f32 scale    = calcFieldFadeScale(progress);
		if (scale <= 0.0f) {
			local_14.zero();
		} else if (scale < 1.0f)
			local_14.scale(scale);
	}

	switch (unk52) {
	case 0:
		particle->mFieldAcceleration.add(local_14);
		break;
	case 1:
		particle->addBaseVelVec(local_14);
		break;
	case 2:
		particle->mFieldVelocity.add(local_14);
		break;
	}
}

void JPABaseField::affect(JPAParticle* particle)
{
	calcFieldVelocity(particle);
}
// TODO: nonmatching stack frame (0x48 instead of 0x50); no fabricated padding.
void JPABaseField::loadFieldBlock(JPADataBlock* block)
{
	s16 fix;

	JSUMemoryInputStream streamImpl(block->getRawData(),
	                                *(u32*)(block->getRawData() + 4));
	JSUInputStream& stream = streamImpl; // TODO: fakematch?

	stream.skip(0xC);
	stream >> unk50;
	stream >> unk51;
	stream >> unk52;
	stream >> unk53;
	stream >> unk54;
	stream.skip(0x2);
	stream >> unk10;
	stream >> unk14;
	stream >> mMaxDistance;
	stream >> unk18.x;
	stream >> unk18.y;
	stream >> unk18.z;
	stream >> unk24.x;
	stream >> unk24.y;
	stream >> unk24.z;
	stream >> unk30;
	stream >> unk34;
	stream >> unk38;

	stream >> fix;
	mFadeInEnd = JPAConvertFixToFloat(fix);
	stream >> fix;
	mFadeOutStart = JPAConvertFixToFloat(fix);
	stream >> fix;
	mFadeInStart = JPAConvertFixToFloat(fix);
	stream >> fix;
	mFadeOutEnd = JPAConvertFixToFloat(fix);
}

JPAGravityField::JPAGravityField() { unk50 = 0; }
JPAGravityField::~JPAGravityField() { }
void JPAGravityField::set()
{
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();
	JGeometry::TVec3<f32> local_18;
	local_18.set(unk24.x, unk24.y, unk24.z);
	if (!checkStatus(STATUS_USE_GLOBAL_COORDS))
		MTXMultVec(info->unk6C, &local_18, &local_18);
	unk7C.scale(unk10, local_18);
}

JPAAirField::JPAAirField() { unk50 = 1; }
void JPAAirField::set()
{
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();
	unk70.set(unk24);
	if (!checkStatus(STATUS_USE_GLOBAL_COORDS))
		MTXMultVec(info->unk6C, &unk70, &unk70);

	unk7C.scale(unk10, unk70);
	if (checkStatus(STATUS_AIR_CONE)) {
		unk64.x = JMASCos(unk30 * 32768.0f);
		if (!checkStatus(STATUS_USE_GLOBAL_COORDS))
			MTXMultVec(info->unk6C, &unk18, &unk58);
		else
			unk58.set(unk18);
	}
}
void JPAAirField::affect(JPAParticle* particle)
{
	JGeometry::TVec3<f32> diff;
	f32 len;
	JGeometry::TVec3<f32> vec;

	if (checkStatus(STATUS_AIR_CONE)) {
		if (!checkStatus(STATUS_USE_GLOBAL_COORDS)) {
			const JGeometry::TVec3<f32>& pos = particle->mLocalPosition;
			diff.sub(pos, unk58);
		} else {
			const JGeometry::TVec3<f32>& pos = particle->mGlobalPosition;
			diff.sub(pos, unk58);
		}

		JGeometry::TVec3<f32> dir;
		dir.set(diff);
		dir.normalize();
		if (unk70.dot(dir) >= unk64.x)
			calcFieldVelocity(particle);
	} else {
		calcFieldVelocity(particle);
	}

	if (checkStatus(STATUS_AIR_CLAMP_VELOCITY)) {
		particle->getBaseVelVec(vec);
		len = vec.length();
		if (len > unk14) {
			vec.scale(unk14 / len);
			particle->setBaseVelVec(vec);
		}
	}
}

JPAMagnetField::JPAMagnetField() { unk50 = 2; }
JPAMagnetField::~JPAMagnetField() { }
void JPAMagnetField::set()
{
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();
	if (!checkStatus(STATUS_USE_GLOBAL_COORDS))
		MTXMultVec(info->unk6C.mMtx, &unk18, &unk58);
	else
		unk58.set(unk18);
}
void JPAMagnetField::affect(JPAParticle* particle)
{
	if (!checkStatus(STATUS_USE_GLOBAL_COORDS))
		unk7C.sub(unk58, particle->mLocalPosition);
	else
		unk7C.sub(unk58, particle->mGlobalPosition);

	unk7C.setLength(unk10);
	calcFieldVelocity(particle);
}

JPANewtonField::JPANewtonField() { unk50 = 3; }
JPANewtonField::~JPANewtonField() { }
void JPANewtonField::set()
{
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();
	if (!checkStatus(STATUS_USE_GLOBAL_COORDS)) {
		MTXMultVec(info->unk6C.mMtx, &unk18, &unk58);
	} else {
		unk58.set(unk18);
	}
	unk34 = unk30 * unk30;
}
void JPANewtonField::affect(JPAParticle* particle)
{
	if (!checkStatus(STATUS_USE_GLOBAL_COORDS))
		unk7C.sub(unk58, particle->mLocalPosition);
	else
		unk7C.sub(unk58, particle->mGlobalPosition);

	if (checkStatus(STATUS_NEWTON_CONSTANT_FORCE)) {
		unk7C.setLength(unk10);
	} else {
		f32 lsq   = unk7C.squared();
		f32 fVar2 = lsq > unk34 ? (unk34 * 10.0f) / lsq : 10;
		unk7C.setLength(unk10 * fVar2);
	}
	calcFieldVelocity(particle);
}

JPAVortexField::JPAVortexField()
{
	unk24.x = 0.0f;
	unk24.y = 1.0f;
	unk24.z = 0.0f;
	unk50   = 4;
}
JPAVortexField::~JPAVortexField() { }
void JPAVortexField::set()
{
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();
	MTXMultVec(info->unkCC, &unk24, &unk58);
	unk30 = unk18.z * unk18.z;
	unk34 = 1.0f / unk30;
}
// TODO: fabricated; clamped-strength inline boundary matches FPR allocation.
// The original helper's identity is not established.
static inline f32 calcVortexStrength(const JPAVortexField& field, f32 fVar1)
{
	if (fVar1 > field.unk30)
		fVar1 = field.unk30;
	f32 ratio = fVar1 * field.unk34;
	return (1.0f - ratio) * field.unk10 + ratio * field.unk14;
}

void JPAVortexField::affect(JPAParticle* particle)
{
	JGeometry::TVec3<f32> localPos;
	particle->getLocalPosition(localPos);
	JGeometry::TVec3<f32> projected;
	projected.scale(unk58.dot(localPos), unk58);
	JGeometry::TVec3<f32> thing3;
	thing3.sub(localPos, projected);

	f32 fVar2 = calcVortexStrength(*this, thing3.squared());

	JGeometry::TVec3<f32> tmp;
	tmp.normalize(thing3);
	unk7C.cross2(tmp, unk58);
	unk7C.scale(fVar2);
	calcFieldVelocity(particle);
}
bool JPAVortexField::checkMaxDistance(JGeometry::TVec3<float>&,
                                      JGeometry::TVec3<float>&)
{
	return false;
}

JPAConvectionField::JPAConvectionField()
{
	unk50 = 7;

	unk24.x = 0.0f;
	unk24.y = 1.0f;
	unk24.z = 0.0f;

	unk18.x = 1.0f;
	unk18.y = 0.0f;
	unk18.z = 0.0f;
}
JPAConvectionField::~JPAConvectionField() { }
void JPAConvectionField::set()
{
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();
	JGeometry::TVec3<f32> prod;
	prod.cross2(unk18, unk24);
	unk18.cross2(prod, unk24);
	unk18.normalize();

	MTXMultVec(info->unkCC, &unk18, &unk58);
	MTXMultVec(info->unkCC, &unk24, &unk64);
	MTXMultVec(info->unkCC, &prod, &unk70);

	unk58.normalize();
	unk64.normalize();
	unk70.normalize();
}
void JPAConvectionField::affect(JPAParticle* particle)
{
	JGeometry::TVec3<f32> thing;
	particle->getLocalPosition(thing);

	JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
	JGeometry::TVec3<f32> thing2;
	if (up == unk64) {
		thing2.set(thing.x, 0.0f, thing.z);
	} else {
		JGeometry::TVec3<f32> a;
		a.scale(unk58.dot(thing), unk58);
		JGeometry::TVec3<f32> b;
		b.scale(unk70.dot(thing), unk70);
		thing2.add(a, b);
	}
	JGeometry::TVec3<f32> projected;
	projected.setLength(thing2, unk30);

	JGeometry::TVec3<f32> thing4;
	thing4.sub(thing, projected);

	JGeometry::TVec3<f32> thing3;
	thing3.cross(unk64, projected);

	unk7C.cross(thing3, thing4);
	unk7C.setLength(unk10);
	if (unk34 != 0.0f) {
		JGeometry::TVec3<f32> contribution;
		contribution.setLength(thing4, unk34);
		unk7C.add(contribution);
	}
	calcFieldVelocity(particle);
}
bool JPAConvectionField::checkMaxDistance(JGeometry::TVec3<float>&,
                                          JGeometry::TVec3<float>&)
{
	return false;
}

JPARandomField::JPARandomField() { unk50 = 5; }
JPARandomField::~JPARandomField() { }
void JPARandomField::affect(JPAParticle* particle)
{
	bool bVar3 = false;
	if (particle->getAge() == 0) {
		bVar3 = true;
	} else {
		if (unk53 != 0) {
			if (particle->getAge() % unk53 == 0) {
				bVar3 = true;
			}
		}
	}

	if (bVar3) {
		unk7C.set(getRandomSF(), getRandomSF(), getRandomSF());
		unk7C.scale(unk10);
		calcFieldVelocity(particle);
	}
}

JPADragField::JPADragField() { unk50 = 6; }
JPADragField::~JPADragField() { }
void JPADragField::affect(JPAParticle* particle)
{
	if (!particle->checkStatus(JPABaseParticle::FLAG_UNK4)) {
		if (particle->getAge() == 0) {
			f32 rnd = unk14 * getRandomSF() + unk10;
			if (rnd > 1.0f)
				rnd = 1.0f;
			particle->mDragForce = rnd;
		}
		f32 scale = calcFieldFadeScale(particle->getLifeTime());
		particle->mCurrentDragForce
		    *= 1.0f - scale * (1.0f - particle->getDragForce());
	} else {
		particle->mCurrentDragForce *= particle->getDragForce();
	}
}

JPAFieldManager::JPAFieldManager() { }
JPAFieldManager::~JPAFieldManager() { }

void JPAFieldManager::deleteField(JPABaseField* field)
{
	unk0.remove(field->getLinkBufferPtr());
	getFieldPool()->prepend(field->getLinkBufferPtr());
}

void JPAFieldManager::deleteAllField()
{
	if (unk0.getNumLinks() == 0)
		return;

	JSULink<JPABaseField>* link = unk0.getFirst();
	while (link) {
		JSULink<JPABaseField>* next = link->getNext();
		JPABaseField* field         = link->getObject();
		deleteField(field);
		link = next;
	}
}

void JPAFieldManager::calcFieldParams()
{
	if (unk0.getNumLinks() == 0)
		return;

	JSULink<JPABaseField>* link = unk0.getFirst();
	while (link) {
		JSULink<JPABaseField>* next = link->getNext();
		JPABaseField* field         = link->getObject();

		field->calcMaxDistance();
		field->set();

		link = next;
	}
}

void JPAFieldManager::affectField(JPAParticle* particle)
{
	JGeometry::TVec3<f32>& particlePos = particle->mGlobalPosition;

	for (JSULink<JPABaseField>* link = unk0.getFirst(); link != nullptr;
	     link                        = link->getNext()) {
		JPABaseField* field = link->getObject();
		if (!field->checkStatus(JPABaseField::STATUS_LIMIT_DISTANCE)) {
			field->affect(particle);
		} else if (!field->checkMaxDistance(particlePos, field->getUnk18())) {
			field->affect(particle);
		}
	}
}

JPABaseField* JPAFieldManager::setField(u8 param_1)
{
	JPABaseField* result = nullptr;
	switch (param_1) {
	case 0:
		result = setGravityField();
		break;
	case 1:
		result = setAirField();
		break;
	case 2:
		result = setMagnetField();
		break;
	case 3:
		result = setNewtonField();
		break;
	case 4:
		result = setVortexField();
		break;
	case 5:
		result = setRandomField();
		break;
	case 6:
		result = setDragField();
		break;
	case 7:
		result = setConvectionField();
		break;
	}
	JUT_ASSERT(result != nullptr);
	return result;
}

JPAGravityField* JPAFieldManager::setGravityField()
{
	JPAGravityField* result = nullptr;

	if (getFieldPool()->getNumLinks() != 0) {
		result = (JPAGravityField*)getFieldPool()->getFirst()->getObject();
		getFieldPool()->remove(result->getLinkBufferPtr());
		new (result) JPAGravityField;
		unk0.append(result->getLinkBufferPtr());
	}

	return result;
}

JPAAirField* JPAFieldManager::setAirField()
{
	JPAAirField* result = nullptr;

	if (getFieldPool()->getNumLinks() != 0) {
		result = (JPAAirField*)getFieldPool()->getFirst()->getObject();
		getFieldPool()->remove(result->getLinkBufferPtr());
		new (result) JPAAirField;
		unk0.append(result->getLinkBufferPtr());
	}

	return result;
}

JPAMagnetField* JPAFieldManager::setMagnetField()
{
	JPAMagnetField* result = nullptr;

	if (getFieldPool()->getNumLinks() != 0) {
		result = (JPAMagnetField*)getFieldPool()->getFirst()->getObject();
		getFieldPool()->remove(result->getLinkBufferPtr());
		new (result) JPAMagnetField;
		unk0.append(result->getLinkBufferPtr());
	}

	return result;
}

JPANewtonField* JPAFieldManager::setNewtonField()
{
	JPANewtonField* result = nullptr;

	if (getFieldPool()->getNumLinks() != 0) {
		result = (JPANewtonField*)getFieldPool()->getFirst()->getObject();
		getFieldPool()->remove(result->getLinkBufferPtr());
		new (result) JPANewtonField;
		unk0.append(result->getLinkBufferPtr());
	}

	return result;
}

JPAVortexField* JPAFieldManager::setVortexField()
{
	JPAVortexField* result = nullptr;

	if (getFieldPool()->getNumLinks() != 0) {
		result = (JPAVortexField*)getFieldPool()->getFirst()->getObject();
		getFieldPool()->remove(result->getLinkBufferPtr());
		new (result) JPAVortexField;
		unk0.append(result->getLinkBufferPtr());
	}

	return result;
}

JPAConvectionField* JPAFieldManager::setConvectionField()
{
	JPAConvectionField* result = nullptr;

	if (getFieldPool()->getNumLinks() != 0) {
		result = (JPAConvectionField*)getFieldPool()->getFirst()->getObject();
		getFieldPool()->remove(result->getLinkBufferPtr());
		new (result) JPAConvectionField;
		unk0.append(result->getLinkBufferPtr());
	}

	return result;
}

JPARandomField* JPAFieldManager::setRandomField()
{
	JPARandomField* result = nullptr;

	if (getFieldPool()->getNumLinks() != 0) {
		result = (JPARandomField*)getFieldPool()->getFirst()->getObject();
		getFieldPool()->remove(result->getLinkBufferPtr());
		new (result) JPARandomField;
		unk0.append(result->getLinkBufferPtr());
	}

	return result;
}

JPADragField* JPAFieldManager::setDragField()
{
	JPADragField* result = nullptr;

	if (getFieldPool()->getNumLinks() != 0) {
		result = (JPADragField*)getFieldPool()->getFirst()->getObject();
		getFieldPool()->remove(result->getLinkBufferPtr());
		new (result) JPADragField;
		unk0.append(result->getLinkBufferPtr());
	}

	return result;
}
