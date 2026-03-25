#include <JSystem/JParticle/JPADraw.hpp>
#include <JSystem/JParticle/JPABaseShape.hpp>
#include <JSystem/JParticle/JPAExtraShape.hpp>
#include <JSystem/JParticle/JPASweepShape.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/JParticle/JPADrawSetupTev.hpp>
#include <JSystem/JParticle/JPAParticle.hpp>
#include <dolphin/gx.h>
#include <macros.h>

JPADrawVisitorContainer JPADraw::vc;
JPADrawClipBoard JPADraw::cb;

BOOL JPADraw::initialize(JPABaseEmitter* emitter,
                         JPATextureResource* tex_resource)
{
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

	unkC2 = 0;
	unkB4 = 1.0f;

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

	unk8F = 0;
	unk8D = 0;
	unk8E = 0;
	unk8C = 0;
	unk8B = 0;
	unk8A = 0;
	unk89 = 0;
	unk88 = 0;

	for (int i = 0; i < ARRAY_COUNT(unk0); ++i)
		unk0[i] = nullptr;

	for (int i = 0; i < ARRAY_COUNT(unk4); ++i)
		unk4[i] = nullptr;

	for (int i = 0; i < ARRAY_COUNT(unk18); ++i)
		unk18[i] = nullptr;

	for (int i = 0; i < ARRAY_COUNT(unk24); ++i)
		unk24[i] = nullptr;

	for (int i = 0; i < ARRAY_COUNT(unk34); ++i)
		unk34[i] = nullptr;

	for (int i = 0; i < ARRAY_COUNT(unk70); ++i)
		unk70[i] = nullptr;

	for (int i = 0; i < ARRAY_COUNT(unk48); ++i)
		unk48[i] = nullptr;

	for (int i = 0; i < ARRAY_COUNT(unk80); ++i)
		unk80[i] = nullptr;

	JPADrawVisitorDefFlags local_28;

	local_28.unk0 = mDrawCtx.mSweepShape == nullptr
	                || mDrawCtx.mSweepShape->isEnableDrawParent();
	local_28.unk4 = mDrawCtx.mBaseShape->isEnablePrm()
	                && mDrawCtx.mBaseShape->isEnablePrmAnm();
	local_28.unk8 = mDrawCtx.mBaseShape->isEnableEnv()
	                && mDrawCtx.mBaseShape->isEnableEnvAnm();
	local_28.unkC = mDrawCtx.mBaseShape->getType() == 5
	                || mDrawCtx.mBaseShape->getType() == 6;
	local_28.unk10 = mDrawCtx.mBaseShape->getType() == 0
	                 || mDrawCtx.mBaseShape->getType() == 1;
	local_28.unk14 = mDrawCtx.mExtraShape != nullptr
	                 && mDrawCtx.mExtraShape->isEnableAlpha();

	setDrawExecVisitorsBeforeCB(local_28);
	unk4[unk89++]  = &vc.mDrawExecCallBack;
	unk18[unk8A++] = &vc.mDrawExecCallBack;
	unk34[unk8C++] = &vc.mDrawExecCallBack;
	unk70[unk8E++] = &vc.mDrawExecCallBack;
	setDrawExecVisitorsAfterCB(local_28);
	setDrawCalcVisitors(local_28);

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

	// TODO: fakematch
	JPABaseEmitter* be = mDrawCtx.mBaseEmitter;

	cb.mPrmColor.r = be->unk180.r;
	cb.mPrmColor.g = be->unk180.g;
	cb.mPrmColor.b = be->unk180.b;
	cb.mPrmColor.a = be->unk180.a;

	be             = mDrawCtx.mBaseEmitter;
	cb.mEnvColor.r = be->unk184.r;
	cb.mEnvColor.g = be->unk184.g;
	cb.mEnvColor.b = be->unk184.b;
	cb.mEnvColor.a = 0xff;

	cb.mViewMtx = view_mtx;

	cb.mSetupTev.setupTev(mDrawCtx.mBaseShape, mDrawCtx.mExTexShape);

	for (int i = 0; i < unk88; ++i)
		unk0[i]->exec(&mDrawCtx);

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
	for (int i = 0; i < unk8B; ++i)
		unk24[i]->calc(&mDrawCtx);
}

void JPADraw::calcParticle(JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();
	params->unk34 += params->unk36;
	for (int i = 0; i < unk8D; ++i)
		unk48[i]->calc(&mDrawCtx, particle);
}

void JPADraw::calcChild(JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamCPtr();
	params->unk34 += params->unk36;
	for (int i = 0; i < unk8F; ++i)
		unk80[i]->calc(&mDrawCtx, particle);
}

void JPADraw::initParticle(JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	// TODO: something wrong here
	f32* v         = JPAGetEmitterInfoPtr()->unk48[1];
	params->unk0.x = v[0];
	params->unk0.y = v[1];
	params->unk0.z = v[2];

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

		params->unk10 = params->unk14 = params->unkC
		    = unkB4
		      * (mDrawCtx.mBaseEmitter->getRandomRF()
		             * mDrawCtx.mExtraShape->getRandomScale()
		         + 1.0f);

		params->unk24 = mDrawCtx.mBaseEmitter->getRandomRF()
		                    * mDrawCtx.mExtraShape->getAlphaWaveRandom()
		                + 1.0f;
	} else {
		params->unk34 = 0;
		params->unk36 = 0;
		params->unkC = params->unk10 = params->unk14 = unkB4;

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

		childPrms->unk10 = childPrms->unkC = inheritFactor * parentPrms->unk10;
		childPrms->unk14 = childPrms->unk24 = inheritFactor * parentPrms->unk14;
	} else {
		childPrms->unk24 = 1.0f;
		childPrms->unk14 = 1.0f;
		childPrms->unkC  = 1.0f;
		childPrms->unk10 = 1.0f;
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

void JPADraw::loadTexture(u8 idx, GXTexMapID map_id)
{
	// TODO: this is 1 instruction too short!
	// mDrawCtx.mTexResource->load(mDrawCtx.mTexIndices[idx], map_id);
}

void JPADraw::setDrawExecVisitorsBeforeCB(
    const JPADraw::JPADrawVisitorDefFlags& flags)
{
	if (mDrawCtx.mExTexShape)
		unk0[unk88++] = &vc.mDrawExecLoadExTex;

	if (flags.unk10) {
		unk4[unk89++] = &vc.mDrawExecGenIdtMtx;
	} else if (mDrawCtx.mBaseShape->isEnableProjection()) {
		if (mDrawCtx.mBaseShape->isEnableTexScrollAnm())
			unk4[unk89++] = &vc.mDrawExecGenPrjTexMtx;
		else
			unk4[unk89++] = &vc.mDrawExecGenPrjMtx;
	} else if (mDrawCtx.mBaseShape->isEnableTexScrollAnm()) {
		if (flags.unkC)
			unk4[unk89++] = &vc.mDrawExecSetTexMtx;
		else
			unk4[unk89++] = &vc.mDrawExecGenTexMtx0;
	} else {
		unk4[unk89++] = &vc.mDrawExecGenIdtMtx;
	}

	if (mDrawCtx.mBaseShape->isEnableGlobalColAnm()) {
		if (flags.unkC || !flags.unk14) {
			unk4[unk89++] = &vc.mDrawExecRegisterColorEmitterPE;
		} else if (flags.unk14) {
			unk4[unk89++] = &vc.mDrawExecRegisterColorEmitterE;
		}
	} else {
		if ((!mDrawCtx.mBaseShape->isEnablePrm()
		     || !mDrawCtx.mBaseShape->isEnablePrmAnm())
		    && flags.unk14 == 0) {
			if (!mDrawCtx.mBaseShape->isEnableEnv()
			    || !mDrawCtx.mBaseShape->isEnableEnvAnm()) {
				unk4[unk89++] = &vc.mDrawExecRegisterColorEmitterPE;
			} else {
				unk4[unk89++] = &vc.mDrawExecRegisterColorEmitterP;
			}
		} else if (!mDrawCtx.mBaseShape->isEnableEnv()
		           || !mDrawCtx.mBaseShape->isEnableEnvAnm()) {
			unk4[unk89++] = &vc.mDrawExecRegisterColorEmitterE;
		}
	}

	if (!mDrawCtx.mBaseShape->textureIsEmpty()
	    && (!mDrawCtx.mBaseShape->isEnableTextureAnm()
	        || mDrawCtx.mBaseShape->isEnableGlobalTexAnm())) {
		unk4[unk89++] = &vc.mDrawExecLoadTexture;
	} else if (mDrawCtx.mBaseShape->textureIsEmpty()) {
		unk4[unk89++] = &vc.mDrawExecLoadDefaultTexture;
	}

	if (mDrawCtx.mSweepShape != nullptr
	    && !mDrawCtx.mSweepShape->isEnableAlphaOut()
	    && !mDrawCtx.mSweepShape->isInheritedAlpha()
	    && !mDrawCtx.mSweepShape->isInheritedRGB()) {
		unk18[unk8A++] = &vc.mDrawExecRegisterColorChildPE;
	}

	if (!mDrawCtx.mBaseShape->isEnableGlobalColAnm()) {
		if (flags.unk4) {
			if (flags.unk8)
				unk34[unk8C++] = &vc.mDrawExecRegisterPrmCEnv;
			else
				unk34[unk8C++] = &vc.mDrawExecRegisterPrmColorAnm;
		} else {
			if (flags.unk14) {
				if (flags.unk8)
					unk34[unk8C++] = &vc.mDrawExecRegisterPrmAEnv;
				else
					unk34[unk8C++] = &vc.mDrawExecRegisterPrmAlphaAnm;
			} else {
				if (flags.unk8)
					unk34[unk8C++] = &vc.mDrawExecRegisterEnvColorAnm;
			}
		}
	} else if (flags.unk14 && !flags.unkC) {
		unk34[unk8C++] = &vc.mDrawExecRegisterPrmAlphaAnm;
	}

	if (!mDrawCtx.mBaseShape->textureIsEmpty()
	    && mDrawCtx.mBaseShape->isEnableTextureAnm()
	    && !mDrawCtx.mBaseShape->isEnableGlobalTexAnm()) {
		unk34[unk8C++] = &vc.mDrawExecLoadTexture;
	}

	switch (mDrawCtx.mBaseShape->getType()) {
	case 0:
		if (mDrawCtx.mExtraShape && mDrawCtx.mExtraShape->isEnableScale())
			unk34[unk8C++] = &vc.mDrawExecSetPointSize;
		break;
	case 1:
		if (mDrawCtx.mExtraShape && mDrawCtx.mExtraShape->isEnableScale())
			unk34[unk8C++] = &vc.mDrawExecSetLineWidth;
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
			unk34[unk8C++] = &vc.mDrawExecSetTexMtx;
		break;
	}

	if (mDrawCtx.mSweepShape) {
		if (mDrawCtx.mSweepShape->isEnableAlphaOut()
		    || mDrawCtx.mSweepShape->isInheritedAlpha()
		    || mDrawCtx.mSweepShape->isInheritedRGB())
			unk70[unk8E++] = &vc.mDrawExecRegisterPrmCEnv;

		switch (mDrawCtx.mSweepShape->getType()) {
		case 0:
			unk70[unk8E++] = &vc.mDrawExecSetPointSize;
			break;
		case 1:
			unk70[unk8E++] = &vc.mDrawExecSetLineWidth;
			break;
		}
	}
}

void JPADraw::setDrawExecVisitorsAfterCB(
    const JPADraw::JPADrawVisitorDefFlags& flags)
{
	if (flags.unk0) {
		switch (mDrawCtx.mBaseShape->getType()) {
		case 0:
			unk34[unk8C++] = &vc.mDrawExecPoint;
			break;
		case 1:
			unk34[unk8C++] = &vc.mDrawExecLine;
			break;

		case 2:
			if (mDrawCtx.mExtraShape && mDrawCtx.mExtraShape->isEnableRotate())
				unk34[unk8C++] = &vc.mDrawExecRotBillBoard;
			else
				unk34[unk8C++] = &vc.mDrawExecBillBoard;
			break;
		case 3:
			if (mDrawCtx.mExtraShape && mDrawCtx.mExtraShape->isEnableRotate())
				unk34[unk8C++] = &vc.mDrawExecRotDirectional;
			else
				unk34[unk8C++] = &vc.mDrawExecDirectional;
			break;
		case 4:
			if (mDrawCtx.mExtraShape && mDrawCtx.mExtraShape->isEnableRotate())
				unk34[unk8C++] = &vc.mDrawExecRotDirectionalCross;
			else
				unk34[unk8C++] = &vc.mDrawExecDirectionalCross;
			break;
		case 10:
			if (mDrawCtx.mExtraShape && mDrawCtx.mExtraShape->isEnableRotate())
				unk34[unk8C++] = &vc.mDrawExecRotYBillBoard;
			else
				unk34[unk8C++] = &vc.mDrawExecYBillBoard;
			break;

		case 7:
			unk34[unk8C++] = &vc.mDrawExecRotation;
			break;
		case 8:
			unk34[unk8C++] = &vc.mDrawExecRotationCross;
			break;
		case 9:
			unk34[unk8C++] = &vc.mDrawExecDirBillBoard;
			break;

		case 5:
			unk4[unk89++] = &vc.mDrawExecStripe;
			break;
		case 6:
			unk4[unk89++] = &vc.mDrawExecStripeCross;
			break;
		}
	}

	if (mDrawCtx.mSweepShape != nullptr) {
		switch (mDrawCtx.mSweepShape->getType()) {
		case 0:
			unk70[unk8E++] = &vc.mDrawExecPoint;
			break;
		case 1:
			unk70[unk8E++] = &vc.mDrawExecLine;
			break;

		case 2:
			if ((mDrawCtx.mExtraShape && mDrawCtx.mExtraShape->isEnableRotate())
			    || mDrawCtx.mSweepShape->isEnableRotate())
				unk70[unk8E++] = &vc.mDrawExecRotBillBoard;
			else
				unk70[unk8E++] = &vc.mDrawExecBillBoard;
			break;
		case 3:
			if ((mDrawCtx.mExtraShape && mDrawCtx.mExtraShape->isEnableRotate())
			    || mDrawCtx.mSweepShape->isEnableRotate())
				unk70[unk8E++] = &vc.mDrawExecRotDirectional;
			else
				unk70[unk8E++] = &vc.mDrawExecDirectional;
			break;
		case 4:
			if ((mDrawCtx.mExtraShape && mDrawCtx.mExtraShape->isEnableRotate())
			    || mDrawCtx.mSweepShape->isEnableRotate())
				unk70[unk8E++] = &vc.mDrawExecRotDirectionalCross;
			else
				unk70[unk8E++] = &vc.mDrawExecDirectionalCross;
			break;
		case 10:
			if ((mDrawCtx.mExtraShape && mDrawCtx.mExtraShape->isEnableRotate())
			    || mDrawCtx.mSweepShape->isEnableRotate())
				unk70[unk8E++] = &vc.mDrawExecRotYBillBoard;
			else
				unk70[unk8E++] = &vc.mDrawExecYBillBoard;
			break;

		case 7:
			unk70[unk8E++] = &vc.mDrawExecRotation;
			break;
		case 8:
			unk70[unk8E++] = &vc.mDrawExecRotationCross;
			break;
		case 9:
			unk70[unk8E++] = &vc.mDrawExecDirBillBoard;
			break;

		case 5:
			unk18[unk8A++] = &vc.mDrawExecStripe;
			break;
		case 6:
			unk18[unk8A++] = &vc.mDrawExecStripeCross;
			break;
		}
	}
}

void JPADraw::setDrawCalcVisitors(const JPADraw::JPADrawVisitorDefFlags& flags)
{
	if ((mDrawCtx.mBaseShape->isEnableGlobalColAnm())
	    && (flags.unk4 || flags.unk8)) {
		switch (mDrawCtx.mBaseShape->unk82) {
		case 0:
			unk24[unk8B++] = &vc.mDrawCalcColorAnmFrameNormal;
			break;
		case 1:
			unk24[unk8B++] = &vc.mDrawCalcColorAnmFrameRepeat;
			break;
		case 2:
			unk24[unk8B++] = &vc.mDrawCalcColorAnmFrameReverse;
			break;
		case 3:
			unk24[unk8B++] = &vc.mDrawCalcColorAnmFrameMerge;
			break;
		case 4:
			unk24[unk8B++] = &vc.mDrawCalcColorAnmFrameRandom;
			break;
		}

		if (flags.unk4)
			unk24[unk8B++] = &vc.mDrawCalcColorPrm;

		if (flags.unk8)
			unk24[unk8B++] = &vc.mDrawCalcColorEnv;
	}

	if (!mDrawCtx.mBaseShape->textureIsEmpty()
	    && mDrawCtx.mBaseShape->isEnableTextureAnm()
	    && mDrawCtx.mBaseShape->isEnableGlobalTexAnm()) {
		switch (mDrawCtx.mBaseShape->getTextureAnmType()) {
		case 0:
			unk24[unk8B++] = &vc.mDrawCalcTextureAnmIndexNormal;
			break;
		case 1:
			unk24[unk8B++] = &vc.mDrawCalcTextureAnmIndexRepeat;
			break;
		case 2:
			unk24[unk8B++] = &vc.mDrawCalcTextureAnmIndexReverse;
			break;
		case 3:
			unk24[unk8B++] = &vc.mDrawCalcTextureAnmIndexMerge;
			break;
		case 4:
			unk24[unk8B++] = &vc.mDrawCalcTextureAnmIndexRandom;
			break;
		}
	}

	if (mDrawCtx.mExtraShape != nullptr
	    && mDrawCtx.mExtraShape->isEnableScale()) {

		if (mDrawCtx.mExtraShape->isEnableScaleAnmX()) {
			if (mDrawCtx.mExtraShape->getAnmTypeX())
				unk48[unk8D++] = &vc.mDrawCalcScaleAnmTimingReverseX;
			else
				unk48[unk8D++] = &vc.mDrawCalcScaleAnmTimingRepeatX;
		} else {
			unk48[unk8D++] = &vc.mDrawCalcScaleAnmTimingNormal;
		}

		if ((mDrawCtx.mExtraShape->isEnableScaleBySpeedX())
		    && mDrawCtx.mBaseShape->getType() != 1)
			unk48[unk8D++] = &vc.mDrawCalcScaleXBySpeed;
		else
			unk48[unk8D++] = &vc.mDrawCalcScaleX;

		if (mDrawCtx.mBaseShape->getType() != 0) {
			if (!mDrawCtx.mExtraShape->isDiffXY()) {
				unk48[unk8D++] = &vc.mDrawCalcScaleCopyX2Y;
			} else {
				if (mDrawCtx.mExtraShape->isEnableScaleAnmY()) {
					if (mDrawCtx.mExtraShape->getAnmTypeY()) {
						unk48[unk8D++] = &vc.mDrawCalcScaleAnmTimingReverseY;
					} else {
						unk48[unk8D++] = &vc.mDrawCalcScaleAnmTimingRepeatY;
					}
				} else {
					if (mDrawCtx.mExtraShape->isEnableScaleAnmX()) {
						unk48[unk8D++] = &vc.mDrawCalcScaleAnmTimingNormal;
					}
				}

				if (mDrawCtx.mExtraShape->isEnableScaleBySpeedY()) {
					unk48[unk8D++] = &vc.mDrawCalcScaleYBySpeed;
				} else {
					unk48[unk8D++] = &vc.mDrawCalcScaleY;
				}
			}
		}
	}

	if (!mDrawCtx.mBaseShape->isEnableGlobalColAnm()) {
		if (flags.unk4 || flags.unk8) {
			switch (mDrawCtx.mBaseShape->getColorRegAnmType()) {
			case 0:
				unk48[unk8D++] = &vc.mDrawCalcColorAnmFrameNormal;
				break;
			case 1:
				unk48[unk8D++] = &vc.mDrawCalcColorAnmFrameRepeat;
				break;
			case 2:
				unk48[unk8D++] = &vc.mDrawCalcColorAnmFrameReverse;
				break;
			case 3:
				unk48[unk8D++] = &vc.mDrawCalcColorAnmFrameMerge;
				break;
			case 4:
				unk48[unk8D++] = &vc.mDrawCalcColorAnmFrameRandom;
				break;
			}

			if (flags.unk4)
				unk48[unk8D++] = &vc.mDrawCalcColorPrm;

			if (flags.unk8)
				unk48[unk8D++] = &vc.mDrawCalcColorEnv;
		}
	} else {
		unk48[unk8D++] = &vc.mDrawCalcColorCopyFromEmitter;
	}

	if (flags.unk14 && !flags.unkC) {
		unk48[unk8D++] = &vc.mDrawCalcAlpha;
		if (mDrawCtx.mExtraShape->isEnableSinWave()) {
			switch (mDrawCtx.mExtraShape->unk79) {
			case 0:
				unk48[unk8D++] = &vc.mDrawCalcAlphaFlickNrmSin;
				break;
			case 1:
				unk48[unk8D++] = &vc.mDrawCalcAlphaFlickAddSin;
				break;
			case 2:
				unk48[unk8D++] = &vc.mDrawCalcAlphaFlickMultSin;
				break;
			}
		}
	}

	if (!mDrawCtx.mBaseShape->textureIsEmpty()
	    && mDrawCtx.mBaseShape->isEnableTextureAnm()
	    && !mDrawCtx.mBaseShape->isEnableGlobalTexAnm()) {
		switch (mDrawCtx.mBaseShape->getTextureAnmType()) {
		case 0:
			unk48[unk8D++] = &vc.mDrawCalcTextureAnmIndexNormal;
			break;
		case 1:
			unk48[unk8D++] = &vc.mDrawCalcTextureAnmIndexRepeat;
			break;
		case 2:
			unk48[unk8D++] = &vc.mDrawCalcTextureAnmIndexReverse;
			break;
		case 3:
			unk48[unk8D++] = &vc.mDrawCalcTextureAnmIndexMerge;
			break;
		case 4:
			unk48[unk8D++] = &vc.mDrawCalcTextureAnmIndexRandom;
			break;
		}
	}

	if (mDrawCtx.mSweepShape) {
		if (mDrawCtx.mSweepShape->isEnableAlphaOut())
			unk80[unk8F++] = &vc.mDrawCalcChildAlphaOut;

		if (mDrawCtx.mSweepShape->isEnableScaleOut())
			unk80[unk8F++] = &vc.mDrawCalcChildScaleOut;
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

	f32 sx    = mDrawCtx.mBaseEmitter->unk174.x;
	f32 sy    = mDrawCtx.mBaseEmitter->unk174.y;
	cb.unk4.x = 25.0f * mDrawCtx.mBaseShape->getBaseSizeX() * sx;
	cb.unk4.y = 25.0f * mDrawCtx.mBaseShape->getBaseSizeY() * sy;

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
		unkC0 = mDrawCtx.mTexIndices[mDrawCtx.mBaseShape->getTextureIndex()];

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

	f32 sx = mDrawCtx.mBaseEmitter->unk174.x;
	f32 sy = mDrawCtx.mBaseEmitter->unk174.y;
	if (!mDrawCtx.mSweepShape->isInheritedScale()) {
		cb.unk4.x = 25.0f * mDrawCtx.mSweepShape->getScaleX() * sx;
		cb.unk4.y = 25.0f * mDrawCtx.mSweepShape->getScaleY() * sy;
	} else {
		cb.unk4.x = 25.0f * mDrawCtx.mBaseShape->getBaseSizeX() * sx;
		cb.unk4.y = 25.0f * mDrawCtx.mBaseShape->getBaseSizeY() * sy;
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

	for (int i = 0; i < unk89; ++i)
		unk4[i]->exec(&mDrawCtx);

	JSUList<JPABaseParticle>* particles
	    = mDrawCtx.mBaseEmitter->getParticleList();
	if (mDrawCtx.mBaseShape->getListOrder()) {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getFirst(); link; link = link->getNext()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < unk8C; ++i)
				unk34[i]->exec(&mDrawCtx, particle);
		}
	} else {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getLast(); link; link = link->getPrev()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < unk8C; ++i)
				unk34[i]->exec(&mDrawCtx, particle);
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

	// TODO: figure out how to use JPADraw::loadTexture
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

	for (int i = 0; i < unk8A; ++i)
		unk18[i]->exec(&mDrawCtx);

	JSUList<JPABaseParticle>* particles
	    = mDrawCtx.mBaseEmitter->getChildParticleList();
	if (mDrawCtx.mBaseShape->getListOrder()) {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getFirst(); link; link = link->getNext()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < unk8E; ++i)
				unk70[i]->exec(&mDrawCtx, particle);
		}
	} else {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getLast(); link; link = link->getPrev()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < unk8E; ++i)
				unk70[i]->exec(&mDrawCtx, particle);
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

	GXSetAlphaCompare(GX_GEQUAL, mDrawCtx.mBaseEmitter->getAlpha(), GX_AOP_OR,
	                  GX_GEQUAL, mDrawCtx.mBaseEmitter->getAlpha());
	GXSetAlphaUpdate(GX_FALSE);
	GXSetColorUpdate(GX_FALSE);
	GXSetCullMode(GX_CULL_NONE);
	if (mDrawCtx.mBaseShape->isClipOn()) {
		GXSetClipMode(GX_CLIP_ENABLE);
		GXSetMisc(GX_MT_XF_FLUSH, 8);
	} else {
		GXSetClipMode(GX_CLIP_DISABLE);
	}

	for (int i = 0; i < unk89; ++i)
		unk4[i]->exec(&mDrawCtx);

	JSUList<JPABaseParticle>* particles
	    = mDrawCtx.mBaseEmitter->getParticleList();
	if (mDrawCtx.mBaseShape->getListOrder()) {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getFirst(); link; link = link->getNext()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < unk8C; ++i)
				unk34[i]->exec(&mDrawCtx, particle);
		}
	} else {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getLast(); link; link = link->getPrev()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < unk8C; ++i)
				unk34[i]->exec(&mDrawCtx, particle);
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

	// TODO: figure out how to use JPADraw::loadTexture
	if (mDrawCtx.mBaseShape->textureIsEmpty()) {
		mDrawCtx.mTexResource->loadDefaultTexture(GX_TEXMAP0);
	} else {
		mDrawCtx.mTexResource->load(
		    mDrawCtx.mTexIndices[mDrawCtx.mSweepShape->getTextureIndex()],
		    GX_TEXMAP0);
	}

	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetZCompLoc(GX_FALSE);
	GXSetAlphaCompare(GX_GEQUAL, mDrawCtx.mBaseEmitter->getAlpha(), GX_AOP_OR,
	                  GX_GEQUAL, mDrawCtx.mBaseEmitter->getAlpha());
	GXSetAlphaUpdate(GX_FALSE);
	GXSetColorUpdate(GX_FALSE);
	GXSetCullMode(GX_CULL_NONE);

	if (mDrawCtx.mSweepShape->isClipOn()) {
		GXSetClipMode(GX_CLIP_ENABLE);
		GXSetMisc(GX_MT_XF_FLUSH, 8);
	} else {
		GXSetClipMode(GX_CLIP_DISABLE);
	}

	for (int i = 0; i < unk8A; ++i)
		unk18[i]->exec(&mDrawCtx);

	JSUList<JPABaseParticle>* particles
	    = mDrawCtx.mBaseEmitter->getChildParticleList();
	if (mDrawCtx.mBaseShape->getListOrder()) {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getFirst(); link; link = link->getNext()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < unk8E; ++i)
				unk70[i]->exec(&mDrawCtx, particle);
		}
	} else {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getLast(); link; link = link->getPrev()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < unk8E; ++i)
				unk70[i]->exec(&mDrawCtx, particle);
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

void JPADraw::getIndTextureID() { }

void JPADraw::getIndSubTextureID() { }

void JPADraw::getSecondTextureID() { }

void JPADraw::loadYBBMtx(MtxPtr mtx)
{
	JGeometry::TVec3<f32> v;
	v.set(0.0f, mtx[1][1], mtx[2][1]);
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
