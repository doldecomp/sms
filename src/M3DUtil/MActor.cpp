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
	mSubBcks   = nullptr;
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

	mAnmData   = anm_data;
	mAnmByType = new MActorAnmBase*[6];
	for (int i = 0; i < 6; ++i)
		mAnmByType[i] = nullptr;

	if (anm_data->getBckAnmData()) {
		mAnmBck = new MActorAnmBck;
		mAnmBck->setAnmData(anm_data->getBckAnmData());
		mAnmByType[ANM_TYPE_BCK] = mAnmBck;
	}

	if (anm_data->getBpkAnmData()) {
		mAnmBpk = new MActorAnmBpk;
		mAnmBpk->setAnmData(anm_data->getBpkAnmData());
		mAnmBpk->setMatColorAnmKeyPtr();
		mAnmByType[ANM_TYPE_BPK] = mAnmBpk;
	}

	if (anm_data->getBtpAnmData()) {
		mAnmBtp = new MActorAnmBtp;
		mAnmBtp->setAnmData(anm_data->getBtpAnmData());
		mAnmBtp->setTexNoAnmFullPtr();
		mAnmByType[ANM_TYPE_BTP] = mAnmBtp;
	}

	if (anm_data->getBtkAnmData()) {
		mAnmBtk = new MActorAnmBtk;
		mAnmBtk->setAnmData(anm_data->getBtkAnmData());
		mAnmBtk->setTexMtxAnmKeyPtr();
		mAnmByType[ANM_TYPE_BTK] = mAnmBtk;
	}

	if (anm_data->getBrkAnmData()) {
		mAnmBrk = new MActorAnmBrk;
		mAnmBrk->setAnmData(anm_data->getBrkAnmData());
		mAnmBrk->setTevColorAnmKeyPtr();
		mAnmBrk->setTevKColorAnmKeyPtr();
		mAnmByType[ANM_TYPE_BRK] = mAnmBrk;
	}

	if (anm_data->getBlkAnmData()) {
		mAnmBlk = new MActorAnmBlk;
		mAnmBlk->setAnmData(anm_data->getBlkAnmData());
		mAnmByType[ANM_TYPE_BLK] = mAnmBlk;
	}

	if (anm_data->getIncidentalAnmNum() > 0) {
		mSubBcks = new MActorAnmBck*[anm_data->getIncidentalAnmNum()];

		JGadget::TList<MActorSubAnmInfo>::iterator it
		    = mAnmData->mIncidentalAnms.begin();
		JGadget::TList<MActorSubAnmInfo>::iterator e
		    = mAnmData->mIncidentalAnms.end();

		for (int i = 0; it != e; ++it, ++i) {
			mSubBcks[i] = new MActorAnmBck;
			mSubBcks[i]->setAnmData(anm_data->getBckAnmData());
			mSubBcks[i]->setJointIndex(it->mJointIdx);
		}
	}
}

void MActor::setMActorAnmData(MActorAnmData* anm_data) { mAnmData = anm_data; }

void MActor::setModel(J3DModel* model, u32 param_2)
{
	mModel       = model;
	mMaterialNum = model->getModelData()->getMaterialNum();
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

	if (mAnmData->getIncidentalAnmNum() > 0) {
		JGadget::TList<MActorSubAnmInfo>::iterator it
		    = mAnmData->mIncidentalAnms.begin();
		JGadget::TList<MActorSubAnmInfo>::iterator e
		    = mAnmData->mIncidentalAnms.end();
		for (int i = 0; it != e; ++it, ++i)
			mSubBcks[i]->setModel(mModel);
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

	if (!mAnmData->getSampleModelData())
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

		if (!mSubBcks)
			return true;

		int idx = mAnmData->partsNameToIdx(part_name);
		return mSubBcks[idx]->endsNext();
	}

	return mAnmByType[type]->endsNext();
}

BOOL MActor::curSubAnmEndsNext(int idx)
{
	if (!mSubBcks)
		return true;

	return mSubBcks[idx]->endsNext();
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

void MActor::setJointCallback(int joint_idx, J3DNodeCallBack callback)
{
	mModel->getModelData()->getJointNodePointer(joint_idx)->setCallBack(
	    callback);
}

void MActor::updateInSubBck()
{
	if (!mSubBcks)
		return;

	for (int i = 0; i < mAnmData->getIncidentalAnmNum(); ++i)
		if (mSubBcks[i]->getCurAnmIdx() >= 0)
			mSubBcks[i]->updateIn();
}

void MActor::updateOutSubBck()
{
	if (!mSubBcks)
		return;

	for (int i = 0; i < mAnmData->getIncidentalAnmNum(); ++i)
		if (mSubBcks[i]->getCurAnmIdx() >= 0)
			mSubBcks[i]->updateOut();
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
#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
	if (param_1->isShadow()) {
		f32 tmp = param_2.x * param_1->mNormal.x
		          + param_2.z * param_1->mNormal.z + param_1->mPlaneDistance;
		f32 groundY = -tmp / param_1->mNormal.y;
		if (groundY + 200.0f > param_2.y)
			setLightID(param_1->getData());
	}
#else
	if (param_1->isShadow()) {
		setLightID(param_1->getData());
	}
#endif
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
		if (mAnmByType[i] && mAnmByType[i]->getCurAnmIdx() >= 0)
			mAnmByType[i]->getFrameCtrl()->update();

	if (mSubBcks)
		for (int i = 0; i < mAnmData->getIncidentalAnmNum(); ++i)
			if (mSubBcks[i]->getCurAnmIdx() >= 0)
				mSubBcks[i]->getFrameCtrl()->update();
}

void MActor::matAnmFrameUpdate()
{
	for (int i = ANM_TYPE_BPK; i < ANM_TYPE_COUNT; ++i)
		if (mAnmByType[i] && mAnmByType[i]->getCurAnmIdx() >= 0)
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

	if (mAnmByType[type]->findName2(name) == mAnmByType[type]->getCurAnmIdx())
		return true;

	return false;
}

bool MActor::checkCurAnmFromIndex(int index, int type)
{
	if (!mAnmByType[type])
		return false;

	if (index == mAnmByType[type]->getCurAnmIdx())
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
	return mAnmByType[type]->getCurAnmIdx();
}

void MActor::setFrameRate(f32 rate, int type)
{
	if (!mAnmByType[type])
		return;

	mAnmByType[type]->getFrameCtrl()->setRate(rate);
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
	if (!mSubBcks)
		return;
	mSubBcks[part_idx]->setAnmFromIndex(index, 0);
}

BOOL MActor::checkCurBckFromIndex(int index)
{
	if (!mAnmBck)
		return false;
	if (index == mAnmBck->getCurAnmIdx())
		return true;
	return false;
}

void MActor::setSubBck(const char* part_name, const char* anm_name)
{
	if (!mAnmBck)
		return;

	if (!mSubBcks)
		return;

	int idx = mAnmData->partsNameToIdx(part_name);
	mSubBcks[idx]->setAnm(anm_name, 0);
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
	if (mAnmByType[ANM_TYPE_BCK]
	    && mAnmByType[ANM_TYPE_BCK]->getCurAnmIdx() >= 0)
		mAnmByType[ANM_TYPE_BCK]->updateIn();

	updateInSubBck();

	if (mAnmByType[ANM_TYPE_BLK]
	    && mAnmByType[ANM_TYPE_BLK]->getCurAnmIdx() >= 0)
		mAnmByType[ANM_TYPE_BLK]->updateIn();
}

void MActor::updateOut()
{
	if (mAnmByType[ANM_TYPE_BCK]
	    && mAnmByType[ANM_TYPE_BCK]->getCurAnmIdx() >= 0)
		mAnmByType[ANM_TYPE_BCK]->updateOut();

	updateOutSubBck();

	if (mAnmByType[ANM_TYPE_BLK]
	    && mAnmByType[ANM_TYPE_BLK]->getCurAnmIdx() >= 0)
		mAnmByType[ANM_TYPE_BLK]->updateOut();
}

void MActor::entryIn()
{
	for (int i = ANM_TYPE_BPK; i < ANM_TYPE_COUNT; ++i)
		if (mAnmByType[i] && mAnmByType[i]->getCurAnmIdx() >= 0)
			mAnmByType[i]->updateIn();
}

void MActor::entryOut()
{
	for (int i = ANM_TYPE_BPK; i < ANM_TYPE_COUNT; ++i)
		if (mAnmByType[i] && mAnmByType[i]->getCurAnmIdx() >= 0)
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
