#!/usr/bin/env python3
"""Map raw field offsets to named TParams fields across Mario source files."""
import re
import os

def params_size(num_fields):
    return 0x08 + num_fields * 0x14

structs = {}

structs['TDeParams'] = [
    ('mHpMax', '8'), ('mRunningMax', '45.0f'), ('mDashMax', '60.0f'),
    ('mDashAcc', '0.5f'), ('mDashBrake', '0.8f'), ('mDashStartTime', '0x78'),
    ('mWaitingRotSp', '0x100'), ('mRunningRotSpMin', '0x200'), ('mRunningRotSpMax', '0x400'),
    ('mRocketRotSp', '0x100'), ('mPumpingRotSpMin', '0x100'), ('mPumpingRotSpMax', '0x200'),
    ('mInvincibleTime', '0x14'), ('mFootPrintTimerMax', '0x208'),
    ('mWaterTriggerRate', '1'), ('mGraffitoNoDmgTime', '10'), ('mRestMax', '0xff'),
    ('mShadowSize', '200.0f'), ('mShadowErase', '0.2f'), ('mHoldRadius', '100.0f'),
    ('mDamageRadius', '42.0f'), ('mDamageHeight', '110.0f'), ('mAttackHeight', '50.0f'),
    ('mTrampleRadius', '100.0f'), ('mPushupRadius', '100.0f'), ('mPushupHeight', '200.0f'),
    ('mHipdropRadius', '250.0f'), ('mQuakeRadius', '500.0f'), ('mQuakeHeight', '500.0f'),
    ('mTramplePowStep1', '30.0f'), ('mTramplePowStep2', '40.0f'), ('mTramplePowStep3', '50.0f'),
    ('mJumpWallHeight', '1000.0f'), ('mThrowPower', '2.0f'), ('mSlipStart', '0.342f'),
    ('mWasOnWaterSlip', '0.97f'), ('mInWaterSlip', '0.9f'), ('mToroccoRotSp', '1.0f'),
    ('mRecoverTimer', '0xf0'), ('mHotTimer', '0x4b0'), ('mFeelDeep', '500.0f'),
    ('mDamageFallHeight', '1000.0f'), ('mForceSlipAngle', '0.5f'), ('mClashSpeed', '50.0f'),
    ('mHangWallMovableAngle', '0.3f'), ('mColMvMax', '10.0f'),
    ('mNoFreezeTime', '0x78'), ('mKickFreezeTime', '5'), ('mSurfStartFreezeTime', '0x78'),
    ('mSleepingCheckDist', '50.0f'), ('mSleepingCheckHeight', '30.0f'),
    ('mIllegalPlaneCtInc', '4'), ('mIllegalPlaneTime', '0x1e0'),
]
structs['TBodyAngleParams'] = [
    ('mHeadRot', '0.0f'), ('mWaistRoll', '0.0f'), ('mWaistPitch', '80.0f'),
    ('mWaistRollMax', '0'), ('mWaistPitchMax', '1000'), ('mWaistAngleChangeRate', '0.07f'),
]
structs['TAttackParams'] = [('mRadius', '100.0f'), ('mHeight', '50.0f')]
structs['TJumpParams'] = [
    ('mGravity', '0.8f'), ('mSpinJumpGravity', '0.6f'), ('mJumpingMax', '80.0f'),
    ('mJumpSpeedBrake', '0.999f'), ('mJumpAccelControl', '0.5f'), ('mJumpSlideControl', '0.5f'),
    ('mTurnJumpForce', '62.0f'), ('mFenceSpeed', '2.0f'), ('mFireDownForce', '80.0f'),
    ('mFireDownControl', '1.0f'), ('mFireBackVelocity', '1.0f'), ('mBroadJumpForce', '80.0f'),
    ('mBroadJumpForceY', '30.0f'), ('mRotateJumpForceY', '62.0f'),
    ('mPopUpSpeedY', '5.0f'), ('mPopUpForceYMult', '10.0f'),
    ('mBackJumpForce', '-16.0f'), ('mBackJumpForceY', '62.0f'),
    ('mHipAttackSpeedY', '-50.0f'), ('mSuperHipAttackSpeedY', '-80.0f'),
    ('mJumpCatchRotXSp', '0x100'), ('mJumpCatchRotXMax', '0x2aaa'),
    ('mRotBroadEnableV', '30.0f'), ('mRotBroadJumpForce', '60.0f'),
    ('mRotBroadJumpForceY', '20.0f'), ('mTrampolineDec', '1.0f'),
    ('mSecJumpEnableSp', '20.0f'), ('mSecJumpForce', '52.0f'),
    ('mSecJumpSpeedMult', '0.0f'), ('mSecJumpXZMult', '0.8f'),
    ('mTriJumpEnableSp', '20.0f'), ('mUltraJumpForce', '70.0f'),
    ('mUltraJumpSpeedMult', '0.25f'), ('mUltraJumpXZMult', '0.8f'),
    ('mValleyDepth', '500.0f'), ('mThrownAccel', '0.5f'), ('mThrownSlide', '0.5f'),
    ('mThrownBrake', '0.98f'), ('mTremblePower', '5.0f'), ('mTrembleAccele', '2.0f'),
    ('mTrembleBrake', '0.99f'), ('mTrembleTime', '600'), ('mClashAngle', '0x3555'),
    ('mJumpJumpCatchSp', '50.0f'), ('mGetOffYoshiY', '30.0f'), ('mSuperHipAttackCt', '0x32'),
]
structs['TRunParams'] = [
    ('mMaxSpeed', '32.0f'), ('mVelMinusBrake', '1.1f'), ('mAddBase', '1.1f'),
    ('mAddVelDiv', '0.0233f'), ('mDecStartNrmY', '0.95f'), ('mDecBrake', '1.0f'),
    ('mSoft2Walk', '8.0f'), ('mWalk2Soft', '5.0f'), ('mSoftStepAnmMult', '0.125f'),
    ('mRunAnmSpeedBase', '1.0f'), ('mRunAnmSpeedMult', '0.06f'),
    ('mMotBlendWalkSp', '1.0f'), ('mMotBlendRunSp', '3.0f'),
    ('mSwimDepth', '120.0f'), ('mInWaterBrake', '0.9f'), ('mInWaterAnmBrake', '0.6f'),
    ('mPumpingSlideSp', '0.1f'), ('mPumpingSlideAnmSp', '0.5f'),
    ('mDoJumpCatchSp', '15.0f'), ('mTurnNeedSp', '10.0f'), ('mDashRotSp', '100'),
]
structs['TSwimParams'] = [
    ('mStartSp', '1.0f'), ('mMoveSp', '0.8f'), ('mMoveBrake', '1.0f'),
    ('mSwimmingRotSpMin', '0x200'), ('mSwimmingRotSpMax', '0x400'),
    ('mPumpingRotSpMin', '0x100'), ('mPumpingRotSpMax', '0x200'),
    ('mGravity', '0.1f'), ('mWaitBouyancy', '1.0f'), ('mMoveBouyancy', '1.0f'),
    ('mUpDownBrake', '0.95f'), ('mCanJumpDepth', '1.0f'), ('mEndDepth', '80.0f'),
    ('mFloatHeight', '120.0f'), ('mStartVMult', '0.1f'), ('mStartVYMult', '0.1f'),
    ('mRush', '3.0f'), ('mAnmBrake', '0.02f'), ('mPaddleSpeedUp', '0.3f'),
    ('mPaddleJumpUp', '1.0f'), ('mFloatUp', '2.0f'), ('mWaterLevelCheckHeight', '10.0f'),
    ('mPaddleDown', '1.0f'), ('mWaitSinkTime', '0x32'), ('mCanBreathDepth', '50.0f'),
    ('mWaitSinkSpeed', '5.0f'), ('mAirDec', '0.001f'), ('mAirDecDive', '0.001f'),
    ('mAirInc', '0.03f'),
]
structs['THangingParams'] = [
    ('mMoveSp', '0.1f'), ('mAnmRate', '0.5f'), ('mRapidTime', '2000'),
    ('mLimitTime', '0x960'), ('mAnmRapid', '8.0f'), ('mDescentSp', '10.0f'),
]
structs['THangRoofParams'] = [('mAnmMult', '0.3f')]
structs['TWireParams'] = [
    ('mRotSpeed', '-8'), ('mRotSpeedTrgHover', '8'), ('mRotSpeedTrgTurbo', '1000'),
    ('mRotSpeedTrgRocket', '1000'), ('mRotSpeedMax', '0x578'), ('mRotStop', '100'),
    ('mRotGravity', '0x14'), ('mRotBrake', '0.98f'), ('mJumpRate', '0.09f'),
    ('mSwingRate', '0.005f'), ('mWireJumpAccelControl', '0.01f'),
    ('mWireJumpSlideControl', '0.3f'), ('mWireJumpMult', '5.0f'),
    ('mWireJumpBase', '20.0f'), ('mWireSwingBrake', '0.99f'), ('mWireSwingMax', '100.0f'),
]
structs['TPullParams'] = [
    ('mPullRateV', '0.3f'), ('mPullRateH', '0.05f'),
    ('mOilPullRateV', '0.1f'), ('mOilPullRateH', '0.01f'),
]
structs['TBarParams'] = [
    ('mClimbSp', '0.035f'), ('mRotateSp', '3.0f'), ('mClimbAnmRate', '0.0039f'),
    ('mCatchRadius', '100.0f'), ('mCatchAngle', '0.8f'),
]
structs['TSurfingParams'] = [
    ('mRotMin', '2048.0f'), ('mRotMax', '1024.0f'), ('mPowMin', '24.0f'),
    ('mPowMax', '64.0f'), ('mAccel', '58.0f'), ('mWaistRoll', '0.25f'),
    ('mWaistPitch', '170.0f'), ('mWaistRollMax', '0x400'), ('mWaistPitchMax', '0x1555'),
    ('mRoll', '-0.45f'), ('mPitch', '-170.0f'), ('mRollMax', '0x4000'),
    ('mPitchMax', '0x1555'), ('mAngleChangeRate', '0.01f'),
    ('mWaistAngleChangeRate', '0.01f'), ('mScaleMin', '0.5f'), ('mScaleMax', '1.0f'),
    ('mScaleMinSpeed', '24.0f'), ('mScaleMaxSpeed', '60.0f'),
    ('mJumpPow', '42.0f'), ('mJumpXZRatio', '0.25f'),
    ('mClashSpeed', '40.0f'), ('mClashAngle', '0x5555'),
]
structs['THHoverParams'] = [('mRotSp', '0x80'), ('mAccelRate', '0.03f'), ('mBrake', '0.95f')]
structs['TDivingParams'] = [
    ('mRotSp', '0x80'), ('mGravity', '0.5f'), ('mAccelControl', '0.02f'),
    ('mSeaBrake', '0.999f'), ('mSeaBrakeY', '0.98f'),
]
structs['TYoshiParams'] = [
    ('mRunYoshiMult', '1.2f'), ('mJumpYoshiMult', '1.0f'), ('mRotYoshiMult', '1.5f'),
    ('mHeadFront', '80.0f'), ('mHeadRadius', '50.0f'),
    ('mHoldOutAccCtrlF', '0.01f'), ('mHoldOutAccCtrlB', '0.023f'),
    ('mHoldOutSldCtrl', '0.3f'), ('mDecBrake', '1.0f'),
]
structs['TWaterEffectParams'] = [
    ('mJumpIntoMdlEffectSpY', '10.0f'), ('mJumpIntoMinY', '20.0f'),
    ('mJumpIntoMaxY', '50.0f'), ('mJumpIntoScaleMin', '0.75f'),
    ('mJumpIntoScaleWidth', '1.0f'), ('mRunningRippleSpeed', '30.0f'),
    ('mRunningRippleDepth', '30.0f'),
]
structs['TControllerParams'] = [
    ('mAnalogLRToZeroVal', '0x1e'), ('mAnalogLRToMiddleVal', '0x5a'),
    ('mAnalogLRToMaxVal', '0x96'), ('mAnalogLRMiddleLevel', '0.1f'),
    ('mStartToWalkLevel', '15.0f'), ('mStickRotateTime', '0x18'),
    ('mLengthMultTimes', '10'), ('mLengthMult', '0.935f'),
    ('mSquatRotMidAnalog', '0.7f'), ('mSquatRotMidValue', '0.05f'),
]
structs['TGraffitoParams'] = [
    ('mSinkTime', '0xf0'), ('mSinkDmgTime', '0xf0'), ('mSinkHeight', '150.0f'),
    ('mSinkMoveMin', '0.3f'), ('mSinkMoveMax', '0.5f'), ('mSinkRecover', '0.05f'),
    ('mSinkJumpRateMin', '0.1f'), ('mSinkJumpRateMax', '0.3f'),
    ('mSinkPumpLimit', '0.25f'), ('mSinkDmgDepth', '0.25f'),
    ('mFireHeight', '1000.0f'), ('mDizzySlipCtMax', '1000'),
    ('mDizzyWalkCtMax', '1000'), ('mDizzyAngleY', '0x7fff'),
    ('mDizzyAngleRate', '400.0f'), ('mDizzyPowerRate', '120.0f'),
    ('mDizzyPower', '20.0f'), ('mFireInvincibleTime', '0x96'),
    ('mFootEraseTimes', '4'), ('mFootEraseSize', '400.0f'), ('mFootEraseFront', '200.0f'),
]
structs['TDirtyParams'] = [
    ('mIncRunning', '0.1f'), ('mIncCatching', '0.3f'), ('mIncSlipping', '0.2f'),
    ('mDecSwimming', '0.5f'), ('mDecWaterHit', '0.2f'), ('mDecRotJump', '0.1f'),
    ('mBrakeStartValSlip', '0.99f'), ('mBrakeStartValRun', '0.98f'),
    ('mDirtyTimeSlip', '600'), ('mDirtyTimeRun', '600'),
    ('mPolSizeSlip', '200.0f'), ('mPolSizeRun', '80.0f'),
    ('mPolSizeFootPrint', '200.0f'), ('mPolSizeJump', '200.0f'),
    ('mSlopeAngle', '0.99f'), ('mDirtyMax', '200.0f'),
    ('mSlipAnmSpeed', '3.0f'), ('mSlipRunSp', '0.01f'), ('mSlipCatchSp', '0.01f'),
    ('mSlipRotate', '0x100'), ('mSlipCatchRotate', '0x100'),
    ('mBrakeSlipNoPollute', '0.98f'), ('mFogTimeYellow', '0xf0'), ('mFogTimeRed', '600'),
]
structs['TMotorParams'] = [
    ('mMotorReturn', '0x19'), ('mMotorTrample', '8'),
    ('mMotorHipDrop', '0xf'), ('mMotorWall', '6'),
]
structs['TParticleParams'] = [
    ('mMeltInWaterMax', '0.5f'), ('mWaveEmitSpeed', '5.0f'), ('mWaveAlphaDec', '5'),
    ('mBubbleDepth', '10.0f'), ('mBodyBubbleSpMin', '0.0f'), ('mBodyBubbleSpMax', '40.0f'),
    ('mBodyBubbleEmitMin', '0.0f'), ('mBodyBubbleEmitMax', '0.5f'),
    ('mBubbleToRipple', '0.3f'), ('mToroccoWind', '0.001f'), ('mToroccoSpark', '0.001f'),
]
structs['TEffectParams'] = [
    ('mDashInc', '0.033f'), ('mDashDec', '0.017f'),
    ('mDashMaxBlendInBlur', '0xb4'), ('mDashMaxBlendInIris', '0xb4'),
    ('mDashBlendScale', '0.2f'),
]
structs['TSlipParams'] = [
    ('mSlipFriction', '0.9f'), ('mSlopeAcceleUp', '0.0f'), ('mSlopeAcceleDown', '0.0f'),
    ('mSlideAcceleUp', '0.0f'), ('mSlideAcceleDown', '0.0f'),
    ('mSlideStopNormal', '15.0f'), ('mSlideStopCatch', '15.0f'),
    ('mJumpEnable', '1'), ('mMissJump', '1'),
    ('mSlideAngleYSp', '0x200'), ('mStickSlideMult', '0.05f'),
]
structs['TUpperParams'] = [
    ('mPumpWaitTime', '10'), ('mPumpAnmSpeed', '0.01f'),
    ('mHoverHeadAngle', '0xe000'), ('mFeelDeepHeadAngle', '0x2000'),
    ('mFrontWallHeadAngle', '0xe000'),
]
structs['TEParams'] = [
    ('mDamage', '1'), ('mDownType', '0'), ('mWaterEmit', '0'), ('mMotor', '0'),
    ('mMinSpeed', '0.0f'), ('mDirty', '0.0f'), ('mInvincibleTime', '0'),
]
structs['TAutoDemoParams'] = [
    ('mWarpInBallsDispTime', '6'), ('mWarpInBallsTime', '0x46'),
    ('mWarpInCapturedTime', '0x78'), ('mWarpInTremble', '15.0f'),
    ('mWarpInVecBase', '0.3f'), ('mWarpTransTremble', '50.0f'), ('mReadRotSp', '0x400f'),
]
structs['TSoundParams'] = [('mStartFallVoiceSpeed', '60.0f')]
structs['TOptionParams'] = [('mZ', '-1000.0f'), ('mXMin', '846.0f'), ('mXMax', '2000.0f')]

layout = [
    ('mDeParams', 'TDeParams'),
    ('mBodyAngleParamsFree', 'TBodyAngleParams'),
    ('mBodyAngleParamsWaterGun', 'TBodyAngleParams'),
    ('mAttackParamsFencePunch', 'TAttackParams'),
    ('mAttackParamsKickRoof', 'TAttackParams'),
    ('mJumpParams', 'TJumpParams'),
    ('mRunParams', 'TRunParams'),
    ('mSwimParams', 'TSwimParams'),
    ('mHangingParams', 'THangingParams'),
    ('mHangRoofParams', 'THangRoofParams'),
    ('mWireParams', 'TWireParams'),
    ('mPullParamsBGBeak', 'TPullParams'),
    ('mPullParamsBGTentacle', 'TPullParams'),
    ('mPullParamsBGFireWanWanBossTail', 'TPullParams'),
    ('mPullParamsFireWanWanTail', 'TPullParams'),
    ('mBarParams', 'TBarParams'),
    ('mSurfingParamsWaterRed', 'TSurfingParams'),
    ('mSurfingParamsGroundRed', 'TSurfingParams'),
    ('mSurfingParamsWaterYellow', 'TSurfingParams'),
    ('mSurfingParamsGroundYellow', 'TSurfingParams'),
    ('mSurfingParamsWaterGreen', 'TSurfingParams'),
    ('mSurfingParamsGroundGreen', 'TSurfingParams'),
    ('mHoverParams', 'THHoverParams'),
    ('mDivingParams', 'TDivingParams'),
    ('mYoshiParams', 'TYoshiParams'),
    ('mWaterEffectParams', 'TWaterEffectParams'),
    ('mControllerParams', 'TControllerParams'),
    ('mGraffitoParams', 'TGraffitoParams'),
    ('mDirtyParams', 'TDirtyParams'),
    ('mMotorParams', 'TMotorParams'),
    ('mParticleParams', 'TParticleParams'),
    ('mEffectParams', 'TEffectParams'),
    ('mSlipParamsNormal', 'TSlipParams'),
    ('mSlipParamsOil', 'TSlipParams'),
    ('mSlipParamsAll', 'TSlipParams'),
    ('mSlipParamsAllSlider', 'TSlipParams'),
    ('mSlipParams45', 'TSlipParams'),
    ('mSlipParamsWaterSlope', 'TSlipParams'),
    ('mSlipParamsWaterGround', 'TSlipParams'),
    ('mSlipParamsYoshi', 'TSlipParams'),
    ('mUpperBodyParams', 'TUpperParams'),
    ('mDmgParamsEnemyCommon', 'TEParams'),
    ('mDmgParamsHamakuri', 'TEParams'),
    ('mDmgParamsNamekuri', 'TEParams'),
    ('mDmgParamsHinokuri', 'TEParams'),
    ('mDmgParamsFire', 'TEParams'),
    ('mDmgParamsBGTentacle', 'TEParams'),
    ('mDmgParamsBossEel', 'TEParams'),
    ('mDmgParamsHanachanBoss', 'TEParams'),
    ('mDmgParamsPoihana', 'TEParams'),
    ('mDmgParamsKiller', 'TEParams'),
    ('mDmgParamsLampTrapIron', 'TEParams'),
    ('mDmgParamsLampTrapSpike', 'TEParams'),
    ('mDmgParamsEnemyMario', 'TEParams'),
    ('mDmgParamsCannotBreath', 'TEParams'),
    ('mDmgParamsGraffitoFire', 'TEParams'),
    ('mDmgParamsGraffitoPoison', 'TEParams'),
    ('mDmgParamsGraffitoElec', 'TEParams'),
    ('mDmgParamsGraffitoLava', 'TEParams'),
    ('mDmgParamsWaterSurface', 'TEParams'),
    ('mDmgMapParams0', 'TEParams'),
    ('mDmgMapParams1', 'TEParams'),
    ('mDmgMapParams2', 'TEParams'),
    ('mDmgMapParams3', 'TEParams'),
    ('mDmgMapParams4', 'TEParams'),
    ('mDmgMapParams5', 'TEParams'),
    ('mDmgMapParams6', 'TEParams'),
    ('mDmgMapParams7', 'TEParams'),
    ('mDmgMapParams8', 'TEParams'),
    ('mDmgMapParams9', 'TEParams'),
    ('mAutoDemoParams', 'TAutoDemoParams'),
    ('mSoundParams', 'TSoundParams'),
    ('mOptionParams', 'TOptionParams'),
]

# Build offset map: value_offset -> (param_name, default_value)
offset = 0x574
offset_map = {}
struct_base_map = {}  # base_offset -> member_name

for member_name, type_name in layout:
    fields = structs[type_name]
    struct_base_map[offset] = (member_name, type_name)
    for i, (field_name, default_val) in enumerate(fields):
        field_base = offset + 0x08 + i * 0x14
        value_offset = field_base + 0x10
        offset_map[value_offset] = (member_name + '.' + field_name, default_val)
    offset += params_size(len(fields))

# Print complete mapping table
print("=" * 90)
print("COMPLETE TParams OFFSET-TO-NAME MAPPING TABLE")
print("=" * 90)
print(f"{'OFFSET':<8} {'PARAM_NAME':<55} DEFAULT_VALUE")
print(f"{'------':<8} {'----------':<55} -------------")

current_struct = None
for off in sorted(offset_map.keys()):
    name, default = offset_map[off]
    struct_name = name.split('.')[0]
    if struct_name != current_struct:
        current_struct = struct_name
        # Find the struct base
        for base, (mname, tname) in struct_base_map.items():
            if mname == struct_name:
                print(f"  --- {mname} ({tname}) @ base 0x{base:04X} ---")
                break
    print(f"0x{off:04X}   {name:<55} {default}")

print(f"\nTotal TParams value offsets: {len(offset_map)}")
print(f"TMario TParams block: 0x0574 - 0x{offset:04X}")

# Now scan source files for raw offset patterns
print("\n" + "=" * 90)
print("RAW OFFSET ACCESSES THAT MAP TO TParams FIELDS")
print("=" * 90)

src_dir = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'src', 'Player')
files = [
    'MarioMove.cpp', 'MarioSpecial.cpp', 'MarioPhysics.cpp', 'MarioRun.cpp',
    'MarioJump.cpp', 'MarioWait.cpp', 'MarioSwim.cpp', 'MarioCheckCol.cpp',
    'MarioUpper.cpp', 'MarioInit.cpp',
]

# Pattern: 0xHEX inside (u8*)this + 0xHEX
pat = re.compile(r'\(u8\*\)this\s*\+\s*0x([0-9a-fA-F]+)')

total_matches = 0
total_tparams = 0
unresolved = {}

for fname in files:
    fpath = os.path.join(src_dir, fname)
    if not os.path.exists(fpath):
        continue
    with open(fpath, 'r', encoding='utf-8', errors='replace') as f:
        lines = f.readlines()

    file_hits = []
    for lineno, line in enumerate(lines, 1):
        for m in pat.finditer(line):
            raw_hex = m.group(1)
            # Handle base+offset patterns like 0x1BC4 + 0x18
            # Check if there's a + 0x after
            after = line[m.end():]
            combined_offset = int(raw_hex, 16)

            # Check for "+ 0xNN)" pattern after the first offset
            add_match = re.match(r'\s*\+\s*0x([0-9a-fA-F]+)', after)
            if add_match:
                combined_offset += int(add_match.group(1), 16)

            total_matches += 1
            if combined_offset in offset_map:
                name, default = offset_map[combined_offset]
                file_hits.append((lineno, combined_offset, name, default, line.rstrip()))
                total_tparams += 1
            elif combined_offset in struct_base_map:
                mname, tname = struct_base_map[combined_offset]
                file_hits.append((lineno, combined_offset, f"{mname} (struct base)", tname, line.rstrip()))
                total_tparams += 1

    if file_hits:
        print(f"\n--- {fname} ({len(file_hits)} TParams accesses) ---")
        for lineno, off, name, default, line_text in file_hits:
            print(f"  Line {lineno:4d}: 0x{off:04X} -> {name}")

print(f"\n{'=' * 90}")
print(f"SUMMARY")
print(f"{'=' * 90}")
print(f"Total raw offset accesses found: {total_matches}")
print(f"Mapped to TParams fields: {total_tparams}")
print(f"Non-TParams offsets: {total_matches - total_tparams}")
