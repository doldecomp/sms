#ifndef JG_VEC3_HPP
#define JG_VEC3_HPP

#include <dolphin/types.h>
#include <dolphin/mtx.h>
#include <JSystem/JGeometry/JGUtil.hpp>

namespace JGeometry {

template <class T> class TVec3 { };

template <> struct TVec3<s16> : public S16Vec {
public:
	TVec3() { }

	TVec3(const S16Vec& b) { set(b.x, b.y, b.z); }

	// fabricated
	TVec3(s16 x_, s16 y_, s16 z_) { set(x_, y_, z_); }

	void set(s16 x_, s16 y_, s16 z_)
	{
		x = (s16)x_;
		y = (s16)y_;
		z = (s16)z_;
	}

	void zero() { x = y = z = 0; }

	void add(const TVec3& operand)
	{
		x += operand.x;
		y += operand.y;
		z += operand.z;
	}

	void add(const TVec3& a, const TVec3& b)
	{
		x = a.x + b.x;
		y = a.y + b.y;
		z = a.z + b.z;
	}

	void sub(const TVec3& translate)
	{
		x -= translate.x;
		y -= translate.y;
		z -= translate.z;
	}

	void sub(const TVec3& fst, const TVec3& snd)
	{
		x = fst.x - snd.x;
		y = fst.y - snd.y;
		z = fst.z - snd.z;
	}

	TVec3& operator+=(const TVec3& other)
	{
		add(other);
		return *this;
	}

	TVec3& operator-=(const TVec3& other)
	{
		sub(other);
		return *this;
	}

	// fabricated and fake and UB but it makes things match??
	// The LEFT operand is by value, and that is settled (header round 12):
	// binding it to a `const TVec3&` instead loses retail's three-word copy
	// in front of `bl TVec3::sub` at every lvalue call site (NPCNeckCallBack).
	// Ruled out: returning `TVec3` by value instead of the UB reference.
	// Project-wide that is total fuzzy_match 96.90 -> 96.82: it wins
	// TMapWire::setFootPointsAtHanged 92.43 -> 99.59, getPosInWire
	// 85.94 -> 99.36 and two exact TVec3 helpers in MarioCollision, but loses
	// CLBRotatePosAndUp 83.84 -> 75.16, TSpineEnemy::doShortCut 94.72 ->
	// 84.75, THamuKuri::isResignationAttack 99.71 -> 89.54 and some thirty
	// others.
	friend const TVec3& operator-(TVec3 fst, const TVec3& snd)
	{
		fst -= snd;
		return fst;
	}

	// fabricated and fake and UB but it makes things match??
	friend const TVec3& operator+(TVec3 fst, const TVec3& snd)
	{
		fst += snd;
		return fst;
	}
};

template <> class TVec3<f32> : public Vec {
public:
	TVec3() { }

	TVec3(const Vec& b) { set(b); }

	template <class T> TVec3(T x_, T y_, T z_) { set(x_, y_, z_); }

	explicit TVec3(f32 value) { setAll(value); }

	TVec3(const TVec3& other)
	{
		// NOTE: yes, this has to use lwz/stw and not lfs/stf.
		// Checked via MarioCollision.cpp where this is not inlined
		*(Vec*)this = *(Vec*)&other;
	}

	TVec3& operator=(const TVec3& other)
	{
		// NOTE: yes, this has to use lwz/stw and not lfs/stf.
		// Checked via enemy.cpp where this is not inlined
		*(Vec*)this = *(Vec*)&other;
		return *this;
	}

	TVec3& operator=(const Vec& other)
	{
		*(Vec*)this = other;
		return *this;
	}

	// fabricated
	operator Vec*() const { return (Vec*)&x; }
	operator const Vec*() const { return (Vec*)&x; }

	void zero() { x = y = z = 0.0f; }

	// The map emits this one weak body in Camera.a lensflare.cpp at 0x1c
	// (three lfs, three stfs, blr), which confirms the three statements, and
	// the whole game calls it out of line exactly eight times: lensflare
	// (three), bosstelesa (two), sunmodel, CameraWarp and EventWatcher. Like
	// the other three-statement TVec3 members that is an inline *depth* tell
	// and nothing else - measured on TSunModel::perform (header round 24),
	// depth 3 still expands it and depth 4 produces the call, and dropping
	// the copy in is worth the whole rest of that function. Note the overload:
	// a `const TVec3<f32>&` argument picks the set<T> member template
	// instead, so every one of those eight sites read its source through
	// something typed `Vec`.
	void set(const Vec& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	// TODO: retail emits set<f>__Q29JGeometry8TVec3<f>Ffff as an *internal*
	// (local) 0x10-byte symbol in 58 objects and calls it from some sites
	// while expanding it at others. This in-class member template already
	// gives that exact name and that exact `local` linkage - our build emits
	// it in 43 objects and `Kazekun` matches it byte for byte - so the
	// structure of this declaration is not the problem. What is still missing
	// is the *per-call-site* inline decision: the units that list the symbol
	// MISSING (`wireTrap`, `killer`, ...) expand it everywhere, so the fix
	// belongs at the one call site per TU that retail leaves out of line, not
	// here. Structures tried against MWCC 1.2.5 with the game flags:
	//
	//   in-class member template (this one)   local, but always inlinable
	//   out-of-class `template <class TY> void TVec3<f32>::set(...)`
	//                                         error: illegal function definition
	//   ... same, qualified as JGeometry::TVec3<float>::set
	//                                         error: illegal function definition
	//   ... same, through a `typedef TVec3<f32>` qualifier
	//                                         error: illegal function definition
	//   explicit `template <> void TVec3<f32>::set<f32>(...)` out of class
	//                                         error: unimplemented C++ feature
	//   member template on the *primary* template, body out of class
	//   (`template <class T> template <class TY> void TVec3<T>::set(...)`)
	//                                         compiles, right mangled name,
	//                                         never inlined - but **weak**,
	//                                         not local, and it would stop
	//                                         every site from expanding
	//   non-template `void set(f32, f32, f32)` defined out of class
	//                                         wrong name (`set__...Ffff`),
	//                                         global linkage
	//
	// Do not reach for the primary-template form: weak linkage plus
	// never-inline is further from retail than what is here now.
	//
	// Nor is an extra inline level between the `TVec3(T, T, T)` constructor
	// and this member the answer, even though it is the only place a level
	// *can* go for a constructor reached through an initialiser list (the
	// deriving class's own header has no room: an initialiser list calls
	// TVec3's constructor directly). Header round 21 measured
	// `TVec3(T x_, T y_, T z_) { setEach_(x_, y_, z_); }` over a one-line
	// forwarder: it breaks the DOL and is total -28 exact functions. It does
	// exactly what the depth model predicts and confirms the per-call-site
	// reading -- the eight units that list `set<f>` MISSING (AnimalBase,
	// Bird, pakkun, killer, wireTrap, Koopa, TabePuku,
	// MarNameRefGen_BossEnemy) all gain retail's `bl`, but the seven that
	// had it exact (cameragc, MtxUtil, GCLogoDir, MarDirectorInitECT,
	// MenuDir, MovieDirector, MarDirectorSetupObjects) lose it, with 149
	// function regressions against 13 improvements. A level here is
	// all-or-nothing and the population is split, so the level has to come
	// from the call site's own chain.
	//
	// Batch 104 measured that last row on the *non*-template members too and
	// it is wrong for them as well. Every `TVec3<f>` member the map lists is
	// weak, emitted once and called, but they are all in-class: retail's own
	// `div` (weak 0x30, boid.cpp) expands `scale` inside its body. The `bl`s
	// are the ordinary depth allowance - a three-statement member goes out of
	// line at depth 4 and a one-statement member at depth 5 - so a retail
	// `bl` to one of these is a measurement of the *call site's* depth, not a
	// property of this header. Moving `scale` out as a primary-template
	// member does reproduce those `bl`s (tamaNoko's `landEffect` 57 -> 83,
	// `TIgaiga::setMeltAnm` 84 -> 96) and costs 217 other functions,
	// `div` itself among them. Fix the missing inline levels in the .cpp.
	template <class TY> void set(TY x_, TY y_, TY z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}

	template <class TY> void set(const TVec3<TY>& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	template <class TY> void setAll(TY value)
	{
		x = value;
		y = value;
		z = value;
	}

	// === arithmetic stuff ===

	void add(const TVec3& operand)
	{
		x += operand.x;
		y += operand.y;
		z += operand.z;
	}

	void add(const TVec3& a, const TVec3& b)
	{
		x = a.x + b.x;
		y = a.y + b.y;
		z = a.z + b.z;
	}

	void sub(const TVec3& translate)
	{
		x -= translate.x;
		y -= translate.y;
		z -= translate.z;
	}

	void sub(const TVec3& fst, const TVec3& snd)
	{
		x = fst.x - snd.x;
		y = fst.y - snd.y;
		z = fst.z - snd.z;
	}

	void mul(const TVec3& b)
	{
		x *= b.x;
		y *= b.y;
		z *= b.z;
	}

	void mul(const TVec3& fst, const TVec3& snd)
	{
		x = fst.x * snd.x;
		y = fst.y * snd.y;
		z = fst.z * snd.z;
	}

	void div(f32 divisor)
	{
		divisor = 1.0f / divisor;
		scale(divisor);
	}

	TVec3& operator+=(const TVec3& other)
	{
		add(other);
		return *this;
	}
	// The `TVec3&` return is settled (research batch 113). Spelling this
	// `void` is what the `a = b - c` pool residue wants -- it drops the
	// `operator-` parameter temporary 8 bytes, halfway to retail's slot -- but
	// it breaks this member's own out-of-line copy: `__ami__` (weak, 0x34,
	// Tongue.cpp) is 100% with the reference return and 98.5% without it,
	// because retail loads `other.x` before `this->x` and only the
	// `return *this;` body orders the two `lfs` that way. Project-wide the
	// `void` spelling is -1 function and ~35 regressions (koopajr/limitkoopa
	// `bind`, `TLeanMirror::loadAfter`, `TLiveActor::bind`, `TBaseNPC::bind`,
	// `TChuuHana::bind`, `TFlyEnemy::fly`, ... all -0.1 to -0.4), so the 8
	// bytes have to come from somewhere else. See docs/catalog/frame-gaps.md,
	// "Research batch 113".
	TVec3& operator-=(const TVec3& other)
	{
		sub(other);
		return *this;
	}
	TVec3& operator*=(const TVec3& other)
	{
		mul(other);
		return *this;
	}
	TVec3& operator*=(f32 other)
	{
		scale(other);
		return *this;
	}
	TVec3& operator/=(f32 other)
	{
		div(other);
		return *this;
	}

	// fabricated and fake and UB but it makes things match??
	//
	// The stack slot this by-value parameter gets is the last open residue of
	// the 130 retail `bl TVec3::sub(const TVec3&)` sites (research batch 116).
	// Retail puts it 4 bytes above the local floor 0xc with 12 bytes of pool
	// above it; this spelling puts it 16 above with the pool below. The
	// measured law is `prefix = 8 x (reference returns the caller copies out
	// of)`, so this shape (two of them, `operator-` and `operator-=`) can only
	// ever land on 0, 8, 16, ...; extra inline levels are free and do not move
	// it. Prefix 4 exists only for
	//     friend const TVec3& operator-(const TVec3& fst, const TVec3& snd)
	//     { TVec3 r; r = fst; r -= snd; return r; }
	// together with `void operator-=` and `void operator=`, and that shape
	// leaves only 4 bytes above the temp where retail has 12 - every construct
	// that adds bytes above also adds 4 to the prefix. Do not re-try `void
	// operator-=` on its own (batch 113: -1 function, ~35 regressions).
	// See docs/catalog/frame-gaps.md, "Research batch 116".
	//
	// Header round 47 closes the whole `const TVec3&` family. Retail calls
	// `__ami__` out of line twice inside `TYoshiTongue::movement`
	// (0x80267cbc, 0x80267eec), and *this* by-value spelling is what puts
	// both `bl`s there: every left-operand-by-reference shape
	//     TVec3 r; *(Vec*)&r = *(const Vec*)&fst; r -= snd; return r;
	// drops the reloc count in Tongue.o from 2 to 0 and takes `__ami__`
	// MISSING, so retail's `operator-` does copy its left operand into a
	// by-value parameter and does route through `operator-=`. Measured on
	// TCoasterEnemy::bind (frame 0x40, retail temp at 0x10, locals floor
	// 0x10, named local 0x28), pool bytes (below, above) the live temp:
	//     stock (this)                                      (8, 4)
	//     const& + raw Vec copy + operator-=                (8, 4)  temp 0x18
	//     same, as a member operator- (this = left)         (8, 4)  identical
	//     same, returning TVec3 by value                    (8, 4)  identical
	//     const& + TVec3 r(fst) copy ctor                   pool +4, worse
	//     const& + r = fst (operator=)                      frame 0x48, worse
	//     const& + r.sub(snd)             `sub` inlines away, 130 sites lose
	//                                      the `bl`, tree -0.18 fuzzy
	//     const& + r.sub(fst, snd) with the two-argument
	//       `sub` respelled as raw copy + sub(snd)          (0, 4), frame
	//                                      0x38 - the temp lands on retail's
	//                                      slot but the frame is 8 short, and
	//                                      tree-wide it is 56.92 -> 56.85
	//                                      (CameraMarioData unlinks)
	// Every weak `TVec3<f>` body the map lists is already emitted by this
	// header, in the map's TU, at the map's size, at 100% (all 21 of them,
	// including the 4-byte `__ct__...Fv` in CameraBGCheck.cpp), so there is
	// no missing out-of-line copy left to buy a reshape with.
	friend const TVec3& operator-(TVec3 fst, const TVec3& snd)
	{
		fst -= snd;
		return fst;
	}

	// fabricated and fake and UB but it makes things match??
	friend const TVec3& operator+(TVec3 fst, const TVec3& snd)
	{
		fst += snd;
		return fst;
	}

	// @fabricated
	// The by-value return is what the tree wants, even though the slot
	// arithmetic of individual call sites argues for the `const TVec3&`
	// spelling its two siblings above use (header round 25). Both
	//     friend const TVec3& operator*(TVec3 fst, f32 snd)
	//     const TVec3& operator*(f32 other) const   // TVec3 r = *this; ...
	// drop the by-value return temporary, leaving one scale target plus one
	// copy-out per step -- exactly retail's six 12-byte slots in
	// TWarpInCallBack::execute, which goes 73.92 -> 92.47, with
	// TEffectColumWater::generate 91.95 -> 99.79, THamuKuri::forceRoll
	// 90.23 -> 99.66, TRope::moveHead 93.60 -> 99.80, SMS_MakeJointsToArc
	// 84.56 -> 88.83 and ~8 more. Tree-wide both are a net loss (97.44 ->
	// 97.42) and, worse, they drop weak out-of-line copies to MISSING:
	// `div`, `dot` and `TUtil<f>::sqrt` in boid (98.06 -> 90.95) and
	// `__ami__` in Tongue, plus TTamaNoko::landEffect 94.64 -> 82.84,
	// TWalker::bind 92.34 -> 86.02, TConeBeam::calcVertices 95.67 -> 88.33,
	// TMapObjPuncher::touchPlayer 99.72 -> 92.57 and ~10 more. So the
	// reference return is right only where the caller spells `a * f`; the
	// regressing units must spell their products some other way, and the
	// return type cannot be settled until those are respelled.
	// The member form alone (by-value return, no argument level) is inert on
	// TWarpInCallBack and +0.01 total, but still loses `__ami__`.
	friend TVec3 operator*(TVec3 fst, f32 snd)
	{
		fst *= snd;
		return fst;
	}
	friend TVec3 operator/(TVec3 fst, f32 snd)
	{
		fst /= snd;
		return fst;
	}

	f32 dot(const TVec3& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	// TODO: the store order is wrong somewhere. TLimitKoopaJr::moveRun's
	// retail code stores x and y, reloads b.x, then computes and stores z,
	// while this shape computes all three and then stores all three. But the
	// 96 call sites in the tree overwhelmingly prefer the shape below; all
	// three alternatives were measured over the whole tree and every one of
	// them regresses and nothing improves:
	//
	//   x = ...; y = ...; z = ...;   (no temporaries)
	//       JPADrawExecStripeCross 96.85 -> 92.80, DirBillBoard 98.43 ->
	//       94.02, SMS_EmitSinkInPollutionEffect 98.75 -> 95.29, and 12 more
	//   two temporaries, z assigned directly
	//       StripeCross 96.85 -> 94.20, DirBillBoard 98.43 -> 94.68, 13 more
	//   set(_x, _y, _z) in one expression (i.e. cross2's shape)
	//       JPAVecToRotaMtx 56.86 -> 54.72, JPAConvectionField::affect
	//       94.46 -> 92.33, 10 more
	//   three temporaries, then set(_x, _y, _z) (batch 62)
	//       nine tenth-of-a-point gains against TBPHeadHit::throwActor
	//       100.00 -> 95.32, TFruitsBoat::moveObject 93.95 -> 93.58,
	//       TWalker::bind 90.36 -> 90.22 and ~10 more
	//
	//   the same three temporaries declared z, y, x, stores still x, y, z
	//       (header round 22, following batch 131's TU-local clone, which
	//       measured 17 -> 13 markers on SMS_EmitSinkInPollutionEffect). The
	//       clone's result does not transfer to the real member: the same
	//       function goes 99.06 -> 97.62, JPAConvectionField::affect 94.46 ->
	//       92.33, bgIntersectLine 89.74 -> 86.31, TQuat4<f>::setRotate 98.79
	//       -> 97.25 and ~25 more, against 8 tenth-of-a-point gains
	//       (TWallAtGraph::init 99.29 -> 99.64, TKumokun::initAttachPlane
	//       91.02 -> 92.00, TAmiNoko::calcRootMatrix 92.16 -> 92.66), total
	//       97.40 -> 97.39 and no function newly matched. The callee-saved
	//       FPR ranking a clone sees is a property of the clone's own
	//       expansion, not of the declaration order here.
	//
	// So moveRun's order comes from that call site, not from here.
	// Same conclusion from TBGTentacle::setAttackTarget (batch 62), where
	// the whole residue is this: retail computes _x and _y from shared
	// loads, stores both, then *reloads* the operand's x for _z, because
	// the store to a sibling TVec3 local invalidates it. Only a body that
	// computes z after the x/y stores can do that, and that is exactly the
	// second trial above -- so the reload is a property of the aliasing
	// call sites, and the JPA sites that want all three computed first are
	// the majority.
	void cross(const TVec3& a, const TVec3& b)
	{
		f32 _x = a.y * b.z - a.z * b.y;
		f32 _y = a.z * b.x - a.x * b.z;
		f32 _z = a.x * b.y - a.y * b.x;

		x = _x;
		y = _y;
		z = _z;
	}

	// Incorrect!!!
	void cross2(const TVec3& a, const TVec3& b)
	{
		set(a.y * b.z - a.z * b.y, //
		    a.z * b.x - a.x * b.z, //
		    a.x * b.y - a.y * b.x);
	}

	void negate()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	void scale(f32 scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
	}

	void scale(f32 scale, const TVec3& b)
	{
		x = b.x * scale;
		y = b.y * scale;
		z = b.z * scale;
	}

	// Operand order is read off the game's only out-of-line copy, the weak
	// symbol MapMirror.cpp emits (Map.a MapMirror.cpp in the map): the body is
	// `lfs f2, 0(r4); lfs f0, 0(r5); fmadds f0, f2, f1, f0`, i.e. the *scaled*
	// operand is the first reference parameter and the addend the second.
	void scaleAdd(f32 scale, const TVec3& b, const TVec3& c)
	{
		x = b.x * scale + c.x;
		y = b.y * scale + c.y;
		z = b.z * scale + c.z;
	}

	// === length stuff ===

	// The scalar body stays. Header round 13 measured batch 73's alternative
	//     TVec3 diff = *this; diff.sub(other); return TUtil<f32>::sqrt(diff.squared());
	// project-wide against a fixed baseline: 5 functions improve, 16 regress
	// (fuzzy_match total 96.91% -> 96.88%). The wins are real and large --
	// TSpineEnemy::isReachedToGoal 67.5% -> 99.7%, TElecNokonoko::
	// isResignationAttack 82.6% -> 99.8%, TNerveRHGraphWander::execute
	// 91.7% -> 99.8%, TNerveElecCarapaceMove::execute 90.6% -> 95.3% -- and so
	// are the losses: TBossMantaManager::updateMantaEscape 84.4% -> 37.6%, the
	// three TNerveBathtubKiller nerves ~97% -> ~76%, TPictureTelesa::touchActor
	// 94.2% -> 67.4%, TMapObjGrassManager::perform 99.9% -> 88.0%,
	// TMario::hipAttacking 99.8% -> 95.7%, four TEnemyMario functions.
	//
	// So the ROM has *both* spellings and they cannot share this member: the
	// sites that want the copy-and-sub form want a second helper, of which
	// AnimalNerve.cpp's file-scope `calcDist(const TVec3&, const TVec3&)` (a
	// map-confirmed inline, no symbol) is the known instance. Look for that
	// helper's real home rather than changing this body; the majority of the
	// tree's 60-odd sites is the scalar form.
	f32 distance(const TVec3& other) const
	{
		return TUtil<f32>::sqrt((x - other.x) * (x - other.x)
		                        + (y - other.y) * (y - other.y)
		                        + (z - other.z) * (z - other.z));
	}

	f32 squared() const { return dot(*this); }

	// The summed form is right: `-fp_contract on` folds it into two `fmadds`,
	// which is what TShine::calc (the only user of this overload) wants.
	// Ruled out in header round 16: three named squares
	// (`f32 sqX = dx * dx; ... return sqX + sqY + sqZ;`), which keeps the
	// three `fmuls` apart. Retail has that uncontracted shape at
	// CPolarSubCamera::getNoticeActor_'s two notice-distance sites and at
	// ctrlMultiPlayerCamera_'s inner loop, but spelling it here costs
	// TShine::calc (99.9 -> 95.5) and the Camera sites also need the helper to
	// be its own inline level, so they keep a TU-local one (see
	// CameraNoticeSquaredDist). The map has no squared-distance symbol at all
	// and only `CLBSquared<f>__Ff` (weak, 8 bytes, CameraNotice.cpp), so
	// nothing supports a shared helper.
	f32 squared(const TVec3& other) const
	{
		f32 dx = x - other.x;
		f32 dy = y - other.y;
		f32 dz = z - other.z;
		return dx * dx + dy * dy + dz * dz;
	}

	// Ruled out: `TUtil<f32>::sqrt(dot(*this))`, i.e. skipping squared().
	// It was tried because retail calls sqrt while expanding dot in
	// EventWatcher's isNear builtins, but those are exact now and the
	// spelling only costs: JGeometry::TVec3<f>::dot stops being emitted at
	// all in boid.o where the map has a weak copy (100 -> 0%, unit
	// matched_code 31.4 -> 30.7), TBoidLeader::calcBoids 95.5 -> 94.2 and
	// TKazekun::flyAroundMario 93.5 -> 89.4, with no gain anywhere. So
	// squared() is the real intermediate and it is what calls dot.
	f32 length() const { return TUtil<f32>::sqrt(squared()); }

	bool isZero() const { return squared() <= TUtil<f32>::epsilon(); }

	void setLength(f32 length) { setLength(*this, length); }

	void normalize() { setLength(*this, TUtil<f32>::one()); }

	void normalize(const TVec3& other) { setLength(other, TUtil<f32>::one()); }

	void setLength(const TVec3& v, f32 length)
	{
		f32 lsq = v.squared();
		if (lsq <= TUtil<f32>::epsilon()) {
			zero();
			return;
		}

		scale(length * JGeometry::TUtil<f32>::inv_sqrt(lsq), v);
	}

	void setMax(const TVec3& max)
	{
		if (x <= max.x)
			x = max.x;
		if (y <= max.y)
			y = max.y;
		if (z <= max.z)
			z = max.z;
	}

	void setMin(const TVec3& min)
	{
		if (x >= min.x)
			x = min.x;
		if (y >= min.y)
			y = min.y;
		if (z >= min.z)
			z = min.z;
	}

	// from SMG
	bool epsilonEquals(const TVec3& other, f32 eps) const
	{
		return TUtil<f32>::epsilonEquals(x, other.x, eps)
		       && TUtil<f32>::epsilonEquals(y, other.y, eps)
		       && TUtil<f32>::epsilonEquals(z, other.z, eps);
	}

	// fabricated
	bool epsilonEquals(const TVec3& other) const
	{
		return TUtil<f32>::epsilonEquals(x, other.x)
		       && TUtil<f32>::epsilonEquals(y, other.y)
		       && TUtil<f32>::epsilonEquals(z, other.z);
	}

	// TODO: SMG's operator== uses epsilonEquals. Maybe this wasn't operator==
	// but a separate function? Eh, whatever.
	bool operator==(const TVec3& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}
};

} // namespace JGeometry

#endif
