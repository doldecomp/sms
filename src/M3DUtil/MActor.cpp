#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <MarioUtil/LightUtil.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <MarioUtil/DrawUtil.hpp>

MActor::MActor(MActorAnmData* param_1)
{
	unk0  = nullptr;
	unk4  = nullptr;
	unk8  = nullptr;
	unkC  = nullptr;
	unk10 = nullptr;
	unk14 = nullptr;
	unk18 = nullptr;
	unk1C = nullptr;
	unk20 = nullptr;
	unk24 = nullptr;
	unk28 = nullptr;
	unk2C = nullptr;
	unk30 = nullptr;

	unk34 = 0;
	unk38 = false;
	unk39 = true;
	unk3C = 0xffff;
	unk40 = true;
	unk44 = 1;

	unk0     = param_1;
	unk28    = new MActorAnmBase*[6];
	unk28[0] = nullptr;
	unk28[1] = nullptr;
	unk28[2] = nullptr;
	unk28[3] = nullptr;
	unk28[4] = nullptr;
	unk28[5] = nullptr;

	if (param_1->getUnk2C()) {
		unkC = new MActorAnmBck;
		unkC->setUnk1C(param_1->getUnk2C());
		unk28[0] = unkC;
	}

	if (param_1->getUnk30()) {
		unk14 = new MActorAnmBpk;
		unk14->setUnk1C(param_1->getUnk30());
		unk14->setMatColorAnmKeyPtr();
		unk28[2] = unk14;
	}

	if (param_1->getUnk34()) {
		unk18 = new MActorAnmBtp;
		unk18->setUnk1C(param_1->getUnk34());
		unk18->setTexNoAnmFullPtr();
		unk28[3] = unk18;
	}

	if (param_1->getUnk38()) {
		unk1C = new MActorAnmBtk;
		unk1C->setUnk1C(param_1->getUnk38());
		unk1C->setTexMtxAnmKeyPtr();
		unk28[4] = unk1C;
	}

	if (param_1->getUnk3C()) {
		unk20 = new MActorAnmBrk;
		unk20->setUnk1C(param_1->getUnk3C());
		unk20->setTevColorAnmKeyPtr();
		unk20->setTevKColorAnmKeyPtr();
		unk28[5] = unk20;
	}

	if (param_1->getUnk40()) {
		unk24 = new MActorAnmBlk;
		unk24->setUnk1C(param_1->getUnk40());
		unk28[1] = unk24;
	}

	if (param_1->getUnk0() > 0) {
		unk10 = new MActorAnmBck*[param_1->getUnk0()];

		JGadget::TList<MActorSubAnmInfo>::iterator it = unk0->unk1C.begin();
		JGadget::TList<MActorSubAnmInfo>::iterator e  = unk0->unk1C.end();

		for (int i = 0; it != e; ++it, ++i) {
			unk10[i] = new MActorAnmBck;
			unk10[i]->setUnk1C(param_1->getUnk2C());
			unk10[i]->unk28 = it->unk0;
		}
	}
}

void MActor::setModel(J3DModel* param_1, u32 param_2)
{
	unk4  = param_1;
	unk34 = param_1->getModelData()->getMaterialNum();
	unk2C = new u16[unk34];
	unk30 = new u16[unk34];
	if ((~param_2 & 0x10000) != 0)
		unk38 = true;

	unk8 = unk4->getModelData()->getJointNodePointer(0)->getMtxCalc();
	for (u16 i = 0; i < unk34; ++i) {
		J3DMaterial* mat = unk4->getModelData()->getMaterialNodePointer(i);
		unk30[i]         = 0x32;
		unk2C[i]         = 0x32;
		for (u16 j = 0; j < mat->getTexGenBlock()->getTexGenNum(); ++j) {
			if (mat->getTexGenBlock()->getTexMtx(j)
			    && mat->getTexGenBlock()->getTexMtx(j)->getInfo()) {
				unk30[i] = i;
			}
		}
	}

	for (int i = 0; i < 6; ++i) {
		if (unk28[i]) {
			unk28[i]->setModel(unk4);
			unk28[i]->checkUseMaterialIDInit(unk2C);
		}
	}

	if (unk0->getUnk0() > 0) {
		JGadget::TList<MActorSubAnmInfo>::iterator it = unk0->unk1C.begin();
		JGadget::TList<MActorSubAnmInfo>::iterator e  = unk0->unk1C.end();
		for (int i = 0; it != e; ++it, ++i) {
			unk10[i]->setModel(unk4);
		}
	}

	for (u16 i = 0; i < unk34; ++i) {
		J3DMaterial* mat    = unk4->getModelData()->getMaterialNodePointer(i);
		J3DMaterialAnm* anm = mat->getMaterialAnm();

		if (anm == nullptr && (unk2C[i] != 0x32 || unk30[i] != 0x32)) {
			anm = new J3DMaterialAnm;
			mat->change();
			mat->setMaterialAnm(anm);
		}
		unk2C[i] = 0x32;
	}

	initDL();

	if (!unk0->getUnk48())
		unk0->createSampleModelData(unk4->getModelData());
}

bool MActor::isCurAnmAlreadyEnd(int param_1)
{
	bool ret = true;

	J3DFrameCtrl* ctrl = getFrameCtrl(param_1);
	if (ctrl) {
		if (!ctrl->checkFlag(1) && !ctrl->checkFlag(2))
			ret = false;
		if (!ret && !(ctrl->mCurrentFrame + 0.1f >= ctrl->mEndFrame))
			ret = false;
	}

	return ret;
}

BOOL MActor::curAnmEndsNext(int param_1, char* param_2)
{
	if (!unk28[param_1])
		return true;

	if (param_1 == 0) {
		if (!param_2) {
			if (unk28[param_1]->unk4.mCurrentFrame + unk28[param_1]->unk4.mSpeed
			        + 0.1f
			    <= unk28[param_1]->unk4.mEndFrame)
				return false;
			else
				return true;
		} else if (unk10) {
			return true;
		} else {
			int idx = unk0->partsNameToIdx(param_2);
			if (unk10[idx]->unk4.mCurrentFrame + unk10[idx]->unk4.mSpeed + 0.1f
			    <= unk10[idx]->unk4.mEndFrame)
				return false;
			else
				return true;
		}
	} else {
		if (unk28[param_1]->unk4.mCurrentFrame + unk28[param_1]->unk4.mSpeed
		        + 0.1f
		    <= unk28[param_1]->unk4.mEndFrame)
			return false;
		else
			return true;
	}
}

void MActor::setAnimation(const char* param_1, int param_2)
{
	if (!unk28[param_2])
		return;

	unk28[param_2]->setAnm(param_1, unk2C);

	if (unk38 && param_2 != 0)
		resetDL();
}

void MActor::initDL()
{
	if (!unk38)
		return;

	j3dSys.setModel(unk4);
	j3dSys.setTexture(unk4->getModelData()->getTexture());
	for (u16 i = 0; i < unk34; ++i) {
		unk4->getMatPacket(i)->unlock();

		if (unk30[i] == 0x32 && unk2C[i] == 0x32) {
			unk4->getModelData()->getMaterialNodePointer(i)->calc(
			    (MtxPtr)&j3dDefaultMtx);
			unk4->getModelData()->getMaterialNodePointer(i)->setCurrentMtx();
			j3dSys.setMatPacket(unk4->getMatPacket(i));
			unk4->getModelData()->getMaterialNodePointer(i)->makeDisplayList();
			unk4->getMatPacket(i)->lock();
		}
	}
}

void MActor::resetDL()
{
	if (!unk38)
		return;

	j3dSys.setModel(unk4);
	j3dSys.setTexture(unk4->getModelData()->getTexture());
	for (u16 i = 0; i < unk34; ++i) {
		if (unk30[i] != 0x32 || unk2C[i] != 0x32) {
			unk4->getMatPacket(i)->unlock();
		} else {
			if (!unk4->getMatPacket(i)->isLocked()) {
				unk4->getModelData()->getMaterialNodePointer(i)->calc(
				    (MtxPtr)&j3dDefaultMtx);
				j3dSys.setMatPacket(unk4->getMatPacket(i));
				unk4->getModelData()
				    ->getMaterialNodePointer(i)
				    ->makeDisplayList();
				unk4->getMatPacket(i)->lock();
			}
		}
	}
}

void MActor::unlockDLIfNeed()
{
	if (!unk38)
		return;

	for (u16 i = 0; i < unk34; ++i)
		if (unk30[i] != 0x32 || unk2C[i] != 0x32)
			unk4->getMatPacket(i)->unlock();
}

void MActor::offMakeDL()
{
	unk38 = false;
	for (u16 i = 0; i < unk34; ++i) {
		unk4->getMatPacket(i)->unlock();
	}
}

void MActor::setJointCallback(int param_1, J3DNodeCallBack param_2)
{
	unk4->getModelData()->getJointNodePointer(param_1)->setCallBack(param_2);
}

void MActor::updateInSubBck()
{
	if (!unk10)
		return;

	for (int i = 0; i < unk0->getUnk0(); ++i)
		if (unk10[i]->getUnk0() >= 0)
			unk10[i]->updateIn();
}

void MActor::updateOutSubBck()
{
	if (!unk10)
		return;

	for (int i = 0; i < unk0->getUnk0(); ++i)
		if (unk10[i]->getUnk0() >= 0)
			unk10[i]->updateOut();
}

void MActor::calcAnm()
{
	frameUpdate();

	updateIn();
	unk4->calc();
	updateOut();
}

void MActor::calc()
{
	if (!unk39)
		return;

	updateIn();
	unk4->calc();
	updateOut();
}

void MActor::viewCalc()
{
	if (unk39)
		unk4->viewCalc();
}

void MActor::setLightID(short light_id)
{
	unk3C = 0;
	unk3C = light_id;
}

void MActor::setLightData(const TBGCheckData*, const JGeometry::TVec3<f32>&) { }

void MActor::setLightType(int param_1)
{
	unk44 = param_1;

	gpLightManager->unk14[param_1]->unk20 = 1;
}

void MActor::entry()
{
	if (!unk39)
		return;

	bool shouldResetLightDrawBuf = false;
	if (unk3C != 0xffff) {
		if (unk3C < 0)
			unk3C = 0;

		gpLightManager->getUnk14(unk44)->changeLightDrawBuffer(unk3C);

		shouldResetLightDrawBuf = true;
	}

	entryIn();
	unk4->entry();
	entryOut();

	if (shouldResetLightDrawBuf)
		gpLightManager->getUnk14(unk44)->resetLightDrawBuffer();
}

void MActor::frameUpdate()
{
	for (int i = 0; i < 6; ++i)
		if (unk28[i] && unk28[i]->getUnk0() >= 0)
			unk28[i]->getFrameCtrl()->update();

	if (unk10)
		for (int i = 0; i < unk0->getUnk0(); ++i)
			if (unk10[i]->getUnk0() >= 0)
				unk10[i]->getFrameCtrl()->update();
}

void MActor::matAnmFrameUpdate()
{
	for (int i = 2; i < 6; ++i)
		if (unk28[i] && unk28[i]->getUnk0() >= 0)
			unk28[i]->getFrameCtrl()->update();
}

void MActor::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 0x2)
		calcAnm();

	if (param_1 & 0x4)
		viewCalc();

	if (param_1 & 0x200)
		entry();
}

bool MActor::checkCurAnm(const char* param_1, int param_2)
{
	if (!unk28[param_2])
		return false;

	if (unk28[param_2]->findName2(param_1) == unk28[param_2]->unk0)
		return true;

	return false;
}

bool MActor::checkCurAnmFromIndex(int param_1, int param_2)
{
	if (!unk28[param_2])
		return false;

	if (param_1 == unk28[param_2]->unk0)
		return true;

	return false;
}

bool MActor::checkAnmFileExist(const char* param_1, int param_2)
{
	if (unk28[param_2] && unk28[param_2]->findName2(param_1) >= 0)
		return true;

	return false;
}

J3DFrameCtrl* MActor::getFrameCtrl(int param_1)
{
	if (!unk28[param_1])
		return nullptr;
	return unk28[param_1]->getFrameCtrl();
}

bool MActor::checkBckPass(float param_1)
{
	if (!unk28[0])
		return false;

	J3DFrameCtrl* ctrl = unk28[0]->getFrameCtrl();

	if (!ctrl)
		return false;

	return ctrl->checkPass(param_1);
}

int MActor::getCurAnmIdx(int param_1) const
{
	if (!unk28[param_1])
		return -1;
	return unk28[param_1]->unk0;
}

void MActor::setFrameRate(float param_1, int param_2)
{
	if (!unk28[param_2])
		return;

	unk28[param_2]->unk4.setSpeed(param_1);
}

void MActor::setBck(const char* name)
{
	if (!unkC)
		return;
	unkC->setAnm(name, 0);
}

void MActor::setBckFromIndex(int index)
{
	if (!unkC)
		return;
	unkC->setAnmFromIndex(index, 0);
}

bool MActor::checkCurBckFromIndex(int index)
{
	if (!unkC)
		return false;
	if (index == unkC->unk0)
		return true;
	return false;
}

void MActor::setBpk(const char* name)
{
	if (!unk14)
		return;
	unk14->setAnm(name, unk2C);
	resetDL();
}

void MActor::setBpkFromIndex(int index)
{
	if (!unk14)
		return;
	unk14->setAnmFromIndex(index, unk2C);
	resetDL();
}

void MActor::setBtp(const char* name)
{
	if (!unk18)
		return;
	unk18->setAnm(name, unk2C);
	resetDL();
}

void MActor::setBtpFromIndex(int index)
{
	if (unk18 == nullptr && index >= 0)
		return;
	if (index < 0)
		return;
	unk18->setAnmFromIndex(index, unk2C);
	resetDL();
}

void MActor::setBtk(const char* name)
{
	if (!unk1C)
		return;
	unk1C->setAnm(name, unk2C);
	resetDL();
}

void MActor::setBtkFromIndex(int index)
{
	if (!unk1C)
		return;
	unk1C->setAnmFromIndex(index, unk2C);
	resetDL();
}

void MActor::setBlk(const char* name)
{
	if (!unk24)
		return;
	unk24->setAnm(name, unk2C);
	resetDL();
}

void MActor::setBrk(const char* name)
{
	if (!unk20)
		return;
	unk20->setAnm(name, unk2C);
	resetDL();
}

void MActor::setBrkFromIndex(int index)
{
	if (!unk20)
		return;
	unk20->setAnmFromIndex(index, unk2C);
	resetDL();
}

void MActor::updateIn()
{
	if (unk28[0] && unk28[0]->getUnk0() >= 0)
		unk28[0]->updateIn();

	updateInSubBck();

	if (unk28[1] && unk28[1]->getUnk0() >= 0)
		unk28[1]->updateIn();
}

void MActor::updateOut()
{
	if (unk28[0] && unk28[0]->getUnk0() >= 0)
		unk28[0]->updateOut();

	updateOutSubBck();

	if (unk28[1] && unk28[1]->getUnk0() >= 0)
		unk28[1]->updateOut();
}

void MActor::entryIn()
{
	for (int i = 2; i < 6; ++i)
		if (unk28[i] && unk28[i]->getUnk0() >= 0)
			unk28[i]->updateIn();
}

void MActor::entryOut()
{
	for (int i = 2; i < 6; ++i)
		if (unk28[i] && unk28[i]->getUnk0() >= 0)
			unk28[i]->updateOut();
}

void MActor::updateMatAnm()
{
	j3dSys.setTexture(unk4->getModelData()->getTexture());
	for (u16 i = 0; i < unk34; ++i)
		if (unk30[i] != 0x32 || unk2C[i] != 0x32)
			SMS_CalcMatAnmAndMakeDL(unk4, i);
}
