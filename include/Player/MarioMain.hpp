#ifndef MARIOMAIN_HPP
#define MARIOMAIN_HPP

#include <JSystem/JGeometry.hpp>
#include <Player/MarioInit.hpp>
#include <System/DrawSyncCallback.hpp>
#include <Strategic/TakeActor.hpp>

class TLiveActor;
class TBGCheckData;
class J3DAnmTexPattern;
class J3DModelData;
class J3DAnmTransform;
struct TBGWallCheckRecord;

// TODO: where should this be?
enum E_SIDEWALK_TYPE {};

struct TRidingInfo {
	const TLiveActor* unk0;
	Vec localPos;
	f32 unk10;
	// maybe more
};

class TMarioGamePad;

// TODO: Add fields
class TMario : public TTakeActor, public TDrawSyncCallback {
public:
	TMario();

	virtual ~TMario() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual u32 receiveMessage(THitActor*, u32);
	virtual MtxPtr getTakingMtx();
	virtual void moveRequest(const JGeometry::TVec3<f32>&);

	virtual void drawSyncCallback(u16);
	virtual void initValues();
	virtual void checkReturn();
	virtual void checkController(JDrama::TGraphics*);
	virtual void playerControl(JDrama::TGraphics*);
	virtual void initModel();
	virtual void drawSpecial(JDrama::TGraphics*);
	virtual void checkCollision();
	virtual void damageExec(THitActor*, int, int, int, f32, int, f32, s16);
	virtual void getVoiceStatus();

	struct TEParams;
	struct JumpSlipRecord;

	void actnMain();
	void pitching();
	void putting();
	void catchLost();
	void takePose();
	void taking();
	void demoMain();
	void disappear();
	void nomotion();
	void footDowning();
	void electricDamage();
	void warpOut();
	void toroccoStart();
	void waitingStart(const JGeometry::TVec3<f32>*, f32);
	void returnStart(const JGeometry::TVec3<f32>*, f32, bool, int);
	void rollingStart(const JGeometry::TVec3<f32>*, f32);
	void startCommon(const JGeometry::TVec3<f32>*, f32);
	void isUnUsualStageStart();
	void warpIn();
	void downLoser();
	void sinkLoser();
	void openDoor();
	void jumpingDemoCommon(u32, int, f32);
	void elecDowning();
	void bottleIn();
	void readBillboard();
	void winDemo();
	void considerTake();
	void calcDamagePos(const JGeometry::TVec3<f32>&);
	void floorDamageExec(const TMario::TEParams&);
	void floorDamageExec(int, int, int, int);
	void loserExec();
	void normalizeNozzle();
	void resetNozzle();
	void trampleExec(THitActor*);
	void canTake(THitActor*);
	void isTakeSituation(THitActor*);
	void dropObject();
	void getAttackAngle(const THitActor*);
	void decHP(int);
	void incHP(int);
	void rumbleStart(int, int);
	void thinkDashEffect();
	void addDamageFog(JDrama::TGraphics*);
	void addDirty();
	void boxDrawPrepare(f32 (*)[4]);
	void drawLogic();
	void entryModels(JDrama::TGraphics*);
	void calcView(JDrama::TGraphics*);
	void calcAnim(u32, JDrama::TGraphics*);
	void removeCallBack();
	void addUpper();
	void setUpperDamageRun();
	void addCallBack(JDrama::TGraphics*);
	void calcAnimHands();
	void calcAnimBody(u32, JDrama::TGraphics*);
	void calcBaseMtx(f32 (*)[4]);
	void calcBaseMtxSwim(f32 (*)[4]);
	void calcBaseMtxPole(f32 (*)[4]);
	void calcBaseMtxTorocco(f32 (*)[4]);
	void considerWaist();
	void isUpperPumpingStyle() const;
	void finalDrawInitialize();
	void initMirrorModel();
	void loadAnmTexPattern(J3DAnmTexPattern**, char*, J3DModelData*);
	void loadBas(void**, const char*);
	void loadAnm(J3DAnmTransform**, const char*);
	void setReverseAnimation(int, f32);
	void setAnimation(int, f32);
	void changeHandByRate(f32);
	void changeHand(int);
	void isAnimeLoopOrStop();
	void isLast1AnimeFrame();
	void getMotionFrameCtrl();
	void getCurrentFrame(int);
	void getRailMtx() const;
	void getTakenMtx();
	void calcBodyPos(JGeometry::TVec3<f32>*);
	void getTrampleCt();
	void setPositions();
	void takeOffGlass();
	void wearGlass();
	bool isWearingHelm();
	bool isWearingCap();
	void setDivHelm();
	void getWallAngle() const;
	void getPumpFrame() const;
	void getCenterAnmMtx();
	void getRootAnmMtx();
	void getHeadRot();
	void getJumpIntoWaterModelData();
	void jumpMain();
	void fallDead();
	void diving();
	void hipAttacking();
	void pullJumping();
	void wireJumping();
	void rotateJumping();
	void rocketing();
	void rocketCheck();
	void boardJumping();
	void rotateBroadJumping();
	void broadJumping();
	void fireLanding();
	void fireJumping();
	void missJumping();
	void trample();
	void thrownDowning();
	void fireDowning();
	void slipFalling();
	void catchStop();
	void stayWall();
	void landSafeDown();
	void jumpForeDown();
	void jumpBackDown();
	void jumpShortForeDown();
	void jumpShortBackDown();
	void checkWallJumping();
	void jumpDownCommon(int, int, f32);
	void jumpingThrow();
	void jumpCatch();
	void jumpWall();
	void uTurnJumping();
	void landing();
	void backJumping();
	void ultraJumping();
	void secJumping();
	void jumping();
	void jumpingCommonEvents();
	void checkBackTrig();
	void considerJumpRotate();
	void jumpingBasic(int, int, int);
	void askStrongGroundTouch();
	void doJumping();
	void setJumpingAttackArea();
	void doSpinJumping();
	void doSlipJumping();
	void checkJumpingThrowStart();
	void startJumpWall();
	void thinkAloha();
	void thinkCube();
	void thinkFreeze();
	void isMario();
	void gunExec();
	void checkWet();
	void thinkSound();
	void thinkTorocco();
	void thinkDiving();
	void thinkYoshiHeadCollision();
	void checkYoshiGetOff();
	void getOffYoshi(bool);
	void thinkParams();
	void thinkSand();
	void thinkWaterSurface();
	void thinkSituation();
	void calcGroundMtx(const JGeometry::TVec3<f32>&);
	void view1stMove();
	void talkMove();
	void canReadBillboard(int);
	void stateMachine();
	void checkPlayerAction(JDrama::TGraphics*);
	void checkRideReCalc();
	void checkRideMovement();
	void getActorMtx(const THitActor&, f32 (*)[4]);
	void checkCurrentPlane();
	void getDmgMapCode(int) const;
	void checkGroundPlane(f32, f32, f32, f32*, const TBGCheckData**);
	void makeHistory();
	void checkStickSmash();
	void checkStickRotate(int*);
	void getLRLevel(unsigned char);
	void getDizzyPower();
	void getDizzyAngle();
	void checkThrowObject();
	void doReturn();
	void checkEnforceJump();
	void checkSink();
	void thinkHeight();
	void thinkDirty();
	void dirtyLimitCheck();
	void checkGraffito();
	void checkGraffitoElec();
	void checkGraffitoSlip();
	void checkGraffitoLava();
	void checkGraffitoFire();
	void checkGraffitoDamage();
	void makeGraffitoDamage(const TMario::TEParams&);
	void checkAllMotions();
	void changePlayerDropping(u32, u32);
	void changePlayerJumping(u32, u32);
	void changePlayerTriJump();
	void changePlayerStatus(u32, u32, bool);
	void throwMario(const JGeometry::TVec3<f32>&, f32);
	void setStatusToRunning(u32, u32);
	void setStatusToJumping(u32, u32);
	void setPlayerJumpSpeed(f32, f32);
	void setMissJumping();
	void isTurnning();
	void isTurnStart();
	void checkPlayerAround(int, f32);
	void isJumpMiss();
	void isSlipLimit();
	void getSlideStopCatch();
	void getSlideStopNormal();
	void canSlipJump();
	void isSlipStart();
	void isFrontSlip(int);
	void checkRoofPlane(const Vec&, f32, const TBGCheckData**);
	void checkWallPlane(Vec*, f32, f32);
	void setPlayerVelocity(f32);
	void setNormalAttackArea();
	void changePos(const Vec&);
	void isSpeedZero();
	void canBendBody();
	void considerRotateJumpStart();
	void addVelocity(f32);
	u32 onYoshi() const;
	void getGroundJumpPower() const;
	void windMove(const JGeometry::TVec3<f32>&);
	void flowMove(const JGeometry::TVec3<f32>&);
	void warpRequest(const JGeometry::TVec3<f32>&, f32);
	void isForceSlip();
	void getRidingMtx(f32 (*)[4]);
	void isWallInFront() const;
	void isInvincible() const;
	void isUnderWater() const;
	void canSquat() const;
	void getJumpSlideControl() const;
	void getJumpAccelControl() const;
	void jumpProcess(int);
	void fallProcess();
	void isFallCancel();
	void checkGroundAtJumping(const Vec&, int);
	void hangonCheck(const TBGCheckData*, const Vec&, const Vec&);
	void barProcess();
	void walkProcess();
	void waitProcess();
	void stopProcess();
	void checkGroundAtWalking(Vec*);
	void checkDescent();
	void keepDistance(const THitActor&, f32);
	void keepDistance(const JGeometry::TVec3<f32>&, f32, f32);
	void playerRefrection(int);
	void moveMain();
	void broadJumpSlip();
	void ultraJumpSlip();
	void uTurnJumpSlip();
	void secJumpSlip();
	void landSlip();
	void jumpSlip();
	void jumpSlipEvents(TMario::JumpSlipRecord*);
	void jumpSlipCommon(short, u32);
	void loserDown();
	void catchDown();
	void safeForeDown();
	void safeBackDown();
	void shortForeDown();
	void shortBackDown();
	void foreDown();
	void backDown();
	void downingCommon(int, f32, int);
	void oilSlope();
	void oilSlip();
	void oilRun();
	void squatSlipping();
	void catching();
	void slipBack();
	void slipBackCommon(int, int, int);
	void slipFore();
	void slipForeCommon(int, int, int, int);
	void slippingBasic(int, int, int);
	void fireDashing();
	void speedSliding();
	void walkEnd();
	void toroccoing();
	void surfing();
	void braking();
	void turnEnd();
	void turnning();
	void rotating();
	void running();
	void doPushingAnimation(const Vec&);
	void changePlayerWaiting();
	void doBraking(f32);
	void doSurfing();
	void getSurfingParamsGround();
	void getSurfingParamsWater();
	void doRunning();
	void doStopping();
	void doSlipping(f32);
	void slopeProcess();
	void doSliding(f32);
	void slideProcess(f32, f32);
	void getSlideStickMult();
	void getChangeAngleSpeed();
	void getSlopeSlideAccele(f32*, f32*);
	void getSlopeNormalAccele(f32*, f32*);
	void doRunningAnimation();
	void getRunningInWaterBrake();
	void isRunningInWater();
	void changePlayerCatching();
	void isRunningTurnning();
	void isRunningSlipStart();
	void changePlayerPower(f32, u32, u32);
	void clashStandard(u32, u32);
	void postureControl();
	void isThrowStart();
	void considerRotateStart();
	void specMain();
	void fencePunch();
	void fenceMove();
	void fenceJumpCatch();
	void fenceCatch();
	void fenceFootCheck();
	void pulling();
	void setPullingAnm(const JGeometry::TVec3<f32>&, f32);
	void getCurrentPullParams(f32*, f32*);
	void wireSWaitToWaitR();
	void wireSWaitToWaitL();
	void wireRolling();
	void getNozzleEmitVX();
	void wireHanging();
	void wireReturn();
	void wireSWaitToHang();
	void wireWaitToHang();
	void changeWireHanging();
	void wireWaitToSWaitR();
	void wireWaitToSWaitL();
	void wireSWait();
	void wireWait();
	void wireMove(f32);
	void getOnWirePosAngle(JGeometry::TVec3<f32>*, short*);
	void taken();
	void hangJumping();
	void descend();
	void ascend();
	void hanging();
	void findNearestWall(const TBGWallCheckRecord&);
	void hangingCommon(int, int);
	void startHangLanding(u32);
	void kickRoofRollDown();
	void kickRoofRollUp();
	void kickRoof();
	void moveRoof();
	void waitRoof();
	void hangRoof();
	void roofCommonEvents();
	void doRoofWaitingProcess();
	void doRoofMovingProcess();
	void hangingCheckRoof(JGeometry::TVec3<f32>*);
	void barHang();
	void barClimb();
	void barWait();
	void barJumpSetting();
	void stateMachineUpper();
	void checkPumpEnable();
	void checkPumping();
	u32 askJumpIntoWaterEffectExist() const;
	void sinkInSandEffect();
	void kickFruitEffect();
	void toroccoEffect();
	void sleepingEffectKill();
	void sleepingEffect();
	void kickRoofEffect();
	void elecEndEffect();
	void elecEffect();
	void rocketEffectStart();
	void rocketEffectNozzle();
	void meltInWaterEffect();
	void emitSandEffect();
	void emitDirtyFootPrint();
	void emitFootPrint(int);
	void emitFootPrintWithEffect(int, int);
	void setFootPrint(const JGeometry::TVec3<f32>&, int);
	void emitRotateShootEffect();
	void emitBlurSpinJump();
	void emitBlurHipDropSuper();
	void emitBlurHipDrop();
	void warpOutEffect(int, f32);
	void warpInLight();
	void warpInEffect();
	void surfingEffect();
	void frontSlipEffect();
	void treeSlipEffect();
	void wallSlipEffect();
	void blurEffect();
	void runningRippleEffect();
	void smallRippleEffect(JGeometry::TVec3<f32>*);
	void swimmingBubbleEffect();
	void bubbleFromBody();
	void bubbleFromMouth(int);
	void inOutWaterEffect(f32);
	void rippleEffectSmall();
	void rippleEffect();
	void smallTouchDownEffect();
	void strongTouchDownEffect();
	void strongTouchDownEffectDisp();
	void emitGetCoinEffect(JGeometry::TVec3<f32>*);
	void emitGetWaterEffect();
	void emitGetEffect();
	void emitRecover();
	void emitSweatSometimes(short);
	void emitSweatSometimes();
	void emitSweat(short);
	void emitSmoke(short);
	void emitParticle(int, short);
	void emitParticle(int, const JGeometry::TVec3<f32>*);
	void emitParticle(int);
	void moveParticle();
	void initParticle();
	void waitMain();
	void slipEnd();
	void brakeEnd();
	void hipAttackEnd();
	void broadJumpEnd();
	void fireJumpEnd();
	void jumpThrowEnd();
	void uTurnJumpEnd();
	void ultraJumpEnd();
	void landEnd();
	void secJumpEnd();
	void jumpEnd();
	void jumpEndEvents(u32);
	void jumpEndCommon(int, int);
	void pullEnd();
	void squatStandup();
	void squatStart();
	void squating();
	void getSideWalkValues(E_SIDEWALK_TYPE*, f32*, f32*);
	void wakeup();
	void sleeping();
	void sleepily();
	void waiting();
	void changeMontemanWaitingAnim();
	void stopCommon(int, int);
	void waitingCommonEvents();
	void checkPutStart();
	void canPut();
	void canSleep();
	void startTalking();
	void swimMain();
	void swimPDown();
	void swimDown();
	void swimPDamage();
	void swimDive();
	void swimUp();
	void swimPaddleEndToWait();
	void swimPaddleEnd();
	void swimPaddle();
	void swimPaddleStart();
	void swimWaitToPaddle();
	void swimWait();
	void swimStart();
	void checkSwimToHangFence();
	void checkSwimJump();
	void doSwimming();
	void isSwimWaiting();
	void setGamePad(TMarioGamePad*);
	void stageSetting();
	void resetHistory();
	void hitNoKeepPull(THitActor*);
	void hitSurfingBoard(THitActor*);
	void hitPickUpEnemy(THitActor*);
	void hangPole(THitActor*);
	void hitBrakable(THitActor*);
	void hitJumpBase(THitActor*);
	void hitBarrel(THitActor*);
	void hitWantToTake(THitActor*);
	void wantToTakeActor(THitActor*);
	void hitPool(THitActor*);
	void hitNpc(THitActor*);
	void hitMario(THitActor*);
	void hitPull(THitActor*);
	void hitPushup(THitActor*);
	void hitHipDrop(THitActor*);
	void hitNormal(THitActor*);
	void getCoinBlue();
	void getCoinRed();
	void getCoin();
	void getGesso(THitActor*);
	// TODO: TWaterGun
	// void getNozzle(THitActor*, TWaterGun::TNozzleType);
	void startSoundActor(u32);
	void stopVoice();
	void startVoiceIfNoVoice(u32);
	void startVoice(u32);
	void soundTorocco();
	void soundHitBound();
	void animSound();
	void soundMovement();
	void startVoiceYoshi(u32);
	void checkStatusType(long) const;

public:
	u32 _070;
	u32 input;
	u32 _078;
	u32 action;
	u32 prevAction;
	s16 actionState;
	s16 actionTimer;
	u32 actionArg;
	f32 intendedMag;
	s16 intendedYaw;
	u16 _092;

	JGeometry::TVec3<s16> faceAngle; // 0x94
	u16 modelFaceAngle;
	u32 _09C;
	u32 _0A0;
	JGeometry::TVec3<f32> vel; // 0xA4

	f32 forwardVel;
	f32 slideVelX;
	f32 slideVelZ;

	char _0BC[0x1C];

	// TODO: TBGCheckData
	TBGCheckData* mWallPlane;   // TBGCheckData 0xD8
	TBGCheckData* mRoofPlane;   // TBGCheckData 0xDC
	TBGCheckData* mGroundPlane; // TBGCheckData 0xE0
	TBGCheckData* waterFloor;   // TBGCheckData 0xE4

	JGeometry::TVec3<f32> floorPosition; // 0xE8

	s16 slopeAngle;
	u16 _0F6;

	u16 lightID; // 0xF8
	// u16 _0FA;

	u32 _0FC[2];

	void* controller; // TMarioControllerWork

	u32 _108[4];

	u32 _118; // gpMarioFlag points here;

	u32 _11C;

	s16 health; // 0x0120

	u16 _122;
	char _124[0x264];

	u16 blooperColor; // TODO: Make enum (0 = red, 1 = yellow, 2 = green)

	char _38A[0x5A];

	void* waterGun; // TWaterGun 0x3E4

	u32 _3E8;
	u32 _3EC;

	void* yoshi; // TYoshi 0x3F0

	char _3F4[0x0FC];

	JGeometry::TVec3<f32> _4F0;

	void* gamePad; // TMarioGamePad

	char _500[0x74];

	// start of TDeParams at 0x574
	TDeParams deParams;

	// TODO: Should these be an array indexed by an enum?
	TBodyAngleParams bodyAngleParamsFree;
	TBodyAngleParams bodyAngleParamsWaterGun;

	TAttackParams attackParamsFencePunch;
	TAttackParams attackParamsKickRoof;
	TJumpParams jumpParams;
	TRunParams runParams;
	TSwimParams swimParams;
	THangingParams hangingParams;
	THangRoofParams hangRoofParams;
	TWireParams wireParams;

	// TODO: Should these be an array indexed by an enum?
	TPullParams pullParamsBGBeak;
	TPullParams pullParamsBGTentacle;
	TPullParams pullParamsBGFireWanWanBossTail;
	TPullParams pullParamsFireWanWanTail;

	TBarParams barParams;

	TSurfingParams surfingParamsWaterRed;
	TSurfingParams surfingParamsGroundRed;
	TSurfingParams surfingParamsWaterYellow;
	TSurfingParams surfingParamsGroundYellow;
	TSurfingParams surfingParamsWaterGreen;
	TSurfingParams surfingParamsGroundGreen;

	THoverParams hoverParams;
	TDivingParams divingParams;
	TYoshiParams yoshiParams;
	TWaterEffectParams waterEffectParams;
	TControllerParams controllerParams;
	TGraffitoParams graffitoParams;
	TDirtyParams dirtyParams;
	TMarioMotorParams marioMotorParams;
	TMarioParticleParams marioParticleParams;
	TMarioEffectParams marioEffectParams;

	// TODO: Should these be an array indexed by an enum?
	TSlipParams slipParamsNormal;
	TSlipParams slipParamsOil;
	TSlipParams slipParamsAll;
	TSlipParams slipParamsAllSlider;
	TSlipParams slipParams45;
	TSlipParams slipParamsWaterSlope;
	TSlipParams slipParamsWaterGround;
	TSlipParams slipParamsYoshi;

	TUpperBodyParams upperBodyParams;

	// TODO: Should these be an array indexed by an enum?
	TDmgParams dmgParamsEnemyCommon;
	TDmgParams dmgParamsHamakuri;
	TDmgParams dmgParamsNamekuri;
	TDmgParams dmgParamsHinokuri;
	TDmgParams dmgParamsFire;
	TDmgParams dmgParamsBGTentacle;
	TDmgParams dmgParamsBossEel;
	TDmgParams dmgParamsHanachanBoss;
	TDmgParams dmgParamsPoihana;
	TDmgParams dmgParamsKiller;
	TDmgParams dmgParamsLampTrapIron;
	TDmgParams dmgParamsLampTrapSpike;
	TDmgParams dmgParamsEnemyMario;
	TDmgParams dmgParamsCannotBreath;
	TDmgParams dmgParamsGraffitoFire;
	TDmgParams dmgParamsGraffitoPoison;
	TDmgParams dmgParamsGraffitoElec;
	TDmgParams dmgParamsGraffitoLava;
	TDmgParams dmgParamsWaterSurface;

	TDmgParams dmgMapParams0;
	TDmgParams dmgMapParams1;
	TDmgParams dmgMapParams2;
	TDmgParams dmgMapParams3;
	TDmgParams dmgMapParams4;
	TDmgParams dmgMapParams5;
	TDmgParams dmgMapParams6;
	TDmgParams dmgMapParams7;
	TDmgParams dmgMapParams8;
	TDmgParams dmgMapParams9;

	TAutoDemoParams autoDemoParams;
	TSoundParams soundParams;
	TOptionParams optionParams;

	char _4290[0x80];
};

extern TMario* gpMarioOriginal;

#endif
