#include <NPC/NpcBase.hpp>
#include <MSound/MSound.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <System/MarDirector.hpp>
#include <Strategic/Spine.hpp>
#include <M3DUtil/LodAnm.hpp>
#include <M3DUtil/MActor.hpp>
#include <Player/MarioAccess.hpp>
#include <Enemy/Graph.hpp>
#include <Camera/cameralib.hpp>
#include <NPC/NpcThrow.hpp>
#include <NPC/NpcSave.hpp>
#include <NPC/NpcCoin.hpp>
#include <NPC/NpcInbetween.hpp>
#include <NPC/NpcParts.hpp>
#include <NPC/NpcNerve.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

// TODO: should be in a header and violate ODR
const char* cNpcPartsNameRootJoint = "__ROOT_JOINT__";

void CalcJumpVelocityY(f32, f32) { }

void TBaseNPC::isNowMotionBlend() const { }

void TBaseNPC::offStopMotionBlend() { }

void TBaseNPC::onStopMotionBlend() { }

void TBaseNPC::setNpcAnm_(EnumNpcAnmKind param_1,
                          EnumNpcStopMotionBlendOnOff param_2)
{
	if (mActorType < 0x400001E && mActorType >= 0x400001C)
		return;

	unk190->unk0 = NPC_ANM_KIND_INVALID;

	if (param_1 == unkD0->getCurrentAnmKind())
		return;

	if (!unkD0->setBckAndBtpAnm(param_1))
		return;

	if (param_1 == NPC_ANM_KIND_UNK11) {
		switch (unk1D9) {
		case 0:
		case 1:
		case 2:
			emitHappyEffect_();
			if (SMSGetMSound()->gateCheck(0x8808))
				MSoundSESystem::MSoundSE::startSoundNpcActor(0x8808, &mPosition,
				                                             0, nullptr, 0, 4);
			break;
		}
	}

	if (isSunflower()) {
		J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(5);
		if (checkUnk1D8(UNK1D8_FLAG_UNK1)) {
			if (param_1 == NPC_ANM_KIND_UNK5) {
				mMActor->setBrkFromIndex(mActorType == 0x400001A ? 1 : 1);
				ctrl->setAttribute(J3DFrameCtrl::ATTR_ONCE);
			} else {
				mMActor->setBrkFromIndex(mActorType == 0x400001A ? 0 : 0);
				ctrl->setRate(0.0f);
			}
		} else if (checkUnk1D8(UNK1D8_FLAG_UNK2)) {
			if (param_1 == NPC_ANM_KIND_UNK1A) {
				mMActor->setBrkFromIndex(mActorType == 0x400001A ? 0 : 0);
				ctrl->setAttribute(J3DFrameCtrl::ATTR_ONCE);
			}
		}
	}

	if (unk168 != nullptr && isPartsAnmNpc()) {
		switch (mActorType) {
		case 0x4000018: {
			bool bVar8 = checkUnk1D8(UNK1D8_FLAG_UNK1);
			if (MActor* mactor = unk168->getPartsMActor(0, 0)) {
				int iVar7;
				switch (param_1) {
				case NPC_ANM_KIND_UNK5:
					if (bVar8)
						iVar7 = 4;
					else
						iVar7 = 6;
					break;

				default:
					if (bVar8)
						iVar7 = 3;
					else
						iVar7 = 5;
					break;
				}
				if (!mactor->checkCurBckFromIndex(iVar7))
					mactor->setBckFromIndex(iVar7);
			}

			if (MActor* mactor = unk168->getPartsMActor(3, 0)) {
				int iVar7;

				switch (param_1) {
				case NPC_ANM_KIND_UNK9:
					iVar7 = 7;
					break;

				case NPC_ANM_KIND_UNK10:
					iVar7 = 14;
					break;

				case NPC_ANM_KIND_UNK1A:
					iVar7 = 13;
					break;

				case NPC_ANM_KIND_UNK5:
				default:
					if (bVar8)
						iVar7 = 11;
					else if (checkUnk1D8(UNK1D8_FLAG_UNK4))
						iVar7 = 9;
					else
						iVar7 = 8;
					break;
				}

				if (!mactor->checkCurBckFromIndex(iVar7))
					mactor->setBckFromIndex(iVar7);
			}

			if (MActor* mactor = unk168->getPartsMActor(4, 0)) {
				int iVar7;
				switch (param_1) {
				case NPC_ANM_KIND_UNK5:
					iVar7 = -1;
					break;
				default:
					iVar7 = 17;
					break;
				}
				if (!mactor->checkCurBckFromIndex(iVar7))
					mactor->setBckFromIndex(iVar7);
			}
		} break;

		case 0x4000010: {
			int iVar7 = 1;
			switch (param_1) {
			case NPC_ANM_KIND_UNK6:
				iVar7 = 2;
				break;
			case NPC_ANM_KIND_UNK5:
				iVar7 = 3;
				break;
			}
			MActor* mactor = unk168->getPartsMActor(9, 0);
			if (mactor != nullptr) {
				if (!mactor->checkCurBckFromIndex(iVar7))
					mactor->setBckFromIndex(iVar7);
			}
		} break;

		case 0x4000015: {
			int iVar7 = 0;
			int iVar3 = 0;
			switch (param_1) {
			case NPC_ANM_KIND_UNK5:
				iVar7 = 2;
				iVar3 = 1;
				break;
			case NPC_ANM_KIND_UNK6:
				iVar7 = 1;
				break;
			}
			MActor* mactor = unk168->getPartsMActor(10, 0);
			if (mactor != nullptr) {
				if (!mactor->checkCurBckFromIndex(iVar7))
					mactor->setBckFromIndex(iVar7);
				mactor->setBtpFromIndex(iVar3);
			}
		} break;
		}
	}

	if (param_2 == NPC_STOP_MOTION_BLEND_ON)
		mInbetweenCtrl->unk24 = mInbetweenCtrl->unk4;
	else
		mInbetweenCtrl->unk24 = 0;

	setCurAnmSound();
}

void TBaseNPC::requestNpcAnm_(EnumNpcAnmKind param_1,
                              EnumNpcStopMotionBlendOnOff param_2)
{
	if (mActorType < 0x400001E && mActorType >= 0x400001C)
		return;

	if (mInbetweenCtrl->isThing()) {
		if (param_1 == unkD0->getCurrentAnmKind()) {
			unk190->unk0 = NPC_ANM_KIND_INVALID;
		} else {
			// TODO: determine real type and add an inline
			UnkNpcStruct* thing = unk190;
			thing->unk0         = param_1;
			thing->unk4         = param_2 != NPC_STOP_MOTION_BLEND_OFF;
		}
	} else {
		setNpcAnm_(param_1, param_2);
	}
}

void TBaseNPC::setKeepAnm_()
{
	EnumNpcAnmKind tmp = unk190->unk0;
	EnumNpcStopMotionBlendOnOff tmp2
	    = (EnumNpcStopMotionBlendOnOff)unk190->unk4;
	unk190->unk0 = NPC_ANM_KIND_INVALID;
	if (tmp != NPC_ANM_KIND_INVALID) {
		if (tmp == unkD0->getCurrentAnmKind())
			unk190->unk0 = NPC_ANM_KIND_INVALID;
		else
			setNpcAnm_(tmp, tmp2);
	}
}

void TBaseNPC::requestTalkAnm_()
{
	EnumNpcAnmKind kind;
	if (mActionFlag & 0x400)
		kind = NPC_ANM_KIND_UNK1;
	else if ((mActionFlag & 0x1) && !(mActionFlag & 0x4))
		kind = NPC_ANM_KIND_UNK13;
	else
		kind = NPC_ANM_KIND_UNK6;

	requestNpcAnm_(kind, NPC_STOP_MOTION_BLEND_ON);
}

void TBaseNPC::randomizeBckAndBtpFrame_()
{
	s16 bckEnd            = 0;
	s16 bckFrame          = 0;
	J3DFrameCtrl* bckCtrl = mMActor->getFrameCtrl(0);
	if (bckCtrl != nullptr) {
		bckEnd   = bckCtrl->getEnd();
		bckFrame = bckEnd * MsRandF();
		bckCtrl->setFrame(bckFrame);
	}
	J3DFrameCtrl* btpCtrl = mMActor->getFrameCtrl(3);
	if (btpCtrl != nullptr) {
		s16 btpEnd   = btpCtrl->getEnd();
		s16 btpFrame = btpEnd == bckEnd ? bckFrame : (s16)(btpEnd * MsRandF());
		btpCtrl->setFrame(btpFrame);
	}
}

void TBaseNPC::walkAnmRateChange_()
{
	f32 dVar13 = MsSqrtf(mLinearVelocity.x * mLinearVelocity.x
	                     + mLinearVelocity.z * mLinearVelocity.z);
	if (dVar13 < 0.001f) {
		switch (unkD0->getCurrentAnmKind()) {
		case NPC_ANM_KIND_UNK0:
		case NPC_ANM_KIND_UNK8: {
			int iVar5
			    = CLBPalFrame(mPtrSaveNormal->mStopWalkAnmRateFrame.get());
			BOOL bVar7 = CLBChaseDecrease(
			    &unk1D0, 0.0f, mPtrSaveNormal->mStopWalkAnmRateChase.get(),
			    0.001f);

			if (unk1CC < iVar5)
				unk1CC += 1;

			if (unk1CC >= iVar5 || !bVar7) {
				unk1CC = 0;
				unk1D0 = 0.0f;

				bool bVar3 = true;
				if (!mInbetweenCtrl->isThing()
				    && !mInbetweenCtrl->isOtherThing())
					bVar3 = false;

				if (!bVar3)
					npcWaitIn();
				else if (!mInbetweenCtrl->isThing())
					mMActor->setFrameRate(unk1D0, 0);
			} else {
				mMActor->setFrameRate(unk1D0, 0);
			}
		} break;

		default:
			npcWaitIn();
			break;
		}
	} else {
		unk1CC = 0;
		switch (unkD0->getCurrentAnmKind()) {
		case NPC_ANM_KIND_UNK0:
		case NPC_ANM_KIND_UNK8: {
			f32 fVar1 = unk228->mSLMinWalkAnmRate.get() * SMSGetAnmFrameRate();

			f32 dVar10;
			f32 dVar12;
			if (unkD0->getCurrentAnmKind() == NPC_ANM_KIND_UNK8) {
				dVar12 = unk228->mSLMaxRunAnmRate.get() * SMSGetAnmFrameRate();
				dVar10 = unk228->mSLMaxRunSpeed.get();
				if (mActionFlag & 0x4000) {
					f32 dVar9 = mPtrSaveNormal->mSLSmokeRunMagnif.get();
					dVar12 *= dVar9;
					dVar10 *= dVar9;
				}
			} else {
				dVar12 = unk228->mSLMaxWalkAnmRate.get() * SMSGetAnmFrameRate();
				dVar10 = unk228->mMaxMarchSpeed.get();
			}

			if (dVar13 > dVar10)
				dVar13 = dVar10;

			f32 dVar131 = MsClamp(
			    CLBCalcRatio(unk228->mSLMinMarchSpeed.get(), dVar10, dVar13),
			    0.0f, 1.0f);

			f32 dVar132 = CLBLinearInbetween(fVar1, dVar12, dVar131);

			if (mColCount != 0)
				dVar132 = dVar12;

			CLBChaseDecrease(&unk1D0, dVar132,
			                 mPtrSaveNormal->mMoveWalkAnmRateChase.get(), 0.0f);
			mMActor->setFrameRate(unk1D0, 0);
		} break;

		default:
			unk1D0 = 0.0f;
			if (mActionFlag & 0x8)
				requestNpcAnm_(NPC_ANM_KIND_UNK8, NPC_STOP_MOTION_BLEND_ON);
			else
				requestNpcAnm_(NPC_ANM_KIND_UNK0, NPC_STOP_MOTION_BLEND_ON);
			break;
		}
	}
}

EnumNpcAnmKind TBaseNPC::getNpcWaitAnmBase_()
{
	EnumNpcAnmKind result = NPC_ANM_KIND_UNK1;
	if (unk1E2 == 0) {
		if (mActionFlag & 0x2)
			result = NPC_ANM_KIND_UNKC;
		else if (mActionFlag & 0x10)
			result = NPC_ANM_KIND_UNK15;
		else if (mActionFlag & 0x20)
			result = NPC_ANM_KIND_UNK6;
		else if (mActionFlag & 0x40)
			result = NPC_ANM_KIND_UNK17;
		else if (mActionFlag & 0x4)
			result = NPC_ANM_KIND_UNK16;
	}
	return result;
}

void TBaseNPC::npcWaitIn()
{
	EnumNpcAnmKind kind = NPC_ANM_KIND_UNK1;

	if (!(mActionFlag & 0x400)) {
		if (unk178 != 0.0f) {
			kind = NPC_ANM_KIND_UNKF;
		} else if (mActionFlag & 0x200) {
			kind = NPC_ANM_KIND_UNK11;
		} else if ((mActionFlag & 0x1) && !(mActionFlag & 0x4)) {
			if (mActionFlag & 0x20) {
				kind = NPC_ANM_KIND_UNK13;
			} else {
				kind = NPC_ANM_KIND_UNK12;
			}
		} else {
			if (!unk124->unk0->isDummy()) {
				if (mSpine->getLatestNerve()
				    == &TNerveNPCGraphWait::theNerve()) {
					bool b = gpMarDirector->unk124 == 1
					         || gpMarDirector->unk124 == 2
					         || gpMarDirector->unk124 == 4;
					if (!b)
						kind = getNpcWaitAnmBase_();
				}
			} else {
				kind = getNpcWaitAnmBase_();
			}
		}
	}

	requestNpcAnm_(kind, NPC_STOP_MOTION_BLEND_ON);
	resetToWait_();
}

void TBaseNPC::npcFallIn()
{
	requestNpcAnm_(NPC_ANM_KIND_UNK2, NPC_STOP_MOTION_BLEND_ON);
	resetToWait_();
}

bool TBaseNPC::npcRecoverFromSinking()
{
	bool result = false;

	if (!checkLiveFlag(LIVE_FLAG_UNK8000000)) {
		if (mMActor->getFrameCtrl(0)->checkPass(32.0f)) {
			onLiveFlag(LIVE_FLAG_UNK8000000);
			f32 dVar6 = getGravityY();
			f32 fVar1 = 0.0f;
			f32 tmp   = unk1C4 - mPosition.y + 150.0f;
			if (dVar6 > 0.0f) {
				fVar1 = dVar6 * 0.5f
				        * (MsSqrtf(tmp * (1.0f / dVar6) * 8.0f + 1.0f) + 1.0f);
			}

			mVelocity.y = fVar1;
			if (mVelocity.y < 5.0f)
				mVelocity.y = 5.0f;
		}
	}

	if (mMActor->isCurAnmAlreadyEnd(0)) {
		result = true;
		offLiveFlag(LIVE_FLAG_AIRBORNE);
		mVelocity.set(0.0f, 0.0f, 0.0f);
		mPosition.y = unk1C4;
		offLiveFlag(LIVE_FLAG_UNK10 | LIVE_FLAG_UNK400000
		            | LIVE_FLAG_UNK8000000);
	} else {
		if (checkLiveFlag(LIVE_FLAG_UNK8000000)) {
			mVelocity.y -= getGravityY();
			if (mVelocity.y < mVelocityMinY)
				mVelocity.y = mVelocityMinY;
			mPosition.y += mVelocity.y;
			if (mVelocity.y <= 0.0f && mPosition.y < unk1C4)
				mPosition.y = unk1C4;
		}
	}

	return result;
}

void TBaseNPC::npcRecoverAfterIn()
{
	requestNpcAnm_(NPC_ANM_KIND_UNK3, NPC_STOP_MOTION_BLEND_ON);
	resetToWait_();
}

void TBaseNPC::npcStepIn()
{
	requestNpcAnm_(NPC_ANM_KIND_UNK4, NPC_STOP_MOTION_BLEND_ON);
	resetToWait_();
}

void TBaseNPC::npcTalkIn()
{
	onLiveFlag(LIVE_FLAG_UNK80000);
	if (mActorType != 0x400001C && mActorType != 0x400001D) {
		if (!isSunflowerReviving()) {
			if (isPeachTired()) {
				requestNpcAnm_(NPC_ANM_KIND_UNK1A, NPC_STOP_MOTION_BLEND_ON);
			} else {
				requestTalkAnm_();
			}
		}
	}

	resetToWait_();
}

void TBaseNPC::npcTalking()
{
	if (isSunflowerReviving()) {
		sunflowerReviving();
		return;
	}

	if (isTurnToMarioWhenTalk()) {
		SMS_GoRotate(mPosition, SMS_GetMarioPos(), getTurnSpeed(),
		             &mRotation.y);
		if (!unk124->getGraph()->isDummy())
			unk1DA |= 0x1;
	}

	bool bVar1 = false;
	if (mActorType == 0x4000018 && checkUnk1D8(UNK1D8_FLAG_UNK2))
		bVar1 = true;

	if (bVar1 && unkD0->getCurrentAnmKind() == NPC_ANM_KIND_UNK1A
	    && mMActor->isCurAnmAlreadyEnd(0)) {
		peachTiredOut_();
	}
}

void TBaseNPC::npcTalkOut()
{
	mTalkForbidCount = 60;

	if (checkLiveFlag(LIVE_FLAG_UNK80000)) {
		unk1E2     = 120;
		bool bVar1 = false;
		if (isPeachTired())
			bVar1 = true;

		if (bVar1)
			peachTiredOut_();

		if (mActionFlag & 0x200) {
			mActionFlag &= ~0x200;
			if (mCoinCtrl) {
				mCoinCtrl->requestAppearCoin(getCursorPos(), mRotation.y, 40);
				mTalkForbidCount = 360;
				unk1E2           = 360;
			}
		}

		offLiveFlag(LIVE_FLAG_UNK8000);
		changeNerveFromTalk_();
		if (mThrowCtrl == nullptr && mActorType == 0x4000006)
			requestNpcAnm_(NPC_ANM_KIND_UNK4, NPC_STOP_MOTION_BLEND_ON);
	}
}

void TBaseNPC::npcTakenIn()
{
	requestNpcAnm_(NPC_ANM_KIND_UNK9, NPC_STOP_MOTION_BLEND_ON);
	resetToWait_();
}

void TBaseNPC::npcDanceIn()
{
	mActionFlag |= 0x4;
	requestNpcAnm_(NPC_ANM_KIND_UNK16, NPC_STOP_MOTION_BLEND_ON);
	resetToWait_();
}

void TBaseNPC::npcHappyIn(u8 param_1)
{
	unk1D9 = param_1;
	mActionFlag |= 0x200;
	requestNpcAnm_(NPC_ANM_KIND_UNK11, NPC_STOP_MOTION_BLEND_ON);
	resetToWait_();
}

void TBaseNPC::npcWetIn()
{
	if (!isSunflowerReviving()) {
		EnumNpcAnmKind EVar7              = NPC_ANM_KIND_UNK5;
		EnumNpcStopMotionBlendOnOff EVar6 = NPC_STOP_MOTION_BLEND_ON;
		if (unk178 != 0.0f) {
			EVar7 = NPC_ANM_KIND_UNK19;
		} else {
			if (mActionFlag & 1) {
				EVar7 = NPC_ANM_KIND_UNK14;

				if (isNormalMonte())
					EVar6 = NPC_STOP_MOTION_BLEND_OFF;
			} else {
				if (isNormalMonte() || mActorType == 0x400000D) {
					if (!checkLiveFlag(LIVE_FLAG_UNK4000000)
					    && MsRandF() < 0.5f)
						EVar7 = NPC_ANM_KIND_UNKB;
					else
						EVar6 = NPC_STOP_MOTION_BLEND_OFF;
				} else {
					if (isNormalMare() || mActorType == 0x4000011) {
						if (MsRandF() < 0.5f)
							EVar7 = NPC_ANM_KIND_UNKB;
					} else if (!(mActorType < 0x4000018
					             && mActorType >= 0x4000016)) {
						(void)mActorType; // TODO: uh, inlining maybe?
					} else if (!checkLiveFlag(LIVE_FLAG_UNK4000000)) {
						if (MsRandF() < 0.5f)
							EVar7 = NPC_ANM_KIND_UNKB;
					}
				}
			}
		}

		requestNpcAnm_(EVar7, EVar6);
	}

	resetToTurn_();
}

bool TBaseNPC::npcWetting()
{
	bool result = false;

	if (unk1DA & 0x2) {
		unk1DA &= ~0x2;

		npcHappyIn(0);
		npcWetOut();
		result = true;
	} else if (unkD0->getCurrentAnmKind() == NPC_ANM_KIND_UNK19) {
		if (unk178 == 0.0f) {
			npcHappyIn(0);
			npcWetOut();
			result = true;
		} else if (mMActor->isCurAnmAlreadyEnd(0)) {
			npcWetOut();
			result = true;
		}
	} else {
		if (isNormalMare() || mActorType == 0x4000011) {
			switch (unkD0->getCurrentAnmKind()) {
			case NPC_ANM_KIND_UNK5:
			case NPC_ANM_KIND_UNKB:
			case NPC_ANM_KIND_UNK14:
				if (mMActor->isCurAnmAlreadyEnd(0)) {
					unk22C->doThing3(240, 480);
					if (unk22C->doThing2()) {
						EnumNpcAnmKind EVar12 = NPC_ANM_KIND_UNK7;
						if (MsRandF() < 0.5f)
							EVar12 = NPC_ANM_KIND_UNK1B;
						requestNpcAnm_(EVar12, NPC_STOP_MOTION_BLEND_OFF);
					}
				}
				break;

			case NPC_ANM_KIND_UNK7:
			case NPC_ANM_KIND_UNK1B:
				if (mMActor->isCurAnmAlreadyEnd(0)) {
					npcWetOut();
					result = true;
				}
				break;
			}
		} else {
			if (isMonte() && isMadNpc() && mActorType != 0x4000006) {
				switch (unkD0->getCurrentAnmKind()) {
				case NPC_ANM_KIND_UNK5:
				case NPC_ANM_KIND_UNK14:
					if (mMActor->isCurAnmAlreadyEnd(0)) {
						if (isStateGoToMad_()) {
							npcMadIn();
						} else {
							npcWetOut();
							result = true;
						}
					}
					break;

				case NPC_ANM_KIND_UNK4:
					if (checkLiveFlag(LIVE_FLAG_UNK2000000))
						npcMadding();
					break;

				case NPC_ANM_KIND_UNKA:
				case NPC_ANM_KIND_UNKB:
					if (checkLiveFlag(LIVE_FLAG_UNK2000000)) {
						if (npcMadding()) {
							npcWetOut();
							result = true;
						}
					} else {
						if (mMActor->isCurAnmAlreadyEnd(0)) {
							npcWetOut();
							result = true;
						}
					}
				}
			} else {
				if (isSunflowerReviving()) {
					if (sunflowerReviving()) {
						npcWetOut();
						result = true;
					}
				} else {
					if ((mActorType < 0x4000018 && mActorType >= 0x4000016)
					    && mMActor->isCurAnmAlreadyEnd(0)) {
						switch (unkD0->getCurrentAnmKind()) {
						case NPC_ANM_KIND_UNK5:
							requestNpcAnm_(NPC_ANM_KIND_UNK7,
							               NPC_STOP_MOTION_BLEND_OFF);
							break;

						case NPC_ANM_KIND_UNKB:
							requestNpcAnm_(NPC_ANM_KIND_UNKA,
							               NPC_STOP_MOTION_BLEND_ON);
							break;

						case NPC_ANM_KIND_UNK14:
							requestNpcAnm_(NPC_ANM_KIND_UNK18,
							               NPC_STOP_MOTION_BLEND_ON);
							break;

						case NPC_ANM_KIND_UNK7:
						case NPC_ANM_KIND_UNK10:
						case NPC_ANM_KIND_UNK18:
							npcWetOut();
							result = true;
							break;
						}
					} else {
						if (mMActor->isCurAnmAlreadyEnd(0)) {
							switch (unkD0->getCurrentAnmKind()) {
							case NPC_ANM_KIND_UNK5:
							case NPC_ANM_KIND_UNKB:
							case NPC_ANM_KIND_UNK14:
							case NPC_ANM_KIND_UNK19:
								npcWetOut();
								result = true;
								break;
							}
						}
					}
				}
			}
		}
	}

	return result;
}

void TBaseNPC::npcWetOut()
{
	offLiveFlag(LIVE_FLAG_UNK2000000);
	offLiveFlag(LIVE_FLAG_UNK4000000);
	if (mSpine->getTop() == &TNerveNPCTalk::theNerve())
		requestTalkAnm_();
	else if (mActorType == 0x4000006)
		requestNpcAnm_(NPC_ANM_KIND_UNK4, NPC_STOP_MOTION_BLEND_ON);
}

void TBaseNPC::npcSinking()
{
	f32 dVar6 = unk1C4 - unk228->mSinkHeight.get();
	if (mPosition.y != dVar6) {
		f32 dVar7 = unk228->mSinkSpeed.get();
		if (isPollutionNpc()) {
			CLBChaseConstantSpecifyFrame(
			    &unk178, 1.0f, (1.0f / dVar7) * (mPosition.y - dVar6));
		}

		if (!CLBChaseGeneralConstantSpecifySpeed(&mPosition.y, dVar6, dVar7)) {
			onLiveFlag(LIVE_FLAG_UNK800000);
			onHitFlag(HIT_FLAG_NO_COLLISION);
			requestNpcAnm_(NPC_ANM_KIND_UNK10, NPC_STOP_MOTION_BLEND_ON);
		}
	}
}

void TBaseNPC::npcThrowIn()
{
	requestNpcAnm_(NPC_ANM_KIND_UNKD, NPC_STOP_MOTION_BLEND_ON);
	resetToWait_();
}

bool TBaseNPC::npcThrowing()
{
	bool result = false;
	int uVar4   = mPtrSaveNormal->mSLThrowStartFrame.get();
	int uVar1   = mSpine->getTime();
	if ((uVar1 == 0 && uVar4 < 20) || uVar1 == uVar4 - 20) {
		onHitFlag(HIT_FLAG_NO_COLLISION);
		unk1DC = CLBPalFrame(30);
	} else if (uVar1 == uVar4) {
		mThrowCtrl->throwMario(this);
	} else if (mMActor->isCurAnmAlreadyEnd(0)) {
		result = true;
	}

	return result;
}

void TBaseNPC::npcMadIn()
{
	onLiveFlag(LIVE_FLAG_UNK2000000);

	if (mActorType == 0x4000007 || (mActionFlag & 0x1)) {
		requestNpcAnm_(NPC_ANM_KIND_UNKA, NPC_STOP_MOTION_BLEND_ON);
		return;
	}

	npcStepIn();
}

bool TBaseNPC::npcMadding()
{
	bool result = false;

	if (!checkLiveFlag(LIVE_FLAG_UNK2000000)) {
		result = true;
	} else {
		switch (unkD0->getCurrentAnmKind()) {
		case NPC_ANM_KIND_UNK4: {
			SMS_GoRotate(mPosition, SMS_GetMarioPos(),
			             unk228->mMadTurnSpeed.get(), &mRotation.y);
			JGeometry::TVec3<f32> axis = SMS_GetMarioPos();
			axis -= mPosition;
			JGeometry::TVec3<f32> copy  = axis;
			JGeometry::TVec3<f32> copy2 = copy;
			JGeometry::TVec3<f32> copy3;
			copy3.set(copy2);
			f32 fVar1 = MsWrap(abs(mRotation.y - MsGetRotFromZaxisY(copy3)),
			                   0.0f, 360.0f);
			if (fVar1 < 0.001f)
				requestNpcAnm_(NPC_ANM_KIND_UNKA, NPC_STOP_MOTION_BLEND_OFF);
			if (!unk124->getGraph()->isDummy())
				unk1DA |= 0x1;
		} break;

		case NPC_ANM_KIND_UNKA:
		case NPC_ANM_KIND_UNKB:
			if (mMActor->isCurAnmAlreadyEnd(0)) {
				result = true;
				offLiveFlag(LIVE_FLAG_UNK2000000);
			}
			break;
		}
	}

	return result;
}

void TBaseNPC::npcBlownIn()
{
	requestNpcAnm_(NPC_ANM_KIND_UNKE, NPC_STOP_MOTION_BLEND_ON);
	resetToTurn_();
}

bool TBaseNPC::npcBlowning()
{
	bool result = false;

	if (!isAirborne())
		result = true;

	return result;
}

void TBaseNPC::npcMareStandIn()
{
	switch (unkD0->getCurrentAnmKind()) {
	case NPC_ANM_KIND_UNKE:
	case NPC_ANM_KIND_UNK5:
		unk22C->doThing3(240, 360);
		break;

	default:
		requestNpcAnm_(NPC_ANM_KIND_UNK7, NPC_STOP_MOTION_BLEND_ON);
		break;
	}

	resetToWait_();
}

bool TBaseNPC::npcMareStanding()
{
	bool result = false;
	switch (unkD0->getCurrentAnmKind()) {
	case NPC_ANM_KIND_UNK5:
	case NPC_ANM_KIND_UNKE:
		if (mMActor->isCurAnmAlreadyEnd(0) && unk22C->doThing2())
			requestNpcAnm_(NPC_ANM_KIND_UNK7, NPC_STOP_MOTION_BLEND_OFF);
		break;

	case NPC_ANM_KIND_UNK7:
		if (mMActor->isCurAnmAlreadyEnd(0))
			result = true;
		break;

	default:
		result = true;
		break;
	}

	return result;
}

void TBaseNPC::peachParasolIn_()
{
	static const TAnmBckMapping sIndividualPeachBck[] = {
		{ 21, 16 },
		{ 22, 18 },
		{ -1, -1 },
	};
	static const TAnmBtpMapping sIndividualPeachBtp[] = {
		{ 4, 2 },
		{ -1, -1 },
	};

	onUnk1D8(UNK1D8_FLAG_UNK1);
	unkD0->unk18 = sIndividualPeachBck;
	unkD0->unk1C = sIndividualPeachBtp;
}

void TBaseNPC::peachParasolOut_()
{
	if (!checkUnk1D8(UNK1D8_FLAG_UNK1))
		return;

	offUnk1D8(UNK1D8_FLAG_UNK1);
	unkD0->unk18 = nullptr;
	unkD0->unk1C = nullptr;
}

void TBaseNPC::peachTiredIn_()
{
	if (checkUnk1D8(UNK1D8_FLAG_UNK2))
		return;

	onUnk1D8(UNK1D8_FLAG_UNK2);
	requestNpcAnm_(NPC_ANM_KIND_UNK10, NPC_STOP_MOTION_BLEND_ON);
}

void TBaseNPC::peachTiredOut_()
{
	if (!checkUnk1D8(UNK1D8_FLAG_UNK2))
		return;

	offUnk1D8(UNK1D8_FLAG_UNK2);
	onUnk1D8(UNK1D8_FLAG_UNK4);

	static const TAnmBckMapping sIndividualPeachBck[] = {
		{ 21, 0 },
		{ -1, -1 },
	};
	static const TAnmBtpMapping sIndividualPeachBtp[] = {
		{ 5, 0 },
		{ -1, -1 },
	};

	unkD0->unk18 = sIndividualPeachBck;
	unkD0->unk1C = sIndividualPeachBtp;
	requestNpcAnm_(NPC_ANM_KIND_UNK6, NPC_STOP_MOTION_BLEND_OFF);
}

void TBaseNPC::sunflowerDownIn_()
{
	static const TAnmBckMapping sIndividualSunflowerBck[] = {
		{ 2, 0 },
		{ 3, 4 },
		{ -1, -1 },
	};
	static const TAnmBtpMapping sIndividualSunflowerBtp[] = {
		{ 3, 0 },
		{ 2, 0 },
		{ -1, -1 },
	};

	onUnk1D8(UNK1D8_FLAG_UNK1);
	offUnk1D8(UNK1D8_FLAG_UNK2);
	unkD0->unk18 = sIndividualSunflowerBck;
	unkD0->unk1C = sIndividualSunflowerBtp;
}

void TBaseNPC::sunflowerReviveIn()
{
	if (checkUnk1D8(UNK1D8_FLAG_UNK1)) {
		offUnk1D8(UNK1D8_FLAG_UNK1);
		onUnk1D8(UNK1D8_FLAG_UNK2);
		unkD0->unk18 = nullptr;
		unkD0->unk1C = nullptr;
		requestNpcAnm_(NPC_ANM_KIND_UNK1A, NPC_STOP_MOTION_BLEND_ON);
	}
}

bool TBaseNPC::sunflowerReviving()
{
	bool result = false;
	if (checkUnk1D8(UNK1D8_FLAG_UNK2)
	    && unkD0->getCurrentAnmKind() == NPC_ANM_KIND_UNK1A) {
		if (mMActor->isCurAnmAlreadyEnd(0)) {
			offUnk1D8(UNK1D8_FLAG_UNK2);
			if (checkLiveFlag(LIVE_FLAG_UNK80000)) {
				requestTalkAnm_();
			} else {
				npcWaitIn();
			}
			result = true;
		}
	}
	return result;
}

void TBaseNPC::monteMESetAnmWhenFar()
{
	switch (unkD0->getCurrentAnmKind()) {
	case NPC_ANM_KIND_UNK1:
		break;

	case NPC_ANM_KIND_UNK2:
		if (mMActor->isCurAnmAlreadyEnd(0))
			requestNpcAnm_(NPC_ANM_KIND_UNK1, NPC_STOP_MOTION_BLEND_OFF);
		break;

	case NPC_ANM_KIND_UNK3:
		if (mMActor->isCurAnmAlreadyEnd(0))
			requestNpcAnm_(NPC_ANM_KIND_UNK2, NPC_STOP_MOTION_BLEND_OFF);
		break;

	default:
		requestNpcAnm_(NPC_ANM_KIND_UNK2, NPC_STOP_MOTION_BLEND_ON);
		break;
	}
}

void TBaseNPC::monteMESetAnmWhenNear()
{
	switch (unkD0->getCurrentAnmKind()) {
	case NPC_ANM_KIND_UNK4:
		break;

	case NPC_ANM_KIND_UNK3:
	case NPC_ANM_KIND_UNKA:
		if (mMActor->isCurAnmAlreadyEnd(0))
			requestNpcAnm_(NPC_ANM_KIND_UNK4, NPC_STOP_MOTION_BLEND_OFF);
		break;

	case NPC_ANM_KIND_UNK2:
		if (mMActor->isCurAnmAlreadyEnd(0))
			requestNpcAnm_(NPC_ANM_KIND_UNK3, NPC_STOP_MOTION_BLEND_OFF);
		break;

	default:
		requestNpcAnm_(NPC_ANM_KIND_UNK3, NPC_STOP_MOTION_BLEND_OFF);
		break;
	}
}
