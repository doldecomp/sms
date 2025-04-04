#include <Map/MapCollisionEntry.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapData.hpp>

void TMapCollisionBase::initCheckData(s16 param_1, u16 param_2,
                                      const TLiveActor* param_3)
{
	unk50.x = unk20[0][3];
	unk50.y = unk20[1][3];
	unk50.z = unk20[2][3];

	if (param_2 & 4) {
		initAllCheckData(param_1, &unk14->x, param_2, param_3);
	} else {
		Vec vecs[350]; // TODO: is size correct?
		MTXMultVecArray(unk20, unk14, vecs, unk10);
		initAllCheckData(param_1, &vecs[0].x, param_2, param_3);
	}
}

void TMapCollisionBase::initAllCheckData(s16, const f32*, u16,
                                         const TLiveActor*)
{
}

void TMapCollisionBase::update()
{
	Vec vecs[350]; // TODO: size?
	MTXMultVecArray(unk20, unk14, vecs, unk10);
	for (int i = unkC; i != 0; --i) {
		// TODO: what is unk1C?
		setCheckData(&vecs[0].x, nullptr, &unk4[i], unk8);
	}
}

void TMapCollisionBase::updateCheckData(const f32*) { }

void TMapCollisionBase::updateVertexPos(f32*) { }

void TMapCollisionBase::updateTrans(const JGeometry::TVec3<f32>&) { }

void TBGCheckData::updateTrans(const JGeometry::TVec3<f32>&) { }

void TMapCollisionBase::setCheckData(const f32*, const s16*, TBGCheckData*, int)
{
}

void TMapCollisionMove::setList()
{
	TBGCheckData* data = unk4;
	for (u32 i = unkC; i != 0; --i) {
		gpMapCollisionData->addCheckDataToGrid(data, unk8);
		++data;
	}
}

void TBGCheckData::setVertex(const JGeometry::TVec3<f32>&,
                             const JGeometry::TVec3<f32>&,
                             const JGeometry::TVec3<f32>&)
{
}
