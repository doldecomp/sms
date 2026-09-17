#ifndef ENEMY_KOOPANERVE_HPP
#define ENEMY_KOOPANERVE_HPP

#include <Enemy/Koopa.hpp>
#include <Strategic/Nerve.hpp>

class TLiveActor;

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
		    = -180.0f
		      + std::fmodf(360.0f + ((koopa->mTargetDir - koopa->mRotation.y)
		                             - -180.0f),
		                   360.0f);
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
		    = -180.0f
		      + std::fmodf(360.0f + ((koopa->mTargetDir - koopa->mRotation.y)
		                             - -180.0f),
		                   360.0f);
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
