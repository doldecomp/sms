#include <System/MarNameRefGen.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <Enemy/EffectObj.hpp>
#include <Enemy/EggGen.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/Gesso.hpp>
#include <Enemy/HamuKuri.hpp>
#include <Enemy/Igaiga.hpp>
#include <Enemy/Launcher.hpp>
#include <Enemy/MameGesso.hpp>
#include <Enemy/NameKuri.hpp>
#include <Enemy/PoiHana.hpp>
#include <Enemy/RiccoHook.hpp>
#include <Enemy/Rocket.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/TamaNoko.hpp>
#include <Enemy/Telesa.hpp>
#include <Enemy/DebuTelesa.hpp>
#include <Enemy/TypicalEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <Enemy/FireWanwan.hpp>
#include <Enemy/GateKeeper.hpp>
#include <Enemy/beam.hpp>
#include <Enemy/Amenbo.hpp>
#include <Enemy/Kumokun.hpp>
#include <Animal/AnimalBase.hpp>
#include <Animal/AnimalManager.hpp>
#include <Animal/Butterfly.hpp>
#include <Animal/fishoid.hpp>

#define DECL_ENEMY(name) class name : public TTypicalEnemy { public: name(const char*); };
#define DECL_MANAGER(name) class name : public TEnemyManager { public: name(const char*); };
DECL_ENEMY(TBeeHive) DECL_MANAGER(TBeeHiveManager)
DECL_ENEMY(TElecNokonoko) DECL_MANAGER(TElecNokonokoManager)
DECL_MANAGER(TChuuHanaManager)
DECL_ENEMY(TSeal) DECL_MANAGER(TSealManager)
DECL_ENEMY(TKukku) DECL_MANAGER(TKukkuManager)
DECL_ENEMY(TKazekun) DECL_MANAGER(TKazekunManager)
DECL_ENEMY(TPakkun) DECL_MANAGER(TPakkunManager)
DECL_ENEMY(THanaSambo) DECL_MANAGER(THanaSamboManager)
DECL_ENEMY(TSamboHead) DECL_MANAGER(TSamboHeadManager)
DECL_ENEMY(TYumbo) DECL_MANAGER(TYumboManager)
DECL_ENEMY(TSamboFlower)
DECL_ENEMY(TPopo) DECL_MANAGER(TPopoManager)
DECL_ENEMY(TTobiPukuLaunchPad)
DECL_MANAGER(TTobiPukuManager)
DECL_ENEMY(TPukuPuku)
DECL_ENEMY(TTobiPuku)
DECL_MANAGER(TTabePukuManager)
DECL_ENEMY(TTabePuku)
DECL_ENEMY(TWireTrap) DECL_MANAGER(TWireTrapManager)
DECL_ENEMY(TCannon) DECL_MANAGER(TCannonManager)
DECL_ENEMY(TBombHei) DECL_MANAGER(TBombHeiManager)
DECL_ENEMY(TKiller) DECL_MANAGER(TKillerManager)
DECL_ENEMY(TAmiNoko) DECL_MANAGER(TAmiNokoManager)
#undef DECL_ENEMY
#undef DECL_MANAGER

class TFruitsBoat : public TTypicalEnemy {
public:
	TFruitsBoat(const char*);
};

class TFruitsBoatManager : public TEnemyManager {
public:
	TFruitsBoatManager(int, const char*);
};

class TAnimalBird : public TAnimalBase {
public:
	TAnimalBird(const char*);
};

class TAnimalBirdManager : public TAnimalManagerBase {
public:
	TAnimalBirdManager(const char*);
};

class THamuKuriLauncherManager : public TLauncherManager {
public:
	THamuKuriLauncherManager(const char* name)
	    : TLauncherManager(name)
	{
	}
};

class TNameKuriLauncherManager : public TLauncherManager {
public:
	TNameKuriLauncherManager(const char* name)
	    : TLauncherManager(name)
	{
	}
};

class TTobiPukuLaunchPadManager : public TSmallEnemyManager {
public:
	TTobiPukuLaunchPadManager(const char*);
};

class TRocketManager : public TEnemyManager {
public:
	TRocketManager(const char*);
};

class THauntLegManager : public TSmallEnemyManager {
public:
	THauntLegManager(const char*);
};

class TStayPakkun : public TTypicalEnemy {
public:
	TStayPakkun(const char* name)
	    : TTypicalEnemy(name)
	{
	}
};

class TSamboFlowerManager : public TEnemyManager {
public:
	TSamboFlowerManager(const char* name)
	    : TEnemyManager(name)
	{
	}
};

class TEffectEnemy : public TTypicalEnemy {
public:
	TEffectEnemy(const char*);
};

class TEffectEnemyManager : public TEnemyManager {
public:
	TEffectEnemyManager(const char* name)
	    : TEnemyManager(name)
	{
	}
};

inline TMewManager::TMewManager(const char* name)
    : TAnimalManagerBase(name)
{
}

inline TDiffusionNameKuriManager::TDiffusionNameKuriManager(const char* name)
    : TNameKuriManager(name)
{
}

inline TSurfGesso::TSurfGesso(const char* name)
    : TGesso(name)
{
}

inline TLandGesso::TLandGesso(const char* name)
    : TGesso(name)
{
}

inline TNameKuriLauncher::TNameKuriLauncher(const char* name)
    : TLauncher(name)
{
}

inline TSimpleEffect::TSimpleEffect(const char* name)
    : JDrama::TActor(name)
    , unk44(1)
{
}

inline TEffectPinnaFunsui::TEffectPinnaFunsui(const char* name)
    : TSimpleEffect(name)
{
}

inline TEffectBiancoFunsui::TEffectBiancoFunsui(const char* name)
    : TSimpleEffect(name)
{
}

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

	if (strcmp(name, "FruitsBoat") == 0)
		return new TFruitsBoat("フルーツ運搬船");

	if (strcmp(name, "FruitsBoatB") == 0)
		return new TFruitsBoat("フルーツ運搬船");

	if (strcmp(name, "FruitsBoatC") == 0)
		return new TFruitsBoat("フルーツ運搬船");

	if (strcmp(name, "FruitsBoatD") == 0)
		return new TFruitsBoat("フルーツ運搬船");

	if (strcmp(name, "FruitsBoatManager") == 0)
		return new TFruitsBoatManager(0, "フルーツ運搬船マネージャ");

	if (strcmp(name, "FruitsBoatManagerB") == 0)
		return new TFruitsBoatManager(1, "フルーツ運搬船マネージャ");

	if (strcmp(name, "FruitsBoatManagerC") == 0)
		return new TFruitsBoatManager(2, "フルーツ運搬船マネージャ");

	if (strcmp(name, "FruitsBoatManagerD") == 0)
		return new TFruitsBoatManager(3, "フルーツ運搬船マネージャ");

	if (strcmp(name, "AnimalMew") == 0)
		return new TAnimalBase(0x80001);

	if (strcmp(name, "MewManager") == 0)
		return new TMewManager;

	if (strcmp(name, "AnimalBird") == 0)
		return new TAnimalBird("(幸せの青い)鳥");

	if (strcmp(name, "AnimalBirdManager") == 0)
		return new TAnimalBirdManager("(幸せの青い)鳥マネージャー");

	if (strcmp(name, "FishoidA") == 0)
		return new TFishoid(0, "回遊魚Ａ");

	if (strcmp(name, "FishoidB") == 0)
		return new TFishoid(1, "回遊魚Ｂ");

	if (strcmp(name, "FishoidC") == 0)
		return new TFishoid(2, "回遊魚Ｃ");

	if (strcmp(name, "FishoidD") == 0)
		return new TFishoid(3, "回遊魚Ｄ");

	if (strcmp(name, "FishoidManager") == 0)
		return new TFishoidManager;

	if (strcmp(name, "BeeHive") == 0)
		return new TBeeHive("ハチの巣とハチ");

	if (strcmp(name, "BeeHiveManager") == 0)
		return new TBeeHiveManager("ハチの巣マネージャー");

	if (strcmp(name, "Butterfly") == 0)
		return new TButterfloid(0, "蝶Ａ");

	if (strcmp(name, "ButterflyB") == 0)
		return new TButterfloid(1, "蝶Ｂ");

	if (strcmp(name, "ButterflyC") == 0)
		return new TButterfloid(2, "蝶Ｃ");

	if (strcmp(name, "ButterflyManager") == 0)
		return new TButterfloidManager("ButterflyManager");

	if (strcmp(name, "EggGenerator") == 0)
		return new TEggGenerator("タマゴジェネレータ");

	if (strcmp(name, "WickedEggGenerator") == 0)
		return new TEggGenerator("タマゴジェネレータ");

	if (strcmp(name, "EggGenManager") == 0)
		return new TEggGenManager("タマゴジェネレータマネージャ");

	if (strcmp(name, "EffectColumWaterManager") == 0)
		return new TEffectColumWaterManager;

	if (strcmp(name, "EffectBombColumWaterManager") == 0)
		return new TEffectBombColumWaterManager;

	if (strcmp(name, "EffectColumSandManager") == 0)
		return new TEffectColumSandManager;

	if (strcmp(name, "EffectExplosionManager") == 0)
		return new TEffectExplosionManager;

	if (strcmp(name, "EffectEnemyManager") == 0)
		return new TEffectEnemyManager("エフェクト敵マネージャー");

	if (strcmp(name, "EffectEnemy") == 0)
		return new TEffectEnemy("エフェクト敵");

	if (strcmp(name, "HauntLegManager") == 0)
		return new THauntLegManager("ハントレッグマネージャー");

	if (strcmp(name, "HamuKuriManager") == 0)
		return new THamuKuriManager;

	if (strcmp(name, "HamuKuri") == 0)
		return new THamuKuri;

	if (strcmp(name, "HaneHamuKuriManager") == 0)
		return new THaneHamuKuriManager("はねハムクリマネージャー");

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

	if (strcmp(name, "PakkunManager") == 0)
		return new TPakkunManager("パックンマネージャー");

	if (strcmp(name, "Pakkun") == 0)
		return new TPakkun("パックン");

	if (strcmp(name, "StayPakkun") == 0)
		return new TStayPakkun("固定パックン");

	if (strcmp(name, "HanaSamboManager") == 0)
		return new THanaSamboManager("ハナサンボマネージャー");

	if (strcmp(name, "HanaSambo") == 0)
		return new THanaSambo("ハナサンボ");

	if (strcmp(name, "SamboHeadManager") == 0)
		return new TSamboHeadManager("サンボヘッドマネージャー");

	if (strcmp(name, "SamboHead") == 0)
		return new TSamboHead("サンボヘッド");

	if (strcmp(name, "YumboManager") == 0)
		return new TYumboManager("ユンボマネージャー");

	if (strcmp(name, "SamboFlowerManager") == 0)
		return new TSamboFlowerManager("サンボフラワーマネージャー");

	if (strcmp(name, "SamboFlower") == 0)
		return new TSamboFlower("サンボフラワー");

	if (strcmp(name, "PopoManager") == 0)
		return new TPopoManager("ポポマネージャー");

	if (strcmp(name, "GorogoroManager") == 0)
		return new TEnemyManager("ゴロゴロマネージャー");

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

	if (strcmp(name, "ElecNokonokoManager") == 0)
		return new TElecNokonokoManager("電気ノコノコマネージャー");

	if (strcmp(name, "ElecNokonoko") == 0)
		return new TElecNokonoko("電気ノコノコ");

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

	if (strcmp(name, "DebuTelesaManager") == 0)
		return new TDebuTelesaManager("デブテルサマネージャー");

	if (strcmp(name, "DebuTelesa") == 0)
		return new TDebuTelesa("デブテルサ");

	if (strcmp(name, "PoiHanaManager") == 0)
		return new TPoiHanaManager;

	if (strcmp(name, "PoiHana") == 0)
		return new TPoiHana;

	if (strcmp(name, "PoiHanaRed") == 0)
		return new TPoiHanaRed;

	if (strcmp(name, "SleepPoiHana") == 0)
		return new TSleepPoiHana;

	if (strcmp(name, "ChuuHanaManager") == 0)
		return new TChuuHanaManager("チュウハナマネージャー");

	if (strcmp(name, "FireWanwanManager") == 0)
		return new TFireWanwanManager;

	if (strcmp(name, "FireWanwan") == 0)
		return new TFireWanwan;

	if (strcmp(name, "TobiPukuLaunchPadManager") == 0)
		return new TTobiPukuLaunchPadManager("とびプク発射台マネージャー");

	if (strcmp(name, "TobiPukuLaunchPad") == 0)
		return new TTobiPukuLaunchPad("とびプク発射台");

	if (strcmp(name, "MoePukuLaunchPadManager") == 0)
		return new TTobiPukuLaunchPadManager("モエプク発射台マネージャー");

	if (strcmp(name, "MoePukuLaunchPad") == 0)
		return new TTobiPukuLaunchPad("モエプク発射台");

	if (strcmp(name, "TobiPukuManager") == 0)
		return new TTobiPukuManager("とびプクマネージャー");

	if (strcmp(name, "MoePukuManager") == 0)
		return new TTobiPukuManager("モエプクマネージャー");

	if (strcmp(name, "PukuPuku") == 0)
		return new TPukuPuku("プクプク");

	if (strcmp(name, "MoePuku") == 0)
		return new TTobiPuku("モエプク");

	if (strcmp(name, "TabePukuManager") == 0)
		return new TTabePukuManager("プクプク(レール巡回)マネージャー");

	if (strcmp(name, "TabePuku") == 0)
		return new TTabePuku("プクプク(レール巡回)");

	if (strcmp(name, "WireTrapManager") == 0)
		return new TWireTrapManager("電線トラップマネージャー");

	if (strcmp(name, "WireTrap") == 0)
		return new TWireTrap("電線トラップ");

	if (strcmp(name, "RocketManager") == 0)
		return new TRocketManager("ロケットマネージャー");

	if (strcmp(name, "Rocket") == 0)
		return new TRocket;

	if (strcmp(name, "Yumbo") == 0)
		return new TYumbo("ユンボ");

	if (strcmp(name, "CannonManager") == 0)
		return new TCannonManager("砲台マネージャー");

	if (strcmp(name, "Cannon") == 0)
		return new TCannon("砲台");

	if (strcmp(name, "BombHeiManager") == 0)
		return new TBombHeiManager("ボム兵マネージャー");

	if (strcmp(name, "BombHei") == 0)
		return new TBombHei("ボム兵");

	if (strcmp(name, "KillerManager") == 0)
		return new TKillerManager("キラーマネージャー");

	if (strcmp(name, "Killer") == 0)
		return new TKiller("キラー");

	if (strcmp(name, "AmiNokoManager") == 0)
		return new TAmiNokoManager("アミノコマネージャー");

	if (strcmp(name, "AmiNoko") == 0)
		return new TAmiNoko("アミノコ");

	if (strcmp(name, "KageMarioModokiManager") == 0)
		return new TKageMarioModokiManager;

	if (strcmp(name, "KageMarioModoki") == 0)
		return new TKageMarioModoki;

	if (strcmp(name, "CommonLauncher") == 0)
		return new TCommonLauncher;

	if (strcmp(name, "CommonLauncherManager") == 0)
		return new TCommonLauncherManager;

	if (strcmp(name, "GateKeeper") == 0)
		return new TBiancoGateKeeper("ビアンコゲートキーパー");

	if (strcmp(name, "GateKeeperManager") == 0)
		return new TBiancoGateKeeperManager("ゲートキーパーマネージャー");

	if (strcmp(name, "OrangeSeal") == 0)
		return new TSeal("OrangeSeal");

	if (strcmp(name, "SealManager") == 0)
		return new TSealManager("シールマネージャ");

	if (strcmp(name, "HamukuriLauncher") == 0)
		return new THamuKuriLauncher;

	if (strcmp(name, "HamukuriLauncherManager") == 0)
		return new THamuKuriLauncherManager("ハムクリランチャーマネージャー");

	if (strcmp(name, "NamekuriLauncher") == 0)
		return new TNameKuriLauncher;

	if (strcmp(name, "NamekuriLauncherManager") == 0)
		return new TNameKuriLauncherManager("ナメクリランチャーマネージャー");

	if (strcmp(name, "IgaigaManager") == 0)
		return new TIgaigaManager("イガイガマネージャー");

	if (strcmp(name, "TamaNokoManager") == 0)
		return new TTamaNokoManager;

	if (strcmp(name, "TamaNoko") == 0)
		return new TTamaNoko;

	if (strcmp(name, "BeamManager") == 0) {
		gpBeamManager = new TBeamManager;
		return gpBeamManager;
	}

	if (strcmp(name, "KukkuManager") == 0)
		return new TKukkuManager("クックマネージャー");

	if (strcmp(name, "Kukku") == 0)
		return new TKukku("Kukku");

	if (strcmp(name, "KumokunManager") == 0)
		return new TKumokunManager("くもくんマネージャー");

	if (strcmp(name, "Kumokun") == 0)
		return new TKumokun;

	if (strcmp(name, "AmenboManager") == 0)
		return new TAmenboManager;

	if (strcmp(name, "Amenbo") == 0)
		return new TAmenbo;

	if (strcmp(name, "KazekunManager") == 0)
		return new TKazekunManager("かぜくんマネージャ");

	if (strcmp(name, "Kazekun") == 0)
		return new TKazekun("かぜくん");

	if (strcmp(name, "EffectPinnaFunsui") == 0)
		return new TEffectPinnaFunsui;

	if (strcmp(name, "EffectBiancoFunsui") == 0)
		return new TEffectBiancoFunsui;

	return nullptr;
}
