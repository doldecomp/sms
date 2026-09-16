#include <Strategic/ObjManager.hpp>

class TKiller {
public:
	static unsigned char mSerialBomb;
	static unsigned char mTrampleDie;
};

class TFlyEnemy {
public:
	static float mTestSp;
	static int mInvalidTime;
	static float mTestMarioSpMax;
};

unsigned char TKiller::mSerialBomb = 1;
unsigned char TKiller::mTrampleDie = 1;
float TFlyEnemy::mTestSp = 2.5f;
int TFlyEnemy::mInvalidTime = 200;
float TFlyEnemy::mTestMarioSpMax = 12.0f;

static const int unk1490[] = { 0, 0, 0 };
static const char unk2322[] = "メモリが足りません\n";

static const char* MtxCalcTypeName[] = {
	"MActorMtxCalcType_Basic クラシックスケールＯＮ",
	"MActorMtxCalcType_Softimage クラシックスケールＯＦＦ",
	"MActorMtxCalcType_MotionBlend モーションブレンド",
	"MActorMtxCalcType_User ユーザー定義",
};

static const char* killer_bastable[] = {
	"/scene/killer/bas/downkiller_down1.bas", 0, 0,
	"/scene/killer/bas/killer_search1.bas", 0,
};

static TModelDataLoadEntry entry[] = {
	{ "killer_model1.bmd", 0x10220000, 0 },
	{ "downkiller_model1.bmd", 0x10220000, 0 },
	{ 0, 0, 0 },
};

extern "C" {
void JSGFGetType__Q26JStage6TActorCFv();
void JSGGetAnimationFrameMax__Q26JStage6TActorCFv();
void JSGGetAnimationFrame__Q26JStage6TActorCFv();
void JSGGetAnimation__Q26JStage6TActorCFv();
void JSGGetData__Q26JStage7TObjectCFUlPvUl();
void JSGGetFlag__Q26JStage7TObjectCFv();
void JSGGetName__Q26JStage7TObjectCFv();
void JSGGetParent__Q26JStage7TObjectCFPPQ26JStage7TObjectPUl();
void JSGGetRotation__Q26JDrama6TActorCFP3Vec();
void JSGGetScaling__Q26JDrama6TActorCFP3Vec();
void JSGGetShape__Q26JStage6TActorCFv();
void JSGGetTranslation__Q26JDrama6TActorCFP3Vec();
void JSGSetAnimationFrame__Q26JStage6TActorFf();
void JSGSetAnimation__Q26JStage6TActorFUl();
void JSGSetData__Q26JStage7TObjectFUlPCvUl();
void JSGSetFlag__Q26JStage7TObjectFUl();
void JSGSetParent__Q26JStage7TObjectFPQ26JStage7TObjectUl();
void JSGSetRelation__Q26JStage7TObjectFbPQ26JStage7TObjectUl();
void JSGSetRotation__Q26JDrama6TActorFRC3Vec();
void JSGSetScaling__Q26JDrama6TActorFRC3Vec();
void JSGSetShape__Q26JStage6TActorFUl();
void JSGSetTranslation__Q26JDrama6TActorFRC3Vec();
void __dt__14TKillerManagerFv();
void __dt__21TNerveKillerExplosionFv();
void __dt__22TNerveFlyEnemyChaseFlyFv();
void __dt__23TNerveFlyEnemyNormalFlyFv();
void __dt__7TKillerFv();
void __dt__9TFlyEnemyFv();
void attackToMario__12TWalkerEnemyFv();
void attackToMario__7TKillerFv();
void behaveToFindMario__12TWalkerEnemyFv();
void behaveToRelease__11TSmallEnemyFv();
void behaveToTaken__11TSmallEnemyFP9THitActor();
void behaveToWater__11TSmallEnemyFP9THitActor();
void behaveToWater__7TKillerFP9THitActor();
void belongToGround__10TLiveActorCFv();
void bind__7TKillerFv();
void bind__9TFlyEnemyFv();
void calcRootMatrix__11TSpineEnemyFv();
void calcRootMatrix__7TKillerFv();
void changeByJuice__11TSmallEnemyFv();
void changeDrawBuffer__13TEnemyManagerFUl();
void changeMove__11TSmallEnemyFv();
void changeOut__11TSmallEnemyFv();
void changeOut__7TKillerFv();
void clipActors__12TLiveManagerFPQ26JDrama9TGraphics();
void clipEnemies__13TEnemyManagerFPQ26JDrama9TGraphics();
void control__10TLiveActorFv();
void createAnmData__11TObjManagerFv();
void createEnemies__18TSmallEnemyManagerFi();
void createEnemyInstance__14TKillerManagerFv();
void createModelDataArray__11TObjManagerFPC19TModelDataLoadEntry();
void createModelData__14TKillerManagerFv();
void createSpcBinary__12TLiveManagerFv();
void decHpByWater__11TSmallEnemyFP9THitActor();
void doKeepDistance__11TSmallEnemyFv();
void drawObject__10TLiveActorFPQ26JDrama9TGraphics();
void endHitWaterJump__11TSmallEnemyFv();
void ensureTakeSituation__10TTakeActorFv();
void flyBehavior__7TKillerFv();
void flyBehavior__9TFlyEnemyFv();
void forceKill__11TSmallEnemyFv();
void forceKill__7TKillerFv();
void genEventCoin__11TSmallEnemyFv();
void genEventCoin__7TKillerFv();
void genRandomItem__11TSmallEnemyFv();
void generateEffectColumWater__11TSmallEnemyFv();
void generateItem__11TSmallEnemyFv();
void getBasNameTable__10TLiveActorCFv();
void getBasNameTable__7TKillerCFv();
void getChangeBlockTime__11TSmallEnemyFv();
void getFocalPoint__10TLiveActorCFv();
void getGravityY__9TFlyEnemyCFv();
void getPhaseShift__11TSpineEnemyCFv();
void getRadiusAtY__10TTakeActorCFf();
void getRootJointMtx__10TLiveActorCFv();
void getSaveParam__11TSpineEnemyCFv();
void getShadowType__10TLiveActorFv();
void getTakingMtx__10TLiveActorFv();
void getType__Q26JDrama6TActorCFv();
void getType__Q26JDrama8TNameRefCFv();
void hasMapCollision__10TLiveActorCFv();
void hasMapCollision__12TLiveManagerCFv();
void initAttacker__12TWalkerEnemyFP9THitActor();
void initSetEnemies__18TSmallEnemyManagerFv();
void init__7TKillerFP12TLiveManager();
void init__9TFlyEnemyFP12TLiveManager();
void initialGraphNode__12TWalkerEnemyFv();
void isCollidMove__11TSmallEnemyFP9THitActor();
void isCollidMove__7TKillerFP9THitActor();
void isEatenByYosshi__11TSmallEnemyFv();
void isFindMario__11TSmallEnemyFf();
void isFindMario__7TKillerFf();
void isHitValid__11TSmallEnemyFUl();
void isHitValid__7TKillerFUl();
void isInhibitedForceMove__11TSmallEnemyFv();
void isReachedToGoal__11TSpineEnemyCFv();
void isResignationAttack__12TWalkerEnemyFv();
void jumpBehavior__11TSmallEnemyFv();
void kill__11TSmallEnemyFv();
void loadAfter__11TSmallEnemyFv();
void loadAfter__18TSmallEnemyManagerFv();
void load__11TSmallEnemyFR20JSUMemoryInputStream();
void load__14TKillerManagerFR20JSUMemoryInputStream();
void moveObject__12TWalkerEnemyFv();
void performOnlyDraw__10TLiveActorFUlPQ26JDrama9TGraphics();
void perform__11TSmallEnemyFUlPQ26JDrama9TGraphics();
void perform__13TEnemyManagerFUlPQ26JDrama9TGraphics();
void receiveMessage__11TSmallEnemyFP9THitActorUl();
void requestShadow__10TLiveActorFv();
void reset__7TKillerFv();
void reset__9TFlyEnemyFv();
void restoreDrawBuffer__13TEnemyManagerFUl();
void save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream();
void scalingChangeActor__11TSmallEnemyFv();
void searchF__11TObjManagerFUsPCc();
void searchF__Q26JDrama8TNameRefFUsPCc();
void sendAttackMsgToMario__11TSmallEnemyFv();
void setAfterDeadEffect__9TFlyEnemyFv();
void setBckAnm__11TSmallEnemyFi();
void setBehavior__11TSmallEnemyFv();
void setChaseFlyAnm__7TKillerFv();
void setChaseFlyAnm__9TFlyEnemyFv();
void setDeadAnm__11TSmallEnemyFv();
void setDeadAnm__7TKillerFv();
void setDeadEffect__11TSmallEnemyFv();
void setFlagOutOfCube__12TLiveManagerFv();
void setFreezeAnm__11TSmallEnemyFv();
void setGenerateAnm__11TSmallEnemyFv();
void setGroundCollision__10TLiveActorFv();
void setMActorAndKeeper__11TSmallEnemyFv();
void setMActorAndKeeper__7TKillerFv();
void setMeltAnm__11TSmallEnemyFv();
void setNormalFlyAnm__7TKillerFv();
void setNormalFlyAnm__9TFlyEnemyFv();
void setRunAnm__11TSmallEnemyFv();
void setWaitAnm__11TSmallEnemyFv();
void setWalkAnm__11TSmallEnemyFv();
void updateAnmSound__11TSmallEnemyFv();
void walkBehavior__12TWalkerEnemyFif();
}

void* __vt__21TNerveKillerExplosion[] = {
	0,
	0,
	(void*)__dt__21TNerveKillerExplosionFv,
	0,
};

static void* killer_vtable_padding[16] = { 0 };

void* __vt__7TKiller[] = {
	0,
	0,
	(void*)__dt__7TKillerFv,
	(void*)getType__Q26JDrama6TActorCFv,
	(void*)load__11TSmallEnemyFR20JSUMemoryInputStream,
	(void*)save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream,
	(void*)loadAfter__11TSmallEnemyFv,
	(void*)searchF__Q26JDrama8TNameRefFUsPCc,
	(void*)perform__11TSmallEnemyFUlPQ26JDrama9TGraphics,
	0,
	0,
	0,
	(void*)JSGFGetType__Q26JStage6TActorCFv,
	(void*)JSGGetName__Q26JStage7TObjectCFv,
	(void*)JSGGetFlag__Q26JStage7TObjectCFv,
	(void*)JSGSetFlag__Q26JStage7TObjectFUl,
	(void*)JSGGetData__Q26JStage7TObjectCFUlPvUl,
	(void*)JSGSetData__Q26JStage7TObjectFUlPCvUl,
	(void*)JSGGetParent__Q26JStage7TObjectCFPPQ26JStage7TObjectPUl,
	(void*)JSGSetParent__Q26JStage7TObjectFPQ26JStage7TObjectUl,
	(void*)JSGSetRelation__Q26JStage7TObjectFbPQ26JStage7TObjectUl,
	0,
	0,
	0,
	0,
	0,
	0,
	(void*)JSGGetShape__Q26JStage6TActorCFv,
	(void*)JSGSetShape__Q26JStage6TActorFUl,
	(void*)JSGGetAnimation__Q26JStage6TActorCFv,
	(void*)JSGSetAnimation__Q26JStage6TActorFUl,
	(void*)JSGGetAnimationFrame__Q26JStage6TActorCFv,
	(void*)JSGSetAnimationFrame__Q26JStage6TActorFf,
	(void*)JSGGetAnimationFrameMax__Q26JStage6TActorCFv,
	(void*)JSGGetTranslation__Q26JDrama6TActorCFP3Vec,
	(void*)JSGSetTranslation__Q26JDrama6TActorFRC3Vec,
	(void*)JSGGetScaling__Q26JDrama6TActorCFP3Vec,
	(void*)JSGSetScaling__Q26JDrama6TActorFRC3Vec,
	(void*)JSGGetRotation__Q26JDrama6TActorCFP3Vec,
	(void*)JSGSetRotation__Q26JDrama6TActorFRC3Vec,
	(void*)receiveMessage__11TSmallEnemyFP9THitActorUl,
	(void*)getTakingMtx__10TLiveActorFv,
	(void*)ensureTakeSituation__10TTakeActorFv,
	0,
	(void*)getRadiusAtY__10TTakeActorCFf,
	(void*)belongToGround__10TLiveActorCFv,
	(void*)getRootJointMtx__10TLiveActorCFv,
	(void*)init__7TKillerFP12TLiveManager,
	(void*)calcRootMatrix__7TKillerFv,
	(void*)setGroundCollision__10TLiveActorFv,
	(void*)control__10TLiveActorFv,
	(void*)bind__7TKillerFv,
	(void*)moveObject__12TWalkerEnemyFv,
	(void*)requestShadow__10TLiveActorFv,
	(void*)drawObject__10TLiveActorFPQ26JDrama9TGraphics,
	(void*)performOnlyDraw__10TLiveActorFUlPQ26JDrama9TGraphics,
	(void*)getShadowType__10TLiveActorFv,
	(void*)kill__11TSmallEnemyFv,
	(void*)getGravityY__9TFlyEnemyCFv,
	(void*)hasMapCollision__10TLiveActorCFv,
	(void*)getFocalPoint__10TLiveActorCFv,
	(void*)updateAnmSound__11TSmallEnemyFv,
	(void*)getBasNameTable__7TKillerCFv,
	(void*)reset__7TKillerFv,
	0,
	0,
	(void*)getSaveParam__11TSpineEnemyCFv,
	(void*)getPhaseShift__11TSpineEnemyCFv,
	(void*)isReachedToGoal__11TSpineEnemyCFv,
	(void*)genRandomItem__11TSmallEnemyFv,
	(void*)genEventCoin__7TKillerFv,
	(void*)generateItem__11TSmallEnemyFv,
	(void*)isEatenByYosshi__11TSmallEnemyFv,
	(void*)setBehavior__11TSmallEnemyFv,
	(void*)jumpBehavior__11TSmallEnemyFv,
	(void*)behaveToWater__7TKillerFP9THitActor,
	(void*)changeByJuice__11TSmallEnemyFv,
	(void*)changeMove__11TSmallEnemyFv,
	(void*)getChangeBlockTime__11TSmallEnemyFv,
	(void*)scalingChangeActor__11TSmallEnemyFv,
	(void*)changeOut__7TKillerFv,
	(void*)behaveToTaken__11TSmallEnemyFP9THitActor,
	(void*)behaveToRelease__11TSmallEnemyFv,
	(void*)setGenerateAnm__11TSmallEnemyFv,
	(void*)setWalkAnm__11TSmallEnemyFv,
	(void*)setDeadAnm__7TKillerFv,
	(void*)setFreezeAnm__11TSmallEnemyFv,
	(void*)setMeltAnm__11TSmallEnemyFv,
	(void*)setWaitAnm__11TSmallEnemyFv,
	(void*)setRunAnm__11TSmallEnemyFv,
	(void*)attackToMario__7TKillerFv,
	(void*)forceKill__7TKillerFv,
	(void*)setMActorAndKeeper__7TKillerFv,
	(void*)initAttacker__12TWalkerEnemyFP9THitActor,
	(void*)isHitValid__7TKillerFUl,
	(void*)isCollidMove__7TKillerFP9THitActor,
	(void*)isInhibitedForceMove__11TSmallEnemyFv,
	(void*)endHitWaterJump__11TSmallEnemyFv,
	(void*)sendAttackMsgToMario__11TSmallEnemyFv,
	(void*)decHpByWater__11TSmallEnemyFP9THitActor,
	(void*)setBckAnm__11TSmallEnemyFi,
	(void*)setDeadEffect__11TSmallEnemyFv,
	(void*)setAfterDeadEffect__9TFlyEnemyFv,
	(void*)doKeepDistance__11TSmallEnemyFv,
	(void*)generateEffectColumWater__11TSmallEnemyFv,
	(void*)isFindMario__7TKillerFf,
	(void*)behaveToFindMario__12TWalkerEnemyFv,
	(void*)walkBehavior__12TWalkerEnemyFif,
	(void*)isResignationAttack__12TWalkerEnemyFv,
	(void*)initialGraphNode__12TWalkerEnemyFv,
	(void*)flyBehavior__7TKillerFv,
	(void*)setChaseFlyAnm__7TKillerFv,
	(void*)setNormalFlyAnm__7TKillerFv,
};

void* __vt__14TKillerManager[] = {
	0,
	0,
	(void*)__dt__14TKillerManagerFv,
	(void*)getType__Q26JDrama8TNameRefCFv,
	(void*)load__14TKillerManagerFR20JSUMemoryInputStream,
	(void*)save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream,
	(void*)loadAfter__18TSmallEnemyManagerFv,
	(void*)searchF__11TObjManagerFUsPCc,
	(void*)perform__13TEnemyManagerFUlPQ26JDrama9TGraphics,
	(void*)createModelData__14TKillerManagerFv,
	(void*)createAnmData__11TObjManagerFv,
	(void*)createModelDataArray__11TObjManagerFPC19TModelDataLoadEntry,
	(void*)clipActors__12TLiveManagerFPQ26JDrama9TGraphics,
	(void*)setFlagOutOfCube__12TLiveManagerFv,
	(void*)createSpcBinary__12TLiveManagerFv,
	(void*)hasMapCollision__12TLiveManagerCFv,
	(void*)createEnemyInstance__14TKillerManagerFv,
	(void*)clipEnemies__13TEnemyManagerFPQ26JDrama9TGraphics,
	(void*)restoreDrawBuffer__13TEnemyManagerFUl,
	(void*)createEnemies__18TSmallEnemyManagerFi,
	(void*)changeDrawBuffer__13TEnemyManagerFUl,
	(void*)initSetEnemies__18TSmallEnemyManagerFv,
};

void* __vt__22TNerveFlyEnemyChaseFly[] = {
	0,
	0,
	(void*)__dt__22TNerveFlyEnemyChaseFlyFv,
	0,
};

void* __vt__23TNerveFlyEnemyNormalFly[] = {
	0,
	0,
	(void*)__dt__23TNerveFlyEnemyNormalFlyFv,
	0,
};

void* __vt__9TFlyEnemy[] = {
	0,
	0,
	(void*)__dt__9TFlyEnemyFv,
	(void*)getType__Q26JDrama6TActorCFv,
	(void*)load__11TSmallEnemyFR20JSUMemoryInputStream,
	(void*)save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream,
	(void*)loadAfter__11TSmallEnemyFv,
	(void*)searchF__Q26JDrama8TNameRefFUsPCc,
	(void*)perform__11TSmallEnemyFUlPQ26JDrama9TGraphics,
	0,
	0,
	0,
	(void*)JSGFGetType__Q26JStage6TActorCFv,
	(void*)JSGGetName__Q26JStage7TObjectCFv,
	(void*)JSGGetFlag__Q26JStage7TObjectCFv,
	(void*)JSGSetFlag__Q26JStage7TObjectFUl,
	(void*)JSGGetData__Q26JStage7TObjectCFUlPvUl,
	(void*)JSGSetData__Q26JStage7TObjectFUlPCvUl,
	(void*)JSGGetParent__Q26JStage7TObjectCFPPQ26JStage7TObjectPUl,
	(void*)JSGSetParent__Q26JStage7TObjectFPQ26JStage7TObjectUl,
	(void*)JSGSetRelation__Q26JStage7TObjectFbPQ26JStage7TObjectUl,
	0,
	0,
	0,
	0,
	0,
	0,
	(void*)JSGGetShape__Q26JStage6TActorCFv,
	(void*)JSGSetShape__Q26JStage6TActorFUl,
	(void*)JSGGetAnimation__Q26JStage6TActorCFv,
	(void*)JSGSetAnimation__Q26JStage6TActorFUl,
	(void*)JSGGetAnimationFrame__Q26JStage6TActorCFv,
	(void*)JSGSetAnimationFrame__Q26JStage6TActorFf,
	(void*)JSGGetAnimationFrameMax__Q26JStage6TActorCFv,
	(void*)JSGGetTranslation__Q26JDrama6TActorCFP3Vec,
	(void*)JSGSetTranslation__Q26JDrama6TActorFRC3Vec,
	(void*)JSGGetScaling__Q26JDrama6TActorCFP3Vec,
	(void*)JSGSetScaling__Q26JDrama6TActorFRC3Vec,
	(void*)JSGGetRotation__Q26JDrama6TActorCFP3Vec,
	(void*)JSGSetRotation__Q26JDrama6TActorFRC3Vec,
	(void*)receiveMessage__11TSmallEnemyFP9THitActorUl,
	(void*)getTakingMtx__10TLiveActorFv,
	(void*)ensureTakeSituation__10TTakeActorFv,
	0,
	(void*)getRadiusAtY__10TTakeActorCFf,
	(void*)belongToGround__10TLiveActorCFv,
	(void*)getRootJointMtx__10TLiveActorCFv,
	(void*)init__9TFlyEnemyFP12TLiveManager,
	(void*)calcRootMatrix__11TSpineEnemyFv,
	(void*)setGroundCollision__10TLiveActorFv,
	(void*)control__10TLiveActorFv,
	(void*)bind__9TFlyEnemyFv,
	(void*)moveObject__12TWalkerEnemyFv,
	(void*)requestShadow__10TLiveActorFv,
	(void*)drawObject__10TLiveActorFPQ26JDrama9TGraphics,
	(void*)performOnlyDraw__10TLiveActorFUlPQ26JDrama9TGraphics,
	(void*)getShadowType__10TLiveActorFv,
	(void*)kill__11TSmallEnemyFv,
	(void*)getGravityY__9TFlyEnemyCFv,
	(void*)hasMapCollision__10TLiveActorCFv,
	(void*)getFocalPoint__10TLiveActorCFv,
	(void*)updateAnmSound__11TSmallEnemyFv,
	(void*)getBasNameTable__10TLiveActorCFv,
	(void*)reset__9TFlyEnemyFv,
	0,
	0,
	(void*)getSaveParam__11TSpineEnemyCFv,
	(void*)getPhaseShift__11TSpineEnemyCFv,
	(void*)isReachedToGoal__11TSpineEnemyCFv,
	(void*)genRandomItem__11TSmallEnemyFv,
	(void*)genEventCoin__11TSmallEnemyFv,
	(void*)generateItem__11TSmallEnemyFv,
	(void*)isEatenByYosshi__11TSmallEnemyFv,
	(void*)setBehavior__11TSmallEnemyFv,
	(void*)jumpBehavior__11TSmallEnemyFv,
	(void*)behaveToWater__11TSmallEnemyFP9THitActor,
	(void*)changeByJuice__11TSmallEnemyFv,
	(void*)changeMove__11TSmallEnemyFv,
	(void*)getChangeBlockTime__11TSmallEnemyFv,
	(void*)scalingChangeActor__11TSmallEnemyFv,
	(void*)changeOut__11TSmallEnemyFv,
	(void*)behaveToTaken__11TSmallEnemyFP9THitActor,
	(void*)behaveToRelease__11TSmallEnemyFv,
	(void*)setGenerateAnm__11TSmallEnemyFv,
	(void*)setWalkAnm__11TSmallEnemyFv,
	(void*)setDeadAnm__11TSmallEnemyFv,
	(void*)setFreezeAnm__11TSmallEnemyFv,
	(void*)setMeltAnm__11TSmallEnemyFv,
	(void*)setWaitAnm__11TSmallEnemyFv,
	(void*)setRunAnm__11TSmallEnemyFv,
	(void*)attackToMario__12TWalkerEnemyFv,
	(void*)forceKill__11TSmallEnemyFv,
	(void*)setMActorAndKeeper__11TSmallEnemyFv,
	(void*)initAttacker__12TWalkerEnemyFP9THitActor,
	(void*)isHitValid__11TSmallEnemyFUl,
	(void*)isCollidMove__11TSmallEnemyFP9THitActor,
	(void*)isInhibitedForceMove__11TSmallEnemyFv,
	(void*)endHitWaterJump__11TSmallEnemyFv,
	(void*)sendAttackMsgToMario__11TSmallEnemyFv,
	(void*)decHpByWater__11TSmallEnemyFP9THitActor,
	(void*)setBckAnm__11TSmallEnemyFi,
	(void*)setDeadEffect__11TSmallEnemyFv,
	(void*)setAfterDeadEffect__9TFlyEnemyFv,
	(void*)doKeepDistance__11TSmallEnemyFv,
	(void*)generateEffectColumWater__11TSmallEnemyFv,
	(void*)isFindMario__11TSmallEnemyFf,
	(void*)behaveToFindMario__12TWalkerEnemyFv,
	(void*)walkBehavior__12TWalkerEnemyFif,
	(void*)isResignationAttack__12TWalkerEnemyFv,
	(void*)initialGraphNode__12TWalkerEnemyFv,
	(void*)flyBehavior__9TFlyEnemyFv,
	(void*)setChaseFlyAnm__9TFlyEnemyFv,
	(void*)setNormalFlyAnm__9TFlyEnemyFv,
};
