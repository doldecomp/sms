#include <Strategic/Strategy.hpp>
#include <Strategic/ObjHitCheck.hpp>
#include <macros.h>

TStrategy* gpStrategy;

void TIdxGroupObj::loadSuper(JSUMemoryInputStream& stream)
{
	JDrama::TViewObjPtrListT<THitActor>::loadSuper(stream);
	stream.read(&unk20, 4);
}

TStrategy::TStrategy(const char* name)
    : JDrama::TViewObj(name)
    , unk50(0)
{
	for (int i = 0; i < ARRAY_COUNT(unk10); ++i)
		unk10[i] = nullptr;
}

void TStrategy::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	new TObjHitCheck();

	int count = stream.readU32();
	for (int i = 0; i < count; ++i) {

		JSUMemoryInputStream stream2(nullptr, 0);
		TIdxGroupObj* ref
		    = (TIdxGroupObj*)JDrama::TNameRef::genObject(stream, stream2);
		if (ref) {
			unk10[15] = ref;
			ref->load(stream2);
			unk10[15] = nullptr;

			unk10[ref->unk20] = ref;
		}
	}

	gpStrategy = this;
}

void TStrategy::loadAfter()
{
	JDrama::TViewObj::loadAfter();
	for (int i = 0; i < 16; ++i)
		if (unk10[i])
			unk10[i]->loadAfter();
}

JDrama::TNameRef* TStrategy::searchF(u16 key, const char* name)
{
	JDrama::TNameRef* ref = JDrama::TViewObj::searchF(key, name);
	if (ref)
		return ref;

	for (int i = 0; i < ARRAY_COUNT(unk10); ++i) {
		if (unk10[i]) {
			JDrama::TNameRef* r = unk10[i]->searchF(key, name);
			if (r)
				return r;
		}
	}

	return nullptr;
}

void TStrategy::perform(u32 param_1, JDrama::TGraphics* param_2)
{

	if ((param_1 & 3) != 0) {
		if (unk10[0] != (TIdxGroupObj*)0x0)
			unk10[0]->testPerform(param_1, param_2);

		if (unk10[3] != (TIdxGroupObj*)0x0)
			unk10[3]->testPerform(param_1, param_2);

		if (unk10[4] != (TIdxGroupObj*)0x0)
			unk10[4]->testPerform(param_1, param_2);

		if (unk10[0xb] != (TIdxGroupObj*)0x0)
			unk10[0xb]->testPerform(param_1, param_2);

		if (unk10[6] != (TIdxGroupObj*)0x0)
			unk10[6]->testPerform(param_1, param_2);

		if (unk10[9] != (TIdxGroupObj*)0x0)
			unk10[9]->testPerform(param_1, param_2);
	}

	if ((param_1 & 8) != 0) {
		if (unk10[0] != (TIdxGroupObj*)0x0)
			unk10[0]->testPerform(param_1, param_2);

		if (unk10[3] != (TIdxGroupObj*)0x0)
			unk10[3]->testPerform(param_1, param_2);

		if (unk10[4] != (TIdxGroupObj*)0x0)
			unk10[4]->testPerform(param_1, param_2);

		if (unk10[5] != (TIdxGroupObj*)0x0)
			unk10[5]->testPerform(param_1, param_2);

		if (unk10[0xb] != (TIdxGroupObj*)0x0)
			unk10[0xb]->testPerform(param_1, param_2);

		if (unk10[6] != (TIdxGroupObj*)0x0)
			unk10[6]->testPerform(param_1, param_2);

		if (unk10[9] != (TIdxGroupObj*)0x0)
			unk10[9]->testPerform(param_1, param_2);
	}
}
