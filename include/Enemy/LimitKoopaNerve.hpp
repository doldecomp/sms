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
// Declaration order here is the order of the theNerve() local statics in the
// map's .sbss and of the twelve-byte __register_global_object nodes in .bss:
// HipDropJump, HipDropStart, TurnR, TurnL, Tumble, Wait, Flame, GetDown,
// Stagger, Fall, GetShowered.  The .data vtable order is independent of it
// (it follows the reverse definition order of the execute bodies in the
// .cpp), so the two orders must not be conflated.

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

DECLARE_LIMITKOOPA_NERVE(TNerveLimitKoopaHipDropJump)
DECLARE_LIMITKOOPA_NERVE(TNerveLimitKoopaHipDropStart)

// TNerveLimitKoopaTurn's vtable is {0, 0, dtor, 0} and the map has no execute
// for it, so it is an abstract intermediate that TNerveLimitKoopaWait and
// TNerveLimitKoopaTumble derive from (both of their destructors walk through
// its vtable).
class TNerveLimitKoopaTurn : public TNerveBase<TLiveActor> {
public:
	virtual ~TNerveLimitKoopaTurn() { }
};

// TNerveLimitKoopaTurnL and TNerveLimitKoopaTurnR survive only as two
// dead-stripped theNerve() statics; the map has no vtable, no destructor and
// no execute for either, so their execute is declared here and defined
// nowhere.  They still reserve their twelve-byte destructor-registration node
// in .bss, which is what puts the JALList block at 0x84 instead of 0x6c.
class TNerveLimitKoopaTurnR : public TNerveLimitKoopaTurn {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>*) const;
	static const TNerveLimitKoopaTurnR& theNerve()
	{
		static TNerveLimitKoopaTurnR nerve;
		return nerve;
	}
};

class TNerveLimitKoopaTurnL : public TNerveLimitKoopaTurn {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>*) const;
	static const TNerveLimitKoopaTurnL& theNerve()
	{
		static TNerveLimitKoopaTurnL nerve;
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

class TNerveLimitKoopaWait : public TNerveLimitKoopaTurn {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>*) const;
	static const TNerveLimitKoopaWait& theNerve()
	{
		static TNerveLimitKoopaWait nerve;
		return nerve;
	}
};

DECLARE_LIMITKOOPA_NERVE(TNerveLimitKoopaFlame)
DECLARE_LIMITKOOPA_NERVE(TNerveLimitKoopaGetDown)
DECLARE_LIMITKOOPA_NERVE(TNerveLimitKoopaStagger)
DECLARE_LIMITKOOPA_NERVE(TNerveLimitKoopaFall)
DECLARE_LIMITKOOPA_NERVE(TNerveLimitKoopaGetShowered)

#endif
