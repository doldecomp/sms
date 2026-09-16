#ifndef ENEMY_TOBI_PUKU_HPP
#define ENEMY_TOBI_PUKU_HPP

#include <Enemy/WalkerEnemy.hpp>

// The flying pukupuku. TMoePuku and TPukuPuku are variants that share its
// whole animation interface and differ only in their vtable.
class TTobiPuku : public TWalkerEnemy {
public:
	TTobiPuku(const char* name);

	virtual ~TTobiPuku();
	virtual void setAttackAnm();
	virtual void setDeadAnm();
	virtual void setDownAirAnm();
	virtual void setDownLandAnm();
	virtual void setFallAnm();
	virtual void setFallEndLandAnm();
	virtual void setJumpAnm();
	virtual void setJumpStartAnm();
	virtual void setPichiAnm();
	virtual void setSwimAnm();
	virtual BOOL isAttackBck();
	virtual BOOL isDeadBck();
	virtual BOOL isFallEndLandBck();
	virtual BOOL isJumpBck();
	virtual BOOL isJumpStartBck();
	virtual bool isPichiEffect();
};

class TMoePuku : public TTobiPuku {
public:
	TMoePuku(const char* name)
	    : TTobiPuku(name)
	{
	}

	virtual ~TMoePuku();
	virtual void setAttackAnm();
	virtual void setDeadAnm();
	virtual void setDownAirAnm();
	virtual void setDownLandAnm();
	virtual void setFallAnm();
	virtual void setFallEndLandAnm();
	virtual void setJumpAnm();
	virtual void setJumpStartAnm();
	virtual void setPichiAnm();
	virtual void setSwimAnm();
	virtual BOOL isAttackBck();
	virtual BOOL isDeadBck();
	virtual BOOL isFallEndLandBck();
	virtual BOOL isJumpBck();
	virtual BOOL isJumpStartBck();
	virtual bool isPichiEffect();
};

#endif
