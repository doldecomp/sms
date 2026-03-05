#include <Enemy/Walker.hpp>
#include <Enemy/Spider.hpp>
#include <Enemy/Enemy.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/Map.hpp>
#include <Strategic/LiveActor.hpp>
#include <Map/MapData.hpp>

static f32 calcFarthestVertex(const TBGCheckData* param_1,
                              const JGeometry::TVec3<f32>& param_2,
                              const JGeometry::TVec3<f32>& param_3)
{
	JGeometry::TVec3<f32> diffs[3];
	diffs[0].set(param_1->mPoint1.x - param_2.x, 0.0f,
	             param_1->mPoint1.z - param_2.z);
	diffs[1].set(param_1->mPoint2.x - param_2.x, 0.0f,
	             param_1->mPoint2.z - param_2.z);
	diffs[2].set(param_1->mPoint3.x - param_2.x, 0.0f,
	             param_1->mPoint3.z - param_2.z);

	int bVar8 = 1;
	f32 f7    = 0.0;
	for (int i = 0; i < 3; ++i) {
		if (diffs[i].dot(param_3) > 0.0f) {
			if (bVar8) {
				bVar8 = false;
				f7    = diffs[i].squared();
			} else if (f7 < diffs[i].squared())
				f7 = diffs[i].squared();
		}
	}

	if (bVar8) {
		f7 = 0.0;
	} else {
		// TODO: yet another copy of the standard sqrt
		// but without the newton iterations...
		volatile f32 tmp = f7 * __frsqrte(f7);
		f7               = tmp;
	}
	return f7;
}

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

	JGeometry::TVec3<f32> lv       = enemy->mLinearVelocity;
	JGeometry::TVec3<f32> local_30 = enemy->mPosition;
	local_30 += lv;
	unk1C = 0;
	if (enemy->checkLiveFlag2(LIVE_FLAG_AIRBORNE)) {
		JGeometry::TVec3<f32> local_3c = enemy->mVelocity;
		local_30 += local_3c;
		local_3c.y -= enemy->getGravityY();
		if (local_3c.y < TLiveActor::mVelocityMinY)
			local_3c.y = TLiveActor::mVelocityMinY;
		enemy->mVelocity = local_3c;
	}
	if (unk30 != 0 || JGeometry::TVec3<f32>(enemy->mVelocity).y < 0.0f) {
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
		    && !local_40->checkFlag(BG_CHECK_FLAG_ILLEGAL)) {
			if (!local_40->isEnemyThrough()) {
				f32 dVar16;
				const TBGCheckData* local_44;
				if (enemy->checkLiveFlag(LIVE_FLAG_UNK1000)) {
					dVar16 = gpMap->checkGroundIgnoreWaterSurface(
					    local_30.x, local_30.y + enemy->getHeadHeight(),
					    local_30.z, &local_44);
				} else {
					dVar16 = gpMap->checkGround(
					    local_30.x, local_30.y + enemy->getHeadHeight(),
					    local_30.z, &local_44);
				}
				dVar16 += 1.0f;
				if (dVar16 > fVar1) {
					local_40 = local_44;
					fVar1    = dVar16;
				}
			}
		}

		if (local_40->checkFlag(BG_CHECK_FLAG_ILLEGAL)) {
			if (unk20 <= 0) {
				enemy->kill();
				return;
			}

			local_30.y = enemy->mPosition.y;
			fVar1      = local_30.y;
			unk20 -= 1;
		} else {
			unk20 = 30;
		}

		if (fVar1 + 0.05f <= local_30.y
		    && !local_40->checkFlag(BG_CHECK_FLAG_ILLEGAL)
		    && !local_40->isEnemyThrough()) {
			local_30.y       = fVar1;
			enemy->mVelocity = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
			enemy->offLiveFlag(LIVE_FLAG_AIRBORNE);
			enemy->offLiveFlag(LIVE_FLAG_UNK8000);
		} else {
			enemy->onLiveFlag(LIVE_FLAG_AIRBORNE);
		}

		enemy->mGroundHeight = fVar1;
		enemy->mGroundPlane  = local_40;
	}

	f32 f31 = local_30.y;
	TBGWallCheckRecord local_70(local_30.x, local_30.y + enemy->getHeadHeight(),
	                            local_30.z, enemy->getWallRadius(), 4, 0);

	if (!gpMap->isTouchedWallsAndMoveXZ(&local_70)) {
		unk4.clear();
		unk14         = 0;
		unk24         = 0;
		enemy->unk138 = nullptr;
	} else {
		const TBGCheckData* pTVar14 = nullptr;
		for (int i = 0; i < local_70.mResultWallsNum; ++i)
			if (!unk4.contain(local_70.mResultWalls[i])) {
				pTVar14 = local_70.mResultWalls[i];
				break;
			}
		if (pTVar14 == nullptr) {
			unk24 += 1;
		} else {
			enemy->unk138 = pTVar14;
			if (!enemy->checkLiveFlag2(LIVE_FLAG_AIRBORNE)
			    && pTVar14->mMaxY - f31 <= unk18 && pTVar14->isPool()) {
				unk1C = 1;
			}

			unk4.push(pTVar14);

			JGeometry::TVec3<f32> normal = pTVar14->getNormal();
			JGeometry::TVec3<f32> local_94;
			local_94.cross(normal, JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f));
			local_94.normalize();
			if (unk14 == 0) {
				JGeometry::TVec3<f32> lv = enemy->mLinearVelocity;
				if (lv.dot(local_94) > 0.0f) {
					unk14 = 2;
				} else {
					local_94.negate();
					unk14 = 1;
				}

				f32 dVar18
				    = calcFarthestVertex(pTVar14, enemy->mPosition, local_94);
				JGeometry::TVec3<f32> local_a0
				    = enemy->mPosition
				      + local_94 * (enemy->getWallRadius() * 2.0f + dVar18);

				if (gpMap->isTouchedOneWallAndMoveXZ(
				        &local_a0.x, local_a0.y + enemy->getHeadHeight(),
				        &local_a0.z, enemy->getWallRadius())) {
					JGeometry::TVec3<f32> local_ac = local_94;
					local_ac.negate();

					f32 dVar18 = calcFarthestVertex(pTVar14, enemy->mPosition,
					                                local_ac);

					local_a0
					    = enemy->mPosition
					      + local_ac * (enemy->getWallRadius() * 2.0f + dVar18);

					if (!gpMap->isTouchedOneWallAndMoveXZ(
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
				JGeometry::TVec3<f32> local_130 = local_94 * fVar4;
				JGeometry::TVec3<f32> local_124
				    = pTVar14->getNormal() * enemy->getWallRadius();

				JGeometry::TVec3<f32> local_254 = enemy->mPosition - local_124;
				JGeometry::TVec3<f32> local_13c = local_254 + local_130;
				enemy->unk114.push(enemy->unkF4);
				enemy->unkF4.unk0 = nullptr;
				enemy->unkF4.unk4 = local_13c;
			} else {
				JGeometry::TVec3<f32> tmp2 = local_94 * fVar4;
				JGeometry::TVec3<f32> tmp1
				    = pTVar14->getNormal() * enemy->getWallRadius();
				JGeometry::TVec3<f32> thing     = enemy->mPosition - tmp1;
				JGeometry::TVec3<f32> local_278 = thing + tmp2;

				enemy->unkF4.unk0 = nullptr;
				enemy->unkF4.unk4 = local_278;
			}
		}
	}

	JGeometry::TVec3<f32> local_218 = local_70.mCenter;
	local_218.y                     = local_30.y;
	enemy->mLinearVelocity          = local_218 - enemy->mPosition;
}

void TWalker::setMode(int param_1)
{
	unk28 = param_1;
	if (unk28 == 1 && unk2C == nullptr)
		unk2C = new TSpider;
}
