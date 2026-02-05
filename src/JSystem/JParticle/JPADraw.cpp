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

BOOL JPADraw::initialize(JPABaseEmitter* param_1, JPATextureResource* param_2)
{
	JPADrawContext::pcb = &cb;

	mDrawCtx.mBaseEmitter = param_1;
	mDrawCtx.mBaseShape   = mDrawCtx.mBaseEmitter->mEmitterDataBlockInfo->unk4;
	mDrawCtx.mExtraShape  = mDrawCtx.mBaseEmitter->mEmitterDataBlockInfo->unk8;
	mDrawCtx.mSweepShape  = mDrawCtx.mBaseEmitter->mEmitterDataBlockInfo->unkC;
	mDrawCtx.mExTexShape  = mDrawCtx.mBaseEmitter->mEmitterDataBlockInfo->unk10;
	mDrawCtx.unk14        = this;
	mDrawCtx.mTexResource = param_2;
	mDrawCtx.mTexIndices  = mDrawCtx.mBaseEmitter->mEmitterDataBlockInfo->unk1C;
	mDrawCtx.unk18        = &mDrawCtx.mBaseEmitter->mParticleList;

	unkC2 = 0;
	unkB4 = 1.0f;

	GXColor white = { 0xff, 0xff, 0xff, 0xff };

	if (mDrawCtx.mBaseShape->isEnablePrm()) {
		if (!(mDrawCtx.mBaseShape->isEnablePrmAnm()))
			mPrmColor = mDrawCtx.mBaseShape->getPrmColor();
	} else {
		mPrmColor = white;
	}

	if (mDrawCtx.mBaseShape->isEnableEnv()) {
		if (!(mDrawCtx.mBaseShape->isEnableEnvAnm()))
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

void JPADraw::draw(MtxPtr param_1)
{
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, 0);
	GXSetCurrentMtx(0);
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
	cb.mEnvColor.r = be->unk184;
	cb.mEnvColor.g = be->unk185;
	cb.mEnvColor.b = be->unk186;
	cb.mEnvColor.a = 0xff;

	// TODO: wut?!
	cb.unk34 = (Mtx44*)param_1;

	cb.mSetupTev.setupTev(mDrawCtx.mBaseShape, mDrawCtx.mExTexShape);

	for (int i = 0; i < unk88; ++i)
		unk0[i]->exec(&mDrawCtx);

	if ((int)mDrawCtx.mBaseShape->unk7A != 0)
		zDraw();

	unkC2 &= ~0x1;

	if ((mDrawCtx.mBaseShape->mFlags >> 1 & 1) == 1UL
	    && mDrawCtx.mSweepShape != nullptr)
		drawChild();

	drawParticle();

	if ((mDrawCtx.mBaseShape->mFlags >> 1 & 1) == 0UL
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
	JPAEmitterInfo* info  = JPAGetEmitterInfoPtr();

	// TODO: something wrong here
	f32* v         = info->unk48[1];
	params->unk0.x = v[0];
	params->unk0.y = v[1];
	params->unk0.z = v[2];

	params->unk2C = mPrmColor;
	params->unk30 = mEnvColor;
	params->unk20 = 1.0f;

	params->unk28 = (int)(mDrawCtx.mBaseEmitter->getRandomF()
	                      * mDrawCtx.mBaseShape->getLoopOffset());

	if (mDrawCtx.mExtraShape != nullptr) {
		if (mDrawCtx.mExtraShape->isEnableRotate() != 0) {
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

void JPADraw::initChild(JPABaseParticle* param_1, JPABaseParticle* param_2)
{
	JPADrawParams* params1 = param_1->getDrawParamPPtr();
	JPADrawParams* params2 = param_2->getDrawParamCPtr();

	params2->unk0 = params1->unk0;

	params2->unk20 = 1.0f;

	if (mDrawCtx.mSweepShape->isInheritedRGB()) {
		f32 f = mDrawCtx.mSweepShape->getInheritRGB();

		params2->unk2C.r = params1->unk2C.r * f;
		params2->unk2C.g = params1->unk2C.g * f;
		params2->unk2C.b = params1->unk2C.b * f;

		params2->unk30.r = params1->unk30.r * f;
		params2->unk30.g = params1->unk30.g * f;
		params2->unk30.b = params1->unk30.b * f;
	} else {
		params2->unk2C = mDrawCtx.mSweepShape->getPrm();
		params2->unk30 = mDrawCtx.mSweepShape->getEnv();
	}

	if (mDrawCtx.mSweepShape->isInheritedAlpha()) {
		f32 f = params1->unk20 * mDrawCtx.mSweepShape->getInheritAlpha();

		params2->unk2C.a = params1->unk2C.a * f;
		params2->unk30.a = params1->unk30.a * f;
	} else {
		params2->unk2C.a = mDrawCtx.mSweepShape->getPrmAlpha();
		params2->unk30.a = mDrawCtx.mSweepShape->getEnvAlpha();
	}

	if (mDrawCtx.mSweepShape->isInheritedScale()) {
		f32 f = mDrawCtx.mSweepShape->getInheritScale();

		params2->unk10 = params2->unkC = f * params1->unk10;
		params2->unk14 = params2->unk24 = f * params1->unk14;
	} else {
		params2->unk24 = 1.0f;
		params2->unk14 = 1.0f;
		params2->unkC  = 1.0f;
		params2->unk10 = 1.0f;
	}

	params2->unk34 = params1->unk34;
	if (mDrawCtx.mSweepShape->isEnableRotate() != 0) {
		params2->unk36 = mDrawCtx.mSweepShape->getRotateSpeed() * 32768.0f;
	} else {
		params2->unk36 = 0;
	}
}

const ResTIMG* JPADraw::swapImage(const ResTIMG* param_1, s16 param_2)
{
	if (param_2 < 0)
		return nullptr;

	return mDrawCtx.mTexResource->swapImage(param_1,
	                                        mDrawCtx.getTexIdx(param_2));
}

void JPADraw::loadTexture(u8 idx, GXTexMapID map_id) { }

void JPADraw::setDrawExecVisitorsBeforeCB(
    const JPADraw::JPADrawVisitorDefFlags& flags)
{
	if (mDrawCtx.mExTexShape)
		unk0[unk88++] = &vc.mDrawExecLoadExTex;

	if (flags.unk10) {
		unk4[unk89++] = &vc.mDrawExecGenIdtMtx;
	} else if ((int)mDrawCtx.mBaseShape->unk7B != 0) {
		if (mDrawCtx.mBaseShape->unk87)
			unk4[unk89++] = &vc.mDrawExecGenPrjTexMtx;
		else
			unk4[unk89++] = &vc.mDrawExecGenPrjMtx;
	} else if (mDrawCtx.mBaseShape->unk87) {
		if (flags.unkC)
			unk4[unk89++] = &vc.mDrawExecSetTexMtx;
		else
			unk4[unk89++] = &vc.mDrawExecGenTexMtx0;
	} else {
		unk4[unk89++] = &vc.mDrawExecGenIdtMtx;
	}

	if (mDrawCtx.mBaseShape->unk68 & 2) {
		if (flags.unkC || !flags.unk14) {
			unk4[unk89++] = &vc.mDrawExecRegisterColorEmitterPE;
		} else if (flags.unk14) {
			unk4[unk89++] = &vc.mDrawExecRegisterColorEmitterE;
		}
	} else {
		if ((!(mDrawCtx.mBaseShape->unk83 & 1)
		     || !(mDrawCtx.mBaseShape->unk83 & 2))
		    && flags.unk14 == 0) {
			if (!(mDrawCtx.mBaseShape->unk84 & 1)
			    || !(mDrawCtx.mBaseShape->unk84 & 2)) {
				unk4[unk89++] = &vc.mDrawExecRegisterColorEmitterPE;
			} else {
				unk4[unk89++] = &vc.mDrawExecRegisterColorEmitterP;
			}
		} else if ((mDrawCtx.mBaseShape->unk84 & 1) == 0
		           || (mDrawCtx.mBaseShape->unk84 & 2) == 0) {
			unk4[unk89++] = &vc.mDrawExecRegisterColorEmitterE;
		}
	}

	if (mDrawCtx.mBaseShape->unk81 == 0
	    && (mDrawCtx.mBaseShape->unk80 == 0
	        || (mDrawCtx.mBaseShape->unk68 & 1))) {
		unk4[unk89++] = &vc.mDrawExecLoadTexture;
	} else if (mDrawCtx.mBaseShape->unk81 != 0) {
		unk4[unk89++] = &vc.mDrawExecLoadDefaultTexture;
	}

	if (mDrawCtx.mSweepShape != nullptr && (int)mDrawCtx.mSweepShape->unk4B == 0
	    && !(mDrawCtx.mSweepShape->unk4E & 2)
	    && !(mDrawCtx.mSweepShape->unk4E & 4)) {
		unk18[unk8A++] = &vc.mDrawExecRegisterColorChildPE;
	}

	if (!(mDrawCtx.mBaseShape->unk68 & 2)) {
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

	if (mDrawCtx.mBaseShape->unk81 == 0 && mDrawCtx.mBaseShape->unk80 != 0
	    && !(mDrawCtx.mBaseShape->unk68 & 1)) {
		unk34[unk8C++] = &vc.mDrawExecLoadTexture;
	}

	switch (mDrawCtx.mBaseShape->mType) {
	case 0:
		if (mDrawCtx.mExtraShape && (mDrawCtx.mExtraShape->unk7E & 1))
			unk34[unk8C++] = &vc.mDrawExecSetPointSize;
		break;
	case 1:
		if (mDrawCtx.mExtraShape && (mDrawCtx.mExtraShape->unk7E & 1))
			unk34[unk8C++] = &vc.mDrawExecSetLineWidth;
		break;

	case 2:
	case 3:
	case 4:
	case 7:
	case 8:
	case 9:
	case 10:
		if (mDrawCtx.mBaseShape->unk87 && (int)mDrawCtx.mBaseShape->unk7B == 0)
			unk34[unk8C++] = &vc.mDrawExecSetTexMtx;
		break;
	}

	if (mDrawCtx.mSweepShape) {
		if ((int)mDrawCtx.mSweepShape->unk4B != 0
		    || (mDrawCtx.mSweepShape->unk4E & 2)
		    || (mDrawCtx.mSweepShape->unk4E & 4))
			unk70[unk8E++] = &vc.mDrawExecRegisterPrmCEnv;

		switch (mDrawCtx.mSweepShape->mType) {
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
		switch (mDrawCtx.mBaseShape->mType) {
		case 0:
			unk34[unk8C++] = &vc.mDrawExecPoint;
			break;
		case 1:
			unk34[unk8C++] = &vc.mDrawExecLine;
			break;

		case 2:
			if (mDrawCtx.mExtraShape && (int)mDrawCtx.mExtraShape->unk7F != 0)
				unk34[unk8C++] = &vc.mDrawExecRotBillBoard;
			else
				unk34[unk8C++] = &vc.mDrawExecBillBoard;
			break;
		case 3:
			if (mDrawCtx.mExtraShape && (int)mDrawCtx.mExtraShape->unk7F != 0)
				unk34[unk8C++] = &vc.mDrawExecRotDirectional;
			else
				unk34[unk8C++] = &vc.mDrawExecDirectional;
			break;
		case 4:
			if (mDrawCtx.mExtraShape && (int)mDrawCtx.mExtraShape->unk7F != 0)
				unk34[unk8C++] = &vc.mDrawExecRotDirectionalCross;
			else
				unk34[unk8C++] = &vc.mDrawExecDirectionalCross;
			break;
		case 10:
			if (mDrawCtx.mExtraShape && (int)mDrawCtx.mExtraShape->unk7F != 0)
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
		switch (mDrawCtx.mSweepShape->mType) {
		case 0:
			unk70[unk8E++] = &vc.mDrawExecPoint;
			break;
		case 1:
			unk70[unk8E++] = &vc.mDrawExecLine;
			break;

		case 2:
			if ((mDrawCtx.mExtraShape && (int)mDrawCtx.mExtraShape->unk7F != 0)
			    || (int)mDrawCtx.mSweepShape->unk4D != 0)
				unk70[unk8E++] = &vc.mDrawExecRotBillBoard;
			else
				unk70[unk8E++] = &vc.mDrawExecBillBoard;
			break;
		case 3:
			if ((mDrawCtx.mExtraShape && (int)mDrawCtx.mExtraShape->unk7F != 0)
			    || (int)mDrawCtx.mSweepShape->unk4D != 0)
				unk70[unk8E++] = &vc.mDrawExecRotDirectional;
			else
				unk70[unk8E++] = &vc.mDrawExecDirectional;
			break;
		case 4:
			if ((mDrawCtx.mExtraShape && (int)mDrawCtx.mExtraShape->unk7F != 0)
			    || (int)mDrawCtx.mSweepShape->unk4D != 0)
				unk70[unk8E++] = &vc.mDrawExecRotDirectionalCross;
			else
				unk70[unk8E++] = &vc.mDrawExecDirectionalCross;
			break;
		case 10:
			if ((mDrawCtx.mExtraShape && (int)mDrawCtx.mExtraShape->unk7F != 0)
			    || (int)mDrawCtx.mSweepShape->unk4D != 0)
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
	if ((mDrawCtx.mBaseShape->unk68 & 2)
	    && (flags.unk4 != 0 || flags.unk8 != 0)) {
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

		if (flags.unk4 != 0)
			unk24[unk8B++] = &vc.mDrawCalcColorPrm;

		if (flags.unk8 != 0)
			unk24[unk8B++] = &vc.mDrawCalcColorEnv;
	}

	if (mDrawCtx.mBaseShape->unk81 == 0 && mDrawCtx.mBaseShape->unk80 != 0
	    && (mDrawCtx.mBaseShape->unk68 & 1)) {
		switch (mDrawCtx.mBaseShape->unk7D) {
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

	if (mDrawCtx.mExtraShape != nullptr && (mDrawCtx.mExtraShape->unk7E & 1)) {

		if (mDrawCtx.mExtraShape->unk7E & 8) {
			if (mDrawCtx.mExtraShape->unk7D)
				unk48[unk8D++] = &vc.mDrawCalcScaleAnmTimingReverseX;
			else
				unk48[unk8D++] = &vc.mDrawCalcScaleAnmTimingRepeatX;
		} else {
			unk48[unk8D++] = &vc.mDrawCalcScaleAnmTimingNormal;
		}

		if ((mDrawCtx.mExtraShape->unk7E & 0x20)
		    && mDrawCtx.mBaseShape->mType != 1)
			unk48[unk8D++] = &vc.mDrawCalcScaleXBySpeed;
		else
			unk48[unk8D++] = &vc.mDrawCalcScaleX;

		if (mDrawCtx.mBaseShape->mType != 0) {
			if ((mDrawCtx.mExtraShape->unk7E & 2) == 0) {
				unk48[unk8D++] = &vc.mDrawCalcScaleCopyX2Y;
			} else {
				if (mDrawCtx.mExtraShape->unk7E & 4) {
					if (mDrawCtx.mExtraShape->unk7B) {
						unk48[unk8D++] = &vc.mDrawCalcScaleAnmTimingReverseY;
					} else {
						unk48[unk8D++] = &vc.mDrawCalcScaleAnmTimingRepeatY;
					}
				} else {
					if ((mDrawCtx.mExtraShape->unk7E & 8) != 0) {
						unk48[unk8D++] = &vc.mDrawCalcScaleAnmTimingNormal;
					}
				}

				if (mDrawCtx.mExtraShape->unk7E & 0x10) {
					unk48[unk8D++] = &vc.mDrawCalcScaleYBySpeed;
				} else {
					unk48[unk8D++] = &vc.mDrawCalcScaleY;
				}
			}
		}
	}

	if ((mDrawCtx.mBaseShape->unk68 & 2) == 0) {
		if (flags.unk4 || flags.unk8) {
			switch (mDrawCtx.mBaseShape->unk82) {
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
		if (mDrawCtx.mExtraShape->unk78 & 2) {
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

	if (mDrawCtx.mBaseShape->unk81 == 0 && mDrawCtx.mBaseShape->unk80 != 0
	    && (mDrawCtx.mBaseShape->unk68 & 1) == 0) {
		switch (mDrawCtx.mBaseShape->unk7D) {
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
		if ((int)mDrawCtx.mSweepShape->unk4B != 0)
			unk80[unk8F++] = &vc.mDrawCalcChildAlphaOut;

		if ((int)mDrawCtx.mSweepShape->unk4A != 0)
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
		loadYBBMtx(*cb.unk34);
		break;
	default:
		MTXCopy(*cb.unk34, cb.unk68);
		break;
	}

	GXLoadPosMtxImm(cb.unk68, 0);

	// TODO: TVec2 arithmetic?
	f32 a   = mDrawCtx.mBaseEmitter->unk174.x;
	f32 b   = mDrawCtx.mBaseEmitter->unk174.y;
	cb.unk4 = a * (mDrawCtx.mBaseShape->mBaseSizeX * 25.0f);
	cb.unk8 = b * (mDrawCtx.mBaseShape->mBaseSizeY * 25.0f);

	if (mDrawCtx.mBaseShape->getType() == 0) {
		cb.unk4 *= 1.02f;
		cb.unk8 *= 1.02f;
	} else if (mDrawCtx.mBaseShape->getType() == 1) {
		cb.unk4 *= 1.02f;
		cb.unk8 *= 0.4f;
	}

	if (mDrawCtx.mExtraShape != nullptr
	    && mDrawCtx.mExtraShape->isEnableScale()) {
		cb.unkC  = cb.unk4 * (mDrawCtx.mExtraShape->unk7C - 1.0f);
		cb.unk10 = cb.unk8 * (mDrawCtx.mExtraShape->unk7A - 1.0f);
	} else {
		cb.unk10 = 0.0f;
		cb.unkC  = 0.0f;
	}

	f32 x = mDrawCtx.mBaseShape->getTilingX();
	f32 y = mDrawCtx.mBaseShape->getTilingY();

	cb.mTexCoords[0].x = 0.0f;
	cb.mTexCoords[0].y = 0.0f;
	cb.mTexCoords[1].x = x;
	cb.mTexCoords[1].y = 0.0f;

	cb.mTexCoords[2].x = x;
	cb.mTexCoords[2].y = y;

	cb.mTexCoords[3].x = 0.0f;
	cb.mTexCoords[3].y = y;

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
		loadYBBMtx(*cb.unk34);
		break;
	default:
		MTXCopy(*cb.unk34, cb.unk68);
		break;
	}

	GXLoadPosMtxImm(cb.unk68, 0);

	f32 fVar3 = mDrawCtx.mBaseEmitter->unk174.x;
	f32 fVar4 = mDrawCtx.mBaseEmitter->unk174.y;
	if (!mDrawCtx.mSweepShape->isInheritedScale()) {
		cb.unk4 = fVar3 * (25.0f * mDrawCtx.mSweepShape->getScaleX());
		cb.unk8 = fVar4 * (25.0f * mDrawCtx.mSweepShape->getScaleY());
	} else {
		cb.unk4 = fVar3 * (25.0f * mDrawCtx.mBaseShape->getBaseSizeX());
		cb.unk8 = fVar4 * (25.0f * mDrawCtx.mBaseShape->getBaseSizeY());
	}

	if (mDrawCtx.mSweepShape->getType() == 0) {
		cb.unk4 *= 1.02f;
		cb.unk8 *= 1.02f;
	} else if (mDrawCtx.mSweepShape->getType() == 1) {
		cb.unk4 *= 1.02f;
		cb.unk8 *= 0.4f;
	}

	cb.unkC = cb.unk10 = 0.0f;

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
	mDrawCtx.unk18 = &mDrawCtx.mBaseEmitter->mParticleList;

	GXSetPointSize(cb.unk4, GX_TO_ONE);
	GXSetLineWidth(cb.unk4, GX_TO_ONE);
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
	mDrawCtx.unk18 = &mDrawCtx.mBaseEmitter->mChildParticleList;
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

	GXSetPointSize(cb.unk4, GX_TO_ONE);
	GXSetLineWidth(cb.unk4, GX_TO_ONE);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetZCompLoc(GX_FALSE);

	GXSetAlphaCompare(GX_GEQUAL, mDrawCtx.mBaseEmitter->unk180.a, GX_AOP_OR,
	                  GX_GEQUAL, mDrawCtx.mBaseEmitter->unk180.a);
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
	GXSetAlphaCompare(GX_GEQUAL, mDrawCtx.mBaseEmitter->unk180.a, GX_AOP_OR,
	                  GX_GEQUAL, mDrawCtx.mBaseEmitter->unk180.a);
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
		if (i < mDrawCtx.mBaseShape->mTextureAnmKeyNum)
			result = mDrawCtx.mBaseShape->mTextureIndices[i];
	} else {
		if (i == 0)
			result = mDrawCtx.mBaseShape->mTextureIndex;
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
