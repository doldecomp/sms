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

void TSpider::bind(TLiveActor* param_1)
{
	TSpineEnemy* enemy = (TSpineEnemy*)param_1;

	JGeometry::TVec3<f32> local_114 = enemy->mLinearVelocity;
	JGeometry::TVec3<f32> local_50  = enemy->mPosition;
	local_50.add(local_114);
	f32 linearX = local_114.x;
	f32 linearY = local_114.y;
	f32 linearZ = local_114.z;

	if (enemy->isAirborne()) {
		JGeometry::TVec3<f32> local_5C = enemy->mVelocity;
		local_50 += local_5C;
		f32 dVar7 = enemy->getGravityY();
		local_5C.y -= dVar7;
		if (local_5C.y < TLiveActor::mVelocityMinY)
			local_5C.y = TLiveActor::mVelocityMinY;

		enemy->mVelocity = local_5C;
	}

	const TBGCheckData* local_60;
	f32 fVar3 = gpMap->checkGround(
	    local_50.x, local_50.y + enemy->getHeadHeight(), local_50.z, &local_60);
	fVar3 += 1.0f;

	if (enemy->mPosition.y - local_50.y > 0.0f) {
		const TBGCheckData* local_64;
		f32 dVar7 = gpMap->checkGround(
		    local_50.x, enemy->mPosition.y + enemy->getHeadHeight(), local_50.z,
		    &local_64);
		dVar7 += 1.0f;
		if (dVar7 > fVar3) {
			local_60 = local_64;
			fVar3    = dVar7;
		}
	}

	if (local_60->checkFlag(BG_CHECK_FLAG_ILLEGAL)) {
		if (unk4 <= 0) {
			enemy->kill();
		} else {
			fVar3      = enemy->mPosition.y;
			local_50.y = fVar3;
			unk4 -= 1;
		}
	} else {
		unk4 = 0x1E;
	}

	if (local_50.y <= fVar3) {
		enemy->mVelocity = JGeometry::TVec3<f32>(0, 0, 0);

		enemy->offLiveFlag(LIVE_FLAG_AIRBORNE);
		enemy->offLiveFlag(LIVE_FLAG_UNK8000);
		local_50.y = fVar3;
	} else {
		enemy->onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	enemy->mGroundHeight = fVar3;
	enemy->mGroundPlane  = local_60;

	TBGWallCheckRecord local_90(local_50.x, local_50.y, local_50.z,
	                            enemy->mBodyScale * enemy->mHeadHeight, 1, 0);

	JGeometry::TVec3<f32> local_bc;
	f32 unaff_f29;

	bool b   = gpMap->isTouchedWallsAndMoveXZ(&local_90);
	local_bc = local_90.mCenter;
	if (!b) {
		if (unk8 > 0) {
			unk8 -= 1;
			unaff_f29 = enemy->mMarchSpeed;
			enemy->offLiveFlag(LIVE_FLAG_AIRBORNE);
			enemy->offLiveFlag(LIVE_FLAG_UNK8000);
			enemy->mVelocity = JGeometry::TVec3<f32>(0, 0, 0);
		} else {
			unkC = 0;

			enemy->unk138 = nullptr;

			unaff_f29 = 0.0f;
		}
		unk10 -= 0.016666667f;
		if (unk10 < 0.0f)
			unk10 = 0.0f;
	} else {
		JGeometry::TVec3<f32> normal = local_90.mResultWalls[0]->getNormal();
		if (normal.x * linearX + normal.y * linearY + normal.z * linearZ
		    < 0.0f) {
			unaff_f29 = enemy->mMarchSpeed;
			enemy->offLiveFlag(LIVE_FLAG_AIRBORNE);
			enemy->offLiveFlag(LIVE_FLAG_UNK8000);
			enemy->mVelocity = JGeometry::TVec3<f32>(0, 0, 0);

			unkC = local_90.mResultWalls[0];

			enemy->unk138 = unkC;

			unk8 = 0x3C;

			normal.scale(enemy->getWallRadius() * unk10);
			local_bc.sub(normal, local_90.mCenter);

			unk10 += 1.0f / 60.0f;
			if (unk10 > 1.0f)
				unk10 = 1.0f;
		}
	}

	JGeometry::TVec3<f32> local_118 = local_bc;
	local_118.y += unaff_f29 - enemy->getHeadHeight();

	enemy->mLinearVelocity = local_118 - enemy->mPosition;
}
