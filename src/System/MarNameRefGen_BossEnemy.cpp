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
// The pool order (SleepBossHanachanManager's temp at 0x10, BossEelManager's
// at 0x14, this expansion's last at 0x1c over a dead word) is set by the
// other name-constructed objects going through their own direct-return
// factory level, `newNameRef<T>(name)` below: with it at the one-argument
// sites the two managers' constructor temps are allocated ahead of this
// expansion's, and the function is exact.  Only the two manager sites are
// load-bearing (either one alone moves only its own slot); the level is used
// at every one-argument site for consistency and is frame-neutral there.
// Levers on this function's own level were all inert on the order (closure
// batch 164, header round 30: reference return, split declaration, base
// return type, binding in the branch, definition order, the level moved into
// SleepBossHanachan.hpp).
static inline TSleepBossHanachan* newSleepBossHanachan(const char* name)
{
	TSleepBossHanachan* actor = new TSleepBossHanachan(name);
	return actor;
}

template <class T> static inline T* newNameRef(const char* name)
{
	return new T(name);
}

JDrama::TNameRef* TMarNameRefGen::getNameRef_BossEnemy(const char* name) const
{
	if (strcmp(name, "EMario") == 0)
		return newNameRef<TEMario>("マリオモドキ");

	if (strcmp(name, "EMarioManager") == 0)
		return newNameRef<TEMarioManager>("典型敵マネージャ");

	if (strcmp(name, "BossHanachan") == 0)
		return newNameRef<TBossHanachan>("?");

	if (strcmp(name, "BossHanachanManager") == 0)
		return newNameRef<TBossHanachanManager>("?");

	if (strcmp(name, "SleepBossHanachan") == 0)
		return newSleepBossHanachan("?");

	if (strcmp(name, "SleepBossHanachanManager") == 0)
		return newNameRef<TSleepBossHanachanManager>("?");

	if (strcmp(name, "BossEel") == 0)
		return newNameRef<TBossEel>("?");

	if (strcmp(name, "BossEelManager") == 0)
		return newNameRef<TBossEelManager>("?");

	if (strcmp(name, "BEelTearsManager") == 0)
		return newNameRef<TBEelTearsManager>("めおとウナギ涙マネージャー");

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
		return newNameRef<TTinKoopa>("メカクッパ");

	if (strcmp(name, "TinKoopaManager") == 0)
		return newNameRef<TTinKoopaManager>("メカクッパマネージャ");

	if (strcmp(name, "CoasterKillerManager") == 0)
		return new TCoasterKillerManager;

	if (strcmp(name, "CoasterKiller") == 0)
		return new TCoasterKiller;

	if (strcmp(name, "KoopaJrManager") == 0)
		return newNameRef<TKoopaJrManager>("クッパジュニアマネージャー");

	if (strcmp(name, "KoopaJr") == 0)
		return newNameRef<TKoopaJr>("クッパジュニア");

	if (strcmp(name, "KoopaJrSubmarineManager") == 0)
		return newNameRef<TKoopaJrSubmarineManager>("クッパジュニアサブマリンマネージャー");

	if (strcmp(name, "KoopaJrSubmarine") == 0)
		return newNameRef<TKoopaJrSubmarine>("クッパジュニアサブマリン");

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
		return newNameRef<TBathtubPeachManager>("バスタブピーチマネージャー");

	if (strcmp(name, "BathtubPeach") == 0)
		return newNameRef<TBathtubPeach>("バスタブピーチ");

	if (strcmp(name, "BossWanwan") == 0)
		return new TBossWanwan;

	if (strcmp(name, "BossWanwanManager") == 0)
		return newNameRef<TBossWanwanManager>("ボスワンワンマネージャ");

	if (strcmp(name, "BossPakkun") == 0)
		return new TBossPakkun;

	if (strcmp(name, "KBossPakkun") == 0)
		return newNameRef<TBossPakkun>("ボスパックン軽");

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
		return newNameRef<TOilBall>("油ダマ");

	if (strcmp(name, "BossManta") == 0)
		return new TBossManta;

	if (strcmp(name, "BossMantaManager") == 0)
		return newNameRef<TBossMantaManager>("ボスマンタマネージャ");

	return nullptr;
}
