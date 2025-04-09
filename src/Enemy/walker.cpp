#include <Enemy/Walker.hpp>
#include <Enemy/Spider.hpp>
#include <Map/MapData.hpp>

static void calcFarthestVertex(const TBGCheckData*,
                               const JGeometry::TVec3<f32>&,
                               const JGeometry::TVec3<f32>&)
{
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
	unk20 = 0x1E;
	unk24 = 0;
	unk4.clear();
}

void TWalker::bind(TLiveActor*) { }

void TWalker::setMode(int param_1)
{
	unk28 = param_1;
	if (unk28 == 1 && unk2C == nullptr)
		unk2C = new TSpider;
}
