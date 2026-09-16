#include <Strategic/LiveActor.hpp>
#include <Strategic/spcinterp.hpp>
#include <Strategic/Spine.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <MarioUtil/MathUtil.hpp>

#include <M3DUtil/InfectiousStrings.hpp> // rogue include needed for matching data

// The enemy script binary. Every one of its members is UNUSED in the map, so
// only its presence matters: deriving from TSpcTypedBinary<TLiveActor> is what
// instantiates that template's weak destructor and vtable, which are the only
// things this translation unit actually contributes to the binary.
class TEinBinary : public TSpcTypedBinary<TLiveActor> {
public:
	TEinBinary(void* data)
	    : TSpcTypedBinary<TLiveActor>(data)
	{
	}

	virtual ~TEinBinary();
	virtual void initUserBuiltin();
};

// TODO: incorrect size. Map records 0xdc (220); these bodies are reconstructed
// from the builtin names alone, since every ein* function is UNUSED and has no
// assembly to compare against. The same applies to the four below.
void einGetLengthToMario(TSpcTypedInterp<TLiveActor>* interp, u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	TLiveActor* owner = interp->getOwner();
	SpcTrace("einGetLengthToMario\n");
	interp->push(owner->mPosition.y);
}

// TODO: incorrect size. Map records 0xf0 (240).
void einGetLengthToCurPathNode(TSpcTypedInterp<TLiveActor>* interp, u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	TLiveActor* owner = interp->getOwner();
	SpcTrace("einGetLengthToCurPathNode\n");
	interp->push(owner->mPosition.y);
}

// TODO: incorrect size. Map records 0xa8 (168).
void einGoToRandomNextGraphNode(TSpcTypedInterp<TLiveActor>* interp, u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	interp->push();
}

// TODO: incorrect size. Map records 0x15c (348).
void einWalkToCurPathNode(TSpcTypedInterp<TLiveActor>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	interp->pop();
	interp->push();
}

// TODO: incorrect size. Map records 0x198 (408).
void einSetGraph(TSpcTypedInterp<TLiveActor>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	interp->pop();
	interp->push();
}

// TODO: incorrect size. Map records 0xa0 (160).
void TEinBinary::initUserBuiltin()
{
	bindSystemDataToSymbol("setGraph", (u32)&einSetGraph);
	bindSystemDataToSymbol("walkToCurPathNode", (u32)&einWalkToCurPathNode);
	bindSystemDataToSymbol("goToRandomNextGraphNode",
	                       (u32)&einGoToRandomNextGraphNode);
	bindSystemDataToSymbol("getLengthToCurPathNode",
	                       (u32)&einGetLengthToCurPathNode);
	bindSystemDataToSymbol("getLengthToMario", (u32)&einGetLengthToMario);
}

// TODO: incorrect size. Map records 0x74 (116).
TEinBinary::~TEinBinary() { }
