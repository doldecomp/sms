#include <Enemy/BossManta.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <System/EmitterViewObj.hpp>
#include <Strategic/Spine.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Map/PollutionManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <System/ParamInst.hpp>
#include <System/Resolution.hpp>
#include <MarioUtil/ReinitGX.hpp>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

TBossManta::TBossManta(const char* name)
    : TSpineEnemy(name)
{
	unk158   = 0.0f;
	unk15C   = 0.0f;
	unk160   = 0.0f;
	unk164   = 0.0f;
	unk168   = 0.0f;
	unk16C   = 0.0f;
	unk170   = 0.0f;
	unk174   = 0.0f;
	unk178   = 1.0f;
	unk17C.x = 0.0f;
	unk17C.y = 0.0f;
	unk17C.z = 0.0f;
	unk188   = 0;
	unk18C   = 0;
	unk190   = 0.0f;
	unk194   = 0.0f;
	unk198   = 0.0f;
	unk19C   = 0;
	unk1A0   = 0;
}

void TBossManta::init(TLiveManager* manager)
{
	// TODO: WIP - core init; tail (unk7C branch) simplified.
	mManager = manager;
	mManager->manageActor(this);
	mMActor     = mMActorKeeper->createMActor("manta.bmd", 0);
	mHeadHeight = 5000.0f;
	initHitActor(0x08000004, 1, 0x80000000, 0.0f, 0.0f, 0.0f, 0.0f);
	unk170 = 0.0f;
	unk174 = 0.0f;
	unk178 = 1.0f;
	calcRootMatrix();
	kill();
	mLiveFlag |= LIVE_FLAG_UNK8;
	mLiveFlag &= ~0x100;
	unk1A4 = 0;
	unk150 = 0.5f;
}
void TBossManta::control()
{
	// TODO: WIP - force-turn lerp drafted; anim-frame blend table (@3118) TODO.
	if (unk1A0 > 0)
		unk1A0--;

	if (getMActor()->checkCurBckFromIndex(3)) {
		JGeometry::TVec3<f32> f(unk164, unk168, unk16C);
		if (f.dot(f) == 0.0000038146973f)
			f.set(0.0f, 0.0f, 0.0f);
		else
			f.setLength(1.0f);

		static const f32 blend[]
		    = { 0.005f, 0.008f, 0.01f, 0.03f, 0.05f, 0.05f };
		f32 b    = blend[unk18C];
		f32 turn = -(0.4f * (unk178 * f.x - unk170 * f.z) - 0.5f);
		unk150   = (1.0f - b) * unk150 + b * turn;
	}
}
void TBossManta::moveObject()
{
	// TODO: WIP - pollution stamp + push-away loop; vector codegen needs work.
	TLiveActor::moveObject();

	static const u8 pollute[] = { 1, 1, 1, 1, 1, 1 };
	if (pollute[unk18C])
		gpPollution->stamp(1, mPosition.x, mPosition.y, mPosition.z,
		                   getPolluteRadius());

	for (int i = 0; i < mColCount; ++i) {
		if (mCollisions[i]->getActorType() != 0x80000001)
			continue;
		JGeometry::TVec3<f32> dir;
		dir.x = gpMarioPos->x - mPosition.x;
		dir.y = gpMarioPos->y - mPosition.y;
		dir.z = gpMarioPos->z - mPosition.z;
		if (dir.dot(dir) == 0.0000038146973f)
			dir.set(0.0f, 0.0f, 0.0f);
		else
			dir.setLength(1.0f);
		SMS_SendMessageToMario(this, 0xE);
	}
}
int TBossManta::sCenterJointIndex;

void TBossManta::calcRootMatrix()
{
	if (unk154 < 0x31)
		unk154++;

	Mtx m;
	m[0][3] = mPosition.x;
	m[1][3] = mPosition.y;
	m[2][3] = mPosition.z;
	m[0][0] = 1.0f * unk178 - 0.0f * unk174;
	m[1][0] = 0.0f * unk170 - 0.0f * unk178;
	m[2][0] = -(1.0f * unk170 - 0.0f * unk174);
	m[0][1] = 0.0f;
	m[1][1] = 1.0f;
	m[2][1] = 0.0f;
	m[0][2] = unk170;
	m[1][2] = unk174;
	m[2][2] = unk178;

	J3DModel* model    = getModel();
	MtxPtr joint       = model->mNodeMatrices[sCenterJointIndex];
	unk17C.x           = joint[0][3];
	unk17C.y           = mPosition.y;
	unk17C.z           = joint[2][3];
	(Vec&)model->unk14 = (Vec&)mScaling;
	MTXCopy(m, model->unk20);
}
void TBossManta::drawObject(JDrama::TGraphics*) { }
void TBossManta::reset() { }
BOOL TBossManta::receiveMessage(THitActor* sender, u32 message)
{
	if (message == 0xF
	    && gpModelWaterManager
	               ->mParticleFlagSOA[((TWaterHitActor*)sender)->unk68]
	           & 0x40)
		return collidedWithWater();
	return FALSE;
}
void TBossManta::updateAttractor()
{
	// TODO: WIP - full attractor/pusher force accumulation is complex vector
	// math; drafted the attractor-direction core only.
	JGeometry::TVec3<f32> v;
	v.x = unk158 - mPosition.x;
	v.y = 0.0f;
	v.z = unk160 - mPosition.z;
	if (v.dot(v) == 0.0000038146973f)
		v.set(0.0f, 0.0f, 0.0f);
	else
		v.setLength(1.0f);
	unk164 = v.x;
	unk168 = v.y;
	unk16C = v.z;
}
f32 TBossManta::getPolluteRadius()
{
	if (unk18C < 4) {
		if (unk18C < 0)
			return 0.0f;
		return ((TBossMantaParams*)getSaveParam())->mSLPolluteRadius.get()
		       * mScaling.x;
	}
	if (unk18C < 6)
		return 100.0f;
	return 0.0f;
}
f32 TBossManta::sScale[] = { 20.0f, 10.0f, 5.0f, 2.0f, 1.0f, 1.0f };

void TBossManta::initNthGeneration(int gen)
{
	static const f32 heights[] = { 10.0f, 5.0f, 1.0f, 0.42f, 0.42f, 0.42f };

	unk18C     = gen;
	f32 s      = sScale[unk18C];
	mScaling.z = s;
	mScaling.x = s;
	mScaling.y = heights[unk18C];
	unk19C     = 0;
	unk150     = 0.5f;
	unk154     = 0x31;

	if (unk18C == 0)
		mSpine->initWith(&TNerveMantaAppearDemo::theNerve());
	else
		mSpine->setNext(&TNerveMantaMove::theNerve());
}
BOOL TBossManta::collidedWithWater()
{
	if (unk1A0 > 0) {
		unk1A0 = 0x1E;
		mSpine->setNext(&TNerveMantaHitWater::theNerve());
		return TRUE;
	}
	return FALSE;
}
BOOL TBossManta::getIntoGraphVec(JGeometry::TVec3<f32>* out)
{
	// TODO: WIP - graph containment; core returns zero vec.
	out->set(0.0f, 0.0f, 0.0f);
	return TRUE;
}

void TBossMantaAdditionalCollision::perform(u32 flags,
                                            JDrama::TGraphics* graphics)
{
	// TODO: WIP - push-away response; vector-op codegen needs work.
	THitActor::perform(flags, graphics);
	if (flags & 1) {
		for (int i = 0; i < mColCount; ++i) {
			if (mCollisions[i]->getActorType() != 0x80000001)
				continue;
			JGeometry::TVec3<f32> dir;
			dir.x = gpMarioPos->x - mPosition.x;
			dir.y = gpMarioPos->y - mPosition.y;
			dir.z = gpMarioPos->z - mPosition.z;
			if (dir.dot(dir) == 0.0000038146973f)
				dir.set(0.0f, 0.0f, 0.0f);
			else
				dir.setLength(1.0f);
			SMS_SendMessageToMario(this, 0xE);
		}
	}
}
BOOL TBossMantaAdditionalCollision::receiveMessage(THitActor* sender,
                                                   u32 message)
{
	if (unk68 == nullptr)
		return FALSE;
	return unk68->receiveMessage(sender, message);
}

TBossMantaAdditionalCollisionSet::TBossMantaAdditionalCollisionSet()
{
	// TODO: WIP - creates 3 collision items; JGadget list wiring omitted.
	unkC = nullptr;
	for (int i = 0; i < 3; ++i)
		(&unk0)[i] = new TBossMantaAdditionalCollision("マンタ当たり");
}
void TBossMantaAdditionalCollisionSet::update(u32 flags,
                                              JDrama::TGraphics* graphics)
{
	if (unkC != nullptr) {
		if (unkC->mLiveFlag & 1) {
			unkC = nullptr;
			return;
		}
		for (int i = 0; i < 3; ++i)
			(&unk0)[i]->perform(flags, graphics);
	}
}
void TBossMantaAdditionalCollisionSet::adapt(TBossManta* manta)
{
	unkC                = manta;
	unk0->mAttackRadius = 54.0f * unkC->mScaling.x;
	unk0->mAttackHeight = 100.0f;
	unk0->mDamageRadius = 54.0f * unkC->mScaling.x;
	unk0->mDamageHeight = 100.0f;
	unk4->mAttackRadius = 26.0f * unkC->mScaling.x;
	unk4->mAttackHeight = 100.0f;
	unk4->mDamageRadius = 26.0f * unkC->mScaling.x;
	unk4->mDamageHeight = 100.0f;
	unk8->mAttackRadius = 26.0f * unkC->mScaling.x;
	unk8->mAttackHeight = 100.0f;
	unk8->mDamageRadius = 26.0f * unkC->mScaling.x;
	unk8->mDamageHeight = 100.0f;
	unk0->unk68         = unkC;
	unk4->unk68         = unkC;
	unk8->unk68         = unkC;
}

TBossMantaParams::TBossMantaParams(const char* name)
    : TSpineEnemyParams(name)
    , PARAM_INIT(mSLPolluteRadius, 100.0f)
    , PARAM_INIT(mSLDamageEffectNum, 3)
    , PARAM_INIT(mSLAppearDemoInitialZ, 7600.0f)
    , PARAM_INIT(mSLAppearDemoWalkSpeed, 3.0f)
    , PARAM_INIT(mSLMantaRed, 0xC4)
    , PARAM_INIT(mSLMantaGreen, 0x80)
    , PARAM_INIT(mSLMantaBlue, 0x5F)
    , PARAM_INIT(mSLMantaAlpha, 0x80)
    , PARAM_INIT(mSLAngryMantaRed, 0xD2)
    , PARAM_INIT(mSLAngryMantaGreen, 0x1E)
    , PARAM_INIT(mSLAngryMantaBlue, 0x5A)
    , PARAM_INIT(mSLAngryMantaAlpha, 0x80)
    , PARAM_INIT(mSLAttractorPower, 622.0f)
    , PARAM_INIT(mSLPusherPower, 23559.0f)
    , PARAM_INIT(mSLEscapeLookPoint, 1000.0f)
    , PARAM_INIT(mSLEscapeLookedPoint, 1000.0f)
    , PARAM_INIT(mSLEscapeRegion, 500.0f)
{
	TParams::load(mPrmPath);
}

TBossMantaManager::TBossMantaManager(const char* name)
    : TEnemyManager(name)
{
	unk7C      = 0;
	unk80      = 0;
	unk88.unk0 = this;
	unk88.unk4 = 0;
	unk90.unk0 = this;
	unk90.unk4 = 0;
	unk7C = new (0x20) u8[SMSGetGameRenderWidth() * SMSGetGameRenderHeight()];
	unk74 = new JGeometry::TVec3<f32>[7];
	unk78 = new JGeometry::TVec3<f32>[2];
}

void TBossMantaManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TBossMantaParams("/enemy/bossmanta.prm");
	TEnemyManager::load(stream);
}
void TBossMantaManager::loadAfter()
{
	// TODO: WIP - particle-flag setup loop omitted.
	TEnemyManager::loadAfter();
}
void TBossMantaManager::perform(u32 flags, JDrama::TGraphics* graphics)
{
	TEnemyManager::perform(flags, graphics);
	if (flags & 8)
		drawMantaShadow(graphics);
	if (flags & 1) {
		unk88.update();
		unk90.update();
		updateMantaEscape();
	}
	for (int i = 0; i < 8; ++i)
		mCollisionSets[i]->update(flags, graphics);
}
void TBossMantaManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "manta.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (1 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}
void TBossMantaManager::spawn(int gen, const JGeometry::TVec3<f32>& pos)
{
	// TODO: 1.5% - logic correct (ring spawn of counts[gen] manta), but the
	// Y-rotation matrix-mult codegen shape needs work.
	static const int counts[] = { 1, 2, 3, 4, 4 };

	int count     = counts[gen];
	f32 baseAngle = 3.1415927f * (2.0f * MsRandF());
	for (int i = 0; i < count; ++i) {
		TBossManta* manta = (TBossManta*)getDeadEnemy();
		if (manta != nullptr) {
			JGeometry::TVec3<f32> dir(0.0f, 0.0f, 1.0f);
			f32 angle = baseAngle + (2.0f * (f32)i * 3.1415927f) / (f32)count;
			Mtx m;
			MTXRotRad(m, 'y', angle);
			MTXMultVec(m, &dir, &dir);
			manta->unk170    = dir.x;
			manta->unk174    = dir.y;
			manta->unk178    = dir.z;
			manta->mPosition = pos;
			manta->initNthGeneration(gen);
		}
	}
}
void TBossMantaManager::createEnemies(int num)
{
	// TODO: WIP - creates enemies + additional collision sets.
	TEnemyManager::createEnemies(num);
}
static const GXColor sMantaEfbMatColor = { 0, 0, 0, 0xFF };

void TBossMantaManager::setupEfbAlpha(JDrama::TGraphics* graphics)
{
	ReInitializeGX();

	Mtx44 proj;
	C_MTXOrtho(proj, (f32)SMSGetGameRenderHeight(), 0.0f, 0.0f,
	           (f32)SMSGetGameRenderWidth(), 0.0f, 1.0f);
	GXSetProjection(proj, GX_ORTHOGRAPHIC);
	GXSetColorUpdate(GX_FALSE);
	GXSetAlphaUpdate(GX_TRUE);
	GXSetDstAlpha(GX_TRUE, 0);
	GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);

	Mtx m;
	PSMTXIdentity(m);
	GXLoadPosMtxImm(m, GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetCullMode(GX_CULL_NONE);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(0.0f, (f32)SMSGetGameRenderHeight(), -1.0f);
	GXPosition3f32((f32)SMSGetGameRenderWidth(), (f32)SMSGetGameRenderHeight(),
	               -1.0f);
	GXPosition3f32((f32)SMSGetGameRenderWidth(), 0.0f, -1.0f);
	GXPosition3f32(0.0f, 0.0f, -1.0f);
	GXEnd();

	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG,
	              GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG,
	              GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXColor matColor = sMantaEfbMatColor;
	GXSetChanMatColor(GX_COLOR0A0, matColor);
	GXSetAlphaUpdate(GX_TRUE);
	GXSetDstAlpha(GX_FALSE, 0);
	GXSetZMode(GX_TRUE, GX_GEQUAL, GX_FALSE);
	GXSetAlphaUpdate(GX_TRUE);
	GXSetProjection(graphics->mProjMtx.mMtx, GX_PERSPECTIVE);
}
void TBossMantaManager::updateMantaEscape()
{
	// TODO: WIP - sets escape flag from manta<->mario distances.
}
void TBossMantaManager::drawMantaShadow(JDrama::TGraphics* graphics)
{
	setupEfbAlpha(graphics);

	for (int i = 0; i < getActiveObjNum(); ++i) {
		TSpineEnemy* enemy = getObj(i);
		if (enemy->checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_CLIPPED_OUT))
			continue;

		GXSetCullMode(GX_CULL_FRONT);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
		for (u16 j = 0; j < enemy->getModel()->getModelData()->getShapeNum();
		     ++j)
			enemy->getModel()->getShapePacket(j)->draw();

		GXSetCullMode(GX_CULL_BACK);
		GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
		for (u16 j = 0; j < enemy->getModel()->getModelData()->getShapeNum();
		     ++j)
			enemy->getModel()->getShapePacket(j)->draw();
	}

	GXSetTexCopySrc(0, 0, (u16)SMSGetGameRenderWidth(),
	                (u16)SMSGetGameRenderHeight());
	GXSetTexCopyDst((u16)SMSGetGameRenderWidth(), (u16)SMSGetGameRenderHeight(),
	                GX_TF_A8, GX_FALSE);
	GXCopyTex(unk7C, GX_FALSE);
	GXPixModeSync();

	Mtx44 proj;
	C_MTXOrtho(proj, (f32)SMSGetGameRenderHeight(), 0.0f, 0.0f,
	           (f32)SMSGetGameRenderWidth(), 0.0f, 1.0f);
	GXSetProjection(proj, GX_ORTHOGRAPHIC);
	GXSetNumTevStages(1);
	GXSetNumChans(0);
	GXSetNumTexGens(1);
	GXSetZCompLoc(GX_FALSE);

	GXTexObj texObj;
	GXInitTexObj(&texObj, unk7C, (u16)SMSGetGameRenderWidth(),
	             (u16)SMSGetGameRenderHeight(), GX_TF_I8, GX_CLAMP, GX_CLAMP,
	             GX_FALSE);
	GXInitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE,
	                GX_FALSE, GX_ANISO_1);
	GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
	GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);

	if (unk88.unk4 == 2) {
		unk84 = (unk84 + 1 > 15) ? 15 : unk84 + 1;
	}

	TBossMantaParams* params = (TBossMantaParams*)getSaveParam();
	f32 t                    = (f32)unk84 / 15.0f;
	unk80.r                  = (u8)((1.0f - t) * (f32)params->mSLMantaRed.get()
                   + t * (f32)params->mSLAngryMantaRed.get());
	unk80.g = (u8)((1.0f - t) * (f32)params->mSLMantaGreen.get()
	               + t * (f32)params->mSLAngryMantaGreen.get());
	unk80.b = (u8)((1.0f - t) * (f32)params->mSLMantaBlue.get()
	               + t * (f32)params->mSLAngryMantaBlue.get());
	unk80.a = (u8)((1.0f - t) * (f32)params->mSLMantaAlpha.get()
	               + t * (f32)params->mSLAngryMantaAlpha.get());
	GXSetTevKColor(GX_KCOLOR0, unk80);

	GXLoadTexObj(&texObj, GX_TEXMAP0);
	GXSetDstAlpha(GX_TRUE, 0xFF);
	GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR_NULL);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
	                  GX_FALSE, GX_PTIDENTITY);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_KONST,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_R8_GT, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXA, GX_CC_ZERO, GX_CC_KONST,
	                GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_COMP_R8_GT, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetDstAlpha(GX_FALSE, 0);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_INVSRCALPHA, GX_LO_NOOP);
	GXSetAlphaUpdate(GX_FALSE);
	GXSetColorUpdate(GX_TRUE);

	Mtx m;
	PSMTXIdentity(m);
	GXLoadPosMtxImm(m, GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetCullMode(GX_CULL_NONE);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(0.0f, (f32)SMSGetGameRenderHeight(), -10.0f);
	GXTexCoord2f32(0.0f, 0.0f);
	GXPosition3f32((f32)SMSGetGameRenderWidth(), (f32)SMSGetGameRenderHeight(),
	               -10.0f);
	GXTexCoord2f32(1.0f, 0.0f);
	GXPosition3f32((f32)SMSGetGameRenderWidth(), 0.0f, -10.0f);
	GXTexCoord2f32(1.0f, 1.0f);
	GXPosition3f32(0.0f, 0.0f, -10.0f);
	GXTexCoord2f32(0.0f, 1.0f);
	GXEnd();

	GXSetProjection(graphics->mProjMtx.mMtx, GX_PERSPECTIVE);
}
TSpineEnemy* TBossMantaManager::createEnemyInstance()
{
	return new TBossManta("マンタ");
}
void TBossMantaManager::TMantaMessageState::update()
{
	// TODO: WIP - message-state machine (switch on unk4).
	switch (unk4) {
	default:
		break;
	}
}
void TBossMantaManager::TMantaBattleState::update()
{
	// TODO: WIP - battle-state machine (switch on unk4; demo cameras).
	switch (unk4) {
	default:
		break;
	}
}

DEFINE_NERVE(TNerveMantaSpawn, TLiveActor)
{
	TBossManta* self = (TBossManta*)spine->getBody();

	if (spine->getTime() < 5) {
		self->mScaling.x *= 1.01f;
		self->mScaling.y *= 1.01f;
		self->mScaling.z *= 1.01f;
	} else {
		self->mScaling.x *= 0.9f;
		self->mScaling.y *= 0.9f;
		self->mScaling.z *= 0.9f;
	}

	if (spine->getTime() == 0) {
		static const int particles[] = { 0xFC, 0xFB, 0xFA, 0xF9 };
		gpMarioParticleManager->emitAndBindToPosPtr(particles[self->unk18C],
		                                            &self->unk17C, 0, self);
		static const u32 sounds[] = { 0x8994, 0x8995, 0x8996, 0x8997 };
		u32 snd                   = sounds[self->unk18C];
		if (gpMSound->gateCheck(snd))
			MSoundSESystem::MSoundSE::startSoundActor(
			    snd, (const Vec*)&self->mPosition, 0, nullptr, 0, 4);
		((TBossMantaManager*)self->getManager())
		    ->spawn(self->unk18C + 1, self->mPosition);
	}

	if (spine->getTime() == 0x1E)
		return TRUE;
	return FALSE;
}
DEFINE_NERVE(TNerveMantaMove, TLiveActor)
{
	// TODO: WIP - movement state (anim/sfx/forces); attractor core drafted.
	TBossManta* self = (TBossManta*)spine->getBody();
	if (spine->getTime() % 100 == 0)
		self->updateAttractor();
	return FALSE;
}
DEFINE_NERVE(TNerveMantaAppearDemo, TLiveActor)
{
	// TODO: WIP - appear-demo state; transition-on-anim-end drafted.
	TBossManta* self = (TBossManta*)spine->getBody();
	if (self->checkCurAnmEnd(0))
		return TRUE;
	return FALSE;
}
DEFINE_NERVE(TNerveMantaDeath, TLiveActor)
{
	TBossManta* self = (TBossManta*)spine->getBody();

	if (spine->getTime() == 0) {
		self->getMActor()->setBckFromIndex(0);
		self->getMActor()->setFrameRate(SMSGetAnmFrameRate(), 0);
		MActorAnmBck* bck = self->getMActor()->getAnmBck();
		if (bck != nullptr)
			bck->setMotionBlendRatio(0.0f);
	}

	if (self->checkCurAnmEnd(0)) {
		gpMarioParticleManager->emitAndBindToPosPtr(0xF8, &self->unk17C, 0,
		                                            self);
		if (gpMSound->gateCheck(MSD_SE_BS_MANTA_VANISH))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_BS_MANTA_VANISH, (const Vec*)&self->mPosition, 0,
			    nullptr, 0, 4);
		self->kill();
	}

	return FALSE;
}
DEFINE_NERVE(TNerveMantaHitWater, TLiveActor)
{
	// TODO: WIP - hit-water state (anim/sound + per-gen data).
	TBossManta* self = (TBossManta*)spine->getBody();
	if (spine->getTime() == 0)
		self->getMActor()->setBckFromIndex(1);
	return FALSE;
}
