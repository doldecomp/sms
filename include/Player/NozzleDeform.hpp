#ifndef NOZZLEDEFORM_HPP
#define NOZZLEDEFORM_HPP

#include <Player/NozzleTrigger.hpp>
#include <Player/NozzleBase.hpp>

class TNozzleDeform : public TNozzleBase {
public:
	TNozzleDeform(const char* name, const char* prm, TWaterGun* fludd);

	virtual s32 getNozzleKind() const { return 3; }
	virtual void movement(const TMarioControllerWork&);
	virtual void emit(int);
	virtual void animation(int);

	/* 0x384 */ TNozzleTrigger mBomb;
};

#endif
