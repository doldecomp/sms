// This object's .rodata blob opens with the System/DummyStrings.hpp pair, then
// two TU-local pollution-texture names, then the four MActorMtxCalcType
// strings; without all three blocks every string offset in the factory is
// 0x34 or 0xE0 low.
// rogue includes needed for matching the .rodata string pool and __sinit
#include <System/DummyStrings.hpp>

// TODO: retail has these two as (object,local) in MarNameRefGen.cpp,
// MarNameRefGen_BossEnemy.cpp, MarDirectorPreEntry.cpp, emario.cpp,
// enemyMario.cpp, cameragc.cpp, MapObjFence.cpp and every Player TU, i.e. they
// came from a shared Player header rather than from each .cpp (the copies in
// src/Player/*.cpp are open-coded).  Parked here until that header is
// identified, because their 0x34 bytes sit between the DummyStrings pair and
// the MActorMtxCalcType names and shift every later string offset.
static const char cDirtyFileName[] = "/scene/map/pollution/H_ma_rak.bti";
static const char cDirtyTexName[]  = "H_ma_rak_dummy";

#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

#include "Enemy/BathtubKiller.hpp"
#include "Enemy/BossGesso.hpp"
#include "Enemy/CoasterKiller.hpp"
#include "Enemy/Enemy.hpp"
#include "Enemy/EnemyManager.hpp"
#include "Enemy/Hinokuri2.hpp"
#include <Enemy/BathtubPeach.hpp>
#include <Enemy/BossEel.hpp>
#include <Enemy/BossHanachan.hpp>
#include <Enemy/SleepBossHanachan.hpp>
#include <Enemy/BossManta.hpp>
#include <Enemy/BossPakkun.hpp>
#include <Enemy/BossTelesaObj.hpp>
#include <Enemy/BossWanwan.hpp>
#include <Enemy/Emario.hpp>
#include <Enemy/Koopa.hpp>
#include <Enemy/KoopaJr.hpp>
#include <Enemy/LimitKoopa.hpp>
#include <Enemy/LimitKoopaJr.hpp>
#include <Enemy/TinKoopa.hpp>
#include <System/MarNameRefGen.hpp>

// TODO: shared-header items this unit still needs, none of them fixable here.
// (The LimitKoopa.hpp pair is done: sizeof(TLimitKoopa) is 0x1c8, and the nine
// nerves moved to LimitKoopaNerve.hpp, which closed __sinit's 0x6c gap.)
//   Enemy/DemoBossHanachan.hpp  TDemoBossHanachanManager(name, prm) is
//       in-class and its body does mSaveParams = new
//       TDemoBossHanachanSaveParams(prm) -- retail stores 0x54 *before* the
//       TSleepBossHanachanManager vtable, so the allocation belongs to the
//       base constructor, not the derived one (TDemoBossHanachanSaveParams
//       has to move above the manager in that header to be complete there)
//   Enemy/SleepBossHanachan.hpp TSleepBossHanachan's initialiser list also
//       has mShinePosition(0, 0, 0) and mMirrorActor(nullptr), and
//       TSleepBossHanachanManager needs an in-class (name) constructor
//   Enemy/BossEel.hpp           TOilBall is in-class (: TBEelTears(name))
//   JGeometry               retail *calls* TVec3<f>::set<f>(0, 0, 0) for
//       mShinePosition (it is this TU's second map symbol, 0x10); our build
//       expands it. Same family as the other set<f> call-vs-expand cases.
JDrama::TNameRef* TMarNameRefGen::getNameRef_BossEnemy(const char* name) const
{
	if (strcmp(name, "EMario") == 0)
		return new TEMario("マリオモドキ");

	if (strcmp(name, "EMarioManager") == 0)
		return new TEMarioManager("典型敵マネージャ");

	if (strcmp(name, "BossHanachan") == 0)
		return new TBossHanachan("?");

	if (strcmp(name, "BossHanachanManager") == 0)
		return new TBossHanachanManager("?");

	if (strcmp(name, "SleepBossHanachan") == 0)
		return new TSleepBossHanachan("?");

	if (strcmp(name, "SleepBossHanachanManager") == 0)
		return new TSleepBossHanachanManager("?");

	if (strcmp(name, "BossEel") == 0)
		return new TBossEel("?");

	if (strcmp(name, "BossEelManager") == 0)
		return new TBossEelManager("?");

	if (strcmp(name, "BEelTearsManager") == 0)
		return new TBEelTearsManager("めおとウナギ涙マネージャー");

	if (strcmp(name, "Koopa") == 0)
		return new TKoopa;

	if (strcmp(name, "KoopaManager") == 0)
		return new TKoopaManager;

	if (strcmp(name, "HinoKuri2") == 0)
		return new THinokuri2;

	if (strcmp(name, "HinoKuri2Manager") == 0)
		return new THinokuri2Manager;

	if (strcmp(name, "BossGesso") == 0)
		return new TBossGesso;

	if (strcmp(name, "BossGessoManager") == 0)
		return new TBossGessoManager;

	if (strcmp(name, "TinKoopa") == 0)
		return new TTinKoopa("メカクッパ");

	if (strcmp(name, "TinKoopaManager") == 0)
		return new TTinKoopaManager("メカクッパマネージャ");

	if (strcmp(name, "CoasterKillerManager") == 0)
		return new TCoasterKillerManager;

	if (strcmp(name, "CoasterKiller") == 0)
		return new TCoasterKiller;

	if (strcmp(name, "KoopaJrManager") == 0)
		return new TKoopaJrManager("クッパジュニアマネージャー");

	if (strcmp(name, "KoopaJr") == 0)
		return new TKoopaJr("クッパジュニア");

	if (strcmp(name, "KoopaJrSubmarineManager") == 0)
		return new TKoopaJrSubmarineManager("クッパジュニアサブマリンマネージャー");

	if (strcmp(name, "KoopaJrSubmarine") == 0)
		return new TKoopaJrSubmarine("クッパジュニアサブマリン");

	if (strcmp(name, "LimitKoopaJrManager") == 0)
		return new TLimitKoopaJrManager;

	if (strcmp(name, "LimitKoopaJr") == 0)
		return new TLimitKoopaJr;

	if (strcmp(name, "LimitKoopaManager") == 0)
		return new TLimitKoopaManager;

	if (strcmp(name, "LimitKoopa") == 0)
		return new TLimitKoopa;

	if (strcmp(name, "BathtubKillerManager") == 0)
		return new TBathtubKillerManager;

	if (strcmp(name, "BathtubKiller") == 0)
		return new TBathtubKiller;

	if (strcmp(name, "BathtubPeachManager") == 0)
		return new TBathtubPeachManager("バスタブピーチマネージャー");

	if (strcmp(name, "BathtubPeach") == 0)
		return new TBathtubPeach("バスタブピーチ");

	if (strcmp(name, "BossWanwan") == 0)
		return new TBossWanwan;

	if (strcmp(name, "BossWanwanManager") == 0)
		return new TBossWanwanManager("ボスワンワンマネージャ");

	if (strcmp(name, "BossPakkun") == 0)
		return new TBossPakkun;

	if (strcmp(name, "KBossPakkun") == 0)
		return new TBossPakkun("ボスパックン軽");

	if (strcmp(name, "BossPakkunManager") == 0)
		return new TBossPakkunManager;

	if (strcmp(name, "KBossPakkunManager") == 0)
		return new TBossPakkunManager("ボスパックン軽マネージャ", 1);

	if (strcmp(name, "BossTelesa") == 0)
		return new TBossTelesa;

	if (strcmp(name, "BossTelesaManager") == 0)
		return new TBossTelesaManager;

	if (strcmp(name, "BubbleManager") == 0)
		return new TBubbleManager;

	if (strcmp(name, "OilBall") == 0)
		return new TOilBall("油ダマ");

	if (strcmp(name, "BossManta") == 0)
		return new TBossManta;

	if (strcmp(name, "BossMantaManager") == 0)
		return new TBossMantaManager("ボスマンタマネージャ");

	return nullptr;
}
