#include <Player/Mario.hpp>
#include <Player/NozzleTrigger.hpp>
#include <Player/Yoshi.hpp>
#include <Map/PollutionLayer.hpp>
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

#ifndef VERSION_D38J01
void TMario::soundMovement()
{
	bool hasShineHolder = true;
	u32 curStatus       = mStatus;

	if (SMSGetMSound()->mSeGateMask & MSSeGate_Continuous)
		mSoundValues.unk18 = 0;
	else
		mSoundValues.unk18 = 1;

	if (onYoshi()) {
		if (mWaterGun->isEmitting()
		    && mWaterGun->getCurrentNozzle()->getUnk378() > 0.0f) {
			SMSGetMSound()->startSoundActor(
			    MSD_SE_YV_WATER, &mYoshi->mTranslation, 0, nullptr, 0, 4);
		}

		if (curStatus & MARIO_STATUS_FLAG_UNK40000) {
			SMSGetMSound()->startSoundActor(
			    MSD_SE_MA_SLIP, &mYoshi->mTranslation, 0, nullptr, 0, 4);
			if (curStatus == MARIO_STATUS_SLIP_FORE
			    && mSoundValues.oldstatus != MARIO_STATUS_SLIP_FORE) {
				SMSGetMSound()->startSoundActor(
				    MSD_SE_YV_FUNBARI, &mYoshi->mTranslation, 0, nullptr, 0, 4);
			}
		} else if (mSoundValues.oldstatus == MARIO_STATUS_SLIP_FORE) {
			SMSGetMSound()->startSoundActor(
			    MSD_SE_YV_PURU_PURU2, &mYoshi->mTranslation, 0, nullptr, 0, 4);
		}

		if ((curStatus & MARIO_STATUS_FLAG_JUMPING)
		    && !(mSoundValues.oldstatus & MARIO_STATUS_FLAG_JUMPING)) {
			SMSGetMSound()->startSoundActor(
			    MSD_SE_YV_JUMP1, &mYoshi->mTranslation, 0, nullptr, 0, 4);
		}

		if (curStatus == MARIO_STATUS_HIP_DROP
		    && mSoundValues.oldstatus != MARIO_STATUS_HIP_DROP) {
			SMSGetMSound()->startSoundActor(MSD_SE_YV_PURU_POWERFUL,
			                                &mYoshi->mTranslation, 0, nullptr,
			                                0, 4);
		}

		if (curStatus == MARIO_STATUS_HIP_ATTACK_END
		    && mSoundValues.oldstatus == MARIO_STATUS_HIP_DROP) {
			SMSGetMSound()->startSoundActor(
			    MSD_SE_YO_HIP_ATTACK, &mYoshi->mTranslation, 0, nullptr, 0, 4);
		}

		if (mSoundValues.oldstatus != MARIO_STATUS_HIP_DROP) {
			BOOL inputBit   = !!(mInput & 4);
			BOOL prevInpBit = !!(mSoundValues.oldaction & 4);

			if (inputBit != 0 && prevInpBit == 0)
				mSoundValues.jtimer = 0;
			if (inputBit != 0)
				mSoundValues.jtimer += 1;

			u8 unk20 = mSoundValues.callsound_delay;
			if (unk20 != 0) {
				mSoundValues.callsound_delay = unk20 - 1;
				u8 unk20New                  = mSoundValues.callsound_delay;
				if (unk20New == 0) {
					if (inputBit == 0) {
						SMSGetMSound()->startSoundActor(MSD_SE_MA_BOUND,
						                                &mYoshi->mTranslation,
						                                0, nullptr, 0, 4);
					}
				} else if (unk20New == 4) {
					if (inputBit == 0) {
						SMSGetMSound()->startSoundActor(MSD_SE_MA_BOUND,
						                                &mYoshi->mTranslation,
						                                0, nullptr, 0, 4);
						SMSGetMSound()->startMarioVoice(MSD_SE_YV_PURU_PURU2, 1,
						                                1);
					}
					mSoundValues.callsound_delay = 0;
				}
			}

			if (inputBit == 0 && prevInpBit != 0) {
				if (mSoundValues.jtimer > 0x78 && mVel.y < -74.0f)
					mSoundValues.callsound_delay = 8;
				else
					mSoundValues.callsound_delay = 4;
			}
		}
	} else {
		BOOL isRotJump = (mStatus == MARIO_STATUS_LEFT_ROTATE_JUMP)
		                 | (mStatus == MARIO_STATUS_RIGHT_ROTATE_JUMP);

		if (isRotJump) {
			if (mSoundValues.mss_state & 0x10) {
				if (!onYoshi())
					SMSGetMSound()->startMarioVoice(MSD_SE_MV24_JUMP_SPECIAL_01,
					                                mHealth, getVoiceStatus());

				mSoundValues.mss_state ^= 0x10;
			}
		} else {
			mSoundValues.mss_state |= 0x10;
		}

		if (isRotJump) {
			mSoundValues.spin_timer += 1;
			if (mSoundValues.spin_timer == 10)
				SMSGetMSound()->startSoundActor(MSD_SE_MA_ROLL_JUMP, &mPosition,
				                                0, nullptr, 0, 4);
		} else {
			mSoundValues.spin_timer = 0;
		}

		if (mStatus == MARIO_STATUS_FORCE_JUMP
		    && mSoundValues.lastStatus != MARIO_STATUS_FORCE_JUMP) {
			if (mGroundPlane != nullptr
			    && mGroundPlane->getActor() != nullptr) {
				startForceJumpSound2(&mPosition, mSoundFlags, 0.0f,
				                     mGroundPlane->getData());
			}
		}

		u8 surfType = 0;
		u32 prev500 = mSoundValues.oldstatus;
		if (prev500 == MARIO_STATUS_TAKE)
			surfType = 1;
		if (prev500 == MARIO_STATUS_PUTTING)
			surfType = 2;

		if (surfType != 0) {
			TTakeActor* held = mHeldObject;
			if (held != nullptr) {
				switch (held->mActorType) {
				case ACTOR_TYPE_FRUIT_COCONUT:
				case ACTOR_TYPE_FRUIT_PAPAYA:
				case ACTOR_TYPE_FRUIT_PINE:
				case ACTOR_TYPE_FRUIT_DURIAN:
				case ACTOR_TYPE_FRUIT_BANANA:
				case ACTOR_TYPE_RED_PEPPER:
				case ACTOR_TYPE_FRUIT_COVER_PINE:
					if (surfType == 1) {
						if (mSoundValues.mss_state & 0x40) {
							SMSGetMSound()->startSoundActor(
							    MSD_SE_MA_TAKE_DRIAN, &mPosition, 0, nullptr, 0,
							    4);
							mSoundValues.mss_state ^= 0x40;
						}
					} else if (mSoundValues.mss_state & 0x40) {
						SMSGetMSound()->startSoundActor(
						    MSD_SE_MA_PUT_DRIAN, &mPosition, 0, nullptr, 0, 4);
						mSoundValues.mss_state ^= 0x40;
					}
					break;
				case ACTOR_TYPE_PLANT_FLOWER:
				case ACTOR_TYPE_WOOD_BARREL:
				case ACTOR_TYPE_BARREL_FLOAT:
				case ACTOR_TYPE_BARREL_OIL:
				case ACTOR_TYPE_DRUM_CAN:
				case ACTOR_TYPE_FOOTBALL:
				case ACTOR_TYPE_FOOTBALL_GOAL:
					if (surfType == 1) {
						if (mSoundValues.mss_state & 0x40) {
							SMSGetMSound()->startSoundActor(
							    MSD_SE_MA_TAKE_BARREL, &mPosition, 0, nullptr,
							    0, 4);
							mSoundValues.mss_state ^= 0x40;
						}
					} else if (mSoundValues.mss_state & 0x40) {
						SMSGetMSound()->startSoundActor(
						    MSD_SE_MA_PUT_BARREL, &mPosition, 0, nullptr, 0, 4);
						mSoundValues.mss_state ^= 0x40;
					}
					break;
				}
			}
		} else {
			mSoundValues.mss_state |= 0x40;
		}

		if (mStatus == MARIO_STATUS_FOOT_DOWN) {
			if (mSoundValues.mss_state & 4) {
				SMSGetMSound()->startSoundActor(MSD_SE_MA_UMARI, &mPosition, 0,
				                                nullptr, 0, 4);
				mSoundValues.mss_state ^= 4;
			}
		} else {
			mSoundValues.mss_state |= 4;
		}

		if (mStatus == MARIO_STATUS_FOOT_DOWN
		    && mSoundValues.oldstatus != MARIO_STATUS_FOOT_DOWN) {
			if (!onYoshi())
				SMSGetMSound()->startMarioVoice(MSD_SE_MV03_DAMAGE_LITLE_01,
				                                mHealth, getVoiceStatus());
			SMSGetMSound()->stopMarioVoice(getVoiceStatus(), 0);
		}

		BOOL doVoice = mSoundValues.oldstatus == MARIO_STATUS_THROWN_DOWN
		               && (!(mInput & 4) && (mSoundValues.oldaction & 4));

		if (mSoundValues.boundtimer != 0)
			mSoundValues.boundtimer -= 1;

		if (doVoice) {
			SMSGetMSound()->startSoundActor(MSD_SE_MA_FALL_AFTER, &mPosition, 0,
			                                nullptr, 0, 4);
			if (mSoundValues.boundtimer == 0) {
				if (!onYoshi()) {
					SMSGetMSound()->startMarioVoice(MSD_SE_MV03_DAMAGE_LITLE_01,
					                                mHealth, getVoiceStatus());
				}
				mSoundValues.boundtimer = 0xB4;
			}
		}

		if (curStatus == MARIO_STATUS_THROWN_DOWN)
			mSoundValues.timer += 1;
		else
			mSoundValues.timer = 0;

		switch (mSoundValues.latesthitactor) {
		case ACTOR_TYPE_POI_HANA:
			if (mSoundValues.timer == 0x3C && !onYoshi()) {
				SMSGetMSound()->startMarioVoice(MSD_SE_MV10B_CRY_JUMP_01,
				                                mHealth, getVoiceStatus());
			}
			break;
		case ACTOR_TYPE_BOSS_UNK14:
		case ACTOR_TYPE_BOSS_UNK15:
		default:
			if (mSoundValues.timer == 0x1E && !onYoshi()) {
				SMSGetMSound()->startMarioVoice(MSD_SE_MV25A_JUMP_HUGE_01,
				                                mHealth, getVoiceStatus());
			}
			break;
		}

		if (mColCount != 0) {
			if (mCollisions[0] != nullptr) {
				mSoundValues.lasthittimer   = 4;
				mSoundValues.latesthitactor = mCollisions[0]->mActorType;
				if (mCollisions[0]->isHitCategory(HIT_CATEGORY_NPC))
					mSoundValues.lasthitactor = 1;
				else
					mSoundValues.lasthitactor = 2;
			}
		} else {
			if (mSoundValues.lasthittimer != 0)
				mSoundValues.lasthittimer -= 1;
			if (mSoundValues.lasthittimer == 0)
				mSoundValues.lasthitactor = 0;
		}

		if (mStatus & 0x20000) {
			BOOL isDownLike
			    = (u32)(mStatus == 0x20466)
			      | (u32)(mStatus == MARIO_STATUS_JUMP_SHORT_BACK_DOWN)
			      | (u32)(mStatus == MARIO_STATUS_JUMP_BACK_DOWN);
			if (mSoundValues.oldstatus == MARIO_STATUS_RUN
			    && curStatus == MARIO_STATUS_SHORT_BACK_DOWN
			    && (mSoundValues.mss_state & 8) && !onYoshi()) {
				SMSGetMSound()->startMarioVoice(MSD_SE_MV03_DAMAGE_LITLE_01,
				                                mHealth, getVoiceStatus());
			}
			if (isDownLike != 0) {
				if ((mSoundValues.mss_state & 8) && !onYoshi()) {
					SMSGetMSound()->startMarioVoice(MSD_SE_MV03_DAMAGE_LITLE_01,
					                                mHealth, getVoiceStatus());
				}
				switch (mSoundValues.lasthitactor) {
				case 0:
					if (isDownLike != 0) {
						if (mSoundValues.mss_state & 8) {
							SMSGetMSound()->startSoundActor(
							    MSD_SE_MA_WALL_COL_HEAD, &mPosition, 0, nullptr,
							    0, 4);
							mSoundValues.mss_state ^= 8;
						}
					} else {
						mSoundValues.mss_state |= 8;
					}
					break;
				case 1:
					if (isDownLike != 0) {
						if (mSoundValues.mss_state & 8) {
							SMSGetMSound()->startSoundActor(
							    MSD_SE_MA_WALL_COL_CMN_S, &mPosition, 0,
							    nullptr, 0, 4);
							mSoundValues.mss_state ^= 8;
						}
					} else {
						mSoundValues.mss_state |= 8;
					}
					break;
				case 2:
					if (isDownLike != 0) {
						if (mSoundValues.mss_state & 8) {
							SMSGetMSound()->startSoundActor(
							    MSD_SE_MA_WALL_COL_CMN_H, &mPosition, 0,
							    nullptr, 0, 4);
							mSoundValues.mss_state ^= 8;
						}
					} else {
						mSoundValues.mss_state |= 8;
					}
					break;
				}
			}
		} else {
			mSoundValues.mss_state |= 8;
		}

		if ((mInput & 0x100) && mWaterGun->getCurrentWater() == 0) {
			SMSGetMSound()->startSoundActor(MSD_SE_PO_ACTION_ON_EMPTY,
			                                &mPosition, 0, nullptr, 0, 4);
		}

		const TWaterGun* gun = mWaterGun;
		s32 pressureScale    = (s32)(100.0f * gun->getHouseProgress());
		if (mSoundValues.oldrate == 0 && (u8)pressureScale != 0) {
			SMSGetMSound()->startSoundActor(MSD_SE_PO_HAND_OFF, &mPosition, 0,
			                                nullptr, 0, 4);
		}
		if (mSoundValues.oldrate == 0x12 && (u8)pressureScale == 0x13
		    && gun->getCurrentNozzleType() == 0) {
			SMSGetMSound()->startSoundActor(MSD_SE_PO_NOZZLE_OFF, &mPosition, 0,
			                                nullptr, 0, 4);
		}
		if (mSoundValues.oldrate == 0x31 && (u8)pressureScale == 0x32) {
			SMSGetMSound()->startSoundActor(MSD_SE_PO_DOWN_OFF, &mPosition, 0,
			                                nullptr, 0, 4);
		}
		mSoundValues.oldrate = (u8)pressureScale;

		if (isWearingCap()) {
			if (!(mSoundValues.mss_state & 0x80)) {
				SMSGetMSound()->startSoundActor(MSD_SE_MA_HAT_STOLEN,
				                                &mPosition, 0, nullptr, 0, 4);
			}
			mSoundValues.mss_state |= 0x80;
		} else {
			if (mSoundValues.mss_state & 0x80) {
				SMSGetMSound()->startSoundActor(MSD_SE_MA_TAKE_HAT, &mPosition,
				                                0, nullptr, 0, 4);
			}
			if (mSoundValues.mss_state & 0x80)
				mSoundValues.mss_state ^= 0x80;
		}

		if (curStatus == MARIO_STATUS_PULLING
		    || curStatus == MARIO_STATUS_OIL_PULLING || curStatus == 0x894) {
			u32 prevSt = mSoundValues.oldstatus;
			if (prevSt != MARIO_STATUS_PULLING
			    && prevSt != MARIO_STATUS_OIL_PULLING && prevSt != 0x894) {
				if (mHealth > 2) {
					if (!onYoshi())
						SMSGetMSound()->startMarioVoice(
						    MSD_SE_MV15_EXERT_INST_01, mHealth,
						    getVoiceStatus());
				} else if (!onYoshi()) {
					SMSGetMSound()->startMarioVoice(MSD_SE_MV34_ACTION_T_01,
					                                mHealth, getVoiceStatus());
				}
				mSoundValues.pull_timer = 0x78;
			} else if (curStatus == 0x894 && prevSt != 0x894) {
				if (mHealth > 2) {
					if (!onYoshi())
						SMSGetMSound()->startMarioVoice(
						    MSD_SE_MA_VO_JUMP_SMALL_0, mHealth,
						    getVoiceStatus());
				} else if (!onYoshi()) {
					SMSGetMSound()->startMarioVoice(MSD_SE_MV41_JUMP_T_03,
					                                mHealth, getVoiceStatus());
				}
			}
		}

		if (mSoundValues.pull_timer != 0)
			mSoundValues.pull_timer -= 1;

		if (mHeldObject != nullptr && (curStatus & 0x400)
		    && mSoundValues.pull_timer == 0 && mSoundValues.unk18 == 0
		    && !onYoshi()) {
			SMSGetMSound()->startMarioVoice(MSD_SE_MV16_EXERT_CONT_01, mHealth,
			                                getVoiceStatus());
		}

		if (mGroundPlane != nullptr) {
			if (mGroundPlane->isWaterSurface() || mGroundPlane->isSea())
				hasShineHolder = false;
			else
				hasShineHolder = true;
		}

		if (curStatus & 0x10000) {
			if (!(mSoundValues.oldstatus & 0x10000)) {
				mSoundValues.stimer = 0x1E;
				if (mSoundValues.mss_state & 0x200)
					mSoundValues.mss_state ^= 0x200;
			}
			if (mSoundValues.stimer != 0) {
				SMSGetMSound()->startSoundActor(MSD_SE_MA_SURF_TAME, &mPosition,
				                                0, nullptr, 0, 4);
				mSoundValues.stimer -= 1;
				if (mSoundValues.stimer == 0) {
					SMSGetMSound()->startSoundActor(MSD_SE_MA_SURF_START_ACCEL,
					                                &mPosition, 0, nullptr, 0,
					                                4);
				}
			}
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_MA_SURF_AIR, &mPosition, nullptr, mForwardVel, 0, 0,
			    nullptr, 0, 4);

		} else {
			mSoundValues.stimer = 0;
			mSoundValues.mss_state |= 0x200;
		}

		switch (curStatus) {
		case MARIO_STATUS_JUMP_BACK_DOWN:
			break;
		case MARIO_STATUS_SURF:
			mSoundValues.mss_state |= 0x100;
			if (mSoundValues.oldstatus == 0x281089A) {
				if (!hasShineHolder) {
					SMSGetMSound()->startSoundActor(
					    MSD_SE_MA_SURF_JUMP, &mPosition, 0, nullptr, 0, 4);
				}
			} else {
				u32 sfx = hasShineHolder ? MSD_SE_MA_SURF_SLIP
				                         : MSD_SE_MA_SURF_WATER;
				if (mSoundValues.stimer == 0) {
					SMSGetMSound()->startSoundActorWithInfo(
					    sfx, &mPosition, nullptr, mForwardVel, 0, 0, nullptr, 0,
					    4);
				}
			}
			break;

		case 0x281089A:
			if (!hasShineHolder && (mSoundValues.mss_state & 0x100)) {
				SMSGetMSound()->startSoundActor(MSD_SE_MA_SURF_JUMP, &mPosition,
				                                0, nullptr, 0, 4);
				mSoundValues.mss_state ^= 0x100;
			}
			break;
		}

		if (checkFlag(MARIO_FLAG_VISIBLE)) {
			if (curStatus == MARIO_STATUS_FORCE_JUMP
			    && mSoundValues.oldstatus != MARIO_STATUS_FORCE_JUMP) {
				if (mHealth > 2) {
					if (!onYoshi())
						SMSGetMSound()->startMarioVoice(
						    MSD_SE_MV24_JUMP_SPECIAL_01, mHealth,
						    getVoiceStatus());
				} else if (!onYoshi()) {
					SMSGetMSound()->startMarioVoice(MSD_SE_MV42_JUMP_HUGE_T_02,
					                                mHealth, getVoiceStatus());
				}
			}
		} else if (curStatus == MARIO_STATUS_FORCE_JUMP
		           && mSoundValues.oldstatus != MARIO_STATUS_FORCE_JUMP
		           && SMSGetApplication()->getCurrArea().getStage() == 2) {
			SMSGetMSound()->startSoundActor(MSD_SE_MA_ROPE_JUMP_C, &mPosition,
			                                0, nullptr, 0, 4);
			if (mHealth > 2) {
				if (!onYoshi())
					SMSGetMSound()->startMarioVoice(MSD_SE_MV24_JUMP_SPECIAL_01,
					                                mHealth, getVoiceStatus());
			} else if (!onYoshi()) {
				SMSGetMSound()->startMarioVoice(MSD_SE_MV42_JUMP_HUGE_T_02,
				                                mHealth, getVoiceStatus());
			}
		}

		if ((mSoundValues.oldstatus & 0x800)
		    && curStatus == MARIO_STATUS_HANGING) {
			if (mHealth > 2) {
				if (!onYoshi())
					SMSGetMSound()->startMarioVoice(MSD_SE_MV15_EXERT_INST_02,
					                                mHealth, getVoiceStatus());
			} else if (!onYoshi()) {
				SMSGetMSound()->startMarioVoice(MSD_SE_MV34_ACTION_T_02,
				                                mHealth, getVoiceStatus());
			}
		}

		if (mSoundValues.oldstatus != MARIO_STATUS_TAKEN
		    && curStatus == MARIO_STATUS_TAKEN && !onYoshi()) {
			SMSGetMSound()->startMarioVoice(MSD_SE_MV06_DAMAGE_WATER_01,
			                                mHealth, getVoiceStatus());
		}

		if (mSoundValues.oldstatus == MARIO_STATUS_LAND_SLIP
		    && curStatus == MARIO_STATUS_LANDING && !onYoshi()) {
			SMSGetMSound()->startMarioVoice(MSD_SE_MV_UNTEI_03, mHealth,
			                                getVoiceStatus());
		}

		u32 prev500_2 = mSoundValues.oldstatus;
		if (prev500_2 & MARIO_STATUS_FLAG_JUMPING) {
			switch (curStatus) {
			case MARIO_STATUS_WIRE_HANGING:
			case MARIO_STATUS_WIRE_WAIT_TO_HANG:
			case MARIO_STATUS_WIRE_S_WAIT_TO_HANG:
			case MARIO_STATUS_WIRE_RETURN:
				if (mHealth > 2) {
					if (!onYoshi())
						SMSGetMSound()->startMarioVoice(
						    MSD_SE_MV15_EXERT_INST_02, mHealth,
						    getVoiceStatus());
				} else if (!onYoshi()) {
					SMSGetMSound()->startMarioVoice(MSD_SE_MV33_T_05, mHealth,
					                                getVoiceStatus());
				}
				break;
			case MARIO_STATUS_WIRE_WAIT:
			case MARIO_STATUS_WIRE_S_WAIT:
			case MARIO_STATUS_WIRE_WAIT_TO_S_WAIT_L:
			case MARIO_STATUS_WIRE_WAIT_TO_S_WAIT_R:
				if ((u32)(prev500_2 - 0x892) > 1) {
					if (mHealth > 2) {
						if (!onYoshi())
							SMSGetMSound()->startMarioVoice(
							    MSD_SE_MV15_EXERT_INST_01, mHealth,
							    getVoiceStatus());
					} else if (!onYoshi()) {
						SMSGetMSound()->startMarioVoice(
						    MSD_SE_MV33_T_06, mHealth, getVoiceStatus());
					}
				}
				break;
			}
		}

		if ((curStatus == MARIO_STATUS_SLIP_FORE
		     || curStatus == MARIO_STATUS_SLIP_BACK)
		    && mSoundValues.unk18 == 0 && !isRunningInWater()) {
			SMSGetMSound()->startSoundActor(MSD_SE_MA_SLIP, &mPosition, 0,
			                                nullptr, 0, 4);
		}

		if (mSoundValues.oldstatus == MARIO_STATUS_HIP_DROP
		    && curStatus != MARIO_STATUS_HIP_DROP) {
			if (!(curStatus & MARIO_STATUS_FLAG_UNK20000)
			    && !(curStatus & MARIO_STATUS_FLAG_UNK80000000)) {
				SMSGetMSound()->startSoundActor(MSD_SE_MA_HIP_ATTACK,
				                                &mPosition, 0, nullptr, 0, 4);
			}
		} else if (curStatus == MARIO_STATUS_KICK_ROOF_ROLL_DOWN
		           && mSoundValues.oldstatus == MARIO_STATUS_HIP_DROP) {
			SMSGetMSound()->startSoundActor(MSD_SE_MA_HIP_ATTACK, &mPosition, 0,
			                                nullptr, 0, 4);
		}

		u32 statLo = mStatus & MARIO_STATUS_TYPE_AND_ID_MASK;
		if (statLo >= 0x150 && statLo <= 0x15C
		    && mStatus != MARIO_STATUS_WIRE_ROLLING) {
			if (mWireBounceVelPrev > 0.0f && mWireBounceVel <= 0.0f) {
				SMSGetMSound()->startSoundActorWithInfo(
				    MSD_SE_OBJ_ROPE_CLEAK_A, &mWireStartPos, nullptr, mWireSag,
				    0, 0, nullptr, 0, 4);
				mWireSfxTimer    = mWireSfxDelay;
				mWireQueuedSfxID = MSD_SE_OBJ_ROPE_CLEAK_A;
			}
			if (mWireBounceVelPrev < 0.0f && mWireBounceVel >= 0.0f) {
				SMSGetMSound()->startSoundActorWithInfo(
				    MSD_SE_OBJ_ROPE_CLEAK_B, &mWireStartPos, nullptr, mWireSag,
				    0, 0, nullptr, 0, 4);
				mWireSfxTimer    = mWireSfxDelay;
				mWireQueuedSfxID = MSD_SE_OBJ_ROPE_CLEAK_B;
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

		if (mStatus == MARIO_STATUS_WARP_IN && mStatusState == 0
		    && mStatusTimer == 1)
			SMSGetMSound()->startSoundActor(MSD_SE_MA_WARP, &mPosition, 0,
			                                nullptr, 0, 4);

		if (mStatus == MARIO_STATUS_ELECTRIC_DAMAGE && mStatusState == 0)
			SMSGetMSound()->startSoundActor(MSD_SE_MA_DAMAGE_ELEC, &mPosition,
			                                0, nullptr, 0, 4);
	}

	mSoundValues.lastStatus = mStatus;
	mSoundValues.oldstatus  = curStatus;
	mSoundValues.oldaction  = mInput;
	mSoundValues.oldflag    = unk78;
}
#else
void TMario::soundMovement()
{
	static u32 oldstatus      = 0;
	static int mss_state      = -1;
	static u32 lastStatus     = 0;
	u32 curStatus             = mStatus;
	static int oldaction      = 0;
	static int oldflag        = 0;
	static int latesthitactor = 0;
	static u8 quarter_timer   = 0;
	bool hasShineHolder       = true;

	quarter_timer += 1;
	if (quarter_timer == 4)
		quarter_timer = 0;

	if (onYoshi()) {
		if (mWaterGun->isEmitting()
		    && mWaterGun->getCurrentNozzle()->getUnk378() > 0.0f) {
			SMSGetMSound()->startSoundActor(
			    MSD_SE_YV_WATER, &mYoshi->mTranslation, 0, nullptr, 0, 4);
		}

		if (curStatus & MARIO_STATUS_FLAG_UNK40000) {
			SMSGetMSound()->startSoundActor(
			    MSD_SE_MA_SLIP, &mYoshi->mTranslation, 0, nullptr, 0, 4);
			if (curStatus == MARIO_STATUS_SLIP_FORE
			    && oldstatus != MARIO_STATUS_SLIP_FORE) {
				SMSGetMSound()->startSoundActor(
				    MSD_SE_YV_FUNBARI, &mYoshi->mTranslation, 0, nullptr, 0, 4);
			}
		} else if (oldstatus == MARIO_STATUS_SLIP_FORE) {
			SMSGetMSound()->startSoundActor(
			    MSD_SE_YV_PURU_PURU2, &mYoshi->mTranslation, 0, nullptr, 0, 4);
		}

		if ((curStatus & MARIO_STATUS_FLAG_JUMPING)
		    && !(oldstatus & MARIO_STATUS_FLAG_JUMPING)) {
			SMSGetMSound()->startSoundActor(
			    MSD_SE_YV_JUMP1, &mYoshi->mTranslation, 0, nullptr, 0, 4);
		}

		if (curStatus == MARIO_STATUS_HIP_DROP
		    && oldstatus != MARIO_STATUS_HIP_DROP) {
			SMSGetMSound()->startSoundActor(MSD_SE_YV_PURU_POWERFUL,
			                                &mYoshi->mTranslation, 0, nullptr,
			                                0, 4);
		}

		if (curStatus == MARIO_STATUS_HIP_ATTACK_END
		    && oldstatus == MARIO_STATUS_HIP_DROP) {
			SMSGetMSound()->startSoundActor(
			    MSD_SE_YO_HIP_ATTACK, &mYoshi->mTranslation, 0, nullptr, 0, 4);
		}

		if (oldstatus != MARIO_STATUS_HIP_DROP) {
			BOOL inputBit   = !!(mInput & 4);
			BOOL prevInpBit = !!(oldaction & 4);
			static u32 jtimer;
			static u8 callsound_delay = 0;

			if (inputBit != 0 && prevInpBit == 0)
				jtimer = 0;
			if (inputBit != 0)
				jtimer += 1;

			if (callsound_delay != 0) {
				callsound_delay -= 1;
				if (callsound_delay == 0) {
					if (inputBit == 0) {
						SMSGetMSound()->startSoundActor(MSD_SE_MA_BOUND,
						                                &mYoshi->mTranslation,
						                                0, nullptr, 0, 4);
					}
				} else if (callsound_delay == 4) {
					if (inputBit == 0) {
						SMSGetMSound()->startSoundActor(MSD_SE_MA_BOUND,
						                                &mYoshi->mTranslation,
						                                0, nullptr, 0, 4);
						SMSGetMSound()->startMarioVoice(MSD_SE_YV_PURU_PURU2, 1,
						                                1);
					}
					callsound_delay = 0;
				}
			}

			if (inputBit == 0 && prevInpBit != 0) {
				if (jtimer > 0x78 && mVel.y < -74.0f)
					callsound_delay = 8;
				else
					callsound_delay = 4;
			}
		}
	} else {
		static u16 spin_timer = 0;
		BOOL isRotJump        = (mStatus == MARIO_STATUS_LEFT_ROTATE_JUMP)
		                 | (mStatus == MARIO_STATUS_RIGHT_ROTATE_JUMP);

		if (isRotJump) {
			if (mss_state & 0x10) {
				if (!onYoshi())
					SMSGetMSound()->startMarioVoice(MSD_SE_MV24_JUMP_SPECIAL_01,
					                                mHealth, getVoiceStatus());

				mss_state ^= 0x10;
			}
		} else {
			mss_state |= 0x10;
		}

		if (isRotJump) {
			spin_timer += 1;
			if (spin_timer == 10)
				SMSGetMSound()->startSoundActor(MSD_SE_MA_ROLL_JUMP, &mPosition,
				                                0, nullptr, 0, 4);
		} else {
			spin_timer = 0;
		}

		if (mStatus == MARIO_STATUS_FORCE_JUMP
		    && lastStatus != MARIO_STATUS_FORCE_JUMP) {
			if (mGroundPlane != nullptr
			    && mGroundPlane->getActor() != nullptr) {
				startForceJumpSound2(&mPosition, mSoundFlags, 0.0f,
				                     mGroundPlane->getData());
			}
		}

		u8 surfType = 0;
		u32 prev500 = oldstatus;
		if (prev500 == MARIO_STATUS_TAKE)
			surfType = 1;
		if (prev500 == MARIO_STATUS_PUTTING)
			surfType = 2;

		if (surfType != 0) {
			TTakeActor* held = mHeldObject;
			if (held != nullptr) {
				switch (held->mActorType) {
				case ACTOR_TYPE_FRUIT_COCONUT:
				case ACTOR_TYPE_FRUIT_PAPAYA:
				case ACTOR_TYPE_FRUIT_PINE:
				case ACTOR_TYPE_FRUIT_DURIAN:
				case ACTOR_TYPE_FRUIT_BANANA:
				case ACTOR_TYPE_RED_PEPPER:
				case ACTOR_TYPE_FRUIT_COVER_PINE:
					if (surfType == 1) {
						if (mss_state & 0x40) {
							SMSGetMSound()->startSoundActor(
							    MSD_SE_MA_TAKE_DRIAN, &mPosition, 0, nullptr, 0,
							    4);
							mss_state ^= 0x40;
						}
					} else if (mss_state & 0x40) {
						SMSGetMSound()->startSoundActor(
						    MSD_SE_MA_PUT_DRIAN, &mPosition, 0, nullptr, 0, 4);
						mss_state ^= 0x40;
					}
					break;
				case ACTOR_TYPE_PLANT_FLOWER:
				case ACTOR_TYPE_WOOD_BARREL:
				case ACTOR_TYPE_BARREL_FLOAT:
				case ACTOR_TYPE_BARREL_OIL:
				case ACTOR_TYPE_DRUM_CAN:
				case ACTOR_TYPE_FOOTBALL:
				case ACTOR_TYPE_FOOTBALL_GOAL:
					if (surfType == 1) {
						if (mss_state & 0x40) {
							SMSGetMSound()->startSoundActor(
							    MSD_SE_MA_TAKE_BARREL, &mPosition, 0, nullptr,
							    0, 4);
							mss_state ^= 0x40;
						}
					} else if (mss_state & 0x40) {
						SMSGetMSound()->startSoundActor(
						    MSD_SE_MA_PUT_BARREL, &mPosition, 0, nullptr, 0, 4);
						mss_state ^= 0x40;
					}
					break;
				}
			}
		} else {
			mss_state |= 0x40;
		}

		if (mStatus == MARIO_STATUS_FOOT_DOWN) {
			if (mss_state & 4) {
				SMSGetMSound()->startSoundActor(MSD_SE_MA_UMARI, &mPosition, 0,
				                                nullptr, 0, 4);
				mss_state ^= 4;
			}
		} else {
			mss_state |= 4;
		}

		BOOL doVoice = oldstatus == MARIO_STATUS_THROWN_DOWN
		               && (!(mInput & 4) && (oldaction & 4));

		static u8 boundtimer = 0;
		if (boundtimer != 0)
			boundtimer -= 1;

		if (doVoice) {
			SMSGetMSound()->startSoundActor(MSD_SE_MA_FALL_AFTER, &mPosition, 0,
			                                nullptr, 0, 4);
			if (boundtimer == 0) {
				if (!onYoshi()) {
					SMSGetMSound()->startMarioVoice(MSD_SE_MV03_DAMAGE_LITLE_01,
					                                mHealth, getVoiceStatus());
				}
				boundtimer = 0xB4;
			}
		}

		static u16 timer = 0;
		if (curStatus == MARIO_STATUS_THROWN_DOWN)
			timer += 1;
		else
			timer = 0;

		switch (latesthitactor) {
		case ACTOR_TYPE_POI_HANA:
			if (timer == 0x3C && !onYoshi()) {
				SMSGetMSound()->startMarioVoice(MSD_SE_MV10B_CRY_JUMP_01,
				                                mHealth, getVoiceStatus());
			}
			break;
		case ACTOR_TYPE_BOSS_UNK14:
		case ACTOR_TYPE_BOSS_UNK15:
		default:
			if (timer == 0x1E && !onYoshi()) {
				SMSGetMSound()->startMarioVoice(MSD_SE_MV25A_JUMP_HUGE_01,
				                                mHealth, getVoiceStatus());
			}
			break;
		}

		static u8 lasthitactor = 0;
		static u8 lasthittimer = 0;
		if (mColCount != 0) {
			if (mCollisions[0] != nullptr) {
				lasthittimer   = 4;
				latesthitactor = mCollisions[0]->mActorType;
				if (mCollisions[0]->isHitCategory(HIT_CATEGORY_NPC))
					lasthitactor = 1;
				else
					lasthitactor = 2;
			}
		} else {
			if (lasthittimer != 0)
				lasthittimer -= 1;
			if (lasthittimer == 0)
				lasthitactor = 0;
		}

		if (mStatus & 0x20000) {
			BOOL isDownLike
			    = (u32)(mStatus == 0x20466)
			      | (u32)(mStatus == MARIO_STATUS_JUMP_SHORT_BACK_DOWN)
			      | (u32)(mStatus == MARIO_STATUS_JUMP_BACK_DOWN);
			if (isDownLike != 0) {
				if ((mss_state & 8) && !onYoshi()) {
					SMSGetMSound()->startMarioVoice(MSD_SE_MV03_DAMAGE_LITLE_01,
					                                mHealth, getVoiceStatus());
				}
				switch (lasthitactor) {
				case 0:
					if (isDownLike != 0) {
						if (mss_state & 8) {
							SMSGetMSound()->startSoundActor(
							    MSD_SE_MA_WALL_COL_HEAD, &mPosition, 0, nullptr,
							    0, 4);
							mss_state ^= 8;
						}
					} else {
						mss_state |= 8;
					}
					break;
				case 1:
					if (isDownLike != 0) {
						if (mss_state & 8) {
							SMSGetMSound()->startSoundActor(
							    MSD_SE_MA_WALL_COL_CMN_S, &mPosition, 0,
							    nullptr, 0, 4);
							mss_state ^= 8;
						}
					} else {
						mss_state |= 8;
					}
					break;
				case 2:
					if (isDownLike != 0) {
						if (mss_state & 8) {
							SMSGetMSound()->startSoundActor(
							    MSD_SE_MA_WALL_COL_CMN_H, &mPosition, 0,
							    nullptr, 0, 4);
							mss_state ^= 8;
						}
					} else {
						mss_state |= 8;
					}
					break;
				}
			}
		} else {
			mss_state |= 8;
		}

		if ((mInput & 0x100) && mWaterGun->getCurrentWater() == 0) {
			SMSGetMSound()->startSoundActor(MSD_SE_PO_ACTION_ON_EMPTY,
			                                &mPosition, 0, nullptr, 0, 4);
		}

		static u8 oldrate;
		const TWaterGun* gun = mWaterGun;
		s32 pressureScale    = (s32)(100.0f * gun->getHouseProgress());
		if (oldrate == 0 && (u8)pressureScale != 0) {
			SMSGetMSound()->startSoundActor(MSD_SE_PO_HAND_OFF, &mPosition, 0,
			                                nullptr, 0, 4);
		}
		if (oldrate == 0x12 && (u8)pressureScale == 0x13
		    && gun->getCurrentNozzleType() == 0) {
			SMSGetMSound()->startSoundActor(MSD_SE_PO_NOZZLE_OFF, &mPosition, 0,
			                                nullptr, 0, 4);
		}
		if (oldrate == 0x31 && (u8)pressureScale == 0x32) {
			SMSGetMSound()->startSoundActor(MSD_SE_PO_DOWN_OFF, &mPosition, 0,
			                                nullptr, 0, 4);
		}
		oldrate = (u8)pressureScale;

		if (isWearingCap()) {
			if (!(mss_state & 0x80)) {
				SMSGetMSound()->startSoundActor(MSD_SE_MA_HAT_STOLEN,
				                                &mPosition, 0, nullptr, 0, 4);
			}
			mss_state |= 0x80;
		} else {
			if (mss_state & 0x80) {
				SMSGetMSound()->startSoundActor(MSD_SE_MA_TAKE_HAT, &mPosition,
				                                0, nullptr, 0, 4);
			}
			if (mss_state & 0x80)
				mss_state ^= 0x80;
		}

		static u8 pull_timer = 0;
		if (curStatus == MARIO_STATUS_PULLING
		    || curStatus == MARIO_STATUS_OIL_PULLING || curStatus == 0x894) {
			u32 prevSt = oldstatus;
			if (prevSt != MARIO_STATUS_PULLING
			    && prevSt != MARIO_STATUS_OIL_PULLING && prevSt != 0x894) {
				if (mHealth > 2) {
					if (!onYoshi())
						SMSGetMSound()->startMarioVoice(
						    MSD_SE_MV15_EXERT_INST_01, mHealth,
						    getVoiceStatus());
				} else if (!onYoshi()) {
					SMSGetMSound()->startMarioVoice(MSD_SE_MV34_ACTION_T_01,
					                                mHealth, getVoiceStatus());
				}
				pull_timer = 0x78;
			} else if (curStatus == 0x894 && prevSt != 0x894) {
				if (mHealth > 2) {
					if (!onYoshi())
						SMSGetMSound()->startMarioVoice(
						    MSD_SE_MA_VO_JUMP_SMALL_0, mHealth,
						    getVoiceStatus());
				} else if (!onYoshi()) {
					SMSGetMSound()->startMarioVoice(MSD_SE_MV41_JUMP_T_03,
					                                mHealth, getVoiceStatus());
				}
			}
		}

		if (pull_timer != 0)
			pull_timer -= 1;

		if (mHeldObject != nullptr && (curStatus & 0x400) && pull_timer == 0
		    && quarter_timer == 0 && !onYoshi()) {
			SMSGetMSound()->startMarioVoice(MSD_SE_MV16_EXERT_CONT_01, mHealth,
			                                getVoiceStatus());
		}

		static u8 stimer = 0;
		if (mGroundPlane != nullptr) {
			if (mGroundPlane->isWaterSurface() || mGroundPlane->isSea())
				hasShineHolder = false;
			else
				hasShineHolder = true;
		}

		if (curStatus & 0x10000) {
			if (!(oldstatus & 0x10000)) {
				stimer = 0x1E;
				if (mss_state & 0x200)
					mss_state ^= 0x200;
			}
			if (stimer != 0) {
				SMSGetMSound()->startSoundActor(MSD_SE_MA_SURF_TAME, &mPosition,
				                                0, nullptr, 0, 4);
				stimer -= 1;
				if (stimer == 0) {
					SMSGetMSound()->startSoundActor(MSD_SE_MA_SURF_START_ACCEL,
					                                &mPosition, 0, nullptr, 0,
					                                4);
				}
			}
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_MA_SURF_AIR, &mPosition, nullptr, mForwardVel, 0, 0,
			    nullptr, 0, 4);

		} else {
			stimer = 0;
			mss_state |= 0x200;
		}

		switch (curStatus) {
		case MARIO_STATUS_JUMP_BACK_DOWN:
			break;
		case MARIO_STATUS_SURF:
			mss_state |= 0x100;
			if (oldstatus == 0x281089A) {
				if (!hasShineHolder) {
					SMSGetMSound()->startSoundActor(
					    MSD_SE_MA_SURF_JUMP, &mPosition, 0, nullptr, 0, 4);
				}
			} else {
				u32 sfx = hasShineHolder ? MSD_SE_MA_SURF_SLIP
				                         : MSD_SE_MA_SURF_WATER;
				if (stimer == 0) {
					SMSGetMSound()->startSoundActorWithInfo(
					    sfx, &mPosition, nullptr, mForwardVel, 0, 0, nullptr, 0,
					    4);
				}
			}
			break;

		case 0x281089A:
			if (!hasShineHolder && (mss_state & 0x100)) {
				SMSGetMSound()->startSoundActor(MSD_SE_MA_SURF_JUMP, &mPosition,
				                                0, nullptr, 0, 4);
				mss_state ^= 0x100;
			}
			break;
		}

		if (checkFlag(MARIO_FLAG_VISIBLE)) {
			if (curStatus == MARIO_STATUS_FORCE_JUMP
			    && oldstatus != MARIO_STATUS_FORCE_JUMP) {
				if (mHealth > 2) {
					if (!onYoshi())
						SMSGetMSound()->startMarioVoice(
						    MSD_SE_MV24_JUMP_SPECIAL_01, mHealth,
						    getVoiceStatus());
				} else if (!onYoshi()) {
					SMSGetMSound()->startMarioVoice(MSD_SE_MV42_JUMP_HUGE_T_02,
					                                mHealth, getVoiceStatus());
				}
			}
		}

		if ((oldstatus & 0x800) && curStatus == MARIO_STATUS_HANGING) {
			if (mHealth > 2) {
				if (!onYoshi())
					SMSGetMSound()->startMarioVoice(MSD_SE_MV15_EXERT_INST_02,
					                                mHealth, getVoiceStatus());
			} else if (!onYoshi()) {
				SMSGetMSound()->startMarioVoice(MSD_SE_MV34_ACTION_T_02,
				                                mHealth, getVoiceStatus());
			}
		}

		if (oldstatus != MARIO_STATUS_TAKEN && curStatus == MARIO_STATUS_TAKEN
		    && !onYoshi()) {
			SMSGetMSound()->startMarioVoice(MSD_SE_MV06_DAMAGE_WATER_01,
			                                mHealth, getVoiceStatus());
		}

		if (oldstatus == MARIO_STATUS_LAND_SLIP
		    && curStatus == MARIO_STATUS_LANDING && !onYoshi()) {
			SMSGetMSound()->startMarioVoice(MSD_SE_MV_UNTEI_03, mHealth,
			                                getVoiceStatus());
		}

		u32 prev500_2 = oldstatus;
		if (prev500_2 & MARIO_STATUS_FLAG_JUMPING) {
			switch (curStatus) {
			case MARIO_STATUS_WIRE_HANGING:
			case MARIO_STATUS_WIRE_WAIT_TO_HANG:
			case MARIO_STATUS_WIRE_S_WAIT_TO_HANG:
			case MARIO_STATUS_WIRE_RETURN:
				if (mHealth > 2) {
					if (!onYoshi())
						SMSGetMSound()->startMarioVoice(
						    MSD_SE_MV15_EXERT_INST_02, mHealth,
						    getVoiceStatus());
				} else if (!onYoshi()) {
					SMSGetMSound()->startMarioVoice(MSD_SE_MV33_T_05, mHealth,
					                                getVoiceStatus());
				}
				break;
			case MARIO_STATUS_WIRE_WAIT:
			case MARIO_STATUS_WIRE_S_WAIT:
			case MARIO_STATUS_WIRE_WAIT_TO_S_WAIT_L:
			case MARIO_STATUS_WIRE_WAIT_TO_S_WAIT_R:
				if ((u32)(prev500_2 - 0x892) > 1) {
					if (mHealth > 2) {
						if (!onYoshi())
							SMSGetMSound()->startMarioVoice(
							    MSD_SE_MV15_EXERT_INST_01, mHealth,
							    getVoiceStatus());
					} else if (!onYoshi()) {
						SMSGetMSound()->startMarioVoice(
						    MSD_SE_MV33_T_06, mHealth, getVoiceStatus());
					}
				}
				break;
			}
		}

		if ((curStatus == MARIO_STATUS_SLIP_FORE
		     || curStatus == MARIO_STATUS_SLIP_BACK)
		    && quarter_timer == 0 && !isRunningInWater()) {
			SMSGetMSound()->startSoundActor(MSD_SE_MA_SLIP, &mPosition, 0,
			                                nullptr, 0, 4);
		}

		if (oldstatus == MARIO_STATUS_HIP_DROP
		    && curStatus != MARIO_STATUS_HIP_DROP) {
			if (!(curStatus & MARIO_STATUS_FLAG_UNK20000)
			    && !(curStatus & MARIO_STATUS_FLAG_UNK80000000)) {
				SMSGetMSound()->startSoundActor(MSD_SE_MA_HIP_ATTACK,
				                                &mPosition, 0, nullptr, 0, 4);
			}
		} else if (curStatus == MARIO_STATUS_KICK_ROOF_ROLL_DOWN
		           && oldstatus == MARIO_STATUS_HIP_DROP) {
			SMSGetMSound()->startSoundActor(MSD_SE_MA_HIP_ATTACK, &mPosition, 0,
			                                nullptr, 0, 4);
		}

		u32 statLo = mStatus & MARIO_STATUS_TYPE_AND_ID_MASK;
		if (statLo >= 0x150 && statLo <= 0x15C
		    && mStatus != MARIO_STATUS_WIRE_ROLLING) {
			if (mWireBounceVelPrev > 0.0f && mWireBounceVel <= 0.0f) {
				SMSGetMSound()->startSoundActorWithInfo(
				    MSD_SE_OBJ_ROPE_CLEAK_A, &mWireStartPos, nullptr, mWireSag,
				    0, 0, nullptr, 0, 4);
				mWireSfxTimer    = mWireSfxDelay;
				mWireQueuedSfxID = MSD_SE_OBJ_ROPE_CLEAK_A;
			}
			if (mWireBounceVelPrev < 0.0f && mWireBounceVel >= 0.0f) {
				SMSGetMSound()->startSoundActorWithInfo(
				    MSD_SE_OBJ_ROPE_CLEAK_B, &mWireStartPos, nullptr, mWireSag,
				    0, 0, nullptr, 0, 4);
				mWireSfxTimer    = mWireSfxDelay;
				mWireQueuedSfxID = MSD_SE_OBJ_ROPE_CLEAK_B;
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

		if (mStatus == MARIO_STATUS_WARP_IN && mStatusState == 0
		    && mStatusTimer == 1)
			SMSGetMSound()->startSoundActor(MSD_SE_MA_WARP, &mPosition, 0,
			                                nullptr, 0, 4);

		if (mStatus == MARIO_STATUS_ELECTRIC_DAMAGE && mStatusState == 0)
			SMSGetMSound()->startSoundActor(MSD_SE_MA_DAMAGE_ELEC, &mPosition,
			                                0, nullptr, 0, 4);
	}

	lastStatus = mStatus;
	oldstatus  = curStatus;
	oldaction  = mInput;
	oldflag    = unk78;
}
#endif

void TMario::animSound()
{
	mSoundFlags = mGroundPlane->mSoundMaterial;

	if (checkFlag(MARIO_FLAG_DIRTY)) {
		if (mPollutionTypeStandingOn == POLLUTION_TYPE_SINK
		    && mSinkTimer >= mGraffitoParams.mSinkTime.get()
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

	if (isSinking()) {
		if (-(mSinkTimer / mGraffitoParams.mSinkTime.get())
		        * mGraffitoParams.mSinkHeight.get()
		    > 30.0f)
			mSoundFlags |= 0x400;
		else
			mSoundFlags |= 0x300;
	}

	if (isOnWire())
		mSoundFlags |= 0x15;

	// TODO: huh?
	if (checkFlag(MARIO_FLAG_HAS_FLUDD))
		mSoundFlags |= 0x0;
	else
		mSoundFlags |= 0x1000;

	if (mPlayerType == PLAYER_TYPE_SHADOW_MARIO)
		mSoundFlags |= 0x20000000;
	else if (mPlayerType == PLAYER_TYPE_MONTE_MAN)
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
	f32 len = (mPosition - mToroccoPos).length();
	SMSGetMSound()->startSoundActorWithInfo(MSD_SE_OBJ_JET_COASTER, &mPosition,
	                                        nullptr, len, 0, 0, nullptr, 0, 4);
}

u8 TMario::getVoiceStatus()
{
	if (onYoshi())
		return 1;

	switch (mPlayerType) {
	case PLAYER_TYPE_SHADOW_MARIO:
		return 2;
	case PLAYER_TYPE_MONTE_MAN:
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
	oldstatus       = 0;
	mss_state       = -1;
	lastStatus      = 0;
	oldaction       = 0;
	oldflag         = 0;
	latesthitactor  = 0;
	unk18           = 0;
	jtimer          = 0;
	callsound_delay = 0;
	spin_timer      = 0;
	boundtimer      = 0;
	timer           = 0;
	latesthitactor  = 0;
	lasthittimer    = 0;
	oldrate         = 1;
	pull_timer      = 0;
	stimer          = 0;
}
