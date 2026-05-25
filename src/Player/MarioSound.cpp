#include <Player/MarioMain.hpp>
#include <Player/NozzleTrigger.hpp>
#include <Player/Yoshi.hpp>
#include <M3DUtil/M3UModelMario.hpp>
#include <Map/MapData.hpp>
#include <System/Application.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TMario::startVoiceYoshi(u32 param_1) { }

// TODO: fake!! use a real inline!
static void startForceJumpSound2(Vec* param_1, u32 param_2, f32 param_3,
                                 u32 param_4)
{
	SMSGetMSound()->startForceJumpSound(param_1, param_2, param_3, param_4);
}

void TMario::soundMovement()
{
	bool hasShineHolder = true;
	u32 curStatus       = mStatus;

	if (SMSGetMSound()->unkA8 & 1)
		mSoundValues.unk18 = 0;
	else
		mSoundValues.unk18 = 1;

	if (onYoshi()) {
		if (mWaterGun->isEmitting()
		    && ((const TWaterGun*)mWaterGun)->getCurrentNozzle()->unk378
		           > 0.0f) {
			SMSGetMSound()->startSoundActor(0x7129, &mYoshi->mTranslation, 0,
			                                nullptr, 0, 4);
		}

		if (curStatus & 0x40000) {
			SMSGetMSound()->startSoundActor(0x1009, &mYoshi->mTranslation, 0,
			                                nullptr, 0, 4);
			if (curStatus == STATUS_SLIP_FORE
			    && mSoundValues.unk00 != STATUS_SLIP_FORE) {
				SMSGetMSound()->startSoundActor(0x7926, &mYoshi->mTranslation,
				                                0, nullptr, 0, 4);
			}
		} else if (mSoundValues.unk00 == STATUS_SLIP_FORE) {
			SMSGetMSound()->startSoundActor(0x792A, &mYoshi->mTranslation, 0,
			                                nullptr, 0, 4);
		}

		if ((curStatus & 0x800) && !(mSoundValues.unk00 & 0x800)) {
			SMSGetMSound()->startSoundActor(0x791D, &mYoshi->mTranslation, 0,
			                                nullptr, 0, 4);
		}

		if (curStatus == STATUS_HIP_DROP
		    && mSoundValues.unk00 != STATUS_HIP_DROP) {
			SMSGetMSound()->startSoundActor(0x792B, &mYoshi->mTranslation, 0,
			                                nullptr, 0, 4);
		}

		if (curStatus == STATUS_HIP_ATTACK_END
		    && mSoundValues.unk00 == STATUS_HIP_DROP) {
			SMSGetMSound()->startSoundActor(0x198D, &mYoshi->mTranslation, 0,
			                                nullptr, 0, 4);
		}

		if (mSoundValues.unk00 != STATUS_HIP_DROP) {
			BOOL inputBit   = !!(mInput & 4);
			BOOL prevInpBit = !!(mSoundValues.unk0C & 4);

			if (inputBit != 0 && prevInpBit == 0)
				mSoundValues.unk1C = 0;
			if (inputBit != 0)
				mSoundValues.unk1C += 1;

			u8 unk20 = mSoundValues.unk20;
			if (unk20 != 0) {
				mSoundValues.unk20 = unk20 - 1;
				u8 unk20New        = mSoundValues.unk20;
				if (unk20New == 0) {
					if (inputBit == 0) {
						SMSGetMSound()->startSoundActor(
						    0x195A, &mYoshi->mTranslation, 0, nullptr, 0, 4);
					}
				} else if (unk20New == 4) {
					if (inputBit == 0) {
						SMSGetMSound()->startSoundActor(
						    0x195A, &mYoshi->mTranslation, 0, nullptr, 0, 4);
						SMSGetMSound()->startMarioVoice(0x792A, 1, 1);
					}
					mSoundValues.unk20 = 0;
				}
			}

			if (inputBit == 0 && prevInpBit != 0) {
				if (mSoundValues.unk1C > 0x78 && mVel.y < -74.0f)
					mSoundValues.unk20 = 8;
				else
					mSoundValues.unk20 = 4;
			}
		}
	} else {
		BOOL isRotJump = (mStatus == STATUS_LEFT_ROTATE_JUMP)
		                 | (mStatus == STATUS_RIGHT_ROTATE_JUMP);

		if (isRotJump) {
			if (mSoundValues.unk04 & 0x10) {
				if (!onYoshi())
					SMSGetMSound()->startMarioVoice(0x78B9, mHealth,
					                                getVoiceStatus());

				mSoundValues.unk04 ^= 0x10;
			}
		} else {
			mSoundValues.unk04 |= 0x10;
		}

		if (isRotJump) {
			mSoundValues.unk22 += 1;
			if (mSoundValues.unk22 == 10)
				SMSGetMSound()->startSoundActor(0x198E, &mPosition, 0, nullptr,
				                                0, 4);
		} else {
			mSoundValues.unk22 = 0;
		}

		if (mStatus == 0x884U && mSoundValues.unk08 != 0x884U) {
			if (mGroundPlane != nullptr
			    && mGroundPlane->getActor() != nullptr) {
				startForceJumpSound2(&mPosition, mSoundFlags, 0.0f,
				                     mGroundPlane->getData());
			}
		}

		u8 surfType = 0;
		u32 prev500 = mSoundValues.unk00;
		if (prev500 == STATUS_TAKE)
			surfType = 1;
		if (prev500 == STATUS_PUTTING)
			surfType = 2;

		if (surfType != 0) {
			TTakeActor* held = mHeldObject;
			if (held != nullptr) {
				switch (held->mActorType) {
				case 0x40000390:
				case 0x40000391:
				case 0x40000392:
				case 0x40000393:
				case 0x40000394:
				case 0x40000395:
				case 0x40000396:
					if (surfType == 1) {
						if (mSoundValues.unk04 & 0x40) {
							SMSGetMSound()->startSoundActor(0x194D, &mPosition,
							                                0, nullptr, 0, 4);
							mSoundValues.unk04 ^= 0x40;
						}
					} else if (mSoundValues.unk04 & 0x40) {
						SMSGetMSound()->startSoundActor(0x194E, &mPosition, 0,
						                                nullptr, 0, 4);
						mSoundValues.unk04 ^= 0x40;
					}
					break;
				case 0x40000046:
				case 0x4000005A:
				case 0x4000005B:
				case 0x4000005C:
				case 0x4000005D:
				case 0x40000064:
				case 0x40000065:
					if (surfType == 1) {
						if (mSoundValues.unk04 & 0x40) {
							SMSGetMSound()->startSoundActor(0x1800, &mPosition,
							                                0, nullptr, 0, 4);
							mSoundValues.unk04 ^= 0x40;
						}
					} else if (mSoundValues.unk04 & 0x40) {
						SMSGetMSound()->startSoundActor(0x194C, &mPosition, 0,
						                                nullptr, 0, 4);
						mSoundValues.unk04 ^= 0x40;
					}
					break;
				}
			}
		} else {
			mSoundValues.unk04 |= 0x40;
		}

		if (mStatus == STATUS_FOOT_DOWN) {
			if (mSoundValues.unk04 & 4) {
				SMSGetMSound()->startSoundActor(0x197B, &mPosition, 0, nullptr,
				                                0, 4);
				mSoundValues.unk04 ^= 4;
			}
		} else {
			mSoundValues.unk04 |= 4;
		}

		if (mStatus == STATUS_FOOT_DOWN
		    && mSoundValues.unk00 != STATUS_FOOT_DOWN) {
			if (!onYoshi())
				SMSGetMSound()->startMarioVoice(0x783B, mHealth,
				                                getVoiceStatus());
			SMSGetMSound()->stopMarioVoice(getVoiceStatus(), 0);
		}

		BOOL doVoice = mSoundValues.unk00 == STATUS_THROWN_DOWN
		               && (!(mInput & 4) && (mSoundValues.unk0C & 4));

		if (mSoundValues.unk24 != 0)
			mSoundValues.unk24 -= 1;

		if (doVoice) {
			SMSGetMSound()->startSoundActor(0x193E, &mPosition, 0, nullptr, 0,
			                                4);
			if (mSoundValues.unk24 == 0) {
				if (!onYoshi()) {
					SMSGetMSound()->startMarioVoice(0x783B, mHealth,
					                                getVoiceStatus());
				}
				mSoundValues.unk24 = 0xB4;
			}
		}

		if (curStatus == STATUS_THROWN_DOWN)
			mSoundValues.unk26 += 1;
		else
			mSoundValues.unk26 = 0;

		switch (mSoundValues.unk14) {
		case 0x10000015:
			if (mSoundValues.unk26 == 0x3C && !onYoshi()) {
				SMSGetMSound()->startMarioVoice(0x786B, mHealth,
				                                getVoiceStatus());
			}
			break;
		case 0x8000014:
		case 0x8000015:
		default:
			if (mSoundValues.unk26 == 0x1E && !onYoshi()) {
				SMSGetMSound()->startMarioVoice(0x78BF, mHealth,
				                                getVoiceStatus());
			}
			break;
		}

		if (mColCount != 0) {
			if (*mCollisions != nullptr) {
				mSoundValues.unk29 = 4;
				mSoundValues.unk14 = (*mCollisions)->mActorType;
				if ((*mCollisions)->checkActorType(0x04000000))
					mSoundValues.unk28 = 1;
				else
					mSoundValues.unk28 = 2;
			}
		} else {
			if (mSoundValues.unk29 != 0)
				mSoundValues.unk29 -= 1;
			if (mSoundValues.unk29 == 0)
				mSoundValues.unk28 = 0;
		}

		if (mStatus & 0x20000) {
			BOOL isDownLike = (u32)(mStatus == 0x20466)
			                  | (u32)(mStatus == 0x208B0)
			                  | (u32)(mStatus == 0x208B3);
			if (mSoundValues.unk00 == STATUS_RUN && curStatus == 0x20462
			    && (mSoundValues.unk04 & 8) && !onYoshi()) {
				SMSGetMSound()->startMarioVoice(0x783B, mHealth,
				                                getVoiceStatus());
			}
			if (isDownLike != 0) {
				if ((mSoundValues.unk04 & 8) && !onYoshi()) {
					SMSGetMSound()->startMarioVoice(0x783B, mHealth,
					                                getVoiceStatus());
				}
			}
			switch (mSoundValues.unk28) {
			case 0:
				if (isDownLike != 0) {
					if (mSoundValues.unk04 & 8) {
						SMSGetMSound()->startSoundActor(0x1965, &mPosition, 0,
						                                nullptr, 0, 4);
						mSoundValues.unk04 ^= 8;
					}
				} else {
					mSoundValues.unk04 |= 8;
				}
				break;
			case 1:
				if (isDownLike != 0) {
					if (mSoundValues.unk04 & 8) {
						SMSGetMSound()->startSoundActor(0x1949, &mPosition, 0,
						                                nullptr, 0, 4);
						mSoundValues.unk04 ^= 8;
					}
				} else {
					mSoundValues.unk04 |= 8;
				}
				break;
			case 2:
				if (isDownLike != 0) {
					if (mSoundValues.unk04 & 8) {
						SMSGetMSound()->startSoundActor(0x1948, &mPosition, 0,
						                                nullptr, 0, 4);
						mSoundValues.unk04 ^= 8;
					}
				} else {
					mSoundValues.unk04 |= 8;
				}
				break;
			}
		} else {
			mSoundValues.unk04 |= 8;
		}

		if ((mInput & 0x100) && mWaterGun->mCurrentWater == 0) {
			SMSGetMSound()->startSoundActor(0x802, &mPosition, 0, nullptr, 0,
			                                4);
		}

		const TWaterGun* gun = mWaterGun;
		s32 pressureScale    = (s32)(100.0f * gun->unk1CEC);
		if (mSoundValues.unk2A == 0 && (u8)pressureScale != 0) {
			SMSGetMSound()->startSoundActor(0x807, &mPosition, 0, nullptr, 0,
			                                4);
		}
		if (mSoundValues.unk2A == 0x12 && (u8)pressureScale == 0x13
		    && (s32)gun->mCurrentNozzle == 0) {
			SMSGetMSound()->startSoundActor(0x808, &mPosition, 0, nullptr, 0,
			                                4);
		}
		if (mSoundValues.unk2A == 0x31 && (u8)pressureScale == 0x32) {
			SMSGetMSound()->startSoundActor(0x815, &mPosition, 0, nullptr, 0,
			                                4);
		}
		mSoundValues.unk2A = (u8)pressureScale;

		if (isWearingCap()) {
			if (!(mSoundValues.unk04 & 0x80)) {
				SMSGetMSound()->startSoundActor(0x1982, &mPosition, 0, nullptr,
				                                0, 4);
			}
			mSoundValues.unk04 |= 0x80;
		} else {
			if (mSoundValues.unk04 & 0x80) {
				SMSGetMSound()->startSoundActor(0x1983, &mPosition, 0, nullptr,
				                                0, 4);
			}
			if (mSoundValues.unk04 & 0x80)
				mSoundValues.unk04 ^= 0x80;
		}

		if (curStatus == 0x560 || curStatus == 0x40561 || curStatus == 0x894) {
			u32 prevSt = mSoundValues.unk00;
			if (prevSt != 0x560 && prevSt != 0x40561 && prevSt != 0x894) {
				if (mHealth > 2) {
					if (!onYoshi())
						SMSGetMSound()->startMarioVoice(0x788F, mHealth,
						                                getVoiceStatus());
				} else if (!onYoshi()) {
					SMSGetMSound()->startMarioVoice(0x78FB, mHealth,
					                                getVoiceStatus());
				}
				mSoundValues.unk2B = 0x78;
			} else if (curStatus == 0x894 && prevSt != 0x894) {
				if (mHealth > 2) {
					if (!onYoshi())
						SMSGetMSound()->startMarioVoice(0x7807, mHealth,
						                                getVoiceStatus());
				} else if (!onYoshi()) {
					SMSGetMSound()->startMarioVoice(0x7903, mHealth,
					                                getVoiceStatus());
				}
			}
		}

		if (mSoundValues.unk2B != 0)
			mSoundValues.unk2B -= 1;

		if (mHeldObject != nullptr && (curStatus & 0x400)
		    && mSoundValues.unk2B == 0 && mSoundValues.unk18 == 0
		    && !onYoshi()) {
			SMSGetMSound()->startMarioVoice(0x7094, mHealth, getVoiceStatus());
		}

		if (mGroundPlane != nullptr) {
			if (mGroundPlane->isWaterSurface() || mGroundPlane->isSea())
				hasShineHolder = false;
			else
				hasShineHolder = true;
		}

		if (curStatus & 0x10000) {
			if (!(mSoundValues.unk00 & 0x10000)) {
				mSoundValues.unk2C = 0x1E;
				if (mSoundValues.unk04 & 0x200)
					mSoundValues.unk04 ^= 0x200;
			}
			if (mSoundValues.unk2C != 0) {
				SMSGetMSound()->startSoundActor(0x27, &mPosition, 0, nullptr, 0,
				                                4);
				mSoundValues.unk2C -= 1;
				if (mSoundValues.unk2C == 0) {
					SMSGetMSound()->startSoundActor(0x826, &mPosition, 0,
					                                nullptr, 0, 4);
				}
			}
			SMSGetMSound()->startSoundActorWithInfo(
			    0x117E, &mPosition, nullptr, mForwardVel, 0, 0, nullptr, 0, 4);

		} else {
			mSoundValues.unk2C = 0;
			mSoundValues.unk04 |= 0x200;
		}

		switch (curStatus) {
		case 0x208B3:
			break;
		case STATUS_SURF:
			mSoundValues.unk04 |= 0x100;
			if (mSoundValues.unk00 == 0x281089A) {
				if (!hasShineHolder) {
					SMSGetMSound()->startSoundActor(0x828, &mPosition, 0,
					                                nullptr, 0, 4);
				}
			} else {
				u32 sfx = hasShineHolder ? 0x117F : 0x117D;
				if (mSoundValues.unk2C == 0) {
					SMSGetMSound()->startSoundActorWithInfo(
					    sfx, &mPosition, nullptr, mForwardVel, 0, 0, nullptr, 0,
					    4);
				}
			}
			break;
		case 0x281089A:
			if (!hasShineHolder && (mSoundValues.unk04 & 0x100)) {
				SMSGetMSound()->startSoundActor(0x828, &mPosition, 0, nullptr,
				                                0, 4);
				mSoundValues.unk04 ^= 0x100;
			}
			break;
		}

		if (checkFlag(MARIO_FLAG_VISIBLE)) {
			if (curStatus == 0x884U && mSoundValues.unk00 != 0x884U) {
				if (mHealth > 2) {
					if (!onYoshi())
						SMSGetMSound()->startMarioVoice(0x78B9, mHealth,
						                                getVoiceStatus());
				} else if (!onYoshi()) {
					SMSGetMSound()->startMarioVoice(0x7907, mHealth,
					                                getVoiceStatus());
				}
			}
		} else if (curStatus == 0x884U && mSoundValues.unk00 != 0x884U
		           && gpApplication.mCurrArea.getStage() == 2) {
			SMSGetMSound()->startSoundActor(0x1812, &mPosition, 0, nullptr, 0,
			                                4);
			if (mHealth > 2) {
				if (!onYoshi())
					SMSGetMSound()->startMarioVoice(0x78B9, mHealth,
					                                getVoiceStatus());
			} else if (!onYoshi()) {
				SMSGetMSound()->startMarioVoice(0x7907, mHealth,
				                                getVoiceStatus());
			}
		}

		if ((mSoundValues.unk00 & 0x800) && curStatus == STATUS_HANGING) {
			if (mHealth > 2) {
				if (!onYoshi())
					SMSGetMSound()->startMarioVoice(0x7890, mHealth,
					                                getVoiceStatus());
			} else if (!onYoshi()) {
				SMSGetMSound()->startMarioVoice(0x78FC, mHealth,
				                                getVoiceStatus());
			}
		}

		if (mSoundValues.unk00 != STATUS_TAKEN && curStatus == STATUS_TAKEN
		    && !onYoshi()) {
			SMSGetMSound()->startMarioVoice(0x784F, mHealth, getVoiceStatus());
		}

		if (mSoundValues.unk00 == STATUS_LAND_SLIP && curStatus == 0x88C
		    && !onYoshi()) {
			SMSGetMSound()->startMarioVoice(0x7913, mHealth, getVoiceStatus());
		}

		u32 prev500_2 = mSoundValues.unk00;
		if (prev500_2 & 0x800) {
			switch (curStatus) {
			case STATUS_WIRE_HANGING:
			case STATUS_WIRE_WAIT_TO_HANG:
			case STATUS_WIRE_S_WAIT_TO_HANG:
			case STATUS_WIRE_RETURN:
				if (mHealth > 2) {
					if (!onYoshi())
						SMSGetMSound()->startMarioVoice(0x7890, mHealth,
						                                getVoiceStatus());
				} else if (!onYoshi()) {
					SMSGetMSound()->startMarioVoice(0x78F2, mHealth,
					                                getVoiceStatus());
				}
				break;
			case STATUS_WIRE_WAIT:
			case STATUS_WIRE_S_WAIT:
			case STATUS_WIRE_WAIT_TO_S_WAIT_L:
			case STATUS_WIRE_WAIT_TO_S_WAIT_R:
				if ((u32)(prev500_2 - 0x892) > 1) {
					if (mHealth > 2) {
						if (!onYoshi())
							SMSGetMSound()->startMarioVoice(0x788F, mHealth,
							                                getVoiceStatus());
					} else if (!onYoshi()) {
						SMSGetMSound()->startMarioVoice(0x78F3, mHealth,
						                                getVoiceStatus());
					}
				}
				break;
			}
		}

		if ((u32)(curStatus - STATUS_SLIP_FORE) <= 1 && mSoundValues.unk18 == 0
		    && !isRunningInWater()) {
			SMSGetMSound()->startSoundActor(0x1009, &mPosition, 0, nullptr, 0,
			                                4);
		}

		if (mSoundValues.unk00 == STATUS_HIP_DROP
		    && curStatus != STATUS_HIP_DROP) {
			if (!(curStatus & 0x20000) && !(curStatus & 0x80000000)) {
				SMSGetMSound()->startSoundActor(0x180E, &mPosition, 0, nullptr,
				                                0, 4);
			}
		} else if (curStatus == STATUS_KICK_ROOF_ROLL_DOWN
		           && mSoundValues.unk00 == STATUS_HIP_DROP) {
			SMSGetMSound()->startSoundActor(0x180E, &mPosition, 0, nullptr, 0,
			                                4);
		}

		u32 statLo = mStatus & STATUS_TYPE_AND_ID_MASK;
		if (statLo >= 0x150 && statLo <= 0x15C && mStatus != 0x10000358) {
			if (mWireBounceVelPrev > 0.0f && mWireBounceVel <= 0.0f) {
				SMSGetMSound()->startSoundActorWithInfo(0x381C, &mWireStartPos,
				                                        nullptr, mWireSag, 0, 0,
				                                        nullptr, 0, 4);
				mWireSfxTimer    = mWireSfxDelay;
				mWireQueuedSfxID = 0x381C;
			}
			if (mWireBounceVelPrev < 0.0f && mWireBounceVel >= 0.0f) {
				SMSGetMSound()->startSoundActorWithInfo(0x381D, &mWireStartPos,
				                                        nullptr, mWireSag, 0, 0,
				                                        nullptr, 0, 4);
				mWireSfxTimer    = mWireSfxDelay;
				mWireQueuedSfxID = 0x381D;
			}
		}

		if (mWireSfxTimer != 0) {
			mWireSfxTimer -= 1;
			if (mWireSfxTimer == 0) {
				SMSGetMSound()->startSoundActorWithInfo(
				    mWireQueuedSfxID, &mWireEndPos, nullptr, mWireSag, 0, 0,
				    nullptr, 0, 4);
			}
		}

		if (mStatus == STATUS_WARP_IN && mStatusState == 0 && mStatusTimer == 1)
			SMSGetMSound()->startSoundActor(0x1979, &mPosition, 0, nullptr, 0,
			                                4);

		if (mStatus == STATUS_ELECTRIC_DAMAGE && mStatusState == 0)
			SMSGetMSound()->startSoundActor(0x1814, &mPosition, 0, nullptr, 0,
			                                4);
	}

	mSoundValues.unk08 = mStatus;
	mSoundValues.unk00 = curStatus;
	mSoundValues.unk0C = mInput;
	mSoundValues.unk10 = unk78;
}

void TMario::animSound()
{
	mSoundFlags = mGroundPlane->unk6;

	if (checkFlag(MARIO_FLAG_UNK_40)) {
		if (unk350 == 0
		    && unk368 >= mGraffitoParams.mSinkTime.get()
		                     * mGraffitoParams.mSinkDmgDepth.get())
			mSoundFlags |= 0x600;
		else
			mSoundFlags |= 0x500;
	} else {
		if (checkFlag(MARIO_FLAG_RECENTLY_LEFT_WATER)
		    || mGroundPlane->isWetGround()) {
			mSoundFlags |= 0x700;
		}

		if (checkFlag(MARIO_FLAG_IN_ANY_WATER)) {
			if (mFloorPosition.z - mPosition.y > 30.0f)
				mSoundFlags |= 0x200;
			else
				mSoundFlags |= 0x100;
		}
	}

	if (unk368 > 0.0f ? TRUE : FALSE) {
		if (-(unk368 / mGraffitoParams.mSinkTime.get())
		        * mGraffitoParams.mSinkHeight.get()
		    > 30.0f)
			mSoundFlags |= 0x400;
		else
			mSoundFlags |= 0x300;
	}

	// TODO: inline
	bool b;
	if (mHolder && mHolder->getActorType() == 0x40000098)
		b = true;
	else
		b = false;
	if (b)
		mSoundFlags |= 0x15;

	// TODO: huh?
	if (checkFlag(MARIO_FLAG_HAS_FLUDD))
		mSoundFlags |= 0x0;
	else
		mSoundFlags |= 0x1000;

	if (unk388 == 1)
		mSoundFlags |= 0x20000000;
	else if (unk388 == 2)
		mSoundFlags |= 0x60000000;

	mSoundFlags += mHealth * 0x1000000;

	if (!onYoshi()) {
		mAnmSound->animeLoop(&mPosition, getCurrentFrame(0),
		                     mModel->getFrameCtrl(0).getRate(), mSoundFlags, 4);
	}
}

void TMario::soundHitBound() { }

void TMario::soundTorocco()
{
	f32 len = JGeometry::TVec3<f32>(mPosition - mToroccoPos).length();
	SMSGetMSound()->startSoundActorWithInfo(0x305A, &mPosition, nullptr, len, 0,
	                                        0, nullptr, 0, 4);
}

u8 TMario::getVoiceStatus()
{
	if (onYoshi())
		return 1;

	switch (unk388) {
	case 1:
		return 2;
	case 2:
		return 6;
	}

	return 0;
}

u32 TMario::startVoice(u32 param_1)
{
	if (onYoshi())
		return 0;

	return SMSGetMSound()->startMarioVoice(param_1, mHealth, getVoiceStatus());
}

u32 TMario::startVoiceIfNoVoice(u32 param_1)
{
	if (SMSGetMSound()->getMarioVoiceID(0) == -1)
		return startVoice(param_1);

	return 0;
}

void TMario::stopVoice()
{
	SMSGetMSound()->stopMarioVoice(getVoiceStatus(), 0);
}

void TMario::startSoundActor(u32 param_1)
{
	SMSGetMSound()->startSoundActor(param_1, &mPosition, 0, nullptr, 0, 4);
}

TMarioSoundValues::TMarioSoundValues()
{
	unk00 = 0;
	unk04 = -1;
	unk08 = 0;
	unk0C = 0;
	unk10 = 0;
	unk14 = 0;
	unk18 = 0;
	unk1C = 0;
	unk20 = 0;
	unk22 = 0;
	unk24 = 0;
	unk26 = 0;
	unk14 = 0;
	unk29 = 0;
	unk2A = 1;
	unk2B = 0;
	unk2C = 0;
}
