#ifndef ENEMY_EFFECT_OBJ_HPP
#define ENEMY_EFFECT_OBJ_HPP

#include <Enemy/Enemy.hpp>

class TEffectModel : public TSpineEnemy {
public:
	TEffectModel(const char*);

	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void control();
	virtual void moveObject();
	virtual void reset();
};

class TEffectColumWater : public TEffectModel {
public:
	TEffectColumWater(const char* name = "エフェクト水柱");
	virtual void init(TLiveManager*);
	virtual void reset();

	void generate(JGeometry::TVec3<f32>&, JGeometry::TVec3<f32>&);
};

#endif
