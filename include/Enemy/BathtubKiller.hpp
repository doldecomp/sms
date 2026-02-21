#ifndef BATHTUB_KILLER
#define BATHTUB_KILLER

#include <Enemy/Enemy.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/Walker.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Spider.hpp>
#include <Enemy/Conductor.hpp>

class TBathtubKiller : public TSmallEnemy {
public:
	TBathtubKiller(const char* name = "バスタブキラー");

	void makeQuat(JGeometry::TVec3<f32> axis, float moveAmountY,
	              float moveAmountX);

	/* 0x194 */ u8 unk194[0x1AC - 0x194];
	/* 0x1AC */ JGeometry::TQuat4<f32> mQuat;
	/* 0x1BC */ u8 unk1BC[0x250 - 0x1BC];
};

#endif
