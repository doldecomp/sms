#include <Enemy/BathtubBinder.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JGeometry/JGUtil.hpp>
#include <Map/BathWaterManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MoveBG/MapObjCorona.hpp>

TBathtubBinder::TBathtubBinder()
    : unk4(nullptr)
    , unk8(nullptr)
{
}

TBathtubBinder::~TBathtubBinder() { }

bool TBathtubBinder::init(f32 param_1, f32 param_2, f32 param_3, f32 param_4,
                          f32 param_5)
{
	unk4 = static_cast<TBathtub*>(JDrama::TNameRefGen::search("バスタブ"));
	unk8 = static_cast<TBathWaterManager*>(
	    JDrama::TNameRefGen::search("バスタブの水"));

	unk20 = param_5;
	unkC  = param_1;
	unk10 = param_2;
	unk14 = param_3;
	unk18 = param_4;
	unk1C = unk18 / (unk10 + unk18);

	if (unk4 == nullptr)
		unk8 = nullptr;

	return unk4 != nullptr;
}

void TBathtubBinder::bind(TLiveActor* param_1)
{
	if (unk4 == nullptr || !unk4->unk29A)
		float_(param_1);
}

void TBathtubBinder::float_(TLiveActor* param_1)
{
	if (unk8 == nullptr)
		return;

	Mtx mtx;
	MsMtxSetRotRPH(mtx, param_1->mRotation.x, param_1->mRotation.y,
	               param_1->mRotation.z);
	JGeometry::TVec3<f32> forward(mtx[0][2], mtx[1][2], mtx[2][2]);

	JGeometry::TVec3<f32> front(forward.x * unkC + param_1->mPosition.x,
	                            param_1->mPosition.y,
	                            forward.z * unkC + param_1->mPosition.z);
	constrain_(front, unk10);
	front.y = unk20 + unk8->getWaterHeight(front.x, front.z);

	JGeometry::TVec3<f32> back(forward.x * -unk14 + param_1->mPosition.x,
	                           param_1->mPosition.y,
	                           forward.z * -unk14 + param_1->mPosition.z);
	constrain_(back, unk18);
	back.y = unk20 + unk8->getWaterHeight(back.x, back.z);

	JGeometry::TVec3<f32> delta;
	delta.sub(front, back);

	param_1->mPosition.y
	    += 0.2f * (unk1C * delta.y + back.y - param_1->mPosition.y);

	if (delta.isZero())
		return;

	f32 horizontal
	    = JGeometry::TUtil<f32>::sqrt(delta.x * delta.x + delta.z * delta.z);
	f32 angle = JGeometry::TUtil<f32>::clamp(
	    matan(horizontal, delta.y) * (360.0f / 65536.0f), -15.0f, 15.0f);
	param_1->mRotation.x += 0.1f * (angle - param_1->mRotation.x);
	param_1->mRotation.z = 0.0f;

	constrain_(param_1->mPosition, 0.5f * (unk10 + unk18));
}

void TBathtubBinder::constrain_(JGeometry::TVec3<f32>& param_1, f32 param_2)
{
	if (unk4 == nullptr)
		return;

	const TBathtubData& data     = unk4->getBathtubData();
	JGeometry::TVec3<f32> center = data.getThing();
	f32 radius = JGeometry::TUtil<f32>::sqrt(data.unk3C * data.unk3C
	                                         - data.unk44 * data.unk44)
	             - param_2;
	f32 centerZ = center.z;
	f32 dx      = param_1.x - center.x;
	f32 dz      = param_1.z - centerZ;
	f32 dist    = dx * dx + dz * dz;
	if (dist > radius * radius) {
		f32 scale = radius * JGeometry::TUtil<f32>::inv_sqrt(dist);
		param_1.set(scale * dx + center.x, param_1.y, scale * dz + centerZ);
	}
	if (param_1.y < center.y)
		param_1.y = center.y;
}
