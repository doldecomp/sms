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

	// The three zero stores are written in place on purpose. Header round 19
	// re-measured every way of putting JGeometry::TVec3<f>::set<f> one inline
	// level deeper here, because retail `bl`s it inside this constructor at
	// BeeHive's four nerves while we store the zeros inline:
	//   * `: unk0(actor), unk4(0.0f, 0.0f, 0.0f)` (member initialiser) is the
	//     best of them -- BeeHive Break 94.0 -> 99.1, Attack 95.6 -> 99.3,
	//     MarioWaterIn 94.5 -> 99.3, doWait 91.6 -> 93.7 -- but it turns the
	//     zeroing into a `bl` at every *other* TPathNode site too, where retail
	//     expands it, and costs about fifteen units: TNerveWalkerTraceMario
	//     100 -> 94.3 (a byte-exact function lost), TNerveBombHeiAttack
	//     99.8 -> 81.1, TPakkun::load 99.7 -> 77.9, gesso/hamukuri/telesa/
	//     popo/cannon/bosstelesa/... Total fuzzy 97.33 -> 97.32.
	//   * `unk4 = JGeometry::TVec3<f32>(0, 0, 0);` (a temporary) adds the copy
	//     as well: BeeHive only reaches 95.5/96.7/96.1 and the same units
	//     regress, and it perturbs a linked unit (the DOL stops matching).
	//   * `unk4.set(0.0f, 0.0f, 0.0f);` is codegen-identical everywhere (the
	//     level is not deep enough to force the call), so it buys nothing.
	//   * Adding the level at BeeHive's call sites instead, through a
	//     `TBoidLeader::setGoal` accessor in Animal/boid.hpp, moves frame
	//     slots rather than the call: by-reference (`setGoal(const TPathNode&)`)
	//     gives Break 94.7 / Attack 96.1 but regresses TNerveBeeHiveWait
	//     97.9 -> 94.5, and letting the accessor build the node
	//     (`setGoal(THitActor*)`) is worse still (Wait 97.9 -> 86.8).
	// Retail's spelling is therefore the one below plus a per-call-site level
	// that BeeHive has and the other fifteen users do not; it is not a
	// property of this constructor. Leave the header alone.
	//
	// fabricated
	TPathNode(THitActor* actor)
	{
		unk0   = actor;
		unk4.x = 0;
		unk4.y = 0;
		unk4.z = 0;
		if (actor) {
			unk4.set(actor->mPosition.x, actor->mPosition.y,
			         actor->mPosition.z);
		}
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
