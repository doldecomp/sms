#ifndef ENEMY_ENEMY_INTERP_HPP
#define ENEMY_ENEMY_INTERP_HPP

#include <Strategic/spcinterp.hpp>

class TLiveActor;

class TEinBinary : public TSpcTypedBinary<TLiveActor> {
public:
	TEinBinary(void* data)
	    : TSpcTypedBinary<TLiveActor>(data)
	{
	}

	virtual void initUserBuiltin();
};

#endif
