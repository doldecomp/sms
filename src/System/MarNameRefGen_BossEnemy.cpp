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
#include <Player/MarioDirtyStrings.hpp>

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
//       Closure batch 120: this is now the unit's *whole* residue. All 43
//       branches and both epilogues match except the TSleepBossHanachan one,
//       and the three differences there are one effect: the missing `bl set`
//       is why retail spills the `new` result to 0x1c(r1) and reloads it into
//       r31 (frame 0x38 against our 0x30) and why the mMirrorActor null store
//       lands after the call. SleepBossHanachan.hpp's initialiser list is
//       already right, so the fix is one inline level between that constructor
//       and TVec3's. A defaulted constructor argument is excluded as the
//       level: the map has no __ct__18TSleepBossHanachan symbol of any arity,
//       so MWCC never synthesised the no-argument forwarder that a default
//       would need. Header round 21 rejected the JGVec3.hpp reading as well:
//       a forwarder between TVec3(T, T, T) and set<f> does give this unit
//       retail's `bl`, but it is all-or-nothing across every TVec3(x, y, z)
//       site, breaks the DOL and is -28 exact functions tree-wide (trial
//       table at the declaration in JGVec3.hpp). The level has to come from
//       somewhere in this unit's own chain, and an initialiser list leaves
//       no room for one in SleepBossHanachan.hpp.
// Retail leaves `TVec3<f>::set<f>(0, 0, 0)` -- TSleepBossHanachan's
// mShinePosition initialiser -- out of line here, which the depth budget only
// does from depth 4 down; through `new TSleepBossHanachan` the member template
// sits at depth 3 and expands.  The missing level belongs in JGeometry (retail
// defines set<f> out of class, non-inline, which is not expressible in MWCC
// 1.2.5 from an in-class member template -- see the trial table in
// JGVec3.hpp), so it is parked here as a TU-local level instead: that is the
// smallest change that reaches only this call site.  Binding the result is the
// +8 of low region retail spends on spilling the object across the constructor
// call (frame 0x30 -> 0x38, exact).  Measured and rejected, all frame 0x40 and
// seven lost instructions: a second nested helper, binding `name` as well,
// binding the result twice, and giving TSleepBossHanachanManager the same
// helper.
// TODO: the last two markers are the two spill slots' positions -- retail puts
// this object's at 0x1c and the manager's at 0x14, ours at 0x14 and 0x18, so
// retail reserves 8 dead bytes below this expansion's temp that one binding
// does not buy.
// Closure batch 164 mapped the pool instead of hunting levers, and the answer
// is an *ordering* fact, not a size one.  There are exactly three live pool
// slots in this function: SleepBossHanachanManager's temp (0x10 in both
// builds), BossEelManager's, and this expansion's.  Deleting the level
// altogether (`TSleepBossHanachan* actor = new TSleepBossHanachan("?");`
// straight in the branch, which loses the `bl set<f>` and seven instructions)
// moves BossEelManager's temp to 0x14 -- retail's slot -- which proves
// retail's pool order is SleepBossHanachanManager, BossEelManager, then this
// expansion *last*, at 0x1c with a dead word at 0x18.  Our builds always
// allocate this expansion's temp before BossEelManager's, i.e. in source
// order, because it comes from a level the caller's own TU supplies.  Every
// lever on the level itself is inert on that order (all still 100.0%, 669
// instructions, frame 0x38, slots 0x14/0x18): returning `TSleepBossHanachan&`
// and taking the address at the call site, splitting the declaration from the
// assignment, returning the base `JDrama::TNameRef*`, moving the binding out
// of the helper into the branch, and defining the helper *after* its caller
// behind a forward declaration (so definition order is inert here too, as it
// is for inlining decisions).  Two-level spellings are worse both ways round
// -- inner bind plus outer pass-through and the reverse both give 98.6%, lose
// the `bl set<f>` block and rotate the pool to 0x10/0x14/0x1c.  So the level
// retail used is not in this TU: a level supplied from inside
// SleepBossHanachan.hpp (between TSleepBossHanachan's constructor and TVec3's)
// would have its temp allocated after the caller's own expansions and is the
// only remaining shape that puts this slot at 0x1c.  That is a shared-header
// change, so it is reported rather than made.
static inline TSleepBossHanachan* newSleepBossHanachan(const char* name)
{
	TSleepBossHanachan* actor = new TSleepBossHanachan(name);
	return actor;
}

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
		return newSleepBossHanachan("?");

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
