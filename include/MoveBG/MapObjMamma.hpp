#ifndef MOVE_BG_MAP_OBJ_MAMMA_HPP
#define MOVE_BG_MAP_OBJ_MAMMA_HPP

#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjEx.hpp>

// TODO: mark virtual methods as such

class TSandLeaf : public TMapObjBase {
public:
	u32 touchWater(THitActor*);
	void control();
	TSandLeaf();
};

class TSandBase : public TMapObjBase {
public:
	void isDown() const;
	void withering();
	TSandBase(const char*);
};

class TSandLeafBase : public TSandBase {
public:
	void grow();
	void control();
	void initMapObj();
	TSandLeafBase();
};

class TSandBomb : public TSandLeaf {
public:
	void makeObjAppeared();
	u32 touchWater(THitActor*);
	u32 getSDLModelFlag() const;
	void initMapObj();
};

class TSandBombBase : public TSandBase {
public:
	void withered();
	void expanded();
	void exploding();
	void explode();
	void waitBeforeExplode();
	void grow();
	void control();
	void findTriggerActor();
	void loadAfter();
	void initMapObj();
	TSandBombBase(const char*);
};

class TSandCastle : public TSandBombBase {
public:
	void withering();
	void expanded();
	void explode();
	void waitBeforeExplode();
	void calcRootMatrix();
	void findTriggerActor();
	void loadAfter();
	void initMapObj();
	TSandCastle(const char*);
};

class TLeanMirror : public TMapObjBase {
public:
	void enemyIsOn() const;
	void draw() const;
	void updateSpeedVec(const JGeometry::TVec3<f32>&, f32);
	u32 receiveMessage(THitActor*, u32);
	void touchPlayer(THitActor*);
	void touchEnemy(THitActor*);
	void calcCurrentMtx(MtxPtr);
	void release();
	void controlGoTarget();
	void controlShake();
	void control();
	void loadAfter();
	u32 getSDLModelFlag() const;
	void initMapObj();
	void load(JSUMemoryInputStream&);
	TLeanMirror(const char*);
};

class TShiningStone : public THitActor {
public:
	void endDemo();
	void putOnLight(TLiveActor*);
	void perform(u32, JDrama::TGraphics*);
	void load(JSUMemoryInputStream&);
	TShiningStone(const char*);
};

class TMammaBlockRotate : public TMapObjBase {
public:
	u32 touchWater(THitActor*);
	void control();
	void initMapObj();
	void load(JSUMemoryInputStream&);
	TMammaBlockRotate(const char*);
};

class TMammaYacht : public TMapObjBase {
public:
	void control();
	void initMapObj();
	TMammaYacht();
};

class TSandBird : public TJointCoin {
public:
	void control();
	void makeObjFromJointName(const char*, unsigned short);
	void nameIsObj(const char*);
	void initMapObj();
	TSandBird(const char*);
};

class TGoalWatermelon : public TMapObjBase {
public:
	void touchActor(THitActor*);
	void control();
	void loadAfter();
	void load(JSUMemoryInputStream&);
	TGoalWatermelon(const char*);
};

class TMammaMirrorMapOperator : public JDrama::TViewObj {
public:
	void show(int);
	void hide(int);
	void perform(u32, JDrama::TGraphics*);
	void loadAfter();
	TMammaMirrorMapOperator(const char*);
};

class TSandEgg : public TMapObjBase {
public:
	u32 getSDLModelFlag() const;
	TSandEgg();
};

#endif
