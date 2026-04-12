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
		                 && !gpCurrentNpc->checkLiveFlag2(
		                     LIVE_FLAG_HIDDEN | LIVE_FLAG_CLIPPED_OUT);

		if (shouldRun) {
			J3DJoint* joint = (J3DJoint*)param_1;
			MtxPtr currMtx  = j3dSys.mModel->mNodeMatrices[joint->mJntNo];

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

				JGeometry::TVec3<f32> toMario(marioPos.x - currMtx[0][3],
				                              marioPos.y - currMtx[1][3],
				                              marioPos.z - currMtx[2][3]);

				f32 dist2 = toMario.squared();
				if (dist2 > 0.001f
				    && dist2 < CLBSquared<f32>(
				           gpCurrentNpc->unk228->mNeckTurnSearchDist.get())
				    && fabs(marioPos.y - currMtx[1][3])
				           < gpCurrentNpc->unk228->mNeckTurnSearchHeight
				                 .get()) {

					JGeometry::TVec3<f32> neckForward(
					    currMtx[0][1], currMtx[1][1], currMtx[2][1]);
					MsVECNormalize(&toMario, &toMario);

					local_148 = MsGetRotFromZaxis(toMario)
					            - MsGetRotFromZaxis(neckForward);
				} else {
					local_148.zero();
				}

				s16 tmp      = CLBDegToShortAngle(local_148.y);
				s16 maxNeckY = gpCurrentNpc->unk228->mNeckMaxAngleY.get();
				s16 minNeckY = -maxNeckY;

				r28 = MsClamp<s16>(tmp, minNeckY, maxNeckY);
			}

			CLBChaseGeneralConstantSpecifySpeed<s16>(
			    &neckY, r28,
			    CLBPalIntSpeed<s16>(
			        gpCurrentNpc->unk228->mNeckAngleYSpeed.get()));

			if (useTracking) {

				s16 tmp2 = CLBDegToShortAngle(local_148.x);
				s16 tmp  = MsClamp<s16>(
                    tmp2, gpCurrentNpc->unk228->mNeckMinAngleX.get(),
                    gpCurrentNpc->unk228->mNeckMaxAngleX.get());

				r30 = CLBEaseOutInbetween<s16>(
				    0.5f * tmp, tmp,
				    CLBCalcRatio<int>(
				        gpCurrentNpc->unk228->mNeckMaxAngleY.get(), 0,
				        neckY >= 0 ? neckY : -neckY));
			}

			CLBChaseGeneralConstantSpecifySpeed<s16>(
			    &neckX, r30,
			    CLBPalIntSpeed<s16>(
			        gpCurrentNpc->unk228->mNeckAngleXSpeed.get()));

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
