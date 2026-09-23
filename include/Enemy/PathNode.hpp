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
	//     Re-measured in header round 52 against pakkun ladder 352's reading
	//     that it would become the map's weak `bl set<f>` at inline depth 4:
	//     it does not. TStayPakkun::load stays at 80.1% with the three zeros
	//     expanded, pakkun keeps `set<f>__Q29JGeometry8TVec3<f>Ffff` as its
	//     MISSING map symbol, and nothing moves anywhere tree-wide (the whole
	//     tree is byte-identical either way). The chain there is
	//     TStayPakkun::load -> setGoalPathMario (1) -> the implicit TPathNode
	//     conversion, built in *setGoalPathMario's* body (2) -> the zero-init
	//     (3): one level short, because Enemy.hpp's `setGoalPath` takes the
	//     node by const reference and so never builds it itself. Buying that
	//     level (a `setGoalPath(THitActor*)` overload, or any extra step
	//     inside this constructor) turns the zeroing into a `bl` at every
	//     TPathNode(THitActor*) site at once, which is the fifteen-unit
	//     regression measured above. Still per-call-site; still no fix here.
	//   * Adding the level at BeeHive's call sites instead, through a
	//     `TBoidLeader::setGoal` accessor in Animal/boid.hpp, moves frame
	//     slots rather than the call: by-reference (`setGoal(const TPathNode&)`)
	//     gives Break 94.7 / Attack 96.1 but regresses TNerveBeeHiveWait
	//     97.9 -> 94.5, and letting the accessor build the node
	//     (`setGoal(THitActor*)`) is worse still (Wait 97.9 -> 86.8).
	//   * Header round c-pathnode re-ran the initialiser tree-wide: pakkun's
	//     TStayPakkun::load 80.1 -> 100 and BeeHive's three nerves -> ~99, but
	//     every setGoalPathMario() user retail expands (TPakkun::load,
	//     TNervePakkunShoot, TGesso::behaveToFindMario, TNerveBombHeiAttack,
	//     TNervePopoWait, TNerveWalkerTraceMario, ...) then `bl`s the zero set
	//     at depth 4 (setGoalPathMario -> ctor -> TVec3(T,T,T) -> set); total
	//     fuzzy 99.20 -> 99.18. Sites that convert at depth 1 (hamukuri's
	//     `setGoalPath((THitActor*)gpMarioAddress)`) keep expanding it, so the
	//     depth reading holds. Building the node in TPakkun::load's own body
	//     (`setGoalPath((THitActor*)gpMarioAddress)`, a named `mario` local,
	//     `TPathNode(...)` temporary, named node, copy-init node) expands the
	//     zeros but puts the node 4 bytes low (retail 0x18, ours 0x14); a
	//     `const TPathNode&` binding grows the frame by 8. Retail gives
	//     TPakkun::load and TStayPakkun::load the same slots (node 0x18,
	//     frame 0x30) and differs only in the zero set's depth, so no single
	//     TPathNode(THitActor*) body serves both. Keeping body-form zeros and
	//     giving TStayPakkun::load its level by delegating to TPakkun::load
	//     (auto-inlined) pushes the whole constructor out of line (45%), and
	//     direct `unk4.x = ...` position stores do not change that.
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
	// TODO: this inline doesn't match in a couple of places. Header round 27
	// promoted batch 158's TU-local walkerEnemy copy here -- reading
	// `unk0->mPosition` raw instead of through `getPosition()` -- and it is a
	// wash, so it stays parked: it closes TFireWanwan::isMissMario and
	// TNerveBPTouchDown and loses TSpineEnemy::isReachedToGoal's weak copy in
	// fishoid and TNerveBPTakeOff, with total fuzzy flat at 97.46%. Dropping
	// the named `goal` reference in Enemy.hpp's isReachedToGoal alongside it
	// is byte-identical, so the two levers are not the same one and the goal
	// binding is not what the raw read displaces.
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
