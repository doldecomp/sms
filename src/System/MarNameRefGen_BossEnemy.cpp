#include <Enemy/BathtubKiller.hpp>
#include <Enemy/BathtubPeach.hpp>
#include <Enemy/BossGesso.hpp>
#include <Enemy/BossHanachan.hpp>
#include <Enemy/BossTelesa.hpp>
#include <Enemy/BossWanwan.hpp>
#include <Enemy/CoasterKiller.hpp>
#include <Enemy/Emario.hpp>
#include <Enemy/BossEel.hpp>
#include <Enemy/BossManta.hpp>
#include <Enemy/BossPakkun.hpp>
#include <Enemy/Hinokuri2.hpp>
#include <Enemy/Koopa.hpp>
#include <Enemy/KoopaJr.hpp>
#include <Enemy/LimitKoopa.hpp>
#include <Enemy/LimitKoopaJr.hpp>
#include <Enemy/SleepBossHanachan.hpp>
#include <Enemy/TinKoopa.hpp>
#include <System/MarNameRefGen.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// rogue includes: the PCH strings at the start of .rodata
#include <System/DummyStrings.hpp>

// mario.MAP: local arrays at .rodata+0x20 and +0x44 in this TU.
// TODO: their original owning header is unknown.
const char cDirtyFileName[] = "/scene/map/pollution/H_ma_rak.bti";
const char cDirtyTexName[]  = "H_ma_rak_dummy";

#include <M3DUtil/InfectiousStrings.hpp>

// TODO: recover the header-inline constructor; its body survives at
// 0x800fddc0 in getNameRef_BossEnemy (mario.MAP has no out-of-line copy).
inline TOilBall::TOilBall(const char* name)
    : TBEelTears(name)
{
}

JDrama::TNameRef* TMarNameRefGen::getNameRef_BossEnemy(const char* name) const
{
	if (strcmp(name, "EMario") == 0)
		return new TEMario;

	if (strcmp(name, "EMarioManager") == 0)
		return new TEMarioManager;

	if (strcmp(name, "BossHanachan") == 0)
		return new TBossHanachan("?");

	if (strcmp(name, "BossHanachanManager") == 0)
		return new TBossHanachanManager("?");

	// TODO: recover mShinePosition's member-construction boundary in
	// SleepBossHanachan.hpp; mario.MAP retains TVec3<f32>::set<f32> at
	// 0x800fdf48, but the current constructor fully inlines it.
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
		return new TKoopa("クッパ");

	if (strcmp(name, "KoopaManager") == 0)
		return new TKoopaManager("クッパマネージャー");

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
		return new TKoopaJrSubmarineManager(
		    "クッパジュニアサブマリンマネージャー");

	if (strcmp(name, "KoopaJrSubmarine") == 0)
		return new TKoopaJrSubmarine("クッパジュニアサブマリン");

	if (strcmp(name, "LimitKoopaJrManager") == 0)
		return new TLimitKoopaJrManager;

	if (strcmp(name, "LimitKoopaJr") == 0)
		return new TLimitKoopaJr;

	if (strcmp(name, "LimitKoopaManager") == 0)
		return new TLimitKoopaManager("クッパマネージャー");

	if (strcmp(name, "LimitKoopa") == 0)
		return new TLimitKoopa("クッパ");

	if (strcmp(name, "BathtubKillerManager") == 0)
		return new TBathtubKillerManager;

	if (strcmp(name, "BathtubKiller") == 0)
		return new TBathtubKiller;

	if (strcmp(name, "BathtubPeachManager") == 0)
		return new TBathtubPeachManager("バスタブピーチマネージャー");

	if (strcmp(name, "BathtubPeach") == 0)
		return new TBathtubPeach("バスタブピーチ");

	if (strcmp(name, "BossWanwan") == 0)
		return new TBossWanwan("ボスワンワン");

	if (strcmp(name, "BossWanwanManager") == 0)
		return new TBossWanwanManager("ボスワンワンマネージャ");

	if (strcmp(name, "BossPakkun") == 0)
		return new TBossPakkun("ボスパックン改");

	if (strcmp(name, "KBossPakkun") == 0)
		return new TBossPakkun("ボスパックン軽");

	if (strcmp(name, "BossPakkunManager") == 0)
		return new TBossPakkunManager("ボスパックンマネージャー", 0);

	if (strcmp(name, "KBossPakkunManager") == 0)
		return new TBossPakkunManager("ボスパックン軽マネージャ", 1);

	if (strcmp(name, "BossTelesa") == 0)
		return new TBossTelesa("ボステレサ");

	if (strcmp(name, "BossTelesaManager") == 0)
		return new TBossTelesaManager("ボステレサマネージャー");

	if (strcmp(name, "BubbleManager") == 0)
		return new TBubbleManager("バブルマネージャー");

	if (strcmp(name, "OilBall") == 0)
		return new TOilBall;

	if (strcmp(name, "BossManta") == 0)
		return new TBossManta;

	if (strcmp(name, "BossMantaManager") == 0)
		return new TBossMantaManager;

	return nullptr;
}
