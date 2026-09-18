#include <NPC/NpcBase.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DNode.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Camera/cameralib.hpp>
#include <Player/MarioAccess.hpp>

BOOL NPCNeckCallBack(J3DNode* param_1, int param_2)
{
	if (param_2 == 0) {
		if (gpCurrentNpc == nullptr)
			return FALSE;

		bool shouldRun = gpCurrentNpc->mNeckJointIndex != -1
		                         && !gpCurrentNpc->checkLiveFlag(
		                             LIVE_FLAG_HIDDEN | LIVE_FLAG_CLIPPED_OUT)
		                     ? true
		                     : false;

		if (shouldRun) {
			J3DJoint* joint = (J3DJoint*)param_1;
			MtxPtr currMtx  = j3dSys.getModel()->getAnmMtx(joint->getJntNo());

			s16 r30 = 0;
			s16 r28 = 0;

			s16 neckX = gpCurrentNpc->unk230->unk0;
			s16 neckY = gpCurrentNpc->unk230->unk2;

			bool useTracking = false;
			if (gpCurrentNpc->isNeedNeckStraight()) {
				r28 = 0;
				r30 = 0;
			} else {
				useTracking = true;
			}

			JGeometry::TVec3<f32> local_148;

			if (useTracking) {
				JGeometry::TVec3<f32> marioPos = SMS_GetMarioPos();
				marioPos.y += 85.0f;

				f32 marioY = marioPos.y;
				JGeometry::TVec3<f32> toMario(marioPos.x - currMtx[0][3],
				                              marioY - currMtx[1][3],
				                              marioPos.z - currMtx[2][3]);

				f32 dist2 = toMario.squared();
				if (dist2 > 0.001f
				    && dist2 < CLBSquared<f32>(gpCurrentNpc->mIndividualParams
				                                   ->mNeckTurnSearchDist.get())
				    && fabs(marioY - currMtx[1][3])
				           < gpCurrentNpc->mIndividualParams
				                 ->mNeckTurnSearchHeight.get()) {

					JGeometry::TVec3<f32> neckForward(
					    currMtx[0][1], currMtx[1][1], currMtx[2][1]);
					MsVECNormalize(&toMario, &toMario);

					// The const reference is load-bearing: JGVec3's
					// `operator-` takes its left operand *by value*, and
					// MWCC donates a temporary straight into that parameter
					// slot, subtracting in place. Binding the temporary to a
					// reference makes the left operand an lvalue, so the
					// by-value copy is emitted (retail's three-word copy at
					// 0x78 ahead of the `bl sub`) and the result is copied
					// out afterwards (96.0 -> 98.3). Rejected: a named
					// TVec3 for the difference or for the left operand (each
					// adds a second copy, 271/274 instructions vs 267), and a
					// TU-local const-reference subtraction helper (inlines
					// `sub` away, 285).
					// TODO: what remains is a 24-byte frame gap (0x178 vs
					// 0x190; 24 bytes of padding gives the exact frame with
					// no instruction change) and one GPR permutation that
					// survives it: retail keeps gpCurrentNpc in r5 and the
					// joint index in r0, we use r6 and r4, and retail emits
					// `li r30, 0` before the isNeedNeckStraight receiver
					// copy. Our temporaries are contiguous (rot 0xb0, the
					// operator- copy 0xbc, rot 0xc8, the scratch Mtx 0xd4)
					// where retail spreads them (0x78 copy, 0xcc/0xd8 rots,
					// 0xf0 Mtx), so the missing 24 bytes are in that
					// expansion region. A parked accessor for
					// mIndividualProps overshoots to 0x1d8 and changes
					// instructions.
					// Closure batch 115: the batch-110 binding level on
					// `gpCurrentNpc` (a TU-local `static inline TBaseNPC*
					// NpcCallbackCurrentNpc()` binding the global and
					// returning it) is worth **+12 of frame per expansion**
					// here, and applying it at exactly the two uses inside the
					// `shouldRun` predicate lands the frame on 0x190 with the
					// instruction count unchanged at 267 (98.3 -> 98.5, the
					// differing rows drop from 102 to 43). It is not committed
					// because the 24 bytes land in the wrong place: every
					// temporary slot is still individually permuted (the
					// MsGetRotFromZaxis buffers at 0xe0/0xc8 against retail's
					// 0xcc/0xd8, the scratch matrix at 0x130 against 0x120),
					// so the level fakes the total rather than reproducing
					// retail's expansion order, and the r5/r6 and r0/r4
					// permutations survive it untouched. The sweep over the
					// sixteen `gpCurrentNpc` reads is monotone (+8 to +16 each,
					// reaching 0x220 at all sixteen) and only the two-site
					// prefix lands 0x190. Also +0: wrapping the predicate in a
					// `static inline bool NpcCallbackNeckIsActive(const
					// TBaseNPC*)` -- a parameter binding is free here where a
					// global binding is not.
					// Batch 131 classified the residue instead of levering it
					// again: the statement below is an `a = b - c` site that
					// really emits `bl JGeometry::TVec3<f32>::sub`, so this
					// function is one of the 130 members of the census in
					// docs/catalog/frame-gaps.md (research batches 113/116/119)
					// and its slot permutation *is* that residue -- retail
					// allocates two 12-byte slots per `a = b - c` statement
					// (the by-value return above the by-value parameter temp)
					// and hoists nothing, which is why its copy sits at 0x78
					// with the rotation buffers above it at 0xcc/0xd8 while
					// ours packs copy and rotations together at 0xb0..0xd4.
					// Every reference-returning spelling of `operator-`
					// allocates exactly one slot, so no call-site lever can
					// reproduce it and the binding-level total is a fake. Do
					// not spend further lever budget here: the unit closes when
					// the by-value-return residue does, and the 24-byte total
					// plus the r5/r6 and r0/r4 permutations should be re-read
					// after that.
					const JGeometry::TVec3<f32>& rotToMario
					    = MsGetRotFromZaxis(toMario);
					local_148 = rotToMario
					            - MsGetRotFromZaxis(neckForward);
				} else {
					local_148.zero();
				}

				s16 tmp = CLBDegToShortAngle(local_148.y);
				s16 maxNeckY
				    = gpCurrentNpc->mIndividualParams->mNeckMaxAngleY.get();
				s16 minNeckY = -maxNeckY;

				r28 = MsClamp<s16>(tmp, minNeckY, maxNeckY);
			}

			CLBChaseGeneralConstantSpecifySpeed<s16>(
			    &neckY, r28,
			    CLBPalIntSpeed<s16>(
			        gpCurrentNpc->mIndividualParams->mNeckAngleYSpeed.get()));

			if (useTracking) {

				s16 tmp2 = CLBDegToShortAngle(local_148.x);
				s16 tmp  = MsClamp<s16>(
                    tmp2, gpCurrentNpc->mIndividualParams->mNeckMinAngleX.get(),
                    gpCurrentNpc->mIndividualParams->mNeckMaxAngleX.get());

				r30 = CLBEaseOutInbetween<s16>(
				    0.5f * tmp, tmp,
				    CLBCalcRatio<int>(
				        gpCurrentNpc->mIndividualParams->mNeckMaxAngleY.get(),
				        0, neckY >= 0 ? neckY : -neckY));
			}

			CLBChaseGeneralConstantSpecifySpeed<s16>(
			    &neckX, r30,
			    CLBPalIntSpeed<s16>(
			        gpCurrentNpc->mIndividualParams->mNeckAngleXSpeed.get()));

			gpCurrentNpc->unk230->set(neckX, neckY);

			Mtx rotMtx;
			f32 h = SHORTANGLE2DEG(neckX);
			f32 r = SHORTANGLE2DEG(neckY);
			MsMtxSetRotRPH(rotMtx, r, 0.0f, h);
			MTXConcat(currMtx, rotMtx, currMtx);
			MTXCopy(currMtx, J3DSys::mCurrentMtx);
		}
	}

	return TRUE;
}
