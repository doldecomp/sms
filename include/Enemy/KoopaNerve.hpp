#ifndef ENEMY_KOOPANERVE_HPP
#define ENEMY_KOOPANERVE_HPP

#include <Enemy/Koopa.hpp>
#include <Strategic/Nerve.hpp>

class TLiveActor;

// Fabricated, but the ROM's shape: every degree wrap in the Koopa units
// computes `l + std::fmodf((r - l) + (t - l), r - l)` with l = -180,
// r = 180, and the two inline levels above std::fmodf are what make MWCC call
// the weak 0x5c copy the map records for Koopa.cpp (and its unreferenced
// duplicate in limitkoopa.cpp) instead of expanding it. Same pair as
// koopajr.cpp's WrapDirectionF / WrapRadianF, in degrees.
static inline f32 KoopaWrapDirection(f32 t, f32 l, f32 r)
{
	return l + std::fmodf((r - l) + (t - l), r - l);
}

static inline f32 KoopaWrapDegrees(f32 angle)
{
	return KoopaWrapDirection(angle, -180.0f, 180.0f);
}

// The nerve singletons in this unit are not the DEFINE_NERVE shape: the map
// mangles their statics as nerve$localstatic0$theNerve__..., which is what
// MWCC emits for a static inside an *inline* function, so theNerve() was
// written out in the header. Same as limitkoopa.cpp's ten nerves.
//
// TNerveKoopaTurn's vtable is {0, 0, dtor, 0} with no execute slot filled, so
// it is an abstract intermediate. Its children are the five nerves whose
// destructors are 0x6c (two vtable stores) rather than 0x5c: Wait, Tumble,
// TurnL, TurnR and Flame.
class TNerveKoopaTurn : public TNerveBase<TLiveActor> {
public:
	virtual ~TNerveKoopaTurn() { }
};

#define DECLARE_KOOPA_NERVE(Name, Base)                                        \
	class Name : public Base {                                                 \
	public:                                                                    \
		virtual BOOL execute(TSpineBase<TLiveActor>*) const;                   \
		static const Name& theNerve()                                          \
		{                                                                      \
			static Name nerve;                                                 \
			return nerve;                                                      \
		}                                                                      \
	};

DECLARE_KOOPA_NERVE(TNerveKoopaFall, TNerveBase<TLiveActor>)
DECLARE_KOOPA_NERVE(TNerveKoopaGetDown, TNerveBase<TLiveActor>)
DECLARE_KOOPA_NERVE(TNerveKoopaGetShowered, TNerveBase<TLiveActor>)
DECLARE_KOOPA_NERVE(TNerveKoopaProvoke, TNerveBase<TLiveActor>)
DECLARE_KOOPA_NERVE(TNerveKoopaStagger, TNerveBase<TLiveActor>)

DECLARE_KOOPA_NERVE(TNerveKoopaFlame, TNerveKoopaTurn)
DECLARE_KOOPA_NERVE(TNerveKoopaTumble, TNerveKoopaTurn)
DECLARE_KOOPA_NERVE(TNerveKoopaWait, TNerveKoopaTurn)
// These two nerves' execute symbols are weak, so the bodies were written in
// the class. They need the complete TKoopa, which is why the whole nerve set
// lives in this header rather than in Koopa.hpp.
// rather than in KoopaNerve.hpp with the other nine.
class TNerveKoopaTurnR : public TNerveKoopaTurn {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const
	{
		TKoopa* koopa = (TKoopa*)spine->getBody();
		f32 diff
		    = KoopaWrapDegrees(koopa->mTargetDir - koopa->mRotation.y);
		bool turned;
		if (diff > koopa->getParam()->turnSpeed.get())
			turned = koopa->turnBody(koopa->getParam()->turnSpeed.get());
		else if (diff > 0.0f)
			turned = koopa->turnBody(diff);
		else
			turned = false;

		if (turned)
			return FALSE;
		return TRUE;
	}

	static const TNerveKoopaTurnR& theNerve()
	{
		static TNerveKoopaTurnR nerve;
		return nerve;
	}
};

class TNerveKoopaTurnL : public TNerveKoopaTurn {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const
	{
		TKoopa* koopa = (TKoopa*)spine->getBody();
		f32 diff
		    = KoopaWrapDegrees(koopa->mTargetDir - koopa->mRotation.y);
		bool turned;
		if (diff < -koopa->getParam()->turnSpeed.get())
			turned = koopa->turnBody(-koopa->getParam()->turnSpeed.get());
		else if (diff < 0.0f)
			turned = koopa->turnBody(diff);
		else
			turned = false;

		if (turned)
			return FALSE;
		return TRUE;
	}

	static const TNerveKoopaTurnL& theNerve()
	{
		static TNerveKoopaTurnL nerve;
		return nerve;
	}
};

#endif
