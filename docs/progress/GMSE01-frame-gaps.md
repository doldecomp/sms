# Frame-gap worklist (GMSE01)

Generated batch 74. Every entry is a game function whose instructions match the
original exactly, differing only in frame size. Per the frame-gap mechanism in
`docs/MATCHING_CATALOG.md`, each needs that many bytes of **uninitialised**
locals declared in the original and never used on the traced path.

A positive number means our frame is too small and locals are missing.
A negative number means ours is too large and we declared locals the original
did not have, or initialised ones it left uninitialised.

Recover a plausibly-named local; do not insert filler. If the declaration
cannot be identified, leave the function and say so.

```
  +160 |  8900B | Player/MarioReceiveMsg :: receiveMessage__6TMarioFP9THitActorUl
  +464 |  6924B | Player/MarioSound :: soundMovement__6TMarioFv
   +88 |  4972B | Player/MarioCheckCol :: checkCollision__6TMarioFv
   +56 |  4384B | GC2D/SelectDir :: rsetup__10TSelectDirFv
   +40 |  4224B | Player/MarioJump :: jumpMain__6TMarioFv
   -24 |  3176B | NPC/NpcAnm :: npcWetting__8TBaseNPCFv
   -48 |  2604B | MarioUtil/ShadowUtil :: drawShadow__19TMBindShadowManagerFUlPQ26JDrama9TGrap
    +8 |  2280B | Player/ModelWaterManager :: drawWaterVolume__18TModelWaterManagerFPA4_f
    +8 |  2264B | Player/MarioWait :: waitMain__6TMarioFv
  +104 |  2148B | Player/MarioMain :: perform__6TMarioFUlPQ26JDrama9TGraphics
  +472 |  2116B | MoveBG/MapObjInit :: initUnique__11TMapObjBaseFv
    -8 |  1968B | Enemy/tamaNoko :: execute__20TNerveTamaNokoAttackCFP24TSpineBase<10TLi
   +32 |  1960B | Enemy/bossManta :: initNthGeneration__10TBossMantaFi
   +80 |  1888B | Enemy/gatekeeper :: init__17TBiancoGateKeeperFP12TLiveManager
  +112 |  1812B | NPC/NpcInitPrg :: init__8TBaseNPCFP12TLiveManager
   -16 |  1808B | Enemy/gesso :: execute__15TNerveGessoFallCFP24TSpineBase<10TLiveAct
   +40 |  1796B | NPC/NpcEffect :: emitParticle___8TBaseNPCFv
   +24 |  1784B | Player/MarioSwim :: swimMain__6TMarioFv
  +120 |  1712B | Enemy/bossManta :: drawMantaShadow__17TBossMantaManagerFPQ26JDrama9TGra
  +160 |  1668B | Player/MarioCap :: perform__9TMarioCapFUlPQ26JDrama9TGraphics
  +200 |  1652B | MoveBG/MapObjManager :: newAndRegisterObjByEventID__18TMapObjBaseManagerFUlP
   +88 |  1644B | Enemy/BossHanachanAnm :: changeAnmRateAndFrameUpdate___13TBossHanachanFv
  +608 |  1592B | Map/MapArea :: polygonIsInGrid__17TMapCollisionDataFffffP12TBGCheck
   +88 |  1560B | Enemy/enemyMario :: initModel__11TEnemyMarioFv
   +16 |  1560B | GC2D/CardSave :: waitForSelectOver__9TCardSaveFv
   +32 |  1472B | Enemy/enemyMario :: checkController__11TEnemyMarioFPQ26JDrama9TGraphics
   +32 |  1472B | Enemy/gatekeeper :: execute__13TNerveBGKWaitCFP24TSpineBase<10TLiveActor
   +72 |  1468B | Player/MarioJump :: hipAttacking__6TMarioFv
   +48 |  1460B | Enemy/bosseel :: execute__20TNerveBossEelOutWaitCFP24TSpineBase<10TLi
   -48 |  1416B | MarioUtil/DrawUtil :: init__19TTrembleModelEffectFP8J3DModel
   +56 |  1396B | Enemy/spider :: bind__7TSpiderFP10TLiveActor
   +16 |  1384B | GC2D/GCConsole2 :: startAppearLife__11TGCConsole2Fi
   +80 |  1376B | Enemy/hinokuri2 :: init__10THinokuri2FP12TLiveManager
  +104 |  1376B | Player/Yoshi :: doSearch__6TYoshiFv
   +24 |  1372B | Enemy/fireWanwan :: emitEffects__11TFireWanwanFv
    +8 |  1360B | Player/ModelWaterManager :: drawRefracAndSpec__18TModelWaterManagerCFv
   +32 |  1348B | GC2D/GCConsole2 :: drawJuice__11TGCConsole2FR13J2DOrthoGraphUl
   -48 |  1336B | Animal/AnimalNerve :: execute__23TNerveAnimalGraphWanderCFP24TSpineBase<10
   +88 |  1328B | Camera/lensglow :: perform__9TLensGlowFUlPQ26JDrama9TGraphics
   -16 |  1292B | GC2D/CardLoad :: drawMessage__9TCardLoadF10TEProgress
   +40 |  1288B | Enemy/bosseel :: execute__26TNerveBossEelMouthOpenWaitCFP24TSpineBase
    -8 |  1288B | NPC/NpcChange :: behaveToBeTrampled___8TBaseNPCFv
    +8 |  1272B | M3DUtil/MActor :: __ct__6MActorFP13MActorAnmData
   +16 |  1256B | Enemy/namekuri :: execute__24TNerveNameKuriJumpAttackCFP24TSpineBase<1
  +104 |  1248B | Enemy/hinokuri2 :: execute__22TNerveHino2GraphWanderCFP24TSpineBase<10T
    -8 |  1244B | M3DUtil/SampleCtrlNode :: __ct__18SampleCtrlMaterialFP11J3DMaterial
   -56 |  1220B | MarioUtil/DrawUtil :: movement__19TTrembleModelEffectFv
  +120 |  1216B | MoveBG/MapObjManager :: load__14TMapObjManagerFR20JSUMemoryInputStream
    +8 |  1184B | Enemy/hamukuri :: behaveToWater__14TDangoHamuKuriFP9THitActor
   +48 |  1168B | Enemy/bgtentacle :: calcAtkParticleAndSE__11TBGTentacleFv
    +8 |  1164B | Enemy/bossManta :: collidedWithWater__10TBossMantaFv
    -8 |  1148B | Enemy/graph :: getEscapeDirLimited__9TGraphWebCFiiRCQ29JGeometry8TV
   +96 |  1148B | M3DUtil/MActorData :: addFileTable__13MActorAnmDataFPCc
   +80 |  1136B | Enemy/conductor :: perform__10TConductorFUlPQ26JDrama9TGraphics
   +24 |  1136B | NPC/NpcChange :: behaveToHitObject___8TBaseNPCFP9THitActor20EnumHitNp
   +32 |  1128B | Camera/cameragc :: ctrlGameCamera___15CPolarSubCameraFv
  +104 |  1104B | System/Application :: proc__12TApplicationFv
   +40 |  1100B | Map/MapEventMare :: depressing__21TMareEventDepressWallFv
   +32 |  1080B | System/MenuDir :: setFixedStageValue__13TMenuDirectorFv
   +72 |  1076B | Enemy/hinokuri2 :: execute__17TNerveHino2DamageCFP24TSpineBase<10TLiveA
    +8 |  1076B | Enemy/smallEnemy :: reset__11TSmallEnemyFv
    +8 |  1068B | Enemy/Kumokun :: execute__16TNerveKumokunFlyCFP24TSpineBase<10TLiveAc
    +8 |  1068B | GC2D/PauseMenu2 :: disappearWindow__11TPauseMenu2Fv
   -24 |  1064B | Enemy/tamaNoko :: calcRootMatrix__9TTamaNokoFv
   +64 |  1044B | Enemy/fireWanwan :: movementBody__18TFireWanwanTailHitFRCQ29JGeometry8TV
   +96 |  1044B | GC2D/Option :: adjustView__16TOptionSoundUnitFv
   +24 |  1044B | MoveBG/MapObjSirena :: moveObject__16TCasinoPanelGateFv
   +48 |  1044B | Strategic/ObjHitCheck :: clearHitNum__12TObjHitCheckFv
   +24 |  1040B | Player/MarioRun :: doSurfing__6TMarioFv
    +8 |  1016B | MarioUtil/DrawUtil :: perform__11TSilhouetteFUlPQ26JDrama9TGraphics
    +8 |  1004B | Player/MarioRun :: turnning__6TMarioFv
   +16 |  1000B | Enemy/bgtentacle :: calcVelocity__Q211TBGTentacle5TNodeFP11TBGTentaclePQ
    -8 |  1000B | Map/BathWaterManager :: loadAfter__17TBathWaterManagerFv
   +48 |  1000B | Player/ModelWaterManager :: __ct__14TWaterEmitInfoFPCc
   +24 |   992B | Enemy/hamukuri :: behaveToWater__13TFireHamuKuriFP9THitActor
    +8 |   984B | Enemy/fireWanwan :: bindPoint__11TFireWanwanFPQ29JGeometry8TVec3<f>RCQ29
   +16 |   984B | MarioUtil/ShadowUtil :: __ct__16TMBindShadowBodyFP9THitActorP8J3DModelf
   +48 |   980B | Map/PollutionLayer :: cleaned__15TPollutionLayerFffff
   +16 |   972B | GC2D/GCConsole2 :: startAppearStar__11TGCConsole2Fv
   +40 |   972B | Map/PollutionAction :: action__15TPollutionLayerFv
   +16 |   968B | Player/MarioRun :: slideProcess__6TMarioFff
    -8 |   964B | MarioUtil/DrawUtil :: tremble__19TTrembleModelEffectFfffi
   +16 |   956B | Enemy/telesa :: load__18TMarioModokiTelesaFR20JSUMemoryInputStream
   +32 |   956B | MoveBG/MapObjTree :: control__16TMapObjTreeScaleFv
    +8 |   952B | GC2D/PauseMenu2 :: appearWindow__11TPauseMenu2Fv
   +72 |   944B | Enemy/hinokuri2 :: execute__18TNerveHino2PolluteCFP24TSpineBase<10TLive
   +24 |   936B | System/EmitterViewObj :: perform__21TMarioParticleManagerFUlPQ26JDrama9TGraph
   -32 |   936B | System/DrawSyncManager :: insert__Q27JGadget116TVector<Q216TDrawSyncManager19T
    +8 |   920B | Enemy/Kumokun :: calcRootMatrix__8TKumokunFv
   +24 |   920B | Player/MarioDraw :: considerWaist__6TMarioFv
   +16 |   916B | GC2D/GCConsole2 :: pauseOut__11TGCConsole2Fv
   +16 |   916B | MoveBG/MapObjCorona :: control__12TBathtubGripFv
   +64 |   912B | Enemy/hinokuri2 :: receiveMessageLv2__10THinokuri2FP9THitActorUl
  +112 |   912B | Player/MarioDraw :: addCallBack__6TMarioFPQ26JDrama9TGraphics
   +32 |   908B | Enemy/smallEnemy :: init__11TSmallEnemyFP12TLiveManager
   +32 |   904B | Camera/CameraOption :: ctrlOptionCamera___15CPolarSubCameraFv
   +80 |   904B | Enemy/fireWanwan :: behaveToWater__11TFireWanwanFP9THitActor
   +24 |   904B | Map/MapEventMare :: movement__13TMareWallRockFv
    -8 |   896B | System/EventWatcher :: evWarpMario__FP32TSpcTypedInterp<13TEventWatcher>Ul
   +56 |   892B | System/MarDirectorPreEntry :: preEntry__12TMarDirectorFP12TPerformList
   -16 |   888B | Enemy/fireWanwan :: bindBody__11TFireWanwanFPQ29JGeometry8TVec3<f>PQ29JG
   -16 |   884B | Animal/boid :: calcForces__11TBoidLeaderCFPC5TBoid
   +24 |   872B | Enemy/BathtubKiller :: resetBathtubKiller__14TBathtubKillerFv
   +16 |   872B | Map/MapEventMare :: rising__21TMareEventDepressWallFv
   +40 |   868B | MoveBG/MapObjHide :: loadAfter__23TWaterHitPictureHideObjFv
   +40 |   856B | MSound/MSoundSE :: __sinit_MSoundSE_cpp
   +24 |   856B | Enemy/hinokuri2 :: calc__13THino2MtxCalcFUs
   +24 |   856B | Map/Sky :: perform__4TSkyFUlPQ26JDrama9TGraphics
    +8 |   848B | Camera/sunmodel :: load__9TSunModelFR20JSUMemoryInputStream
  +168 |   848B | Enemy/bosseel :: perform__14TBossEelVortexFUlPQ26JDrama9TGraphics
   +48 |   844B | Enemy/conductor :: genEnemyFromPollution__10TConductorFv
   +32 |   844B | Strategic/ObjHitCheck :: checkActorsHit__12TObjHitCheckFv
   +32 |   840B | Enemy/bgtentacle :: calcPosition__Q211TBGTentacle5TNodeFP11TBGTentacle
   +40 |   836B | Enemy/telesa :: bind__7TTelesaFv
   +16 |   832B | Player/MarioAutodemo :: demoMain__6TMarioFv
    +8 |   828B | Enemy/mameGesso :: execute__21TNerveMameGessoDamageCFP24TSpineBase<10TL
   +48 |   828B | Player/MarioWait :: squating__6TMarioFv
    +8 |   824B | Enemy/tamaNoko :: execute__22TNerveTamaNokoHitWaterCFP24TSpineBase<10T
    +8 |   820B | Enemy/poihana :: execute__18TNervePoihanaSleepCFP24TSpineBase<10TLive
   +32 |   816B | GC2D/PauseMenu2 :: load__11TPauseMenu2FR20JSUMemoryInputStream
   +16 |   812B | Enemy/mameGesso :: execute__21TNerveMameGessoObjectCFP24TSpineBase<10TL
   +24 |   808B | MarioUtil/ShadowUtil :: request__19TMBindShadowManagerFRC20TCircleShadowRequ
    +8 |   804B | System/Application :: setupThreadFuncLogo__12TApplicationFv
   +16 |   800B | GC2D/GCConsole2 :: startDisappearStar__11TGCConsole2Fv
   +72 |   796B | Player/MarioMove :: checkSink__6TMarioFv
   +40 |   796B | Player/MarioRun :: slippingBasic__6TMarioFiii
    +8 |   792B | Enemy/gesso :: rollCheck__6TGessoFv
    +8 |   792B | Enemy/walkerEnemy :: behaveToFindMario__12TWalkerEnemyFv
   -16 |   792B | GC2D/CardSave :: drawMessage__9TCardSaveF10TEProgress
   +16 |   792B | GC2D/ProgSelect :: perform__11TProgSelectFUlPQ26JDrama9TGraphics
    +8 |   788B | Enemy/hamukuri :: execute__25TNerveHamuKuriBoundFreezeCFP24TSpineBase<
    +8 |   788B | Enemy/bossManta :: update__Q217TBossMantaManager17TMantaBattleStateFv
   +16 |   780B | Enemy/BossHanachanMain :: bind__13TBossHanachanFv
   +40 |   776B | MoveBG/MapObjBase :: makeObjAppeared__11TMapObjBaseFv
   +24 |   776B | MoveBG/MapObjGeneral :: receiveMessage__14TMapObjGeneralFP9THitActorUl
    -8 |   772B | Enemy/smallEnemy :: genEventCoin__11TSmallEnemyFv
   +24 |   772B | Player/MarioMove :: thinkParams__6TMarioFv
    -8 |   768B | Enemy/hamukuri :: execute__29TNerveHaneHamuKuriMoveOnGraphCFP24TSpineB

138 functions, 182932 bytes of code behind them.
```
