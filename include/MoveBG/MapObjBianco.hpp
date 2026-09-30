#ifndef MOVEBG_MAP_OBJ_BIANCO_HPP
#define MOVEBG_MAP_OBJ_BIANCO_HPP

#include <MoveBG/MapObjBase.hpp>

class TBiancoWatermill : public TMapObjBase {
public:
	void turnByEnemy(THitActor*, const TBGCheckData*);
};

#endif
