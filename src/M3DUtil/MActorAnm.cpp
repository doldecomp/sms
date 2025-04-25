#include <M3DUtil/MActorAnm.hpp>
#include <M3DUtil/MActorData.hpp>
#include <M3DUtil/MotionBlendCtrl.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DCluster.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>

void MActorAnmBase::checkUseMaterialIDInit(u16*) { }

void MActorAnmBase::checkUseMaterialID(u16*) { }

void MActorAnmBck::initSimpleMotionBlend(int param_1)
{
	if (!unk34) {
		bool thing = false;
		if (unk2A == 0)
			thing = true;
		unk34 = new TMotionBlendCtrl(thing, param_1);
	}
	unk2A = 2;
}

void MActorAnmBck::initNormalMotionBlend()
{
	if (!unk34) {
		bool thing = false;
		if (unk2A == 0)
			thing = true;
		unk34 = new TMotionBlendCtrl(thing);
	}
	unk2A = 2;
}

float MActorAnmBck::getMotionBlendRatio() const
{
	if (!unk34)
		return 0.0f;
	else
		return unk34->getMotionBlendRatio();
}

void MActorAnmBck::setMotionBlendRatio(float param_1)
{
	if (unk34)
		unk34->setMotionBlendRatio(param_1);
}

J3DAnmTransform* MActorAnmBck::getOldMotionBlendAnmPtr() const
{
	if (!unk34)
		return nullptr;
	else
		return unk34->getOldMotionBlendAnmPtr();
}

void MActorAnmBck::setOldMotionBlendAnmPtr(J3DAnmTransform* param_1)
{
	if (unk34)
		unk34->setOldMotionBlendAnmPtr(param_1);
}

float MActorAnmBck::getOldMotionBlendFrame() const
{
	if (unk34)
		return unk34->getOldMotionBlendFrame();
	else
		return 0.0f;
}

// TODO: figure out something w/ these "things"
inline J3DMtxCalcSoftimageAnm* createThing()
{
	return new J3DMtxCalcSoftimageAnm(nullptr);
}
inline J3DMtxCalcBasicAnm* createThing2()
{
	return new J3DMtxCalcBasicAnm(nullptr);
}

void MActorAnmBck::setModel(J3DModel* param_1)
{
	u8 thing = 0;
	unk18    = param_1;
	if (param_1->getModelData()->unkC & 1)
		thing = 1;
	unk2A = thing;
	switch (thing) {
	case 1:
		if (unk30 == nullptr)
			unk30 = createThing();
		break;
	case 0:
		if (unk2C == nullptr)
			unk2C = createThing2();
		break;
	}
}

void MActorAnmBck::updateIn()
{
	J3DJoint* joint = unk18->getModelData()->getJointNodePointer(unk28);
	unk24->setFrame(unk4.getCurrentFrame());
	switch (unk2A) {
	case 0:
		unk2C->mOne[0] = unk24;
		joint->setMtxCalc(unk2C);
		break;
	case 1:
		unk30->mOne[0] = unk24;
		joint->setMtxCalc(unk30);
		break;
	case 2:
		joint->setMtxCalc(unk34->unk8);
		unk34->execSimpleMotionBlend();
		break;
	case 3:
		joint->setMtxCalc(unk38);
		break;
	}
}

void MActorAnmBck::updateOut()
{
	unk18->getModelData()->getJointNodePointer(unk28)->setMtxCalc(nullptr);
}

void MActorAnmBck::setAnmFromIndex(int param_1, u16* param_2)
{
	if (unk2A == 2 && unk0 != -1) {
		unk34->keepCurAnm(getData()->getAnmPtr(unk0), unk4.getCurrentFrame());
	}

	unk0 = param_1;

	if (param_1 >= 0) {
		unk24 = getData()->getAnmPtr(param_1);
		unk4.init(unk24->getFrameMax());
		unk4.setAttribute(unk24->getAttribute());
		unk4.setRate(SMSGetAnmFrameRate());
	}

	if (unk2A == 2) {
		unk34->setNewAnm(getData()->getAnmPtr(unk0));
	}
}

void MActorAnmBtp::setTexNoAnmFullPtr()
{
	int count = unk1C->getAnmNum();
	unk28     = new J3DTexNoAnm*[count];
	for (int i = 0; i < count; ++i) {
		J3DAnmTexPattern* anm = getData()->getAnmPtr(i);

		int materials = anm->getUpdateMaterialNum();
		unk28[i]      = new J3DTexNoAnm[materials];

		for (int j = 0; j < materials; ++j) {
			unk28[i][j].setAnmIndex(j);
			unk28[i][j].setAnmTexPattern(getData()->getAnmPtr(i));
		}
	}
}

void MActorAnmBtp::checkUseMaterialIDInit(u16* param_1)
{
	for (int i = 0; i < getData()->getAnmNum(); ++i) {
		J3DAnmTexPattern* anm = getData()->getAnmPtr(i);
		for (u16 j = 0; j < anm->getUpdateMaterialNum(); ++j) {
			u16 id = anm->getUpdateMaterialID(j);
			for (u16 k = 0; k < unk18->getModelData()->getMaterialNum(); ++k) {
				if (strcmp(anm->getUpdateMaterialName()->getName(j),
				           unk18->getModelData()->getMaterialName()->getName(k))
				    == 0) {
					param_1[k] = id;
					break;
				}
			}
		}
	}
}

void MActorAnmBtp::checkUseMaterialID(u16* param_1)
{
	if (param_1 == nullptr || unk24 == nullptr)
		return;

	for (u16 i = 0; i < unk24->getUpdateMaterialNum(); ++i) {
		u32 id = unk24->getUpdateMaterialID(i);
		if (id != 0xffff)
			param_1[id] = id;
	}
}

void MActorAnmBtp::updateIn()
{
	unk24->setFrame(unk4.getCurrentFrame());
	unk18->getModelData()->setTexNoAnimator(unk24, unk28[unk0]);
}

void MActorAnmBtp::updateOut()
{
	unk18->getModelData()->removeTexNoAnimator(unk24);
}

void MActorAnmBtk::checkUseMaterialIDInit(u16* param_1)
{
	for (int i = 0; i < getData()->getAnmNum(); ++i) {
		J3DAnmTextureSRTKey* anm = getData()->getAnmPtr(i);
		for (u16 j = 0; j < anm->getUpdateMaterialNum(); ++j) {
			for (u16 k = 0; k < unk18->getModelData()->getMaterialNum(); ++k) {
				if (strcmp(anm->getUpdateMaterialName()->getName(j),
				           unk18->getModelData()->getMaterialName()->getName(k))
				    == 0) {
					param_1[k] = j;
					break;
				}
			}
		}
	}
}

void MActorAnmBtk::checkUseMaterialID(u16* param_1)
{
	if (param_1 == nullptr || unk24 == nullptr)
		return;

	for (u16 i = 0; i < unk24->getUpdateMaterialNum(); ++i) {
		u32 id = unk24->getUpdateMaterialID(i);
		if (id != 0xffff)
			param_1[id] = id;
	}
}

void MActorAnmBtk::setTexMtxAnmKeyPtr()
{
	int count = unk1C->getAnmNum();
	unk28     = new J3DTexMtxAnm*[count];
	for (int i = 0; i < count; ++i) {
		J3DAnmTextureSRTKey* anm = getData()->getAnmPtr(i);

		int materials = anm->getUpdateMaterialNum();
		unk28[i]      = new J3DTexMtxAnm[materials];

		for (int j = 0; j < materials; ++j) {
			unk28[i][j].setAnmIndex(j);
			unk28[i][j].setAnmTransform(getData()->getAnmPtr(i));
		}
	}
}

void MActorAnmBtk::updateIn()
{
	unk24->setFrame(unk4.getCurrentFrame());
	unk18->getModelData()->setTexMtxAnimator(unk24, unk28[unk0], unk28[unk0]);
}

void MActorAnmBtk::updateOut()
{
	unk18->getModelData()->removeTexMtxAnimator(unk24);
}

void MActorAnmBpk::checkUseMaterialIDInit(u16* param_1)
{
	for (int i = 0; i < getData()->getAnmNum(); ++i) {
		J3DAnmColorKey* anm = getData()->getAnmPtr(i);
		for (u16 j = 0; j < anm->getUpdateMaterialNum(); ++j) {
			for (u16 k = 0; k < unk18->getModelData()->getMaterialNum(); ++k) {
				if (strcmp(anm->getUpdateMaterialName()->getName(j),
				           unk18->getModelData()->getMaterialName()->getName(k))
				    == 0) {
					param_1[k] = j;
					break;
				}
			}
		}
	}
}

void MActorAnmBpk::checkUseMaterialID(u16* param_1)
{
	if (param_1 == nullptr || unk24 == nullptr)
		return;

	for (u16 i = 0; i < unk24->getUpdateMaterialNum(); ++i) {
		u32 id = unk24->getUpdateMaterialID(i);
		if (id != 0xffff)
			param_1[id] = id;
	}
}

void MActorAnmBpk::setMatColorAnmKeyPtr()
{
	int count = unk1C->getAnmNum();
	unk28     = new J3DMatColorAnm*[count];
	for (int i = 0; i < count; ++i) {
		J3DAnmColorKey* anm = getData()->getAnmPtr(i);

		int materials = anm->getUpdateMaterialNum();
		unk28[i]      = new J3DMatColorAnm[materials];

		for (int j = 0; j < materials; ++j) {
			unk28[i][j].setAnmIndex(j);
			unk28[i][j].setAnmColor(getData()->getAnmPtr(i));
		}
	}
}

void MActorAnmBpk::updateIn()
{
	unk24->setFrame(unk4.getCurrentFrame());
	unk18->getModelData()->setMatColorAnimator(unk24, unk28[unk0]);
}

void MActorAnmBpk::updateOut()
{
	unk18->getModelData()->removeMatColorAnimator(unk24);
}

void MActorAnmBrk::setTevColorAnmKeyPtr()
{
	int count = unk1C->getAnmNum();
	unk28     = new J3DTevColorAnm*[count];
	for (int i = 0; i < count; ++i) {
		J3DAnmTevRegKey* anm = getData()->getAnmPtr(i);

		int materials = anm->getCRegUpdateMaterialNum();
		unk28[i]      = new J3DTevColorAnm[materials];

		for (int j = 0; j < materials; ++j) {
			unk28[i][j].setAnmIndex(j);
			unk28[i][j].setAnmTevReg(getData()->getAnmPtr(i));
		}
	}
}

void MActorAnmBrk::setTevKColorAnmKeyPtr()
{
	int count = unk1C->getAnmNum();
	unk2C     = new J3DTevKColorAnm*[count];
	for (int i = 0; i < count; ++i) {
		J3DAnmTevRegKey* anm = getData()->getAnmPtr(i);

		int materials = anm->getKRegUpdateMaterialNum();
		unk2C[i]      = new J3DTevKColorAnm[materials];

		for (int j = 0; j < materials; ++j) {
			unk2C[i][j].setAnmIndex(j);
			unk2C[i][j].setAnmTevReg(getData()->getAnmPtr(i));
		}
	}
}

void MActorAnmBrk::updateIn()
{
	unk24->setFrame(unk4.getCurrentFrame());
	unk18->getModelData()->setTevRegAnimator(unk24, unk28[unk0], unk2C[unk0]);
}

void MActorAnmBrk::updateOut()
{
	unk18->getModelData()->removeTevRegAnimator(unk24);
}

void MActorAnmBrk::checkUseMaterialIDInit(u16* param_1)
{
	for (u16 i = 0; i < unk18->getModelData()->getMaterialNum(); ++i)
		if (param_1[i] == 0x32)
			param_1[i] = i;
}

void MActorAnmBrk::checkUseMaterialID(u16* param_1)
{
	for (u16 i = 0; i < unk18->getModelData()->getMaterialNum(); ++i)
		if (param_1[i] == 0x32)
			param_1[i] = i;
}

void MActorAnmBlk::updateIn()
{
	if (unk24->getFrameMax() <= unk4.getCurrentFrame() + 1.0f)
		unk4.setFrame(0.0f);
	unk24->setFrame(unk4.getCurrentFrame());
	unk28->setAnm(unk24);
}

void MActorAnmBlk::updateOut() { unk28->setAnm(nullptr); }

void MActorAnmBlk::setAnmFromIndex(int param_1, u16* param_2)
{
	unk0 = param_1;

	if (param_1 >= 0) {
		unk24 = getData()->getAnmPtr(param_1);
		unk4.init(unk24->getFrameMax());
		unk4.setAttribute(unk24->getAttribute());
		unk4.setRate(SMSGetAnmFrameRate());
	}
}
