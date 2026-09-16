#ifndef MOVEBG_MAPOBJMAMMA_HPP
#define MOVEBG_MAPOBJMAMMA_HPP

#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjEx.hpp>
#include <Strategic/HitActor.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>

// Class hierarchy recovered from each destructor's vtable chain. Only the
// destructors are reconstructed; no other member is written yet.

class TSandEgg : public TMapObjBase {
public:
	// fabricated: the original supplies its own name here.
	TSandEgg()
	    : TMapObjBase()
	{
	}

	virtual ~TSandEgg();
};

class TSandBase : public TMapObjBase {
public:
	// fabricated: the original supplies its own name here.
	TSandBase()
	    : TMapObjBase()
	{
	}

	virtual ~TSandBase();
};

class TSandBombBase : public TSandBase {
public:
	// fabricated: the original supplies its own name here.
	TSandBombBase()
	    : TSandBase()
	{
	}

	virtual ~TSandBombBase();
};

class TMammaMirrorMapOperator : public JDrama::TViewObj {
public:
	// fabricated: the original supplies its own name here.
	TMammaMirrorMapOperator()
	    : JDrama::TViewObj()
	{
	}

	virtual ~TMammaMirrorMapOperator();
	// fabricated: TViewObj::perform is pure virtual and the map records no
	// out-of-line override for this class.
	virtual void perform(u32 cue, JDrama::TGraphics* graphics) { }
};

class TGoalWatermelon : public TMapObjBase {
public:
	// fabricated: the original supplies its own name here.
	TGoalWatermelon()
	    : TMapObjBase()
	{
	}

	virtual ~TGoalWatermelon();
};

class TWatermelonStatic : public TMapObjBase {
public:
	// fabricated: the original supplies its own name here.
	TWatermelonStatic()
	    : TMapObjBase()
	{
	}

	virtual ~TWatermelonStatic();
};

class TSandBird : public TJointCoin {
public:
	// fabricated: the original supplies its own name here.
	TSandBird()
	    : TJointCoin()
	{
	}

	virtual ~TSandBird();
};

class TMammaYacht : public TMapObjBase {
public:
	// fabricated: the original supplies its own name here.
	TMammaYacht()
	    : TMapObjBase()
	{
	}

	virtual ~TMammaYacht();
};

class TMammaBlockRotate : public TMapObjBase {
public:
	// fabricated: the original supplies its own name here.
	TMammaBlockRotate()
	    : TMapObjBase()
	{
	}

	virtual ~TMammaBlockRotate();
};

class TShiningStone : public THitActor {
public:
	// fabricated: the original supplies its own name here.
	TShiningStone()
	    : THitActor()
	{
	}

	virtual ~TShiningStone();
};

class TLeanMirror : public TMapObjBase {
public:
	// fabricated: the original supplies its own name here.
	TLeanMirror()
	    : TMapObjBase()
	{
	}

	virtual ~TLeanMirror();
};

class TSandCastle : public TSandBombBase {
public:
	// fabricated: the original supplies its own name here.
	TSandCastle()
	    : TSandBombBase()
	{
	}

	virtual ~TSandCastle();
};

class TSandLeafBase : public TSandBase {
public:
	// fabricated: the original supplies its own name here.
	TSandLeafBase()
	    : TSandBase()
	{
	}

	virtual ~TSandLeafBase();
};

class TSandLeaf : public TSandLeafBase {
public:
	TSandLeaf()
	    : TSandLeafBase()
	{
	}

	// Defined in MapObjManager.cpp, not here.
	virtual ~TSandLeaf();
};

class TSandBomb : public TSandLeaf {
public:
	TSandBomb()
	    : TSandLeaf()
	{
	}

	virtual ~TSandBomb();
};

#endif
