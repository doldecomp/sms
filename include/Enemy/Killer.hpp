#ifndef ENEMY_KILLER_HPP
#define ENEMY_KILLER_HPP

#include <Enemy/SmallEnemy.hpp>
#include <Enemy/KillerNerve.hpp>

// TODO: fabricated stub. The map derives TKiller from a TFlyEnemy that is not
// in the tree yet; only what cannon.cpp touches is declared here.
class TKiller : public TSmallEnemy {
public:
	TKiller(const char*);

	void setColorType();
	bool isRollFly();

	/* 0x194 */ u8 unk194[0x1A5 - 0x194];
	/* 0x1A5 */ u8 unk1A5;
	/* 0x1A8 */ JGeometry::TVec3<f32> unk1A8;
};

#endif
