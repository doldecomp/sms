#ifndef J3D_GRAPH_BASE_COMPONENTS_INDTEXORDER
#define J3D_GRAPH_BASE_COMPONENTS_INDTEXORDER

#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>

extern const J3DIndTexOrderInfo j3dDefaultIndTexOrderNull;

class J3DIndTexOrder : public J3DIndTexOrderInfo {
public:
	J3DIndTexOrder() { *(J3DIndTexOrderInfo*)this = j3dDefaultIndTexOrderNull; }

	u8 getCoord() const { return mCoord; }
	u8 getMap() const { return mMap; }

	void load(u32) const;
};

#endif
