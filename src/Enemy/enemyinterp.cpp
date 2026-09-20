#include <Strategic/LiveActor.hpp>
#include <Strategic/spcinterp.hpp>

static void enemyInterpPush(TSpcTypedInterp<TLiveActor>* interp)
{
	interp->push();
}

static TSpcBinary* enemyInterpCreateBinary(void* data)
{
	return new TSpcTypedBinary<TLiveActor>(data);
}
