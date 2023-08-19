#include "dolphin.h"

#include "Player/MarioMain.hpp"
#include "JSystem/JGeometry.hpp"

// TODO: Move TLiveActor to correct header, then replace this with said header
#include "types.h"

TMario* gpMarioAddress;
JGeometry::TVec3<float>* gpMarioPos;
s16 *gpMarioAngleX, *gpMarioAngleY, *gpMarioAngleZ;
f32 *gpMarioSpeedX, *gpMarioSpeedY, *gpMarioSpeedZ;
u16* gpMarioLightID;
u32* gpMarioFlag;
f32* gpMarioThrowPower;
void** gpMarioGroundPlane; // TODO: TBGCheckData**

bool SMS_IsMarioRoofing()
{
	bool ret;

	u32 val = gpMarioOriginal->action & 0x1FF;

	if (val >= 0x147 and val <= 0x14A) {
		ret = true;
	} else {
		ret = false;
	}

	return ret;
}

bool SMS_IsMarioFencing()
{
	bool ret;

	u32 val = gpMarioOriginal->action & 0x1FF;

	if (val >= 0x168 and val <= 0x16C) {
		ret = true;
	} else {
		ret = false;
	}

	return ret;
}

u32 SMS_GetMarioStatus(THitActor* actor) { return ((TMario*)actor)->action; }

void* SMS_GetMarioRfPlane() { return gpMarioOriginal->ceil; }

void* SMS_GetMarioWlPlane() { return gpMarioOriginal->wall; }

void* SMS_GetMarioGrPlane() { return gpMarioOriginal->floor; }

u32 SMS_GetMarioStatus() { return gpMarioOriginal->action; }

void SMS_WindMoveMario(const JGeometry::TVec3<float>& vec)
{
	gpMarioOriginal->windMove(vec);
}

void SMS_FlowMoveMario(const JGeometry::TVec3<float>& vec)
{
	gpMarioOriginal->flowMove(vec);
}

void SMS_MarioWarpRequest(const JGeometry::TVec3<float>& vec, float f)
{
	gpMarioOriginal->warpRequest(vec, f);
}

// clang-format off
#pragma push
asm void SMS_MarioMoveRequest(const JGeometry::TVec3<float>& vec) {
    nofralloc
    /* 80152A3C 0014F97C  7C 08 02 A6 */  mflr r0;
    /* 80152A40 0014F980  7C 64 1B 78 */  mr r4, r3;
    /* 80152A44 0014F984  90 01 00 04 */  stw r0, 4(r1);
    /* 80152A48 0014F988  94 21 FF F8 */  stwu r1, -8(r1);
    /* 80152A4C 0014F98C  80 6D 98 B8 */  lwz r3, gpMarioOriginal(r13);
    /* 80152A50 0014F990  81 83 00 00 */  lwz r12, 0(r3);
    /* 80152A54 0014F994  81 8C 00 AC */  lwz r12, 0xac(r12);
    /* 80152A58 0014F998  7D 88 03 A6 */  mtlr r12;
    /* 80152A5C 0014F99C  4E 80 00 21 */  blrl;
    /* 80152A60 0014F9A0  80 01 00 0C */  lwz r0, 0xc(r1);
    /* 80152A64 0014F9A4  38 21 00 08 */  addi r1, r1, 8;
    /* 80152A68 0014F9A8  7C 08 03 A6 */  mtlr r0;
    /* 80152A6C 0014F9AC  4E 80 00 20 */  blr;
}
#pragma pop
// clang-format on

u8 SMS_IsMarioDashing()
{
	u8 ret;

	if ((gpMarioOriginal->_118 & 0x4000) != 0) {
		ret = true;
	} else {
		ret = false;
	}

	return !!ret;
}

u32 SMS_IsMarioOnYoshi()
{
	u32 ret = gpMarioOriginal->onYoshi();

	return !!ret;
}

bool SMS_IsMarioOpeningDoor()
{
	if (gpMarioOriginal->action == 0x1320
	    || gpMarioOriginal->action == 0x1321) {
		return true;
	} else {
		return false;
	}
}

// clang-format off
#pragma push
asm u8 SMS_IsMarioOnWire() {
    nofralloc
    /* 80152B00 0014FA40  80 6D 98 B8 */    lwz r3, gpMarioOriginal(r13)
    /* 80152B04 0014FA44  80 03 00 68 */    lwz r0, 0x68(r3)
    /* 80152B08 0014FA48  28 00 00 00 */    cmplwi r0, 0
    /* 80152B0C 0014FA4C  41 82 00 20 */    beq lbl_80152B2C
    /* 80152B10 0014FA50  80 63 00 68 */    lwz r3, 0x68(r3)
    /* 80152B14 0014FA54  80 63 00 4C */    lwz r3, 0x4c(r3)
    /* 80152B18 0014FA58  3C 03 C0 00 */    addis r0, r3, 0xc000
    /* 80152B1C 0014FA5C  28 00 00 98 */    cmplwi r0, 0x98
    /* 80152B20 0014FA60  40 82 00 0C */    bne lbl_80152B2C
    /* 80152B24 0014FA64  38 00 00 01 */    li r0, 1
    /* 80152B28 0014FA68  48 00 00 08 */    b lbl_80152B30
    lbl_80152B2C:
    /* 80152B2C 0014FA6C  38 00 00 00 */    li r0, 0
    lbl_80152B30:
    /* 80152B30 0014FA70  54 00 06 3E */    clrlwi r0, r0, 0x18
    /* 80152B34 0014FA74  7C 60 00 D0 */    neg r3, r0
    /* 80152B38 0014FA78  30 03 FF FF */    addic r0, r3, -1
    /* 80152B3C 0014FA7C  7C 00 19 10 */    subfe r0, r0, r3
    /* 80152B40 0014FA80  54 03 06 3E */    clrlwi r3, r0, 0x18
    /* 80152B44 0014FA84  4E 80 00 20 */    blr 
}
#pragma pop
// clang-format on

u8 SMS_IsMarioTouchGround4cm()
{
	u8 ret;

	if (gpMarioOriginal->position.y
	    <= 4.0f + gpMarioOriginal->floorPosition.y) {
		ret = 1;
	} else {
		ret = 0;
	}

	if (ret)
		return 1;
	else
		return 0;
}

void SMS_ThrowMario(const JGeometry::TVec3<float>& vec, float f)
{
	gpMarioOriginal->throwMario(vec, f);
}

// clang-format off
#pragma push
asm void SMS_SendMessageToMario(THitActor *m, u32 mesg) {
    nofralloc
    /* 80152BB4 0014FAF4  7C 08 02 A6 */    mflr r0
    /* 80152BB8 0014FAF8  7C 85 23 78 */    mr r5, r4
    /* 80152BBC 0014FAFC  90 01 00 04 */    stw r0, 4(r1)
    /* 80152BC0 0014FB00  7C 60 1B 78 */    mr r0, r3
    /* 80152BC4 0014FB04  7C 04 03 78 */    mr r4, r0
    /* 80152BC8 0014FB08  94 21 FF F8 */    stwu r1, -8(r1)
    /* 80152BCC 0014FB0C  80 6D 98 B8 */    lwz r3, gpMarioOriginal(r13)
    /* 80152BD0 0014FB10  81 83 00 00 */    lwz r12, 0(r3)
    /* 80152BD4 0014FB14  81 8C 00 A0 */    lwz r12, 0xa0(r12)
    /* 80152BD8 0014FB18  7D 88 03 A6 */    mtlr r12
    /* 80152BDC 0014FB1C  4E 80 00 21 */    blrl 
    /* 80152BE0 0014FB20  2C 03 00 00 */    cmpwi r3, 0
    /* 80152BE4 0014FB24  41 82 00 0C */    beq lbl_80152BF0
    /* 80152BE8 0014FB28  38 60 00 01 */    li r3, 1
    /* 80152BEC 0014FB2C  48 00 00 08 */    b lbl_80152BF4
    lbl_80152BF0:
    /* 80152BF0 0014FB30  38 60 00 00 */    li r3, 0
    lbl_80152BF4:
    /* 80152BF4 0014FB34  80 01 00 0C */    lwz r0, 0xc(r1)
    /* 80152BF8 0014FB38  38 21 00 08 */    addi r1, r1, 8
    /* 80152BFC 0014FB3C  7C 08 03 A6 */    mtlr r0
    /* 80152C00 0014FB40  4E 80 00 20 */    blr 
}
#pragma pop
// clang-format on

void* SMS_GetMarioWaterGun()
{ // TODO: returns TWaterGun
	return gpMarioOriginal->waterGun;
}

f32 SMS_GetMarioGravity() { return gpMarioOriginal->jumpParams.gravity.value; }

f32 SMS_GetMarioGrLevel() { return gpMarioOriginal->floorPosition.y; }

f32 SMS_GetMarioDamageRadius() { return gpMarioOriginal->damageRadius; }

s16 SMS_GetMarioHP() { return gpMarioOriginal->health; }

bool SMS_IsMarioHeadSlideAttack()
{
	if (gpMarioOriginal->action == 0x00800456
	    || gpMarioOriginal->action == 0x0080088A) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioStatusElecDamage()
{
	if (gpMarioOriginal->action == 0x00020338) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioStatusThrownDown()
{
	if (gpMarioOriginal->action == 0x000208B8) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioStatusHipDrop()
{
	if (gpMarioOriginal->action == 0x008008A9) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioStatusTypeSwimming()
{
	if (gpMarioOriginal->action & (1 << 13)) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioStatusTypeJumping()
{
	if (gpMarioOriginal->action & (1 << 11)) {
		return 1;
	} else {
		return 0;
	}
}

TLiveActor* SMS_GetMarioLiveActor() { return (TLiveActor*)gpMarioAddress; }

THitActor* SMS_GetMarioHitActor() { return (THitActor*)gpMarioAddress; }

u32 SMS_AskJumpIntoWaterEffectExist() // bool?
{
	gpMarioOriginal->askJumpIntoWaterEffectExist();
}

void* SMS_GetYoshi()
{ // TODO: returns TYoshi
	return gpMarioOriginal->yoshi;
}

void SMS_SetMarioAccessParams()
{
	s16* angle;
	f32* speed;

	gpMarioAddress = gpMarioOriginal;
	gpMarioPos     = &gpMarioOriginal->position;

	angle         = &gpMarioOriginal->faceAngle.x;
	gpMarioAngleX = angle;
	gpMarioAngleY = angle + 1;
	gpMarioAngleZ = angle + 2;

	speed         = &gpMarioOriginal->vel.x;
	gpMarioSpeedX = speed;
	gpMarioSpeedY = speed + 1;
	gpMarioSpeedZ = speed + 2;

	gpMarioLightID = &gpMarioOriginal->lightID;
	gpMarioFlag    = &gpMarioOriginal->_118;

	gpMarioThrowPower  = &gpMarioOriginal->deParams.throwPower.value;
	gpMarioGroundPlane = &gpMarioOriginal->floor;
}
