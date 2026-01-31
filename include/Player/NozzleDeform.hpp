#ifndef NOZZLEDEFORM_HPP
#define NOZZLEDEFORM_HPP

#include <Player/NozzleBase.hpp>

class TNozzleDeform : public TNozzleBase {
public:
	TNozzleDeform(const char* name, const char* prm, TWaterGun* fludd)
	    : TNozzleBase(name, prm, fludd)
	    , mBomb(name, "/Mario/WaterGun/NozzleDeformBomb.prm", fludd)
	{
		init();
	}

	virtual s32 getNozzleKind() const { return 3; }
	virtual void movement(const TMarioControllerWork&);
	virtual void emit(int);
	virtual void animation(int);

	/* 0x384 */ TNozzleTrigger mBomb;
};

#endif
