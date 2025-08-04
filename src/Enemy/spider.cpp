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
	JGeometry::TVec3<f32> fVar712 = param_1->mLinearVelocity;
	JGeometry::TVec3<f32> local_50;
	local_50.add(param_1->mPosition, fVar712);

	if (param_1->checkLiveFlag2(0x80)) {
		JGeometry::TVec3<f32> local_5C = param_1->mVelocity;
		local_50 += local_5C;
		f32 dVar7 = param_1->getGravityY();
		local_5C.y -= dVar7;
		if (local_5C.y < TLiveActor::mVelocityMinY)
			local_5C.y = TLiveActor::mVelocityMinY;

		param_1->mVelocity = local_5C;
	}

	const TBGCheckData* local_60;
	f32 fVar10 = ((TSpineEnemy*)param_1)->mBodyScale * param_1->mHeadHeight;
	f32 fVar3  = 1.0f
	            + gpMap->checkGround(local_50.x, local_50.y + fVar10,
	                                 local_50.z, &local_60);

	if (param_1->mPosition.y - local_50.y > 0.0f) {
		const TBGCheckData* local_64;
		f32 fVar11 = ((TSpineEnemy*)param_1)->mBodyScale * param_1->mHeadHeight;
		f32 dVar7  = 1.0f
		            + gpMap->checkGround(local_50.x, local_50.y + fVar11,
		                                 local_50.z, &local_64);
		if (dVar7 > fVar3) {
			local_60 = local_64;
			fVar3    = dVar7;
		}
	}

	if (local_60->checkFlag2(0x10)) {
		if (unk4 <= 0) {
			param_1->kill();
		} else {
			fVar3      = param_1->mPosition.y;
			local_50.y = fVar3;
			unk4 -= 1;
		}
	} else {
		unk4 = 0x1E;
	}

	if (local_50.y <= fVar3) {
		param_1->mVelocity = JGeometry::TVec3<f32>(0, 0, 0);

		param_1->offLiveFlag(0x80);
		param_1->offLiveFlag(0x8000);
		local_50.y = fVar3;
	} else {
		param_1->onLiveFlag(0x80);
	}

	param_1->mGroundHeight = fVar3;
	param_1->mGroundPlane  = local_60;

	TBGWallCheckRecord local_90(
	    local_50.x, local_50.y, local_50.z,
	    ((TSpineEnemy*)param_1)->mBodyScale * param_1->mHeadHeight, 1, 0);

	JGeometry::TVec3<f32> local_9c;

	f32 unaff_f29;

	if (!gpMap->isTouchedWallsAndMoveXZ(&local_90)) {
		f32 fVar1;
		if (unk8 > 0) {
			unk8 -= 1;
			fVar1 = ((TSpineEnemy*)param_1)->mMarchSpeed;
			param_1->offLiveFlag(0x80);
			param_1->offLiveFlag(0x8000);
			param_1->mVelocity = JGeometry::TVec3<f32>(0, 0, 0);
		} else {
			unkC = 0;

			((TSpineEnemy*)param_1)->unk138 = nullptr;

			fVar1 = 0.0f;
		}
		unaff_f29 = fVar1;
		unk10 -= 0.016666667f;
		if (unk10 < 0.0f)
			unk10 = 0.0f;
	} else {
		if (local_90.unk1C[0]->mNormal.dot(fVar712)) {
			param_1->offLiveFlag(0x80);
			param_1->offLiveFlag(0x8000);
			param_1->mVelocity = JGeometry::TVec3<f32>(0, 0, 0);

			unkC = local_90.unk1C[0];

			((TSpineEnemy*)param_1)->unk138 = unkC;

			unk8 = 0x3C;

			f32 fVar1 = unk10 * ((TSpineEnemy*)param_1)->mBodyScale
			            * ((TSpineEnemy*)param_1)->mWallRadius;
			JGeometry::TVec3<f32> tmp;
			tmp.scale(fVar1, local_90.unk1C[0]->mNormal);
			local_9c.sub(tmp, local_90.unk0);

			unk10 += 0.016666667f;
			if (unk10 > 1.0f)
				unk10 = 1.0f;
		}
	}

	JGeometry::TVec3<f32> local_118 = local_9c;
	local_118.y += unaff_f29
	               - ((TSpineEnemy*)param_1)->mBodyScale * param_1->mHeadHeight;

	local_118 -= param_1->mPosition;

	param_1->mLinearVelocity = local_118;
}
