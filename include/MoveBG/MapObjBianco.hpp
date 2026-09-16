#ifndef MOVEBG_MAPOBJBIANCO_HPP
#define MOVEBG_MAPOBJBIANCO_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjFloat.hpp>
#include <MoveBG/MapObjHide.hpp>
#include <MoveBG/MapObjTurn.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/LiveActor.hpp>

// Class hierarchy recovered from each destructor's vtable chain. Only the
// destructors are reconstructed; no other member is written yet. The
// constructors are fabricated -- the original supplies each object's own name.

class TBellWatermill : public TMapObjTurn {
public:
	TBellWatermill()
	    : TMapObjTurn()
	{
	}

	virtual ~TBellWatermill();
};

class TBiancoBell : public TMapObjBase {
public:
	TBiancoBell()
	    : TMapObjBase()
	{
	}

	virtual ~TBiancoBell();
};

class TBiancoMiniWindmill : public THideObjBase {
public:
	TBiancoMiniWindmill()
	    : THideObjBase()
	{
	}

	virtual ~TBiancoMiniWindmill();
};

class TBGCheckData;

class TBiancoWatermill : public TMapObjBase {
public:
	TBiancoWatermill()
	    : TMapObjBase()
	{
	}

	virtual ~TBiancoWatermill();

	void turnByEnemy(THitActor*, const TBGCheckData*);
};

class TBiancoWatermillVertical : public TMapObjBase {
public:
	TBiancoWatermillVertical()
	    : TMapObjBase()
	{
	}

	virtual ~TBiancoWatermillVertical();
};

class TBigWindmill : public TMapObjBase {
public:
	TBigWindmill()
	    : TMapObjBase()
	{
	}

	virtual ~TBigWindmill();
};

class TLampSeesaw : public TMapObjBase {
public:
	TLampSeesaw()
	    : TMapObjBase()
	{
	}

	virtual ~TLampSeesaw();
};

class TLampSeesawMain : public TLampSeesaw {
public:
	TLampSeesawMain()
	    : TLampSeesaw()
	{
	}

	virtual ~TLampSeesawMain();
};

class TLeafBoat : public TMapObjBase {
public:
	TLeafBoat()
	    : TMapObjBase()
	{
	}

	virtual ~TLeafBoat();
};

class TLeafBoatRotten : public TLeafBoat {
public:
	TLeafBoatRotten()
	    : TLeafBoat()
	{
	}

	virtual ~TLeafBoatRotten();
};

class TMapObjRootPakkun : public TLiveActor {
public:
	TMapObjRootPakkun()
	    : TLiveActor()
	{
	}

	virtual ~TMapObjRootPakkun();
};

class TWoodLog : public TMapObjFloatOnSea {
public:
	TWoodLog()
	    : TMapObjFloatOnSea()
	{
	}

	virtual ~TWoodLog();
};

#endif
