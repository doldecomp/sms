#include <Enemy/BathtubKiller.hpp>
#include <Enemy/BossGesso.hpp>
#include <Enemy/CoasterKiller.hpp>
#include <Enemy/Emario.hpp>
#include <Enemy/BossEel.hpp>
#include <Enemy/BossManta.hpp>
#include <Enemy/Hinokuri2.hpp>
#include <System/MarNameRefGen.hpp>

JDrama::TNameRef* TMarNameRefGen::getNameRef_BossEnemy(const char* name) const
{
	if (strcmp(name, "EMario") == 0)
		return new TEMario;

	if (strcmp(name, "EMarioManager") == 0)
		return new TEMarioManager;

	// TODO:
	// if ( strcmp( name, "BossHanachan" ) == 0 )
	//     return new TBossHanachan("?");

	// TODO:
	// if ( strcmp( name, "BossHanachanManager" ) == 0 )
	//     return new TBossHanachanManager("?");

	// TODO:
	// if (strcmp(name, "SleepBossHanachan") == 0)
	// 	return new TSleepBossHanachan("?");

	// TODO:
	// if (strcmp(name, "SleepBossHanachanManager") == 0)
	// 	return new TDemoBossHanachanManager("?");

	if (strcmp(name, "BossEel") == 0)
		return new TBossEel;

	if (strcmp(name, "BossEelManager") == 0)
		return new TBossEelManager("?");

	if (strcmp(name, "BEelTearsManager") == 0)
		return new TBEelTearsManager("めおとウナギ涙マネージャー");

	// TODO:
	// if ( strcmp( name, "Koopa" ) == 0 )
	//     return new TKoopa("クッパ");

	// TODO:
	// if ( strcmp( name, "KoopaManager" ) == 0 )
	//     return new TKoopaManager("クッパマネージャー");

	if (strcmp(name, "HinoKuri2") == 0)
		return new THinokuri2;

	if (strcmp(name, "HinoKuri2Manager") == 0)
		return new THinokuri2Manager;

	if (strcmp(name, "BossGesso") == 0)
		return new TBossGesso;

	if (strcmp(name, "BossGessoManager") == 0)
		return new TBossGessoManager;

	// TODO:
	// if ( strcmp( name, "TinKoopa" ) == 0 )
	//     return new TTinKoopa("メカクッパ");

	// TODO:
	// if ( strcmp( name, "TinKoopaManager" ) == 0 )
	//     return new TTinKoopaManager("メカクッパマネージャ");

	if (strcmp(name, "CoasterKillerManager") == 0)
		return new TCoasterKillerManager;

	if (strcmp(name, "CoasterKiller") == 0)
		return new TCoasterKiller;

	// TODO:
	// if ( strcmp( name, "KoopaJrManager" ) == 0 )
	//     return new TKoopaJrManager("クッパジュニアマネージャー");

	// TODO:
	// if ( strcmp( name, "KoopaJr" ) == 0 )
	//     return new TKoopaJr("クッパジュニア");

	// TODO:
	// if ( strcmp( name, "KoopaJrSubmarineManager" ) == 0 )
	//     return new
	//     TKoopaJrSubmarineManager("クッパジュニアサブマリンマネージャー");

	// TODO:
	// if ( strcmp( name, "KoopaJrSubmarine" ) == 0 )
	//     return new TKoopaJrSubmarine("クッパジュニアサブマリン");

	// TODO:
	// if ( strcmp( name, "LimitKoopaJrManager" ) == 0 )
	//     return new
	//     TLimitKoopaJrManager("リミットクッパジュニアマネージャー");

	// TODO:
	// if ( strcmp( name, "LimitKoopaJr" ) == 0 )
	//     return new TLimitKoopaJr("リミットクッパジュニア");

	// TODO:
	// if ( strcmp( name, "LimitKoopaManager" ) == 0 )
	//     return new TLimitKoopaManager("クッパマネージャー");

	// TODO:
	// if ( strcmp( name, "LimitKoopa" ) == 0 )
	//     return new TLimitKoopa("クッパ");

	if (strcmp(name, "BathtubKillerManager") == 0)
		return new TBathtubKillerManager;

	if (strcmp(name, "BathtubKiller") == 0)
		return new TBathtubKiller;

	// TODO:
	// if ( strcmp( name, "BathtubPeachManager" ) == 0 )
	//     return new TBathtubPeachManager("バスタブピーチマネージャー");

	// TODO:
	// if ( strcmp( name, "BathtubPeach" ) == 0 )
	//     return new TBathtubPeach("バスタブピーチ");

	// TODO:
	// if ( strcmp( name, "BossWanwan" ) == 0 )
	//     return new TBossWanwan("ボスワンワン");

	// TODO:
	// if ( strcmp( name, "BossWanwanManager" ) == 0 )
	//     return new TBossWanwanManager("ボスワンワンマネージャ");

	// TODO:
	// if ( strcmp( name, "BossPakkun" ) == 0 )
	//     return new TBossPakkun("ボスパックン改");

	// TODO:
	// if ( strcmp( name, "KBossPakkun" ) == 0 )
	//     return new TBossPakkun("ボスパックン軽");

	// TODO:
	// if ( strcmp( name, "BossPakkunManager" ) == 0 )
	//     return new TBossPakkunManager("ボスパックンマネージャー", 0);

	// TODO:
	// if ( strcmp( name, "KBossPakkunManager" ) == 0 )
	//     return new TBossPakkunManager("ボスパックン軽マネージャ", 1);

	// TODO:
	// if ( strcmp( name, "BossTelesa" ) == 0 )
	//     return new TBossTelesa("ボステレサ");

	// TODO:
	// if ( strcmp( name, "BossTelesaManager" ) == 0 )
	//     return new TBossTelesaManager("ボステレサマネージャー");

	// TODO:
	// if ( strcmp( name, "BubbleManager" ) == 0 )
	//     return new TBubbleManager("バブルマネージャー");

	if (strcmp(name, "OilBall") == 0)
		return new TOilBall;

	if (strcmp(name, "BossManta") == 0)
		return new TBossManta;

	if (strcmp(name, "BossMantaManager") == 0)
		return new TBossMantaManager;

	return nullptr;
}
