#include <Strategic/Strategy.hpp>
#include <Strategic/ObjHitCheck.hpp>
#include <macros.h>

TStrategy* gpStrategy;

void TIdxGroupObj::loadSuper(JSUMemoryInputStream& stream)
{
	JDrama::TViewObjPtrListT<THitActor>::loadSuper(stream);
	stream >> unk20;
}

TStrategy::TStrategy(const char* name)
    : JDrama::TViewObj(name)
    , mHitCheckOffFlags(0)
{
	for (s32 i = 0; i < ARRAY_COUNT(mGroups); ++i)
		mGroups[i] = nullptr;
}

void TStrategy::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);

	TObjHitCheck* hitCheck = new TObjHitCheck();

	int count = stream.readU32();
	for (int i = 0; i < count; ++i) {

		JSUMemoryInputStream stream2(nullptr, 0);
		TIdxGroupObj* ref
		    = (TIdxGroupObj*)JDrama::TNameRef::genObject(stream, stream2);
		if (ref) {
			mGroups[IDX_GROUP_LOADING] = ref;
			ref->load(stream2);
			mGroups[IDX_GROUP_LOADING] = nullptr;

			mGroups[ref->unk20] = ref;
		}
	}

	gpStrategy = this;
}

void TStrategy::loadAfter()
{
	JDrama::TViewObj::loadAfter();
	for (int i = 0; i < 16; ++i)
		if (mGroups[i])
			mGroups[i]->loadAfter();
}

JDrama::TNameRef* TStrategy::searchF(u16 key, const char* name)
{
	JDrama::TNameRef* ref = JDrama::TViewObj::searchF(key, name);
	if (ref)
		return ref;

	for (int i = 0; i < ARRAY_COUNT(mGroups); ++i) {
		if (mGroups[i]) {
			JDrama::TNameRef* r = mGroups[i]->searchF(key, name);
			if (r)
				return r;
		}
	}

	return nullptr;
}

void TStrategy::perform(u32 cue, JDrama::TGraphics* graphics)
{

	if ((cue & (CUE_MOVE | CUE_CALC_ANIM)) != 0) {
		if (mGroups[IDX_GROUP_MAP] != nullptr)
			mGroups[IDX_GROUP_MAP]->testPerform(cue, graphics);

		if (mGroups[IDX_GROUP_OBJECT] != nullptr)
			mGroups[IDX_GROUP_OBJECT]->testPerform(cue, graphics);

		if (mGroups[IDX_GROUP_GRAFFITI] != nullptr)
			mGroups[IDX_GROUP_GRAFFITI]->testPerform(cue, graphics);

		if (mGroups[IDX_GROUP_INIT] != nullptr)
			mGroups[IDX_GROUP_INIT]->testPerform(cue, graphics);

		if (mGroups[IDX_GROUP_PLAYER] != nullptr)
			mGroups[IDX_GROUP_PLAYER]->testPerform(cue, graphics);

		if (mGroups[IDX_GROUP_NPC] != nullptr)
			mGroups[IDX_GROUP_NPC]->testPerform(cue, graphics);
	}

	if ((cue & CUE_DRAW) != 0) {
		if (mGroups[IDX_GROUP_MAP] != nullptr)
			mGroups[IDX_GROUP_MAP]->testPerform(cue, graphics);

		if (mGroups[IDX_GROUP_OBJECT] != nullptr)
			mGroups[IDX_GROUP_OBJECT]->testPerform(cue, graphics);

		if (mGroups[IDX_GROUP_GRAFFITI] != nullptr)
			mGroups[IDX_GROUP_GRAFFITI]->testPerform(cue, graphics);

		if (mGroups[IDX_GROUP_ITEM] != nullptr)
			mGroups[IDX_GROUP_ITEM]->testPerform(cue, graphics);

		if (mGroups[IDX_GROUP_INIT] != nullptr)
			mGroups[IDX_GROUP_INIT]->testPerform(cue, graphics);

		if (mGroups[IDX_GROUP_PLAYER] != nullptr)
			mGroups[IDX_GROUP_PLAYER]->testPerform(cue, graphics);

		if (mGroups[IDX_GROUP_NPC] != nullptr)
			mGroups[IDX_GROUP_NPC]->testPerform(cue, graphics);
	}
}
