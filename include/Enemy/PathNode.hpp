#ifndef ENEMY_PATH_NODE_HPP
#define ENEMY_PATH_NODE_HPP

#include <Strategic/HitActor.hpp>

class TPathNode {
public:
	~TPathNode() { }

	TPathNode()
	{
		unk0   = nullptr;
		unk4.x = 0;
		unk4.y = 0;
		unk4.z = 0;
	}

	// fabricated
	TPathNode(THitActor* actor)
	{
		unk0   = actor;
		unk4.x = 0;
		unk4.y = 0;
		unk4.z = 0;
	}

	// fabricated
	TPathNode(const JGeometry::TVec3<f32>& point)
	{
		unk0 = nullptr;
		unk4 = point;
	}
	// TODO: this inline doesn't match in a couple of places.
	const JGeometry::TVec3<f32>& getPoint() const
	{
		if (unk0 != 0)
			return unk0->getPosition();

		return unk4;
	}

public:
	/* 0x0 */ THitActor* unk0;
	/* 0x4 */ JGeometry::TVec3<f32> unk4;
};

#endif
