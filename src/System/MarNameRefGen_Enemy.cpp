#include "Animal/AnimalBase.hpp"
#include "Animal/AnimalManager.hpp"
#include "Enemy/EffectObj.hpp"
#include "Enemy/EnemyManager.hpp"
#include "Enemy/Gesso.hpp"
#include "Enemy/HamuKuri.hpp"
#include "Enemy/Launcher.hpp"
#include "Enemy/MameGesso.hpp"
#include "Enemy/NameKuri.hpp"
#include "Enemy/PoiHana.hpp"
#include "Enemy/RiccoHook.hpp"
#include "Enemy/Rocket.hpp"
#include "Enemy/SmallEnemy.hpp"
#include "Enemy/TamaNoko.hpp"
#include "Enemy/Telesa.hpp"
#include "Enemy/TypicalEnemy.hpp"
#include "Enemy/WalkerEnemy.hpp"
#include <System/MarNameRefGen.hpp>

extern TTypicalEnemy* gpBeamManager; // killer.hpp

JDrama::TNameRef* TMarNameRefGen::getNameRef_Enemy(const char* name) const
{

	if (strcmp(name, "TypicalEnemy") == 0)
		return new TTypicalEnemy;

	if (strcmp(name, "TypicalManager") == 0)
		return new TTypicalManager;

	if (strcmp(name, "RiccoHook") == 0)
		return new TRiccoHook;

	if (strcmp(name, "RiccoHookManager") == 0)
		return new TRiccoHookManager;

	if (strcmp(name, "EnemyManager") == 0)
		return new TEnemyManager;

	// TODO:
	// if (strcmp(name, "FruitsBoat") == 0 || strcmp(name, "FruitsBoatG") == 0
	// ||
	//     strcmp(name, "FruitsBoatP") == 0 || strcmp(name, "FruitsBoatY") == 0)
	//     return new TFruitsBoat("フルーツの船");

	// TODO:
	// if (strcmp(name, "FruitsBoatManager") == 0)
	// 	return new TFruitsBoatManager(0, "フルーツの船マネージャー");

	// TODO:
	// if (strcmp(name, "FruitsBoatGManager") == 0)
	// 	return new TFruitsBoatManager(1, "フルーツの船マネージャー");

	// TODO:
	// if (strcmp(name, "FruitsBoatPManager") == 0)
	// 	return new TFruitsBoatManager(2, "フルーツの船マネージャー");

	// TODO:
	// if (strcmp(name, "FruitsBoatYManager") == 0)
	//     return new TFruitsBoatManager(3, "フルーツの船マネージャー");

	if (strcmp(name, "AnimalMew") == 0)
		return new TAnimalBase(0x80001);

	if (strcmp(name, "MewManager") == 0)
		return new TMewManager;

	// TODO:
	// if ( strcmp(name, "AnimalBird") == 0)
	//     return new TAnimalBird;

	// TODO:
	// if ( strcmp(name, "AnimalBirdManager") == 0)
	//     return new TAnimalBirdManager;

	// TODO:
	// if ( strcmp(name, "FishoidA") == 0)
	//     return new TFishoid;

	// TODO:
	// if ( strcmp(name, "FishoidB") == 0)
	//     return new TFishoid;

	// TODO:
	// if ( strcmp(name, "FishoidC") == 0)
	//     return new TFishoid;

	// TODO:
	// if ( strcmp(name, "FishoidD") == 0)
	//     return new TFishoid;

	// TODO:
	// if ( strcmp(name, "FishoidManager") == 0)
	//     return new TFishoidManager;

	// TODO:
	// if ( strcmp( name, "BeeHive" ) == 0 )
	//     return new TBeeHive;

	// TODO:
	// if ( strcmp(name, "BeeHiveManager") == 0)
	//     return new TBeeHiveManager;

	// TODO:
	// if ( strcmp(name, "Butterfly") == 0)
	//     return new TButterfloid;

	// TODO:
	// if ( strcmp(name, "ButterflyB") == 0)
	//     return new TButterfloid;

	// TODO:
	// if ( strcmp(name, "ButterflyC") == 0)
	//     return new TButterfloid;

	// TODO:
	// if ( strcmp(name, "ButterflyManager") == 0)
	//     return new TButterfloidManager;

	// TODO:
	// if ( strcmp(name, "EggGenerator") == 0)
	//     return new TEggGenerator;

	// TODO:
	// if ( strcmp(name, "WickedEggGenerator") == 0)
	//     return new TEggGenerator;

	// TODO:
	// if ( strcmp(name, "EggGenManager") == 0)
	//     return new TEggGenManager;

	if (strcmp(name, "EffectColumWaterManager") == 0)
		return new TEffectColumWaterManager;

	if (strcmp(name, "EffectBombColumWaterManager") == 0)
		return new TEffectBombColumWaterManager;

	if (strcmp(name, "EffectColumSandManager") == 0)
		return new TEffectColumSandManager;

	if (strcmp(name, "EffectExplosionManager") == 0)
		return new TEffectExplosionManager;

	// TODO:
	// if (strcmp(name, "EffectEnemyManager") == 0)
	// 	return new TEffectEnemyManager("エネミーマネージャー");

	// TODO:
	// if ( strcmp(name, "EffectEnemy") == 0)
	//     return new TEffectEnemy("エフェクト敵");

	// TODO:
	// if ( strcmp(name, "HauntLegManager") == 0)
	//     return new THauntLegManager;

	if (strcmp(name, "HamuKuriManager") == 0)
		return new THamuKuriManager;

	if (strcmp(name, "HamuKuri") == 0)
		return new THamuKuri;

	if (strcmp(name, "HaneHamuKuriManager") == 0)
		return new THaneHamuKuriManager;

	if (strcmp(name, "HaneHamuKuri") == 0)
		return new THaneHamuKuri;

	if (strcmp(name, "HaneHamuKuri2") == 0)
		return new THaneHamuKuri2;

	if (strcmp(name, "DoroHaneKuriManager") == 0)
		return new TDoroHaneKuriManager;

	if (strcmp(name, "DoroHaneKuri") == 0)
		return new TDoroHaneKuri;

	if (strcmp(name, "DangoHamuKuriManager") == 0)
		return new TDangoHamuKuriManager;

	if (strcmp(name, "BossDangoHamuKuriManager") == 0)
		return new TBossDangoHamuKuriManager;

	if (strcmp(name, "BossDangoHamuKuri") == 0)
		return new TBossDangoHamuKuri;

	if (strcmp(name, "FireHamuKuriManager") == 0)
		return new TFireHamuKuriManager;

	if (strcmp(name, "FireHamuKuri") == 0)
		return new TFireHamuKuri;

	if (strcmp(name, "DoroHamuKuriManager") == 0)
		return new TDoroHamuKuriManager;

	if (strcmp(name, "DoroHamuKuri") == 0)
		return new TDoroHamuKuri;

	if (strcmp(name, "NameKuriManager") == 0)
		return new TNameKuriManager;

	if (strcmp(name, "NameKuri") == 0)
		return new TNameKuri;

	if (strcmp(name, "DiffusionNameKuriManager") == 0)
		return new TDiffusionNameKuriManager;

	// TODO:
	// if ( strcmp(name, "PakkunManager") == 0)
	//     return new TPakkunManager;

	// TODO:
	// if ( strcmp( name, "Pakkun" ) == 0 )
	//     return new TPakkun;

	// TODO:
	// if ( strcmp(name, "StayPakkun") == 0)
	//     return new TStayPakkun;

	// TODO:
	// if ( strcmp(name, "HanaSamboManager") == 0)
	//     return new THanaSamboManager;

	// TODO:
	// if ( strcmp(name, "HanaSambo") == 0)
	//     return new THanaSambo;

	// TODO:
	// if ( strcmp(name, "SamboHeadManager") == 0)
	//     return new TSamboHeadManager;

	// TODO:
	// if ( strcmp(name, "SamboHead") == 0)
	//     return new TSamboHead;

	// TODO:
	// if ( strcmp(name, "YumboManager") == 0)
	//     return new TYumboManager;

	// TODO:
	// if (strcmp(name, "SamboFlowerManager") == 0)
	// 	return new TSamboFlowerManager("サンボフラワーマネージャー");

	// TODO:
	// if ( strcmp(name, "SamboFlower") == 0)
	//     return new TSamboFlower;

	// TODO:
	// if ( strcmp(name, "PopoManager") == 0)
	//     return new TPopoManager;

	// TODO:
	// if ( strcmp( name, "Popo" ) == 0 )
	//     return new TPopo;

	// TODO:
	// if ( strcmp(name, "GorogoroManager") == 0)
	//     return new TGorogoroManager;

	if (strcmp(name, "GessoManager") == 0)
		return new TGessoManager;

	if (strcmp(name, "Gesso") == 0)
		return new TGesso;

	if (strcmp(name, "SurfGesso") == 0)
		return new TSurfGesso;

	if (strcmp(name, "LandGesso") == 0)
		return new TLandGesso;

	if (strcmp(name, "MameGessoManager") == 0)
		return new TMameGessoManager;

	if (strcmp(name, "MameGesso") == 0)
		return new TMameGesso;

	// TODO:
	// if ( strcmp(name, "ElecNokonokoManager") == 0)
	//     return new TElecNokonokoManager;

	// TODO:
	// if ( strcmp(name, "ElecNokonoko") == 0)
	//     return new TElecNokonoko;

	if (strcmp(name, "TelesaManager") == 0)
		return new TTelesaManager;

	if (strcmp(name, "Telesa") == 0)
		return new TTelesa;

	if (strcmp(name, "SeeTelesa") == 0)
		return new TSeeTelesa;

	if (strcmp(name, "LoopTelesa") == 0)
		return new TLoopTelesa;

	if (strcmp(name, "BoxTelesa") == 0)
		return new TBoxTelesa;

	if (strcmp(name, "MarioModokiTelesa") == 0)
		return new TMarioModokiTelesa;

	// TODO:
	// if ( strcmp(name, "DebuTelesaManager") == 0)
	//     return new TDebuTelesaManager("デブテルサマネージャー");

	// TODO:
	// if ( strcmp(name, "DebuTelesa") == 0)
	//     return new TDebuTelesa;

	if (strcmp(name, "PoiHanaManager") == 0)
		return new TPoiHanaManager;

	if (strcmp(name, "PoiHana") == 0)
		return new TPoiHana;

	if (strcmp(name, "PoiHanaRed") == 0)
		return new TPoiHana;

	if (strcmp(name, "SleepPoiHana") == 0)
		return new TSleepPoiHana;

	// TODO:
	// if ( strcmp(name, "ChuuHanaManager") == 0)
	//     return new TChuuHanaManager;

	// TODO:
	// if ( strcmp(name, "FireWanwanManager") == 0)
	//     return new TFireWanwanManager;

	// TODO:
	// if ( strcmp(name, "FireWanwan") == 0)
	//     return new TFireWanwan;

	// TODO:
	// if ( strcmp(name, "TobiPukuLaunchPadManager") == 0)
	//     return new TTobiPukuLaunchPadManager;

	// TODO:
	// if ( strcmp(name, "TobiPukuLaunchPad") == 0)
	//     return new TTobiPukuLaunchPad;

	// TODO:
	// if ( strcmp(name, "MoePukuLaunchPadManager") == 0)
	//     return new TTobiPukuLaunchPadManager;

	// TODO:
	// if ( strcmp(name, "MoePukuLaunchPad") == 0)
	//     return new TTobiPukuLaunchPad;

	// TODO:
	// if ( strcmp(name, "TobiPukuManager") == 0)
	//     return new TTobiPukuManager;

	// TODO:
	// if ( strcmp(name, "MoePukuManager") == 0)
	//     return new TTobiPukuManager;

	// TODO:
	// if ( strcmp(name, "PukuPuku") == 0)
	//     return new TPukuPuku;

	// TODO:
	// if ( strcmp( name, "MoePuku" ) == 0 )
	//     return new TTobiPuku;

	// TODO:
	// if ( strcmp(name, "TabePukuManager") == 0)
	//     return new TTabePukuManager;

	// TODO:
	// if ( strcmp(name, "TabePuku") == 0)
	//     return new TTabePuku;

	// TODO:
	// if ( strcmp(name, "WireTrapManager") == 0)
	//     return new TWireTrapManager;

	// TODO:
	// if ( strcmp(name, "WireTrap") == 0)
	//     return new TWireTrap;

	// TODO:
	// if ( strcmp(name, "RocketManager") == 0)
	//     return new TRocketManager;

	if (strcmp(name, "Rocket") == 0)
		return new TRocket;

	// TODO:
	// if ( strcmp( name, "Yumbo" ) == 0 )
	//     return new TYumbo;

	// TODO:
	// if ( strcmp(name, "CannonManager") == 0)
	//     return new TCannonManager;

	// TODO:
	// if ( strcmp( name, "Cannon" ) == 0 )
	//     return new TCannon;

	// TODO:
	// if ( strcmp(name, "BombHeiManager") == 0)
	//     return new TBombHeiManager;

	// TODO:
	// if ( strcmp( name, "BombHei" ) == 0 )
	//     return new TBombHei;

	// TODO:
	// if ( strcmp(name, "KillerManager") == 0)
	//     return new TKillerManager;

	// TODO:
	// if ( strcmp( name, "Killer" ) == 0 )
	//     return new TKiller;

	// TODO:
	// if ( strcmp(name, "AmiNokoManager") == 0)
	//     return new TAmiNokoManager;

	// TODO:
	// if ( strcmp( name, "AmiNoko" ) == 0 )
	//     return new TAmiNoko;

	if (strcmp(name, "KageMarioModokiManager") == 0)
		return new TKageMarioModokiManager;

	if (strcmp(name, "KageMarioModoki") == 0)
		return new TKageMarioModoki;

	if (strcmp(name, "CommonLauncher") == 0)
		return new TCommonLauncher;

	if (strcmp(name, "CommonLauncherManager") == 0)
		return new TCommonLauncherManager;

	// TODO:
	// if ( strcmp(name, "GateKeeper") == 0)
	//     return new TBiancoGateKeeper;

	// TODO:
	// if ( strcmp(name, "GateKeeperManager") == 0)
	//     return new TBiancoGateKeeperManager;

	// TODO:
	// if ( strcmp(name, "OrangeSeal") == 0)
	//     return new TSeal;

	// TODO:
	// if ( strcmp(name, "SealManager") == 0)
	//     return new TSealManager;

	if (strcmp(name, "HamukuriLauncher") == 0)
		return new THamuKuriLauncher;

	// TODO:
	// if (strcmp(name, "HamukuriLauncherManager") == 0)
	// 	return new THamukuriLauncherManager("ハムクリランチャーマネージャー");

	if (strcmp(name, "NamekuriLauncher") == 0)
		return new TNameKuriLauncher;

	// TODO:
	// if (strcmp(name, "NamekuriLauncherManager") == 0)
	// 	return new TNamekuriLauncherManager("ナメクリランチャーマネージャー");

	// if ( strcmp(name, "IgaigaManager") == 0)
	//     return new TIgaigaManager;

	if (strcmp(name, "TamaNokoManager") == 0)
		return new TTamaNokoManager;

	if (strcmp(name, "TamaNoko") == 0)
		return new TTamaNoko;

	// TODO:
	// if ( strcmp(name, "BeamManager") == 0) {
	//     gpBeamManager = new TBeamManager;
	//     return gpBeamManager;
	// }

	// TODO:
	// if ( strcmp( name, "KukkuManager" ) == 0 )
	//     return new TKukkuManager;

	// TODO:
	// if ( strcmp( name, "Kukku" ) == 0 )
	//     return new TKukku;

	// TODO:
	// if ( strcmp( name, "KumokunManager" ) == 0 )
	//     return new TKumokunManager;

	// TODO:
	// if ( strcmp( name, "Kumokun" ) == 0 )
	//     return new TKumokun;

	// TODO:
	// if ( strcmp( name, "AmenboManager" ) == 0 )
	//     return new TAmenboManager;

	// TODO:
	// if ( strcmp( name, "Amenbo" ) == 0 )
	//     return new TAmenbo;

	// TODO:
	// if ( strcmp( name, "KazekunManager" ) == 0 )
	//     return new TKazekunManager;

	// TODO:
	// if ( strcmp( name, "Kazekun" ) == 0 )
	//     return new TKazekun;

	if (strcmp(name, "EffectPinnaFunsui") == 0)
		return new TEffectPinnaFunsui;

	if (strcmp(name, "EffectBiancoFunsui") == 0)
		return new TEffectBiancoFunsui;

	return nullptr;
}
