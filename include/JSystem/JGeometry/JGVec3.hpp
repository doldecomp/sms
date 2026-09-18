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

	// fabricated
	operator Vec*() const { return (Vec*)&x; }
	operator const Vec*() const { return (Vec*)&x; }

	void zero() { x = y = z = 0.0f; }

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

	void scaleAdd(f32 scale, const TVec3& b, const TVec3& c)
	{
		x = b.x + c.x * scale;
		y = b.y + c.y * scale;
		z = b.z + c.z * scale;
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
