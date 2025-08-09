#include <Enemy/Walker.hpp>
#include <Enemy/Spider.hpp>
#include <Enemy/Enemy.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/Map.hpp>
#include <Strategic/LiveActor.hpp>
#include <Map/MapData.hpp>

#pragma dont_inline on
static f32 calcFarthestVertex(const TBGCheckData*, const JGeometry::TVec3<f32>&,
                              const JGeometry::TVec3<f32>&)
{
}
#pragma dont_inline off

TWalker::TWalker()
    : unk4(4)
    , unk14(0)
    , unk18(1.0f)
    , unk1C(0)
    , unk20(0)
    , unk24(0)
    , unk28(0)
    , unk2C(0)
    , unk30(1)
{
	reset();
}

TWalker::~TWalker() { }

void TWalker::reset()
{
	unk14 = 0;
	unk1C = 0;
	unk20 = 30;
	unk24 = 0;
	unk4.clear();
}

void TWalker::bind(TLiveActor* param_1)
{
	TSpineEnemy* enemy = (TSpineEnemy*)param_1;

	if (unk28 == 1 && unk2C != nullptr) {
		unk2C->bind(enemy);
		return;
	}

	JGeometry::TVec3<f32> local_30 = enemy->mPosition;
	local_30 += enemy->mLinearVelocity;
	unk1C = 0;
	if (enemy->checkLiveFlag(LIVE_FLAG_AIRBORNE)) {
		JGeometry::TVec3<f32> local_3c = enemy->mVelocity;
		local_30 += local_3c;
		local_3c.y -= enemy->getGravityY();
		if (local_3c.y < TLiveActor::mVelocityMinY)
			local_3c.y = TLiveActor::mVelocityMinY;
		enemy->mVelocity = local_3c;
	}

	if (unk30 == 0) {
		JGeometry::TVec3<f32> local_c4 = enemy->mVelocity;
	}

	f32 fVar1;
	const TBGCheckData* local_40;
	if (enemy->checkLiveFlag(LIVE_FLAG_UNK1000)) {
		fVar1 = gpMap->checkGroundIgnoreWaterSurface(
		    local_30.x, local_30.y + enemy->getHeadHeight(), local_30.z,
		    &local_40);
	} else {
		fVar1 = gpMap->checkGround(local_30.x,
		                           local_30.y + enemy->getHeadHeight(),
		                           local_30.z, &local_40);
	}
	fVar1 += 1.0f;

	if (unk30 == 2 && enemy->mPosition.y - local_30.y > 0.0f
	    && !local_40->checkFlag2(0x10)) {
		if (!local_40->checkSomething7()) {
			f32 dVar16;
			const TBGCheckData* local_44;
			if (enemy->checkLiveFlag(LIVE_FLAG_UNK1000)) {
				dVar16 = gpMap->checkGroundIgnoreWaterSurface(
				    local_30.x, local_30.y + enemy->getHeadHeight(), local_30.z,
				    &local_44);
			} else {
				dVar16 = gpMap->checkGround(local_30.x,
				                            local_30.y + enemy->getHeadHeight(),
				                            local_30.z, &local_44);
			}
			dVar16 += 1.0f;
			if (fVar1 < dVar16) {
				local_40 = local_44;
				fVar1    = dVar16;
			}
		}
	}

	if (local_40->checkFlag2(0x10)) {
		unk20 = 30;
	} else {
		if (unk20 < 1) {
			enemy->kill();
			return;
		}

		local_30.y = enemy->mPosition.y;
		unk20 -= 1;
		fVar1 = local_30.y;
	}

	if (fVar1 * 0.05f < local_30.y || local_40->checkFlag2(0x10)) {
		enemy->onLiveFlag(LIVE_FLAG_AIRBORNE);
	} else {
		if (local_40->checkSomething7()) { }

		enemy->mVelocity = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
		enemy->offLiveFlag(LIVE_FLAG_AIRBORNE);
		enemy->offLiveFlag(LIVE_FLAG_UNK8000);
		local_30.y = fVar1;
	}

	enemy->mGroundHeight = fVar1;
	enemy->mGroundPlane  = local_40;

	TBGWallCheckRecord local_70(local_30, enemy->getWallRadius(), 4, 0);
	if (gpMap->isTouchedWallsAndMoveXZ(&local_70)) {
		unk4.clear();
		unk14         = 0;
		unk24         = 0;
		enemy->unk138 = nullptr;
	} else {
		const TBGCheckData* pTVar14;
		for (int i = 0; i < local_70.unk14; ++i)
			if (!unk4.contain(local_70.unk1C[i])) {
				pTVar14 = local_70.unk1C[i];
				break;
			}
		if (pTVar14 == nullptr) {
			unk24 += 1;
		} else {
			enemy->unk138 = pTVar14;
			if (enemy->checkLiveFlag(LIVE_FLAG_AIRBORNE)
			    && pTVar14->mMaxY - local_30.y <= unk18
			    && pTVar14->checkSomething2()) {
				unk1C = 1;
			}

			unk4.push(pTVar14);

			JGeometry::TVec3<f32> normal = pTVar14->getNormal();
			JGeometry::TVec3<f32> local_94;
			local_94.cross(normal, JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f));
			local_94.normalize();
			if (unk14 == 0) {
				if (enemy->mLinearVelocity.dot(local_94) <= 0.0f) {
					local_94.negate();
					unk14 = 1;
				} else {
					unk14 = 2;
				}

				f32 dVar18
				    = calcFarthestVertex(pTVar14, enemy->mPosition, local_94);
				JGeometry::TVec3<f32> local_224 = local_94;
				local_224.scale((enemy->getWallRadius()) * 2.0f + dVar18);
				JGeometry::TVec3<f32> local_e8  = local_224;
				JGeometry::TVec3<f32> local_1e0 = enemy->mPosition;
				local_1e0 += local_e8;
				JGeometry::TVec3<f32> local_a0 = local_1e0;

				if (gpMap->isTouchedOneWallAndMoveXZ(
				        &local_a0.x, local_a0.y + enemy->getHeadHeight(),
				        &local_a0.z, enemy->getWallRadius())) {
					JGeometry::TVec3<f32> local_ac = local_94;
					local_ac.negate();

					f32 dVar18 = calcFarthestVertex(pTVar14, enemy->mPosition,
					                                local_ac);
					JGeometry::TVec3<f32> local_230 = local_ac;
					local_230.scale((enemy->getWallRadius()) * 2.0f + dVar18);
					JGeometry::TVec3<f32> local_100 = local_230;
					JGeometry::TVec3<f32> local_1ec = enemy->mPosition;
					local_1ec += local_100;
					local_a0 = local_1ec;
					if (gpMap->isTouchedOneWallAndMoveXZ(
					        &local_a0.x, local_a0.y + enemy->getHeadHeight(),
					        &local_a0.z, enemy->getWallRadius())) {
						local_94.negate();
					}
				}
			} else if (unk14 == 1) {
				local_94.negate();
			}

			f32 dVar18
			    = calcFarthestVertex(pTVar14, enemy->mPosition, local_94);
			f32 fVar4 = (enemy->getWallRadius()) * 2.0f + dVar18;
			if (enemy->unk114.empty()) {
				JGeometry::TVec3<f32> local_23c = local_94;
				local_23c.scale(fVar4);
				JGeometry::TVec3<f32> local_130 = local_23c;
				JGeometry::TVec3<f32> local_248 = pTVar14->getNormal();
				local_248.scale(enemy->getWallRadius());
				JGeometry::TVec3<f32> local_124 = local_248;
				JGeometry::TVec3<f32> local_254 = enemy->mPosition;
				local_254 -= local_124;
				JGeometry::TVec3<f32> local_118 = local_254;
				JGeometry::TVec3<f32> local_1f8 = local_254;
				local_1f8 += local_130;
				JGeometry::TVec3<f32> local_13c = local_1f8;
				enemy->unk114.push(enemy->unkF4);
				enemy->unkF4.unk0 = nullptr;
				enemy->unkF4.unk4 = local_1f8;
			} else {
				JGeometry::TVec3<f32> local_260 = local_94;
				local_260.scale(fVar4);
				JGeometry::TVec3<f32> local_170 = local_260;
				JGeometry::TVec3<f32> local_26c = pTVar14->getNormal();
				local_26c.scale(enemy->getWallRadius());
				JGeometry::TVec3<f32> local_164 = local_26c;
				JGeometry::TVec3<f32> local_278 = enemy->mPosition;
				local_278 -= local_164;
				JGeometry::TVec3<f32> local_158 = local_278;
				JGeometry::TVec3<f32> local_208 = local_278;
				local_158.add(local_208);
				enemy->unkF4.unk0 = nullptr;
				enemy->unkF4.unk4 = local_208;
			}
		}
	}

	JGeometry::TVec3<f32> local_218 = local_70.unk0;
	local_218.y                     = local_30.y;
	local_218 -= enemy->mPosition;
	enemy->mLinearVelocity = local_218;
}

void TWalker::setMode(int param_1)
{
	unk28 = param_1;
	if (unk28 == 1 && unk2C == nullptr)
		unk2C = new TSpider;
}
