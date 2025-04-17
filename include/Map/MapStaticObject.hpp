#ifndef MAP_MAP_STATIC_OBJECT_HPP
#define MAP_MAP_STATIC_OBJECT_HPP

#include <Strategic/HitActor.hpp>

class JPABaseEmitter;
class J3DModelData;

class TMapStaticObj : public THitActor {
public:
	TMapStaticObj(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();

	void init(const char*);
	void initModel(const char*);
	void initMapCollision(const char*);
	void initUnique();
	void perform(unsigned long, JDrama::TGraphics*);
	void calcUnique(JPABaseEmitter*);
	void getModel() const;
	J3DModelData* getModelData() const;
};

#endif
