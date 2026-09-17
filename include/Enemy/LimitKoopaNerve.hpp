#ifndef ENEMY_LIMITKOOPANERVE_HPP
#define ENEMY_LIMITKOOPANERVE_HPP

#include <Enemy/LimitKoopa.hpp>
#include <Strategic/Nerve.hpp>

class TLiveActor;

// The nerve singletons in this unit are not the DEFINE_NERVE shape: the map
// mangles their statics as nerve$localstatic0$theNerve__..., which is what
// MWCC emits for a static inside an *inline* function, so theNerve() was
// written out in the header.
//
// TODO: TNerveLimitKoopaTurn's vtable is {0, 0, dtor, 0} and the map has no
// execute for it, so it is an abstract intermediate that TNerveLimitKoopaWait
// and TNerveLimitKoopaTumble derive from (both of their destructors walk
// through its vtable). TNerveLimitKoopaTurnL and TNerveLimitKoopaTurnR exist
// only as two dead-stripped theNerve() statics with no vtables of their own,
// so the two turn-direction singletons cannot be reconstructed from the map;
// they are left out.
class TNerveLimitKoopaTurn : public TNerveBase<TLiveActor> {
public:
	virtual ~TNerveLimitKoopaTurn() { }
};

class TNerveLimitKoopaWait : public TNerveLimitKoopaTurn {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>*) const;
	static const TNerveLimitKoopaWait& theNerve()
	{
		static TNerveLimitKoopaWait nerve;
		return nerve;
	}
};

class TNerveLimitKoopaTumble : public TNerveLimitKoopaTurn {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>*) const;
	static const TNerveLimitKoopaTumble& theNerve()
	{
		static TNerveLimitKoopaTumble nerve;
		return nerve;
	}
};

#define DECLARE_LIMITKOOPA_NERVE(Name)                                         \
	class Name : public TNerveBase<TLiveActor> {                               \
	public:                                                                    \
		virtual BOOL execute(TSpineBase<TLiveActor>*) const;                   \
		static const Name& theNerve()                                          \
		{                                                                      \
			static Name nerve;                                                 \
			return nerve;                                                      \
		}                                                                      \
	};

DECLARE_LIMITKOOPA_NERVE(TNerveLimitKoopaGetDown)
DECLARE_LIMITKOOPA_NERVE(TNerveLimitKoopaGetShowered)
DECLARE_LIMITKOOPA_NERVE(TNerveLimitKoopaStagger)
DECLARE_LIMITKOOPA_NERVE(TNerveLimitKoopaHipDropStart)
DECLARE_LIMITKOOPA_NERVE(TNerveLimitKoopaHipDropJump)
DECLARE_LIMITKOOPA_NERVE(TNerveLimitKoopaFlame)
DECLARE_LIMITKOOPA_NERVE(TNerveLimitKoopaFall)

#endif
