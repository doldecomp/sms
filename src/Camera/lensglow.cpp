#include <Camera/LensGlow.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DAnmLoader.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JMath.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Camera/Camera.hpp>
#include <Camera/CameraMarioData.hpp>
#include <Camera/SunMgr.hpp>
#include <Camera/cameralib.hpp>
#include <Camera/SunModel.hpp>
#include <System/Resolution.hpp>
#include <stdio.h>

static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

TLensGlow::TLensGlow(bool param_1, const char* name)
    : JDrama::TViewObj(name)
    , unk10(nullptr)
    , unk14(nullptr)
    , unk18(nullptr)
    , unk1C()
    , unk30(nullptr)
    , unk34()
    , unk48(0.0f)
    , unk4C(0.0f)
    , unk50(0.5f)
    , unk54(0.5f)
    , unk58(0.5f)
    , unk5C(-1)
    , unk5D(2)
    , unk68(10.0f)
    , unk6C(10.0f)
    , unk70(0.1f)
    , unk80(0.0f)
    , unk84(0.0f)
    , unk88(0.0f)
    , unk8C(0.0f)
    , unk90(0.1f)
    , unk94(1.5f)
{
	const char* base = cSunVolumeName;
	if (param_1) {
		base  = cSunsetVolumeName;
		unk68 = 15.0f;
		unk6C = 10.0f;
	}

	// Dead 4-byte local, the same leftover declaration as in
	// TLensFlare::TLensFlare and TSunModel::load (the three sibling functions
	// that build a path out of cSunVolumeName): retail puts `buf` at 0x48 and
	// leaves one word between it and the outgoing-argument area, ours lands at
	// 0x44 without it. An assigned local is register-allocated and reserves
	// nothing, so it was never written to; the original name is unrecoverable.
	char buf[0x100];
	int pathLen;

	snprintf(buf, sizeof(buf), "%s/%s", base, "glow.bmd");
	unk10 = J3DModelLoaderDataBase::load(JKRGetResource(buf),
	                                     J3DMLF_MaterialPEFull
	                                         | (2 << J3DMLF_TevStageNumShift));
	unk14 = new J3DModel(unk10, 0, 1);

	snprintf(buf, sizeof(buf), "%s/%s", base, "glow.btk");
	unk18
	    = (J3DAnmTextureSRTKey*)J3DAnmLoaderDataBase::load(JKRGetResource(buf));
	unk18->searchUpdateMaterialID(unk10);

	snprintf(buf, sizeof(buf), "%s/%s", base, "glow.brk");
	unk30 = (J3DAnmTevRegKey*)J3DAnmLoaderDataBase::load(JKRGetResource(buf));
	unk30->searchUpdateMaterialID(unk10);

	u16 i;
	int matCount = unk10->getMaterialNum();
	for (i = 0; i < matCount; ++i) {
		J3DMaterialAnm* anm = new J3DMaterialAnm;
		unk10->getMaterialNodePointer(i)->change();
		unk10->getMaterialNodePointer(i)->setMaterialAnm(anm);
	}

	unk10->entryTexMtxAnimator(unk18);
	unk10->entryTevRegAnimator(unk30);

	unk1C.init(unk18->getFrameMax());
	unk1C.setRate(SMSGetAnmFrameRate());
	unk1C.setAttribute(J3DFrameCtrl::ATTR_LOOP);

	unk34.init(unk30->getFrameMax());
	unk34.setRate(SMSGetAnmFrameRate());
	unk34.setAttribute(J3DFrameCtrl::ATTR_LOOP);

	unk74.x = 500.0f;
	unk74.y = 500.0f;
	unk74.z = 0.0f;
	unk60 = unk64 = unk6C;
}

// TODO: 99.0%, and the whole residue is dead low region: frame 0x120 against
// retail's 0x178. Every referenced slot in the lower group is exactly 0x60
// higher in retail (`avg` 0x6c -> 0xcc, `mtx` 0x74 -> 0xd4, `scaleV` 0xa4 ->
// 0x104) and every slot in the upper group exactly 0x58 higher (the three
// int -> float magic-double pairs 0xc8/0xd0/0xd8 -> 0x120/0x128/0x130, the
// stmw 0xe0 -> 0x138), so retail has **96 bytes more inline-expansion pool**
// below `avg` and **8 bytes less** slack between `scaleV` and the conversion
// temporaries. `volatile char trash[88]` reaches 0x178 and 99.1% with no new
// instruction, confirming the body.
// None of the four CLB helpers can be the carrier: CLBLinearInbetween,
// CLBEaseOutInbetween, CLBChaseDecrease and CLBCalcScaleTranslateMatrix are
// all real `bl`s here, and the carrier test needs a callee with no matching
// out-of-line copy. That leaves the inlined ones, and all of them live in
// shared headers: `TSunModel::isInBounds` (SunModel.hpp -- sunmgr.cpp
// measured one dead 48-byte non-trivial local there taking this frame
// 0x120 -> 0x150), `TSunModel::getUnk191/getUnk194`, the TVec2 accumulate
// loop and the J3DMaterial colour accessors. `J3DFrameCtrl::update` is *not*
// one of them: retail `bl`s it twice here (0x8002DD54, 0x8002DD5C), so it
// carries no pool, and the "48 + 48" arithmetic that used to fit is gone.
// A dead 48-byte *trivial* local (a bare `Mtx`) in isInBounds is worth zero,
// because MWCC drops an unused POD array in an inlined callee; only a
// non-trivial object moves the frame, and no such object is nameable for a
// bounds predicate. Recorded at the declaration; header round 21 rejected it.
// With the frame padded, three independent residues remain, all of them
// zero-frame: (a) an f0/f1 swap inside the isInBounds expansion plus an r4/r5
// swap in its position walk; (b) `gpSunModel->getUnk194()` lands in f29 in
// retail and f31 in ours, which rotates f27/f29/f30/f31 through the rest of
// the function; (c) the first two conversion temporaries are swapped (retail
// uses 0x128 then 0x130, we use 0x130 then 0x128). Measured with the padding
// in place and rejected: swapping the operands of
// `(f32)(thing - unk5D) * (1.0f / (f32)(17 - unk5D))` (byte-identical) and
// spelling it as a real division (95.3%, five extra instructions).
// Batch 151: the carrier family is now confirmed productive elsewhere --
// TMapCollisionData::polygonIsInGrid closed on exactly this mechanism, three
// reserved `TVec3` locals per expansion of two UNUSED helpers -- so the 96
// bytes here are very likely reserved locals of the inlined `SunModel.hpp`
// bodies, at 12 bytes per `TVec3` (or 8 per `TVec2`) per expansion. This unit
// cannot be closed without editing that shared header, which is out of scope
// for a unit agent, and a TU-local wrapper cannot stand in: `isInBounds`
// expands once, so parking 96 bytes there would mean eight dead vectors in one
// forwarder. The exact requirement for whoever owns SunModel.hpp is +96 bytes
// of reserved locals across the expansions below `avg` **and** -8 bytes of
// slack between `scaleV` and the conversion temporaries; two dead 48-byte
// non-trivial locals in `isInBounds` alone would give 0x180, eight over.
void TLensGlow::perform(u32 cue, JDrama::TGraphics* graphics)
{
	bool inBounds = false;
	if (gpCameraMario->isMarioIndoor()) {
		inBounds = false;
	} else {
		inBounds = gpSunModel->isInBounds(unk94);
	}

	if (cue & CUE_MOVE) {
		f32 dispRatio = gpSunModel->getUnk194();
		u8 thing      = gpSunModel->getUnk191();

		if (thing <= unk5D) {
			unk4C = 0.0f;
		} else {
			f32 t   = ((f32)(thing - unk5D)) * (1.0f / (f32)(17 - unk5D));
			f32 lin = CLBLinearInbetween(0.0f, 1.0f, t);
			unk4C   = CLBEaseOutInbetween(0.0f, (f32)unk5C, lin);
		}

		f32 ratio;
		if (unk48 < unk4C) {
			ratio = unk50;
		} else if (dispRatio == 0.0f)
			ratio = unk58;
		else
			ratio = unk54;

		CLBChaseDecrease(&unk48, unk4C, ratio, 0.0f);

		// target scale based on sun model's dispersion
		unk64 = CLBLinearInbetween(unk6C * 0.002f, unk68 * 0.002f, dispRatio);
		CLBChaseDecrease(&unk60, unk64, unk70, 0.0f);

		// Compute base screen-space center

		f32 a  = gpSunModel->unkF8[0].x;
		f32 cx = (f32)(SMSGetGameRenderWidth() >> 1) * a;
		f32 b  = gpSunModel->unkF8[0].y;
		f32 cy = (f32)(SMSGetGameRenderHeight() >> 1) * b;

		if (thing == 0) {
			unk8C = 0.0f;
			unk88 = 0.0f;
		} else {
			if (dispRatio >= 0.5f) {
				unk8C = 0.0f;
				unk88 = 0.0f;
			} else {
				JGeometry::TVec2<f32> avg(0.0f, 0.0f);

				const JGeometry::TVec2<f32>* it2 = gpSunModel->unkF8;
				const bool* it1                  = gpSunModel->unk180;
				for (int i = 0; i < 17; ++i, ++it2, ++it1)
					if (*it1)
						avg += *it2;

				f32 avgx = avg.x * (1.0f / (f32)thing);
				f32 avgy = avg.y * (1.0f / (f32)thing);

				f32 tx = CLBLinearInbetween(avgx, gpSunModel->unkF8[0].x,
				                            dispRatio * 2.0f);
				f32 ty = CLBLinearInbetween(avgy, gpSunModel->unkF8[0].y,
				                            dispRatio * 2.0f);

				u16 w  = SMSGetGameRenderWidth();
				u16 h  = SMSGetGameRenderHeight();
				f32 ax = tx * (w >> 1);
				f32 ay = ty * (h >> 1);
				unk88  = ax - cx;
				unk8C  = ay - cy;
			}
		}

		CLBChaseDecrease(&unk80, unk88, unk90, 0.0f);
		CLBChaseDecrease(&unk84, unk8C, unk90, 0.0f);

		unk74.x = cx + unk80;
		unk74.y = cy + unk84;
	}

	if (cue & CUE_CALC_ANIM) {
		unk1C.update();
		unk34.update();

		if (inBounds) {
			JGeometry::TVec3<f32> scaleV(unk60, unk60, 1.0f);

			Mtx mtx;
			CLBCalcScaleTranslateMatrix(mtx, scaleV, unk74);
			unk14->setBaseTRMtx(mtx);
			unk14->calc();
		}
	}

	if ((cue & CUE_ENTRY) != 0 && inBounds) {
		int matCount = unk10->getMaterialNum();
		for (u16 i = 0; i < matCount; ++i) {
			J3DGXColorS10 c;
			c         = *unk10->getMaterialNodePointer(i)->getTevColor(0);
			c.color.a = (s16)unk48;
			unk10->getMaterialNodePointer(i)->setTevColor(0, &c);
		}

		unk18->setFrame(unk1C.getFrame());
		unk30->setFrame(unk34.getFrame());

		unk14->entry();
	}

	if ((cue & CUE_CALC_VIEW) && inBounds) {
		unk14->viewCalc();
	}
}
