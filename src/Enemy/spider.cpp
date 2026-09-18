#include <Enemy/Spider.hpp>
#include <Enemy/Enemy.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/Map.hpp>

TSpider::TSpider()
    : unk4(0)
    , unk8(0)
    , unkC(0)
    , unk10(0.0f)
{
}

TSpider::~TSpider() { }

// TODO: all 349 instructions and every register match; the only differences in
// the whole function are `r1` displacements. Frame 0x120 against the ROM's
// 0x158, and the slot map says exactly where the 60 missing bytes of locals sit
// (ROM offset -> ours):
//
//   0x8 -0x40  outgoing arguments, 56 bytes (ours 60: we are +4 here)
//   0x114/0x108/0xfc/0xf8/0xf4  the five named locals, in declaration order
//                               downward - local_114, local_50, local_5C and
//                               the two TBGCheckData pointers. Exact.
//   0xe8-0xf4  12 bytes of alignment below them; we have it too
//   0xc8/0xbc/0xb0  TBGWallCheckRecord, local_bc, normal. Exact.
//   0xa4-0xb0  12 bytes we do NOT have
//   0x98/0x8c/0x80  the three TVec3<f32>(0,0,0) setVelocity temporaries
//   0x4c-0x80  52 bytes we do NOT have
//   0x40       operator-'s return buffer for the final mLinearVelocity store
//
// So the temporary pool is allocated downward in expansion order and the two
// holes are dead temporaries: one 12-byte slot allocated *before* the first
// setVelocity temporary, and 52 bytes between the last setVelocity temporary
// and operator-'s buffer.
//
// Measured with a TU-local wrapper around setVelocity carrying an
// uninitialised non-trivial local: the cost is exactly sizeof x 4 expansions
// (4 B -> +0x10, 8 -> +0x20, 12 -> +0x30, 16 -> +0x40, 20 -> +0x50), so no
// single dead local in setVelocity can pay 56; a 12-byte one gets to 0x150.
// Rejected: setVelocity by value (0x128, 355 instructions), mVelocity.set(0,0,0)
// in place of setVelocity (0xf8, 19 structural diffs), and a named delta with
// sub() in place of operator- (0x110, 360 instructions).
//
// Best next hypothesis: the 52-byte hole is one 48-byte object plus 4 bytes of
// alignment (nothing else in the pool is 4-aligned at 0x4c), i.e. a scratch Mtx
// in an inlined callee between local_bc.sub() and the final operator-; and the
// 12-byte hole above the setVelocity temporaries is a fourth TVec3 temporary
// from the `param_1->setVelocity(local_5C)` site in the airborne branch.
void TSpider::bind(TLiveActor* param_1)
{
	TSpineEnemy* enemy = (TSpineEnemy*)param_1;
	Vec local_114     = param_1->mLinearVelocity;
	JGeometry::TVec3<f32> local_50 = param_1->mPosition;
	local_50.x += local_114.x;
	local_50.y += local_114.y;
	local_50.z += local_114.z;

	if (param_1->isAirborne()) {
		JGeometry::TVec3<f32> local_5C = param_1->mVelocity;
		local_50 += local_5C;
		f32 dVar7 = param_1->getGravityY();
		local_5C.y -= dVar7;
		if (local_5C.y < TLiveActor::mVelocityMinY)
			local_5C.y = TLiveActor::mVelocityMinY;

		param_1->setVelocity(local_5C);
	}

	const TBGCheckData* local_60;
	f32 nextY = local_50.y;
	f32 nextZ = local_50.z;
	f32 fVar3 = gpMap->checkGround(
	    local_50.x, nextY + enemy->getHeadHeight(), nextZ, &local_60);
	fVar3 += 1.0f;

	if (param_1->mPosition.y - nextY > 0.0f) {
		const TBGCheckData* local_64;
		f32 dVar7 = gpMap->checkGround(
		    local_50.x, param_1->mPosition.y + enemy->getHeadHeight(),
		    nextZ, &local_64);
		dVar7 += 1.0f;
		if (dVar7 > fVar3) {
			local_60 = local_64;
			fVar3    = dVar7;
		}
	}

	if (local_60->checkFlag(BG_CHECK_FLAG_ILLEGAL)) {
		if (unk4 <= 0) {
			param_1->kill();
		} else {
			local_50.y = param_1->mPosition.y;
			fVar3      = local_50.y;
			unk4 -= 1;
		}
	} else {
		unk4 = 0x1E;
	}

	if (local_50.y <= fVar3) {
		local_50.y = fVar3;
		param_1->setVelocity(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));

		param_1->offLiveFlag(LIVE_FLAG_AIRBORNE);
		param_1->offLiveFlag(LIVE_FLAG_UNK8000);
	} else {
		param_1->onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	param_1->mGroundHeight = fVar3;
	param_1->mGroundPlane  = local_60;

	TBGWallCheckRecord local_90(
	    local_50.x, local_50.y + enemy->getHeadHeight(), local_50.z,
	    enemy->getWallRadius(), 1, 0);

	// TODO: The original also leaves this register unassigned when the wall
	// normal dot velocity is nonnegative; recover the intended source structure.
	f32 unaff_f29;

	BOOL b = gpMap->isTouchedWallsAndMoveXZ(&local_90);
	JGeometry::TVec3<f32> local_bc = local_90.mCenter;
	if (!b) {
		if (unk8 > 0) {
			unk8 -= 1;
			unaff_f29 = enemy->mMarchSpeed;
			param_1->offLiveFlag(LIVE_FLAG_AIRBORNE);
			param_1->offLiveFlag(LIVE_FLAG_UNK8000);
			param_1->setVelocity(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
		} else {
			unkC = 0;

			enemy->unk138 = nullptr;

			unaff_f29 = 0.0f;
		}
		unk10 -= 0.016666667f;
		if (unk10 < 0.0f)
			unk10 = 0.0f;
	} else {
		JGeometry::TVec3<f32> normal;
		normal = local_90.mResultWalls[0]->mNormal;
		if (normal.x * local_114.x + normal.y * local_114.y
		        + normal.z * local_114.z
		    < 0.0f) {
			unaff_f29 = enemy->mMarchSpeed;
			param_1->offLiveFlag(LIVE_FLAG_AIRBORNE);
			param_1->offLiveFlag(LIVE_FLAG_UNK8000);
			param_1->setVelocity(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));

			unkC = local_90.mResultWalls[0];

			enemy->unk138 = unkC;

			unk8 = 0x3C;

			normal.scale(unk10 * enemy->getWallRadius());
			local_bc.sub(normal);

			unk10 += 1.0f / 60.0f;
			if (unk10 > 1.0f)
				unk10 = 1.0f;
		}
	}

	local_50 = local_bc;
	local_50.y += unaff_f29 - enemy->getHeadHeight();

	param_1->mLinearVelocity = local_50 - param_1->mPosition;
}
