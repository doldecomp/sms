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
#include <Camera/CubeManagerBase.hpp>
#include <Map/MapData.hpp>

MActor::MActor(MActorAnmData* anm_data)
{
	mAnmData   = nullptr;
	mModel     = nullptr;
	unk8       = nullptr;
	mAnmBck    = nullptr;
	unk10      = nullptr;
	mAnmBpk    = nullptr;
	mAnmBtp    = nullptr;
	mAnmBtk    = nullptr;
	mAnmBrk    = nullptr;
	mAnmBlk    = nullptr;
	mAnmByType = nullptr;
	unk2C      = nullptr;
	unk30      = nullptr;

	mMaterialNum = 0;
	mMakeDl      = false;
	unk39        = true;
	mLightId     = 0xffff;
	unk40        = true;
	unk44        = 1;

	mAnmData      = anm_data;
	mAnmByType    = new MActorAnmBase*[6];
	mAnmByType[0] = nullptr;
	mAnmByType[1] = nullptr;
	mAnmByType[2] = nullptr;
	mAnmByType[3] = nullptr;
	mAnmByType[4] = nullptr;
	mAnmByType[5] = nullptr;

	if (anm_data->getUnk2C()) {
		mAnmBck = new MActorAnmBck;
		mAnmBck->setUnk1C(anm_data->getUnk2C());
		mAnmByType[ANM_TYPE_BCK] = mAnmBck;
	}

	if (anm_data->getUnk30()) {
		mAnmBpk = new MActorAnmBpk;
		mAnmBpk->setUnk1C(anm_data->getUnk30());
		mAnmBpk->setMatColorAnmKeyPtr();
		mAnmByType[ANM_TYPE_BPK] = mAnmBpk;
	}

	if (anm_data->getUnk34()) {
		mAnmBtp = new MActorAnmBtp;
		mAnmBtp->setUnk1C(anm_data->getUnk34());
		mAnmBtp->setTexNoAnmFullPtr();
		mAnmByType[ANM_TYPE_BTP] = mAnmBtp;
	}

	if (anm_data->getUnk38()) {
		mAnmBtk = new MActorAnmBtk;
		mAnmBtk->setUnk1C(anm_data->getUnk38());
		mAnmBtk->setTexMtxAnmKeyPtr();
		mAnmByType[ANM_TYPE_BTK] = mAnmBtk;
	}

	if (anm_data->getUnk3C()) {
		mAnmBrk = new MActorAnmBrk;
		mAnmBrk->setUnk1C(anm_data->getUnk3C());
		mAnmBrk->setTevColorAnmKeyPtr();
		mAnmBrk->setTevKColorAnmKeyPtr();
		mAnmByType[ANM_TYPE_BRK] = mAnmBrk;
	}

	if (anm_data->getUnk40()) {
		mAnmBlk = new MActorAnmBlk;
		mAnmBlk->setUnk1C(anm_data->getUnk40());
		mAnmByType[ANM_TYPE_BLK] = mAnmBlk;
	}

	if (anm_data->getUnk0() > 0) {
		unk10 = new MActorAnmBck*[anm_data->getUnk0()];

		JGadget::TList<MActorSubAnmInfo>::iterator it = mAnmData->unk1C.begin();
		JGadget::TList<MActorSubAnmInfo>::iterator e  = mAnmData->unk1C.end();

		for (int i = 0; it != e; ++it, ++i) {
			unk10[i] = new MActorAnmBck;
			unk10[i]->setUnk1C(anm_data->getUnk2C());
			unk10[i]->unk28 = it->unk0;
		}
	}
}

void MActor::setMActorAnmData(MActorAnmData* anm_data) { mAnmData = anm_data; }

void MActor::setModel(J3DModel* param_1, u32 param_2)
{
	mModel       = param_1;
	mMaterialNum = param_1->getModelData()->getMaterialNum();
	unk2C        = new u16[mMaterialNum];
	unk30        = new u16[mMaterialNum];
	if ((~param_2 & 0x10000) != 0)
		onMakeDL();

	unk8 = mModel->getModelData()->getJointNodePointer(0)->getMtxCalc();
	for (u16 i = 0; i < mMaterialNum; ++i) {
		J3DMaterial* mat = mModel->getModelData()->getMaterialNodePointer(i);
		unk30[i]         = 0x32;
		unk2C[i]         = 0x32;
		for (u8 j = 0; j < mat->getTexGenBlock()->getTexGenNum(); ++j) {
			if (mat->getTexGenBlock()->getTexMtx(j)
			    && mat->getTexGenBlock()->getTexMtx(j)->getInfo()) {
				unk30[i] = i;
			}
		}
	}

	for (int i = 0; i < 6; ++i) {
		if (mAnmByType[i]) {
			mAnmByType[i]->setModel(mModel);
			mAnmByType[i]->checkUseMaterialIDInit(unk2C);
		}
	}

	if (mAnmData->getUnk0() > 0) {
		JGadget::TList<MActorSubAnmInfo>::iterator it = mAnmData->unk1C.begin();
		JGadget::TList<MActorSubAnmInfo>::iterator e  = mAnmData->unk1C.end();
		for (int i = 0; it != e; ++it, ++i) {
			unk10[i]->setModel(mModel);
		}
	}

	for (u16 i = 0; i < mMaterialNum; ++i) {
		J3DMaterial* mat    = mModel->getModelData()->getMaterialNodePointer(i);
		J3DMaterialAnm* anm = mat->getMaterialAnm();

		if (anm == nullptr && (unk2C[i] != 0x32 || unk30[i] != 0x32)) {
			anm = new J3DMaterialAnm;
			mat->change();
			mat->setMaterialAnm(anm);
		}
		unk2C[i] = 0x32;
	}

	initDL();

	if (!mAnmData->getUnk48())
		mAnmData->createSampleModelData(mModel->getModelData());
}

bool MActor::isCurAnmAlreadyEnd(int type)
{
	bool result = true;

	J3DFrameCtrl* ctrl = getFrameCtrl(type);
	if (ctrl) {
		result = ctrl->checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE)
		         || ctrl->checkState(J3DFrameCtrl::STATE_LOOPED_ONCE)
		         || ctrl->getFrame() + 0.1f >= ctrl->getEnd();
	}

	return result;
}

BOOL MActor::curAnmEndsNext(int type, char* part_name)
{
	if (!mAnmByType[type])
		return true;

	if (type == 0) {
		if (!part_name)
			return mAnmByType[type]->endsNext();

		if (!unk10)
			return true;

		int idx = mAnmData->partsNameToIdx(part_name);
		return unk10[idx]->endsNext();
	}

	return mAnmByType[type]->endsNext();
}

BOOL MActor::curSubAnmEndsNext(int idx)
{
	if (!unk10)
		return true;

	return unk10[idx]->endsNext();
}

void MActor::setAnimation(const char* name, int type)
{
	if (!mAnmByType[type])
		return;

	mAnmByType[type]->setAnm(name, unk2C);

	if (mMakeDl && type != 0)
		resetDL();
}

void MActor::initDL()
{
	if (!mMakeDl)
		return;

	j3dSys.setModel(mModel);
	j3dSys.setTexture(mModel->getModelData()->getTexture());
	for (u16 i = 0; i < mMaterialNum; ++i) {
		mModel->getMatPacket(i)->unlock();

		if (unk30[i] == 0x32 && unk2C[i] == 0x32) {
			mModel->getModelData()->getMaterialNodePointer(i)->calc(
			    (MtxPtr)&j3dDefaultMtx);
			mModel->getModelData()->getMaterialNodePointer(i)->setCurrentMtx();
			j3dSys.setMatPacket(mModel->getMatPacket(i));
			mModel->getModelData()
			    ->getMaterialNodePointer(i)
			    ->makeDisplayList();
			mModel->getMatPacket(i)->lock();
		}
	}
}

void MActor::resetDL()
{
	if (!mMakeDl)
		return;

	j3dSys.setModel(mModel);
	j3dSys.setTexture(mModel->getModelData()->getTexture());
	for (u16 i = 0; i < mMaterialNum; ++i) {
		if (unk30[i] != 0x32 || unk2C[i] != 0x32) {
			mModel->getMatPacket(i)->unlock();
		} else {
			if (!mModel->getMatPacket(i)->isLocked()) {
				mModel->getModelData()->getMaterialNodePointer(i)->calc(
				    (MtxPtr)&j3dDefaultMtx);
				j3dSys.setMatPacket(mModel->getMatPacket(i));
				mModel->getModelData()
				    ->getMaterialNodePointer(i)
				    ->makeDisplayList();
				mModel->getMatPacket(i)->lock();
			}
		}
	}
}

void MActor::initDLByIndex(u16) { }

void MActor::unlockDLIfNeed()
{
	if (!mMakeDl)
		return;

	for (u16 i = 0; i < mMaterialNum; ++i)
		if (unk30[i] != 0x32 || unk2C[i] != 0x32)
			mModel->getMatPacket(i)->unlock();
}

void MActor::onMakeDL() { mMakeDl = true; }

void MActor::offMakeDL()
{
	mMakeDl = false;
	for (u16 i = 0; i < mMaterialNum; ++i)
		mModel->getMatPacket(i)->unlock();
}

const char* MActor::getCurAnmName(int) const { }

void MActor::setJointCallback(int param_1, J3DNodeCallBack param_2)
{
	mModel->getModelData()->getJointNodePointer(param_1)->setCallBack(param_2);
}

void MActor::updateInSubBck()
{
	if (!unk10)
		return;

	for (int i = 0; i < mAnmData->getUnk0(); ++i)
		if (unk10[i]->getCurIdx() >= 0)
			unk10[i]->updateIn();
}

void MActor::updateOutSubBck()
{
	if (!unk10)
		return;

	for (int i = 0; i < mAnmData->getUnk0(); ++i)
		if (unk10[i]->getCurIdx() >= 0)
			unk10[i]->updateOut();
}

void MActor::calcAnm()
{
	frameUpdate();

	updateIn();
	mModel->calc();
	updateOut();
}

void MActor::calc()
{
	if (!unk39)
		return;

	updateIn();
	mModel->calc();
	updateOut();
}

void MActor::viewCalc()
{
	if (unk39)
		mModel->viewCalc();
}

void MActor::loadSetDeformData(const char*) { }

void MActor::setLightID(s16 light_id)
{
	mLightId = 0;
	mLightId = light_id;
}

void MActor::setLightData(const TBGCheckData* param_1,
                          const JGeometry::TVec3<f32>& param_2)
{
	if (!unk40)
		return;

	if (gpCubeShadow != nullptr && gpCubeShadow->getInCubeNo(param_2) != -1) {
		mLightId = 1;
		return;
	}

	if (param_1 == nullptr)
		return;

	mLightId = 0;
	if (param_1->isShadow()) {
		setLightID(param_1->getData());
	}
}

void MActor::setLightType(int light_type)
{
	unk44 = light_type;
	gpLightManager->getLightSet(light_type)->enable();
}

void MActor::update() { }

void MActor::entry()
{
	if (!unk39)
		return;

	bool shouldResetLightDrawBuf = false;
	if (mLightId != 0xffff) {
		if (mLightId < 0)
			mLightId = 0;

		gpLightManager->getLightSet(unk44)->changeLightDrawBuffer(mLightId);

		shouldResetLightDrawBuf = true;
	}

	entryIn();
	mModel->entry();
	entryOut();

	if (shouldResetLightDrawBuf)
		gpLightManager->getLightSet(unk44)->resetLightDrawBuffer();
}

void MActor::frameUpdate()
{
	for (int i = ANM_TYPE_FIRST; i < ANM_TYPE_COUNT; ++i)
		if (mAnmByType[i] && mAnmByType[i]->getCurIdx() >= 0)
			mAnmByType[i]->getFrameCtrl()->update();

	if (unk10)
		for (int i = 0; i < mAnmData->getUnk0(); ++i)
			if (unk10[i]->getCurIdx() >= 0)
				unk10[i]->getFrameCtrl()->update();
}

void MActor::matAnmFrameUpdate()
{
	for (int i = ANM_TYPE_BPK; i < ANM_TYPE_COUNT; ++i)
		if (mAnmByType[i] && mAnmByType[i]->getCurIdx() >= 0)
			mAnmByType[i]->getFrameCtrl()->update();
}

void MActor::perform(u32 cue, JDrama::TGraphics*)
{
	if (cue & CUE_CALC_ANIM)
		calcAnm();

	if (cue & CUE_CALC_VIEW)
		viewCalc();

	if (cue & CUE_ENTRY)
		entry();
}

BOOL MActor::checkCurAnm(const char* name, int type)
{
	if (!mAnmByType[type])
		return false;

	if (mAnmByType[type]->findName2(name) == mAnmByType[type]->getCurIdx())
		return true;

	return false;
}

bool MActor::checkCurAnmFromIndex(int index, int type)
{
	if (!mAnmByType[type])
		return false;

	if (index == mAnmByType[type]->getCurIdx())
		return true;

	return false;
}

bool MActor::checkAnmFileExist(const char* name, int type)
{
	if (mAnmByType[type] && mAnmByType[type]->findName2(name) >= 0)
		return true;

	return false;
}

J3DFrameCtrl* MActor::getFrameCtrl(int type)
{
	if (!mAnmByType[type])
		return nullptr;
	return mAnmByType[type]->getFrameCtrl();
}

BOOL MActor::checkBckPass(f32 pass_frame)
{
	if (!mAnmByType[0])
		return false;

	J3DFrameCtrl* ctrl = mAnmByType[0]->getFrameCtrl();

	if (!ctrl)
		return false;

	return ctrl->checkPass(pass_frame);
}

int MActor::getCurAnmIdx(int type) const
{
	if (!mAnmByType[type])
		return -1;
	return mAnmByType[type]->unk0;
}

void MActor::setFrameRate(f32 rate, int type)
{
	if (!mAnmByType[type])
		return;

	mAnmByType[type]->unk4.setRate(rate);
}

void MActor::setBck(const char* name)
{
	if (!mAnmBck)
		return;
	mAnmBck->setAnm(name, 0);
}

void MActor::setBckFromIndex(int index)
{
	if (!mAnmBck)
		return;
	mAnmBck->setAnmFromIndex(index, 0);
}

void MActor::setSubBckFromIndex(int index, int part_idx)
{
	if (!unk10)
		return;
	unk10[part_idx]->setAnmFromIndex(index, 0);
}

BOOL MActor::checkCurBckFromIndex(int index)
{
	if (!mAnmBck)
		return false;
	if (index == mAnmBck->unk0)
		return true;
	return false;
}

void MActor::setSubBck(const char* part_name, const char* anm_name)
{
	if (!mAnmBck)
		return;

	if (!unk10)
		return;

	int idx = mAnmData->partsNameToIdx(part_name);
	unk10[idx]->setAnm(anm_name, 0);
}

void MActor::setBpk(const char* name)
{
	if (!mAnmBpk)
		return;
	mAnmBpk->setAnm(name, unk2C);
	resetDL();
}

void MActor::setBpkFromIndex(int index)
{
	if (!mAnmBpk)
		return;
	mAnmBpk->setAnmFromIndex(index, unk2C);
	resetDL();
}

void MActor::setBtp(const char* name)
{
	if (!mAnmBtp)
		return;
	mAnmBtp->setAnm(name, unk2C);
	resetDL();
}

void MActor::setBtpFromIndex(int index)
{
	if (mAnmBtp == nullptr && index >= 0)
		return;
	if (index < 0)
		return;
	mAnmBtp->setAnmFromIndex(index, unk2C);
	resetDL();
}

void MActor::setBtk(const char* name)
{
	if (!mAnmBtk)
		return;
	mAnmBtk->setAnm(name, unk2C);
	resetDL();
}

void MActor::setBtkFromIndex(int index)
{
	if (!mAnmBtk)
		return;
	mAnmBtk->setAnmFromIndex(index, unk2C);
	resetDL();
}

void MActor::setBlk(const char* name)
{
	if (!mAnmBlk)
		return;
	mAnmBlk->setAnm(name, unk2C);
	resetDL();
}

void MActor::setBlkFromIndex(int index)
{
	if (!mAnmBlk)
		return;
	mAnmBlk->setAnmFromIndex(index, unk2C);
	resetDL();
}

void MActor::setBrk(const char* name)
{
	if (!mAnmBrk)
		return;
	mAnmBrk->setAnm(name, unk2C);
	resetDL();
}

void MActor::setBrkFromIndex(int index)
{
	if (!mAnmBrk)
		return;
	mAnmBrk->setAnmFromIndex(index, unk2C);
	resetDL();
}

void MActor::updateIn()
{
	if (mAnmByType[ANM_TYPE_BCK] && mAnmByType[ANM_TYPE_BCK]->getCurIdx() >= 0)
		mAnmByType[ANM_TYPE_BCK]->updateIn();

	updateInSubBck();

	if (mAnmByType[ANM_TYPE_BLK] && mAnmByType[ANM_TYPE_BLK]->getCurIdx() >= 0)
		mAnmByType[ANM_TYPE_BLK]->updateIn();
}

void MActor::updateOut()
{
	if (mAnmByType[ANM_TYPE_BCK] && mAnmByType[ANM_TYPE_BCK]->getCurIdx() >= 0)
		mAnmByType[ANM_TYPE_BCK]->updateOut();

	updateOutSubBck();

	if (mAnmByType[ANM_TYPE_BLK] && mAnmByType[ANM_TYPE_BLK]->getCurIdx() >= 0)
		mAnmByType[ANM_TYPE_BLK]->updateOut();
}

void MActor::entryIn()
{
	for (int i = ANM_TYPE_BPK; i < ANM_TYPE_COUNT; ++i)
		if (mAnmByType[i] && mAnmByType[i]->getCurIdx() >= 0)
			mAnmByType[i]->updateIn();
}

void MActor::entryOut()
{
	for (int i = ANM_TYPE_BPK; i < ANM_TYPE_COUNT; ++i)
		if (mAnmByType[i] && mAnmByType[i]->getCurIdx() >= 0)
			mAnmByType[i]->updateOut();
}

void MActor::updateMatAnm()
{
	j3dSys.setTexture(mModel->getModelData()->getTexture());
	for (u16 i = 0; i < mMaterialNum; ++i)
		if (unk30[i] != 0x32 || unk2C[i] != 0x32)
			SMS_CalcMatAnmAndMakeDL(mModel, i);
}

void MActor::dumpReport() { }
