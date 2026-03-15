#include "Map/MapStaticObject.hpp"
#include "Map/MapWireManager.hpp"
#include "MoveBG/Item.hpp"
#include "MoveBG/ItemManager.hpp"
#include "MoveBG/MapObjAirport.hpp"
#include "MoveBG/MapObjBall.hpp"
#include "MoveBG/MapObjBase.hpp"
#include "MoveBG/MapObjBlock.hpp"
#include "MoveBG/MapObjCloud.hpp"
#include "MoveBG/MapObjCorona.hpp"
#include "MoveBG/MapObjDolpic.hpp"
#include "MoveBG/MapObjEx.hpp"
#include "MoveBG/MapObjFence.hpp"
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
#include "MoveBG/MapObjTown.hpp"
#include "MoveBG/MapObjTrap.hpp"
#include "MoveBG/MapObjTree.hpp"
#include "MoveBG/MapObjTurn.hpp"
#include "MoveBG/MapObjWave.hpp"
#include "MoveBG/ModelGate.hpp"
#include "MoveBG/Pool.hpp"
#include "MoveBG/WoodBarrel.hpp"
#include <System/MarNameRefGen.hpp>

JDrama::TNameRef* TMarNameRefGen::getNameRef_MapObj(const char* name) const
{
	if (strcmp(name, "MapObjBase") == 0)
		return new TMapObjBase("地形オブジェ基底");

	if (strcmp(name, "MapObjGeneral") == 0)
		return new TMapObjGeneral("汎用地形オブジェ");

	if (strcmp(name, "Item") == 0)
		return new TItem("アイテム");

	if (strcmp(name, "Fruit") == 0)
		return new TResetFruit("無限フルーツ");

	if (strcmp(name, "ResetFruit") == 0)
		return new TResetFruit("無限フルーツ");

	if (strcmp(name, "RandomFruit") == 0)
		return new TRandomFruit("ランダムフルーツ");

	if (strcmp(name, "CoverFruit") == 0)
		return new TCoverFruit("フタのフルーツ");

	if (strcmp(name, "MapStaticObj") == 0)
		return new TMapStaticObj("簡易マップオブジェ");

	if (strcmp(name, "MapObjSoundGroup") == 0)
		return new TMapObjSoundGroup("環境音テスト");

	if (strcmp(name, "MapObjManager") == 0)
		return new TMapObjManager("地形オブジェ管理");

	if (strcmp(name, "MapObjBaseManager") == 0)
		return new TMapObjBaseManager("地形基底オブジェ管理");

	if (strcmp(name, "ItemManager") == 0)
		return new TItemManager("アイテムマネージャー");

	if (strcmp(name, "PoolManager") == 0)
		return new TPoolManager("水場マネージャー");

	if (strcmp(name, "MapWireManager") == 0)
		return new TMapWireManager("ワイヤー管理");

	// TODO:
	// if ( strcmp( name, "MapObjFlagManager" ) == 0 )
	//     return new TMapObjFlagManager   ( "旗管理" );

	if (strcmp(name, "MapObjPoleManager") == 0)
		return new TMapObjPoleManager("旗管理");

	if (strcmp(name, "MapObjWave") == 0)
		return new TMapObjWave("波の表現");

	// TODO:
	// if ( strcmp( name, "MapObjFlag" ) == 0 )
	//     return new TMapObjFlag  ("旗");

	if (strcmp(name, "RockPlane") == 0)
		return new TRockPlane("岩平面");

	if (strcmp(name, "SandPlane") == 0)
		return new TSandPlane("砂平面");

	if (strcmp(name, "MapObjGrassManager") == 0)
		return new TMapObjGrassManager("草管理");

	if (strcmp(name, "MapObjGrassGroup") == 0)
		return new TMapObjGrassGroup;

	if (strcmp(name, "MirrorMapOperator") == 0)
		return new TMammaMirrorMapOperator("鏡内地形操作");

	if (strcmp(name, "MapObjChangeStage") == 0)
		return new TMapObjChangeStage("ステージ切り替え");

	if (strcmp(name, "MapObjChangeStageHipDrop") == 0)
		return new TMapObjChangeStageHipDrop(
		    "ステージ切り替え（ヒップドロップ）");

	if (strcmp(name, "MapObjStartDemo") == 0)
		return new TMapObjStartDemo("デモ開始オブジェ");

	if (strcmp(name, "DamageObj") == 0)
		return new TDamageObj("ダメージオブジェ");

	if (strcmp(name, "MapObjSmoke") == 0)
		return new TMapObjSmoke("黒煙");

	if (strcmp(name, "MapObjWaterSpray") == 0)
		return new TMapObjWaterSpray("汎用水しぶき");

	if (strcmp(name, "JuiceBlock") == 0)
		return new TJuiceBlock("ジュースブロック");

	if (strcmp(name, "Billboard") == 0)
		return new TMapObjBillboard("看板");

	if (strcmp(name, "MapObjFloatOnSea") == 0)
		return new TMapObjFloatOnSea("海に浮くオブジェ");

	if (strcmp(name, "WoodBarrel") == 0)
		return new TWoodBarrel("木樽");

	if (strcmp(name, "Door") == 0)
		return new TDoor("ドア");

	if (strcmp(name, "Fence") == 0)
		return new TFence("フェンス");

	if (strcmp(name, "FenceRevolve") == 0)
		return new TRevolvingFenceOuter("フェンス外側");

	if (strcmp(name, "FenceInner") == 0)
		return new TRevolvingFenceInner("フェンス内側");

	if (strcmp(name, "FenceWaterH") == 0)
		return new TFenceWaterH("水回転フェンス（水平）");

	if (strcmp(name, "FenceWaterV") == 0)
		return new TFenceWater("水回転フェンス（垂直）");

	if (strcmp(name, "RailFence") == 0)
		return new TRailFence("レールフェンス");

	if (strcmp(name, "Football") == 0)
		return new TMapObjBall("レールフェンス");

	if (strcmp(name, "Manhole") == 0)
		return new TManhole("マンホール");

	if (strcmp(name, "Pool") == 0)
		return new TPool("水場");

	// TODO:
	// if (strcmp(name, "WaterHitPictureHideObj") == 0)
	// 	return new TWaterHitPictureHideObj("オブジェ出現の絵");

	if (strcmp(name, "MapObjSwitch") == 0)
		return new TMapObjSwitch("オブジェスイッチ");

	if (strcmp(name, "RedCoinSwitch") == 0)
		return new TRedCoinSwitch("赤コインスイッチ");

	if (strcmp(name, "HideObjInfo") == 0)
		return new THideObjInfo("オブジェ出現情報");

	// TODO:
	// if (strcmp(name, "FruitBasket") == 0)
	// 	return new TFruitBasket("バスケット");

	if (strcmp(name, "BasketReverse") == 0)
		return new TBasketReverse("さかさバスケット");

	// TODO:
	// if ( strcmp(name, "FruitBasketEvent") == 0 )
	// 	return new TFruitBasketEvent("バスケット（イベント用）");

	if (strcmp(name, "JumpBase") == 0)
		return new TJumpBase("移動ジャンプ台");

	if (strcmp(name, "LampTrapIron") == 0)
		return new TLampTrapIron("鉄板ランプ");

	if (strcmp(name, "LampTrapSpike") == 0)
		return new TLampTrapSpike("トゲランプ");

	if (strcmp(name, "MapObjBase") == 0)
		return new TMapObjBase("地形オブジェ基底");

	if (strcmp(name, "MapObjSteam") == 0)
		return new TMapObjSteam("水蒸気");

	if (strcmp(name, "BananaTree") == 0)
		return new TMapObjTree("木");

	if (strcmp(name, "Palm") == 0)
		return new TMapObjTree("木");

	if (strcmp(name, "PalmOugi") == 0)
		return new TMapObjTree("木");

	if (strcmp(name, "PalmSago") == 0)
		return new TMapObjTree("木");

	if (strcmp(name, "PalmNatume") == 0)
		return new TMapObjTree("地形オブジェ基底");

	if (strcmp(name, "FruitTree") == 0)
		return new TMapObjBase("地形オブジェ基底");

	if (strcmp(name, "MapObjTreeScale") == 0)
		return new TMapObjTreeScale("スケールの木");

	if (strcmp(name, "RideCloud") == 0)
		return new TRideCloud("乗れる雲");

	if (strcmp(name, "AirportSwitch") == 0)
		return new TAirportSwitch("水場スイッチ");

	if (strcmp(name, "AirportPool") == 0)
		return new TPool("水場");

	// TODO:
	// if (strcmp(name, "WoodBox") == 0)
	// 	return new TWoodBox ("木箱");

	if (strcmp(name, "AirportEventSink") == 0)
		return new TAirportEventSink("水場スイッチ");

	if (strcmp(name, "JellyGate") == 0)
		return new TModelGate("<TModelGate>");

	if (strcmp(name, "GateShadow") == 0)
		return new TGateShadow;

	if (strcmp(name, "IceCar") == 0)
		return new TMapObjBase("地形オブジェ基底");

	if (strcmp(name, "MonumentShine") == 0)
		return new TMonumentShine("モニュメントシャイン");

	if (strcmp(name, "BellDolpicTV") == 0)
		return new TBellDolpic(1, "ドルピック鐘");

	if (strcmp(name, "BellDolpicPolice") == 0)
		return new TBellDolpic(0, "ドルピック鐘");

	if (strcmp(name, "DptMonteFence") == 0)
		return new TDptMonteFence("投げモンテフェンス");

	if (strcmp(name, "DolWeathercock") == 0)
		return new TMapObjTurn("水回転オブジェ");

	if (strcmp(name, "DemoCannon") == 0)
		return new TDemoCannon("デモ砲台");

	if (strcmp(name, "TurboNozzleDoor") == 0)
		return new TTurboNozzleDoor("ターボノズルドア");

	if (strcmp(name, "CraneRotY") == 0)
		return new TCraneRotY("デモ砲台");

	if (strcmp(name, "craneUpDown") == 0)
		return new TCraneUpDown("上下クレーン");

	// TODO:
	// if ( strcmp(name, "RiccoLog") == 0 )
	// 	return new TWoodLog("丸太");

	if (strcmp(name, "GesoSurfBoard") == 0)
		return new TItem("アイテム");

	if (strcmp(name, "SurfGesoRed") == 0)
		new TSurfGesoObj("イカサーフィン");

	if (strcmp(name, "SurfGesoYellow") == 0)
		return new TSurfGesoObj("イカサーフィン");

	if (strcmp(name, "SurfGesoGreen") == 0)
		return new TSurfGesoObj("イカサーフィン");

	if (strcmp(name, "riccoWatermill") == 0)
		return new TRiccoWatermill("リコ水車");

	if (strcmp(name, "submarine") == 0)
		return new TMapObjBase("地形オブジェ基底");

	if (strcmp(name, "RiccoSwitch") == 0)
		return new TFruitSwitch("フルーツスイッチ");

	if (strcmp(name, "RiccoSwitchShine") == 0)
		return new TFruitLauncher("フルーツ発射口");

	// TODO:
	// if ( strcmp(name, "BigWindmill") == 0 )
	// 	return new TBigWindmill("巨大風車");

	// TODO:
	// if ( strcmp(name, "MiniWindmill") == 0 )
	// 	return new TBiancoMiniWindmill("風車（ビアンコ小）");

	if (strcmp(name, "WindmillRoof") == 0)
		return new TMapObjBase("地形オブジェ基底");

	// TODO:
	// if ( strcmp(name, "MapObjRootPakkun") == 0 )
	// 	return new TMapObjRootPakkun("ボスパックンの根");

	// TODO:
	// if (strcmp(name, "BiaBell") == 0)
	// 	return new TBiancoBell("ベル水車");

	// TODO:
	// if ( strcmp(name, "BiaWatermill") == 0 )
	// 	return new TBiancoWatermill("水車（ビアンコ大）");

	// TODO:
	// if ( strcmp(name, "BellWatermill") == 0 )
	// 	return new TBellWatermill("ベル水車");

	// TODO:
	// if ( strcmp(name, "BiaWatermillVertical") == 0 )
	// 	return new TBiancoWatermillVertical("水車（ビアンコ垂直）");

	if (strcmp(name, "BiaTurnBridge") == 0)
		return new TMapObjBase("地形オブジェ基底");

	// TODO:
	// if ( strcmp(name, "LeafBoat") == 0 )
	// 	return new TLeafBoat("リーフボート");

	// TODO:
	// if ( strcmp(name, "LeafBoatRotten") == 0 )
	// 	return new TLeafBoatRotten("腐ったリーフボート");

	// TODO:
	// if ( strcmp(name, "LampSeesawMain") == 0 )
	// 	return new TLampSeesawMain("ランプシーソー");

	// TODO:
	// if ( strcmp(name, "LampSeesaw") == 0 )
	// 	return new TLampSeesaw("ランプシーソー（従）");

	if (strcmp(name, "SandBird") == 0)
		return new TSandBird("おおすな鳥");

	if (strcmp(name, "SandLeafBase") == 0)
		return new TSandLeafBase("すなやまの芽の土台");

	if (strcmp(name, "SandBombBase") == 0)
		return new TSandBombBase("すなやま爆弾の土台");

	if (strcmp(name, "SandBomb") == 0)
		return new TSandBomb;

	if (strcmp(name, "LeanMirror") == 0)
		return new TLeanMirror("ぐらぐら鏡");

	if (strcmp(name, "WaterMelon") == 0)
		return new TBigWatermelon("お化けスイカ");

	if (strcmp(name, "ShiningStone") == 0)
		return new TShiningStone("太陽石");

	if (strcmp(name, "SandCastle") == 0)
		return new TSandCastle("砂の城");

	if (strcmp(name, "MammaBlockRotate") == 0)
		return new TMammaBlockRotate("太陽の塔ブロック");

	if (strcmp(name, "MammaSurfboard") == 0)
		return new TMapObjBase("地形オブジェ基底");

	if (strcmp(name, "MammaYacht") == 0)
		return new TMammaYacht("砂の城");

	if (strcmp(name, "GoalWatermelon") == 0)
		return new TGoalWatermelon("スイカゴール");

	if (strcmp(name, "SandEgg") == 0)
		return new TSandEgg("すなのたまご");

	if (strcmp(name, "Merrygoround") == 0)
		return new TMerrygoround("メリーゴーランド");

	if (strcmp(name, "FerrisWheel") == 0)
		return new TFerrisWheel("観覧車");

	if (strcmp(name, "Viking") == 0)
		return new TViking("バイキング");

	// TODO:
	// if ( strcmp(name, "SirenaGate") == 0 )
	// 	return new TSirenaGate("シレナゲート");

	if (strcmp(name, "PinnaDoor") == 0)
		return new TPinnaEntrance("シレナゲート");

	if (strcmp(name, "PinnaDoorOpen") == 0)
		return new TPinnaEntrance("ピンナ入り口");

	if (strcmp(name, "BalloonKoopaJr") == 0)
		return new TBalloonKoopaJr("風船（クッパＪｒ）");

	if (strcmp(name, "ShellCup") == 0)
		return new TShellCup("シェルカップ");

	if (strcmp(name, "GateShell") == 0)
		return new TMareGate("マーレゲート");

	if (strcmp(name, "MareGate") == 0)
		return new TMareGate("マーレゲート");

	if (strcmp(name, "WaterRecoverObj") == 0)
		return new TWaterRecoverObj("水回復オブジェ");

	if (strcmp(name, "AmiKing") == 0)
		return new TAmiKing("アミキング");

	if (strcmp(name, "PinnaCoaster") == 0)
		return new TPinnaCoaster("コースター");

	if (strcmp(name, "Cogwheel") == 0)
		return new TCogwheel("天秤");

	if (strcmp(name, "MapObjElasticCode") == 0)
		return new TMapObjElasticCode("ゴムひも");

	if (strcmp(name, "MapObjGrowTree") == 0)
		return new TMapObjGrowTree("もやしの木");

	if (strcmp(name, "WireBell") == 0)
		return new TWireBell("ワイヤー鈴（紫）");

	if (strcmp(name, "MuddyBoat") == 0)
		return new TMuddyBoat("どろの船");

	if (strcmp(name, "Puncher") == 0)
		return new TMapObjPuncher("パンチャー");

	if (strcmp(name, "CoinFish") == 0)
		return new TJointCoin("ジョイントコイン");

	if (strcmp(name, "MareFall") == 0)
		return new TMareFall("マーレ滝");

	if (strcmp(name, "MareCork") == 0)
		return new TMareCork("マーレコルク");

	if (strcmp(name, "MareEventPoint") == 0)
		return new TMareEventPoint("イベントポイント");

	// TODO:
	// if ( strcmp(name, "CasinoRoulette") == 0 )
	// 	return new TCasinoRoulette("カジノルーレット");

	// TODO:
	// if ( strcmp(name, "Roulette") == 0 )
	// 	return new TRoulette("ルーレット");

	// TODO:
	// if ( strcmp(name, "SlotDrum") == 0 )
	// 	return new TSlotDrum("スロットマシーン");

	// TODO:
	// if ( strcmp(name, "ItemSlotDrum") == 0 )
	// 	return new TItemSlotDrum("スロットマシーン");

	// TODO:
	// if ( strcmp(name, "TelesaSlot") == 0 )
	// 	return new TTelesaSlot("btelesaSlot");

	// TODO:
	// if ( strcmp(name, "CasinoPanelGate") == 0 )
	// 	return new TCasinoPanelGate("カジノパネルゲート");

	// TODO:
	// if ( strcmp(name, "WarpArea") == 0 )
	// 	return new TWarpAreaActor("ワープエリア");

	if (strcmp(name, "GlassBreak") == 0)
		return new TMapObjGeneral("汎用地形オブジェ");

	// TODO:
	// if (strcmp(name, "Closet") == 0)
	// 	return new TCloset("クローゼット");

	// TODO:
	// if (strcmp(name, "Donchou") == 0)
	// 	return new TDonchou("パネルカーテン");

	// TODO:
	// if ( strcmp(name, "SakuCasino") == 0 )
	// 	return new TSakuCasino("パネル柵");

	// TODO:
	// if ( strcmp(name, "SirenabossWall") == 0 )
	// 	return new TSirenabossWall("ボステレサ部屋壁");

	// TODO:
	// if ( strcmp(name, "SirenaCasinoRoof") == 0 )
	// 	return new TSirenaCasinoRoof("カジノ部屋天井");

	// TODO:
	// if ( strcmp(name, "ChestRevolve") == 0 )
	// 	return new TChestRevolve("回転棚");

	// TODO:
	// if ( strcmp(name, "PosterTeresa") == 0 )
	// 	return new TWaterHitPictureHideObj("オブジェ出現の絵");

	// TODO:
	// if ( strcmp(name, "PictureTeresa") == 0 )
	// 	return new TPictureTelesa("テルサの絵");

	if (strcmp(name, "PanelBreak") == 0)
		return new TMapObjGeneral("汎用地形オブジェ");

	// TODO:
	// if ( strcmp(name, "PanelRevolve") == 0 )
	// 	return new TPanelRevolve("回転棚");

	if (strcmp(name, "TelesaBlock") == 0)
		return new TTelesaBlock;

	if (strcmp(name, "MonteRoot") == 0)
		return new TMapObjMonteRoot("根っこ");

	if (strcmp(name, "JumpMushroom") == 0)
		return new TJumpMushroom("ジャンプきのこ");

	if (strcmp(name, "HangingBridge") == 0)
		return new THangingBridge("つり橋");

	if (strcmp(name, "SwingBoard") == 0)
		return new TSwingBoard("つり橋");

	if (strcmp(name, "GoalFlag") == 0)
		return new TGoalFlag("ゴールフラグ");

	if (strcmp(name, "FluffManager") == 0)
		return new TFluffManager("特別な綿毛");

	if (strcmp(name, "Bathtub") == 0)
		return new TBathtub("バスタブ");

	if (strcmp(name, "FileLoadBlockA") == 0)
		return new TFileLoadBlock("ファイル読み込みブロック");

	if (strcmp(name, "FileLoadBlockB") == 0)
		return new TFileLoadBlock("ファイル読み込みブロック");

	if (strcmp(name, "FileLoadBlockC") == 0)
		return new TFileLoadBlock("ファイル読み込みブロック");

	if (strcmp(name, "NormalLift") == 0)
		return new TNormalLift("ノーマルリフト");

	if (strcmp(name, "EXRollCube") == 0)
		return new TRailBlock("移動板");

	if (strcmp(name, "EXKickBoard") == 0)
		return new TNormalLift("ノーマルリフト");

	if (strcmp(name, "Umaibou") == 0)
		return new TRollBlock("回転板");

	if (strcmp(name, "Kamaboko") == 0)
		return new TNormalLift("ノーマルリフト");

	if (strcmp(name, "GetaGreen") == 0)
		return new TRollBlock("回転板");

	if (strcmp(name, "GetaOrange") == 0)
		return new TRollBlock("回転板");

	if (strcmp(name, "RailBlock") == 0)
		return new TRailBlock("移動板");

	if (strcmp(name, "RailBlockR") == 0)
		return new TRailBlock("移動板");

	if (strcmp(name, "RailBlockY") == 0)
		return new TRailBlock("移動板");

	if (strcmp(name, "RailBlockB") == 0)
		return new TRailBlock("移動板");

	if (strcmp(name, "RollBlock") == 0)
		return new TRollBlock("回転板");

	if (strcmp(name, "RollBlockR") == 0)
		return new TRollBlock("回転板");

	if (strcmp(name, "RollBlockY") == 0)
		return new TRollBlock("回転板");

	if (strcmp(name, "RollBlockB") == 0)
		return new TRollBlock("回転板");

	if (strcmp(name, "Uirou") == 0)
		return new TNormalLift("ノーマルリフト");

	if (strcmp(name, "Castella") == 0)
		return new TNormalLift("ノーマルリフト");

	if (strcmp(name, "Hikidashi") == 0)
		return new TNormalLift("ノーマルリフト");

	if (strcmp(name, "YoshiBlock") == 0)
		return new TWoodBlock("木のブロック");

	if (strcmp(name, "MapObjNail") == 0)
		return new TMapObjNail("くぎ");

	if (strcmp(name, "WoodBlock") == 0)
		return new TWoodBlock("木のブロック");

	if (strcmp(name, "MapObjRevivalPollution") == 0)
		return new TMapObjRevivalPollution("復活落書き");

	if (strcmp(name, "PolluterBase") == 0)
		return new TPolluterBase("汚染オブジェ");

	if (strcmp(name, "Coin") == 0)
		return new TCoin;

	if (strcmp(name, "CoinRed") == 0)
		return new TCoinRed;

	if (strcmp(name, "CoinBlue") == 0)
		return new TCoinBlue;

	if (strcmp(name, "HideObj") == 0)
		return new THideObjBase("隠しオブジェ");

	// TODO:
	// if ( strcmp(name, "WaterHitHideObj") == 0 )
	// 	return new TWaterHitHideObj("水ヒットオブジェ");

	// TODO:
	// if (strcmp(name, "FruitHitHideObj") == 0)
	// 	return new TFruitHitHideObj("水ヒットオブジェ");

	// TODO:
	// if (strcmp(name, "HipDropHideObj") == 0)
	// 	return new THipDropHideObj("ヒップドロップオブジェ");

	// TODO:
	// if (strcmp(name, "HideObjPictureTwin") == 0)
	// 	return new THideObjPictureTwin("ふたご落書き");

	if (strcmp(name, "FlowerCoin") == 0)
		return new TFlowerCoin;

	if (strcmp(name, "Shine") == 0)
		return new TShine;

	if (strcmp(name, "NozzleBox") == 0)
		return new TNozzleBox;

	if (strcmp(name, "ItemNozzle") == 0)
		return new TItemNozzle;

	if (strcmp(name, "Mushroom1up") == 0)
		return new TMushroom1up(0, "１ＵＰキノコ");

	if (strcmp(name, "Mushroom1upR") == 0)
		return new TMushroom1up(1, "１ＵＰキノコ");

	if (strcmp(name, "Mushroom1upX") == 0)
		return new TMushroom1up(2, "１ＵＰキノコ");

	if (strcmp(name, "EggYoshi") == 0)
		return new TEggYoshi("ヨッシーの卵");

	if (strcmp(name, "NormalBlock") == 0)
		return new TMapObjBase("地形オブジェ基底");

	if (strcmp(name, "SuperHipDropBlock") == 0)
		return new TSuperHipDropBlock("スーパーヒップドロップブロック");

	if (strcmp(name, "IceBlock") == 0)
		return new TIceBlock("アイスブロック");

	if (strcmp(name, "BreakableBlock") == 0)
		return new TBreakableBlock;

	if (strcmp(name, "BrickBlock") == 0)
		return new TBrickBlock("レンガブロック");

	if (strcmp(name, "WatermelonBlock") == 0)
		return new TBreakHideObj("壊れる隠しオブジェ");

	if (strcmp(name, "SandBlock") == 0)
		return new TSandBlock;

	return nullptr;
}
