#include <Animal/AnimalManager.hpp>
#include <Animal/AnimalSave.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <MSound/SoundEffects.hpp>
#include <MSound/MSoundSE.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <Camera/Camera.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <Strategic/LiveActor.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: promote into EnemyManager.hpp. Retail's TEnemyManager::getObj(int)
// casts TObjManager::getObj()'s result directly; our version routes through
// TLiveManager::getObj(), and that extra inline level costs one 4-byte
// temporary, which shifts every local of clipEnemies up by 4. Parked as a
// TU-local helper because the shared header is off limits in this batch.
static inline TSpineEnemy* AnimalManagerGetObj(TAnimalManagerBase* manager,
                                               int i)
{
	return (TSpineEnemy*)manager->TObjManager::getObj(i);
}

TAnimalManagerBase::TAnimalManagerBase(const char* name)
    : TEnemyManager(name)
{
	mViewClipNear   = 250.0f;
	mViewClipFarPtr = nullptr;
	mAnimalSave     = nullptr;
}

void TAnimalManagerBase::loadSaveParams_(const char* path)
{
	mAnimalSave     = new TAnimalSaveIndividual(path);
	mViewClipNear   = mAnimalSave->mSLViewClipNear.get();
	mViewClipFarPtr = &mAnimalSave->mSLViewClipFar.get();
	unk3C           = mAnimalSave->mSLViewClipRadius.get();
}

void TAnimalManagerBase::clipEnemies(JDrama::TGraphics* graphics)
{
	f32 nearClip = mViewClipNear;
	SetViewFrustumClipCheckPerspective(gpCamera->getFovy(), gpCamera->getAspect(),
	                                   nearClip, *mViewClipFarPtr);

	int count = getObjNum();
	for (int i = 0; i < count; ++i) {
		TSpineEnemy* actor        = AnimalManagerGetObj(this, i);
		JGeometry::TVec3<f32> pos = actor->mPosition;
		pos.y += 75.0f;

		if (actor->checkLiveFlag(LIVE_FLAG_UNK2000)
		    && SMS_IsInOtherFastCube(pos)) {
			actor->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		} else {
			if (ViewFrustumClipCheck(graphics, &actor->mPosition, unk3C))
				actor->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
			else
				actor->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		}
	}
}

void TMewManager::load(JSUMemoryInputStream& stream)
{
	TAnimalManagerBase::load(stream);
	loadSaveParams_("/Animal/mew.prm");
}

// TODO: frame 0x18 vs retail 0x28; all 15 instructions match. One dead 12-byte
// non-trivial local (a TVec3<f32>) in an inlined callee is the unique fit across
// this whole family: it predicts 0x28 here, 0x28 in TAnimalBase::loadAfter (one
// guarded registerTrans), 0x30 in TAnimalBird::loadAfter (two registerTrans) and
// 0x38 in TAnimalBirdManager::loadAfter (two createRandPlayVec), each at the
// right instruction count; 8 and 16 bytes each miss two of the four. See
// docs/catalog/frame-gaps.md, "The dead low region", for the probe table.
// What that callee is stays unknown: MSound::setPlayerInfo matches with six
// MSRandPlay calls, so there is no wrapper around those, and TMewManager shares
// no base with TAnimalBird below TNameRef. Ruled out (all zero): mObjNum vs
// getObjNum()/getCapacity()/a named u16/an explicit (u16) cast, inline forwarder
// levels above TNameRef::loadAfter, 1-/2-/3-parameter inline wrappers with no
// local, and argument-count effects on the outgoing parameter area.
// Batch 74 narrowed the carrier: registerTrans and createRandPlayVec are both
// emitted, matching out-of-line statics reached by a real bl, so neither can
// hold the local itself -- it has to sit in a wrapper that inlines everywhere
// and leaves no map symbol, and the Animal TUs' weak lists name no candidate.
// See docs/catalog/frame-gaps.md, "Problem B addendum", for the new lead: a
// named by-value 12-byte result reserves a slot just like a dead local does.
// Closure batch 103 *confirmed* the carrier mechanically: wrapping the call in
// a TU-local `static inline` that holds an uninitialised
// `JGeometry::TVec3<f32>` takes this function to 100.0% at the same 15
// instructions, so the residue is exactly one 12-byte non-trivial local of an
// inlined wrapper. It is not committed because the wrapper has to be shared
// (Bird.cpp's two loadAfters need the same level) and nothing in the map names
// it -- MSound.hpp, which already carries the symbol-less `startSoundActor`
// inline, is the natural home. Also re-measured as zero here: a named `u16` for
// getObjNum(), a named `u32` for the sound id, and a dead TVec3 in this
// function's *own* body (+8, giving 0x20, not 0x28 -- an own-body 12-byte local
// is worth 8 while an inlined callee's is worth 16).
void TMewManager::loadAfter()
{
	TAnimalManagerBase::loadAfter();
	MSoundSESystem::MSRandPlay::createRandPlayVec(MSD_SE_OBJ_KAMOME_SOLO,
	                                              getObjNum());
}

void TMewManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "kamome_high.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (1 << J3DMLF_TevStageNumShift),
		  1 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}
