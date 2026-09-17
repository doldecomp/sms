#include <JSystem/JParticle/JPADraw.hpp>
#include <JSystem/JParticle/JPABaseShape.hpp>
#include <JSystem/JParticle/JPAExtraShape.hpp>
#include <JSystem/JParticle/JPAExTexShape.hpp>
#include <JSystem/JParticle/JPASweepShape.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/JParticle/JPADrawSetupTev.hpp>
#include <JSystem/JParticle/JPAParticle.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include <dolphin/gx.h>
#include <macros.h>

JPADrawVisitorContainer JPADraw::vc;
JPADrawClipBoard JPADraw::cb;

BOOL JPADraw::initialize(JPABaseEmitter* emitter,
                         JPATextureResource* tex_resource)
{
	int i;

	mDrawCtx.pcb = &cb;

	mDrawCtx.mBaseEmitter = emitter;

	mDrawCtx.mBaseShape
	    = mDrawCtx.mBaseEmitter->getEmitterDataBlockInfoPtr()->getBaseShape();
	mDrawCtx.mExtraShape
	    = mDrawCtx.mBaseEmitter->getEmitterDataBlockInfoPtr()->getExtraShape();
	mDrawCtx.mSweepShape
	    = mDrawCtx.mBaseEmitter->getEmitterDataBlockInfoPtr()->getSweepShape();
	mDrawCtx.mExTexShape
	    = mDrawCtx.mBaseEmitter->getEmitterDataBlockInfoPtr()->getExTexShape();

	mDrawCtx.unk14        = this;
	mDrawCtx.mTexResource = tex_resource;
	mDrawCtx.mTexIndices  = mDrawCtx.mBaseEmitter->getEmitterDataBlockInfoPtr()
	                           ->getTextureDataBase();
	mDrawCtx.unk18 = mDrawCtx.mBaseEmitter->getParticleList();

	unkC2     = 0;
	mScaleOut = 1.0f;

	GXColor white = { 0xff, 0xff, 0xff, 0xff };

	if (mDrawCtx.mBaseShape->isEnablePrm()) {
		if (!mDrawCtx.mBaseShape->isEnablePrmAnm())
			mPrmColor = mDrawCtx.mBaseShape->getPrmColor();
	} else {
		mPrmColor = white;
	}

	if (mDrawCtx.mBaseShape->isEnableEnv()) {
		if (!mDrawCtx.mBaseShape->isEnableEnvAnm())
			mEnvColor = mDrawCtx.mBaseShape->getEnvColor();
	} else {
		mEnvColor = white;
	}

	calcChldVisNum  = 0;
	calcPtclVisNum  = 0;
	execChldVisNum  = 0;
	execPtclVisNum  = 0;
	calcEmtrVisNum  = 0;
	execEmtrCVisNum = 0;
	execEmtrPVisNum = 0;
	execEmtrVisNum  = 0;

	for (i = 0; i < ARRAY_COUNT(mpExecEmtrVis); ++i)
		mpExecEmtrVis[i] = nullptr;

	for (i = 0; i < ARRAY_COUNT(mpExecEmtrPVis); ++i)
		mpExecEmtrPVis[i] = nullptr;

	for (i = 0; i < ARRAY_COUNT(mpExecEmtrCVis); ++i)
		mpExecEmtrCVis[i] = nullptr;

	for (i = 0; i < ARRAY_COUNT(mpCalcEmtrVis); ++i)
		mpCalcEmtrVis[i] = nullptr;

	for (i = 0; i < ARRAY_COUNT(mpExecPtclVis); ++i)
		mpExecPtclVis[i] = nullptr;

	for (i = 0; i < ARRAY_COUNT(mpExecChldVis); ++i)
		mpExecChldVis[i] = nullptr;

	for (i = 0; i < ARRAY_COUNT(mpCalcPtclVis); ++i)
		mpCalcPtclVis[i] = nullptr;

	for (i = 0; i < ARRAY_COUNT(mpCalcChldVis); ++i)
		mpCalcChldVis[i] = nullptr;

	JPADrawVisitorDefFlags flags;

	flags.mbIsEnableDrawParent = mDrawCtx.mSweepShape == nullptr
	                             || mDrawCtx.mSweepShape->isEnableDrawParent();
	flags.mbHasPrmAnm = mDrawCtx.mBaseShape->isEnablePrm()
	                    && mDrawCtx.mBaseShape->isEnablePrmAnm();
	flags.mbHasEnvAnm = mDrawCtx.mBaseShape->isEnableEnv()
	                    && mDrawCtx.mBaseShape->isEnableEnvAnm();
	flags.mbIsStripe = mDrawCtx.mBaseShape->getType() == 5
	                   || mDrawCtx.mBaseShape->getType() == 6;
	flags.mbIsPointOrLine = mDrawCtx.mBaseShape->getType() == 0
	                        || mDrawCtx.mBaseShape->getType() == 1;
	flags.mbIsEnableAlpha = mDrawCtx.mExtraShape != nullptr
	                        && mDrawCtx.mExtraShape->isEnableAlpha();

	setDrawExecVisitorsBeforeCB(flags);
	mpExecEmtrPVis[execEmtrPVisNum++] = &vc.mDrawExecCallBack;
	mpExecEmtrCVis[execEmtrCVisNum++] = &vc.mDrawExecCallBack;
	mpExecPtclVis[execPtclVisNum++]   = &vc.mDrawExecCallBack;
	mpExecChldVis[execChldVisNum++]   = &vc.mDrawExecCallBack;
	setDrawExecVisitorsAfterCB(flags);
	setDrawCalcVisitors(flags);

	JUT_ASSERT(execEmtrVisNum <= 1);
	JUT_ASSERT(execEmtrPVisNum <= 5);
	JUT_ASSERT(execEmtrCVisNum <= 3);
	JUT_ASSERT(calcEmtrVisNum <= 4);
	JUT_ASSERT(execPtclVisNum <= 5);
	JUT_ASSERT(execChldVisNum <= 4);
	JUT_ASSERT(calcPtclVisNum <= 10);
	JUT_ASSERT(calcChldVisNum <= 2);

	return true;
}

void JPADraw::draw(MtxPtr view_mtx)
{
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, 0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetNumChans(0);
	GXSetTevDirect(GX_TEVSTAGE0);
	GXSetTevDirect(GX_TEVSTAGE1);
	GXSetTevDirect(GX_TEVSTAGE2);
	GXSetCoPlanar(0);
	GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);

	GXSetBlendMode(mDrawCtx.mBaseShape->getBlendMode1(),
	               mDrawCtx.mBaseShape->getSrcBlendFactor1(),
	               mDrawCtx.mBaseShape->getDstBlendFactor1(),
	               mDrawCtx.mBaseShape->getBlendOp1());

	mDrawCtx.mBaseEmitter->getBasePrmColor(cb.mPrmColor);
	mDrawCtx.mBaseEmitter->getBaseEnvColor(cb.mEnvColor);

	cb.mViewMtx = view_mtx;

	cb.mSetupTev.setupTev(mDrawCtx.mBaseShape, mDrawCtx.mExTexShape);

	for (int i = 0; i < execEmtrVisNum; ++i)
		mpExecEmtrVis[i]->exec(&mDrawCtx);

	if (mDrawCtx.mBaseShape->isEnableAnmTone())
		zDraw();

	unkC2 &= ~0x1;

	if (mDrawCtx.mBaseShape->getChildOrder() == 1
	    && mDrawCtx.mSweepShape != nullptr)
		drawChild();

	drawParticle();

	if (mDrawCtx.mBaseShape->getChildOrder() == 0
	    && mDrawCtx.mSweepShape != nullptr)
		drawChild();

	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
}

void JPADraw::calc()
{
	for (int i = 0; i < calcEmtrVisNum; ++i)
		mpCalcEmtrVis[i]->calc(&mDrawCtx);
}

void JPADraw::calcParticle(JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();
	params->unk34 += params->unk36;
	for (int i = 0; i < calcPtclVisNum; ++i)
		mpCalcPtclVis[i]->calc(&mDrawCtx, particle);
}

void JPADraw::calcChild(JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamCPtr();
	params->unk34 += params->unk36;
	for (int i = 0; i < calcChldVisNum; ++i)
		mpCalcChldVis[i]->calc(&mDrawCtx, particle);
}

void JPADraw::initParticle(JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	params->unk0.set(JPAGetEmitterInfoPtr()->mEmitterAxisY);

	params->mPrmColor = mPrmColor;
	params->mEnvColor = mEnvColor;
	params->mAlpha    = 1.0f;

	params->unk28 = (s32)(mDrawCtx.mBaseEmitter->getRandomF()
	                      * mDrawCtx.mBaseShape->getLoopOffset());

	if (mDrawCtx.mExtraShape != nullptr) {
		if (mDrawCtx.mExtraShape->isEnableRotate()) {
			params->unk34 = mDrawCtx.mExtraShape->getRotateAngle() * 32768.0f
			                + mDrawCtx.mBaseEmitter->getRandomSF()
			                      * mDrawCtx.mExtraShape->getRotateRandomAngle()
			                      * 65536.0f;

			s16 sVar4;
			if (mDrawCtx.mBaseEmitter->getRandomRF()
			    < mDrawCtx.mExtraShape->getRotateDirection()) {
				sVar4 = mDrawCtx.mExtraShape->getRotateSpeed()
				        * (mDrawCtx.mExtraShape->getRotateRandomSpeed()
				               * mDrawCtx.mBaseEmitter->getRandomRF()
				           + 1.0f)
				        * 32768.0f;
			} else {
				sVar4 = -mDrawCtx.mExtraShape->getRotateSpeed()
				        * (mDrawCtx.mExtraShape->getRotateRandomSpeed()
				               * mDrawCtx.mBaseEmitter->getRandomRF()
				           + 1.0f)
				        * 32768.0f;
			}
			params->unk36 = sVar4;
		} else {
			params->unk34 = 0;
			params->unk36 = 0;
		}

		params->mScaleX = params->mScaleY = params->unkC
		    = mScaleOut
		      * (mDrawCtx.mBaseEmitter->getRandomRF()
		             * mDrawCtx.mExtraShape->getRandomScale()
		         + 1.0f);

		params->unk24 = mDrawCtx.mBaseEmitter->getRandomRF()
		                    * mDrawCtx.mExtraShape->getAlphaWaveRandom()
		                + 1.0f;
	} else {
		params->unk34 = 0;
		params->unk36 = 0;
		params->unkC = params->mScaleX = params->mScaleY = mScaleOut;

		params->unk24 = 1.0f;
	}
}

void JPADraw::initChild(JPABaseParticle* parent, JPABaseParticle* child)
{
	JPADrawParams* parentPrms = parent->getDrawParamPPtr();
	JPADrawParams* childPrms  = child->getDrawParamCPtr();

	childPrms->unk0 = parentPrms->unk0;

	childPrms->mAlpha = 1.0f;
	f32 inheritFactor;

	if (mDrawCtx.mSweepShape->isInheritedRGB()) {
		inheritFactor = mDrawCtx.mSweepShape->getInheritRGB();

		childPrms->mPrmColor.r = parentPrms->mPrmColor.r * inheritFactor;
		childPrms->mPrmColor.g = parentPrms->mPrmColor.g * inheritFactor;
		childPrms->mPrmColor.b = parentPrms->mPrmColor.b * inheritFactor;

		childPrms->mEnvColor.r = parentPrms->mEnvColor.r * inheritFactor;
		childPrms->mEnvColor.g = parentPrms->mEnvColor.g * inheritFactor;
		childPrms->mEnvColor.b = parentPrms->mEnvColor.b * inheritFactor;
	} else {
		childPrms->mPrmColor = mDrawCtx.mSweepShape->getPrm();
		childPrms->mEnvColor = mDrawCtx.mSweepShape->getEnv();
	}

	if (mDrawCtx.mSweepShape->isInheritedAlpha()) {
		inheritFactor
		    = parentPrms->mAlpha * mDrawCtx.mSweepShape->getInheritAlpha();

		childPrms->mPrmColor.a = parentPrms->mPrmColor.a * inheritFactor;
		childPrms->mEnvColor.a = parentPrms->mEnvColor.a * inheritFactor;
	} else {
		childPrms->mPrmColor.a = mDrawCtx.mSweepShape->getPrmAlpha();
		childPrms->mEnvColor.a = mDrawCtx.mSweepShape->getEnvAlpha();
	}

	if (mDrawCtx.mSweepShape->isInheritedScale()) {
		inheritFactor = mDrawCtx.mSweepShape->getInheritScale();

		childPrms->mScaleX = childPrms->unkC
		    = inheritFactor * parentPrms->mScaleX;
		childPrms->mScaleY = childPrms->unk24
		    = inheritFactor * parentPrms->mScaleY;
	} else {
		childPrms->unk24   = 1.0f;
		childPrms->mScaleY = 1.0f;
		childPrms->unkC    = 1.0f;
		childPrms->mScaleX = 1.0f;
	}

	childPrms->unk34 = parentPrms->unk34;
	if (mDrawCtx.mSweepShape->isEnableRotate()) {
		childPrms->unk36
		    = (s16)(mDrawCtx.mSweepShape->getRotateSpeed() * 32768.0f);
	} else {
		childPrms->unk36 = 0;
	}
}

const ResTIMG* JPADraw::swapImage(const ResTIMG* param_1, s16 param_2)
{
	if (param_2 < 0)
		return nullptr;

	u8 id   = param_2;
	u32 idx = mDrawCtx.mTexIndices[id];
	return mDrawCtx.mTexResource->swapImage(param_1, idx);
}

BOOL JPADraw::loadTexture(u8 idx, GXTexMapID map_id)
{
	JUT_ASSERT(
	    mDrawCtx.mBaseEmitter->getEmitterDataBlockInfoPtr()->getTextureNum()
	    > idx);
	mDrawCtx.mTexResource->load(mDrawCtx.mTexIndices[idx], map_id);
	return true;
}

void JPADraw::setDrawExecVisitorsBeforeCB(
    const JPADraw::JPADrawVisitorDefFlags& flags)
{
	if (mDrawCtx.mExTexShape)
		mpExecEmtrVis[execEmtrVisNum++] = &vc.mDrawExecLoadExTex;

	if (flags.mbIsPointOrLine) {
		mpExecEmtrPVis[execEmtrPVisNum++] = &vc.mDrawExecGenIdtMtx;
	} else if (mDrawCtx.mBaseShape->isEnableProjection()) {
		if (mDrawCtx.mBaseShape->isEnableTexScrollAnm())
			mpExecEmtrPVis[execEmtrPVisNum++] = &vc.mDrawExecGenPrjTexMtx;
		else
			mpExecEmtrPVis[execEmtrPVisNum++] = &vc.mDrawExecGenPrjMtx;
	} else if (mDrawCtx.mBaseShape->isEnableTexScrollAnm()) {
		if (flags.mbIsStripe)
			mpExecEmtrPVis[execEmtrPVisNum++] = &vc.mDrawExecSetTexMtx;
		else
			mpExecEmtrPVis[execEmtrPVisNum++] = &vc.mDrawExecGenTexMtx0;
	} else {
		mpExecEmtrPVis[execEmtrPVisNum++] = &vc.mDrawExecGenIdtMtx;
	}

	if (mDrawCtx.mBaseShape->isEnableGlobalColAnm()) {
		if (flags.mbIsStripe || !flags.mbIsEnableAlpha) {
			mpExecEmtrPVis[execEmtrPVisNum++]
			    = &vc.mDrawExecRegisterColorEmitterPE;
		} else if (flags.mbIsEnableAlpha) {
			mpExecEmtrPVis[execEmtrPVisNum++]
			    = &vc.mDrawExecRegisterColorEmitterE;
		}
	} else {
		if ((!mDrawCtx.mBaseShape->isEnablePrm()
		     || !mDrawCtx.mBaseShape->isEnablePrmAnm())
		    && flags.mbIsEnableAlpha == 0) {
			if (!mDrawCtx.mBaseShape->isEnableEnv()
			    || !mDrawCtx.mBaseShape->isEnableEnvAnm()) {
				mpExecEmtrPVis[execEmtrPVisNum++]
				    = &vc.mDrawExecRegisterColorEmitterPE;
			} else {
				mpExecEmtrPVis[execEmtrPVisNum++]
				    = &vc.mDrawExecRegisterColorEmitterP;
			}
		} else if (!mDrawCtx.mBaseShape->isEnableEnv()
		           || !mDrawCtx.mBaseShape->isEnableEnvAnm()) {
			mpExecEmtrPVis[execEmtrPVisNum++]
			    = &vc.mDrawExecRegisterColorEmitterE;
		}
	}

	if (!mDrawCtx.mBaseShape->textureIsEmpty()
	    && (!mDrawCtx.mBaseShape->isEnableTextureAnm()
	        || mDrawCtx.mBaseShape->isEnableGlobalTexAnm())) {
		mpExecEmtrPVis[execEmtrPVisNum++] = &vc.mDrawExecLoadTexture;
	} else if (mDrawCtx.mBaseShape->textureIsEmpty()) {
		mpExecEmtrPVis[execEmtrPVisNum++] = &vc.mDrawExecLoadDefaultTexture;
	}

	if (mDrawCtx.mSweepShape != nullptr
	    && !mDrawCtx.mSweepShape->isEnableAlphaOut()
	    && !mDrawCtx.mSweepShape->isInheritedAlpha()
	    && !mDrawCtx.mSweepShape->isInheritedRGB()) {
		mpExecEmtrCVis[execEmtrCVisNum++] = &vc.mDrawExecRegisterColorChildPE;
	}

	if (!mDrawCtx.mBaseShape->isEnableGlobalColAnm()) {
		if (flags.mbHasPrmAnm) {
			if (flags.mbHasEnvAnm)
				mpExecPtclVis[execPtclVisNum++] = &vc.mDrawExecRegisterPrmCEnv;
			else
				mpExecPtclVis[execPtclVisNum++]
				    = &vc.mDrawExecRegisterPrmColorAnm;
		} else {
			if (flags.mbIsEnableAlpha) {
				if (flags.mbHasEnvAnm)
					mpExecPtclVis[execPtclVisNum++]
					    = &vc.mDrawExecRegisterPrmAEnv;
				else
					mpExecPtclVis[execPtclVisNum++]
					    = &vc.mDrawExecRegisterPrmAlphaAnm;
			} else {
				if (flags.mbHasEnvAnm)
					mpExecPtclVis[execPtclVisNum++]
					    = &vc.mDrawExecRegisterEnvColorAnm;
			}
		}
	} else if (flags.mbIsEnableAlpha && !flags.mbIsStripe) {
		mpExecPtclVis[execPtclVisNum++] = &vc.mDrawExecRegisterPrmAlphaAnm;
	}

	if (!mDrawCtx.mBaseShape->textureIsEmpty()
	    && mDrawCtx.mBaseShape->isEnableTextureAnm()
	    && !mDrawCtx.mBaseShape->isEnableGlobalTexAnm()) {
		mpExecPtclVis[execPtclVisNum++] = &vc.mDrawExecLoadTexture;
	}

	switch (mDrawCtx.mBaseShape->getType()) {
	case 0:
		if (mDrawCtx.mExtraShape && mDrawCtx.mExtraShape->isEnableScale())
			mpExecPtclVis[execPtclVisNum++] = &vc.mDrawExecSetPointSize;
		break;
	case 1:
		if (mDrawCtx.mExtraShape && mDrawCtx.mExtraShape->isEnableScale())
			mpExecPtclVis[execPtclVisNum++] = &vc.mDrawExecSetLineWidth;
		break;

	case 2:
	case 3:
	case 4:
	case 7:
	case 8:
	case 9:
	case 10:
		if (mDrawCtx.mBaseShape->isEnableTexScrollAnm()
		    && !mDrawCtx.mBaseShape->isEnableProjection())
			mpExecPtclVis[execPtclVisNum++] = &vc.mDrawExecSetTexMtx;
		break;
	}

	if (mDrawCtx.mSweepShape) {
		if (mDrawCtx.mSweepShape->isEnableAlphaOut()
		    || mDrawCtx.mSweepShape->isInheritedAlpha()
		    || mDrawCtx.mSweepShape->isInheritedRGB())
			mpExecChldVis[execChldVisNum++] = &vc.mDrawExecRegisterPrmCEnv;

		switch (mDrawCtx.mSweepShape->getType()) {
		case 0:
			mpExecChldVis[execChldVisNum++] = &vc.mDrawExecSetPointSize;
			break;
		case 1:
			mpExecChldVis[execChldVisNum++] = &vc.mDrawExecSetLineWidth;
			break;
		}
	}
}

void JPADraw::setDrawExecVisitorsAfterCB(
    const JPADraw::JPADrawVisitorDefFlags& flags)
{
	if (flags.mbIsEnableDrawParent) {
		switch (mDrawCtx.mBaseShape->getType()) {
		case 0:
			mpExecPtclVis[execPtclVisNum++] = &vc.mDrawExecPoint;
			break;
		case 1:
			mpExecPtclVis[execPtclVisNum++] = &vc.mDrawExecLine;
			break;

		case 2:
			if (mDrawCtx.mExtraShape && mDrawCtx.mExtraShape->isEnableRotate())
				mpExecPtclVis[execPtclVisNum++] = &vc.mDrawExecRotBillBoard;
			else
				mpExecPtclVis[execPtclVisNum++] = &vc.mDrawExecBillBoard;
			break;
		case 3:
			if (mDrawCtx.mExtraShape && mDrawCtx.mExtraShape->isEnableRotate())
				mpExecPtclVis[execPtclVisNum++] = &vc.mDrawExecRotDirectional;
			else
				mpExecPtclVis[execPtclVisNum++] = &vc.mDrawExecDirectional;
			break;
		case 4:
			if (mDrawCtx.mExtraShape && mDrawCtx.mExtraShape->isEnableRotate())
				mpExecPtclVis[execPtclVisNum++]
				    = &vc.mDrawExecRotDirectionalCross;
			else
				mpExecPtclVis[execPtclVisNum++] = &vc.mDrawExecDirectionalCross;
			break;
		case 10:
			if (mDrawCtx.mExtraShape && mDrawCtx.mExtraShape->isEnableRotate())
				mpExecPtclVis[execPtclVisNum++] = &vc.mDrawExecRotYBillBoard;
			else
				mpExecPtclVis[execPtclVisNum++] = &vc.mDrawExecYBillBoard;
			break;

		case 7:
			mpExecPtclVis[execPtclVisNum++] = &vc.mDrawExecRotation;
			break;
		case 8:
			mpExecPtclVis[execPtclVisNum++] = &vc.mDrawExecRotationCross;
			break;
		case 9:
			mpExecPtclVis[execPtclVisNum++] = &vc.mDrawExecDirBillBoard;
			break;

		case 5:
			mpExecEmtrPVis[execEmtrPVisNum++] = &vc.mDrawExecStripe;
			break;
		case 6:
			mpExecEmtrPVis[execEmtrPVisNum++] = &vc.mDrawExecStripeCross;
			break;
		}
	}

	if (mDrawCtx.mSweepShape != nullptr) {
		switch (mDrawCtx.mSweepShape->getType()) {
		case 0:
			mpExecChldVis[execChldVisNum++] = &vc.mDrawExecPoint;
			break;
		case 1:
			mpExecChldVis[execChldVisNum++] = &vc.mDrawExecLine;
			break;

		case 2:
			if ((mDrawCtx.mExtraShape && mDrawCtx.mExtraShape->isEnableRotate())
			    || mDrawCtx.mSweepShape->isEnableRotate())
				mpExecChldVis[execChldVisNum++] = &vc.mDrawExecRotBillBoard;
			else
				mpExecChldVis[execChldVisNum++] = &vc.mDrawExecBillBoard;
			break;
		case 3:
			if ((mDrawCtx.mExtraShape && mDrawCtx.mExtraShape->isEnableRotate())
			    || mDrawCtx.mSweepShape->isEnableRotate())
				mpExecChldVis[execChldVisNum++] = &vc.mDrawExecRotDirectional;
			else
				mpExecChldVis[execChldVisNum++] = &vc.mDrawExecDirectional;
			break;
		case 4:
			if ((mDrawCtx.mExtraShape && mDrawCtx.mExtraShape->isEnableRotate())
			    || mDrawCtx.mSweepShape->isEnableRotate())
				mpExecChldVis[execChldVisNum++]
				    = &vc.mDrawExecRotDirectionalCross;
			else
				mpExecChldVis[execChldVisNum++] = &vc.mDrawExecDirectionalCross;
			break;
		case 10:
			if ((mDrawCtx.mExtraShape && mDrawCtx.mExtraShape->isEnableRotate())
			    || mDrawCtx.mSweepShape->isEnableRotate())
				mpExecChldVis[execChldVisNum++] = &vc.mDrawExecRotYBillBoard;
			else
				mpExecChldVis[execChldVisNum++] = &vc.mDrawExecYBillBoard;
			break;

		case 7:
			mpExecChldVis[execChldVisNum++] = &vc.mDrawExecRotation;
			break;
		case 8:
			mpExecChldVis[execChldVisNum++] = &vc.mDrawExecRotationCross;
			break;
		case 9:
			mpExecChldVis[execChldVisNum++] = &vc.mDrawExecDirBillBoard;
			break;

		case 5:
			mpExecEmtrCVis[execEmtrCVisNum++] = &vc.mDrawExecStripe;
			break;
		case 6:
			mpExecEmtrCVis[execEmtrCVisNum++] = &vc.mDrawExecStripeCross;
			break;
		}
	}
}

void JPADraw::setDrawCalcVisitors(const JPADraw::JPADrawVisitorDefFlags& flags)
{
	if ((mDrawCtx.mBaseShape->isEnableGlobalColAnm())
	    && (flags.mbHasPrmAnm || flags.mbHasEnvAnm)) {
		switch (mDrawCtx.mBaseShape->unk82) {
		case 0:
			mpCalcEmtrVis[calcEmtrVisNum++] = &vc.mDrawCalcColorAnmFrameNormal;
			break;
		case 1:
			mpCalcEmtrVis[calcEmtrVisNum++] = &vc.mDrawCalcColorAnmFrameRepeat;
			break;
		case 2:
			mpCalcEmtrVis[calcEmtrVisNum++] = &vc.mDrawCalcColorAnmFrameReverse;
			break;
		case 3:
			mpCalcEmtrVis[calcEmtrVisNum++] = &vc.mDrawCalcColorAnmFrameMerge;
			break;
		case 4:
			mpCalcEmtrVis[calcEmtrVisNum++] = &vc.mDrawCalcColorAnmFrameRandom;
			break;
		}

		if (flags.mbHasPrmAnm)
			mpCalcEmtrVis[calcEmtrVisNum++] = &vc.mDrawCalcColorPrm;

		if (flags.mbHasEnvAnm)
			mpCalcEmtrVis[calcEmtrVisNum++] = &vc.mDrawCalcColorEnv;
	}

	if (!mDrawCtx.mBaseShape->textureIsEmpty()
	    && mDrawCtx.mBaseShape->isEnableTextureAnm()
	    && mDrawCtx.mBaseShape->isEnableGlobalTexAnm()) {
		switch (mDrawCtx.mBaseShape->getTextureAnmType()) {
		case 0:
			mpCalcEmtrVis[calcEmtrVisNum++]
			    = &vc.mDrawCalcTextureAnmIndexNormal;
			break;
		case 1:
			mpCalcEmtrVis[calcEmtrVisNum++]
			    = &vc.mDrawCalcTextureAnmIndexRepeat;
			break;
		case 2:
			mpCalcEmtrVis[calcEmtrVisNum++]
			    = &vc.mDrawCalcTextureAnmIndexReverse;
			break;
		case 3:
			mpCalcEmtrVis[calcEmtrVisNum++] = &vc.mDrawCalcTextureAnmIndexMerge;
			break;
		case 4:
			mpCalcEmtrVis[calcEmtrVisNum++]
			    = &vc.mDrawCalcTextureAnmIndexRandom;
			break;
		}
	}

	if (mDrawCtx.mExtraShape != nullptr
	    && mDrawCtx.mExtraShape->isEnableScale()) {

		if (mDrawCtx.mExtraShape->isEnableScaleAnmX()) {
			if (mDrawCtx.mExtraShape->getAnmTypeX())
				mpCalcPtclVis[calcPtclVisNum++]
				    = &vc.mDrawCalcScaleAnmTimingReverseX;
			else
				mpCalcPtclVis[calcPtclVisNum++]
				    = &vc.mDrawCalcScaleAnmTimingRepeatX;
		} else {
			mpCalcPtclVis[calcPtclVisNum++] = &vc.mDrawCalcScaleAnmTimingNormal;
		}

		if ((mDrawCtx.mExtraShape->isEnableScaleBySpeedX())
		    && mDrawCtx.mBaseShape->getType() != 1)
			mpCalcPtclVis[calcPtclVisNum++] = &vc.mDrawCalcScaleXBySpeed;
		else
			mpCalcPtclVis[calcPtclVisNum++] = &vc.mDrawCalcScaleX;

		if (mDrawCtx.mBaseShape->getType() != 0) {
			if (!mDrawCtx.mExtraShape->isDiffXY()) {
				mpCalcPtclVis[calcPtclVisNum++] = &vc.mDrawCalcScaleCopyX2Y;
			} else {
				if (mDrawCtx.mExtraShape->isEnableScaleAnmY()) {
					if (mDrawCtx.mExtraShape->getAnmTypeY()) {
						mpCalcPtclVis[calcPtclVisNum++]
						    = &vc.mDrawCalcScaleAnmTimingReverseY;
					} else {
						mpCalcPtclVis[calcPtclVisNum++]
						    = &vc.mDrawCalcScaleAnmTimingRepeatY;
					}
				} else {
					if (mDrawCtx.mExtraShape->isEnableScaleAnmX()) {
						mpCalcPtclVis[calcPtclVisNum++]
						    = &vc.mDrawCalcScaleAnmTimingNormal;
					}
				}

				if (mDrawCtx.mExtraShape->isEnableScaleBySpeedY()) {
					mpCalcPtclVis[calcPtclVisNum++]
					    = &vc.mDrawCalcScaleYBySpeed;
				} else {
					mpCalcPtclVis[calcPtclVisNum++] = &vc.mDrawCalcScaleY;
				}
			}
		}
	}

	if (!mDrawCtx.mBaseShape->isEnableGlobalColAnm()) {
		if (flags.mbHasPrmAnm || flags.mbHasEnvAnm) {
			switch (mDrawCtx.mBaseShape->getColorRegAnmType()) {
			case 0:
				mpCalcPtclVis[calcPtclVisNum++]
				    = &vc.mDrawCalcColorAnmFrameNormal;
				break;
			case 1:
				mpCalcPtclVis[calcPtclVisNum++]
				    = &vc.mDrawCalcColorAnmFrameRepeat;
				break;
			case 2:
				mpCalcPtclVis[calcPtclVisNum++]
				    = &vc.mDrawCalcColorAnmFrameReverse;
				break;
			case 3:
				mpCalcPtclVis[calcPtclVisNum++]
				    = &vc.mDrawCalcColorAnmFrameMerge;
				break;
			case 4:
				mpCalcPtclVis[calcPtclVisNum++]
				    = &vc.mDrawCalcColorAnmFrameRandom;
				break;
			}

			if (flags.mbHasPrmAnm)
				mpCalcPtclVis[calcPtclVisNum++] = &vc.mDrawCalcColorPrm;

			if (flags.mbHasEnvAnm)
				mpCalcPtclVis[calcPtclVisNum++] = &vc.mDrawCalcColorEnv;
		}
	} else {
		mpCalcPtclVis[calcPtclVisNum++] = &vc.mDrawCalcColorCopyFromEmitter;
	}

	if (flags.mbIsEnableAlpha && !flags.mbIsStripe) {
		mpCalcPtclVis[calcPtclVisNum++] = &vc.mDrawCalcAlpha;
		if (mDrawCtx.mExtraShape->isEnableSinWave()) {
			switch (mDrawCtx.mExtraShape->getAlphaWaveType()) {
			case JPAAlphaWaveType_Nrm:
				mpCalcPtclVis[calcPtclVisNum++] = &vc.mDrawCalcAlphaFlickNrmSin;
				break;
			case JPAAlphaWaveType_Add:
				mpCalcPtclVis[calcPtclVisNum++] = &vc.mDrawCalcAlphaFlickAddSin;
				break;
			case JPAAlphaWaveType_Mult:
				mpCalcPtclVis[calcPtclVisNum++]
				    = &vc.mDrawCalcAlphaFlickMultSin;
				break;
			}
		}
	}

	if (!mDrawCtx.mBaseShape->textureIsEmpty()
	    && mDrawCtx.mBaseShape->isEnableTextureAnm()
	    && !mDrawCtx.mBaseShape->isEnableGlobalTexAnm()) {
		switch (mDrawCtx.mBaseShape->getTextureAnmType()) {
		case 0:
			mpCalcPtclVis[calcPtclVisNum++]
			    = &vc.mDrawCalcTextureAnmIndexNormal;
			break;
		case 1:
			mpCalcPtclVis[calcPtclVisNum++]
			    = &vc.mDrawCalcTextureAnmIndexRepeat;
			break;
		case 2:
			mpCalcPtclVis[calcPtclVisNum++]
			    = &vc.mDrawCalcTextureAnmIndexReverse;
			break;
		case 3:
			mpCalcPtclVis[calcPtclVisNum++] = &vc.mDrawCalcTextureAnmIndexMerge;
			break;
		case 4:
			mpCalcPtclVis[calcPtclVisNum++]
			    = &vc.mDrawCalcTextureAnmIndexRandom;
			break;
		}
	}

	if (mDrawCtx.mSweepShape) {
		if (mDrawCtx.mSweepShape->isEnableAlphaOut())
			mpCalcChldVis[calcChldVisNum++] = &vc.mDrawCalcChildAlphaOut;

		if (mDrawCtx.mSweepShape->isEnableScaleOut())
			mpCalcChldVis[calcChldVisNum++] = &vc.mDrawCalcChildScaleOut;
	}
}

void JPADraw::setParticleClipBoard()
{
	switch (mDrawCtx.mBaseShape->getType()) {
	case 2:
	case 9:
		MTXIdentity(cb.unk68);
		break;
	case 10:
		loadYBBMtx(cb.mViewMtx);
		break;
	default:
		MTXCopy(cb.mViewMtx, cb.unk68);
		break;
	}

	GXLoadPosMtxImm(cb.unk68, GX_PNMTX0);

	JGeometry::TVec3<f32> scale;
	mDrawCtx.mBaseEmitter->getGlobalParticleScale(scale);
	cb.unk4.x = 25.0f * mDrawCtx.mBaseShape->getBaseSizeX() * scale.x;
	cb.unk4.y = 25.0f * mDrawCtx.mBaseShape->getBaseSizeY() * scale.y;

	if (mDrawCtx.mBaseShape->getType() == 0) {
		cb.unk4.x *= 1.02f;
		cb.unk4.y *= 1.02f;
	} else if (mDrawCtx.mBaseShape->getType() == 1) {
		cb.unk4.x *= 1.02f;
		cb.unk4.y *= 0.4f;
	}

	if (mDrawCtx.mExtraShape != nullptr
	    && mDrawCtx.mExtraShape->isEnableScale()) {
		cb.unkC.x = cb.unk4.x * (mDrawCtx.mExtraShape->getPivotX() - 1.0f);
		cb.unkC.y = cb.unk4.y * (mDrawCtx.mExtraShape->getPivotY() - 1.0f);
	} else {
		cb.unkC.x = cb.unkC.y = 0.0f;
	}

	f32 tilingX = mDrawCtx.mBaseShape->getTilingX();
	f32 tilingY = mDrawCtx.mBaseShape->getTilingY();

	cb.mTexCoords[0].x = 0.0f;
	cb.mTexCoords[0].y = 0.0f;
	cb.mTexCoords[1].x = tilingX;
	cb.mTexCoords[1].y = 0.0f;

	cb.mTexCoords[2].x = tilingX;
	cb.mTexCoords[2].y = tilingY;

	cb.mTexCoords[3].x = 0.0f;
	cb.mTexCoords[3].y = tilingY;

	if (!mDrawCtx.mBaseShape->textureIsEmpty()
	    && !mDrawCtx.mBaseShape->isEnableTextureAnm())
		mTexIdx = mDrawCtx.mTexIndices[mDrawCtx.mBaseShape->getTextureIndex()];

	cb.mDirTypeFunc = nullptr;
	cb.mRotTypeFunc = nullptr;

	if (mDrawCtx.mBaseShape->getType() == 3
	    || mDrawCtx.mBaseShape->getType() == 9
	    || mDrawCtx.mBaseShape->getType() == 4
	    || mDrawCtx.mBaseShape->getType() == 5
	    || mDrawCtx.mBaseShape->getType() == 6) {
		switch (mDrawCtx.mBaseShape->getDirType()) {
		case 0:
			cb.mDirTypeFunc = &dirTypeVel;
			break;
		case 1:
			cb.mDirTypeFunc = &dirTypePos;
			break;
		case 2:
			cb.mDirTypeFunc = &dirTypePosInv;
			break;
		case 3:
			cb.mDirTypeFunc = &dirTypeEmtrDir;
			break;
		case 4:
			cb.mDirTypeFunc = &dirTypePrevPtcl;
			break;
		}
	}

	if (mDrawCtx.mBaseShape->getType() == 3
	    || mDrawCtx.mBaseShape->getType() == 4
	    || mDrawCtx.mBaseShape->getType() == 7
	    || mDrawCtx.mBaseShape->getType() == 8) {
		switch (mDrawCtx.mBaseShape->getRotType()) {
		case 0:
			cb.mRotTypeFunc = &rotTypeY;
			break;
		case 1:
			cb.mRotTypeFunc = &rotTypeX;
			break;
		case 2:
			cb.mRotTypeFunc = &rotTypeZ;
			break;
		case 3:
			cb.mRotTypeFunc = &rotTypeXYZ;
			break;
		case 4:
			cb.mRotTypeFunc = &rotTypeYJiggle;
			break;
		}
	}
}

void JPADraw::setChildClipBoard()
{
	switch (mDrawCtx.mSweepShape->getType()) {
	case 2:
	case 9:
		MTXIdentity(cb.unk68);
		break;
	case 10:
		loadYBBMtx(cb.mViewMtx);
		break;
	default:
		MTXCopy(cb.mViewMtx, cb.unk68);
		break;
	}

	GXLoadPosMtxImm(cb.unk68, GX_PNMTX0);

	JGeometry::TVec3<f32> scale;
	mDrawCtx.mBaseEmitter->getGlobalParticleScale(scale);
	if (!mDrawCtx.mSweepShape->isInheritedScale()) {
		cb.unk4.x = 25.0f * mDrawCtx.mSweepShape->getScaleX() * scale.x;
		cb.unk4.y = 25.0f * mDrawCtx.mSweepShape->getScaleY() * scale.y;
	} else {
		cb.unk4.x = 25.0f * mDrawCtx.mBaseShape->getBaseSizeX() * scale.x;
		cb.unk4.y = 25.0f * mDrawCtx.mBaseShape->getBaseSizeY() * scale.y;
	}

	if (mDrawCtx.mSweepShape->getType() == 0) {
		cb.unk4.x *= 1.02f;
		cb.unk4.y *= 1.02f;
	} else if (mDrawCtx.mSweepShape->getType() == 1) {
		cb.unk4.x *= 1.02f;
		cb.unk4.y *= 0.4f;
	}

	cb.unkC.zero();

	cb.mTexCoords[1].y = 0.0;
	cb.mTexCoords[0].y = 0.0;
	cb.mTexCoords[3].x = 0.0;
	cb.mTexCoords[0].x = 0.0;
	cb.mTexCoords[3].y = 1.0;
	cb.mTexCoords[2].y = 1.0;
	cb.mTexCoords[2].x = 1.0;
	cb.mTexCoords[1].x = 1.0;

	cb.mDirTypeFunc = nullptr;
	cb.mRotTypeFunc = nullptr;

	if (mDrawCtx.mSweepShape->getType() == 3
	    || mDrawCtx.mSweepShape->getType() == 9
	    || mDrawCtx.mSweepShape->getType() == 4
	    || mDrawCtx.mSweepShape->getType() == 5
	    || mDrawCtx.mSweepShape->getType() == 6) {
		switch (mDrawCtx.mSweepShape->getDirType()) {
		case 0:
			cb.mDirTypeFunc = &dirTypeVel;
			break;
		case 1:
			cb.mDirTypeFunc = &dirTypePos;
			break;
		case 2:
			cb.mDirTypeFunc = &dirTypePosInv;
			break;
		case 3:
			cb.mDirTypeFunc = &dirTypeEmtrDir;
			break;
		case 4:
			cb.mDirTypeFunc = &dirTypePrevPtcl;
			break;
		}
	}

	if (mDrawCtx.mSweepShape->getType() == 3
	    || mDrawCtx.mSweepShape->getType() == 4
	    || mDrawCtx.mSweepShape->getType() == 7
	    || mDrawCtx.mSweepShape->getType() == 8) {
		switch (mDrawCtx.mSweepShape->getRotType()) {
		case 0:
			cb.mRotTypeFunc = &rotTypeY;
			break;
		case 1:
			cb.mRotTypeFunc = &rotTypeX;
			break;
		case 2:
			cb.mRotTypeFunc = &rotTypeZ;
			break;
		case 3:
			cb.mRotTypeFunc = &rotTypeXYZ;
			break;
		case 4:
			cb.mRotTypeFunc = &rotTypeYJiggle;
			break;
		}
	}
}

void JPADraw::drawParticle()
{
	unkC2 &= ~0x2;
	setParticleClipBoard();
	mDrawCtx.unk18 = mDrawCtx.mBaseEmitter->getParticleList();

	GXSetPointSize(cb.unk4.x, GX_TO_ONE);
	GXSetLineWidth(cb.unk4.x, GX_TO_ONE);
	GXSetZMode(mDrawCtx.mBaseShape->isEnableZCmp(),
	           mDrawCtx.mBaseShape->getZCmpFunction(),
	           mDrawCtx.mBaseShape->isEnableZCmpUpdate());
	GXSetZCompLoc(mDrawCtx.mBaseShape->getZCompLoc());
	GXSetAlphaCompare(mDrawCtx.mBaseShape->getAlphaCmpComp0(),
	                  mDrawCtx.mBaseShape->getAlphaCmpRef0(),
	                  mDrawCtx.mBaseShape->getAlphaCmpOp(),
	                  mDrawCtx.mBaseShape->getAlphaCmpComp1(),
	                  mDrawCtx.mBaseShape->getAlphaCmpRef1());

	GXSetAlphaUpdate(mDrawCtx.mBaseShape->isEnableAlphaUpdate());
	GXSetColorUpdate(GX_TRUE);
	GXSetCullMode(GX_CULL_NONE);
	if ((mDrawCtx.mBaseShape->isClipOn())) {
		GXSetClipMode(GX_CLIP_ENABLE);
		GXSetMisc(GX_MT_XF_FLUSH, 8);
	} else {
		GXSetClipMode(GX_CLIP_DISABLE);
	}

	for (int i = 0; i < execEmtrPVisNum; ++i)
		mpExecEmtrPVis[i]->exec(&mDrawCtx);

	JSUList<JPABaseParticle>* particles
	    = mDrawCtx.mBaseEmitter->getParticleList();
	if (mDrawCtx.mBaseShape->getListOrder()) {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getFirst(); link; link = link->getNext()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < execPtclVisNum; ++i)
				mpExecPtclVis[i]->exec(&mDrawCtx, particle);
		}
	} else {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getLast(); link; link = link->getPrev()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < execPtclVisNum; ++i)
				mpExecPtclVis[i]->exec(&mDrawCtx, particle);
		}
	}

	GXSetMisc(GX_MT_XF_FLUSH, 0);
}

void JPADraw::drawChild()
{
	unkC2 |= 0x2;
	setChildClipBoard();
	mDrawCtx.unk18 = mDrawCtx.mBaseEmitter->getChildParticleList();
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
	GXEnableTexOffsets(GX_TEXCOORD0, GX_TRUE, GX_TRUE);

	if (mDrawCtx.mBaseShape->textureIsEmpty()) {
		mDrawCtx.mTexResource->loadDefaultTexture(GX_TEXMAP0);
	} else {
		mDrawCtx.mTexResource->load(
		    mDrawCtx.mTexIndices[mDrawCtx.mSweepShape->getTextureIndex()],
		    GX_TEXMAP0);
	}

	GXSetZMode(mDrawCtx.mBaseShape->isEnableZCmp(),
	           mDrawCtx.mBaseShape->getZCmpFunction(),
	           mDrawCtx.mBaseShape->isEnableZCmpUpdate());
	GXSetZCompLoc(mDrawCtx.mBaseShape->getZCompLoc());
	GXSetAlphaCompare(mDrawCtx.mBaseShape->getAlphaCmpComp0(),
	                  mDrawCtx.mBaseShape->getAlphaCmpRef0(),
	                  mDrawCtx.mBaseShape->getAlphaCmpOp(),
	                  mDrawCtx.mBaseShape->getAlphaCmpComp1(),
	                  mDrawCtx.mBaseShape->getAlphaCmpRef1());
	GXSetAlphaUpdate(mDrawCtx.mBaseShape->isEnableAlphaUpdate());
	GXSetColorUpdate(GX_TRUE);
	GXSetCullMode(GX_CULL_NONE);

	if (mDrawCtx.mSweepShape->isClipOn()) {
		GXSetClipMode(GX_CLIP_ENABLE);
		GXSetMisc(GX_MT_XF_FLUSH, 8);
	} else {
		GXSetClipMode(GX_CLIP_DISABLE);
	}

	for (int i = 0; i < execEmtrCVisNum; ++i)
		mpExecEmtrCVis[i]->exec(&mDrawCtx);

	JSUList<JPABaseParticle>* particles
	    = mDrawCtx.mBaseEmitter->getChildParticleList();
	if (mDrawCtx.mBaseShape->getListOrder()) {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getFirst(); link; link = link->getNext()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < execChldVisNum; ++i)
				mpExecChldVis[i]->exec(&mDrawCtx, particle);
		}
	} else {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getLast(); link; link = link->getPrev()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < execChldVisNum; ++i)
				mpExecChldVis[i]->exec(&mDrawCtx, particle);
		}
	}

	GXSetMisc(GX_MT_XF_FLUSH, 0);
}

void JPADraw::zDraw()
{
	unkC2 |= 1;
	if (mDrawCtx.mBaseShape->getChildOrder() == 1
	    && mDrawCtx.mSweepShape != nullptr)
		zDrawChild();

	zDrawParticle();

	if (mDrawCtx.mBaseShape->getChildOrder() == 0
	    && mDrawCtx.mSweepShape != nullptr)
		zDrawChild();
}

void JPADraw::zDrawParticle()
{
	unkC2 &= ~0x2;
	setParticleClipBoard();
	mDrawCtx.unk18 = mDrawCtx.mBaseEmitter->getParticleList();

	GXSetPointSize(cb.unk4.x, GX_TO_ONE);
	GXSetLineWidth(cb.unk4.x, GX_TO_ONE);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetZCompLoc(GX_FALSE);

	GXSetAlphaCompare(GX_GEQUAL, mDrawCtx.mBaseEmitter->getGlobalAlpha(),
	                  GX_AOP_OR, GX_GEQUAL,
	                  mDrawCtx.mBaseEmitter->getGlobalAlpha());
	GXSetAlphaUpdate(GX_FALSE);
	GXSetColorUpdate(GX_FALSE);
	GXSetCullMode(GX_CULL_NONE);
	if (mDrawCtx.mBaseShape->isClipOn()) {
		GXSetClipMode(GX_CLIP_ENABLE);
		GXSetMisc(GX_MT_XF_FLUSH, 8);
	} else {
		GXSetClipMode(GX_CLIP_DISABLE);
	}

	for (int i = 0; i < execEmtrPVisNum; ++i)
		mpExecEmtrPVis[i]->exec(&mDrawCtx);

	JSUList<JPABaseParticle>* particles
	    = mDrawCtx.mBaseEmitter->getParticleList();
	if (mDrawCtx.mBaseShape->getListOrder()) {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getFirst(); link; link = link->getNext()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < execPtclVisNum; ++i)
				mpExecPtclVis[i]->exec(&mDrawCtx, particle);
		}
	} else {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getLast(); link; link = link->getPrev()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < execPtclVisNum; ++i)
				mpExecPtclVis[i]->exec(&mDrawCtx, particle);
		}
	}

	GXSetMisc(GX_MT_XF_FLUSH, 0);
}

void JPADraw::zDrawChild()
{
	unkC2 |= 0x2;
	setChildClipBoard();
	mDrawCtx.unk18 = mDrawCtx.mBaseEmitter->getChildParticleList();
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
	GXEnableTexOffsets(GX_TEXCOORD0, GX_TRUE, GX_TRUE);

	if (mDrawCtx.mBaseShape->textureIsEmpty()) {
		mDrawCtx.mTexResource->loadDefaultTexture(GX_TEXMAP0);
	} else {
		mDrawCtx.mTexResource->load(
		    mDrawCtx.mTexIndices[mDrawCtx.mSweepShape->getTextureIndex()],
		    GX_TEXMAP0);
	}

	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetZCompLoc(GX_FALSE);
	GXSetAlphaCompare(GX_GEQUAL, mDrawCtx.mBaseEmitter->getGlobalAlpha(),
	                  GX_AOP_OR, GX_GEQUAL,
	                  mDrawCtx.mBaseEmitter->getGlobalAlpha());
	GXSetAlphaUpdate(GX_FALSE);
	GXSetColorUpdate(GX_FALSE);
	GXSetCullMode(GX_CULL_NONE);

	if (mDrawCtx.mSweepShape->isClipOn()) {
		GXSetClipMode(GX_CLIP_ENABLE);
		GXSetMisc(GX_MT_XF_FLUSH, 8);
	} else {
		GXSetClipMode(GX_CLIP_DISABLE);
	}

	for (int i = 0; i < execEmtrCVisNum; ++i)
		mpExecEmtrCVis[i]->exec(&mDrawCtx);

	JSUList<JPABaseParticle>* particles
	    = mDrawCtx.mBaseEmitter->getChildParticleList();
	if (mDrawCtx.mBaseShape->getListOrder()) {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getFirst(); link; link = link->getNext()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < execChldVisNum; ++i)
				mpExecChldVis[i]->exec(&mDrawCtx, particle);
		}
	} else {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getLast(); link; link = link->getPrev()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < execChldVisNum; ++i)
				mpExecChldVis[i]->exec(&mDrawCtx, particle);
		}
	}

	GXSetMisc(GX_MT_XF_FLUSH, 0);
}

s16 JPADraw::getMainTextureID(u8 i)
{
	s16 result = -1;
	if (mDrawCtx.mBaseShape->unk80 != 0) {
		if (i < mDrawCtx.mBaseShape->getTextureAnmKeyNum())
			result = mDrawCtx.mBaseShape->getTextureIndex(i);
	} else {
		if (i == 0)
			result = mDrawCtx.mBaseShape->getTextureIndex();
	}
	return result;
}

s16 JPADraw::getIndTextureID()
{
	s16 result = -1;
	if (mDrawCtx.mExTexShape != nullptr
	    && mDrawCtx.mExTexShape->getIndTexMode() != 0)
		result = mDrawCtx.mExTexShape->getIndTextureID();
	return result;
}

s16 JPADraw::getIndSubTextureID()
{
	s16 result = -1;
	if (mDrawCtx.mExTexShape != nullptr
	    && mDrawCtx.mExTexShape->getIndTexMode() == 2)
		result = mDrawCtx.mExTexShape->getSubTextureID();
	return result;
}

s16 JPADraw::getSecondTextureID()
{
	s16 result = -1;
	if (mDrawCtx.mExTexShape != nullptr
	    && mDrawCtx.mExTexShape->isEnableSecondTex())
		result = mDrawCtx.mExTexShape->getSecondTexIndex();
	return result;
}

void JPADraw::loadYBBMtx(MtxPtr mtx)
{
	JGeometry::TVec3<f32> v(0.0f, mtx[1][1], mtx[2][1]);
	JUT_ASSERT(!v.isZero());
	v.normalize();

	cb.unk38[0][0] = 1.0f;
	cb.unk38[0][1] = 0.0f;
	cb.unk38[0][2] = 0.0f;
	cb.unk38[0][3] = mtx[0][3];

	cb.unk38[1][0] = 0.0f;
	cb.unk38[1][1] = v.y;
	cb.unk38[1][2] = -v.z;
	cb.unk38[1][3] = mtx[1][3];

	cb.unk38[2][0] = 0.0f;
	cb.unk38[2][1] = v.z;
	cb.unk38[2][2] = v.y;
	cb.unk38[2][3] = mtx[2][3];

	MTXIdentity(cb.unk68);
}
