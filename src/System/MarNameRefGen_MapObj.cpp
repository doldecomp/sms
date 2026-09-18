#include "Enemy/BossTelesaObj.hpp"
#include "Map/MapStaticObject.hpp"
#include "Map/MapWireManager.hpp"
#include "MoveBG/Item.hpp"
#include "MoveBG/ItemManager.hpp"
#include "MoveBG/MapObjAirport.hpp"
#include "MoveBG/MapObjBall.hpp"
#include "MoveBG/MapObjBase.hpp"
#include "MoveBG/MapObjBianco.hpp"
#include "MoveBG/MapObjBlock.hpp"
#include "MoveBG/MapObjCloud.hpp"
#include "MoveBG/MapObjCorona.hpp"
#include "MoveBG/MapObjDolpic.hpp"
#include "MoveBG/MapObjEx.hpp"
#include "MoveBG/MapObjFence.hpp"
#include "MoveBG/MapObjFlag.hpp"
#include "MoveBG/MapObjFloat.hpp"
#include "MoveBG/MapObjGeneral.hpp"
#include "MoveBG/MapObjGrass.hpp"
#include "MoveBG/MapObjHide.hpp"
#include "MoveBG/MapObjItem2.hpp"
#include "MoveBG/MapObjMamma.hpp"
#include "MoveBG/MapObjManager.hpp"
#include "MoveBG/MapObjMare.hpp"
#include "MoveBG/MapObjMonte.hpp"
#include "MoveBG/MapObjOption.hpp"
#include "MoveBG/MapObjPinna.hpp"
#include "MoveBG/MapObjPlane.hpp"
#include "MoveBG/MapObjPole.hpp"
#include "MoveBG/MapObjPollution.hpp"
#include "MoveBG/MapObjRicco.hpp"
#include "MoveBG/MapObjSample.hpp"
#include "MoveBG/MapObjSirena.hpp"
#include "MoveBG/MapObjTown.hpp"
#include "MoveBG/MapObjTrap.hpp"
#include "MoveBG/MapObjTree.hpp"
#include "MoveBG/MapObjTurn.hpp"
#include "MoveBG/MapObjWave.hpp"
#include "MoveBG/ModelGate.hpp"
#include "MoveBG/Pool.hpp"
#include "MoveBG/WoodBarrel.hpp"
#include <System/MarNameRefGen.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <Map/MapCollisionManager.hpp>

// TODO (closure batch 114): 97.3%. The residue is entirely in the *inlined
// constructors* of MoveBG classes, so every fix is an edit to another unit's
// header and none of them were made here. Grouped by the class whose vtable
// store precedes the diff:
//
// Missing member initialisations (the ROM's ctor body has them, ours does not;
// the store order is descending, i.e. the batch-89 assignment chain
// `m.x = m.y = m.z = v;`, not an initialiser list):
//   TMapObjStartDemo   `stw 0, 0x138`
//   TMapObjBillboard   `stw 0, 0x150`
//   TTurboNozzleDoor   `stfs 0.0f` to 0x140, 0x13c, 0x138 (one TVec3)
//   TBigWindmill       `stw 0, 0x148`
//   TMapObjRootPakkun  `stw 0, 0x138`
//   TMapObjPuncher     `stfs 0.0f, 0x138`
//   TMareCork          `stw 0, 0x138` and `stb 0, 0x154`
//   TMareEventPoint    `stw 0, 0x68`
//   TPictureTelesa     `stb 0, 0x174`
//
// Extra member initialisations we emit and the ROM does not:
//   TCraneUpDown       `stfs` to 0x140/0x144
//   TFruitLauncher     `stw 0` to 0x138/0x13c/0x140
//
// Initialiser list vs assignment chain (we emit `bl TVec3::set<f>`, the ROM
// three descending `stfs`): TBalloonKoopaJr's `mCenterPos(0,0,0)` at 0x148 and
// TAmiKing's vector at 0x13c both want a ctor *body* chain instead.
//
// Called where the ROM inlines: TSandBlock::TSandBlock(const char*) and
// THideObj's constructor are out-of-line for us and expanded there (their
// bodies belong in the class).
//
// TMapObjSteam's name argument is `li r4, @5102@sda21` in the ROM and
// `addi r4, r31, 0x104` here: its default name string lives in the small-data
// pool, not the TU's .rodata block.
//
// Everything from 0x2580 on (~250 instructions of r30/r29 renaming plus the
// 0x50-vs-0x60 frame) is one cascade from the missing
// TTelesaSlot::TTelesaSlot(const char*): the extra callee-saved register it
// needs renames the object pointer for the whole tail. That constructor's
// depth-1 inlining refusal is the standing research item from batch 104, so it
// is left alone here.
JDrama::TNameRef* TMarNameRefGen::getNameRef_MapObj(const char* name) const
{
	if (strcmp(name, "MapObjBase") == 0)
		return new TMapObjBase;

	if (strcmp(name, "MapObjGeneral") == 0)
		return new TMapObjGeneral;

	if (strcmp(name, "Item") == 0)
		return new TItem;

	if (strcmp(name, "Fruit") == 0)
		return new TResetFruit;

	if (strcmp(name, "ResetFruit") == 0)
		return new TResetFruit;

	if (strcmp(name, "RandomFruit") == 0)
		return new TRandomFruit;

	if (strcmp(name, "CoverFruit") == 0)
		return new TCoverFruit;

	if (strcmp(name, "MapStaticObj") == 0)
		return new TMapStaticObj;

	if (strcmp(name, "MapObjSoundGroup") == 0)
		return new TMapObjSoundGroup;

	if (strcmp(name, "MapObjManager") == 0)
		return new TMapObjManager;

	if (strcmp(name, "MapObjBaseManager") == 0)
		return new TMapObjBaseManager;

	if (strcmp(name, "ItemManager") == 0)
		return new TItemManager;

	if (strcmp(name, "PoolManager") == 0)
		return new TPoolManager;

	if (strcmp(name, "MapWireManager") == 0)
		return new TMapWireManager;

	if (strcmp(name, "MapObjFlagManager") == 0)
		return new TMapObjFlagManager("旗管理");

	if (strcmp(name, "MapObjPoleManager") == 0)
		return new TMapObjPoleManager;

	if (strcmp(name, "MapObjWave") == 0)
		return new TMapObjWave;

	if (strcmp(name, "MapObjFlag") == 0)
		return new TMapObjFlag("旗");

	if (strcmp(name, "RockPlane") == 0)
		return new TRockPlane;

	if (strcmp(name, "SandPlane") == 0)
		return new TSandPlane;

	if (strcmp(name, "MapObjGrassManager") == 0)
		return new TMapObjGrassManager;

	if (strcmp(name, "MapObjGrassGroup") == 0)
		return new TMapObjGrassGroup;

	if (strcmp(name, "MirrorMapOperator") == 0)
		return new TMammaMirrorMapOperator;

	if (strcmp(name, "MapObjChangeStage") == 0)
		return new TMapObjChangeStage;

	if (strcmp(name, "MapObjChangeStageHipDrop") == 0)
		return new TMapObjChangeStageHipDrop;

	if (strcmp(name, "MapObjStartDemo") == 0)
		return new TMapObjStartDemo;

	if (strcmp(name, "DamageObj") == 0)
		return new TDamageObj;

	if (strcmp(name, "MapObjSmoke") == 0)
		return new TMapObjSmoke;

	if (strcmp(name, "MapObjWaterSpray") == 0)
		return new TMapObjWaterSpray;

	if (strcmp(name, "JuiceBlock") == 0)
		return new TJuiceBlock;

	if (strcmp(name, "Billboard") == 0)
		return new TMapObjBillboard;

	if (strcmp(name, "MapObjFloatOnSea") == 0)
		return new TMapObjFloatOnSea;

	if (strcmp(name, "WoodBarrel") == 0)
		return new TWoodBarrel;

	if (strcmp(name, "Door") == 0)
		return new TDoor;

	if (strcmp(name, "Fence") == 0)
		return new TFence;

	if (strcmp(name, "FenceRevolve") == 0)
		return new TRevolvingFenceOuter;

	if (strcmp(name, "FenceInner") == 0)
		return new TRevolvingFenceInner;

	if (strcmp(name, "FenceWaterH") == 0)
		return new TFenceWaterH;

	if (strcmp(name, "FenceWaterV") == 0)
		return new TFenceWater;

	if (strcmp(name, "RailFence") == 0)
		return new TRailFence;

	if (strcmp(name, "Football") == 0)
		return new TMapObjBall;

	if (strcmp(name, "Manhole") == 0)
		return new TManhole;

	if (strcmp(name, "Pool") == 0)
		return new TPool;

	if (strcmp(name, "WaterHitPictureHideObj") == 0)
		return new TWaterHitPictureHideObj;

	if (strcmp(name, "MapObjSwitch") == 0)
		return new TMapObjSwitch;

	if (strcmp(name, "RedCoinSwitch") == 0)
		return new TRedCoinSwitch;

	if (strcmp(name, "HideObjInfo") == 0)
		return new THideObjInfo;

	if (strcmp(name, "FruitBasket") == 0)
		return new TFruitBasket;

	if (strcmp(name, "BasketReverse") == 0)
		return new TBasketReverse;

	if (strcmp(name, "FruitBasketEvent") == 0)
		return new TFruitBasketEvent;

	if (strcmp(name, "JumpBase") == 0)
		return new TJumpBase;

	if (strcmp(name, "LampTrapIron") == 0)
		return new TLampTrapIron;

	if (strcmp(name, "LampTrapSpike") == 0)
		return new TLampTrapSpike;

	if (strcmp(name, "MapObjBase") == 0)
		return new TMapObjBase;

	if (strcmp(name, "MapObjSteam") == 0)
		return new TMapObjSteam;

	if (strcmp(name, "BananaTree") == 0)
		return new TMapObjTree;

	if (strcmp(name, "Palm") == 0)
		return new TMapObjTree;

	if (strcmp(name, "PalmOugi") == 0)
		return new TMapObjTree;

	if (strcmp(name, "PalmSago") == 0)
		return new TMapObjTree;

	if (strcmp(name, "PalmNatume") == 0)
		return new TMapObjTree("地形オブジェ基底");

	if (strcmp(name, "FruitTree") == 0)
		return new TMapObjBase;

	if (strcmp(name, "MapObjTreeScale") == 0)
		return new TMapObjTreeScale;

	if (strcmp(name, "RideCloud") == 0)
		return new TRideCloud;

	if (strcmp(name, "AirportSwitch") == 0)
		return new TAirportSwitch;

	if (strcmp(name, "AirportPool") == 0)
		return new TPool;

	if (strcmp(name, "WoodBox") == 0)
		return new TWoodBox;

	if (strcmp(name, "AirportEventSink") == 0)
		return new TAirportEventSink;

	if (strcmp(name, "JellyGate") == 0)
		return new TModelGate;

	if (strcmp(name, "GateShadow") == 0)
		return new TGateShadow;

	if (strcmp(name, "IceCar") == 0)
		return new TMapObjBase;

	if (strcmp(name, "MonumentShine") == 0)
		return new TMonumentShine;

	if (strcmp(name, "BellDolpicTV") == 0)
		return new TBellDolpic(1);

	if (strcmp(name, "BellDolpicPolice") == 0)
		return new TBellDolpic(0);

	if (strcmp(name, "DptMonteFence") == 0)
		return new TDptMonteFence;

	if (strcmp(name, "DolWeathercock") == 0)
		return new TMapObjTurn;

	if (strcmp(name, "DemoCannon") == 0)
		return new TDemoCannon;

	if (strcmp(name, "TurboNozzleDoor") == 0)
		return new TTurboNozzleDoor;

	if (strcmp(name, "CraneRotY") == 0)
		return new TCraneRotY;

	if (strcmp(name, "craneUpDown") == 0)
		return new TCraneUpDown;

	if (strcmp(name, "RiccoLog") == 0)
		return new TWoodLog("丸太");

	if (strcmp(name, "GesoSurfBoard") == 0)
		return new TItem;

	if (strcmp(name, "SurfGesoRed") == 0)
		return new TSurfGesoObj;

	if (strcmp(name, "SurfGesoYellow") == 0)
		return new TSurfGesoObj;

	if (strcmp(name, "SurfGesoGreen") == 0)
		return new TSurfGesoObj;

	if (strcmp(name, "riccoWatermill") == 0)
		return new TRiccoWatermill;

	if (strcmp(name, "submarine") == 0)
		return new TMapObjBase;

	if (strcmp(name, "RiccoSwitch") == 0)
		return new TFruitSwitch;

	if (strcmp(name, "RiccoSwitchShine") == 0)
		return new TFruitLauncher;

	if (strcmp(name, "BigWindmill") == 0)
		return new TBigWindmill("巨大風車");

	if (strcmp(name, "MiniWindmill") == 0)
		return new TBiancoMiniWindmill("風車（ビアンコ小）");

	if (strcmp(name, "WindmillRoof") == 0)
		return new TMapObjBase;

	if (strcmp(name, "MapObjRootPakkun") == 0)
		return new TMapObjRootPakkun("ボスパックンの根");

	if (strcmp(name, "BiaBell") == 0)
		return new TBiancoBell("ベル水車");

	if (strcmp(name, "BiaWatermill") == 0)
		return new TBiancoWatermill("水車（ビアンコ大）");

	if (strcmp(name, "BellWatermill") == 0)
		return new TBellWatermill("ベル水車");

	if (strcmp(name, "BiaWatermillVertical") == 0)
		return new TBiancoWatermillVertical("水車（ビアンコ垂直）");

	if (strcmp(name, "BiaTurnBridge") == 0)
		return new TMapObjBase;

	if (strcmp(name, "LeafBoat") == 0)
		return new TLeafBoat("リーフボート");

	if (strcmp(name, "LeafBoatRotten") == 0)
		return new TLeafBoatRotten("腐ったリーフボート");

	if (strcmp(name, "LampSeesawMain") == 0)
		return new TLampSeesawMain("ランプシーソー");

	if (strcmp(name, "LampSeesaw") == 0)
		return new TLampSeesaw("ランプシーソー（従）");

	if (strcmp(name, "SandBird") == 0)
		return new TSandBird;

	if (strcmp(name, "SandLeafBase") == 0)
		return new TSandLeafBase;

	if (strcmp(name, "SandBombBase") == 0)
		return new TSandBombBase;

	if (strcmp(name, "SandBomb") == 0)
		return new TSandBomb;

	if (strcmp(name, "LeanMirror") == 0)
		return new TLeanMirror;

	if (strcmp(name, "WaterMelon") == 0)
		return new TBigWatermelon;

	if (strcmp(name, "WatermelonStatic") == 0)
		return new TWatermelonStatic;

	if (strcmp(name, "ShiningStone") == 0)
		return new TShiningStone;

	if (strcmp(name, "SandCastle") == 0)
		return new TSandCastle;

	if (strcmp(name, "MammaBlockRotate") == 0)
		return new TMammaBlockRotate;

	if (strcmp(name, "MammaSurfboard") == 0)
		return new TMapObjBase;

	if (strcmp(name, "MammaYacht") == 0)
		return new TMammaYacht;

	if (strcmp(name, "GoalWatermelon") == 0)
		return new TGoalWatermelon;

	if (strcmp(name, "SandEgg") == 0)
		return new TSandEgg;

	if (strcmp(name, "Merrygoround") == 0)
		return new TMerrygoround;

	if (strcmp(name, "FerrisWheel") == 0)
		return new TFerrisWheel;

	if (strcmp(name, "Viking") == 0)
		return new TViking;

	if (strcmp(name, "SirenaGate") == 0)
		return new TSirenaGate;

	if (strcmp(name, "PinnaDoor") == 0)
		return new TPinnaEntrance;

	if (strcmp(name, "PinnaDoorOpen") == 0)
		return new TPinnaEntrance;

	if (strcmp(name, "BalloonKoopaJr") == 0)
		return new TBalloonKoopaJr;

	if (strcmp(name, "ShellCup") == 0)
		return new TShellCup;

	if (strcmp(name, "GateShell") == 0)
		return new TMareGate;

	if (strcmp(name, "MareGate") == 0)
		return new TMareGate;

	if (strcmp(name, "WaterRecoverObj") == 0)
		return new TWaterRecoverObj;

	if (strcmp(name, "AmiKing") == 0)
		return new TAmiKing;

	if (strcmp(name, "PinnaCoaster") == 0)
		return new TPinnaCoaster;

	if (strcmp(name, "Cogwheel") == 0)
		return new TCogwheel;

	if (strcmp(name, "MapObjElasticCode") == 0)
		return new TMapObjElasticCode;

	if (strcmp(name, "MapObjGrowTree") == 0)
		return new TMapObjGrowTree;

	if (strcmp(name, "WireBell") == 0)
		return new TWireBell;

	if (strcmp(name, "MuddyBoat") == 0)
		return new TMuddyBoat;

	if (strcmp(name, "Puncher") == 0)
		return new TMapObjPuncher;

	if (strcmp(name, "CoinFish") == 0)
		return new TJointCoin;

	if (strcmp(name, "MareFall") == 0)
		return new TMareFall;

	if (strcmp(name, "MareCork") == 0)
		return new TMareCork;

	if (strcmp(name, "MareEventPoint") == 0)
		return new TMareEventPoint;

	if (strcmp(name, "CasinoRoulette") == 0)
		return new TCasinoRoulette;

	if (strcmp(name, "Roulette") == 0)
		return new TRoulette;

	if (strcmp(name, "SlotDrum") == 0)
		return new TSlotDrum;

	if (strcmp(name, "ItemSlotDrum") == 0)
		return new TItemSlotDrum;

	// TODO: the map keeps TTelesaSlot's in-class constructor as a weak
	// out-of-line copy (0x98) *and* calls it here (the `bl` is straight
	// after `__nw__FUl`, so at depth 1), so MWCC refused the expansion at
	// this site. We still expand it, which costs one extra callee-saved
	// register for the rest of the ladder.
	// Batch 104 probed the caller-budget reading and everything else it
	// could think of, and all of it expands: an in-class constructor has no
	// statement limit at depth 1 (1..30 measured); a base initialiser, two
	// bases, a vtable, a virtual destructor, an array-of-class member
	// needing `__construct_array` and a defaulted `const char*` argument all
	// together still expand; and a `new` ladder with 256 distinct in-class
	// constructors and 2571 inlined instructions expands every one of them.
	// The one shape that does give weak-plus-`bl` is a member of a class
	// *template* defined outside the class body, which TTelesaSlot is not.
	// Still open; see codegen-tells.md, research batch 104.
	if (strcmp(name, "TelesaSlot") == 0)
		return new TTelesaSlot("btelesaSlot");

	if (strcmp(name, "CasinoPanelGate") == 0)
		return new TCasinoPanelGate;

	if (strcmp(name, "WarpArea") == 0)
		return new TWarpAreaActor;

	if (strcmp(name, "GlassBreak") == 0)
		return new TMapObjGeneral;

	if (strcmp(name, "Closet") == 0)
		return new TCloset;

	if (strcmp(name, "Donchou") == 0)
		return new TDonchou;

	if (strcmp(name, "SakuCasino") == 0)
		return new TSakuCasino;

	if (strcmp(name, "SirenabossWall") == 0)
		return new TSirenabossWall;

	if (strcmp(name, "SirenaCasinoRoof") == 0)
		return new TSirenaCasinoRoof;

	if (strcmp(name, "ChestRevolve") == 0)
		return new TChestRevolve;

	if (strcmp(name, "PosterTeresa") == 0)
		return new TWaterHitPictureHideObj;

	if (strcmp(name, "PictureTeresa") == 0)
		return new TPictureTelesa;

	if (strcmp(name, "PanelBreak") == 0)
		return new TMapObjGeneral;

	if (strcmp(name, "PanelRevolve") == 0)
		return new TPanelRevolve;

	if (strcmp(name, "TelesaBlock") == 0)
		return new TTelesaBlock;

	if (strcmp(name, "MonteRoot") == 0)
		return new TMapObjMonteRoot;

	if (strcmp(name, "JumpMushroom") == 0)
		return new TJumpMushroom;

	if (strcmp(name, "HangingBridge") == 0)
		return new THangingBridge;

	if (strcmp(name, "SwingBoard") == 0)
		return new TSwingBoard;

	if (strcmp(name, "GoalFlag") == 0)
		return new TGoalFlag;

	if (strcmp(name, "FluffManager") == 0)
		return new TFluffManager;

	if (strcmp(name, "Bathtub") == 0)
		return new TBathtub;

	if (strcmp(name, "FileLoadBlockA") == 0)
		return new TFileLoadBlock;

	if (strcmp(name, "FileLoadBlockB") == 0)
		return new TFileLoadBlock;

	if (strcmp(name, "FileLoadBlockC") == 0)
		return new TFileLoadBlock;

	if (strcmp(name, "NormalLift") == 0)
		return new TNormalLift;

	if (strcmp(name, "EXRollCube") == 0)
		return new TRailBlock;

	if (strcmp(name, "EXKickBoard") == 0)
		return new TNormalLift;

	if (strcmp(name, "Umaibou") == 0)
		return new TRollBlock;

	if (strcmp(name, "Kamaboko") == 0)
		return new TNormalLift;

	if (strcmp(name, "GetaGreen") == 0)
		return new TRollBlock;

	if (strcmp(name, "GetaOrange") == 0)
		return new TRollBlock;

	if (strcmp(name, "RailBlock") == 0)
		return new TRailBlock;

	if (strcmp(name, "RailBlockR") == 0)
		return new TRailBlock;

	if (strcmp(name, "RailBlockY") == 0)
		return new TRailBlock;

	if (strcmp(name, "RailBlockB") == 0)
		return new TRailBlock;

	if (strcmp(name, "RollBlock") == 0)
		return new TRollBlock;

	if (strcmp(name, "RollBlockR") == 0)
		return new TRollBlock;

	if (strcmp(name, "RollBlockY") == 0)
		return new TRollBlock;

	if (strcmp(name, "RollBlockB") == 0)
		return new TRollBlock;

	if (strcmp(name, "Uirou") == 0)
		return new TNormalLift;

	if (strcmp(name, "Castella") == 0)
		return new TNormalLift;

	if (strcmp(name, "Hikidashi") == 0)
		return new TNormalLift;

	if (strcmp(name, "YoshiBlock") == 0)
		return new TWoodBlock;

	if (strcmp(name, "MapObjNail") == 0)
		return new TMapObjNail;

	if (strcmp(name, "WoodBlock") == 0)
		return new TWoodBlock;

	if (strcmp(name, "MapObjRevivalPollution") == 0)
		return new TMapObjRevivalPollution;

	if (strcmp(name, "PolluterBase") == 0)
		return new TPolluterBase;

	if (strcmp(name, "Coin") == 0)
		return new TCoin;

	if (strcmp(name, "CoinRed") == 0)
		return new TCoinRed;

	if (strcmp(name, "CoinBlue") == 0)
		return new TCoinBlue;

	if (strcmp(name, "HideObj") == 0)
		return new THideObjBase;

	if (strcmp(name, "WaterHitHideObj") == 0)
		return new TWaterHitHideObj;

	if (strcmp(name, "FruitHitHideObj") == 0)
		return new TFruitHitHideObj;

	if (strcmp(name, "HipDropHideObj") == 0)
		return new THipDropHideObj;

	if (strcmp(name, "HideObjPictureTwin") == 0)
		return new THideObjPictureTwin;

	if (strcmp(name, "FlowerCoin") == 0)
		return new TFlowerCoin;

	if (strcmp(name, "Shine") == 0)
		return new TShine;

	if (strcmp(name, "NozzleBox") == 0)
		return new TNozzleBox;

	if (strcmp(name, "ItemNozzle") == 0)
		return new TItemNozzle;

	if (strcmp(name, "Mushroom1up") == 0)
		return new TMushroom1up(0);

	if (strcmp(name, "Mushroom1upR") == 0)
		return new TMushroom1up(1);

	if (strcmp(name, "Mushroom1upX") == 0)
		return new TMushroom1up(2);

	if (strcmp(name, "EggYoshi") == 0)
		return new TEggYoshi;

	if (strcmp(name, "NormalBlock") == 0)
		return new TMapObjBase;

	if (strcmp(name, "SuperHipDropBlock") == 0)
		return new TSuperHipDropBlock;

	if (strcmp(name, "IceBlock") == 0)
		return new TIceBlock;

	if (strcmp(name, "BreakableBlock") == 0)
		return new TBreakableBlock;

	if (strcmp(name, "BrickBlock") == 0)
		return new TBrickBlock;

	if (strcmp(name, "WatermelonBlock") == 0)
		return new TBreakHideObj;

	if (strcmp(name, "SandBlock") == 0)
		return new TSandBlock;

	return nullptr;
}
