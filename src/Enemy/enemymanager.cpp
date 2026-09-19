#include <Enemy/EnemyManager.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Enemy.hpp>
#include <System/TimeRec.hpp>
#include <MSound/MAnmSound.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

bool TEnemyManager::mIsCopyAnmMtx = true;

TSpineEnemyParams::TSpineEnemyParams(const char* path)
    : TParams(path)
    , PARAM_INIT(mSLHeadHeight, 120.0f)
    , PARAM_INIT(mSLBodyRadius, 30.0f)
    , PARAM_INIT(mSLWallRadius, 50.0f)
    , PARAM_INIT(mSLClipRadius, 300.0f)
    , PARAM_INIT(mSLFarClip, 10000.0f)
    , PARAM_INIT(mSLHitPointMax, 1)
    , PARAM_INIT(mSLInstanceNum, 100)
    , PARAM_INIT(mSLActiveEnemyNum, 10)
{
	TParams::load(mPrmPath);
}

void TSharedMActorSet::init(MActorAnmData* param_1, J3DModelData* param_2,
                            const char* param_3, int param_4)
{
	unk4 = param_4;
	unk0 = new MActor*[unk4];

	f32 coeff = 1.0f / unk4;
	for (int i = 0; i < unk4; ++i) {
		J3DModel* model = new J3DModel(param_2, 0, 1);
		unk0[i]         = new MActor(param_1);
		unk0[i]->setModel(model, 0);
		unk0[i]->setBck(param_3);
		J3DFrameCtrl* ctrl = unk0[i]->getFrameCtrl(ANM_TYPE_BCK);
		ctrl->setFrame(coeff * ctrl->getEnd() * i);
	}

	unk8 = unk0[0]->getCurAnmIdx(ANM_TYPE_BCK);
}

void TSharedMActorSet::calcAnm()
{
	for (int i = 0; i < unk4; ++i)
		unk0[i]->calcAnm();
}

void TSharedMActorSet::setScale(const JGeometry::TVec3<f32>&) { }

TEnemyManager::TEnemyManager(const char* name)
    : TLiveManager(name)
    , unk38(nullptr)
    , unk3C(1.0f)
    , unk40(nullptr)
    , unk44(0)
    , unk48(0)
    , unk4C(0xffffffff)
    , unk50(0)
{
	gpConductor->registerEnemyManager(this);
}

TEnemyManager::~TEnemyManager() { }

void TEnemyManager::createSharedMActorSet(const char** param_1)
{
	if (unk44 <= 0)
		return;

	u32 num = 0;
	for (int i = 0; param_1[i] != nullptr; ++i)
		++num;

	// TODO: ewwwwwwwwwwww
	u32 prev = (volatile int&)unk44;
	unk44    = num;
	unk40    = new TSharedMActorSet[unk44];

	for (int i = 0; i < unk44; ++i) {
		unk40[i].init(getMActorAnmData(),
		              getModelDataKeeper()->getNthData(0)->getModelData(),
		              param_1[i], prev);
	}
}

TSharedMActorSet* TEnemyManager::getSharedMActorSet(int idx)
{
	if (!unk40)
		return nullptr;

	for (int i = 0; i < unk44; ++i)
		if (idx == unk40[i].getIdx())
			return &unk40[i];

	return nullptr;
}

void TEnemyManager::load(JSUMemoryInputStream& stream)
{
	TLiveManager::load(stream);
	createModelData();
	stream >> unk44;
}

TSpineEnemy* TEnemyManager::createEnemyInstance() { return nullptr; }

void TEnemyManager::createEnemy() { }

void TEnemyManager::createEnemies(int count)
{
	if (count + getObjNum() > getCapacity())
		count = getCapacity() - getObjNum();

	if (unk38 != nullptr && count + getObjNum() > unk38->mSLInstanceNum.get())
		count = unk38->mSLInstanceNum.get() - getObjNum();

	if (count < 0)
		return;

	// TODO: 8 bytes of frame short (0xa8 against retail's 0xb0). Slot
	// triage: the JGadget iterator pool splits into a 5-word group (retail
	// 0x5c..0x6f, ours 0x54..0x67) and a 3-word group (retail 0x78..0x83,
	// ours 0x6c..0x77) with the `insert` hint at 0x84 in both, so group A is
	// +8 short and group B +12, i.e. the gap between the two groups is 8 in
	// retail and 4 here. That is the JGadget iterator temp-pool *grouping*
	// residue (frame-gaps.md batch 133, also open on TSeal::init and
	// TPerformList::perform), and its only known cause is the implicit
	// derived-from-base conversion on TList::insert's return, which lives in
	// a shared JGadget header. Rejected here: the loop body as a call to the
	// UNUSED createEnemy() (map 0xf8, five structural differences), search2
	// with the cast (codegen-identical), push_back instead of add() (lands
	// the frame, breaks the body), a TU-local binding level on getObjNum()
	// (+24).
	for (int i = 0; i < count; ++i) {

		TSpineEnemy* enemy = createEnemyInstance();

		if (!enemy)
			continue;

		JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")->add(enemy);

		enemy->init(this);
	}
}

void TEnemyManager::clipEnemies(JDrama::TGraphics* graphics)
{
	f32 fVar1;
	f32 fVar2;
	if (unk38 == nullptr) {
		fVar1 = 300.0f;
		fVar2 = gpConductor->unk84.mEnemyFarClip.get();
	} else {
		fVar2 = unk38->mSLFarClip.get();
		fVar1 = unk38->mSLClipRadius.get();
	}

	TLiveManager::clipActorsAux(graphics, fVar2, fVar1);
}

void TEnemyManager::setSharedFlags()
{
	if (!unk40)
		return;

	for (int i = 0; i < getObjNum(); ++i) {
		TSpineEnemy* enemy = getObj(i);
		enemy->offLiveFlag(LIVE_FLAG_UNK4000);
		if (!enemy->checkLiveFlag(LIVE_FLAG_HIDDEN | LIVE_FLAG_CLIPPED_OUT)) {
			int idx = enemy->getMActor()->getCurAnmIdx(ANM_TYPE_BCK);
			for (int i = 0; i < unk44; ++i) {
				if (idx < 0 || idx == unk40[i].getIdx()) {
					enemy->onLiveFlag(LIVE_FLAG_UNK4000);
					break;
				}
			}
		}
	}
}

void TEnemyManager::updateAnmSoundShared()
{
	if (!unk40)
		return;

	if (!getObj(0)->getAnmSound())
		return;

	for (int i = 0; i < getObjNum(); ++i) {
		TSpineEnemy* enemy = getObj(i);
		if (!enemy->checkLiveFlag(LIVE_FLAG_HIDDEN | LIVE_FLAG_CLIPPED_OUT)) {
			int idx = enemy->getMActor()->getCurAnmIdx(ANM_TYPE_BCK);
			for (int i = 0; i < unk44; ++i) {
				if (idx < 0 || idx == unk40[i].getIdx()) {
					// unused (mistake)
					J3DFrameCtrl* ctrl2
					    = unk40[i]
					          .getMActor(enemy->getInstanceIndex())
					          ->getFrameCtrl(ANM_TYPE_BCK);
					if (enemy->mAnmSoundPath) {
						J3DFrameCtrl* ctrl
						    = unk40[i]
						          .getMActor(enemy->getInstanceIndex())
						          ->getFrameCtrl(ANM_TYPE_BCK);

						enemy->getAnmSound()->animeLoop(
						    (Vec*)&enemy->getPosition(), ctrl->getFrame(),
						    ctrl->getRate(), 0, 4);
					}
					break;
				}
			}
		}
	}
}

// Binding level worth +8 of low region, landing
// TEnemyManager::copyFromShared's frame at 0x100 (batch 121).
static inline J3DModelData* EnemymanagerGetModelData(J3DModel* p)
{
	J3DModelData* modelData = p->getModelData();
	return modelData;
}

void TEnemyManager::copyFromShared()
{
	// The concat scratch is declared before the saved view matrix on purpose:
	// retail puts the saved view at 0x78 and the concat result at 0xa8, and
	// function-scope named locals are allocated downward from the frame top in
	// declaration order, so the concat matrix has to come first.
	Mtx concatMtx;
	Mtx viewMtx;
	MTXCopy(j3dSys.getViewMtx(), viewMtx);

	s32 r29 = getActiveObjNum();

	for (int i = 0; i < r29; ++i) {
		TSpineEnemy* enemy = getObj(i);
		if (!enemy->checkLiveFlag(LIVE_FLAG_UNK4000)
		    || enemy->checkLiveFlag(LIVE_FLAG_HIDDEN | LIVE_FLAG_CLIPPED_OUT))
			continue;

		int iVar6 = enemy->getMActor()->getCurAnmIdx(ANM_TYPE_BCK);
		for (int j = 0; j < unk44; ++j) {
			if (iVar6 >= 0 && iVar6 != unk40[j].unk8)
				continue;

			J3DModel* model
			    = unk40[j].getMActor(enemy->getInstanceIndex())->getModel();
			MtxPtr src = enemy->getModel()->getBaseTRMtx();
			MTXScaleApply(src, src, enemy->mScaling.x, enemy->mScaling.y,
			              enemy->mScaling.z);
			MTXConcat(viewMtx, src, concatMtx);
			j3dSys.setViewMtx(concatMtx);

			model->viewCalc();

			enemy->getModel()->swapAllMtx();
			enemy->getModel()->calcNrmMtx();
			enemy->getModel()->prepareShapePackets();

			J3DPSMtxArrayCopy(*model->getDrawMtxPtr(),
			                  *enemy->getModel()->getDrawMtxPtr(),
			                  EnemymanagerGetModelData(model)->getDrawMtxNum());

			DCStoreRange(enemy->getModel()->getDrawMtxPtr(),
			             model->getModelData()->getDrawMtxNum() * sizeof(Mtx));

			break;
		}
	}

	j3dSys.setViewMtx(viewMtx);
}

// TODO: 99.7%, frame-exact (0xf0). Two residues, both known-open classes.
// (1) The two TTimeRec::startTimer(0xff,...) colour temporaries: retail builds
// them at 0xb8 and 0xbc, i.e. adjacent with a 4-byte stride, while ours are at
// 0xb0 and 0xb8 -- the JUTColor temp-stride residue.
// (2) One opcode at 0x81c: in the alive-count loop retail materialises the
// strength-reduced byte offset by copying the zero it already has in
// aliveNum's register (`li r5, 0; addi r3, r5, 0`) where we emit two
// independent `li 0`s. The same shape is open in TEMario::perform, so it is a
// constant-reuse property of MWCC's loop setup, not a spelling here.
// TODO: 99.7%. Two residues, probably one cause: our low region is 4 bytes
// short (the TTimeRec colour temp sits at 0xb8 not 0xbc -- the known-open
// JUTColor temp stride), and retail initialises the first loop's index by
// copying the already-materialised zero (`addi r3, r5, 0`) instead of a fresh
// `li r3, 0`. Refuted: hoisting the index declaration out of the `for`.
void TEnemyManager::performShared(u32 param_1, JDrama::TGraphics* param_2)
{
	if (unk30 & 1)
		TTimeRec::startTimer();

	int num2     = getActiveObjNum();
	int aliveNum = 0;
	for (int i = 0; i < num2; ++i)
		if (!((TSpineEnemy*)TObjManager::getObj(i))
		         ->checkLiveFlag(LIVE_FLAG_DEAD))
			++aliveNum;

	if (aliveNum <= 0) {
		if ((unk30 & 1))
			TTimeRec::endTimer();
		return;
	}

	if (param_1 & CUE_CALC_ANIM) {
		clipEnemies(param_2);
		for (int i = 0; i < unk44; ++i)
			unk40[i].calcAnm();
		setSharedFlags();
		updateAnmSoundShared();
	}

	if (param_1 & CUE_CALC_VIEW)
		copyFromShared();

	if (unk30 & 1) {
		TTimeRec::endTimer();
		TTimeRec::startTimer(0xff, 0x00, 0x00);
	}

	int num = getActiveObjNum();
	if (param_1 & CUE_MOVE) {
		for (int i = num; i < mObjNum; ++i)
			getObj(i)->onHitFlag(HIT_FLAG_NO_COLLISION);
	}

	for (int i = 0; i < num; ++i) {
		TSpineEnemy* enemy = getObj(i);
		if (enemy->checkLiveFlag(LIVE_FLAG_DEAD))
			continue;

		if (param_1 & CUE_MOVE)
			enemy->moveObject();

		if (param_1 & CUE_CALC_ANIM) {
			enemy->updateSquareToMario();
			enemy->calcRootMatrix();
			if (!enemy->checkLiveFlag(LIVE_FLAG_UNK4000)) {
				enemy->getMActor()->frameUpdate();
				if (!enemy->checkLiveFlag(LIVE_FLAG_HIDDEN
				                          | LIVE_FLAG_CLIPPED_OUT)) {
					enemy->getMActor()->calc();
					enemy->updateAnmSound();
				}
			} else {
				enemy->getMActor()->matAnmFrameUpdate();
			}

			if (param_1 & CUE_CALC_VIEW)
				enemy->requestShadow();

			if (!enemy->checkLiveFlag(LIVE_FLAG_HIDDEN
			                          | LIVE_FLAG_CLIPPED_OUT)) {
				if ((param_1 & CUE_CALC_VIEW)
				    && !enemy->checkLiveFlag(LIVE_FLAG_UNK4000))
					enemy->getMActor()->viewCalc();
				if (param_1 & CUE_ENTRY) {
					enemy->getMActor()->setLightData(enemy->getGroundPlane(),
					                                 enemy->mPosition);
					enemy->getMActor()->entry();
				}
			}
		}
	}

	if (unk30 & 1)
		TTimeRec::endTimer();
}

void TEnemyManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	changeDrawBuffer(cue);
	if (unk40) {
		performShared(cue, graphics);
		restoreDrawBuffer(cue);
		return;
	}

	if (unk30 & 1)
		TTimeRec::startTimer();

	if (cue & CUE_CALC_ANIM) {
		clipEnemies(graphics);
		setFlagOutOfCube();
	}

	if (unk30 & 1) {
		TTimeRec::endTimer();
		TTimeRec::startTimer(0xff, 0x0, 0x0);
	}

	int num = getActiveObjNum();
	// The cue test is inside the loop: MWCC unswitches it and leaves the
	// dead getObj() load out of the other copy, which is why retail has two
	// loops over the same range and only one of them touches the object.
	for (int i = num; i < mObjNum; ++i) {
		// One accessor level shallower than getObj(i): the three-level
		// TEnemyManager::getObj costs 8 bytes of frame here (0xa8 against
		// retail's 0xa0), and the THitActor* spelling without the cast
		// loses the register assignment.
		TSpineEnemy* enemy = (TSpineEnemy*)TObjManager::getObj(i);
		if (cue & CUE_MOVE)
			enemy->onHitFlag(HIT_FLAG_NO_COLLISION);
	}

	for (int i = 0; i < num; ++i)
		getObj(i)->testPerform(cue, graphics);

	restoreDrawBuffer(cue);
	if (unk30 & 1)
		TTimeRec::endTimer();
}

TSpineEnemy* TEnemyManager::getNearestEnemy(const JGeometry::TVec3<f32>& p)
{
	f32 dist          = 0.0f;
	TSpineEnemy* best = nullptr;

	for (int i = 0; i < getActiveObjNum(); ++i) {
		TSpineEnemy* candidate = getObj(i);

		if (candidate->checkLiveFlag(LIVE_FLAG_DEAD))
			continue;

		f32 d = VECDistance((Vec*)&p, (Vec*)&candidate->mPosition);
		if (!best || dist > d) {
			best = candidate;
			dist = d;
		}
	}

	return best;
}

TSpineEnemy* TEnemyManager::getDeadEnemy()
{
	s32 num = getActiveObjNum();
	for (int i = 0; i < num; ++i)
		if (getObj(i)->checkLiveFlag(LIVE_FLAG_DEAD))
			return getObj(i);

	return nullptr;
}

TSpineEnemy* TEnemyManager::getFarOutEnemy()
{
	f32 dist          = -1.0f;
	TSpineEnemy* best = nullptr;

	for (int i = 0; i < getActiveObjNum(); ++i) {
		TSpineEnemy* enemy = getObj(i);
		if (enemy->checkLiveFlag(LIVE_FLAG_DEAD)) {
			best = enemy;
			break;
		}

		if (!enemy->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)
		    || !enemy->checkLiveFlag(LIVE_FLAG_UNK800))
			continue;

		if (!best || dist > enemy->getDistToMarioSquared()) {
			best = enemy;
			dist = enemy->getDistToMarioSquared();
		}
	}

	return best;
}

void TEnemyManager::killChildren()
{
	for (int i = 0; i < mObjNum; ++i)
		getObj(i)->onLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_UNK40);
}

void TEnemyManager::killChildrenWithin(const JGeometry::TVec3<f32>& p, f32 r)
{
	if (!mObjNum)
		return;

	if (!getObj(0)->checkActorType(ACTOR_TYPE_ENEMY)
	    && !getObj(0)->checkActorType(ACTOR_TYPE_BOSS))
		return;

	for (int i = 0; i < mObjNum; ++i) {
		TSpineEnemy* enemy = getObj(i);
		if (VECSquareDistance((Vec*)&p, (Vec*)&enemy->mPosition) <= r * r)
			enemy->onLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_UNK40);
	}
}

void TEnemyManager::killOtherEnemies() { }

int TEnemyManager::countLivingEnemy() const
{
	s32 num = getActiveObjNum();

	int result = 0;
	for (int i = 0; i < num; ++i)
		if (!getObj(i)->checkLiveFlag(LIVE_FLAG_DEAD))
			++result;

	return result;
}

void TEnemyManager::createCopyAnmMtx(int) { }

// Binding level worth +8 of low region; with getScaling() above it this lands
// copyAnmMtx's frame at retail's 0xc0 (base 0xb0). Applied at one site only --
// the level saturates per receiver, so any single site does it.
static inline MActor* EnemymanagerGetMActor(const TSpineEnemy* p)
{
	MActor* actor = p->getMActor();
	return actor;
}

static inline int EnemymanagerGetCurAnmFrameNo(const TSpineEnemy* p)
{
	return p->getCurAnmFrameNo(ANM_TYPE_BCK);
}

// TODO: frame-exact, but retail keeps the scaled animation-frame index in r27
// and the scratch matrix pointer in r28 while we have them the other way
// round, which costs the slwi/lfs/addi ordering at 0x88 too. afStack_5C also
// sits at 0x64 in retail and 0x68 here, so one 4-byte named local is missing
// above it even though the frame total is right. By the callee-saved rule
// (r31 downwards in reverse introduction order) retail's claim order is
// r29 stride temp, r28 wtf, r27 f, r26 mtx, i.e. an introduction order of
// mtx, f, wtf -- our build introduces them in exactly reverse source order
// (mtx, wtf, f), so no permutation of the three that keeps `wtf = afStack_5C`
// after its array can produce retail's. Measured: moving `wtf` after `mtx`
// 96.1 -> 96.0; hoisting `afStack_5C` + `wtf` above the getCurAnmFrameNo call
// 96.1 -> 96.8 but the swap survives and the wtf materialisation moves ahead
// of the call. Declaration-order permutations of `f`, `afStack_5C` and `wtf`
// (six tried earlier) do not move it, and dropping `wtf` loses two
// instructions retail has.
// Closed (header round 24): the UNUSED
// `TPosition3<TMatrix34<SMatrix34C<f>>>::TPosition3()` is not a missing
// definition and is not a lead for this function. JGPosition3.hpp already
// spells it in-class with an empty body, which is exactly the map's 4 bytes
// (one `blr`), and a weak in-class body that every site inlines leaves no
// symbol -- so nothing about it can be "defined" anywhere. What the map
// records is a weak out-of-line copy that survived compilation and was then
// dead-stripped, and its emission slot places it between createCopyAnmMtx and
// countLivingEnemy, i.e. immediately after the function that referenced it.
// That reference has to be an array construction: `new TPosition3f[n]` passes
// the constructor's *address* to __construct_array, which forces the
// out-of-line copy, and because createCopyAnmMtx (UNUSED, 0x15c) is itself
// dead both references die with it. So the real finding is about that stub:
// createCopyAnmMtx allocates the `unk48` buffer as an array of TPosition3f,
// and `unk48` is therefore a TPosition3f array rather than the `Mtx**` we
// declare. Spelling `afStack_5C` as TPosition3f is codegen-identical here
// (96.1%, same five markers), so the r27/r28 swap has to come from somewhere
// else.
// Closure batch 211: our four locals come out in **forward** declaration order
// (f r28, wtf r27, mtx r26, i r25) and retail's are wtf, f, mtx, i, so retail
// declares the scratch matrix before the frame index -- which is the variant
// batch 205 measured at 96.8% with the swap surviving, because an initialised
// `MtxPtr wtf = afStack_5C;` also drags its materialisation ahead of the
// getCurAnmFrameNo call. The 4-byte hole above afStack_5C (0x64 retail against
// 0x68 here, same 0xc0 total) says retail's first-declared named local is a
// 4-byte one, i.e. something declared *before* the matrix. Research 210's
// inlined-call rule does not apply: the rotated pair is a local and a stack
// address, not a receiver and an argument (see the bucket ladder recorded on
// TBossHanachan::setHeadAndBodyAnm).
// Closure batch 222 applied research 221's lever and landed most of it:
// routing the frame number through the TU-local binding level
// `EnemymanagerGetCurAnmFrameNo` makes the instruction stream **exact** (109
// instructions, no inserted or deleted ones, frame 0xc0) -- the `lfs f0` /
// `addi rD, r1, off` pair at 0x88 now comes out in retail's order -- taking
// the function 96.1 -> 99.7%.  A *named* local inside that helper
// (`int frame = ...; return frame;`) is a different and worse lever: it splits
// the raw index from its `slwi` into two callee-saved values and costs +8 of
// frame (0xc8, Mtx at 0x70, 12 markers), i.e. research 221's "a named local in
// an inlined callee reserves stack" priced here at 8, not 4.
// Residue (6 markers): retail reuses one register for the index and its
// `slwi` (r27) and gives `wtf` r28, we spend r29 on the raw index, r28 on the
// shifted one and r27 on `wtf`; afStack_5C is still 0x68 against retail's
// 0x64, so the low region is 4 bytes too big (retail pays the difference back
// as alignment pad under the 8-aligned float-conversion slot at 0x98, which is
// why both frames are 0xc0).  Dropping `wtf` is -8 of low region, not -4
// (0xb8, 14 markers, and it loses an instruction retail has), so the missing
// knob is a -4 one below the matrix.
bool TEnemyManager::copyAnmMtx(TSpineEnemy* enemy)
{
	if (unk4C != EnemymanagerGetMActor(enemy)->getCurAnmIdx(ANM_TYPE_BCK))
		return false;

	int f = EnemymanagerGetCurAnmFrameNo(enemy);
	enemy->calcRootMatrix();
	enemy->updateAnmSound();
	enemy->getMActor()->frameUpdate();

	Mtx afStack_5C;
	MtxPtr wtf = afStack_5C;
	MtxPtr mtx = enemy->getMActor()->getModel()->getBaseTRMtx();

	const JGeometry::TVec3<f32>& v = enemy->getScaling();
	mtx[0][0] *= v.x;
	mtx[0][1] *= v.y;
	mtx[0][2] *= v.z;
	mtx[1][0] *= v.x;
	mtx[1][1] *= v.y;
	mtx[1][2] *= v.z;
	mtx[2][0] *= v.x;
	mtx[2][1] *= v.y;
	mtx[2][2] *= v.z;

	for (int i = 0; i < unk50; ++i) {
		MTXConcat(mtx, unk48[f][i], afStack_5C);
		enemy->getMActor()->getModel()->setAnmMtx(i, wtf);
	}

	if (enemy->getMActor()->getModel()->getModelData()->getWEvlpMtxNum())
		enemy->getModel()->calcWeightEnvelopeMtx();

	return true;
}
